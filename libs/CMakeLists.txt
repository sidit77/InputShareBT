set(CMAKE_CXX_STANDARD 17)

set(BTSTACK_ROOT "btstack")
message("BTSTACK_ROOT: ${BTSTACK_ROOT}")

# BTstack sources
file(GLOB SOURCES_SRC       "${BTSTACK_ROOT}/src/*.c" "${BTSTACK_ROOT}/example/sco_demo_util.c")
file(GLOB SOURCES_BLE       "${BTSTACK_ROOT}/src/ble/*.c")
file(GLOB SOURCES_GATT      "${BTSTACK_ROOT}/src/ble/gatt-service/*.c")
file(GLOB SOURCES_CLASSIC   "${BTSTACK_ROOT}/src/classic/*.c")
file(GLOB SOURCES_MESH      "${BTSTACK_ROOT}/src/mesh/*.c")
file(GLOB SOURCES_BLUEDROID "${BTSTACK_ROOT}/3rd-party/bluedroid/encoder/srce/*.c" "${BTSTACK_ROOT}/3rd-party/bluedroid/decoder/srce/*.c")
file(GLOB SOURCES_MD5       "${BTSTACK_ROOT}/3rd-party/md5/md5.c")
file(GLOB SOURCES_UECC      "${BTSTACK_ROOT}/3rd-party/micro-ecc/uECC.c")
file(GLOB SOURCES_YXML      "${BTSTACK_ROOT}/3rd-party/yxml/yxml.c")
file(GLOB SOURCES_HXCMOD    "${BTSTACK_ROOT}/3rd-party/hxcmod-player/*.c"  "${BTSTACK_ROOT}/3rd-party/hxcmod-player/mods/*.c")
file(GLOB SOURCES_RIJNDAEL  "${BTSTACK_ROOT}/3rd-party/rijndael/rijndael.c")

set(SOURCES_POSIX
        ${BTSTACK_ROOT}/platform/posix/btstack_audio_portaudio.c
        ${BTSTACK_ROOT}/platform/posix/btstack_tlv_posix.c
        ${BTSTACK_ROOT}/platform/posix/wav_util.c
        )

set(SOURCES_WINDOWS
        ${BTSTACK_ROOT}/platform/windows/btstack_run_loop_windows.c
        ${BTSTACK_ROOT}/platform/windows/btstack_stdin_windows.c
        ${BTSTACK_ROOT}/platform/windows/btstack_uart_block_windows.c
        ${BTSTACK_ROOT}/platform/windows/hci_transport_h2_winusb.c)


set(LWIP_CORE_SRC
        ${BTSTACK_ROOT}/3rd-party/lwip/core/src/core/def.c
        ${BTSTACK_ROOT}/3rd-party/lwip/core/src/core/inet_chksum.c
        ${BTSTACK_ROOT}/3rd-party/lwip/core/src/core/init.c
        ${BTSTACK_ROOT}/3rd-party/lwip/core/src/core/ip.c
        ${BTSTACK_ROOT}/3rd-party/lwip/core/src/core/mem.c
        ${BTSTACK_ROOT}/3rd-party/lwip/core/src/core/memp.c
        ${BTSTACK_ROOT}/3rd-party/lwip/core/src/core/netif.c
        ${BTSTACK_ROOT}/3rd-party/lwip/core/src/core/pbuf.c
        ${BTSTACK_ROOT}/3rd-party/lwip/core/src/core/tcp.c
        ${BTSTACK_ROOT}/3rd-party/lwip/core/src/core/tcp_in.c
        ${BTSTACK_ROOT}/3rd-party/lwip/core/src/core/tcp_out.c
        ${BTSTACK_ROOT}/3rd-party/lwip/core/src/core/timeouts.c
        ${BTSTACK_ROOT}/3rd-party/lwip/core/src/core/udp.c
        )
set (LWIP_IPV4_SRC
        ${BTSTACK_ROOT}/3rd-party/lwip/core/src/core/ipv4/acd.c
        ${BTSTACK_ROOT}/3rd-party/lwip/core/src/core/ipv4/dhcp.c
        ${BTSTACK_ROOT}/3rd-party/lwip/core/src/core/ipv4/etharp.c
        ${BTSTACK_ROOT}/3rd-party/lwip/core/src/core/ipv4/icmp.c
        ${BTSTACK_ROOT}/3rd-party/lwip/core/src/core/ipv4/ip4.c
        ${BTSTACK_ROOT}/3rd-party/lwip/core/src/core/ipv4/ip4_addr.c
        ${BTSTACK_ROOT}/3rd-party/lwip/core/src/core/ipv4/ip4_frag.c
        )
set (LWIP_NETIF_SRC
        ${BTSTACK_ROOT}/3rd-party/lwip/core/src/netif/ethernet.c
        )
set (LWIP_HTTPD
        ${BTSTACK_ROOT}/3rd-party/lwip/core/src/apps/http/altcp_proxyconnect.c
        ${BTSTACK_ROOT}/3rd-party/lwip/core/src/apps/http/fs.c
        ${BTSTACK_ROOT}/3rd-party/lwip/core/src/apps/http/httpd.c
        )
set (LWIP_DHCPD
        ${BTSTACK_ROOT}/3rd-party/lwip/dhcp-server/dhserver.c
        )
set (LWIP_PORT
        ${BTSTACK_ROOT}/platform/lwip/port/sys_arch.c
        ${BTSTACK_ROOT}/platform/lwip/bnep_lwip.c
        )

set (SOURCES_LWIP ${LWIP_CORE_SRC} ${LWIP_IPV4_SRC} ${LWIP_NETIF_SRC} ${LWIP_HTTPD} ${LWIP_DHCPD} ${LWIP_PORT})

file(GLOB SOURCES_BLE_OFF   "${BTSTACK_ROOT}/src/ble/le_device_db_memory.c")
list(REMOVE_ITEM SOURCES_BLE ${SOURCES_BLE_OFF})


# select POSIX vs Windows versions
IF (WIN32)
    message("Building for Windows using WinUSB")
    set(SOURCES_HCI_USB ${BTSTACK_ROOT}/platform/windows/hci_transport_h2_winusb.c)
    set(SOURCES_STDIN   ${BTSTACK_ROOT}/platform/windows/btstack_stdin_windows.c)
    link_libraries( winusb setupapi)
ELSE()
    message("Building for POSIX using libusb")

    # fink pkgconfig
    find_package(PkgConfig REQUIRED)

    # libusb
    pkg_check_modules(LIBUSB REQUIRED libusb-1.0)
    include_directories(${LIBUSB_INCLUDE_DIRS})
    link_directories(${LIBUSB_LIBRARY_DIRS})
    link_libraries(${LIBUSB_LIBRARIES})

    set(SOURCES_HCI_USB ${BTSTACK_ROOT}/platform/libusb/hci_transport_h2_libusb.c)
    set(SOURCES_STDIN   ${BTSTACK_ROOT}/platform/posix/btstack_stdin_posix.c)

    list(APPEND SOURCES_POSIX ${BTSTACK_ROOT}/platform/posix/btstack_network_posix.c)
    list(APPEND SOURCES_POSIX ${BTSTACK_ROOT}/platform/posix/btstack_uart_block_posix.c)
ENDIF()

set(SOURCES
        ${SOURCES_MD5}
        ${SOURCES_YXML}
        ${SOURCES_BLUEDROID}
        ${SOURCES_POSIX}
        ${SOURCES_RIJNDAEL}
        ${SOURCES_HCI_USB}
        ${SOURCES_STDIN}
        ${SOURCES_SRC}
        ${SOURCES_BLE}
        ${SOURCES_GATT}
        ${SOURCES_MESH}
        ${SOURCES_CLASSIC}
        ${SOURCES_UECC}
        ${SOURCES_HXCMOD}
        ${SOURCES_WINDOWS})
list(SORT SOURCES)

# BTstack include
set(INCLUDES
    ${BTSTACK_ROOT}/3rd-party/micro-ecc
    ${BTSTACK_ROOT}/3rd-party/bluedroid/decoder/include
    ${BTSTACK_ROOT}/3rd-party/bluedroid/encoder/include
    ${BTSTACK_ROOT}/3rd-party/md5
    ${BTSTACK_ROOT}/3rd-party/hxcmod-player
    ${BTSTACK_ROOT}/3rd-party/hxcmod-player/mod
    ${BTSTACK_ROOT}/3rd-party/lwip/core/src/include
    ${BTSTACK_ROOT}/3rd-party/lwip/dhcp-server
    ${BTSTACK_ROOT}/3rd-party/rijndael
    ${BTSTACK_ROOT}/3rd-party/yxml
    ${BTSTACK_ROOT}/3rd-party/tinydir
    ${BTSTACK_ROOT}/src
    ${BTSTACK_ROOT}/platform/posix
    ${BTSTACK_ROOT}/platform/embedded
    ${BTSTACK_ROOT}/platform/lwip
    ${BTSTACK_ROOT}/platform/lwip/port
    ${BTSTACK_ROOT}/platform/qt
    ${BTSTACK_ROOT}/platform/windows
    config)

# create static lib
add_library(btstack-lib STATIC ${SOURCES})
target_include_directories(btstack-lib PUBLIC ${INCLUDES})