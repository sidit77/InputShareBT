#include "bluetooth.h"

#include <cstdio>
#include <cstring>
#include <cinttypes>
#include <btstack.h>
#include <btstack_tlv.h>
#include <btstack_tlv_posix.h>
#include <btstack_run_loop_qt.h>
#include "btstack_debug.h"
#include "btstack_event.h"
#include "btstack_memory.h"
#include "btstack_run_loop.h"
#include "ble/le_device_db_tlv.h"
#include "classic/btstack_link_key_db_tlv.h"
#include "hci.h"
#include "hci_dump.h"
#include "hid.h"

// to enable demo text on POSIX systems
// #undef HAVE_BTSTACK_STDIN

// from USB HID Specification 1.1, Appendix B.1
const uint8_t hid_descriptor_keyboard_boot_mode[] = {

        0x05, 0x01,                    // Usage Page (Generic Desktop)
        0x09, 0x06,                    // Usage (Keyboard)
        0xa1, 0x01,                    // Collection (Application)

        // Modifier byte

        0x75, 0x01,                    //   Report Size (1)
        0x95, 0x08,                    //   Report Count (8)
        0x05, 0x07,                    //   Usage Page (Key codes)
        0x19, 0xe0,                    //   Usage Minimum (Keyboard LeftControl)
        0x29, 0xe7,                    //   Usage Maxium (Keyboard Right GUI)
        0x15, 0x00,                    //   Logical Minimum (0)
        0x25, 0x01,                    //   Logical Maximum (1)
        0x81, 0x02,                    //   Input (Data, Variable, Absolute)

        // Reserved byte

        0x75, 0x01,                    //   Report Size (1)
        0x95, 0x08,                    //   Report Count (8)
        0x81, 0x03,                    //   Input (Constant, Variable, Absolute)

        // LED report + padding

        0x95, 0x05,                    //   Report Count (5)
        0x75, 0x01,                    //   Report Size (1)
        0x05, 0x08,                    //   Usage Page (LEDs)
        0x19, 0x01,                    //   Usage Minimum (Num Lock)
        0x29, 0x05,                    //   Usage Maxium (Kana)
        0x91, 0x02,                    //   Output (Data, Variable, Absolute)

        0x95, 0x01,                    //   Report Count (1)
        0x75, 0x03,                    //   Report Size (3)
        0x91, 0x03,                    //   Output (Constant, Variable, Absolute)

        // Keycodes

        0x95, 0x06,                    //   Report Count (6)
        0x75, 0x08,                    //   Report Size (8)
        0x15, 0x00,                    //   Logical Minimum (0)
        0x25, 0xff,                    //   Logical Maximum (1)
        0x05, 0x07,                    //   Usage Page (Key codes)
        0x19, 0x00,                    //   Usage Minimum (Reserved (no event indicated))
        0x29, 0xff,                    //   Usage Maxium (Reserved)
        0x81, 0x00,                    //   Input (Data, Array)

        0xc0,                          // End collection
};

// STATE

static uint8_t hid_service_buffer[250];
static uint8_t device_id_sdp_service_buffer[100];
static const char hid_device_name[] = "BTstack HID Keyboard";
static btstack_packet_callback_registration_t hci_event_callback_registration;
static uint16_t hid_cid;
static bd_addr_t device_addr;
static uint8_t hid_boot_device = 0;

#ifdef HAVE_BTSTACK_STDIN
static const char * device_addr_string = "BC:EC:5D:E6:15:03";
#endif

static enum {
    APP_BOOTING,
    APP_NOT_CONNECTED,
    APP_CONNECTING,
    APP_CONNECTED
} app_state = APP_BOOTING;


// HID Report sending
static std::optional<std::pair<HIDModifierKeys, HIDKeys>> send_keys;
//static HIDKeys send_keycodes;
//static HIDModifierKeys send_modifier;

static void send_key(HIDModifierKeys modifier, HIDKeys keycode){
    send_keys = std::make_pair(modifier, keycode);
    hid_device_request_can_send_now_event(hid_cid);
}

static void send_report(HIDModifierKeys modifier, HIDKeys keycode){
    std::array<uint8_t, 10> report = {
            0xa1,
            static_cast<uint8_t>(modifier), 0, 0,
            static_cast<uint8_t>(keycode[0]),
            static_cast<uint8_t>(keycode[0]),
            static_cast<uint8_t>(keycode[0]),
            static_cast<uint8_t>(keycode[0]),
            static_cast<uint8_t>(keycode[0]),
            static_cast<uint8_t>(keycode[0])};
    hid_device_send_interrupt_message(hid_cid, report.data(), report.size());
}

void bt_send_char(HIDModifierKeys mod,  HIDKeys key) {
    switch (app_state){
        case APP_BOOTING:
        case APP_CONNECTING:
            // ignore
            break;

        case APP_CONNECTED:
            // send keyu
            //found = keycode_and_modifer_us_for_character(character, &keycode, &modifier);
            send_key(mod, key);
            break;
        case APP_NOT_CONNECTED:
            printf("Connecting to %s...\n", bd_addr_to_str(device_addr));
            hid_device_connect(device_addr, &hid_cid);
            break;
        default:
            btstack_assert(false);
            break;
    }
}

#define TLV_DB_PATH_PREFIX "btstack_"
#define TLV_DB_PATH_POSTFIX ".tlv"
static char tlv_db_path[100];
static const btstack_tlv_t * tlv_impl;
static btstack_tlv_posix_t   tlv_context;
static bd_addr_t             local_addr;

static void packet_handler(uint8_t packet_type, uint16_t channel, uint8_t * packet, uint16_t packet_size){
    UNUSED(channel);
    UNUSED(packet_size);
    uint8_t status;
    switch (packet_type){
        case HCI_EVENT_PACKET:
            switch (hci_event_packet_get_type(packet)){
                case BTSTACK_EVENT_STATE:
                    if (btstack_event_state_get_state(packet) != HCI_STATE_WORKING)
                        return;
                    app_state = APP_NOT_CONNECTED;

                    gap_local_bd_addr(local_addr);
                    printf("BTstack up and running on %s.\n", bd_addr_to_str(local_addr));
                    strcpy(tlv_db_path, TLV_DB_PATH_PREFIX);
                    strcat(tlv_db_path, bd_addr_to_str(local_addr));
                    strcat(tlv_db_path, TLV_DB_PATH_POSTFIX);
                    tlv_impl = btstack_tlv_posix_init_instance(&tlv_context, tlv_db_path);
                    btstack_tlv_set_instance(tlv_impl, &tlv_context);
#ifdef ENABLE_CLASSIC
                    hci_set_link_key_db(btstack_link_key_db_tlv_get_instance(tlv_impl, &tlv_context));
#endif
#ifdef ENABLE_BLE
                    le_device_db_tlv_configure(tlv_impl, &tlv_context);
#endif

                    break;

                case HCI_EVENT_USER_CONFIRMATION_REQUEST:
                    // ssp: inform about user confirmation request
                    log_info("SSP User Confirmation Request with numeric value '%06" PRIu32"'\n", hci_event_user_confirmation_request_get_numeric_value(packet));
                    log_info("SSP User Confirmation Auto accept\n");
                    break;

                case HCI_EVENT_HID_META:
                    switch (hci_event_hid_meta_get_subevent_code(packet)){
                        case HID_SUBEVENT_CONNECTION_OPENED:
                            status = hid_subevent_connection_opened_get_status(packet);
                            if (status) {
                                // outgoing connection failed
                                printf("Connection failed, status 0x%x\n", status);
                                app_state = APP_NOT_CONNECTED;
                                hid_cid = 0;
                                return;
                            }
                            app_state = APP_CONNECTED;
                            hid_cid = hid_subevent_connection_opened_get_hid_cid(packet);
                            printf("HID Connected, please start typing...\n");

                            break;
                        case HID_SUBEVENT_CONNECTION_CLOSED:
                            printf("HID Disconnected\n");
                            app_state = APP_NOT_CONNECTED;
                            hid_cid = 0;
                            break;
                        case HID_SUBEVENT_CAN_SEND_NOW:
                            if (send_keys){
                                send_report(send_keys->first, send_keys->second);
                                //send_keys = std::nullopt;
                                hid_device_request_can_send_now_event(hid_cid);
                            } else {
                                send_report(HIDModifierKeys::None, {});
                            }
                            break;
                        default:
                            break;
                    }
                    break;
                default:
                    break;
            }
            break;
        default:
            break;
    }
}

void bt_init() {

    // Prevent stdout buffering
    setvbuf(stdout, NULL, _IONBF, 0);

    printf("BTstack/windows-winusb booting up\n");


    /// GET STARTED with BTstack ///
    btstack_memory_init();
    btstack_run_loop_init(btstack_run_loop_qt_get_instance());

    // if (usb_path_len){
    //     hci_transport_usb_set_path(usb_path_len, usb_path);
    // }

    // use logger: format HCI_DUMP_PACKETLOGGER, HCI_DUMP_BLUEZ or HCI_DUMP_STDOUT

    char pklg_path[100];
    strcpy(pklg_path, "hci_dump");

    strcat(pklg_path, ".pklg");
    printf("Packet Log: %s\n", pklg_path);
    hci_dump_open(pklg_path, HCI_DUMP_PACKETLOGGER);

    // init HCI
    hci_init(hci_transport_usb_instance(), NULL);

    // inform about BTstack state
    hci_event_callback_registration.callback = &packet_handler;
    hci_add_event_handler(&hci_event_callback_registration);


    /* @section Main Application Setup
 *
 * @text Listing MainConfiguration shows main application code.
 * To run a HID Device service you need to initialize the SDP, and to create and register HID Device record with it.
 * At the end the Bluetooth stack is started.
 */

/* LISTING_START(MainConfiguration): Setup HID Device */

    // setup app
    // allow to get found by inquiry
    gap_discoverable_control(1);
    // use Limited Discoverable Mode; Peripheral; Keyboard as CoD
    gap_set_class_of_device(0x2540);
    // set local name to be identified - zeroes will be replaced by actual BD ADDR
    gap_set_local_name("HID Keyboard Demo 00:00:00:00:00:00");
    // allow for role switch in general and sniff mode
    gap_set_default_link_policy_settings( LM_LINK_POLICY_ENABLE_ROLE_SWITCH | LM_LINK_POLICY_ENABLE_SNIFF_MODE );
    // allow for role switch on outgoing connections - this allow HID Host to become master when we re-connect to it
    gap_set_allow_role_switch(true);

    // L2CAP
    l2cap_init();

    // SDP Server
    sdp_init();
    memset(hid_service_buffer, 0, sizeof(hid_service_buffer));

    uint8_t hid_virtual_cable = 0;
    uint8_t hid_remote_wake = 1;
    uint8_t hid_reconnect_initiate = 1;
    uint8_t hid_normally_connectable = 1;

    // hid sevice subclass 2540 Keyboard, hid counntry code 33 US, hid virtual cable off, hid reconnect initiate off, hid boot device off
    hid_create_sdp_record(hid_service_buffer, 0x10001, 0x2540, 33,
                          hid_virtual_cable, hid_remote_wake, hid_reconnect_initiate, hid_normally_connectable,
                          hid_boot_device, hid_descriptor_keyboard_boot_mode, sizeof(hid_descriptor_keyboard_boot_mode), hid_device_name);
    printf("HID service record size: %u\n", de_get_len( hid_service_buffer));
    sdp_register_service(hid_service_buffer);

    // See https://www.bluetooth.com/specifications/assigned-numbers/company-identifiers if you don't have a USB Vendor ID and need a Bluetooth Vendor ID
    // device info: BlueKitchen GmbH, product 1, version 1
    device_id_create_sdp_record(device_id_sdp_service_buffer, 0x10003, DEVICE_ID_VENDOR_ID_SOURCE_BLUETOOTH, BLUETOOTH_COMPANY_ID_BLUEKITCHEN_GMBH, 1, 1);
    printf("Device ID SDP service record size: %u\n", de_get_len((uint8_t*)device_id_sdp_service_buffer));
    sdp_register_service(device_id_sdp_service_buffer);

    // HID Device
    hid_device_init(hid_boot_device, sizeof(hid_descriptor_keyboard_boot_mode), hid_descriptor_keyboard_boot_mode);

    // register for HCI events
    //hci_event_callback_registration.callback = &packet_handler;
    //hci_add_event_handler(&hci_event_callback_registration);

    // register for HID events
    hid_device_register_packet_handler(&packet_handler);

    sscanf_bd_addr(device_addr_string, device_addr);
    //btstack_stdin_setup(stdin_process);

    hci_power_control(HCI_POWER_ON);

    // go
    //btstack_run_loop_execute();

}

void bt_destroy(){
    printf("CTRL-C - SIGINT received, shutting down..\n");
    log_info("sigint_handler: shutting down");

    // reset anyway
    //btstack_stdin_reset();

    // power down
    hci_power_control(HCI_POWER_OFF);
    hci_close();
    log_info("Good bye, see you.\n");
}