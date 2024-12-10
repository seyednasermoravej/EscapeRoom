/* Networking DHCPv4 client */

/*
 * Copyright (c) 2017 ARM Ltd.
 * Copyright (c) 2016 Intel Corporation.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "dhcpClient.h"

LOG_MODULE_REGISTER(net_dhcpv4_client, LOG_LEVEL_INF);

sem_t dhcpActive;

static uint8_t ntp_server[4];

static struct net_mgmt_event_callback mgmt_cb;

static struct net_dhcpv4_option_callback dhcp_cb;

static void start_dhcpv4_client(struct net_if *iface, void *user_data)
{
	// ARG_UNUSED(user_data);
	net_hostname_set((const char*)user_data, strlen((const char*)user_data));
	LOG_INF("Start on %s: index=%d", net_if_get_device(iface)->name,
	net_if_get_by_iface(iface));
	#if defined(CONFIG_BOARD_RPI_PICO_RP2040_W)
		struct wifi_connect_req_params connect_params = {
        .ssid = "Safineh-2",
        .ssid_length = strlen("Safineh-2"),
        .psk = "gXvNavG4mg@9c9Z8",
        .psk_length = strlen("gXvNavG4mg@9c9Z8"),
		.security = WIFI_SECURITY_TYPE_PSK,
    };	
	net_mgmt(NET_REQUEST_WIFI_CONNECT, iface, &connect_params, sizeof(connect_params));
	#endif
	net_dhcpv4_start(iface);
}

static void handler(struct net_mgmt_event_callback *cb,
		    uint32_t mgmt_event,
		    struct net_if *iface)
{
	int i = 0;

	if (mgmt_event != NET_EVENT_IPV4_ADDR_ADD) {
		return;
	}

	for (i = 0; i < NET_IF_MAX_IPV4_ADDR; i++) {
		char buf[NET_IPV4_ADDR_LEN];

		if (iface->config.ip.ipv4->unicast[i].ipv4.addr_type !=
							NET_ADDR_DHCP) {
			continue;
		}

		LOG_INF("   Address[%d]: %s", net_if_get_by_iface(iface),
			net_addr_ntop(AF_INET,
			    &iface->config.ip.ipv4->unicast[i].ipv4.address.in_addr,
						  buf, sizeof(buf)));
		LOG_INF("    Subnet[%d]: %s", net_if_get_by_iface(iface),
			net_addr_ntop(AF_INET,
				       &iface->config.ip.ipv4->unicast[i].netmask,
				       buf, sizeof(buf)));
		LOG_INF("    Router[%d]: %s", net_if_get_by_iface(iface),
			net_addr_ntop(AF_INET,
						 &iface->config.ip.ipv4->gw,
						 buf, sizeof(buf)));
		LOG_INF("Lease time[%d]: %u seconds", net_if_get_by_iface(iface),
			iface->config.dhcpv4.lease_time);
			// struct in_addr gateway_addr = {192, 168, 9, 1};
			// struct in_addr gateway_addr = {192, 168, 0, 1};
		// 	struct in_addr gateway_addr = {192, 168, 0, 1};
		// net_if_ipv4_set_gw(iface, &gateway_addr);

		const struct net_linkaddr *link_addr = net_if_get_link_addr(iface);
		const uint8_t *mac = link_addr->addr;

		LOG_INF("MAC Address: %02x:%02x:%02x:%02x:%02x:%02x\n",
			mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
			sem_post(&dhcpActive);
	}
}

static void option_handler(struct net_dhcpv4_option_callback *cb,
			   size_t length,
			   enum net_dhcpv4_msg_type msg_type,
			   struct net_if *iface)
{
	char buf[NET_IPV4_ADDR_LEN];

	LOG_INF("DHCP Option %d: %s", cb->option,
		net_addr_ntop(AF_INET, cb->data, buf, sizeof(buf)));
}

int dhcpClient(const char *deviceName)
{
	LOG_INF("Run dhcpv4 client");

	sem_init(&dhcpActive, 0, 0);
	net_mgmt_init_event_callback(&mgmt_cb, handler,
				     NET_EVENT_IPV4_ADDR_ADD);
	net_mgmt_add_event_callback(&mgmt_cb);

	net_dhcpv4_init_option_callback(&dhcp_cb, option_handler,
					DHCP_OPTION_NTP, ntp_server,
					sizeof(ntp_server));
	
	net_dhcpv4_add_option_callback(&dhcp_cb);

	net_if_foreach(start_dhcpv4_client, deviceName);
	sem_wait(&dhcpActive);

    sem_destroy(&dhcpActive);
	return 0;
}
