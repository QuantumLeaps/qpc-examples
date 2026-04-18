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
#ifndef LWIP_H
#define LWIP_H

               // protection against including lwIP in unauthorized modules
#ifndef LWIP_ALLOWED
    #error The lwIP code is not reentrant and is not allowed in this module.
#endif

#include "lwip/opt.h"     // lwIP options come first
#include "lwip/stats.h"
#include "lwip/tcp.h"
#include "lwip/tcp_impl.h"
#include "lwip/udp.h"
#include "lwip/tcpip.h"
#include "lwip/mem.h"
#include "lwip/autoip.h"
#include "lwip/dhcp.h"

#include "netif/etharp.h"
#include "netif/eth_driver.h"

// utilities added by QL...
#ifdef __cplusplus
extern "C" {
#endif

//!
//! Allocate a transport-layer pbuf and copies the provided data buffer 'data'
//! of length 'len' bytes into the payload(s) of the pbuf. The function takes
//! care of splitting the data into successive pbuf payloads, if necessary.
//!
//! The function returns the newly created pbuf or NULL if the pbuf cannot
//! be allocated.
//!
struct pbuf *pbuf_new(u8_t *data, u16_t len);

#ifdef __cplusplus
}
#endif

#endif // LWIP_H
