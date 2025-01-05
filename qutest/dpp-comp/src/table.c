//$file${.::table.c} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
//
// Model: dpp-comp.qm
// File:  ${.::table.c}
//
// This code has been generated by QM 7.0.1 <www.state-machine.com/qm>.
// DO NOT EDIT THIS FILE MANUALLY. All your changes will be lost.
//
// Copyright (c) 2005 Quantum Leaps, LLC. All rights reserved.
//
//                 ____________________________________
//                /                                   /
//               /    GGGGGGG    PPPPPPPP   LL       /
//              /   GG     GG   PP     PP  LL       /
//             /   GG          PP     PP  LL       /
//            /   GG   GGGGG  PPPPPPPP   LL       /
//           /   GG      GG  PP         LL       /
//          /     GGGGGGG   PP         LLLLLLL  /
//         /___________________________________/
//
// SPDX-License-Identifier: GPL-3.0-or-later
//
// This generated code is open-source software licensed under the GNU
// General Public License (GPL) as published by the Free Software Foundation
// (see <https://www.gnu.org/licenses>).
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
//
//$endhead${.::table.c} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
#include "qpc.h"
#include "dpp.h"
#include "bsp.h"

Q_DEFINE_THIS_FILE

// helper function to provide the RIGHT neighbor of a Philo[n]
static inline uint8_t right(uint8_t const n) {
    return (uint8_t)((n + (N_PHILO - 1U)) % N_PHILO);
}

// helper function to provide the LEFT neighbor of a Philo[n]
static inline uint8_t left(uint8_t const n) {
    return (uint8_t)((n + 1U) % N_PHILO);
}

#define FREE ((uint8_t)0U)
#define USED ((uint8_t)1U)

#define THINKING "thinking"
#define HUNGRY   "hungry  "
#define EATING   "eating  "

//$declare${Cont::Table} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

//${Cont::Table} .............................................................
typedef struct Table {
// protected:
    QActive super;

// public:

// private:
    uint8_t fork[N_PHILO];
    uint8_t isHungry[N_PHILO];
} Table;
extern Table Table_inst;

// protected:
static QState Table_initial(Table * const me, void const * const par);
static QState Table_active(Table * const me, QEvt const * const e);
static QState Table_serving(Table * const me, QEvt const * const e);
static QState Table_paused(Table * const me, QEvt const * const e);
//$enddecl${Cont::Table} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

//$skip${QP_VERSION} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
// Check for the minimum required QP version
#if (QP_VERSION < 730U) || (QP_VERSION != ((QP_RELEASE^4294967295U)%0x2710U))
#error qpc version 7.3.0 or higher required
#endif
//$endskip${QP_VERSION} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
//$define${Shared::Table_ctor} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

//${Shared::Table_ctor} ......................................................
void Table_ctor(void) {
    Table *me = &Table_inst;
    QActive_ctor(&me->super, Q_STATE_CAST(&Table_initial));

    for (uint8_t n = 0U; n < N_PHILO; ++n) {
        Philo_ctor(n); // create the Philo component

        me->fork[n] = FREE;
        me->isHungry[n] = 0U;
    }
}
//$enddef${Shared::Table_ctor} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
//$define${Shared::AO_Table} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

//${Shared::AO_Table} ........................................................
QActive * const AO_Table = &Table_inst.super;
//$enddef${Shared::AO_Table} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
//$define${Cont} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

//${Cont::Table} .............................................................
Table Table_inst;

//${Cont::Table::SM} .........................................................
static QState Table_initial(Table * const me, void const * const par) {
    //${Cont::Table::SM::initial}
    Q_UNUSED_PAR(par);

    QS_OBJ_DICTIONARY(&Table_inst);

    QActive_subscribe(&me->super, PAUSE_SIG);
    QActive_subscribe(&me->super, SERVE_SIG);
    QActive_subscribe(&me->super, TEST_SIG);

    for (uint8_t n = 0U; n < N_PHILO; ++n) {
        me->fork[n] = FREE;
        me->isHungry[n] = 0U;

        QASM_INIT(SM_Philo[n], (void *)0, QS_AP_ID + n);
        BSP_displayPhilStat(n, "thinking");
    }

    QS_FUN_DICTIONARY(&Table_active);
    QS_FUN_DICTIONARY(&Table_serving);
    QS_FUN_DICTIONARY(&Table_paused);

    return Q_TRAN(&Table_serving);
}

//${Cont::Table::SM::active} .................................................
static QState Table_active(Table * const me, QEvt const * const e) {
    QState status_;
    switch (e->sig) {
        //${Cont::Table::SM::active::TIMEOUT}
        case TIMEOUT_SIG: {
            uint16_t n = Q_EVT_CAST(CompTimeEvt)->compNum;
            QASM_DISPATCH(SM_Philo[n], e, QS_AP_ID + n);
            status_ = Q_HANDLED();
            break;
        }
        //${Cont::Table::SM::active::EAT}
        case EAT_SIG: {
            Q_ERROR();
            status_ = Q_HANDLED();
            break;
        }
        //${Cont::Table::SM::active::TEST}
        case TEST_SIG: {
            status_ = Q_HANDLED();
            break;
        }
        default: {
            status_ = Q_SUPER(&QHsm_top);
            break;
        }
    }
    return status_;
}

//${Cont::Table::SM::active::serving} ........................................
static QState Table_serving(Table * const me, QEvt const * const e) {
    QState status_;
    switch (e->sig) {
        //${Cont::Table::SM::active::serving}
        case Q_ENTRY_SIG: {
            for (uint8_t n = 0U; n < N_PHILO; ++n) { // give permissions to eat...
                if ((me->isHungry[n] != 0U)
                    && (me->fork[left(n)] == FREE)
                    && (me->fork[n] == FREE))
                {
                    me->fork[left(n)] = USED;
                    me->fork[n] = USED;

                    // synchronously dispatch EAT event to the Philo component
                    TableEvt const evt = {
                        QEVT_INITIALIZER(EAT_SIG),
                        .philoId = n
                    };
                    QASM_DISPATCH(SM_Philo[n], &evt.super, QS_AP_ID + n);

                    me->isHungry[n] = 0U;
                    BSP_displayPhilStat(n, "eating  ");
                }
            }
            status_ = Q_HANDLED();
            break;
        }
        //${Cont::Table::SM::active::serving::HUNGRY}
        case HUNGRY_SIG: {
            // find the index of the Philo from the event
            uint8_t n = Q_EVT_CAST(TableEvt)->philoId;
            // philo ID must be in range and he must be not hungry
            Q_ASSERT((n < N_PHILO) && (!me->isHungry[n]));

            BSP_displayPhilStat(n, "hungry  ");
            uint8_t m = left(n);
            //${Cont::Table::SM::active::serving::HUNGRY::[bothfree]}
            if ((me->fork[m] == FREE) && (me->fork[n] == FREE)) {
                me->fork[m] = USED;
                me->fork[n] = USED;

                // synchronously dispatch EAT event to the Philo component
                TableEvt const evt = {
                    QEVT_INITIALIZER(EAT_SIG),
                    .philoId = n
                };
                QASM_DISPATCH(SM_Philo[n], &evt.super, QS_AP_ID + n);
                BSP_displayPhilStat(n, "eating  ");
                status_ = Q_HANDLED();
            }
            //${Cont::Table::SM::active::serving::HUNGRY::[else]}
            else {
                me->isHungry[n] = 1U;
                status_ = Q_HANDLED();
            }
            break;
        }
        //${Cont::Table::SM::active::serving::DONE}
        case DONE_SIG: {
            // find the index of the Philo from the event
            uint8_t n = Q_EVT_CAST(TableEvt)->philoId;
            // philo ID must be in range and he must be not hungry
            Q_ASSERT((n < N_PHILO) && (!me->isHungry[n]));

            BSP_displayPhilStat(n, "thinking");
            uint8_t m = left(n);
            // both forks of Phil[n] must be used
            Q_ASSERT((me->fork[n] == USED) && (me->fork[m] == USED));

            me->fork[m] = FREE;
            me->fork[n] = FREE;
            m = right(n); // check the right neighbor

            if ((me->isHungry[m] != 0U) && (me->fork[m] == FREE)) {
                me->fork[n] = USED;
                me->fork[m] = USED;
                me->isHungry[m] = 0U;

                // synchronously dispatch EAT event to the Philo component
                TableEvt const evt = {
                    QEVT_INITIALIZER(EAT_SIG),
                    .philoId = m
                };
                QASM_DISPATCH(SM_Philo[m], &evt.super, QS_AP_ID + m);
                BSP_displayPhilStat(m, "eating  ");
            }
            m = left(n); // check the left neighbor
            n = left(m); // left fork of the left neighbor
            if ((me->isHungry[m] != 0U) && (me->fork[n] == FREE)) {
                me->fork[m] = USED;
                me->fork[n] = USED;
                me->isHungry[m] = 0U;

                // synchronously dispatch EAT event to the Philo component
                TableEvt const evt = {
                    QEVT_INITIALIZER(EAT_SIG),
                    .philoId = m
                };
                QASM_DISPATCH(SM_Philo[m], &evt.super, QS_AP_ID + m);
                BSP_displayPhilStat(m, "eating  ");
            }
            status_ = Q_HANDLED();
            break;
        }
        //${Cont::Table::SM::active::serving::EAT}
        case EAT_SIG: {
            Q_ERROR();
            status_ = Q_HANDLED();
            break;
        }
        //${Cont::Table::SM::active::serving::PAUSE}
        case PAUSE_SIG: {
            status_ = Q_TRAN(&Table_paused);
            break;
        }
        default: {
            status_ = Q_SUPER(&Table_active);
            break;
        }
    }
    return status_;
}

//${Cont::Table::SM::active::paused} .........................................
static QState Table_paused(Table * const me, QEvt const * const e) {
    QState status_;
    switch (e->sig) {
        //${Cont::Table::SM::active::paused}
        case Q_ENTRY_SIG: {
            BSP_displayPaused(1U);
            status_ = Q_HANDLED();
            break;
        }
        //${Cont::Table::SM::active::paused}
        case Q_EXIT_SIG: {
            BSP_displayPaused(0U);
            status_ = Q_HANDLED();
            break;
        }
        //${Cont::Table::SM::active::paused::SERVE}
        case SERVE_SIG: {
            status_ = Q_TRAN(&Table_serving);
            break;
        }
        //${Cont::Table::SM::active::paused::HUNGRY}
        case HUNGRY_SIG: {
            // find the index of the Philo from the event
            uint8_t n = Q_EVT_CAST(TableEvt)->philoId;
            // philo ID must be in range and he must be not hungry
            Q_ASSERT((n < N_PHILO) && (!me->isHungry[n]));

            me->isHungry[n] = 1U;
            BSP_displayPhilStat(n, "hungry  ");
            status_ = Q_HANDLED();
            break;
        }
        //${Cont::Table::SM::active::paused::DONE}
        case DONE_SIG: {
            // find the index of the Philo from the event
            uint8_t n = Q_EVT_CAST(TableEvt)->philoId;
            // philo ID must be in range and he must be not hungry
            Q_ASSERT((n < N_PHILO) && (!me->isHungry[n]));

            BSP_displayPhilStat(n, "thinking");
            uint8_t m = left(n);
            // both forks of Phil[n] must be used
            Q_ASSERT((me->fork[n] == USED) && (me->fork[m] == USED));

            me->fork[m] = FREE;
            me->fork[n] = FREE;
            status_ = Q_HANDLED();
            break;
        }
        default: {
            status_ = Q_SUPER(&Table_active);
            break;
        }
    }
    return status_;
}
//$enddef${Cont} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
