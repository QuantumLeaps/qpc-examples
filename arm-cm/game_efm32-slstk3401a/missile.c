//$file${.::missile.c} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
//
// Model: game.qm
// File:  ${.::missile.c}
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
//$endhead${.::missile.c} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
#include "qpc.h"
#include "bsp.h"
#include "game.h"

// Q_DEFINE_THIS_FILE

// local objects -----------------------------------------------------------
//$declare${AOs::Missile} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

//${AOs::Missile} ............................................................
typedef struct Missile {
// protected:
    QActive super;

// private:
    uint8_t x;
    uint8_t y;
    uint8_t exp_ctr;

// public:
} Missile;
extern Missile Missile_inst;

// public:
static void Missile_ctor(Missile * const me);

// protected:
static QState Missile_initial(Missile * const me, void const * const par);
static QState Missile_armed(Missile * const me, QEvt const * const e);
static QState Missile_flying(Missile * const me, QEvt const * const e);
static QState Missile_exploding(Missile * const me, QEvt const * const e);
//$enddecl${AOs::Missile} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

// Public-scope objects ----------------------------------------------------
//$skip${QP_VERSION} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
// Check for the minimum required QP version
#if (QP_VERSION < 730U) || (QP_VERSION != ((QP_RELEASE^4294967295U)%0x2710U))
#error qpc version 7.3.0 or higher required
#endif
//$endskip${QP_VERSION} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
//$define${Shared::AO_Missile} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

//${Shared::AO_Missile} ......................................................
QActive * const AO_Missile = &Missile_inst.super;
//$enddef${Shared::AO_Missile} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

// Active object definition ------------------------------------------------
//$define${Shared::Missile_ctor_call} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

//${Shared::Missile_ctor_call} ...............................................
void Missile_ctor_call(void) {
    Missile_ctor(&Missile_inst);
}
//$enddef${Shared::Missile_ctor_call} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
//$define${AOs::Missile} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

//${AOs::Missile} ............................................................
Missile Missile_inst;

//${AOs::Missile::ctor} ......................................................
static void Missile_ctor(Missile * const me) {
    QActive_ctor(&me->super, Q_STATE_CAST(&Missile_initial));
}

//${AOs::Missile::SM} ........................................................
static QState Missile_initial(Missile * const me, void const * const par) {
    //${AOs::Missile::SM::initial}
    (void)par; // unused parameter
    QActive_subscribe(&me->super, TIME_TICK_SIG);

    // local signals
    QS_SIG_DICTIONARY(MISSILE_FIRE_SIG,   me);
    QS_SIG_DICTIONARY(HIT_WALL_SIG,       me);
    QS_SIG_DICTIONARY(DESTROYED_MINE_SIG, me);

    QS_FUN_DICTIONARY(&Missile_armed);
    QS_FUN_DICTIONARY(&Missile_flying);
    QS_FUN_DICTIONARY(&Missile_exploding);

    return Q_TRAN(&Missile_armed);
}

//${AOs::Missile::SM::armed} .................................................
static QState Missile_armed(Missile * const me, QEvt const * const e) {
    QState status_;
    switch (e->sig) {
        //${AOs::Missile::SM::armed::MISSILE_FIRE}
        case MISSILE_FIRE_SIG: {
            me->x = Q_EVT_CAST(ObjectPosEvt)->x;
            me->y = Q_EVT_CAST(ObjectPosEvt)->y;
            status_ = Q_TRAN(&Missile_flying);
            break;
        }
        default: {
            status_ = Q_SUPER(&QHsm_top);
            break;
        }
    }
    return status_;
}

//${AOs::Missile::SM::flying} ................................................
static QState Missile_flying(Missile * const me, QEvt const * const e) {
    QState status_;
    switch (e->sig) {
        //${AOs::Missile::SM::flying::TIME_TICK}
        case TIME_TICK_SIG: {
            //${AOs::Missile::SM::flying::TIME_TICK::[me->x+GAME_MISSILE_SPEED_X<GAME~}
            if (me->x + GAME_MISSILE_SPEED_X < GAME_TUNNEL_WIDTH) {
                ObjectImageEvt *oie;
                me->x += GAME_MISSILE_SPEED_X;
                //tell the Tunnel to draw the Missile and test for wall hits
                oie = Q_NEW(ObjectImageEvt, MISSILE_IMG_SIG);
                oie->x   = me->x;
                oie->y   = me->y;
                oie->bmp = MISSILE_BMP;
                QACTIVE_POST(AO_Tunnel, (QEvt *)oie, me);
                status_ = Q_HANDLED();
            }
            //${AOs::Missile::SM::flying::TIME_TICK::[else]}
            else {
                status_ = Q_TRAN(&Missile_armed);
            }
            break;
        }
        //${AOs::Missile::SM::flying::HIT_WALL}
        case HIT_WALL_SIG: {
            status_ = Q_TRAN(&Missile_exploding);
            break;
        }
        //${AOs::Missile::SM::flying::DESTROYED_MINE}
        case DESTROYED_MINE_SIG: {
            QACTIVE_POST(AO_Ship, e, me);
            status_ = Q_TRAN(&Missile_armed);
            break;
        }
        default: {
            status_ = Q_SUPER(&QHsm_top);
            break;
        }
    }
    return status_;
}

//${AOs::Missile::SM::exploding} .............................................
static QState Missile_exploding(Missile * const me, QEvt const * const e) {
    QState status_;
    switch (e->sig) {
        //${AOs::Missile::SM::exploding}
        case Q_ENTRY_SIG: {
            me->exp_ctr = 0U;
            status_ = Q_HANDLED();
            break;
        }
        //${AOs::Missile::SM::exploding::TIME_TICK}
        case TIME_TICK_SIG: {
            //${AOs::Missile::SM::exploding::TIME_TICK::[(me->x>=GAME_SPEED_X)&&(me->exp~}
            if ((me->x >= GAME_SPEED_X) && (me->exp_ctr < 15U)) {
                ObjectImageEvt *oie;

                ++me->exp_ctr;           // advance the explosion counter
                me->x -= GAME_SPEED_X;   // move the explosion by one step

                // tell the Tunnel to render the current stage of Explosion
                oie = Q_NEW(ObjectImageEvt, EXPLOSION_SIG);
                oie->x   = me->x + 3U;   // x-pos of explosion
                oie->y   = (int8_t)((int)me->y - 4U); // y-pos
                oie->bmp = EXPLOSION0_BMP + (me->exp_ctr >> 2);
                QACTIVE_POST(AO_Tunnel, (QEvt *)oie, me);
                status_ = Q_HANDLED();
            }
            //${AOs::Missile::SM::exploding::TIME_TICK::[else]}
            else {
                status_ = Q_TRAN(&Missile_armed);
            }
            break;
        }
        default: {
            status_ = Q_SUPER(&QHsm_top);
            break;
        }
    }
    return status_;
}
//$enddef${AOs::Missile} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
