//============================================================================
// Example, Zephyr RTOS kernel
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
#include "qpc.h"                 // QP/C real-time event framework
#include "bsp.h"                 // Board Support Package
#include "app.h"                 // Application

#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/reboot.h>
// add other drivers if necessary...

//============================================================================
// The devicetree node identifier for the "led0" alias.
#define LED0_NODE DT_ALIAS(led0)

Q_DEFINE_THIS_FILE  // file name for assertions

// Local objects -------------------------------------------------------------
static struct gpio_dt_spec const l_led0 = GPIO_DT_SPEC_GET(LED0_NODE, gpios);
static struct k_timer zephyr_tick_timer;

#ifdef Q_SPY
    enum AppRecords { // application-specific trace records
        LED_STAT = QS_USER,
    };

    // QSpy source IDs
    static QSpyId const timerID = { QS_ID_AP };
#endif // Q_SPY

//============================================================================
// Error handler

Q_NORETURN Q_onError(char const * const module, int_t const id) {
    // NOTE: this implementation of the error handler is intended only
    // for debugging and MUST be changed for deployment of the application
    // (assuming that you ship your production code with assertions enabled).
    Q_UNUSED_PAR(module);
    Q_UNUSED_PAR(id);
    QS_ASSERTION(module, id, 10000U); // report assertion to QS

#ifndef NDEBUG
    Q_PRINTK("\nERROR in %s:%d\n", module, id);
    k_panic(); // debug build: halt the system for error search...
#endif

    sys_reboot(SYS_REBOOT_COLD); // release build: reboot the system
    for (;;) { // explicitly "no-return"
    }
}
//............................................................................
// Zephyr error handler redirecting to the QP error handler
void k_sys_fatal_error_handler(unsigned int reason,
    const struct arch_esf *esf)
{
    Q_UNUSED_PAR(esf);
    Q_onError("zephyr", reason);
}

//............................................................................
static void zephyr_tick_function(struct k_timer *tid); // prototype
static void zephyr_tick_function(struct k_timer *tid) {
    Q_UNUSED_PAR(tid);

    QTIMEEVT_TICK_X(0U, &timerID); // process time events at rate 0
}

//============================================================================
// BSP...

void BSP_init(void const * const arg) {
    Q_UNUSED_PAR(arg);

    int ret = gpio_pin_configure_dt(&l_led0, GPIO_OUTPUT_ACTIVE);
    Q_ASSERT(ret >= 0);

    k_timer_init(&zephyr_tick_timer, &zephyr_tick_function, (void *)0);

    // initialize QS software tracing...
    if (!QS_INIT(arg)) {
        Q_ERROR();
    }

    // QS dictionaries...
    QS_OBJ_DICTIONARY(&timerID);
    QS_SIG_DICTIONARY(TIMEOUT_SIG, (void *)0);
    QS_USR_DICTIONARY(LED_STAT);

    // setup the QS filters...
    QS_GLB_FILTER(QS_GRP_ALL);  // enable all records
    QS_GLB_FILTER(-QS_QF_TICK); // exclude the tick record

    // no dynamic events -- no need to call QF_poolInit();
    // no publish-subscribe -- no need to call QActive_psInit();
}
//............................................................................
void BSP_ledOn(void) {
    gpio_pin_set_dt(&l_led0, true);
    Q_PRINTK("BSP_ledOn\n");
    // application-specific record
    QS_BEGIN_ID(LED_STAT, AO_Blinky->prio)
        QS_STR("ON"); // LED status
    QS_END()
}
//............................................................................
void BSP_ledOff(void) {
    gpio_pin_set_dt(&l_led0, false);
    Q_PRINTK("BSP_ledOff\n");
    // application-specific record
    QS_BEGIN_ID(LED_STAT, AO_Blinky->prio)
        QS_STR("OFF"); // LED status
    QS_END()
}

//============================================================================
// QF...

void QF_onStartup(void) {
    // instantiate and start AOs/threads...
    Blinky_ctor();
    static QEvtPtr blinkyQueueSto[10];
    static K_THREAD_STACK_DEFINE(blinkyStack, 512);
    QActive_start(AO_Blinky,
        1U,                    // QP prio. of the AO
        blinkyQueueSto,        // event queue storage
        Q_DIM(blinkyQueueSto), // queue length [events]
        blinkyStack,           // private stack for embOS
        K_THREAD_STACK_SIZEOF(blinkyStack), // stack size [Zephyr]
        (void *)0);            // no initialization param

    k_timer_start(&zephyr_tick_timer, K_MSEC(1), K_MSEC(1));
    Q_PRINTK("QF_onStartup\n");
}
//............................................................................
void QF_onCleanup(void) {
    Q_PRINTK("QF_onCleanup\n");
}

//============================================================================
#ifdef Q_SPY

#include <zephyr/drivers/uart.h>

// select the Zephyr shell UART
// NOTE: you can change this to other UART peripheral if desired
static struct device const *uart_dev =
     DEVICE_DT_GET(DT_CHOSEN(zephyr_shell_uart));

//............................................................................
static void uart_cb(const struct device *dev, void *user_data) {
    if (!uart_irq_update(uart_dev)) {
        return;
    }

    if (uart_irq_rx_ready(uart_dev)) {
        uint8_t buf[32];
        int n = uart_fifo_read(uart_dev, buf, sizeof(buf));
        for (uint8_t const *p = buf; n > 0; --n, ++p) {
            QS_RX_PUT(*p);
        }
    }
}
//............................................................................
uint8_t QS_onStartup(void const * const arg) {
    Q_UNUSED_PAR(arg);

    Q_REQUIRE(uart_dev != (struct device *)0);

    static uint8_t qsTxBuf[2*1024]; // buffer for QS-TX channel
    QS_initBuf  (qsTxBuf, sizeof(qsTxBuf));

    static uint8_t qsRxBuf[128];  // buffer for QS-RX channel
    QS_rxInitBuf(qsRxBuf, sizeof(qsRxBuf));

    // configure interrupt and callback to receive data
    uart_irq_callback_user_data_set(uart_dev, &uart_cb, (void *)0);
    uart_irq_rx_enable(uart_dev);

    return 1U; // return success
}
//............................................................................
void QS_onCleanup(void) {
}
//............................................................................
QSTimeCtr QS_onGetTime(void) {  // NOTE: invoked with interrupts DISABLED
    return k_cycle_get_32();
}
//............................................................................
// NOTE:
// No critical section in QS_onFlush() to avoid nesting of critical sections
// in case QS_onFlush() is called from Q_onError().
void QS_onFlush(void) {
    uint16_t len = 0xFFFFU; // to get as many bytes as available
    uint8_t const *buf;
    while ((buf = QS_getBlock(&len)) != (uint8_t*)0) {
        for (; len != 0U; --len, ++buf) {
            uart_poll_out(uart_dev, *buf);
        }
        len = 0xFFFFU; // to get as many bytes as available
    }
}
//............................................................................
void QS_onReset(void) {
    sys_reboot(SYS_REBOOT_COLD);
}
//............................................................................
void QS_onCommand(uint8_t cmdId,
    uint32_t param1, uint32_t param2, uint32_t param3)
{
    Q_UNUSED_PAR(cmdId);
    Q_UNUSED_PAR(param1);
    Q_UNUSED_PAR(param2);
    Q_UNUSED_PAR(param3);
}
//............................................................................
void QF_onIdle(void) {
    QS_rxParse();   // parse any QS-RX bytes

    uint16_t len = 0xFFFFU; // big number to get all available bytes

    QF_CRIT_STAT
    QF_CRIT_ENTRY();
    uint8_t const *buf = QS_getBlock(&len);
    QF_CRIT_EXIT();

    // transmit the bytes via the UART...
    for (; len != 0U; --len, ++buf) {
        uart_poll_out(uart_dev, *buf);
    }
}

#endif // Q_SPY
