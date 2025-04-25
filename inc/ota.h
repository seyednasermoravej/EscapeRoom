#if(CONFIG_BOOTLOADER_MCUBOOT)

#ifndef __OTA__H__
#define __OTA__H__

#include <zephyr/logging/log.h>

#include <zephyr/net/net_ip.h>
#include <zephyr/net/socket.h>
#include <zephyr/net/tls_credentials.h>
#include <zephyr/net/http/client.h>
#include <zephyr/drivers/flash.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/dfu/flash_img.h>
#include <zephyr/dfu/mcuboot.h>


#include <zephyr/sys/reboot.h>

// #include "ca_certificate.h"

#define HTTP_PORT 8000
#define HTTPS_PORT 4443

#if defined(CONFIG_NET_CONFIG_PEER_IPV6_ADDR)
#define SERVER_ADDR6  CONFIG_NET_CONFIG_PEER_IPV6_ADDR
#else
#define SERVER_ADDR6 ""
#endif

#if defined(CONFIG_NET_CONFIG_PEER_IPV4_ADDR)
#define SERVER_ADDR4  CONFIG_NET_CONFIG_PEER_IPV4_ADDR
#else
#define SERVER_ADDR4 ""
#endif

#define MAX_RECV_BUF_LEN 1024
#define SLOT_SIZE FIXED_PARTITION_SIZE(slot1_partition)
enum otaResponse
{
    HTTP_OTA_OK,
    HTTP_OTA_ERROR,
};



class Ota
{
public:
    Ota(const char *_serverIp);
    int upgrade(const char *fileAddress);

private:
    int setup_socket(sa_family_t family, const char *server, int port,
			int *sock, struct sockaddr *addr, socklen_t addr_len);
    int connect_socket(sa_family_t family, const char *server, int port,
			  int *sock, struct sockaddr *addr, socklen_t addr_len);
    int run_queries(void);

    char serverIp[17];


};

#endif
#endif






