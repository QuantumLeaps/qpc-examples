//============================================================================
// lwIP consolidated file for QP-lwIP integration
//
// Copyright (C) 2005 Quantum Leaps, LLC. All rights reserved.
//
//                    Q u a n t u m  L e a P s
//                    ------------------------
//                    Modern Embedded Software
//
// SPDX-License-Identifier: GPL-3.0-or-later OR LicenseRef-QL-commercial
//
// This software is dual-licensed under the terms of the open-source GNU
// General Public License (GPL) or under the terms of one of the closed-
// source Quantum Leaps commercial licenses.
//
// Redistributions in source code must retain this top-level comment block.
// Plagiarizing this software to sidestep the license obligations is illegal.
//
// NOTE:
// The GPL does NOT permit the incorporation of this code into proprietary
// programs. Please contact Quantum Leaps for commercial licensing options,
// which expressly supersede the GPL and are designed explicitly for
// closed-source distribution.
//
// Quantum Leaps contact information:
// <www.state-machine.com/licensing>
// <info@state-machine.com>
//============================================================================
#include "lwip/opt.h"  // lwIP options come first
#include "lwip/api.h"
#include "lwip/tcp.h"
#include "lwip/udp.h"
#include "lwip/tcpip.h"
#include "lwip/mem.h"

// NOTE: all the following modules contain conditional compilation protection,
// so that code will be actually generated only if the appropriate options
// are set in the lwipopts.h header file included from lwip/opt.h.
//

// Core lwIP TCP/IP stack ..................................................
#include "core/def.c"
#include "core/dhcp.c"
#include "core/dns.c"
#include "core/init.c"
#include "core/mem.c"
#include "core/memp.c"
#include "core/netif.c"
#include "core/pbuf.c"
#include "core/raw.c"
#include "core/stats.c"
#include "core/sys.c"
#include "core/tcp.c"
#include "core/tcp_in.c"
#include "core/tcp_out.c"
#include "core/timers.c"
#include "core/udp.c"

// IPv4 ....................................................................
#include "core/ipv4/autoip.c"
#include "core/ipv4/icmp.c"
#include "core/ipv4/igmp.c"
#include "core/ipv4/inet.c"
#include "core/ipv4/inet_chksum.c"
#include "core/ipv4/ip.c"
#include "core/ipv4/ip_addr.c"
#include "core/ipv4/ip_frag.c"

// SNMP ....................................................................
#include "core/snmp/asn1_dec.c"
#include "core/snmp/asn1_enc.c"
#include "core/snmp/mib2.c"
#include "core/snmp/mib_structs.c"
#include "core/snmp/msg_in.c"
#include "core/snmp/msg_out.c"

// Ethernet interface ......................................................
#include "netif/etharp.c"
//???#include "netif/loopif.c"

// PPP .....................................................................
#if PPP_SUPPORT
    #include "netif/ppp/auth.c"
    #include "netif/ppp/chap.c"
    #include "netif/ppp/chpms.c"
    #include "netif/ppp/fsm.c"
    #include "netif/ppp/ipcp.c"
    #include "netif/ppp/lcp.c"
    #include "netif/ppp/magic.c"
    #include "netif/ppp/md5.c"
    #include "netif/ppp/pap.c"
    #include "netif/ppp/ppp.c"
    #include "netif/ppp/ppp_oe.c"
    #include "netif/ppp/randm.c"
    #include "netif/ppp/vj.c"
#endif // PPP_SUPPORT

// utilities added by QL ...................................................
//!
//! Allocate a transport-layer pbuf and copies the provided data buffer 'data'
//! of length 'len' bytes into the payload(s) of the pbuf. The function takes
//! care of splitting the data into successive pbuf payloads, if necessary.
//!
//! The function returns the newly created pbuf or NULL if the pbuf cannot
//! be allocated.
//!
struct pbuf *pbuf_new(u8_t *data, u16_t len) {
    struct pbuf *p = pbuf_alloc(PBUF_TRANSPORT, len, PBUF_RAM);
    struct pbuf *q = p;
    while ((q != (struct pbuf *)0) && (len >= q->len)) {
        memcpy(q->payload, data, q->len); // copy data into payload
        len  -= q->len; // remaining length
        data += q->len; // remaining data chunk
        q = q->next;    // get next pbuf
    }
    return p;
}
