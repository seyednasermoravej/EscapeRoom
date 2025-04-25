#include "ota.h"
#if(CONFIG_BOOTLOADER_MCUBOOT)
#define CONFIG_SIMPLE_HTTP_OTA_DOWNLOAD_TIMEOUT	30

#define HTTP_TIMEOUT (1000 * CONFIG_SIMPLE_HTTP_OTA_DOWNLOAD_TIMEOUT * MSEC_PER_SEC)


LOG_MODULE_REGISTER(ota, LOG_LEVEL_DBG);



struct HttpOtaContext
{
    int sock;
    struct flash_img_context flash_ctx;
    uint8_t recv_buf[ MAX_RECV_BUF_LEN];
    enum otaResponse status;
    size_t content_length;
}ota_context;
static bool retry = true;

Ota:: Ota(const char *_serverIp)
{
    strcpy(serverIp, _serverIp);
    // run_queries();
	bool image_ok = false;
	image_ok = boot_is_img_confirmed();
	LOG_INF("Image is %s confirmed", image_ok? "": "not");
	if(!image_ok)
	{
		int ret = boot_write_img_confirmed();
		if(ret < 0)
		{
			LOG_ERR("Image couldn't confirmed: %d", ret);
		}
		LOG_INF("Erasing slot1");
		ret = boot_erase_img_bank(FIXED_PARTITION_ID(slot1_partition));
		if(ret)
		{
			LOG_ERR("Failed to erase second slot");
		}
	}

}
int Ota:: setup_socket(sa_family_t family, const char *server, int port,
			int *sock, struct sockaddr *addr, socklen_t addr_len)
{
	const char *family_str = family == AF_INET ? "IPv4" : "IPv6";
	int ret = 0;

	memset(addr, 0, addr_len);

	if (family == AF_INET) {
		net_sin(addr)->sin_family = AF_INET;
		net_sin(addr)->sin_port = htons(port);
		inet_pton(family, server, &net_sin(addr)->sin_addr);
	} else {
		net_sin6(addr)->sin6_family = AF_INET6;
		net_sin6(addr)->sin6_port = htons(port);
		inet_pton(family, server, &net_sin6(addr)->sin6_addr);
	}

	// if (IS_ENABLED(CONFIG_NET_SOCKETS_SOCKOPT_TLS)) {
	// 	sec_tag_t sec_tag_list[] = {
	// 		CA_CERTIFICATE_TAG,
	// 	};

	// 	*sock = socket(family, SOCK_STREAM, IPPROTO_TLS_1_2);
	// 	if (*sock >= 0) {
	// 		ret = setsockopt(*sock, SOL_TLS, TLS_SEC_TAG_LIST,
	// 				 sec_tag_list, sizeof(sec_tag_list));
	// 		if (ret < 0) {
	// 			LOG_ERR("Failed to set %s secure option (%d)",
	// 				family_str, -errno);
	// 			ret = -errno;
	// 		}

	// 		ret = setsockopt(*sock, SOL_TLS, TLS_HOSTNAME,
	// 				 TLS_PEER_HOSTNAME,
	// 				 sizeof(TLS_PEER_HOSTNAME));
	// 		if (ret < 0) {
	// 			LOG_ERR("Failed to set %s TLS_HOSTNAME "
	// 				"option (%d)", family_str, -errno);
	// 			ret = -errno;
	// 		}
	// 	}
	// } else {
		*sock = socket(family, SOCK_STREAM, IPPROTO_TCP);
	// }

	if (*sock < 0) {
		LOG_ERR("Failed to create %s HTTP socket (%d)", family_str,
			-errno);
	}

	return ret;
}

// static int payload_cb(int sock, struct http_request *req, void *user_data)
// {
// 	const char *content[] = {
// 		"foobar",
// 		"chunked",
// 		"last"
// 	};
// 	char tmp[64];
// 	int i, pos = 0;

// 	for (i = 0; i < ARRAY_SIZE(content); i++) {
// 		pos += snprintk(tmp + pos, sizeof(tmp) - pos,
// 				"%x\r\n%s\r\n",
// 				(unsigned int)strlen(content[i]),
// 				content[i]);
// 	}

// 	pos += snprintk(tmp + pos, sizeof(tmp) - pos, "0\r\n\r\n");

// 	(void)send(sock, tmp, pos, 0);

// 	return pos;
// }

static void response_cb(struct http_response *rsp,
			enum http_final_call final_data,
			void *user_data)
{
	static size_t body_len;
	uint8_t *body_data = NULL;
	int ret = 0;
	// LOG_DBG("Entered call back");
	if(rsp->http_status_code != 200)
	{
		ota_context.status = HTTP_OTA_ERROR;
		LOG_ERR("Could not download firmware, error: %d", rsp->http_status_code);
		retry = true;
		return;
	}
	retry = false;
	if(rsp->content_length > SLOT_SIZE)
	{
		ota_context.status = HTTP_OTA_ERROR;
		LOG_ERR("file too much size");
		return;
	}
	if(ota_context.content_length == 0)
	{
		// LOG_DBG("content length is zero");
		body_data = rsp->body_frag_start;
		body_len = rsp->data_len;
		body_len -= (rsp-> body_frag_start - rsp->recv_buf);
	}
	else
	{
		LOG_DBG("content length is  not zero");
		body_data = rsp->body_frag_start;
		body_len = rsp->data_len;
	}
	if((rsp->body_found == 1) && (body_data == NULL))
	{

		LOG_DBG("body is null");
		body_data = rsp->recv_buf;
		body_len = rsp->data_len;
	}
	if(body_data != NULL)
	{
		LOG_DBG("wrote to flash %d bytes", body_len);
		ret = flash_img_buffered_write(&ota_context.flash_ctx, body_data, body_len, false);
		if(ret < 0)
		{
			ota_context.status = HTTP_OTA_ERROR;
			LOG_ERR("flash write error");
			return;
		}
	}
	// if (final_data == HTTP_DATA_MORE) {
	// 	LOG_INF("Partial data received (%zd bytes)", rsp->data_len);
	// } else if (final_data == HTTP_DATA_FINAL) {
	// 	LOG_INF("All the data received (%zd bytes)", rsp->data_len);
	// }

	// LOG_INF("Response to %s", (const char *)user_data);
	// LOG_INF("Response status %s", rsp->http_status);
}

int Ota:: connect_socket(sa_family_t family, const char *server, int port,
			  int *sock, struct sockaddr *addr, socklen_t addr_len)
{
	int ret;

	ret = setup_socket(family, server, port, sock, addr, addr_len);
	if (ret < 0 || *sock < 0) {
		return -1;
	}

	ret = connect(*sock, addr, addr_len);
	if (ret < 0) {
		LOG_ERR("Cannot connect to %s remote (%d)",
			family == AF_INET ? "IPv4" : "IPv6",
			-errno);
		close(*sock);
		*sock = -1;
		ret = -errno;
	}

	return ret;
}

// int Ota:: run_queries(void)
// {
// 	struct sockaddr_in6 addr6;
// 	struct sockaddr_in addr4;
// 	int sock4 = -1, sock6 = -1;
// 	int32_t timeout = 3 * MSEC_PER_SEC;
// 	int ret = 0;
// 	int port = HTTP_PORT;

// 	// if (IS_ENABLED(CONFIG_NET_SOCKETS_SOCKOPT_TLS)) {
// 	// 	ret = tls_credential_add(CA_CERTIFICATE_TAG,
// 	// 				 TLS_CREDENTIAL_CA_CERTIFICATE,
// 	// 				 ca_certificate,
// 	// 				 sizeof(ca_certificate));
// 	// 	if (ret < 0) {
// 	// 		LOG_ERR("Failed to register public certificate: %d",
// 	// 			ret);
// 	// 		return ret;
// 	// 	}

// 	// 	port = HTTPS_PORT;
// 	// }

// 	if (IS_ENABLED(CONFIG_NET_IPV4)) {
// 		(void)connect_socket(AF_INET, serverIp, port,
// 				     &sock4, (struct sockaddr *)&addr4,
// 				     sizeof(addr4));
// 	}

// 	if (IS_ENABLED(CONFIG_NET_IPV6)) {
// 		(void)connect_socket(AF_INET6, SERVER_ADDR6, port,
// 				     &sock6, (struct sockaddr *)&addr6,
// 				     sizeof(addr6));
// 	}

// 	if (sock4 < 0 && sock6 < 0) {
// 		LOG_ERR("Cannot create HTTP connection.");
// 		return -ECONNABORTED;
// 	}

// 	if (sock4 >= 0 && IS_ENABLED(CONFIG_NET_IPV4)) {
// 		struct http_request req;

// 		memset(&req, 0, sizeof(req));

// 		req.method = HTTP_GET;
// 		req.url = "/";
// 		req.host = serverIp;
// 		req.protocol = "HTTP/1.1";
// 		req.response = response_cb;
// 		req.recv_buf = recv_buf_ipv4;
// 		req.recv_buf_len = sizeof(recv_buf_ipv4);

//         char user_data[] = "erase2.uf2";
// 		ret = http_client_req(sock4, &req, timeout, user_data);
// 		if(ret < 0)
// 		{
// 			LOG_ERR("Error downloading file ret = %d", ret);
// 		}
// 		else
// 		{

// 		}

// 		close(sock4);
// 	}

// 	if (sock6 >= 0 && IS_ENABLED(CONFIG_NET_IPV6)) {
// 		struct http_request req;

// 		memset(&req, 0, sizeof(req));

// 		req.method = HTTP_GET;
// 		req.url = "/";
// 		req.host = SERVER_ADDR6;
// 		req.protocol = "HTTP/1.1";
// 		req.response = response_cb;
// 		req.recv_buf = recv_buf_ipv6;
// 		req.recv_buf_len = sizeof(recv_buf_ipv6);

//         char user_data[] = "IPv6 GET";
// 		ret = http_client_req(sock6, &req, timeout, user_data);

// 		close(sock6);
// 	}

// 	sock4 = -1;
// 	sock6 = -1;

// 	if (IS_ENABLED(CONFIG_NET_IPV4)) {
// 		(void)connect_socket(AF_INET, serverIp, port,
// 				     &sock4, (struct sockaddr *)&addr4,
// 				     sizeof(addr4));
// 	}

// 	if (IS_ENABLED(CONFIG_NET_IPV6)) {
// 		(void)connect_socket(AF_INET6, SERVER_ADDR6, port,
// 				     &sock6, (struct sockaddr *)&addr6,
// 				     sizeof(addr6));
// 	}

// 	if (sock4 < 0 && sock6 < 0) {
// 		LOG_ERR("Cannot create HTTP connection.");
// 		return -ECONNABORTED;
// 	}

// 	if (sock4 >= 0 && IS_ENABLED(CONFIG_NET_IPV4)) {
// 		struct http_request req;

// 		memset(&req, 0, sizeof(req));

// 		req.method = HTTP_POST;
// 		req.url = "/foobar";
// 		req.host = serverIp;
// 		req.protocol = "HTTP/1.1";
// 		req.payload = "foobar";
// 		req.payload_len = strlen(req.payload);
// 		req.response = response_cb;
// 		req.recv_buf = recv_buf_ipv4;
// 		req.recv_buf_len = sizeof(recv_buf_ipv4);

//         char user_data[] = "IPv4 GET";
// 		ret = http_client_req(sock4, &req, timeout, user_data);

// 		close(sock4);
// 	}

// 	if (sock6 >= 0 && IS_ENABLED(CONFIG_NET_IPV6)) {
// 		struct http_request req;

// 		memset(&req, 0, sizeof(req));

// 		req.method = HTTP_POST;
// 		req.url = "/";
// 		req.host = SERVER_ADDR6;
// 		req.protocol = "HTTP/1.1";
// 		req.payload = "foobar";
// 		req.payload_len = strlen(req.payload);
// 		req.response = response_cb;
// 		req.recv_buf = recv_buf_ipv6;
// 		req.recv_buf_len = sizeof(recv_buf_ipv6);
//         char user_data[] = "IPv6 POST";

// 		ret = http_client_req(sock6, &req, timeout, user_data);

// 		close(sock6);
// 	}

// 	/* Do a chunked POST request */

// 	sock4 = -1;
// 	sock6 = -1;

// 	if (IS_ENABLED(CONFIG_NET_IPV4)) {
// 		(void)connect_socket(AF_INET, serverIp, port,
// 				     &sock4, (struct sockaddr *)&addr4,
// 				     sizeof(addr4));
// 	}

// 	if (IS_ENABLED(CONFIG_NET_IPV6)) {
// 		(void)connect_socket(AF_INET6, SERVER_ADDR6, port,
// 				     &sock6, (struct sockaddr *)&addr6,
// 				     sizeof(addr6));
// 	}

// 	if (sock4 < 0 && sock6 < 0) {
// 		LOG_ERR("Cannot create HTTP connection.");
// 		return -ECONNABORTED;
// 	}

// 	if (sock4 >= 0 && IS_ENABLED(CONFIG_NET_IPV4)) {
// 		struct http_request req;
// 		const char *headers[] = {
// 			"Transfer-Encoding: chunked\r\n",
// 			NULL
// 		};

// 		memset(&req, 0, sizeof(req));

// 		req.method = HTTP_POST;
// 		req.url = "/chunked-test";
// 		req.host = serverIp;
// 		req.protocol = "HTTP/1.1";
// 		req.payload_cb = payload_cb;
// 		req.header_fields = headers;
// 		req.response = response_cb;
// 		req.recv_buf = recv_buf_ipv4;
// 		req.recv_buf_len = sizeof(recv_buf_ipv4);

//         char user_data[] = "IPv4 POST";
// 		ret = http_client_req(sock4, &req, timeout, user_data);

// 		close(sock4);
// 	}

// 	if (sock6 >= 0 && IS_ENABLED(CONFIG_NET_IPV6)) {
// 		struct http_request req;
// 		const char *headers[] = {
// 			"Transfer-Encoding: chunked\r\n",
// 			NULL
// 		};

// 		memset(&req, 0, sizeof(req));

// 		req.method = HTTP_POST;
// 		req.url = "/chunked-test";
// 		req.host = SERVER_ADDR6;
// 		req.protocol = "HTTP/1.1";
// 		req.payload_cb = payload_cb;
// 		req.header_fields = headers;
// 		req.response = response_cb;
// 		req.recv_buf = recv_buf_ipv6;
// 		req.recv_buf_len = sizeof(recv_buf_ipv6);

//         char user_data[] = "IPv6 POST";
// 		ret = http_client_req(sock6, &req, timeout, user_data);

// 		close(sock6);
// 	}

// 	return ret;
// }

int Ota:: upgrade(const char *fileAddress)
{
	struct sockaddr_in addr4;
	int ret = 0;
	int port = HTTP_PORT;
	LOG_INF("Entered upgrade");

	flash_img_init(&ota_context.flash_ctx);

	while (retry)
	{
		connect_socket(AF_INET, serverIp, port, &ota_context.sock, (struct sockaddr *)&addr4, sizeof(addr4));
		// connect_socket(AF_INET, serverIp, port, &ota_context.sock, (struct sockaddr *)&addr4, sizeof(addr4));

		if(ota_context.sock < 0)
		{
			LOG_ERR("can't creare http connection");
			// return -ECONNABORTED;
		}

		if(ota_context.sock >= 0)
		{
			struct http_request req;
			memset(&req, 0, sizeof(req));
			req.method = HTTP_GET;
			req.url = fileAddress;
			req.host = serverIp;
			// req.url = "/zephyr.signed.bin";
			// req.host = "10.42.0.1";
			req.protocol = "HTTP/1.1";
			req.response = response_cb;
			req.recv_buf = ota_context.recv_buf;
			req.recv_buf_len = sizeof(ota_context.recv_buf);

			ota_context.content_length = 0;
			ota_context.status = HTTP_OTA_OK;
			LOG_DBG("host is: %s", req.host);
			LOG_DBG("url is: %s", req.url);
			ret = http_client_req(ota_context.sock, &req, HTTP_TIMEOUT, NULL);
			if(ret < 0 || ota_context.status != HTTP_OTA_OK)
			{
				LOG_ERR("Error download, ret is %d, status: %d", ret, ota_context.status);
			}
			else
			{
				/* flash image*/
				int ret = flash_img_buffered_write(&ota_context.flash_ctx, NULL, 0, true);
				if(ret < 0)
				{
					LOG_ERR("flash write error, %d", ret);
				}
				/* boot upgrade check*/
				ret = boot_request_upgrade(BOOT_UPGRADE_TEST);
				if(ret == 0)
				{
					LOG_INF("Update installed, resetting the board");
					sys_reboot(1);
				}
				else
				{
					LOG_ERR("Update not installed, %d", ret);
				}
			}
		}
		if(ota_context.sock >= 0)
		{
			close(ota_context.sock);
		}
	}
	retry = false;
	return ret;
}


#endif
