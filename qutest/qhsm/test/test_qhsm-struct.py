# test-script for QUTest unit testing harness
# see https://www.state-machine.com/qtools/qutest.html

# preamble...
def on_reset():
    expect_run()
    glb_filter(GRP_SM, GRP_U1)
    current_obj(OBJ_SM, "the_sm")

# tests...
#==========================================================
test("QHsmTst init")
init()
expect("===RTC===> St-Init  Obj=the_sm,State=QHsm_top->QHsmTst_s2")
expect("===RTC===> St-Entry Obj=the_sm,State=QHsmTst_s")
expect("===RTC===> St-Entry Obj=the_sm,State=QHsmTst_s2")
expect("===RTC===> St-Init  Obj=the_sm,State=QHsmTst_s2->QHsmTst_s211")
expect("===RTC===> St-Entry Obj=the_sm,State=QHsmTst_s21")
expect("===RTC===> St-Entry Obj=the_sm,State=QHsmTst_s211")
expect("@timestamp Init===> Obj=the_sm,State=QHsmTst_s211")
expect("@timestamp Trg-Done QS_RX_EVENT")

#----------------------------------------------------------
test("QHsmTst isIn", NORESET)
command(0) # get state handler
expect("@timestamp CMD QHsmTst_s211")
expect("@timestamp Trg-Done QS_RX_COMMAND")

command(1, 1) # is in state s1
expect("@timestamp CMD 0 1") # NO
expect("@timestamp Trg-Done QS_RX_COMMAND")

command(1, 2) # is in state s2
expect("@timestamp CMD 1 2") # YES
expect("@timestamp Trg-Done QS_RX_COMMAND")

command(1, 21) # is in state s21
expect("@timestamp CMD 1 21") # YES
expect("@timestamp Trg-Done QS_RX_COMMAND")

command(1, 211) # is in state s211
expect("@timestamp CMD 1 211") # YES
expect("@timestamp Trg-Done QS_RX_COMMAND")

#----------------------------------------------------------
test("QHsmTst dispatch", NORESET)
dispatch("A_SIG")
expect("@timestamp Disp===> Obj=the_sm,Sig=A_SIG,State=QHsmTst_s211")
expect("===RTC===> St-Exit  Obj=the_sm,State=QHsmTst_s211")
expect("===RTC===> St-Exit  Obj=the_sm,State=QHsmTst_s21")
expect("===RTC===> St-Entry Obj=the_sm,State=QHsmTst_s21")
expect("===RTC===> St-Init  Obj=the_sm,State=QHsmTst_s21->QHsmTst_s211")
expect("===RTC===> St-Entry Obj=the_sm,State=QHsmTst_s211")
expect("@timestamp ===>Tran Obj=the_sm,Sig=A_SIG,State=QHsmTst_s21->QHsmTst_s211")
expect("@timestamp Trg-Done QS_RX_EVENT")

dispatch("B_SIG")
expect("@timestamp Disp===> Obj=the_sm,Sig=B_SIG,State=QHsmTst_s211")
expect("===RTC===> St-Exit  Obj=the_sm,State=QHsmTst_s211")
expect("===RTC===> St-Entry Obj=the_sm,State=QHsmTst_s211")
expect("@timestamp ===>Tran Obj=the_sm,Sig=B_SIG,State=QHsmTst_s21->QHsmTst_s211")
expect("@timestamp Trg-Done QS_RX_EVENT")

dispatch("C_SIG")
expect("@timestamp Disp===> Obj=the_sm,Sig=C_SIG,State=QHsmTst_s211")
expect("===RTC===> St-Exit  Obj=the_sm,State=QHsmTst_s211")
expect("===RTC===> St-Entry Obj=the_sm,State=QHsmTst_s212")
expect("@timestamp ===>Tran Obj=the_sm,Sig=C_SIG,State=QHsmTst_s211->QHsmTst_s212")
expect("@timestamp Trg-Done QS_RX_EVENT")

dispatch("D_SIG")
expect("@timestamp Disp===> Obj=the_sm,Sig=D_SIG,State=QHsmTst_s212")
expect("===RTC===> St-Exit  Obj=the_sm,State=QHsmTst_s212")
expect("===RTC===> St-Exit  Obj=the_sm,State=QHsmTst_s21")
expect("===RTC===> St-Init  Obj=the_sm,State=QHsmTst_s2->QHsmTst_s211")
expect("===RTC===> St-Entry Obj=the_sm,State=QHsmTst_s21")
expect("===RTC===> St-Entry Obj=the_sm,State=QHsmTst_s211")
expect("@timestamp ===>Tran Obj=the_sm,Sig=D_SIG,State=QHsmTst_s21->QHsmTst_s211")
expect("@timestamp Trg-Done QS_RX_EVENT")

dispatch("E_SIG")
expect("@timestamp Disp===> Obj=the_sm,Sig=E_SIG,State=QHsmTst_s211")
expect("===RTC===> St-Exit  Obj=the_sm,State=QHsmTst_s211")
expect("===RTC===> St-Exit  Obj=the_sm,State=QHsmTst_s21")
expect("===RTC===> St-Entry Obj=the_sm,State=QHsmTst_s22")
expect("@timestamp ===>Tran Obj=the_sm,Sig=E_SIG,State=QHsmTst_s2->QHsmTst_s22")
expect("@timestamp Trg-Done QS_RX_EVENT")

dispatch("F_SIG")
expect("@timestamp Disp===> Obj=the_sm,Sig=F_SIG,State=QHsmTst_s22")
expect("===RTC===> St-Exit  Obj=the_sm,State=QHsmTst_s22")
expect("===RTC===> St-Exit  Obj=the_sm,State=QHsmTst_s2")
expect("===RTC===> St-Entry Obj=the_sm,State=QHsmTst_s1")
expect("===RTC===> St-Entry Obj=the_sm,State=QHsmTst_s11")
expect("===RTC===> St-Init  Obj=the_sm,State=QHsmTst_s11->QHsmTst_s111")
expect("===RTC===> St-Entry Obj=the_sm,State=QHsmTst_s111")
expect("@timestamp ===>Tran Obj=the_sm,Sig=F_SIG,State=QHsmTst_s2->QHsmTst_s111")
expect("@timestamp Trg-Done QS_RX_EVENT")

dispatch("H_SIG")
expect("@timestamp Disp===> Obj=the_sm,Sig=H_SIG,State=QHsmTst_s111")
expect("@timestamp =>Ignore Obj=the_sm,Sig=H_SIG,State=QHsmTst_s111")
expect("@timestamp Trg-Done QS_RX_EVENT")

dispatch("C_SIG")
expect("@timestamp Disp===> Obj=the_sm,Sig=C_SIG,State=QHsmTst_s111")
expect("===RTC===> St-Exit  Obj=the_sm,State=QHsmTst_s111")
expect("===RTC===> St-Entry Obj=the_sm,State=QHsmTst_s112")
expect("@timestamp ===>Tran Obj=the_sm,Sig=C_SIG,State=QHsmTst_s111->QHsmTst_s112")
expect("@timestamp Trg-Done QS_RX_EVENT")

dispatch("H_SIG")
expect("@timestamp Disp===> Obj=the_sm,Sig=H_SIG,State=QHsmTst_s112")
expect("===RTC===> St-Exit  Obj=the_sm,State=QHsmTst_s112")
expect("===RTC===> St-Exit  Obj=the_sm,State=QHsmTst_s11")
expect("===RTC===> St-Exit  Obj=the_sm,State=QHsmTst_s1")
expect("===RTC===> St-Init  Obj=the_sm,State=QHsmTst_s->QHsmTst_s1")
expect("===RTC===> St-Entry Obj=the_sm,State=QHsmTst_s1")
expect("===RTC===> St-Init  Obj=the_sm,State=QHsmTst_s1->QHsmTst_s111")
expect("===RTC===> St-Entry Obj=the_sm,State=QHsmTst_s11")
expect("===RTC===> St-Entry Obj=the_sm,State=QHsmTst_s111")
expect("@timestamp ===>Tran Obj=the_sm,Sig=H_SIG,State=QHsmTst_s112->QHsmTst_s111")
expect("@timestamp Trg-Done QS_RX_EVENT")

#==========================================================
test("QHsmTst guards")
init()
expect("===RTC===> St-Init  Obj=the_sm,State=QHsm_top->QHsmTst_s2")
expect("===RTC===> St-Entry Obj=the_sm,State=QHsmTst_s")
expect("===RTC===> St-Entry Obj=the_sm,State=QHsmTst_s2")
expect("===RTC===> St-Init  Obj=the_sm,State=QHsmTst_s2->QHsmTst_s211")
expect("===RTC===> St-Entry Obj=the_sm,State=QHsmTst_s21")
expect("===RTC===> St-Entry Obj=the_sm,State=QHsmTst_s211")
expect("@timestamp Init===> Obj=the_sm,State=QHsmTst_s211")
expect("@timestamp Trg-Done QS_RX_EVENT")

dispatch("D_SIG")
expect("@timestamp Disp===> Obj=the_sm,Sig=D_SIG,State=QHsmTst_s211")
expect("===RTC===> St-Exit  Obj=the_sm,State=QHsmTst_s211")
expect("===RTC===> St-Exit  Obj=the_sm,State=QHsmTst_s21")
expect("===RTC===> St-Init  Obj=the_sm,State=QHsmTst_s2->QHsmTst_s211")
expect("===RTC===> St-Entry Obj=the_sm,State=QHsmTst_s21")
expect("===RTC===> St-Entry Obj=the_sm,State=QHsmTst_s211")
expect("@timestamp ===>Tran Obj=the_sm,Sig=D_SIG,State=QHsmTst_s21->QHsmTst_s211")
expect("@timestamp Trg-Done QS_RX_EVENT")

dispatch("D_SIG")
expect("@timestamp Disp===> Obj=the_sm,Sig=D_SIG,State=QHsmTst_s211")
expect("===RTC===> St-Unhnd Obj=the_sm,Sig=D_SIG,State=QHsmTst_s21")
expect("@timestamp =>Ignore Obj=the_sm,Sig=D_SIG,State=QHsmTst_s211")
expect("@timestamp Trg-Done QS_RX_EVENT")

dispatch("I_SIG")
expect("@timestamp Disp===> Obj=the_sm,Sig=I_SIG,State=QHsmTst_s211")
expect("@timestamp =>Intern Obj=the_sm,Sig=I_SIG,State=QHsmTst_s2")
expect("@timestamp Trg-Done QS_RX_EVENT")

dispatch("I_SIG")
expect("@timestamp Disp===> Obj=the_sm,Sig=I_SIG,State=QHsmTst_s211")
expect("===RTC===> St-Unhnd Obj=the_sm,Sig=I_SIG,State=QHsmTst_s2")
expect("@timestamp =>Intern Obj=the_sm,Sig=I_SIG,State=QHsmTst_s")
expect("@timestamp Trg-Done QS_RX_EVENT")

#==========================================================
test("QHsmTst history")
init()
expect("===RTC===> St-Init  Obj=the_sm,State=QHsm_top->QHsmTst_s2")
expect("===RTC===> St-Entry Obj=the_sm,State=QHsmTst_s")
expect("===RTC===> St-Entry Obj=the_sm,State=QHsmTst_s2")
expect("===RTC===> St-Init  Obj=the_sm,State=QHsmTst_s2->QHsmTst_s211")
expect("===RTC===> St-Entry Obj=the_sm,State=QHsmTst_s21")
expect("===RTC===> St-Entry Obj=the_sm,State=QHsmTst_s211")
expect("@timestamp Init===> Obj=the_sm,State=QHsmTst_s211")
expect("@timestamp Trg-Done QS_RX_EVENT")

dispatch("G_SIG")
expect("@timestamp Disp===> Obj=the_sm,Sig=G_SIG,State=QHsmTst_s211")
expect("===RTC===> St-Exit  Obj=the_sm,State=QHsmTst_s211")
expect("===RTC===> St-Exit  Obj=the_sm,State=QHsmTst_s21")
expect("===RTC===> St-Exit  Obj=the_sm,State=QHsmTst_s2")
expect("===RTC===> St-Hist  Obj=the_sm,State=QHsmTst_s21->QHsmTst_s111")
expect("===RTC===> St-Entry Obj=the_sm,State=QHsmTst_s1")
expect("===RTC===> St-Entry Obj=the_sm,State=QHsmTst_s11")
expect("===RTC===> St-Entry Obj=the_sm,State=QHsmTst_s111")
expect("@timestamp ===>Tran Obj=the_sm,Sig=G_SIG,State=QHsmTst_s21->QHsmTst_s111")
expect("@timestamp Trg-Done QS_RX_EVENT")
command(1, 111) # is in state s111
expect("@timestamp CMD 1 111")
expect("@timestamp Trg-Done QS_RX_COMMAND")

dispatch("C_SIG")
expect("@timestamp Disp===> Obj=the_sm,Sig=C_SIG,State=QHsmTst_s111")
expect("===RTC===> St-Exit  Obj=the_sm,State=QHsmTst_s111")
expect("===RTC===> St-Entry Obj=the_sm,State=QHsmTst_s112")
expect("@timestamp ===>Tran Obj=the_sm,Sig=C_SIG,State=QHsmTst_s111->QHsmTst_s112")
expect("@timestamp Trg-Done QS_RX_EVENT")

dispatch("C_SIG")
expect("@timestamp Disp===> Obj=the_sm,Sig=C_SIG,State=QHsmTst_s112")
expect("===RTC===> St-Exit  Obj=the_sm,State=QHsmTst_s112")
expect("===RTC===> St-Exit  Obj=the_sm,State=QHsmTst_s11")
expect("===RTC===> St-Exit  Obj=the_sm,State=QHsmTst_s1")
expect("===RTC===> St-Exit  Obj=the_sm,State=QHsmTst_s")
expect("===RTC===> St-Entry Obj=the_sm,State=QHsmTst_t")
expect("@timestamp ===>Tran Obj=the_sm,Sig=C_SIG,State=QHsmTst_s->QHsmTst_t")
expect("@timestamp Trg-Done QS_RX_EVENT")

dispatch("C_SIG")
expect("@timestamp Disp===> Obj=the_sm,Sig=C_SIG,State=QHsmTst_t")
expect("===RTC===> St-Exit  Obj=the_sm,State=QHsmTst_t")
expect("===RTC===> St-Hist  Obj=the_sm,State=QHsmTst_t->QHsmTst_s1")
expect("===RTC===> St-Entry Obj=the_sm,State=QHsmTst_s")
expect("===RTC===> St-Entry Obj=the_sm,State=QHsmTst_s1")
expect("===RTC===> St-Init  Obj=the_sm,State=QHsmTst_s1->QHsmTst_s111")
expect("===RTC===> St-Entry Obj=the_sm,State=QHsmTst_s11")
expect("===RTC===> St-Entry Obj=the_sm,State=QHsmTst_s111")
expect("@timestamp ===>Tran Obj=the_sm,Sig=C_SIG,State=QHsmTst_t->QHsmTst_s111")
expect("@timestamp Trg-Done QS_RX_EVENT")

dispatch("G_SIG")
expect("@timestamp Disp===> Obj=the_sm,Sig=G_SIG,State=QHsmTst_s111")
expect("===RTC===> St-Exit  Obj=the_sm,State=QHsmTst_s111")
expect("===RTC===> St-Exit  Obj=the_sm,State=QHsmTst_s11")
expect("===RTC===> St-Exit  Obj=the_sm,State=QHsmTst_s1")
expect("===RTC===> St-Hist  Obj=the_sm,State=QHsmTst_s11->QHsmTst_s211")
expect("===RTC===> St-Entry Obj=the_sm,State=QHsmTst_s2")
expect("===RTC===> St-Entry Obj=the_sm,State=QHsmTst_s21")
expect("===RTC===> St-Entry Obj=the_sm,State=QHsmTst_s211")
expect("@timestamp ===>Tran Obj=the_sm,Sig=G_SIG,State=QHsmTst_s11->QHsmTst_s211")
expect("@timestamp Trg-Done QS_RX_EVENT")