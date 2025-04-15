#ifndef TSTSM_SF_H_
#define TSTSM_SF_H_

extern QAsm * const the_TstSM0;
void TstSM0_ctor(void);

extern QAsm * const the_TstSM1;
void TstSM1_ctor(void);

extern QAsm * const the_TstSM2;
void TstSM2_ctor(void);

extern QAsm * const the_TstSM3;
void TstSM3_ctor(void);

extern QAsm * const the_TstSM4;
void TstSM4_ctor(void);

extern QAsm * const the_TstSM5;
void TstSM5_ctor(void);

extern QAsm * const the_TstSM6;
void TstSM6_ctor(void);

bool TstSM6_isIn(uint32_t const state_num);
QStateHandler TstSM6_child(uint32_t const  state_num);

#endif // TSTSM_SF_H_
