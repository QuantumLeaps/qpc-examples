//============================================================================
// QP configuration file (host with console and just QEP)
//
// Copyright (C) 2005 Quantum Leaps, LLC. All rights reserved.
//
//                   Q u a n t u m  L e a P s
//                   ------------------------
//                   Modern Embedded Software
//
// SPDX-License-Identifier: GPL-3.0-or-later OR LicenseRef-QL-commercial
//
// The QP/C software is dual-licensed under the terms of the open-source GNU
// General Public License (GPL) or under the terms of one of the closed-
// source Quantum Leaps commercial licenses.
//
// Redistributions in source code must retain this top-level comment block.
// Plagiarizing this software to sidestep the license obligations is illegal.
//
// NOTE:
// The GPL (see <www.gnu.org/licenses/gpl-3.0>) does NOT permit the
// incorporation of the QP/C software into proprietary programs. Please
// contact Quantum Leaps for commercial licensing options, which expressly
// supersede the GPL and are designed explicitly for licensees interested
// in using QP/C in closed-source proprietary applications.
//
// Quantum Leaps contact information:
// <www.state-machine.com/licensing>
// <info@state-machine.com>
//============================================================================
#ifndef QP_CONFIG_H_
#define QP_CONFIG_H_

//-------- <<< Use Configuration Wizard in Context Menu >>> -----------------

// <o>QP API compatibility version (QP_API_VERSION)
//   <0=>  0   (Maximum compatibility)
//   <691=>691 (QP 6.9.1 or newer)
//   <734=>7.3.4 (QP 7.3.4 or newer)
//   <9999=>9999 (Latest only)
// <i>QP API backwards compatibility with the QP/C API version.
// <i>Lower QP_API_VERSION values enable backwards compatibility
// <i>with lower (older) QP API versions.
// <i>For example, QP_API_VERSION==691 will enable the compatibility
// <i>layer with QP version 6.9.1 and newer, but not older than 6.9.1.
// <i>QP_API_VERSION==0 enables the maximum currently supported
// <i>backwards compatibility. Conversely, QP_API_VERSION==9999 means
// <i>that no backwards compatibility layer should be enabled.
// <i>Default: 0 (All supported)
#define QP_API_VERSION 0

//..........................................................................
// <h>QP Functional Safety (FuSa) Subsystem (Q_UNSAFE)
// <i>The QP FuSa Subsystem consists of the following facilities:
// <i>- Software assertions as a recommended technique
// <i>  (called Failure Assertion Programming (FAP) in IEC 61508)
// <i>- Software Self-Monitoring (SSM), which encompasses such techniques:
// <i>  * Duplicate Inverse Storage for critical variables
// <i>  * Memory Markers for critical objects (e.g., events)
// <i>  * Hard-limits for all loops
// <i>  * Memory Isolation by means of Memory Protection Unit (MPU)

// <c3>Disable QP FuSa in development
// <i>Disable assertions and other self monitoring features
// <i>in development build configurations (NDEBUG undefined).
// <i>VIOLATES functional safety standards. NOT recommended !!!
//#ifndef NDEBUG
//#define Q_UNSAFE
//#endif
// </c>

// <c3>Disable QP FuSa in production release
// <i>Disable assertions and other self monitoring features
// <i>in the release build configurations (NDEBUG defined).
// <i>VIOLATES functional safety standards. NOT recommended !!!
//#ifdef NDEBUG
//#define Q_UNSAFE
//#endif
// </c>

// </h>

//..........................................................................
// <h>QEP Event Processor
// <i>Events and state machines.

// <o>Event signal size (Q_SIGNAL_SIZE)
//   <1U=>1
//   <2U=>2 (default)
//   <4U=>4
// <i>Size of the QEvt signal for QEP/QF [bytes]
// <i>Default: 2
#define Q_SIGNAL_SIZE  2U

// </h>

//..........................................................................
// <h>QF Framework
// <i>Active Object framework

// <o>Maximum # event pools (QF_MAX_EPOOL)
// <0=>0 no event pools
// <1=>1 <2=>2 <3=>3 (default) <4=>4 <5=>5
// <6=>6 <7=>7 <8=>8 <9=>9 <10=>10 <11=>11
// <12=>12 <13=>13 <14=>14 <15=>15
// <i>Maximum # Event Pools <1..15>
// <i>Default: 3
#define QF_MAX_EPOOL 0U

#define QF_CRIT_STAT
#define QF_CRIT_ENTRY() ((void)0)
#define QF_CRIT_EXIT()  ((void)0)

#define QF_MEM_SYS()    ((void)0)
#define QF_MEM_APP()    ((void)0)

// </h>

//------------- <<< end of configuration section >>> -----------------------

#endif // QP_CONFIG_H_
