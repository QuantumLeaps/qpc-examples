# test-script for QUTest unit testing harness
# see https://www.state-machine.com/qtools/qutest.html

# preamble...
def on_reset():
    expect_run()
    glb_filter(GRP_SM, GRP_U1)
    current_obj(OBJ_SM, "the_sm")

# tests...
#==========================================================
test("TstSM init")
init()
expect("===RTC===> St-Init  Obj=the_sm,State=NULL->TstSM_s2")
expect("===RTC===> St-Entry Obj=the_sm,State=TstSM_s")
expect("===RTC===> St-Entry Obj=the_sm,State=TstSM_s2")
expect("===RTC===> St-Init  Obj=the_sm,State=TstSM_s2->TstSM_s211")
expect("===RTC===> St-Entry Obj=the_sm,State=TstSM_s21")
expect("===RTC===> St-Entry Obj=the_sm,State=TstSM_s211")
expect("@timestamp Init===> Obj=the_sm,State=TstSM_s211")
expect("@timestamp Trg-Done QS_RX_EVENT")

#----------------------------------------------------------
test("TstSM isIn", NORESET)
command(0) # get state handler
expect("@timestamp CMD TstSM_s211")
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
test("TstSM dispatch", NORESET)
dispatch("A_SIG")
expect("@timestamp Disp===> Obj=the_sm,Sig=A_SIG,State=TstSM_s211")
expect("===RTC===> St-Exit  Obj=the_sm,State=TstSM_s211")
expect("===RTC===> St-Exit  Obj=the_sm,State=TstSM_s21")
expect("===RTC===> St-Entry Obj=the_sm,State=TstSM_s21")
expect("===RTC===> St-Init  Obj=the_sm,State=TstSM_s21->TstSM_s211")
expect("===RTC===> St-Entry Obj=the_sm,State=TstSM_s211")
expect("@timestamp ===>Tran Obj=the_sm,Sig=A_SIG,State=TstSM_s21->TstSM_s211")
expect("@timestamp Trg-Done QS_RX_EVENT")

dispatch("B_SIG")
expect("@timestamp Disp===> Obj=the_sm,Sig=B_SIG,State=TstSM_s211")
expect("===RTC===> St-Exit  Obj=the_sm,State=TstSM_s211")
expect("===RTC===> St-Entry Obj=the_sm,State=TstSM_s211")
expect("@timestamp ===>Tran Obj=the_sm,Sig=B_SIG,State=TstSM_s21->TstSM_s211")
expect("@timestamp Trg-Done QS_RX_EVENT")

dispatch("C_SIG")
expect("@timestamp Disp===> Obj=the_sm,Sig=C_SIG,State=TstSM_s211")
expect("===RTC===> St-Exit  Obj=the_sm,State=TstSM_s211")
expect("===RTC===> St-Entry Obj=the_sm,State=TstSM_s212")
expect("@timestamp ===>Tran Obj=the_sm,Sig=C_SIG,State=TstSM_s211->TstSM_s212")
expect("@timestamp Trg-Done QS_RX_EVENT")

dispatch("D_SIG")
expect("@timestamp Disp===> Obj=the_sm,Sig=D_SIG,State=TstSM_s212")
expect("===RTC===> St-Exit  Obj=the_sm,State=TstSM_s212")
expect("===RTC===> St-Exit  Obj=the_sm,State=TstSM_s21")
expect("===RTC===> St-Init  Obj=the_sm,State=TstSM_s2->TstSM_s211")
expect("===RTC===> St-Entry Obj=the_sm,State=TstSM_s21")
expect("===RTC===> St-Entry Obj=the_sm,State=TstSM_s211")
expect("@timestamp ===>Tran Obj=the_sm,Sig=D_SIG,State=TstSM_s21->TstSM_s211")
expect("@timestamp Trg-Done QS_RX_EVENT")

dispatch("E_SIG")
expect("@timestamp Disp===> Obj=the_sm,Sig=E_SIG,State=TstSM_s211")
expect("===RTC===> St-Exit  Obj=the_sm,State=TstSM_s211")
expect("===RTC===> St-Exit  Obj=the_sm,State=TstSM_s21")
expect("===RTC===> St-Entry Obj=the_sm,State=TstSM_s22")
expect("@timestamp ===>Tran Obj=the_sm,Sig=E_SIG,State=TstSM_s2->TstSM_s22")
expect("@timestamp Trg-Done QS_RX_EVENT")

dispatch("F_SIG")
expect("@timestamp Disp===> Obj=the_sm,Sig=F_SIG,State=TstSM_s22")
expect("===RTC===> St-Exit  Obj=the_sm,State=TstSM_s22")
expect("===RTC===> St-Exit  Obj=the_sm,State=TstSM_s2")
expect("===RTC===> St-Entry Obj=the_sm,State=TstSM_s1")
expect("===RTC===> St-Entry Obj=the_sm,State=TstSM_s11")
expect("===RTC===> St-Init  Obj=the_sm,State=TstSM_s11->TstSM_s111")
expect("===RTC===> St-Entry Obj=the_sm,State=TstSM_s111")
expect("@timestamp ===>Tran Obj=the_sm,Sig=F_SIG,State=TstSM_s2->TstSM_s111")
expect("@timestamp Trg-Done QS_RX_EVENT")

dispatch("H_SIG")
expect("@timestamp Disp===> Obj=the_sm,Sig=H_SIG,State=TstSM_s111")
expect("@timestamp =>Ignore Obj=the_sm,Sig=H_SIG,State=TstSM_s111")
expect("@timestamp Trg-Done QS_RX_EVENT")

dispatch("C_SIG")
expect("@timestamp Disp===> Obj=the_sm,Sig=C_SIG,State=TstSM_s111")
expect("===RTC===> St-Exit  Obj=the_sm,State=TstSM_s111")
expect("===RTC===> St-Entry Obj=the_sm,State=TstSM_s112")
expect("@timestamp ===>Tran Obj=the_sm,Sig=C_SIG,State=TstSM_s111->TstSM_s112")
expect("@timestamp Trg-Done QS_RX_EVENT")

dispatch("H_SIG")
expect("@timestamp Disp===> Obj=the_sm,Sig=H_SIG,State=TstSM_s112")
expect("===RTC===> St-Exit  Obj=the_sm,State=TstSM_s112")
expect("===RTC===> St-Exit  Obj=the_sm,State=TstSM_s11")
expect("===RTC===> St-Exit  Obj=the_sm,State=TstSM_s1")
expect("===RTC===> St-Init  Obj=the_sm,State=TstSM_s->TstSM_s1")
expect("===RTC===> St-Entry Obj=the_sm,State=TstSM_s1")
expect("===RTC===> St-Init  Obj=the_sm,State=TstSM_s1->TstSM_s111")
expect("===RTC===> St-Entry Obj=the_sm,State=TstSM_s11")
expect("===RTC===> St-Entry Obj=the_sm,State=TstSM_s111")
expect("@timestamp ===>Tran Obj=the_sm,Sig=H_SIG,State=TstSM_s112->TstSM_s111")
expect("@timestamp Trg-Done QS_RX_EVENT")

#==========================================================
test("TstSM guards")
init()
expect("===RTC===> St-Init  Obj=the_sm,State=NULL->TstSM_s2")
expect("===RTC===> St-Entry Obj=the_sm,State=TstSM_s")
expect("===RTC===> St-Entry Obj=the_sm,State=TstSM_s2")
expect("===RTC===> St-Init  Obj=the_sm,State=TstSM_s2->TstSM_s211")
expect("===RTC===> St-Entry Obj=the_sm,State=TstSM_s21")
expect("===RTC===> St-Entry Obj=the_sm,State=TstSM_s211")
expect("@timestamp Init===> Obj=the_sm,State=TstSM_s211")
expect("@timestamp Trg-Done QS_RX_EVENT")

dispatch("D_SIG")
expect("@timestamp Disp===> Obj=the_sm,Sig=D_SIG,State=TstSM_s211")
expect("===RTC===> St-Exit  Obj=the_sm,State=TstSM_s211")
expect("===RTC===> St-Exit  Obj=the_sm,State=TstSM_s21")
expect("===RTC===> St-Init  Obj=the_sm,State=TstSM_s2->TstSM_s211")
expect("===RTC===> St-Entry Obj=the_sm,State=TstSM_s21")
expect("===RTC===> St-Entry Obj=the_sm,State=TstSM_s211")
expect("@timestamp ===>Tran Obj=the_sm,Sig=D_SIG,State=TstSM_s21->TstSM_s211")
expect("@timestamp Trg-Done QS_RX_EVENT")

dispatch("D_SIG")
expect("@timestamp Disp===> Obj=the_sm,Sig=D_SIG,State=TstSM_s211")
expect("===RTC===> St-Unhnd Obj=the_sm,Sig=D_SIG,State=TstSM_s21")
expect("@timestamp =>Ignore Obj=the_sm,Sig=D_SIG,State=TstSM_s211")
expect("@timestamp Trg-Done QS_RX_EVENT")

dispatch("I_SIG")
expect("@timestamp Disp===> Obj=the_sm,Sig=I_SIG,State=TstSM_s211")
expect("@timestamp =>Intern Obj=the_sm,Sig=I_SIG,State=TstSM_s2")
expect("@timestamp Trg-Done QS_RX_EVENT")

dispatch("I_SIG")
expect("@timestamp Disp===> Obj=the_sm,Sig=I_SIG,State=TstSM_s211")
expect("===RTC===> St-Unhnd Obj=the_sm,Sig=I_SIG,State=TstSM_s2")
expect("@timestamp =>Intern Obj=the_sm,Sig=I_SIG,State=TstSM_s")
expect("@timestamp Trg-Done QS_RX_EVENT")

#==========================================================
test("TstSM history")
init()
expect("===RTC===> St-Init  Obj=the_sm,State=NULL->TstSM_s2")
expect("===RTC===> St-Entry Obj=the_sm,State=TstSM_s")
expect("===RTC===> St-Entry Obj=the_sm,State=TstSM_s2")
expect("===RTC===> St-Init  Obj=the_sm,State=TstSM_s2->TstSM_s211")
expect("===RTC===> St-Entry Obj=the_sm,State=TstSM_s21")
expect("===RTC===> St-Entry Obj=the_sm,State=TstSM_s211")
expect("@timestamp Init===> Obj=the_sm,State=TstSM_s211")
expect("@timestamp Trg-Done QS_RX_EVENT")

note("shallow history")
dispatch("G_SIG")
expect("@timestamp Disp===> Obj=the_sm,Sig=G_SIG,State=TstSM_s211")
expect("===RTC===> St-Hist  Obj=the_sm,State=TstSM_s21->TstSM_s11")
expect("===RTC===> St-Exit  Obj=the_sm,State=TstSM_s211")
expect("===RTC===> St-Exit  Obj=the_sm,State=TstSM_s21")
expect("===RTC===> St-Exit  Obj=the_sm,State=TstSM_s2")
expect("===RTC===> St-Entry Obj=the_sm,State=TstSM_s1")
expect("===RTC===> St-Entry Obj=the_sm,State=TstSM_s11")
expect("===RTC===> St-Init  Obj=the_sm,State=TstSM_s11->TstSM_s111")
expect("===RTC===> St-Entry Obj=the_sm,State=TstSM_s111")
expect("@timestamp ===>Tran Obj=the_sm,Sig=G_SIG,State=TstSM_s21->TstSM_s111")
expect("@timestamp Trg-Done QS_RX_EVENT")
command(1, 111) # is in state s111
expect("@timestamp CMD 1 111")
expect("@timestamp Trg-Done QS_RX_COMMAND")

dispatch("C_SIG")
expect("@timestamp Disp===> Obj=the_sm,Sig=C_SIG,State=TstSM_s111")
expect("===RTC===> St-Exit  Obj=the_sm,State=TstSM_s111")
expect("===RTC===> St-Entry Obj=the_sm,State=TstSM_s112")
expect("@timestamp ===>Tran Obj=the_sm,Sig=C_SIG,State=TstSM_s111->TstSM_s112")
expect("@timestamp Trg-Done QS_RX_EVENT")

dispatch("C_SIG")
expect("@timestamp Disp===> Obj=the_sm,Sig=C_SIG,State=TstSM_s112")
expect("===RTC===> St-Exit  Obj=the_sm,State=TstSM_s112")
expect("===RTC===> St-Exit  Obj=the_sm,State=TstSM_s11")
expect("===RTC===> St-Exit  Obj=the_sm,State=TstSM_s1")
expect("===RTC===> St-Exit  Obj=the_sm,State=TstSM_s")
expect("===RTC===> St-Entry Obj=the_sm,State=TstSM_t")
expect("@timestamp ===>Tran Obj=the_sm,Sig=C_SIG,State=TstSM_s->TstSM_t")
expect("@timestamp Trg-Done QS_RX_EVENT")

note("shallow history")
dispatch("C_SIG")
expect("@timestamp Disp===> Obj=the_sm,Sig=C_SIG,State=TstSM_t")
expect("===RTC===> St-Hist  Obj=the_sm,State=TstSM_t->TstSM_s1")
expect("===RTC===> St-Exit  Obj=the_sm,State=TstSM_t")
expect("===RTC===> St-Entry Obj=the_sm,State=TstSM_s")
expect("===RTC===> St-Entry Obj=the_sm,State=TstSM_s1")
expect("===RTC===> St-Init  Obj=the_sm,State=TstSM_s1->TstSM_s111")
expect("===RTC===> St-Entry Obj=the_sm,State=TstSM_s11")
expect("===RTC===> St-Entry Obj=the_sm,State=TstSM_s111")
expect("@timestamp ===>Tran Obj=the_sm,Sig=C_SIG,State=TstSM_t->TstSM_s111")
expect("@timestamp Trg-Done QS_RX_EVENT")

note("deep history")
dispatch("G_SIG")
expect("@timestamp Disp===> Obj=the_sm,Sig=G_SIG,State=TstSM_s111")
expect("===RTC===> St-Hist  Obj=the_sm,State=TstSM_s11->TstSM_s211")
expect("===RTC===> St-Exit  Obj=the_sm,State=TstSM_s111")
expect("===RTC===> St-Exit  Obj=the_sm,State=TstSM_s11")
expect("===RTC===> St-Exit  Obj=the_sm,State=TstSM_s1")
expect("===RTC===> St-Entry Obj=the_sm,State=TstSM_s2")
expect("===RTC===> St-Entry Obj=the_sm,State=TstSM_s21")
expect("===RTC===> St-Entry Obj=the_sm,State=TstSM_s211")
expect("@timestamp ===>Tran Obj=the_sm,Sig=G_SIG,State=TstSM_s11->TstSM_s211")
expect("@timestamp Trg-Done QS_RX_EVENT")

dispatch("C_SIG")
expect("@timestamp Disp===> Obj=the_sm,Sig=C_SIG,State=TstSM_s211")
expect("===RTC===> St-Exit  Obj=the_sm,State=TstSM_s211")
expect("===RTC===> St-Entry Obj=the_sm,State=TstSM_s212")
expect("@timestamp ===>Tran Obj=the_sm,Sig=C_SIG,State=TstSM_s211->TstSM_s212")
expect("@timestamp Trg-Done QS_RX_EVENT")

dispatch("C_SIG")
expect("@timestamp Disp===> Obj=the_sm,Sig=C_SIG,State=TstSM_s212")
expect("===RTC===> St-Exit  Obj=the_sm,State=TstSM_s212")
expect("===RTC===> St-Exit  Obj=the_sm,State=TstSM_s21")
expect("===RTC===> St-Exit  Obj=the_sm,State=TstSM_s2")
expect("===RTC===> St-Exit  Obj=the_sm,State=TstSM_s")
expect("===RTC===> St-Entry Obj=the_sm,State=TstSM_t")
expect("@timestamp ===>Tran Obj=the_sm,Sig=C_SIG,State=TstSM_s->TstSM_t")
expect("@timestamp Trg-Done QS_RX_EVENT")

note("deep history")
dispatch("E_SIG")
expect("@timestamp Disp===> Obj=the_sm,Sig=E_SIG,State=TstSM_t")
expect("===RTC===> St-Hist  Obj=the_sm,State=TstSM_t->TstSM_s212")
expect("===RTC===> St-Exit  Obj=the_sm,State=TstSM_t")
expect("===RTC===> St-Entry Obj=the_sm,State=TstSM_s")
expect("===RTC===> St-Entry Obj=the_sm,State=TstSM_s2")
expect("===RTC===> St-Entry Obj=the_sm,State=TstSM_s21")
expect("===RTC===> St-Entry Obj=the_sm,State=TstSM_s212")
expect("@timestamp ===>Tran Obj=the_sm,Sig=E_SIG,State=TstSM_t->TstSM_s212")
expect("@timestamp Trg-Done QS_RX_EVENT")
