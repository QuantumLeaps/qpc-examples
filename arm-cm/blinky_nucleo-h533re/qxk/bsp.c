//============================================================================
// Blinky example, NUCLEO-H533RE board, QK kernel
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
#include "qpc.h"          // QP/C real-time event framework
#include "bsp.h"          // Board Support Package
#include "app.h"          // Application

#include "stm32h533xx.h"  // CMSIS-compliant header file for the MCU used
// add other drivers if necessary...

//============================================================================
Q_DEFINE_THIS_FILE  // define the name of this file for assertions

// ROM configuration for the MCU
#define ROM_START 0x08000000
#define ROM_SIZE  (512*1024)

// RAM configuration for the MCU
// NOTE; The following RAM constants lump all SRAM blocks into one 272K block.
// STM323H533RE suuports: SRAM1 (128K), SRAM2 (80K with ECC), SRAM3 (64K).
#define RAM_START 0x20000000
#define RAM_SIZE  (272*1024)

// HSI CPU clock (64MHz) divisor (allowed 1U, 2U, 4U, 8U)
#define HSI_CLK_DIV 8U  // 64MHz/8 == 8MHz

// LED pins available on the board (just one user LED LD2--Green on PA.5)
#define LD2_PIN  5U

// Button pins available on the board (just one user Button B1 on PC.13)
#define B1_PIN   13U

// macros from STM32Cube LL:
#define SET_BIT(REG, BIT)     ((REG) |= (BIT))
#define CLEAR_BIT(REG, BIT)   ((REG) &= ~(BIT))
#define READ_BIT(REG, BIT)    ((REG) & (BIT))
#define CLEAR_REG(REG)        ((REG) = (0x0))
#define WRITE_REG(REG, VAL)   ((REG) = (VAL))
#define READ_REG(REG)         ((REG))
#define MODIFY_REG(REG, CLEARMASK, SETMASK) \
    WRITE_REG((REG), ((READ_REG(REG) & (~(CLEARMASK))) | (SETMASK)))

// Local-scope objects -----------------------------------------------------

#ifdef Q_SPY
enum AppRecords { // application-specific trace records
    LED_STAT = QS_USER,
};

// QSpy source IDs...
static QSpyId const l_SysTick_Handler = { QS_ID_AP };
#endif // Q_SPY

//============================================================================
// Error handler

Q_NORETURN Q_onError(char const * const module, int_t const id) {
    // NOTE: this implementation of the error handler is intended only
    // for debugging and MUST be changed for deployment of the application.
    Q_UNUSED_PAR(module);
    Q_UNUSED_PAR(id);
    QS_ASSERTION(module, id, 10000U); // report assertion to QS

#ifndef NDEBUG
    // light up the user LED
    GPIOA->BSRR = (1U << LD2_PIN);  // turn LD2 on
    for (;;) { // for debugging, hang on in an endless loop...
    }
#endif
    NVIC_SystemReset();
    for (;;) { // explicitly "no-return"
    }
}
//............................................................................
// assertion failure handler for the library code, including the startup code
void assert_failed(char const * const module, int_t const id); // prototype
void assert_failed(char const * const module, int_t const id) {
    Q_onError(module, id);
}

// ISRs ======================================================================
void SysTick_Handler(void); // prototype
void SysTick_Handler(void) {
    QXK_ISR_ENTRY();   // inform QXK about entering an ISR

    QTIMEEVT_TICK_X(0U, &l_SysTick_Handler); // time events for rate 0

    QXK_ISR_EXIT();  // inform QXK about exiting an ISR
}

//............................................................................
#ifdef Q_SPY
// ISR for receiving bytes from the QSPY Back-End
// NOTE: This ISR is "QF-unaware" meaning that it does not interact with
// the QF/QK and is not disabled. Such ISRs don't need to call
// QK_ISR_ENTRY/QK_ISR_EXIT and they cannot post or publish events.

void USART2_IRQHandler(void); // prototype
void USART2_IRQHandler(void) { // used in QS-RX (kernel UNAWARE interrupt)
    // while Read Data Register or RX-FIFO Not Empty
    while ((USART2->ISR & USART_ISR_RXNE_Msk ) != 0U) {
        uint32_t const b = USART2->RDR;
        QS_RX_PUT(b);
    }

    QXK_ARM_ERRATUM_838869();
}
#endif // Q_SPY

//============================================================================
// BSP...

static void MPU_Config(void) {
    MPU->CTRL = 0U; // disable the MPU
    __DSB();
    __ISB();

    // Enable MemManage Faults
    SCB->SHCSR |= SCB_SHCSR_MEMFAULTENA_Msk;

    // Set Attr 0
    ARM_MPU_SetMemAttr(0UL,
        ARM_MPU_ATTR(     // Normal memory
            // Outer Write-Through non-transient
            ARM_MPU_ATTR_MEMORY_(1UL, 0UL, 1UL, 0UL),
            // Inner Write-Through non-transient
            ARM_MPU_ATTR_MEMORY_(1UL, 0UL, 1UL, 0UL))
    );

    MPU->RNR = 0U; // region 0 (for ROM: read-only, can-execute)
    MPU->RBAR = ARM_MPU_RBAR(ROM_START,
        ARM_MPU_SH_NON,        // SH: Normal memory (not-shareable)
        1U,                    // RO: Normal memory, read-only
        0U,                    // NP: Normal memory, privileged access only
        0U);                   // XN: eXecute never (disabled)
    MPU->RLAR = ARM_MPU_RLAR(ROM_START + ROM_SIZE - 1U, 0U);

    MPU->RNR = 1U; // region 0 (for RAM: read-write, execute-never)
    MPU->RBAR = ARM_MPU_RBAR(RAM_START,
        ARM_MPU_SH_OUTER,      // SH: Normal memory (outer shareable)
        0U,                    // RO: Normal memory, read/write
        0U,                    // NP: Normal memory, privileged access only
        1U);                   // XN: eXecute never
    MPU->RLAR = ARM_MPU_RLAR(RAM_START + RAM_SIZE - 1U, 0U);

    /* NULL-region protection:
    unnecessary because addresses 0..ROM_START-1 are already illegal

    MPU->RNR = 7U; // region 7 (for NULL-pointer protection)
    MPU->RBAR = ARM_MPU_RBAR(0x00000000U,
        ARM_MPU_SH_OUTER,
        ARM_MPU_AP_RW,
        ARM_MPU_AP_PO,
        ARM_MPU_XN);
    MPU->RLAR = ARM_MPU_RLAR(ROM_START - 1U, 0U);
    */

    // Enable MPU with all region definitions
    __DMB();
    MPU->CTRL = MPU_CTRL_PRIVDEFENA_Msk
                | MPU_CTRL_HFNMIENA_Msk
                | MPU_CTRL_ENABLE_Msk;

    __DSB();
    __ISB();
}
//..........................................................................
static void SystemClock_Config(void); // prototype
static void SystemClock_Config(void) {
#if HSI_CLK_DIV == 1U
    uint32_t const rcc_cr_hisdiv = 0U;              // div by 1
    uint32_t const flash_latency = FLASH_ACR_LATENCY_3WS;
#elif HSI_CLK_DIV == 2U
    uint32_t const rcc_cr_hisdiv = RCC_CR_HSIDIV_0; // div by 2
    uint32_t const flash_latency = FLASH_ACR_LATENCY_1WS;
#elif HSI_CLK_DIV == 4U
    uint32_t const rcc_cr_hisdiv = RCC_CR_HSIDIV_1; // div by 4
    uint32_t const flash_latency = FLASH_ACR_LATENCY_0WS;
#elif HSI_CLK_DIV == 8U
    uint32_t const rcc_cr_hisdiv = RCC_CR_HSIDIV;   // div by 8
    uint32_t const flash_latency = FLASH_ACR_LATENCY_0WS;
#else
    error Unsupported HSI_CLK_DIV
#endif

    // Flash latency depends on the clock speed...
    MODIFY_REG(FLASH->ACR, FLASH_ACR_LATENCY, flash_latency);
    while (READ_BIT(FLASH->ACR, 0U)) {
    }

    MODIFY_REG(PWR->VOSCR, PWR_VOSCR_VOS, 0U);
    while (READ_BIT(PWR->VOSSR, PWR_VOSSR_VOSRDY) != PWR_VOSSR_VOSRDY) {
    }

    SET_BIT(RCC->CR, RCC_CR_HSION);
    while (READ_BIT(RCC->CR, RCC_CR_HSIRDY) != RCC_CR_HSIRDY) {
    }

    MODIFY_REG(RCC->HSICFGR, RCC_HSICFGR_HSITRIM, 64U << RCC_HSICFGR_HSITRIM_Pos);
    MODIFY_REG(RCC->CR, RCC_CR_HSIDIV, rcc_cr_hisdiv);
    MODIFY_REG(RCC->CFGR1, RCC_CFGR1_SW, 0x0);
    while (READ_BIT(RCC->CFGR1, RCC_CFGR1_SWS) != 0U) {
    }

    MODIFY_REG(RCC->CFGR2, RCC_CFGR2_HPRE, 0U);
    MODIFY_REG(RCC->CFGR2, RCC_CFGR2_PPRE1, 0U);
    MODIFY_REG(RCC->CFGR2, RCC_CFGR2_PPRE2, 0U);
    MODIFY_REG(RCC->CFGR2, RCC_CFGR2_PPRE3, 0U);

    // update SystemCoreClock
    SystemCoreClockUpdate();
}
//..........................................................................
static void GPIOA_out(uint32_t const pin); // prototype
static void GPIOA_out(uint32_t const pin) {
    MODIFY_REG(GPIOA->OSPEEDR,
               GPIO_OSPEEDR_OSPEED0 << (pin * GPIO_OSPEEDR_OSPEED1_Pos),
               1U << (pin * GPIO_OSPEEDR_OSPEED1_Pos)); // speed==1
    MODIFY_REG(GPIOA->OTYPER, 1U << pin,  0U << pin); // output
    MODIFY_REG(GPIOA->PUPDR,
               GPIO_PUPDR_PUPD0 << (pin * GPIO_PUPDR_PUPD1_Pos),
               0U << (pin * GPIO_PUPDR_PUPD1_Pos)); // PUSHPULL
    MODIFY_REG(GPIOA->MODER,
               GPIO_MODER_MODE0 << (pin * GPIO_MODER_MODE1_Pos),
               1U << (pin * GPIO_MODER_MODE1_Pos)); // MODE_1
}
//..........................................................................
static void GPIOC_inp(uint32_t const pin); // prototype
static void GPIOC_inp(uint32_t const pin) {
    uint32_t tmp;

    // configure IO Direction mode (input)
    tmp = GPIOC->MODER; //
    tmp &= ~(GPIO_MODER_MODE0 << (pin * GPIO_MODER_MODE1_Pos));
    GPIOC->MODER = tmp;

    // activate the Pull-up or Pull down resistor for the current IO
    tmp = GPIOC->PUPDR;
    tmp &= ~(GPIO_PUPDR_PUPD0 << (pin * GPIO_PUPDR_PUPD1_Pos));
    tmp |= (0x2U << (pin * GPIO_PUPDR_PUPD1_Pos));
    GPIOC->PUPDR = tmp;

    // EXTI mode configuration...
    tmp = EXTI->EXTICR[pin >> 2U];
    tmp &= ~((0x0FUL) << ((pin & 0x03U) * EXTI_EXTICR1_EXTI1_Pos));
    tmp |= (0x2 << ((pin & 0x03U) * EXTI_EXTICR1_EXTI1_Pos));
    EXTI->EXTICR[pin >> 2U] = tmp;

    // clear Rising Falling edge configuration
    tmp = EXTI->RTSR1;
    tmp &= ~((uint32_t)(1U << pin));
    tmp |= (1U << pin);
    EXTI->RTSR1 = tmp;

    tmp = EXTI->FTSR1;
    tmp &= ~((uint32_t)(1U << pin));
    EXTI->FTSR1 = tmp;

    // clear EXTI line configuration
    tmp = EXTI->EMR1;
    tmp &= ~((uint32_t)(1U << pin));
    EXTI->EMR1 = tmp;

    tmp = EXTI->IMR1;
    tmp &= ~((uint32_t)(1U << pin));
    tmp |= (1U << pin);
    EXTI->IMR1 = tmp;
}
//..........................................................................
void BSP_init(void const * const arg) {
    Q_UNUSED_PAR(arg);

    // setup the MPU...
    MPU_Config();

    // initialize I-CACHE
    MODIFY_REG(ICACHE->CR, ICACHE_CR_WAYSEL, 0U); // 1-way
    SET_BIT(ICACHE->CR, ICACHE_CR_EN); // enable

    // flash prefetch buffer enable
    SET_BIT(FLASH->ACR, FLASH_ACR_PRFTEN);

    // setup the clock
    SystemClock_Config();

    // NOTE: The VFP (hardware Floating Point) unit is configured by QXK

    // enable GPIOA clock port for the LD4 LED
    SET_BIT(RCC->AHB2ENR, RCC_AHB2ENR_GPIOAEN);

    // set all used GPIOA pins as push-pull output, no pull-up, pull-down
    GPIOA_out(LD2_PIN);

    // enable GPIOC clock port for the Button B1
    SET_BIT(RCC->AHB2ENR, RCC_AHB2ENR_GPIOCEN);

    // set Button B1 pin on GPIOC as input, no pull-up, pull-down
    GPIOC_inp(B1_PIN);

    // initialize QS software tracing...
    if (!QS_INIT(arg)) {
        Q_ERROR();
    }

    // QS dictionaries...
    QS_OBJ_DICTIONARY(&l_SysTick_Handler);
    QS_SIG_DICTIONARY(TIMEOUT_SIG, (void *)0);
    QS_USR_DICTIONARY(LED_STAT);

    // setup the QS filters...
    QS_GLB_FILTER(QS_GRP_ALL);  // enable all records
    QS_GLB_FILTER(-QS_QF_TICK); // exclude the tick record

    // no dynamic events -- no need to call QF_poolInit();
    // no publish-subscribe -- no need to call QActive_psInit();

    // instantiate and start AOs/threads...
    Blinky_ctor();
    static QEvtPtr blinkyQueueSto[10];
    QActive_start(AO_Blinky,
        1U,                    // QP prio. of the AO
        blinkyQueueSto,        // event queue storage
        Q_DIM(blinkyQueueSto), // queue length [events]
        (void *)0, 0U,         // no stack storage
        (void *)0);            // no initialization param
}
//............................................................................
void BSP_ledOn(void) {
    GPIOA->BSRR = (1U << LD2_PIN);  // turn LED on
    // application-specific record
    QS_BEGIN_ID(LED_STAT, AO_Blinky->prio)
        QS_STR("ON"); // LED status
    QS_END()
}
//............................................................................
void BSP_ledOff(void) {
    GPIOA->BRR = (1U << LD2_PIN);  // turn LED off
    // application-specific record
    QS_BEGIN_ID(LED_STAT, AO_Blinky->prio)
        QS_STR("OFF"); // LED status
    QS_END()
}

//============================================================================
// QF callbacks...

void QF_onStartup(void) {
    // set up the SysTick timer to fire at BSP_TICKS_PER_SEC rate
    SystemCoreClockUpdate();
    SysTick_Config((SystemCoreClock / BSP_TICKS_PER_SEC) + 1U);

    // assign all priority bits for preemption-prio. and none to sub-prio.
    NVIC_SetPriorityGrouping(0U);

    // set priorities of ALL ISRs used in the system, see NOTE1
    NVIC_SetPriority(USART2_IRQn,    0U); // kernel UNAWARE interrupt
    NVIC_SetPriority(EXTI0_IRQn,     QF_AWARE_ISR_CMSIS_PRI + 0U);
    NVIC_SetPriority(SysTick_IRQn,   QF_AWARE_ISR_CMSIS_PRI + 1U);
    // ...

    // enable IRQs...
    NVIC_EnableIRQ(EXTI0_IRQn);

#ifdef Q_SPY
    NVIC_EnableIRQ(USART2_IRQn); // UART1 interrupt used for QS-RX
#endif
}
//............................................................................
void QF_onCleanup(void) {
}

//............................................................................
void QXK_onIdle(void) {
    // toggle an LED on and then off (not enough LEDs, see NOTE2)
    //QF_INT_DISABLE();
    //GPIOA->BSRR = (1U << LD2_PIN); // turn LED[n] on
    //GPIOA->BRR  = (1U << LD2_PIN); // turn LED[n] off
    //QF_INT_ENABLE();

#ifdef Q_SPY
    QS_rxParse();  // parse all the received bytes

    // while Transmit Data Register Empty or TX-FIFO Not Full
    while ((USART2->ISR & USART_ISR_TXE_TXFNF_Msk) != 0U) {
        QF_INT_DISABLE();
        uint16_t b = QS_getByte();
        QF_INT_ENABLE();

        if (b != QS_EOD) {   // not End-Of-Data?
            USART2->TDR = b; // put into the DR register
        }
        else {
            break; // done for now
        }
    }
#elif defined NDEBUG
    // Put the CPU and peripherals to the low-power mode.
    // you might need to customize the clock management for your application,
    // see the datasheet for your particular Cortex-M MCU.
    __WFI(); // Wait-For-Interrupt
#endif
}

//============================================================================
// QS callbacks...
#ifdef Q_SPY

//............................................................................
static uint16_t const QS_UARTPrescTable[12] = {
    1U, 2U, 4U, 6U, 8U, 10U, 12U, 16U, 32U, 64U, 128U, 256U
};

#define __LL_USART_DIV_SAMPLING16(__PERIPHCLK__, __PRESCALER__, __BAUDRATE__) \
  ((((__PERIPHCLK__)/(USART_PRESCALER_TAB[(__PRESCALER__)]))\
    + ((__BAUDRATE__)/2U))/(__BAUDRATE__))

#define QS_UART_DIV_SAMPLING16(__PCLK__, __BAUD__, __CLOCKPRESCALER__) \
  ((((__PCLK__)/QS_UARTPrescTable[(__CLOCKPRESCALER__)]) \
  + ((__BAUD__)/2U)) / (__BAUD__))

// USART2 pins PA.2 and PA.3
#define USART2_TX_PIN 2U
#define USART2_RX_PIN 3U

//............................................................................
uint8_t QS_onStartup(void const *arg) {
    Q_UNUSED_PAR(arg);

    static uint8_t qsTxBuf[2*1024]; // buffer for QS-TX channel
    QS_initBuf(qsTxBuf, sizeof(qsTxBuf));

    static uint8_t qsRxBuf[100];    // buffer for QS-RX channel
    QS_rxInitBuf(qsRxBuf, sizeof(qsRxBuf));

    // enable peripheral clock for USART2 and its pins
    SET_BIT(RCC->APB1LENR, RCC_APB1LENR_USART2EN); // USART2 clock
    SET_BIT(RCC->AHB2ENR, RCC_AHB2ENR_GPIOAEN); // GPIOA clock for pins

    // Configure GPIOA to USART2_TX and USART2_RX pins.........................
    uint32_t tmp;

    // configure GPIOA to USART2_TX ...........................................
    // configure alternate function
    tmp = GPIOA->AFR[USART2_TX_PIN >> 3U];
    tmp &= ~(0x0FUL << ((USART2_TX_PIN & 0x07U) * GPIO_AFRL_AFSEL1_Pos));
    tmp |= (7U << ((USART2_TX_PIN & 0x07U) * GPIO_AFRL_AFSEL1_Pos));
    GPIOA->AFR[USART2_TX_PIN >> 3U] = tmp; // 0x700

    // configure IO Direction mode (Input, Output, Alternate or Analog)
    tmp = GPIOA->MODER;
    tmp &= ~(GPIO_MODER_MODE0 << (USART2_TX_PIN * GPIO_MODER_MODE1_Pos));
    tmp |= (2U << (USART2_TX_PIN * GPIO_MODER_MODE1_Pos));
    GPIOA->MODER = tmp;

    // configure the IO Speed
    tmp = GPIOA->OSPEEDR;
    tmp &= ~(GPIO_OSPEEDR_OSPEED0 << (USART2_TX_PIN * GPIO_OSPEEDR_OSPEED1_Pos));
    tmp |= (2U << (USART2_TX_PIN * GPIO_OSPEEDR_OSPEED1_Pos));
    GPIOA->OSPEEDR = tmp;

    // configure the IO Output Type
    tmp = GPIOA->OTYPER;
    tmp &= ~(GPIO_OTYPER_OT0 << USART2_TX_PIN) ;
    GPIOA->OTYPER = tmp;

    // activate the Pull-up or Pull down resistor for the current IO
    tmp = GPIOA->PUPDR;
    tmp &= ~(GPIO_PUPDR_PUPD0 << (USART2_TX_PIN * GPIO_PUPDR_PUPD1_Pos));
    tmp |= (1U << (USART2_TX_PIN * GPIO_PUPDR_PUPD1_Pos));
    GPIOA->PUPDR = tmp;

    // configure GPIOA to USART2_RX ...........................................

    // configure alternate function
    tmp = GPIOA->AFR[USART2_RX_PIN >> 3U];
    tmp &= ~(0xFU << ((USART2_RX_PIN & 0x07U) * GPIO_AFRL_AFSEL1_Pos));
    tmp |= (7U << ((USART2_RX_PIN & 0x7U) * GPIO_AFRL_AFSEL1_Pos));
    GPIOA->AFR[USART2_RX_PIN >> 3U] = tmp;

    // configure IO Direction mode (Input, Output, Alternate or Analog)
    tmp = GPIOA->MODER;
    tmp &= ~(GPIO_MODER_MODE0 << (USART2_RX_PIN * GPIO_MODER_MODE1_Pos));
    tmp |= (2U << (USART2_RX_PIN * GPIO_MODER_MODE1_Pos));
    GPIOA->MODER = tmp;

    // configure the IO Speed
    tmp = GPIOA->OSPEEDR;
    tmp &= ~(GPIO_OSPEEDR_OSPEED0 << (USART2_RX_PIN * GPIO_OSPEEDR_OSPEED1_Pos));
    tmp |= (2U << (USART2_RX_PIN * GPIO_OSPEEDR_OSPEED1_Pos));
    GPIOA->OSPEEDR = tmp;

    // configure the IO Output Type
    tmp = GPIOA->OTYPER;
    tmp &= ~(GPIO_OTYPER_OT0 << USART2_RX_PIN) ;
    GPIOA->OTYPER = tmp;

    // activate the Pull-up or Pull down resistor for the current IO
    tmp = GPIOA->PUPDR;
    tmp &= ~(GPIO_PUPDR_PUPD0 << (USART2_RX_PIN * GPIO_PUPDR_PUPD1_Pos));
    tmp |= (1U << (USART2_RX_PIN * GPIO_PUPDR_PUPD1_Pos));
    GPIOA->PUPDR = tmp;


    // configure USART2..............................................
    MODIFY_REG(USART2->CR1,
        (USART_CR1_M | USART_CR1_PCE | USART_CR1_PS |
         USART_CR1_TE | USART_CR1_RE | USART_CR1_OVER8),
        0U  | // data==8bits
        0U  | // parity==none
        12U | // direction==TX & RX
        0U);  // oversampling==16
    MODIFY_REG(USART2->CR2,
        USART_CR2_STOP,
        0U);  // stop-bits==1
    MODIFY_REG(USART2->CR3,
        USART_CR3_RTSE | USART_CR3_CTSE,
        0U);  // hardware-flow=NO

    // baud rate
    USART2->BRR = QS_UART_DIV_SAMPLING16(
                       SystemCoreClock, // USART2 clock
                       115200U,         // baud rate
                       0U);             // prescaler

    MODIFY_REG(USART2->PRESC,
        USART_PRESC_PRESCALER,
        0U); // prescaler=DIV1

    // FIFO thresholds
    MODIFY_REG(USART2->CR3,
        USART_CR3_TXFTCFG,
        0U << USART_CR3_TXFTCFG_Pos); // TX-FIFO=threshold-1
    MODIFY_REG(USART2->CR3,
        USART_CR3_RXFTCFG,
        0U << USART_CR3_RXFTCFG_Pos); // RX-FIFO=threshold-1

    SET_BIT(USART2->CR1, USART_CR1_FIFOEN);  // enable FIFO

    // asynch mode
    CLEAR_BIT(USART2->CR2, USART_CR2_LINEN | USART_CR2_CLKEN);
    CLEAR_BIT(USART2->CR3, USART_CR3_SCEN | USART_CR3_IREN | USART_CR3_HDSEL);

    // RXNE and RX FIFO Not Empty Interrupt Enable
    SET_BIT(USART2->CR1, USART_CR1_RXNEIE_Msk);

    SET_BIT(USART2->CR1, USART_CR1_UE); // enable USART

    // configure Timer5 for delivering QS time-stamp...........................
    SET_BIT(RCC->APB1LENR, RCC_APB1LENR_TIM5EN);

    tmp = TIM5->CR1;
    MODIFY_REG(tmp, TIM_CR1_DIR | TIM_CR1_CMS, 0U); // counter-mode=Up
    MODIFY_REG(tmp, TIM_CR1_CKD, 0U); // clock-division=1
    WRITE_REG(TIM5->CR1, tmp);

    WRITE_REG(TIM5->ARR, 0xFFFFFFFFU); // auto-reload
    WRITE_REG(TIM5->PSC, 0U); // prescaler=1
    SET_BIT(TIM5->EGR, TIM_EGR_UG); // update event to reload prescaler

    CLEAR_BIT(TIM5->CR1, TIM_CR1_ARPE); // disable ARR preload
    MODIFY_REG(TIM5->SMCR, TIM_SMCR_SMS | TIM_SMCR_ECE, 0U); // internal clock
    MODIFY_REG(TIM5->CR2, TIM_CR2_MMS, 0U); // reset timer synchronization
    CLEAR_BIT(TIM5->SMCR, TIM_SMCR_MSM); // disable master-slave

    // enable Timer5 to start time stamp
    SET_BIT(TIM5->CR1, TIM_CR1_CEN);

    return 1U; // return success
}
//............................................................................
void QS_onCleanup(void) {
}
//............................................................................
QSTimeCtr QS_onGetTime(void) { // NOTE: invoked with interrupts DISABLED
    return TIM5->CNT; // 32-bit Timer5 count
}
//............................................................................
// NOTE:
// No critical section in QS_onFlush() to avoid nesting of critical sections
// in case QS_onFlush() is called from Q_onError().
void QS_onFlush(void) {
    for (;;) {
        uint16_t const b = QS_getByte();
        if (b != QS_EOD) {
            while ((USART2->ISR & USART_ISR_TXE_Msk) == 0U) { // TXE not empty
            }
            USART2->TDR = b;
        }
        else {
            break;
        }
    }
}
//............................................................................
void QS_onReset(void) {
    NVIC_SystemReset();
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

#endif // Q_SPY

//============================================================================
// NOTE1:
// The QF_AWARE_ISR_CMSIS_PRI constant from the QF port specifies the highest
// ISR priority that is disabled by the QF framework. The value is suitable
// for the NVIC_SetPriority() CMSIS function.
//
// Only ISRs prioritized at or below the QF_AWARE_ISR_CMSIS_PRI level (i.e.,
// with the numerical values of priorities equal or higher than
// QF_AWARE_ISR_CMSIS_PRI) are allowed to call the QXK_ISR_ENTRY/
// QXK_ISR_ENTRY macros or any other QF/QXK services. These ISRs are
// "QF-aware".
//
// Conversely, any ISRs prioritized above the QF_AWARE_ISR_CMSIS_PRI priority
// level (i.e., with the numerical values of priorities less than
// QF_AWARE_ISR_CMSIS_PRI) are never disabled and are not aware of the kernel.
// Such "QF-unaware" ISRs cannot call ANY QF/QXK services. In particular they
// can NOT call the macros QXK_ISR_ENTRY/QXK_ISR_EXIT. The only mechanism
// by which a "QF-unaware" ISR can communicate with the QF framework is by
// triggering a "QF-aware" ISR, which can post/publish events.
//
// NOTE2:
// The User LED is used to visualize the idle loop activity. The brightness
// of the LED is proportional to the frequency of the idle loop.
// Please note that the LED is toggled with interrupts locked, so no interrupt
// execution time contributes to the brightness of the User LED.
//
