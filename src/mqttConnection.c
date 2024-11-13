/*
 * Copyright (c) 2017 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "mqttConnection.h"

#include "zephyr/drivers/gpio.h"

#include "topics.h"

bool command = false;

LOG_MODULE_REGISTER(net_mqtt_publisher_sample, LOG_LEVEL_DBG);

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

/* Buffers for MQTT client. */
static APP_BMEM uint8_t rx_buffer[APP_MQTT_BUFFER_SIZE];
static APP_BMEM uint8_t tx_buffer[APP_MQTT_BUFFER_SIZE];

// static int publisher(const char *message, const char *topic);
static int publisher(const char *message, const char *topic, char *ipAddress);

char serverIpAddress[17] = {0};
#if defined(CONFIG_MQTT_LIB_WEBSOCKET)
/* Making RX buffer large enough that the full IPv6 packet can fit into it */
#define MQTT_LIB_WEBSOCKET_RECV_BUF_LEN 1280

/* Websocket needs temporary buffer to store partial packets */
static APP_BMEM uint8_t temp_ws_rx_buf[MQTT_LIB_WEBSOCKET_RECV_BUF_LEN];
#endif

/* The mqtt client struct */
static APP_BMEM struct mqtt_client client_ctx;

/* MQTT Broker details. */
static APP_BMEM struct sockaddr_storage broker;

#if defined(CONFIG_SOCKS)
static APP_BMEM struct sockaddr socks5_proxy;
#endif

static APP_BMEM struct zsock_pollfd fds[1];
static APP_BMEM int nfds;

static APP_BMEM bool connected;

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

static int subscribe(struct mqtt_client *const c)
{

	struct mqtt_topic mqttLists[100] = {0};
	mqttLists[0] = servo_topic;
	mqttLists[1] = k3_topic;
	mqttLists[2] = lcd_topic;
	mqttLists[3] = builtInLed_topic;
	mqttLists[4] = puzzleType_topic;
	mqttLists[5] = led1_topic;
	mqttLists[6] = led2_topic;
	mqttLists[7] = led3_topic;
	mqttLists[8] = led4_topic;
	mqttLists[9] = led5_topic;
	mqttLists[10] = led6_topic;
	mqttLists[11] = led7_topic;
	mqttLists[12] = led8_topic;
	mqttLists[13] = stepperPosition_topic;
	mqttLists[14] = stepperSpeed_topic;

	const struct mqtt_subscription_list subscription_list = {
		.list = mqttLists, .list_count = 15, .message_id = 34};
	for(uint8_t i = 0; i < subscription_list.list_count; i++)
	{
	LOG_INF("Subscribing to: %s len %u", subscription_list.list[i].topic.utf8,
		(unsigned int)strlen(subscription_list.list[i].topic.utf8));

	}

	return mqtt_subscribe(c, &subscription_list);
}
static void prepare_fds(struct mqtt_client *client)
{
	if (client->transport.type == MQTT_TRANSPORT_NON_SECURE) {
		fds[0].fd = client->transport.tcp.sock;
	}
#if defined(CONFIG_MQTT_LIB_TLS)
	else if (client->transport.type == MQTT_TRANSPORT_SECURE) {
		fds[0].fd = client->transport.tls.sock;
	}
#endif

	fds[0].events = ZSOCK_POLLIN;
	nfds = 1;
}

static void clear_fds(void)
{
	nfds = 0;
}

static int wait(int timeout)
{
	int ret = 0;

	if (nfds > 0) {
		ret = zsock_poll(fds, nfds, timeout);
		if (ret < 0) {
			LOG_ERR("poll error: %d", errno);
		}
	}

	return ret;
}

void mqtt_evt_handler(struct mqtt_client *client, const struct mqtt_evt *evt)
{
	int err;
	char *payload = (char *)k_malloc(sizeof(char) * MESSAGE_QUEUE_LEN);
	switch (evt->type) {
	case MQTT_EVT_CONNACK:
		if (evt->result != 0) {
			LOG_ERR("MQTT connect failed %d", evt->result);
			break;
		}

		connected = true;
		LOG_INF("MQTT client connected!");
		subscribe(client);
		break;

	case MQTT_EVT_DISCONNECT:
		LOG_INF("MQTT client disconnected %d", evt->result);

		connected = false;
		clear_fds();

		break;

	case MQTT_EVT_PUBACK:
		if (evt->result != 0) {
			LOG_ERR("MQTT PUBACK error %d", evt->result);
			break;
		}

		LOG_INF("PUBACK packet id: %u", evt->param.puback.message_id);

		break;

	case MQTT_EVT_SUBACK:
		if (evt->result != 0) {
			LOG_ERR("MQTT SUBACK error: %d", evt->result);
			break;
		}

		LOG_INF("SUBACK packet id: %u", evt->param.suback.message_id);
		break;

	case MQTT_EVT_PUBREC:
		if (evt->result != 0) {
			LOG_ERR("MQTT PUBREC error %d", evt->result);
			break;
		}

		LOG_INF("PUBREC packet id: %u", evt->param.pubrec.message_id);

		const struct mqtt_pubrel_param rel_param = {.message_id =
								    evt->param.pubrec.message_id};

		err = mqtt_publish_qos2_release(client, &rel_param);
		if (err != 0) {
			LOG_ERR("Failed to send MQTT PUBREL: %d", err);
		}

		break;

	case MQTT_EVT_PUBCOMP:
		if (evt->result != 0) {
			LOG_ERR("MQTT PUBCOMP error %d", evt->result);
			break;
		}

		LOG_INF("PUBCOMP packet id: %u", evt->param.pubcomp.message_id);

		break;

	case MQTT_EVT_PINGRESP:
		LOG_INF("PINGRESP packet");
		break;

	case MQTT_EVT_PUBLISH:
		memset(payload, 0, MESSAGE_QUEUE_LEN);
		mqtt_read_publish_payload(client, payload, MESSAGE_QUEUE_LEN);
		// err = k_msgq_put(&msqSendToMQTT, payload, K_NO_WAIT);
		// char *topicName = (char *)k_malloc(sizeof(char) * MESSAGE_QUEUE_LEN);
		// memset(topicName, 0, MESSAGE_QUEUE_LEN);
		struct MqttMsg msg;
		memset(&msg, 0, sizeof(struct MqttMsg));
		strncpy(msg.topic, evt->param.publish.message.topic.topic.utf8, evt->param.publish.message.topic.topic.size);
		strcpy(msg.msg, payload);
		err = k_msgq_put(&msqReceivedFromMQTT, &msg, K_NO_WAIT);

		break;
	default:
		break;
	}
}

// static char *get_mqtt_payload(enum mqtt_qos qos)
// {
// #if APP_BLUEMIX_TOPIC
// 	static APP_BMEM char payload[30];

// 	snprintk(payload, sizeof(payload), "{d:{temperature:%d}}", (uint8_t)sys_rand32_get());
// #else
// 	static int counter = 0;
// 	char buff[10];
// 	memset(buff, 0, 10);
// 	static APP_DMEM char payload[40];
// 	memset(payload, 0, 40);
// 	sprintf(payload, "Hi Bram: %d", counter);
// 	counter++;
// #endif

// 	return payload;
// }

// static char *get_mqtt_topic(void)
// {
// #if APP_BLUEMIX_TOPIC
// 	return "iot-2/type/" BLUEMIX_DEVTYPE "/id/" BLUEMIX_DEVID "/evt/" BLUEMIX_EVENT
// 	       "/fmt/" BLUEMIX_FORMAT;
// #else
// 	return "pub/escapeRoom";
// #endif
// }

static int publish(struct mqtt_client *client, enum mqtt_qos qos, const char *message, const char *topic)
{
	struct mqtt_publish_param param;

	param.message.topic.qos = qos;
	param.message.topic.topic.utf8 = (uint8_t *)topic;
	param.message.topic.topic.size = strlen(param.message.topic.topic.utf8);
	param.message.payload.data = (uint8_t *)message;
	// param.message.payload.data = get_mqtt_payload(qos);
	param.message.payload.len = strlen(param.message.payload.data);
	param.message_id = sys_rand32_get();
	param.dup_flag = 0U;
	param.retain_flag = 0U;

	return mqtt_publish(client, &param);
}

#define RC_STR(rc) ((rc) == 0 ? "OK" : "ERROR")

#define PRINT_RESULT(func, rc) LOG_INF("%s: %d <%s>", (func), rc, RC_STR(rc))

// static void broker_init()
static void broker_init(char *serveripaddress)
{
#if defined(CONFIG_NET_IPV6)
	struct sockaddr_in6 *broker6 = (struct sockaddr_in6 *)&broker;

	broker6->sin6_family = AF_INET6;
	broker6->sin6_port = htons(SERVER_PORT);
	zsock_inet_pton(AF_INET6, SERVER_ADDR, &broker6->sin6_addr);

#if defined(CONFIG_SOCKS)
	struct sockaddr_in6 *proxy6 = (struct sockaddr_in6 *)&socks5_proxy;

	proxy6->sin6_family = AF_INET6;
	proxy6->sin6_port = htons(SOCKS5_PROXY_PORT);
	zsock_inet_pton(AF_INET6, SOCKS5_PROXY_ADDR, &proxy6->sin6_addr);
#endif
#else
	struct sockaddr_in *broker4 = (struct sockaddr_in *)&broker;

	broker4->sin_family = AF_INET;
	broker4->sin_port = htons(SERVER_PORT);
	// LOG_DBG("This is the length of the SERVER IP addrese: %d", strlen(SERVER_IP_ADDRESS));
	// LOG_DBG("This is the length of the serverIp address addrese: %d", strlen(serverIpAddress));
	zsock_inet_pton(AF_INET, serverIpAddress, &broker4->sin_addr);
	// zsock_inet_pton(AF_INET, SERVER_IP_ADDRESS, &broker4->sin_addr);
#if defined(CONFIG_SOCKS)
	struct sockaddr_in *proxy4 = (struct sockaddr_in *)&socks5_proxy;

	proxy4->sin_family = AF_INET;
	proxy4->sin_port = htons(SOCKS5_PROXY_PORT);
	zsock_inet_pton(AF_INET, SOCKS5_PROXY_ADDR, &proxy4->sin_addr);
#endif
#endif
}

// static void client_init(struct mqtt_client *client)
static void client_init(struct mqtt_client *client, char *serverIpAddress)
{
	mqtt_client_init(client);

	// broker_init();
	broker_init(serverIpAddress);

	/* MQTT client configuration */
	client->broker = &broker;
	client->evt_cb = mqtt_evt_handler;
	client->client_id.utf8 = (uint8_t *)MQTT_CLIENTID;
	client->client_id.size = strlen(MQTT_CLIENTID);
	client->password = NULL;
	client->user_name = NULL;
	client->protocol_version = MQTT_VERSION_3_1_1;

	/* MQTT buffers configuration */
	client->rx_buf = rx_buffer;
	client->rx_buf_size = sizeof(rx_buffer);
	client->tx_buf = tx_buffer;
	client->tx_buf_size = sizeof(tx_buffer);

	/* MQTT transport configuration */
#if defined(CONFIG_MQTT_LIB_TLS)
#if defined(CONFIG_MQTT_LIB_WEBSOCKET)
	client->transport.type = MQTT_TRANSPORT_SECURE_WEBSOCKET;
#else
	client->transport.type = MQTT_TRANSPORT_SECURE;
#endif

	struct mqtt_sec_config *tls_config = &client->transport.tls.config;

	tls_config->peer_verify = TLS_PEER_VERIFY_REQUIRED;
	tls_config->cipher_list = NULL;
	tls_config->sec_tag_list = m_sec_tags;
	tls_config->sec_tag_count = ARRAY_SIZE(m_sec_tags);
#if defined(MBEDTLS_X509_CRT_PARSE_C) || defined(CONFIG_NET_SOCKETS_OFFLOAD)
	tls_config->hostname = TLS_SNI_HOSTNAME;
#else
	tls_config->hostname = NULL;
#endif

#else
#if defined(CONFIG_MQTT_LIB_WEBSOCKET)
	client->transport.type = MQTT_TRANSPORT_NON_SECURE_WEBSOCKET;
#else
	client->transport.type = MQTT_TRANSPORT_NON_SECURE;
#endif
#endif

#if defined(CONFIG_MQTT_LIB_WEBSOCKET)
	client->transport.websocket.config.host = SERVER_ADDR;
	client->transport.websocket.config.url = "/mqtt";
	client->transport.websocket.config.tmp_buf = temp_ws_rx_buf;
	client->transport.websocket.config.tmp_buf_len = sizeof(temp_ws_rx_buf);
	client->transport.websocket.timeout = 5 * MSEC_PER_SEC;
#endif

#if defined(CONFIG_SOCKS)
	mqtt_client_set_proxy(client, &socks5_proxy,
			      socks5_proxy.sa_family == AF_INET ? sizeof(struct sockaddr_in)
								: sizeof(struct sockaddr_in6));
#endif
}

/* In this routine we block until the connected variable is 1 */
static int try_to_connect(struct mqtt_client *client, char *serverIpAddress)
// static int try_to_connect(struct mqtt_client *client)
{
	int rc, i = 0;

	while (i++ < APP_CONNECT_TRIES && !connected) {

		client_init(client, serverIpAddress);
		// client_init(client);

		rc = mqtt_connect(client);
		if (rc != 0) {
			PRINT_RESULT("mqtt_connect", rc);
			k_sleep(K_MSEC(APP_SLEEP_MSECS));
			continue;
		}

		prepare_fds(client);

		if (wait(APP_CONNECT_TIMEOUT_MS)) {
			mqtt_input(client);
		}

		if (!connected) {
			mqtt_abort(client);
		}
	}

	if (connected) {
		return 0;
	}

	return -EINVAL;
}

static int process_mqtt_and_sleep(struct mqtt_client *client, int timeout)
{
	int64_t remaining = timeout;
	int64_t start_time = k_uptime_get();
	int rc;

	while (remaining > 0 && connected) {
		if (wait(remaining)) {
			rc = mqtt_input(client);
			if (rc != 0) {
				PRINT_RESULT("mqtt_input", rc);
				return rc;
			}
		}

		rc = mqtt_live(client);
		if (rc != 0 && rc != -EAGAIN) {
			PRINT_RESULT("mqtt_live", rc);
			return rc;
		} else if (rc == 0) {
			rc = mqtt_input(client);
			if (rc != 0) {
				PRINT_RESULT("mqtt_input", rc);
				return rc;
			}
		}

		remaining = timeout + start_time - k_uptime_get();
	}

	return 0;
}

#define SUCCESS_OR_EXIT(rc)                                                                        \
	{                                                                                          \
		if (rc != 0) {                                                                     \
			return;                                                                  \
		}                                                                                  \
	}
#define SUCCESS_OR_BREAK(rc)                                                                       \
	{                                                                                          \
		if (rc != 0) {                                                                     \
			break;                                                                     \
		}                                                                                  \
	}

// static int publisher(const char *message, const char *topic)
static int publisher(const char *message, const char *topic, char *serverIpAddress)
{
	int rc = 0;

	LOG_INF("attempting to connect: ");
	// rc = try_to_connect(&client_ctx);
	rc = try_to_connect(&client_ctx, serverIpAddress);
	PRINT_RESULT("try_to_connect", rc);
	SUCCESS_OR_EXIT(rc);

	if (connected) {

		// rc = mqtt_ping(&client_ctx);
		// PRINT_RESULT("mqtt_ping", rc);
		// SUCCESS_OR_BREAK(rc);

		// rc = process_mqtt_and_sleep(&client_ctx, APP_SLEEP_MSECS);

		rc = publish(&client_ctx, MQTT_QOS_1_AT_LEAST_ONCE, message, topic);
		// rc = publish(&client_ctx, MQTT_QOS_2_EXACTLY_ONCE, message, topic);
		PRINT_RESULT("mqtt_publish", rc);

		rc = process_mqtt_and_sleep(&client_ctx, APP_SLEEP_MSECS);
	}
}
#if defined(CONFIG_USERSPACE)
#define STACK_SIZE 2048

#if defined(CONFIG_NET_TC_THREAD_COOPERATIVE)
#define THREAD_PRIORITY K_PRIO_COOP(CONFIG_NUM_COOP_PRIORITIES - 1)
#else
#define THREAD_PRIORITY K_PRIO_PREEMPT(8)
#endif

K_THREAD_DEFINE(app_thread, STACK_SIZE, start_app, NULL, NULL, NULL, THREAD_PRIORITY, K_USER, -1);

static K_HEAP_DEFINE(app_mem_pool, 1024 * 2);
#endif

void mqttEntryPoint(void * serverIpAddressIn, void *, void *)
{
#if defined(CONFIG_MQTT_LIB_TLS)
	int rc;

	rc = tls_init();
	PRINT_RESULT("tls_init", rc);
#endif
	strcat(serverIpAddress, (char *)serverIpAddressIn);
	struct MqttMsg msg = {0};
	// memset(&msg, 0, sizeof(struct MqttMsg));
	while (1) {
		while(k_msgq_get(&msqSendToMQTT, &msg, K_NO_WAIT) == 0) {
			// publisher(msg.msg, msg.topic);
			publisher(msg.msg, msg.topic, serverIpAddress);
			memset(&msg, 0, sizeof(struct MqttMsg));
		}
		int rc = 0;

		LOG_INF("attempting to connect: ");
		rc = try_to_connect(&client_ctx, serverIpAddress);
		// rc = try_to_connect(&client_ctx);
		PRINT_RESULT("try_to_connect", rc);
		SUCCESS_OR_EXIT(rc);
		if(connected)
		{

			rc = process_mqtt_and_sleep(&client_ctx, APP_SLEEP_MSECS);
		}
		k_msleep(1000);
	}
}

K_THREAD_STACK_DEFINE(mqttStackArea, MQTT_STACK_SIZE);

struct k_thread mqttThread;

void mqttThreadCreate(char *serverIpAddress)
{
	k_tid_t mqtt =
		k_thread_create(&mqttThread, mqttStackArea, K_THREAD_STACK_SIZEOF(mqttStackArea),
				mqttEntryPoint, (void *)serverIpAddress, NULL, NULL, MQTT_PRIORITY, 0, K_NO_WAIT);
	k_thread_name_set(mqtt, "mqtt");
}
