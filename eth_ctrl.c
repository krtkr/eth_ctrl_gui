/*
 * eth_ctrl.c
 *
 *  Created on: 14 ноября 2016 г.
 *      Author: krtkr
 *
 * MIT License
 *
 * Copyright (c) 2016 Kirill Kranke (krtkr), kranke.kirill@gmail.com
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */

#include "eth_ctrl.h"

#include <malloc.h>
#include <string.h>

#include <unistd.h>

#include <errno.h>
#include <sys/socket.h>
#include <sys/ioctl.h>

#include <linux/if.h>
#include <linux/if_arp.h>
#include <linux/if_packet.h>
#include <linux/if_ether.h>
#include <linux/ethtool.h>
#include <linux/sockios.h>
#include <netinet/in.h>

typedef struct {
	struct ifreq ifr;
	int socket;
	struct sockaddr_ll src_address;
	struct sockaddr_ll dst_address;
} connection_t;

#define ETH_CTRL_FRAME_TYPE 0xa1b5

const char* eth_ctrl_error_msgs[9] = {
		"no error",
		"conn pointer is null",
		"attempt to reopen or forgot to set connection to zero on initialization",
		"malloc failed: unable to allocate memory, see errno",
		"NIC name is null",
		"NIC name is too long",
		"socket failed: unable to open socket, see errno",
		"ioctl failed: unable to get HWADDR, see errno",
		"ioctl failed: unable to get NIC index, see errno",
};

const uint8_t eth_ctrl_mac[6] = {0x00, 0xa1, 0xb5, 0x00, 0x00, 0x00};

int eth_ctrl_open(eth_ctrl_conn_t* conn, const char* nic_name) {
	int retVal = -1;
	errno = 0;
	if (!conn) {
		retVal = ETH_CTRL_CONN_IS_NULL;
		goto cleanup_nothing;
	}

	if (conn->connection) {
		retVal = ETH_CTRL_ATTEMPT_TO_REOPEN;
		goto cleanup_nothing;
	}

	errno = EINVAL;
	conn->connection = malloc(sizeof(connection_t));
	if (conn->connection == 0) {
		retVal = ETH_CTRL_MALLOC_ERROR;
		goto cleanup_nothing;
	}
	connection_t* internal = (connection_t*)conn->connection;
	memset(internal, 0, sizeof(connection_t));

	errno = 0;
	/*
	 * If you use PF_PACKET,SOCK_DGRAM, then it builds the link-layer headers
	 * for you, which is normally what you want. You still need to build
	 * whatever higher protocol you are using on top though.
	 */
	internal->socket = socket(AF_PACKET, SOCK_DGRAM, htons(ETH_CTRL_FRAME_TYPE));
	if (internal->socket == -1) {
		retVal = ETH_CTRL_UNABLE_TO_OPEN_SOCKET;
		goto cleanup_internal;
	}

	/*RAW communication*/
	internal->dst_address.sll_family   = AF_PACKET;

	if (nic_name) {
		internal->dst_address.sll_protocol = htons(ETH_CTRL_FRAME_TYPE);

		errno = 0;
		if (strlen(nic_name) > sizeof(internal->ifr.ifr_ifrn.ifrn_name)) {
			retVal = ETH_CTRL_NIC_NAME_TOO_LONG;
			goto cleanup_socket;
		}

		memcpy(internal->ifr.ifr_ifrn.ifrn_name, nic_name, strlen(nic_name));

		errno = 0;
		if (ioctl(internal->socket, SIOCGIFHWADDR, &(internal->ifr)) == -1) {
			retVal = ETH_CTRL_UNABLE_TO_GET_HWADDR;
			goto cleanup_socket;
		}

		errno = 0;
		if (ioctl(internal->socket, SIOCGIFINDEX, &(internal->ifr)) == -1) {
			retVal = ETH_CTRL_UNABLE_TO_GET_NIC_INDEX;
			goto cleanup_socket;
		}

		/*index of the network device */
		internal->dst_address.sll_ifindex   = internal->ifr.ifr_ifru.ifru_ivalue;

		errno = 0;
		if (bind(internal->socket, (struct sockaddr*)&(internal->dst_address), sizeof(internal->dst_address)) == -1) {
			retVal = ETH_CTRL_UNABLE_TO_GET_NIC_INDEX;
			goto cleanup_socket;
		}
	} else {
		retVal = ETH_CTRL_UNABLE_TO_OPEN_SOCKET;
		goto cleanup_internal;
	}

	// See http://linuxdoc.ru/packet.html for some details

	/*ARP hardware identifier is Ethernet*/
	internal->dst_address.sll_hatype   = ARPHRD_ETHER;

	/*target is another host*/
	internal->dst_address.sll_pkttype  = PACKET_HOST;

	/*address length*/
	internal->dst_address.sll_halen    = ETH_ALEN;

	/* MAC - begin */
	memcpy(internal->dst_address.sll_addr, eth_ctrl_mac, 6);

	/*MAC - end*/
	internal->dst_address.sll_addr[6]  = 0x00;/*not used*/
	internal->dst_address.sll_addr[7]  = 0x00;/*not used*/

	memcpy(&(internal->src_address), &(internal->dst_address), sizeof(internal->src_address));

	return(ETH_CTRL_NO_ERROR);

cleanup_socket:
	close(internal->socket);

cleanup_internal:
	free(conn->connection);
	conn->connection = 0;

cleanup_nothing:
	return(retVal);
};

int eth_ctrl_send(eth_ctrl_conn_t* conn, eth_ctrl_tx_data_t* tx_data, unsigned module_id) {
	connection_t* internal = (connection_t*)conn->connection;
	internal->dst_address.sll_addr[5] = module_id;
	errno = 0;
	ssize_t retVal = sendto(internal->socket, (void*)tx_data, sizeof(eth_ctrl_tx_data_t), 0, (struct sockaddr*)&(internal->dst_address), sizeof(internal->dst_address));
	if (retVal < 0) {
		return(-1);
	}
	else {
		return(retVal != sizeof(eth_ctrl_tx_data_t));
	}

	return(-1);
};

int eth_ctrl_recv(eth_ctrl_conn_t* conn, eth_ctrl_rx_data_t* rx_data) {
	connection_t* internal = (connection_t*)conn->connection;
	socklen_t addrlen = sizeof(internal->src_address);

	errno = 0;
	do {
		ssize_t retVal = recvfrom(internal->socket, (void*)rx_data, sizeof(eth_ctrl_rx_data_t), 0, (struct sockaddr*)&(internal->src_address), &addrlen);
		if(retVal < 0 || (retVal != sizeof(eth_ctrl_rx_data_t))) {
			return(-1);
		}
	} while (memcmp(internal->src_address.sll_addr, eth_ctrl_mac, 5));

	return(internal->src_address.sll_addr[5]);
};

void eth_ctrl_close(eth_ctrl_conn_t* conn) {
	if(conn && conn->connection) {
		connection_t* internal = (connection_t*)conn->connection;
		close(internal->socket);
		free(conn->connection);
		conn->connection = 0;
	}
};

int eth_ctrl_version() {
	return(4);
};

