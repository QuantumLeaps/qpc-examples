//$file${.::mine2.c} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
//
// Model: game.qm
// File:  ${.::mine2.c}
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
//$endhead${.::mine2.c} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
#include "qpc.h"
#include "bsp.h"
#include "game.h"

Q_DEFINE_THIS_FILE

// encapsulated declaration of the Mine2 HSM -------------------------------
//$declare${AOs::Mine2} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

//${AOs::Mine2} ..............................................................
typedef struct Mine2 {
// protected:
    QHsm super;

// private:
    uint8_t x;
    uint8_t y;

// public:
    uint8_t exp_ctr;
} Mine2;
extern Mine2 Mine2_inst[GAME_MINES_MAX];

// public:
static void Mine2_ctor(Mine2 * const me);

// protected:
static QState Mine2_initial(Mine2 * const me, void const * const par);
static QState Mine2_unused(Mine2 * const me, QEvt const * const e);
static QState Mine2_used(Mine2 * const me, QEvt const * const e);
static QState Mine2_planted(Mine2 * const me, QEvt const * const e);
static QState Mine2_exploding(Mine2 * const me, QEvt const * const e);
//$enddecl${AOs::Mine2} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

// helper macro to provide the ID of this mine
#define MINE_ID(me_)    ((uint8_t)((me_) - &Mine2_inst[0]))

// Mine2 class definition --------------------------------------------------
//$skip${QP_VERSION} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
// Check for the minimum required QP version
#if (QP_VERSION < 730U) || (QP_VERSION != ((QP_RELEASE^4294967295U)%0x2710U))
#error qpc version 7.3.0 or higher required
#endif
//$endskip${QP_VERSION} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
//$define${Shared::Mine2_ctor_call} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

//${Shared::Mine2_ctor_call} .................................................
QHsm * Mine2_ctor_call(uint8_t id) {
    Q_REQUIRE(id < GAME_MINES_MAX);
    Mine2_ctor(&Mine2_inst[id]);
    return &Mine2_inst[id].super;
}
//$enddef${Shared::Mine2_ctor_call} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
//$define${AOs::Mine2} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

//${AOs::Mine2} ..............................................................
Mine2 Mine2_inst[GAME_MINES_MAX];

//${AOs::Mine2::ctor} ........................................................
static void Mine2_ctor(Mine2 * const me) {
    // superclass' ctor
    QHsm_ctor(&me->super, Q_STATE_CAST(&Mine2_initial));
}

//${AOs::Mine2::SM} ..........................................................
static QState Mine2_initial(Mine2 * const me, void const * const par) {
    //${AOs::Mine2::SM::initial}
    static  uint8_t dict_sent;

    if (!dict_sent) {
        // object dictionaries for Mine2 pool...
        QS_OBJ_DICTIONARY(&Mine2_inst[0]);
        QS_OBJ_DICTIONARY(&Mine2_inst[1]);
        QS_OBJ_DICTIONARY(&Mine2_inst[2]);
        QS_OBJ_DICTIONARY(&Mine2_inst[3]);
        QS_OBJ_DICTIONARY(&Mine2_inst[4]);

        //function dictionaries for Mine2 HSM...
        QS_FUN_DICTIONARY(&Mine2_initial);
        QS_FUN_DICTIONARY(&Mine2_unused);
        QS_FUN_DICTIONARY(&Mine2_used);
        QS_FUN_DICTIONARY(&Mine2_planted);
        QS_FUN_DICTIONARY(&Mine2_exploding);

        dict_sent = 1U;
    }
    // local signals...
    QS_SIG_DICTIONARY(MINE_PLANT_SIG,    me);
    QS_SIG_DICTIONARY(MINE_DISABLED_SIG, me);
    QS_SIG_DICTIONARY(MINE_RECYCLE_SIG,  me);
    QS_SIG_DICTIONARY(SHIP_IMG_SIG,      me);
    QS_SIG_DICTIONARY(MISSILE_IMG_SIG,   me);

    (void)par; // unused parameter
    return Q_TRAN(&Mine2_unused);
}

//${AOs::Mine2::SM::unused} ..................................................
static QState Mine2_unused(Mine2 * const me, QEvt const * const e) {
    QState status_;
    switch (e->sig) {
        //${AOs::Mine2::SM::unused::MINE_PLANT}
        case MINE_PLANT_SIG: {
            me->x = Q_EVT_CAST(ObjectPosEvt)->x;
            me->y = Q_EVT_CAST(ObjectPosEvt)->y;
            status_ = Q_TRAN(&Mine2_planted);
            break;
        }
        default: {
            status_ = Q_SUPER(&QHsm_top);
            break;
        }
    }
    return status_;
}

//${AOs::Mine2::SM::used} ....................................................
static QState Mine2_used(Mine2 * const me, QEvt const * const e) {
    QState status_;
    switch (e->sig) {
        //${AOs::Mine2::SM::used}
        case Q_EXIT_SIG: {
            // tell the Tunnel that this mine is becoming disabled
            MineEvt *mev = Q_NEW(MineEvt, MINE_DISABLED_SIG);
            mev->id = MINE_ID(me);
            QACTIVE_POST(AO_Tunnel, (QEvt *)mev, me);
            status_ = Q_HANDLED();
            break;
        }
        //${AOs::Mine2::SM::used::MINE_RECYCLE}
        case MINE_RECYCLE_SIG: {
            status_ = Q_TRAN(&Mine2_unused);
            break;
        }
        default: {
            status_ = Q_SUPER(&QHsm_top);
            break;
        }
    }
    return status_;
}

//${AOs::Mine2::SM::used::planted} ...........................................
static QState Mine2_planted(Mine2 * const me, QEvt const * const e) {
    QState status_;
    switch (e->sig) {
        //${AOs::Mine2::SM::used::planted::TIME_TICK}
        case TIME_TICK_SIG: {
            //${AOs::Mine2::SM::used::planted::TIME_TICK::[me->x>=GAME_SPEED_X]}
            if (me->x >= GAME_SPEED_X) {
                ObjectImageEvt *oie;
                 me->x -= GAME_SPEED_X; // move the mine 1 step
                // tell the Tunnel to draw the Mine
                oie = Q_NEW(ObjectImageEvt, MINE_IMG_SIG);
                oie->x   = me->x;
                oie->y   = me->y;
                oie->bmp = MINE2_BMP;
                QACTIVE_POST(AO_Tunnel, (QEvt *)oie, me);
                status_ = Q_HANDLED();
            }
            //${AOs::Mine2::SM::used::planted::TIME_TICK::[else]}
            else {
                status_ = Q_TRAN(&Mine2_unused);
            }
            break;
        }
        //${AOs::Mine2::SM::used::planted::SHIP_IMG}
        case SHIP_IMG_SIG: {
            uint8_t x   = Q_EVT_CAST(ObjectImageEvt)->x;
            uint8_t y   = Q_EVT_CAST(ObjectImageEvt)->y;
            uint8_t bmp = Q_EVT_CAST(ObjectImageEvt)->bmp;
            //${AOs::Mine2::SM::used::planted::SHIP_IMG::[collisionwithMINE2_BMP?]}
            if (BSP_doBitmapsOverlap(MINE2_BMP, me->x, me->y, bmp, x, y)) {
                static MineEvt const mine1_hit = {
                    QEVT_INITIALIZER(HIT_MINE_SIG), // the QEvt base instance
                    2U  // type of the mine (2 for Mine type-2)
                };
                QACTIVE_POST(AO_Ship, (QEvt *)&mine1_hit, me);
                // go straight to 'disabled' and let the Ship do the exploding
                status_ = Q_TRAN(&Mine2_unused);
            }
            else {
                status_ = Q_UNHANDLED();
            }
            break;
        }
        //${AOs::Mine2::SM::used::planted::MISSILE_IMG}
        case MISSILE_IMG_SIG: {
            uint8_t x   = Q_EVT_CAST(ObjectImageEvt)->x;
            uint8_t y   = Q_EVT_CAST(ObjectImageEvt)->y;
            uint8_t bmp = Q_EVT_CAST(ObjectImageEvt)->bmp;
            //${AOs::Mine2::SM::used::planted::MISSILE_IMG::[collisionwithMINE2_MISSILE_BMP?~}
            if (BSP_doBitmapsOverlap(MINE2_MISSILE_BMP, me->x, me->y, bmp, x, y)) {
                // NOTE: Mine type-2 is nastier than Mine type-1.
                // The type-2 mine can hit the Ship with any of its
                // "tentacles". However, it can be destroyed by the
                // Missile only by hitting its center, defined as
                // a smaller bitmap MINE2_MISSILE_BMP.
                static ScoreEvt const mine2_destroyed = {
                    QEVT_INITIALIZER(DESTROYED_MINE_SIG),  // the QEvt base instance
                    45U  // score for destroying Mine type-2
                };
                QACTIVE_POST(AO_Missile, (QEvt *)&mine2_destroyed, me);
                status_ = Q_TRAN(&Mine2_exploding);
            }
            else {
                status_ = Q_UNHANDLED();
            }
            break;
        }
        default: {
            status_ = Q_SUPER(&Mine2_used);
            break;
        }
    }
    return status_;
}

//${AOs::Mine2::SM::used::exploding} .........................................
static QState Mine2_exploding(Mine2 * const me, QEvt const * const e) {
    QState status_;
    switch (e->sig) {
        //${AOs::Mine2::SM::used::exploding}
        case Q_ENTRY_SIG: {
            me->exp_ctr = 0U;
            status_ = Q_HANDLED();
            break;
        }
        //${AOs::Mine2::SM::used::exploding::TIME_TICK}
        case TIME_TICK_SIG: {
            //${AOs::Mine2::SM::used::exploding::TIME_TICK::[stillonscreen?]}
            if ((me->x >= GAME_SPEED_X) && (me->exp_ctr < 15U)) {
                ObjectImageEvt *oie;
                ++me->exp_ctr;  // advance the explosion counter
                 me->x -= GAME_SPEED_X; // move explosion by 1 step

                // tell the Game to render the current stage of Explosion
                oie = Q_NEW(ObjectImageEvt, EXPLOSION_SIG);
                oie->x   = me->x + 1U;  // x of explosion
                oie->y   = (int8_t)((int)me->y - 4 + 2); // y of explosion
                oie->bmp = EXPLOSION0_BMP + (me->exp_ctr >> 2);
                QACTIVE_POST(AO_Tunnel, (QEvt *)oie, me);
                status_ = Q_HANDLED();
            }
            //${AOs::Mine2::SM::used::exploding::TIME_TICK::[else]}
            else {
                status_ = Q_TRAN(&Mine2_unused);
            }
            break;
        }
        default: {
            status_ = Q_SUPER(&Mine2_used);
            break;
        }
    }
    return status_;
}
//$enddef${AOs::Mine2} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
