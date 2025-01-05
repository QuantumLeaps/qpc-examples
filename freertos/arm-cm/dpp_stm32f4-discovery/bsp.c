//============================================================================
// Product: DPP example, STM32F4-Discovery board, FreeRTOS kernel
// Last updated for version 8.0.0
// Last updated on  2024-09-18
//
//                   Q u a n t u m  L e a P s
//                   ------------------------
//                   Modern Embedded Software
//
// Copyright (C) 2005 Quantum Leaps, LLC. <state-machine.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later OR LicenseRef-QL-commercial
//
// This software is dual-licensed under the terms of the open source GNU
// General Public License version 3 (or any later version), or alternatively,
// under the terms of one of the closed source Quantum Leaps commercial
// licenses.
//
// The terms of the open source GNU General Public License version 3
// can be found at: <www.gnu.org/licenses/gpl-3.0>
//
// The terms of the closed source Quantum Leaps commercial licenses
// can be found at: <www.state-machine.com/licensing>
//
// Redistributions in source code must retain this top-level comment block.
// Plagiarizing this software to sidestep the license obligations is illegal.
//
// Contact information:
// <www.state-machine.com/licensing>
// <info@state-machine.com>
//============================================================================
#include "qpc.h"                 // QP/C real-time event framework
#include "dpp.h"                 // DPP Application interface
#include "bsp.h"                 // Board Support Package

#include "stm32f4xx.h"  // CMSIS-compliant header file for the MCU used
#include "stm32f4xx_exti.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_usart.h"
// add other drivers if necessary...

Q_DEFINE_THIS_FILE  // define the name of this file for assertions

// "RTOS-aware" interrupt priorities for FreeRTOS on ARM Cortex-M, NOTE1
#define RTOS_AWARE_ISR_CMSIS_PRI \
    (configMAX_SYSCALL_INTERRUPT_PRIORITY >> (8-__NVIC_PRIO_BITS))

// Local-scope objects -----------------------------------------------------
#define LED_GPIO_PORT     GPIOD
#define LED_GPIO_CLK      RCC_AHB1Periph_GPIOD

#define LED4_PIN          GPIO_Pin_12
#define LED3_PIN          GPIO_Pin_13
#define LED5_PIN          GPIO_Pin_14
#define LED6_PIN          GPIO_Pin_15

#define BTN_GPIO_PORT     GPIOA
#define BTN_GPIO_CLK      RCC_AHB1Periph_GPIOA
#define BTN_B1            GPIO_Pin_0

static uint32_t l_rndSeed;

#ifdef Q_SPY
    static QSTimeCtr QS_tickTime_;
    static QSTimeCtr QS_tickPeriod_;

    // QS identifiers for non-QP sources of events
    static uint8_t const l_TickHook = 0U;

    enum AppRecords { // application-specific trace records
        PHILO_STAT = QS_USER,
        PAUSED_STAT,
    };

#endif

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
    // for debugging, hang on in an endless loop...
    for (;;) {
    }
#endif
    NVIC_SystemReset();
}
//............................................................................
void assert_failed(char const * const module, int_t const id); // prototype
void assert_failed(char const * const module, int_t const id) {
    Q_onError(module, id);
}

// Application hooks used in this project ====================================
// NOTE: only the "FromISR" API variants are allowed in vApplicationTickHook

void vApplicationTickHook(void) {
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    // process time events at rate 0
    QTIMEEVT_TICK_FROM_ISR(0U, &xHigherPriorityTaskWoken, &l_TickHook);

    // Perform the debouncing of buttons. The algorithm for debouncing
    // adapted from the book "Embedded Systems Dictionary" by Jack Ganssle
    // and Michael Barr, page 71.
    static struct {
        uint32_t depressed;
        uint32_t previous;
    } buttons = { 0U, 0U };

    uint32_t current = BTN_GPIO_PORT->IDR; // read BTN GPIO
    uint32_t tmp = buttons.depressed; // save debounced depressed buttons
    buttons.depressed |= (buttons.previous & current); // set depressed
    buttons.depressed &= (buttons.previous | current); // clear released
    buttons.previous   = current; // update the history
    tmp ^= buttons.depressed;     // changed debounced depressed
    current = buttons.depressed;

    if ((tmp & BTN_B1) != 0U) { // debounced B1 state changed?
        if ((current & BTN_B1) != 0U) { // is B1 depressed?
            static QEvt const pauseEvt = QEVT_INITIALIZER(PAUSE_SIG);
            QACTIVE_PUBLISH_FROM_ISR(&pauseEvt,
                                &xHigherPriorityTaskWoken,
                                &l_TickHook);
        }
        else { // the button is released
            static QEvt const serveEvt = QEVT_INITIALIZER(SERVE_SIG);
            QACTIVE_PUBLISH_FROM_ISR(&serveEvt,
                                &xHigherPriorityTaskWoken,
                                &l_TickHook);
        }
    }

#ifdef Q_SPY
    tmp = SysTick->CTRL; // clear CTRL_COUNTFLAG
    QS_tickTime_ += QS_tickPeriod_; // account for the clock rollover
#endif

    // notify FreeRTOS to perform context switch from ISR, if needed
    portEND_SWITCHING_ISR(xHigherPriorityTaskWoken);
}

//............................................................................
void vApplicationIdleHook(void) {
    // toggle the User LED on and then off, see NOTE01
    QF_INT_DISABLE();
    BSP_ledOn();
    BSP_ledOff();
    QF_INT_ENABLE();

#ifdef Q_SPY
    QS_rxParse();  // parse all the received bytes

    if ((USART2->SR & USART_FLAG_TXE) != 0) { // TXE empty?
        QF_INT_DISABLE();
        uint16_t b = QS_getByte();
        QF_INT_ENABLE();

        if (b != QS_EOD) {  // not End-Of-Data?
            USART2->DR = b; // put into the DR register
        }
    }
#elif defined NDEBUG
    // Put the CPU and peripherals to the low-power mode.
    // you might need to customize the clock management for your application,
    // see the datasheet for your particular Cortex-M MCU.
    //
    // !!!CAUTION!!!
    // The WFI instruction stops the CPU clock, which unfortunately disables
    // the JTAG port, so the ST-Link debugger can no longer connect to the
    // board. For that reason, the call to __WFI() has to be used with CAUTION.
    //
    // NOTE: If you find your board "frozen" like this, strap BOOT0 to VDD and
    // reset the board, then connect with ST-Link Utilities and erase the part.
    // The trick with BOOT(0) is it gets the part to run the System Loader
    // instead of your broken code. When done disconnect BOOT0, and start over.
    //
    //__WFI(); // Wait-For-Interrupt
#endif
}
//............................................................................
void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName) {
    (void)xTask;
    (void)pcTaskName;
    Q_ERROR();
}
//............................................................................
// configSUPPORT_STATIC_ALLOCATION is set to 1, so the application must
// provide an implementation of vApplicationGetIdleTaskMemory() to provide
// the memory that is used by the Idle task.
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer,
                                    StackType_t **ppxIdleTaskStackBuffer,
                                    uint32_t *pulIdleTaskStackSize )
{
    // If the buffers to be provided to the Idle task are declared inside
    // this function then they must be declared static - otherwise they will
    // be allocated on the stack and so not exists after this function exits.
    //
    static StaticTask_t xIdleTaskTCB;
    static StackType_t uxIdleTaskStack[ configMINIMAL_STACK_SIZE ];

    // Pass out a pointer to the StaticTask_t structure in which the
    // Idle task's state will be stored.
    *ppxIdleTaskTCBBuffer = &xIdleTaskTCB;

    // Pass out the array that will be used as the Idle task's stack.
    *ppxIdleTaskStackBuffer = uxIdleTaskStack;

    // Pass out the size of the array pointed to by *ppxIdleTaskStackBuffer.
    // Note that, as the array is necessarily of type StackType_t,
    // configMINIMAL_STACK_SIZE is specified in words, not bytes.
    //
    *pulIdleTaskStackSize = Q_DIM(uxIdleTaskStack);
}

// BSP functions =============================================================

//............................................................................
void BSP_init(void) {
    // Configure the MPU to prevent NULL-pointer dereferencing ...
    MPU->RBAR = 0x0U                          // base address (NULL)
                | MPU_RBAR_VALID_Msk          // valid region
                | (MPU_RBAR_REGION_Msk & 7U); // region #7
    MPU->RASR = (7U << MPU_RASR_SIZE_Pos)     // 2^(7+1) region
                | (0x0U << MPU_RASR_AP_Pos)   // no-access region
                | MPU_RASR_ENABLE_Msk;        // region enable
    MPU->CTRL = MPU_CTRL_PRIVDEFENA_Msk       // enable background region
                | MPU_CTRL_ENABLE_Msk;        // enable the MPU
    __ISB();
    __DSB();

    // NOTE: SystemInit() has been already called from the startup code
    // but SystemCoreClock needs to be updated
    SystemCoreClockUpdate();

    // NOTE: VFP (hardware Floating Point) unit is configured by FreeRTOS

    // Initialize thr port for the LEDs
    RCC_AHB1PeriphClockCmd(LED_GPIO_CLK , ENABLE);

    // GPIO Configuration for the LEDs...
    GPIO_InitTypeDef GPIO_struct;
    GPIO_struct.GPIO_Mode  = GPIO_Mode_OUT;
    GPIO_struct.GPIO_OType = GPIO_OType_PP;
    GPIO_struct.GPIO_PuPd  = GPIO_PuPd_UP;
    GPIO_struct.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_struct.GPIO_Pin = LED3_PIN;
    GPIO_Init(LED_GPIO_PORT, &GPIO_struct);
    LED_GPIO_PORT->BSRRH = LED3_PIN; // turn LED off

    GPIO_struct.GPIO_Pin = LED4_PIN;
    GPIO_Init(LED_GPIO_PORT, &GPIO_struct);
    LED_GPIO_PORT->BSRRH = LED4_PIN; // turn LED off

    GPIO_struct.GPIO_Pin = LED5_PIN;
    GPIO_Init(LED_GPIO_PORT, &GPIO_struct);
    LED_GPIO_PORT->BSRRH = LED5_PIN; // turn LED off

    GPIO_struct.GPIO_Pin = LED6_PIN;
    GPIO_Init(LED_GPIO_PORT, &GPIO_struct);
    LED_GPIO_PORT->BSRRH = LED6_PIN; // turn LED off

    // Initialize thr port for Button
    RCC_AHB1PeriphClockCmd(BTN_GPIO_CLK , ENABLE);

    // GPIO Configuration for the Button...
    GPIO_struct.GPIO_Pin   = BTN_B1;
    GPIO_struct.GPIO_Mode  = GPIO_Mode_IN;
    GPIO_struct.GPIO_OType = GPIO_OType_PP;
    GPIO_struct.GPIO_PuPd  = GPIO_PuPd_DOWN;
    GPIO_struct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(BTN_GPIO_PORT, &GPIO_struct);

    BSP_randomSeed(1234U);

    // initialize the QS software tracing...
    if (!QS_INIT((void *)0)) {
        Q_ERROR();
    }

    // dictionaries...
    QS_OBJ_DICTIONARY(&l_TickHook);
    QS_USR_DICTIONARY(PHILO_STAT);
    QS_USR_DICTIONARY(PAUSED_STAT);

    QS_ONLY(produce_sig_dict());

    // setup the QS filters...
    QS_GLB_FILTER(QS_ALL_RECORDS);   // all records
    QS_GLB_FILTER(-QS_QF_TICK);      // exclude the clock tick
}
//............................................................................
void BSP_start(void) {
    // initialize event pools
    static QF_MPOOL_EL(TableEvt) smlPoolSto[2*N_PHILO]; // small pool
    QF_poolInit(smlPoolSto, sizeof(smlPoolSto), sizeof(smlPoolSto[0]));

    // initialize publish-subscribe
    static QSubscrList subscrSto[MAX_PUB_SIG];
    QActive_psInit(subscrSto, Q_DIM(subscrSto));

    // start the active objects/threads...
    static QEvtPtr philoQueueSto[N_PHILO][N_PHILO];
    static StackType_t philoStack[N_PHILO][configMINIMAL_STACK_SIZE];
    for (uint8_t n = 0U; n < N_PHILO; ++n) {
        Philo_ctor(n); // instantiate all Philosopher active objects
        QActive_setAttr(AO_Philo[n], TASK_NAME_ATTR, "Philo");
        QActive_start(AO_Philo[n],  // AO to start
            Q_PRIO(n + 3U, 3U),      // QP prio., FreeRTOS prio.
            philoQueueSto[n],        // event queue storage
            Q_DIM(philoQueueSto[n]), // queue length [events]
            philoStack[n],           // stack storage
            sizeof(philoStack[n]),   // stack size [bytes]
            (QEvt *)0);              // initialization event (not used)
    }

    static QEvtPtr tableQueueSto[N_PHILO];
    static StackType_t tableStack[configMINIMAL_STACK_SIZE];
    Table_ctor(); // instantiate the Table active object
    QActive_setAttr(AO_Table, TASK_NAME_ATTR, "Table");
    QActive_start(AO_Table,         // AO to start
        Q_PRIO(N_PHILO + 7U, 7U),    // QP prio., FreeRTOS prio.
        tableQueueSto,               // event queue storage
        Q_DIM(tableQueueSto),        // queue length [events]
        tableStack,                  // stack storage
        sizeof(tableStack),          // stack size [bytes]
        (QEvt *)0);                  // initialization event (not used)
}
//............................................................................
void BSP_displayPhilStat(uint8_t n, char const *stat) {
    Q_UNUSED_PAR(n);

    if (stat[0] == 'h') {
        LED_GPIO_PORT->BSRRL = LED3_PIN; // turn LED on
    }
    else {
        LED_GPIO_PORT->BSRRH = LED3_PIN; // turn LED off
    }
    if (stat[0] == 'e') {
        LED_GPIO_PORT->BSRRL = LED5_PIN; // turn LED on
    }
    else {
        LED_GPIO_PORT->BSRRH = LED5_PIN; // turn LED off
    }

    // app-specific trace record...
    QS_BEGIN_ID(PHILO_STAT, AO_Table->prio)
        QS_U8(1, n);  // Philosopher number
        QS_STR(stat); // Philosopher status
    QS_END()
}
//............................................................................
void BSP_displayPaused(uint8_t const paused) {
    if (paused) {
        LED_GPIO_PORT->BSRRL = LED4_PIN; // turn LED on
    }
    else {
        LED_GPIO_PORT->BSRRH = LED4_PIN; // turn LED off
    }

    // application-specific trace record
    QS_BEGIN_ID(PAUSED_STAT, AO_Table->prio)
        QS_U8(1, paused);  // Paused status
    QS_END()
}
//............................................................................
void BSP_randomSeed(uint32_t const seed) {
    l_rndSeed = seed;
}
//............................................................................
uint32_t BSP_random(void) { // a very cheap pseudo-random-number generator
    vTaskSuspendAll(); // lock FreeRTOS scheduler
    // "Super-Duper" Linear Congruential Generator (LCG)
    // LCG(2^32, 3*7*11*13*23, 0, seed)
    //
    uint32_t rnd = l_rndSeed * (3U*7U*11U*13U*23U);
    l_rndSeed = rnd; // set for the next time
    xTaskResumeAll(); // unlock the FreeRTOS scheduler

    return (rnd >> 8U);
}
//............................................................................
void BSP_ledOn(void) {
    LED_GPIO_PORT->BSRRL = LED6_PIN;
}
//............................................................................
void BSP_ledOff(void) {
    LED_GPIO_PORT->BSRRH = LED6_PIN;
}
//............................................................................
void BSP_terminate(int16_t result) {
    Q_UNUSED_PAR(result);
}

//============================================================================

// QF callbacks --------------------------------------------------------------

void QF_onStartup(void) {
    // set up the SysTick timer to fire at BSP_TICKS_PER_SEC rate
    //SysTick_Config(SystemCoreClock / BSP_TICKS_PER_SEC); // done in FreeRTOS

    // assign all priority bits for preemption-prio. and none to sub-prio.
    NVIC_SetPriorityGrouping(0U);

    // set priorities of ALL ISRs used in the system, see NOTE1
    NVIC_SetPriority(USART2_IRQn,    0U); // kernel UNAWARE interrupt
    NVIC_SetPriority(SysTick_IRQn,   RTOS_AWARE_ISR_CMSIS_PRI + 0U);
    // ...

    // enable IRQs...
#ifdef Q_SPY
    NVIC_EnableIRQ(USART2_IRQn); // USART2 interrupt used for QS-RX
#endif
}
//............................................................................
void QF_onCleanup(void) {
}

//============================================================================
// QS callbacks...
#ifdef Q_SPY

//............................................................................
#define __DIV(__PCLK, __BAUD)       (((__PCLK / 4) *25)/(__BAUD))
#define __DIVMANT(__PCLK, __BAUD)   (__DIV(__PCLK, __BAUD)/100)
#define __DIVFRAQ(__PCLK, __BAUD)   \
    (((__DIV(__PCLK, __BAUD) - (__DIVMANT(__PCLK, __BAUD) * 100)) \
        * 16 + 50) / 100)
#define __USART_BRR(__PCLK, __BAUD) \
    ((__DIVMANT(__PCLK, __BAUD) << 4)|(__DIVFRAQ(__PCLK, __BAUD) & 0x0F))

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

    // enable peripheral clock for USART2
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

    // GPIOA clock enable
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

    // GPIOA Configuration:  USART2 TX on PA2 and RX on PA3
    GPIO_InitTypeDef GPIO_struct;
    GPIO_struct.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
    GPIO_struct.GPIO_Mode = GPIO_Mode_AF;
    GPIO_struct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_struct.GPIO_OType = GPIO_OType_PP;
    GPIO_struct.GPIO_PuPd = GPIO_PuPd_UP ;
    GPIO_Init(GPIOA, &GPIO_struct);

    // Connect USART2 pins to AF2
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2); // TX = PA2
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2); // RX = PA3

    USART_InitTypeDef USART_struct;
    USART_struct.USART_BaudRate = 115200;
    USART_struct.USART_WordLength = USART_WordLength_8b;
    USART_struct.USART_StopBits = USART_StopBits_1;
    USART_struct.USART_Parity = USART_Parity_No;
    USART_struct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_struct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
    USART_Init(USART2, &USART_struct);

    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE); // enable RX interrupt
    USART_Cmd(USART2, ENABLE); // enable USART2

    QS_tickPeriod_ = SystemCoreClock / BSP_TICKS_PER_SEC;
    QS_tickTime_ = QS_tickPeriod_; // to start the timestamp at zero

    return 1U; // return success
}
//............................................................................
void QS_onCleanup(void) {
}
//............................................................................
QSTimeCtr QS_onGetTime(void) { // NOTE: invoked with interrupts DISABLED
    if ((SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) == 0U) { // not set?
        return QS_tickTime_ - (QSTimeCtr)SysTick->VAL;
    }
    else { // the rollover occurred, but the SysTick_ISR did not run yet
        return QS_tickTime_ + QS_tickPeriod_ - (QSTimeCtr)SysTick->VAL;
    }
}
//............................................................................
// NOTE:
// No critical section in QS_onFlush() to avoid nesting of critical sections
// in case QS_onFlush() is called from Q_onError().
void QS_onFlush(void) {
    for (;;) {
        uint16_t b = QS_getByte();
        if (b != QS_EOD) {
            while ((USART2->SR & USART_FLAG_TXE) == 0U) { // while TXE not empty
            }
            USART2->DR = b; // put into the DR register
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
// ISR for receiving bytes from the QSPY Back-End
// NOTE: This ISR is "QF-unaware" meaning that it does not interact with
// the QF/QK and is not disabled. Such ISRs don't need to call
// QK_ISR_ENTRY/QK_ISR_EXIT and they cannot post or publish events.

//............................................................................
void USART2_IRQHandler(void); // prototype
void USART2_IRQHandler(void) { // used in QS-RX (kernel UNAWARE interrutp)
    // is RX register NOT empty?
    if ((USART2->SR & USART_SR_RXNE) != 0U) {
        uint32_t b = USART2->DR;
        QS_RX_PUT(b);
    }
}

#endif // Q_SPY
//----------------------------------------------------------------------------

//============================================================================
// NOTE1:
// The configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY constant from the
// FreeRTOS configuration file specifies the highest ISR priority that
// is disabled by the QF framework. The value is suitable for the
// NVIC_SetPriority() CMSIS function.
//
// Only ISRs prioritized at or below the
// configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY level (i.e.,
// with the numerical values of priorities equal or higher than
// configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY) are allowed to call any
// QP/FreeRTOS services. These ISRs are "kernel-aware".
//
// Conversely, any ISRs prioritized above the
// configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY priority level (i.e., with
// the numerical values of priorities less than
// configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY) are never disabled and are
// not aware of the kernel. Such "kernel-unaware" ISRs cannot call any
// QP/FreeRTOS services. The only mechanism by which a "kernel-unaware" ISR
// can communicate with the QF framework is by triggering a "kernel-aware"
// ISR, which can post/publish events.
//
// For more information, see article "Running the RTOS on a ARM Cortex-M Core"
// http://www.freertos.org/RTOS-Cortex-M3-M4.html
//
// NOTE2:
// The User LED is used to visualize the idle loop activity. The brightness
// of the LED is proportional to the frequency of invocations of the idle loop.
// Please note that the LED is toggled with interrupts locked, so no interrupt
// execution time contributes to the brightness of the User LED.

