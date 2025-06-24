#ifndef QHSMTST_H_
#define QHSMTST_H_

enum QHsmTstSignals {
    A_SIG = Q_USER_SIG,
    B_SIG,
    C_SIG,
    D_SIG,
    E_SIG,
    F_SIG,
    G_SIG,
    H_SIG,
    I_SIG,
    X_SIG, // not used
    MAX_SIG
};

void QHsmTst_ctor(void);

// opaque pointer to the QHsmTst AO
extern QActive * const AO_QHsmTst;

#endif // QHSMTST_H_
