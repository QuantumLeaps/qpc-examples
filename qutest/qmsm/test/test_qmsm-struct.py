# test-script for QUTest unit testing harness
# see https://www.state-machine.com/qtools/qutest.html

# preamble...
def on_reset():
    expect_run()
    glb_filter(GRP_SM, GRP_U1)
    current_obj(OBJ_SM, "the_sm")

# tests...
#==========================================================
test("QMsmTst init")
init()
expect("===RTC===> St-Init  Obj=the_sm,State=NULL->QMsmTst_sub")
expect("===RTC===> St-Entry Obj=the_sm,State=QMsmTst_s")
expect("===RTC===> St-Entry Obj=the_sm,State=QMsmTst_sub")
expect("===RTC===> St-Init  Obj=the_sm,State=QMsmTst_sub->QMsmTst_sub_sub11")
expect("===RTC===> St-Entry Obj=the_sm,State=QMsmTst_sub_sub1")
expect("===RTC===> St-Entry Obj=the_sm,State=QMsmTst_sub_sub11")
expect("@timestamp Init===> Obj=the_sm,State=QMsmTst_sub_sub11")
expect("@timestamp Trg-Done QS_RX_EVENT")

#----------------------------------------------------------
test("QMsmTst isIn", NORESET)
command(0) # get state handler
expect("@timestamp CMD QMsmTst_sub_sub11")
expect("@timestamp Trg-Done QS_RX_COMMAND")

command(1, 1) # is in state s1
expect("@timestamp CMD 0 1") # NO
expect("@timestamp Trg-Done QS_RX_COMMAND")

#command(1, 2) # is in state sub
#expect("@timestamp CMD 1 2") # YES
#expect("@timestamp Trg-Done QS_RX_COMMAND")

command(1, 11) # is in state sub1
expect("@timestamp CMD 1 11") # YES
expect("@timestamp Trg-Done QS_RX_COMMAND")

command(1, 111) # is in state sub11
expect("@timestamp CMD 1 111") # YES
expect("@timestamp Trg-Done QS_RX_COMMAND")

#----------------------------------------------------------
test("QMsmTst dispatch", NORESET)
dispatch("A_SIG")
expect("@timestamp Disp===> Obj=the_sm,Sig=A_SIG,State=QMsmTst_sub_sub11")
expect("===RTC===> St-Exit  Obj=the_sm,State=QMsmTst_sub_sub11")
expect("===RTC===> St-Exit  Obj=the_sm,State=QMsmTst_sub_sub1")
expect("===RTC===> St-Entry Obj=the_sm,State=QMsmTst_sub_sub1")
expect("===RTC===> St-Init  Obj=the_sm,State=QMsmTst_sub_sub1->QMsmTst_sub_sub11")
expect("===RTC===> St-Entry Obj=the_sm,State=QMsmTst_sub_sub11")
expect("@timestamp ===>Tran Obj=the_sm,Sig=A_SIG,State=QMsmTst_sub_sub1->QMsmTst_sub_sub11")
expect("@timestamp Trg-Done QS_RX_EVENT")

dispatch("B_SIG")
expect("@timestamp Disp===> Obj=the_sm,Sig=B_SIG,State=QMsmTst_sub_sub11")
expect("===RTC===> St-Exit  Obj=the_sm,State=QMsmTst_sub_sub11")
expect("===RTC===> St-Entry Obj=the_sm,State=QMsmTst_sub_sub11")
expect("@timestamp ===>Tran Obj=the_sm,Sig=B_SIG,State=QMsmTst_sub_sub1->QMsmTst_sub_sub11")
expect("@timestamp Trg-Done QS_RX_EVENT")

dispatch("C_SIG")
expect("@timestamp Disp===> Obj=the_sm,Sig=C_SIG,State=QMsmTst_sub_sub11")
expect("===RTC===> St-Exit  Obj=the_sm,State=QMsmTst_sub_sub11")
expect("===RTC===> St-Entry Obj=the_sm,State=QMsmTst_sub_sub12")
expect("@timestamp ===>Tran Obj=the_sm,Sig=C_SIG,State=QMsmTst_sub_sub11->QMsmTst_sub_sub12")
expect("@timestamp Trg-Done QS_RX_EVENT")

dispatch("D_SIG")
expect("@timestamp Disp===> Obj=the_sm,Sig=D_SIG,State=QMsmTst_sub_sub12")
expect("===RTC===> St-Exit  Obj=the_sm,State=QMsmTst_sub_sub12")
expect("===RTC===> St-Exit  Obj=the_sm,State=QMsmTst_sub_sub1")
expect("===RTC===> St-Init  Obj=the_sm,State=QMsmTst_sub->QMsmTst_sub_sub11")
expect("===RTC===> St-Entry Obj=the_sm,State=QMsmTst_sub_sub1")
expect("===RTC===> St-Entry Obj=the_sm,State=QMsmTst_sub_sub11")
expect("@timestamp ===>Tran Obj=the_sm,Sig=D_SIG,State=QMsmTst_sub_sub1->QMsmTst_sub_sub11")
expect("@timestamp Trg-Done QS_RX_EVENT")

dispatch("E_SIG")
expect("@timestamp Disp===> Obj=the_sm,Sig=E_SIG,State=QMsmTst_sub_sub11")
expect("===RTC===> St-Exit  Obj=the_sm,State=QMsmTst_sub_sub11")
expect("===RTC===> St-Exit  Obj=the_sm,State=QMsmTst_sub_sub1")
expect("===RTC===> St-Entry Obj=the_sm,State=QMsmTst_sub_sub2")
expect("@timestamp ===>Tran Obj=the_sm,Sig=E_SIG,State=QMsmTst_sub->QMsmTst_sub_sub2")
expect("@timestamp Trg-Done QS_RX_EVENT")

dispatch("F_SIG")
expect("@timestamp Disp===> Obj=the_sm,Sig=F_SIG,State=QMsmTst_sub_sub2")
expect("===RTC===> St-Exit  Obj=the_sm,State=QMsmTst_sub_sub2")
expect("===RTC===> St-Exit  Obj=the_sm,State=QMsmTst_sub")
expect("===RTC===> St-Entry Obj=the_sm,State=QMsmTst_sub")
expect("===RTC===> St-EP    Obj=the_sm,State=QMsmTst_sub->QMsmTst_sub_sub1")
expect("===RTC===> St-Entry Obj=the_sm,State=QMsmTst_sub_sub1")
expect("===RTC===> St-Init  Obj=the_sm,State=QMsmTst_sub_sub1->QMsmTst_sub_sub11")
expect("===RTC===> St-Entry Obj=the_sm,State=QMsmTst_sub_sub11")
expect("@timestamp ===>Tran Obj=the_sm,Sig=F_SIG,State=QMsmTst_s2->QMsmTst_sub_sub11")
expect("@timestamp Trg-Done QS_RX_EVENT")

dispatch("H_SIG")
expect("@timestamp Disp===> Obj=the_sm,Sig=H_SIG,State=QMsmTst_sub_sub11")
expect("@timestamp =>Ignore Obj=the_sm,Sig=H_SIG,State=QMsmTst_sub_sub11")
expect("@timestamp Trg-Done QS_RX_EVENT")

dispatch("C_SIG")
expect("@timestamp Disp===> Obj=the_sm,Sig=C_SIG,State=QMsmTst_sub_sub11")
expect("===RTC===> St-Exit  Obj=the_sm,State=QMsmTst_sub_sub11")
expect("===RTC===> St-Entry Obj=the_sm,State=QMsmTst_sub_sub12")
expect("@timestamp ===>Tran Obj=the_sm,Sig=C_SIG,State=QMsmTst_sub_sub11->QMsmTst_sub_sub12")
expect("@timestamp Trg-Done QS_RX_EVENT")

dispatch("H_SIG")
expect("@timestamp Disp===> Obj=the_sm,Sig=H_SIG,State=QMsmTst_sub_sub12")
expect("===RTC===> St-Exit  Obj=the_sm,State=QMsmTst_sub_sub12")
expect("===RTC===> St-Exit  Obj=the_sm,State=QMsmTst_sub_sub1")
expect("===RTC===> St-Exit  Obj=the_sm,State=QMsmTst_s1")
expect("===RTC===> St-Init  Obj=the_sm,State=QMsmTst_s->QMsmTst_sub")
expect("===RTC===> St-Entry Obj=the_sm,State=QMsmTst_sub")
expect("===RTC===> St-Init  Obj=the_sm,State=QMsmTst_sub->QMsmTst_sub_sub11")
expect("===RTC===> St-Entry Obj=the_sm,State=QMsmTst_sub_sub1")
expect("===RTC===> St-Entry Obj=the_sm,State=QMsmTst_sub_sub11")
expect("@timestamp ===>Tran Obj=the_sm,Sig=H_SIG,State=QMsmTst_sub_sub12->QMsmTst_sub_sub11")
expect("@timestamp Trg-Done QS_RX_EVENT")

#==========================================================
test("QMsmTst guards")
init()
expect("===RTC===> St-Init  Obj=the_sm,State=NULL->QMsmTst_sub")
expect("===RTC===> St-Entry Obj=the_sm,State=QMsmTst_s")
expect("===RTC===> St-Entry Obj=the_sm,State=QMsmTst_sub")
expect("===RTC===> St-Init  Obj=the_sm,State=QMsmTst_sub->QMsmTst_sub_sub11")
expect("===RTC===> St-Entry Obj=the_sm,State=QMsmTst_sub_sub1")
expect("===RTC===> St-Entry Obj=the_sm,State=QMsmTst_sub_sub11")
expect("@timestamp Init===> Obj=the_sm,State=QMsmTst_sub_sub11")
expect("@timestamp Trg-Done QS_RX_EVENT")

dispatch("D_SIG")
expect("@timestamp Disp===> Obj=the_sm,Sig=D_SIG,State=QMsmTst_sub_sub11")
expect("===RTC===> St-Exit  Obj=the_sm,State=QMsmTst_sub_sub11")
expect("===RTC===> St-Exit  Obj=the_sm,State=QMsmTst_sub_sub1")
expect("===RTC===> St-Init  Obj=the_sm,State=QMsmTst_sub->QMsmTst_sub_sub11")
expect("===RTC===> St-Entry Obj=the_sm,State=QMsmTst_sub_sub1")
expect("===RTC===> St-Entry Obj=the_sm,State=QMsmTst_sub_sub11")
expect("@timestamp ===>Tran Obj=the_sm,Sig=D_SIG,State=QMsmTst_sub_sub1->QMsmTst_sub_sub11")
expect("@timestamp Trg-Done QS_RX_EVENT")

dispatch("D_SIG")
expect("@timestamp Disp===> Obj=the_sm,Sig=D_SIG,State=QMsmTst_sub_sub11")
expect("===RTC===> St-Unhnd Obj=the_sm,Sig=D_SIG,State=QMsmTst_sub_sub1")
expect("@timestamp =>Ignore Obj=the_sm,Sig=D_SIG,State=QMsmTst_sub_sub11")
expect("@timestamp Trg-Done QS_RX_EVENT")

dispatch("I_SIG")
expect("@timestamp Disp===> Obj=the_sm,Sig=I_SIG,State=QMsmTst_sub_sub11")
expect("@timestamp =>Intern Obj=the_sm,Sig=I_SIG,State=QMsmTst_sub")
expect("@timestamp Trg-Done QS_RX_EVENT")

dispatch("I_SIG")
expect("@timestamp Disp===> Obj=the_sm,Sig=I_SIG,State=QMsmTst_sub_sub11")
#expect("===RTC===> St-Unhnd Obj=the_sm,Sig=I_SIG,State=QMsmTst_sub")
expect("@timestamp =>Intern Obj=the_sm,Sig=I_SIG,State=QMsmTst_s")
expect("@timestamp Trg-Done QS_RX_EVENT")

#==========================================================
test("QMsmTst history")
init()
expect("===RTC===> St-Init  Obj=the_sm,State=NULL->QMsmTst_sub")
expect("===RTC===> St-Entry Obj=the_sm,State=QMsmTst_s")
expect("===RTC===> St-Entry Obj=the_sm,State=QMsmTst_sub")
expect("===RTC===> St-Init  Obj=the_sm,State=QMsmTst_sub->QMsmTst_sub_sub11")
expect("===RTC===> St-Entry Obj=the_sm,State=QMsmTst_sub_sub1")
expect("===RTC===> St-Entry Obj=the_sm,State=QMsmTst_sub_sub11")
expect("@timestamp Init===> Obj=the_sm,State=QMsmTst_sub_sub11")
expect("@timestamp Trg-Done QS_RX_EVENT")

dispatch("G_SIG")
expect("@timestamp Disp===> Obj=the_sm,Sig=G_SIG,State=QMsmTst_sub_sub11")
expect("===RTC===> St-Exit  Obj=the_sm,State=QMsmTst_sub_sub11")
expect("===RTC===> St-Exit  Obj=the_sm,State=QMsmTst_sub_sub1")
expect("===RTC===> St-Exit  Obj=the_sm,State=QMsmTst_s2")
expect("===RTC===> St-Entry Obj=the_sm,State=QMsmTst_sub")
expect("===RTC===> St-Hist  Obj=the_sm,State=QMsmTst_sub->QMsmTst_sub_sub11")
expect("===RTC===> St-Entry Obj=the_sm,State=QMsmTst_sub_sub1")
expect("===RTC===> St-Entry Obj=the_sm,State=QMsmTst_sub_sub11")
expect("@timestamp ===>Tran Obj=the_sm,Sig=G_SIG,State=QMsmTst_sub_sub1->QMsmTst_sub_sub11")
expect("@timestamp Trg-Done QS_RX_EVENT")

dispatch("C_SIG")
expect("@timestamp Disp===> Obj=the_sm,Sig=C_SIG,State=QMsmTst_sub_sub11")
expect("===RTC===> St-Exit  Obj=the_sm,State=QMsmTst_sub_sub11")
expect("===RTC===> St-Entry Obj=the_sm,State=QMsmTst_sub_sub12")
expect("@timestamp ===>Tran Obj=the_sm,Sig=C_SIG,State=QMsmTst_sub_sub11->QMsmTst_sub_sub12")
expect("@timestamp Trg-Done QS_RX_EVENT")

dispatch("C_SIG")
expect("@timestamp Disp===> Obj=the_sm,Sig=C_SIG,State=QMsmTst_sub_sub12")
expect("===RTC===> St-Exit  Obj=the_sm,State=QMsmTst_sub_sub12")
expect("===RTC===> St-Exit  Obj=the_sm,State=QMsmTst_sub_sub1")
expect("===RTC===> St-Exit  Obj=the_sm,State=QMsmTst_sub")
expect("===RTC===> St-Exit  Obj=the_sm,State=QMsmTst_s")
expect("===RTC===> St-Entry Obj=the_sm,State=QMsmTst_t")
expect("@timestamp ===>Tran Obj=the_sm,Sig=C_SIG,State=QMsmTst_s->QMsmTst_t")
expect("@timestamp Trg-Done QS_RX_EVENT")

dispatch("C_SIG")
expect("@timestamp Disp===> Obj=the_sm,Sig=C_SIG,State=QMsmTst_t")
expect("===RTC===> St-Exit  Obj=the_sm,State=QMsmTst_t")
expect("===RTC===> St-Entry Obj=the_sm,State=QMsmTst_s")
expect("===RTC===> St-Hist  Obj=the_sm,State=QMsmTst_s->QMsmTst_s1")
expect("===RTC===> St-Entry Obj=the_sm,State=QMsmTst_s1")
expect("===RTC===> St-Entry Obj=the_sm,State=QMsmTst_sub_sub1")
expect("===RTC===> St-Entry Obj=the_sm,State=QMsmTst_sub_sub11")
expect("@timestamp ===>Tran Obj=the_sm,Sig=C_SIG,State=QMsmTst_t->QMsmTst_sub_sub11")
expect("@timestamp Trg-Done QS_RX_EVENT")

dispatch("G_SIG")
expect("@timestamp Disp===> Obj=the_sm,Sig=G_SIG,State=QMsmTst_sub_sub11")
expect("===RTC===> St-Exit  Obj=the_sm,State=QMsmTst_sub_sub11")
expect("===RTC===> St-Exit  Obj=the_sm,State=QMsmTst_sub_sub1")
expect("===RTC===> St-Exit  Obj=the_sm,State=QMsmTst_s1")
expect("===RTC===> St-Entry Obj=the_sm,State=QMsmTst_sub")
expect("===RTC===> St-Hist  Obj=the_sm,State=QMsmTst_sub->QMsmTst_sub_sub11")
expect("===RTC===> St-Entry Obj=the_sm,State=QMsmTst_sub_sub1")
expect("===RTC===> St-Entry Obj=the_sm,State=QMsmTst_sub_sub11")
expect("@timestamp ===>Tran Obj=the_sm,Sig=G_SIG,State=QMsmTst_sub_sub1->QMsmTst_sub_sub11")
expect("@timestamp Trg-Done QS_RX_EVENT")
