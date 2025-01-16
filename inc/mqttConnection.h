#ifndef __MQTT__H__
#define __MQTT__H__

#include <zephyr/logging/log.h>

#include <zephyr/kernel.h>
#include <zephyr/net/socket.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/random/random.h>

#include <string.h>
#include <errno.h>

#include "config.h"

#include "mqttConnection.h"

#include "zephyr/drivers/gpio.h"

#include "topics.h"

#include "messageQueues.h"

#include "main.h"

#define APP_CONNECT_TIMEOUT_MS	3000
#define APP_SLEEP_MSECS		500

#define APP_CONNECT_TRIES	10

#define APP_MQTT_BUFFER_SIZE    512	

#define MQTT_USERNAME       "District21"
#define MQTT_PASSWORD       "District21!"

#define MQTT_STACK_SIZE     3072 
#define MQTT_PRIORITY      8 
// #define SERVER_IP_ADDRESS    "192.168.100.169"
#define SERVER_IP_ADDRESS    "5.196.78.28"
// #define SERVER_IP_ADDRESS    "91.121.93.94"

#if defined(CONFIG_USERSPACE)
#include <zephyr/app_memory/app_memdomain.h>
K_APPMEM_PARTITION_DEFINE(app_partition);
struct k_mem_domain app_domain;
#define APP_BMEM K_APP_BMEM(app_partition)
#define APP_DMEM K_APP_DMEM(app_partition)
#else
#define APP_BMEM
#define APP_DMEM
#endif


#define RC_STR(rc) ((rc) == 0 ? "OK" : "ERROR")

#define PRINT_RESULT(func, rc) LOG_DBG("%s: %d <%s>", (func), rc, RC_STR(rc))

#if defined(CONFIG_MQTT_LIB_WEBSOCKET)
/* Making RX buffer large enough that the full IPv6 packet can fit into it */
#define MQTT_LIB_WEBSOCKET_RECV_BUF_LEN 1280

/* Websocket needs temporary buffer to store partial packets */
static APP_BMEM uint8_t temp_ws_rx_buf[MQTT_LIB_WEBSOCKET_RECV_BUF_LEN];
#endif

#if defined(CONFIG_SOCKS)
static APP_BMEM struct sockaddr socks5_proxy;
#endif

#if defined(CONFIG_MQTT_LIB_TLS)

#include "test_certs.h"

#define TLS_SNI_HOSTNAME "localhost"
#define APP_CA_CERT_TAG  1
#define APP_PSK_TAG      2

static APP_DMEM sec_tag_t m_sec_tags[] = {
#if defined(MBEDTLS_X509_CRT_PARSE_C) || defined(CONFIG_NET_SOCKETS_OFFLOAD)
	APP_CA_CERT_TAG,
#endif
#if defined(MBEDTLS_KEY_EXCHANGE_SOME_PSK_ENABLED)
	APP_PSK_TAG,
#endif
};

static int tls_init(void)
{
	int err = -EINVAL;

#if defined(MBEDTLS_X509_CRT_PARSE_C) || defined(CONFIG_NET_SOCKETS_OFFLOAD)
	err = tls_credential_add(APP_CA_CERT_TAG, TLS_CREDENTIAL_CA_CERTIFICATE, ca_certificate,
				 sizeof(ca_certificate));
	if (err < 0) {
		LOG_ERR("Failed to register public certificate: %d", err);
		return err;
	}
#endif

#if defined(MBEDTLS_KEY_EXCHANGE_SOME_PSK_ENABLED)
	err = tls_credential_add(APP_PSK_TAG, TLS_CREDENTIAL_PSK, client_psk, sizeof(client_psk));
	if (err < 0) {
		LOG_ERR("Failed to register PSK: %d", err);
		return err;
	}

	err = tls_credential_add(APP_PSK_TAG, TLS_CREDENTIAL_PSK_ID, client_psk_id,
				 sizeof(client_psk_id) - 1);
	if (err < 0) {
		LOG_ERR("Failed to register PSK ID: %d", err);
	}
#endif

	return err;
}

#endif /* CONFIG_MQTT_LIB_TLS */
class Mqtt
{
public:
    Mqtt(const char *_serverIpAddress, struct mqtt_topic *_mqttList, uint16_t _mqttCount): serverIpAddress(_serverIpAddress), mqttList(_mqttList), mqttCount(_mqttCount){}

    void publisher(const char *message, const char *topic);
    int subscribe();
    int try_to_connect();
    APP_BMEM bool connected = false;
    int process_mqtt_and_sleep(int timeout);

    void mqtt_evt_handler(const struct mqtt_evt *evt);
    static void mqtt_evt_handlerWrapper(struct mqtt_client *clientData, const struct mqtt_evt *evt);

    APP_BMEM struct mqtt_client client_ctx;
    APP_BMEM struct mqtt_client *client = &client_ctx;
    APP_BMEM uint16_t mqttCount;
    struct mqtt_topic *mqttList;




private:
    const char *serverIpAddress;
    APP_BMEM uint8_t rx_buffer[APP_MQTT_BUFFER_SIZE];
    APP_BMEM uint8_t tx_buffer[APP_MQTT_BUFFER_SIZE];


    /* The mqtt client struct */

    /* MQTT Broker details. */
    APP_BMEM struct sockaddr_storage broker;

    APP_BMEM struct zsock_pollfd fds[1];
    APP_BMEM int nfds;

    // static APP_BMEM uint8_t rx_buffer[APP_MQTT_BUFFER_SIZE];
    // static APP_BMEM uint8_t tx_buffer[APP_MQTT_BUFFER_SIZE];

    // /* The mqtt client struct */
    // static APP_BMEM struct mqtt_client client_ctx;

    // /* MQTT Broker details. */
    // static APP_BMEM struct sockaddr_storage broker;

    // static APP_BMEM struct zsock_pollfd fds[1];
    // static APP_BMEM int nfds;

    void broker_init();
    int publish(enum mqtt_qos qos, const char *message, const char *topic);

    void client_init();
    void prepare_fds();
    void clear_fds();
    int wait(int timeout);
};

#endif