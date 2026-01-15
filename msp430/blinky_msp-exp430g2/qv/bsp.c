//============================================================================
// Product: Blinky on MSP-EXP430G2, cooperative QV kernel
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

#include <msp430g2553.h>  // MSP430 variant used
// add other drivers if necessary...

//============================================================================
Q_DEFINE_THIS_FILE  // file name for assertions

// Local-scope defines -------------------------------------------------------
// 1MHz clock setting, see BSP_init()
#define BSP_MCK     1000000U
#define BSP_SMCLK   1000000U

#define LED1        (1U << 0U)
#define LED2        (1U << 6U)

#ifdef Q_SPY
    #error Simple Blinky Application does not provide Spy build configuration
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
    for (;;) { // for debugging, hang on in an endless loop...
    }
#else
    WDTCTL = WDTPW | WDTHOLD;
    __asm("    push &0xFFFE");
    for (;;) { // explicitly "no-return"
    }
#endif
}
//............................................................................
// assertion failure handler for the startup and library code
void assert_failed(char const * const module, int_t const id); // prototype
void assert_failed(char const * const module, int_t const id) {
    Q_onError(module, id);
}

// ISRs used in the application ==============================================

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
    __interrupt void TIMER0_A0_ISR(void); // prototype
    #pragma vector=TIMER0_A0_VECTOR
    __interrupt void TIMER0_A0_ISR(void)
#elif defined(__GNUC__)
    void __attribute__ ((interrupt(TIMER0_A0_VECTOR))) TIMER0_A0_ISR (void)
#else
    #error MSP430 compiler not supported!
#endif
{
    TACTL &= ~TAIFG; // clear the interrupt pending flag
    QTIMEEVT_TICK_X(0U, (void *)0); // process all time events at rate 0
#ifdef NDEBUG
    __low_power_mode_off_on_exit(); // see NOTE1
#endif
}

//============================================================================
// BSP...

void BSP_init(void const * const arg) {
    Q_UNUSED_PAR(arg);

    WDTCTL = WDTPW | WDTHOLD; // stop watchdog timer

    // configure the Basic Clock Module
    DCOCTL = 0;             // Select lowest DCOx and MODx settings
    BCSCTL1 = CALBC1_1MHZ;  // Set DCO
    DCOCTL = CALDCO_1MHZ;

    P1DIR |= (LED1 | LED2);  // set LED1 and LED2 pins to output

    // no software tracing -- no need to initialize and setup QS
    // no dynamic events -- no need to call QF_poolInit();
    // no publish-subscribe -- no need to call QActive_psInit();
}
//............................................................................
void BSP_ledOn(void) {
    P1OUT |= LED1; // turn LED1 on
}
//............................................................................
void BSP_ledOff(void) {
    P1OUT &= ~LED1; // turn LED1 off
}

//============================================================================
// QF callbacks...

void QF_onStartup(void) {
    // instantiate and start AOs/threads...
    Blinky_ctor();
    static QEvtPtr blinkyQueueSto[10];
    QActive_start(AO_Blinky,
        1U,                    // QP prio. of the AO
        blinkyQueueSto,        // event queue storage
        Q_DIM(blinkyQueueSto), // queue length [events]
        (void *)0, 0U,         // no stack storage
        (void *)0);            // no initialization param

    TACTL  = (ID_3 | TASSEL_2 | MC_1);  // SMCLK, /8 divider, upmode
    TACCR0 = (((BSP_SMCLK / 8U) + BSP_TICKS_PER_SEC/2U) / BSP_TICKS_PER_SEC);
    CCTL0 = CCIE;  // CCR0 interrupt enabled
}
//............................................................................
void QF_onCleanup(void) {
}

//............................................................................
void QV_onIdle(void) { // NOTE: called with interrupts DISABLED, see NOTE1
    // toggle LED2 on and then off, see NOTE2
    P1OUT |=  LED2;        // turn LED2 on
    P1OUT &= ~LED2;        // turn LED2 off

#ifdef NDEBUG
    // Put the CPU and peripherals to the low-power mode.
    // you might need to customize the clock management for your application,
    // see the datasheet for your particular MSP430 MCU.
    __low_power_mode_1(); // enter LPM1; also ENABLES interrupts
#else
    QF_INT_ENABLE(); // just enable interrupts
#endif
}

//============================================================================
// NOTE1:
// With the cooperative QV kernel for MSP430, it is necessary to explicitly
// turn the low-power mode OFF in the interrupt, because the return
// from the interrupt will restore the CPU status register, which will
// re-enter the low-power mode. This, in turn, will prevent the QV event-loop
// from running.
//
// NOTE2:
// One of the LEDs is used to visualize the idle loop activity. The brightness
// of the LED is proportional to the frequency of invocations of the idle loop.
// Please note that the LED is toggled with interrupts locked, so no interrupt
// execution time contributes to the brightness of the User LED.
//
