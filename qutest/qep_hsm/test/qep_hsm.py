# test-script for QUTest unit testing harness
# see https://www.state-machine.com/qtools/qutest.html

note("QHsm functional and structural tests...")

#=============================================================================
# preamble...

#=============================================================================
# tests...

#-----------------------------------------------------------------------------
test("TUN_QP_qep_hsm_01")
note("QP version")
#
glb_filter(GRP_UA)
command("QP_VERSION_CMD") # get QP_versionStr
expect("@timestamp COMMAND QP_VERSION_CMD QP/C ?.?.?")
expect("@timestamp Trg-Done QS_RX_COMMAND")

#-----------------------------------------------------------------------------
test("TUN_QP_qep_hsm_02", NORESET)
note("init")
#
glb_filter(GRP_UA)
init()
expect("@timestamp BSP_DISPLAY top-INIT;")
expect("@timestamp BSP_DISPLAY s-ENTRY;")
expect("@timestamp BSP_DISPLAY s2-ENTRY;")
expect("@timestamp BSP_DISPLAY s2-INIT;")
expect("@timestamp BSP_DISPLAY s21-ENTRY;")
expect("@timestamp BSP_DISPLAY s211-ENTRY;")
expect("@timestamp Trg-Done QS_RX_EVENT")

#-----------------------------------------------------------------------------
test("TUN_QP_qep_hsm_03", NORESET)
note("isIn")
command("QHSM_GET_STATE_HANDLER_CMD") # get state handler
expect("@timestamp COMMAND QHSM_GET_STATE_HANDLER_CMD TstSM_s211")
expect("@timestamp Trg-Done QS_RX_COMMAND")
#
command("TSTSM_IS_IN_CMD", 1) # is in state s1
expect("@timestamp COMMAND TSTSM_IS_IN_CMD 0 1") # NO
expect("@timestamp Trg-Done QS_RX_COMMAND")
#
command("TSTSM_IS_IN_CMD", 2) # is in state s2
expect("@timestamp COMMAND TSTSM_IS_IN_CMD 1 2") # YES
expect("@timestamp Trg-Done QS_RX_COMMAND")
#
command("TSTSM_IS_IN_CMD", 21) # is in state s21
expect("@timestamp COMMAND TSTSM_IS_IN_CMD 1 21") # YES
expect("@timestamp Trg-Done QS_RX_COMMAND")
#
command("TSTSM_IS_IN_CMD", 211) # is in state s211
expect("@timestamp COMMAND TSTSM_IS_IN_CMD 1 211") # YES
expect("@timestamp Trg-Done QS_RX_COMMAND")

#-----------------------------------------------------------------------------
test("TUN_QP_qep_hsm_04", NORESET)
note("dispatch")
dispatch("A_SIG")
expect("@timestamp BSP_DISPLAY s21-A;")
expect("@timestamp BSP_DISPLAY s211-EXIT;")
expect("@timestamp BSP_DISPLAY s21-EXIT;")
expect("@timestamp BSP_DISPLAY s21-ENTRY;")
expect("@timestamp BSP_DISPLAY s21-INIT;")
expect("@timestamp BSP_DISPLAY s211-ENTRY;")
expect("@timestamp Trg-Done QS_RX_EVENT")
#
dispatch("D_SIG")
expect("@timestamp BSP_DISPLAY s21-D;")
expect("@timestamp BSP_DISPLAY s211-EXIT;")
expect("@timestamp BSP_DISPLAY s21-EXIT;")
expect("@timestamp BSP_DISPLAY s2-INIT;")
expect("@timestamp BSP_DISPLAY s21-ENTRY;")
expect("@timestamp BSP_DISPLAY s211-ENTRY;")
expect("@timestamp Trg-Done QS_RX_EVENT")
#
dispatch("B_SIG")
expect("@timestamp BSP_DISPLAY s2-B;")
expect("@timestamp BSP_DISPLAY s211-EXIT;")
expect("@timestamp BSP_DISPLAY s21-EXIT;")
expect("@timestamp BSP_DISPLAY s22-ENTRY;")
expect("@timestamp Trg-Done QS_RX_EVENT")
#
dispatch("F_SIG")
expect("@timestamp BSP_DISPLAY s2-F;")
expect("@timestamp BSP_DISPLAY s22-EXIT;")
expect("@timestamp BSP_DISPLAY s2-EXIT;")
expect("@timestamp BSP_DISPLAY s1-ENTRY;")
expect("@timestamp BSP_DISPLAY s11-ENTRY;")
expect("@timestamp BSP_DISPLAY s11-INIT;")
expect("@timestamp BSP_DISPLAY s111-ENTRY;")
expect("@timestamp Trg-Done QS_RX_EVENT")
#
dispatch("H_SIG")
note("ignored")
expect("@timestamp Trg-Done QS_RX_EVENT")
#
dispatch("C_SIG")
expect("@timestamp BSP_DISPLAY s111-C;")
expect("@timestamp BSP_DISPLAY s111-EXIT;")
expect("@timestamp BSP_DISPLAY s112-ENTRY;")
expect("@timestamp Trg-Done QS_RX_EVENT")
#
dispatch("H_SIG")
expect("@timestamp BSP_DISPLAY s112-H;")
expect("@timestamp BSP_DISPLAY s112-EXIT;")
expect("@timestamp BSP_DISPLAY s11-EXIT;")
expect("@timestamp BSP_DISPLAY s1-EXIT;")
expect("@timestamp BSP_DISPLAY s-INIT;")
expect("@timestamp BSP_DISPLAY s1-ENTRY;")
expect("@timestamp BSP_DISPLAY s1-INIT;")
expect("@timestamp BSP_DISPLAY s11-ENTRY;")
expect("@timestamp BSP_DISPLAY s111-ENTRY;")
expect("@timestamp Trg-Done QS_RX_EVENT")

#-----------------------------------------------------------------------------
test("TUN_QP_qep_hsm_05")
note("guards...")
init()
expect("@timestamp Trg-Done QS_RX_EVENT")
#
glb_filter(GRP_UA)
dispatch("D_SIG")
expect("@timestamp BSP_DISPLAY s21-D;")
expect("@timestamp BSP_DISPLAY s211-EXIT;")
expect("@timestamp BSP_DISPLAY s21-EXIT;")
expect("@timestamp BSP_DISPLAY s2-INIT;")
expect("@timestamp BSP_DISPLAY s21-ENTRY;")
expect("@timestamp BSP_DISPLAY s211-ENTRY;")
expect("@timestamp Trg-Done QS_RX_EVENT")
#
dispatch("D_SIG")
expect("@timestamp Trg-Done QS_RX_EVENT")
#
dispatch("I_SIG")
expect("@timestamp BSP_DISPLAY s2-I;")
expect("@timestamp Trg-Done QS_RX_EVENT")
#
dispatch("I_SIG")
expect("@timestamp BSP_DISPLAY s-I;")
expect("@timestamp Trg-Done QS_RX_EVENT")

#-----------------------------------------------------------------------------
test("TUN_QP_qep_hsm_06")
note("history...")
init()
expect("@timestamp Trg-Done QS_RX_EVENT")
#
note("shallow history")
glb_filter(GRP_UA)
dispatch("G_SIG")
expect("@timestamp BSP_DISPLAY s21-G->H*-s1;")
expect("@timestamp BSP_DISPLAY s211-EXIT;")
expect("@timestamp BSP_DISPLAY s21-EXIT;")
expect("@timestamp BSP_DISPLAY s2-EXIT;")
expect("@timestamp BSP_DISPLAY s1-ENTRY;")
expect("@timestamp BSP_DISPLAY s11-ENTRY;")
expect("@timestamp BSP_DISPLAY s11-INIT;")
expect("@timestamp BSP_DISPLAY s111-ENTRY;")
expect("@timestamp Trg-Done QS_RX_EVENT")
#
command("TSTSM_IS_IN_CMD", 111) # is in state s111
expect("@timestamp COMMAND TSTSM_IS_IN_CMD 1 111")
expect("@timestamp Trg-Done QS_RX_COMMAND")
#
dispatch("C_SIG")
expect("@timestamp BSP_DISPLAY s111-C;")
expect("@timestamp BSP_DISPLAY s111-EXIT;")
expect("@timestamp BSP_DISPLAY s112-ENTRY;")
expect("@timestamp Trg-Done QS_RX_EVENT")
#
dispatch("C_SIG")
expect("@timestamp BSP_DISPLAY s-C;")
expect("@timestamp BSP_DISPLAY s112-EXIT;")
expect("@timestamp BSP_DISPLAY s11-EXIT;")
expect("@timestamp BSP_DISPLAY s1-EXIT;")
expect("@timestamp BSP_DISPLAY s-EXIT;")
expect("@timestamp BSP_DISPLAY t-ENTRY;")
expect("@timestamp Trg-Done QS_RX_EVENT")
#
command("TSTSM_IS_IN_CMD", 0) # is in state s
expect("@timestamp COMMAND TSTSM_IS_IN_CMD 0 0") # NO
expect("@timestamp Trg-Done QS_RX_COMMAND")
#
note("shallow history")
dispatch("C_SIG")
expect("@timestamp BSP_DISPLAY t-C->H-s;")
expect("@timestamp BSP_DISPLAY t-EXIT;")
expect("@timestamp BSP_DISPLAY s-ENTRY;")
expect("@timestamp BSP_DISPLAY s1-ENTRY;")
expect("@timestamp BSP_DISPLAY s1-INIT;")
expect("@timestamp BSP_DISPLAY s11-ENTRY;")
expect("@timestamp BSP_DISPLAY s111-ENTRY;")
expect("@timestamp Trg-Done QS_RX_EVENT")
#
note("deep history")
dispatch("G_SIG")
expect("@timestamp BSP_DISPLAY s11-G->H*-s2;")
expect("@timestamp BSP_DISPLAY s111-EXIT;")
expect("@timestamp BSP_DISPLAY s11-EXIT;")
expect("@timestamp BSP_DISPLAY s1-EXIT;")
expect("@timestamp BSP_DISPLAY s2-ENTRY;")
expect("@timestamp BSP_DISPLAY s21-ENTRY;")
expect("@timestamp BSP_DISPLAY s211-ENTRY;")
expect("@timestamp Trg-Done QS_RX_EVENT")
#
dispatch("C_SIG")
expect("@timestamp BSP_DISPLAY s211-C;")
expect("@timestamp BSP_DISPLAY s211-EXIT;")
expect("@timestamp BSP_DISPLAY s212-ENTRY;")
expect("@timestamp Trg-Done QS_RX_EVENT")
#
dispatch("C_SIG")
expect("@timestamp BSP_DISPLAY s-C;")
expect("@timestamp BSP_DISPLAY s212-EXIT;")
expect("@timestamp BSP_DISPLAY s21-EXIT;")
expect("@timestamp BSP_DISPLAY s2-EXIT;")
expect("@timestamp BSP_DISPLAY s-EXIT;")
expect("@timestamp BSP_DISPLAY t-ENTRY;")
expect("@timestamp Trg-Done QS_RX_EVENT")
#
note("deep history")
dispatch("E_SIG")
expect("@timestamp BSP_DISPLAY t-E->H*-s2;")
expect("@timestamp BSP_DISPLAY t-EXIT;")
expect("@timestamp BSP_DISPLAY s-ENTRY;")
expect("@timestamp BSP_DISPLAY s2-ENTRY;")
expect("@timestamp BSP_DISPLAY s21-ENTRY;")
expect("@timestamp BSP_DISPLAY s212-ENTRY;")
expect("@timestamp Trg-Done QS_RX_EVENT")

#-----------------------------------------------------------------------------
test("TUN_QP_qep_hsm_07")
note("init")
#
glb_filter(GRP_SM, GRP_U0)
init()
expect("===RTC===> St-Init  Obj=TstSM_inst,State=QHsm_top->TstSM_s2")
expect("===RTC===> St-Entry Obj=TstSM_inst,State=TstSM_s")
expect("===RTC===> St-Entry Obj=TstSM_inst,State=TstSM_s2")
expect("===RTC===> St-Init  Obj=TstSM_inst,State=TstSM_s2->TstSM_s211")
expect("===RTC===> St-Entry Obj=TstSM_inst,State=TstSM_s21")
expect("===RTC===> St-Entry Obj=TstSM_inst,State=TstSM_s211")
expect("@timestamp Init===> Obj=TstSM_inst,State=TstSM_s211")
expect("@timestamp Trg-Done QS_RX_EVENT")

#-----------------------------------------------------------------------------
test("TUN_QP_qep_hsm_08", NORESET)
note("isIn")
#
command("QHSM_GET_STATE_HANDLER_CMD") # get state handler
expect("@timestamp COMMAND QHSM_GET_STATE_HANDLER_CMD TstSM_s211")
expect("@timestamp Trg-Done QS_RX_COMMAND")
#
command("TSTSM_IS_IN_CMD", 1) # is in state s1
expect("@timestamp COMMAND TSTSM_IS_IN_CMD 0 1") # NO
expect("@timestamp Trg-Done QS_RX_COMMAND")
#
command("TSTSM_IS_IN_CMD", 2) # is in state s2
expect("@timestamp COMMAND TSTSM_IS_IN_CMD 1 2") # YES
expect("@timestamp Trg-Done QS_RX_COMMAND")
#
command("TSTSM_IS_IN_CMD", 21) # is in state s21
expect("@timestamp COMMAND TSTSM_IS_IN_CMD 1 21") # YES
expect("@timestamp Trg-Done QS_RX_COMMAND")
#
command("TSTSM_IS_IN_CMD", 211) # is in state s211
expect("@timestamp COMMAND TSTSM_IS_IN_CMD 1 211") # YES
expect("@timestamp Trg-Done QS_RX_COMMAND")

#-----------------------------------------------------------------------------
test("TUN_QP_qep_hsm_09", NORESET)
note("dispatch")
#
dispatch("A_SIG")
expect("@timestamp Disp===> Obj=TstSM_inst,Sig=A_SIG,State=TstSM_s211")
expect("===RTC===> St-Exit  Obj=TstSM_inst,State=TstSM_s211")
expect("===RTC===> St-Exit  Obj=TstSM_inst,State=TstSM_s21")
expect("===RTC===> St-Entry Obj=TstSM_inst,State=TstSM_s21")
expect("===RTC===> St-Init  Obj=TstSM_inst,State=TstSM_s21->TstSM_s211")
expect("===RTC===> St-Entry Obj=TstSM_inst,State=TstSM_s211")
expect("@timestamp ===>Tran Obj=TstSM_inst,Sig=A_SIG,State=TstSM_s21->TstSM_s211")
expect("@timestamp Trg-Done QS_RX_EVENT")
#
dispatch("D_SIG")
expect("@timestamp Disp===> Obj=TstSM_inst,Sig=D_SIG,State=TstSM_s211")
expect("===RTC===> St-Exit  Obj=TstSM_inst,State=TstSM_s211")
expect("===RTC===> St-Exit  Obj=TstSM_inst,State=TstSM_s21")
expect("===RTC===> St-Init  Obj=TstSM_inst,State=TstSM_s2->TstSM_s211")
expect("===RTC===> St-Entry Obj=TstSM_inst,State=TstSM_s21")
expect("===RTC===> St-Entry Obj=TstSM_inst,State=TstSM_s211")
expect("@timestamp ===>Tran Obj=TstSM_inst,Sig=D_SIG,State=TstSM_s21->TstSM_s211")
expect("@timestamp Trg-Done QS_RX_EVENT")
#
dispatch("B_SIG")
expect("@timestamp Disp===> Obj=TstSM_inst,Sig=B_SIG,State=TstSM_s211")
expect("===RTC===> St-Exit  Obj=TstSM_inst,State=TstSM_s211")
expect("===RTC===> St-Exit  Obj=TstSM_inst,State=TstSM_s21")
expect("===RTC===> St-Entry Obj=TstSM_inst,State=TstSM_s22")
expect("@timestamp ===>Tran Obj=TstSM_inst,Sig=B_SIG,State=TstSM_s2->TstSM_s22")
expect("@timestamp Trg-Done QS_RX_EVENT")
#
dispatch("F_SIG")
expect("@timestamp Disp===> Obj=TstSM_inst,Sig=F_SIG,State=TstSM_s22")
expect("===RTC===> St-Exit  Obj=TstSM_inst,State=TstSM_s22")
expect("===RTC===> St-Exit  Obj=TstSM_inst,State=TstSM_s2")
expect("===RTC===> St-Entry Obj=TstSM_inst,State=TstSM_s1")
expect("===RTC===> St-Entry Obj=TstSM_inst,State=TstSM_s11")
expect("===RTC===> St-Init  Obj=TstSM_inst,State=TstSM_s11->TstSM_s111")
expect("===RTC===> St-Entry Obj=TstSM_inst,State=TstSM_s111")
expect("@timestamp ===>Tran Obj=TstSM_inst,Sig=F_SIG,State=TstSM_s2->TstSM_s111")
expect("@timestamp Trg-Done QS_RX_EVENT")
#
dispatch("H_SIG")
expect("@timestamp Disp===> Obj=TstSM_inst,Sig=H_SIG,State=TstSM_s111")
expect("@timestamp =>Ignore Obj=TstSM_inst,Sig=H_SIG,State=TstSM_s111")
expect("@timestamp Trg-Done QS_RX_EVENT")
#
dispatch("C_SIG")
expect("@timestamp Disp===> Obj=TstSM_inst,Sig=C_SIG,State=TstSM_s111")
expect("===RTC===> St-Exit  Obj=TstSM_inst,State=TstSM_s111")
expect("===RTC===> St-Entry Obj=TstSM_inst,State=TstSM_s112")
expect("@timestamp ===>Tran Obj=TstSM_inst,Sig=C_SIG,State=TstSM_s111->TstSM_s112")
expect("@timestamp Trg-Done QS_RX_EVENT")
#
dispatch("H_SIG")
expect("@timestamp Disp===> Obj=TstSM_inst,Sig=H_SIG,State=TstSM_s112")
expect("===RTC===> St-Exit  Obj=TstSM_inst,State=TstSM_s112")
expect("===RTC===> St-Exit  Obj=TstSM_inst,State=TstSM_s11")
expect("===RTC===> St-Exit  Obj=TstSM_inst,State=TstSM_s1")
expect("===RTC===> St-Init  Obj=TstSM_inst,State=TstSM_s->TstSM_s1")
expect("===RTC===> St-Entry Obj=TstSM_inst,State=TstSM_s1")
expect("===RTC===> St-Init  Obj=TstSM_inst,State=TstSM_s1->TstSM_s111")
expect("===RTC===> St-Entry Obj=TstSM_inst,State=TstSM_s11")
expect("===RTC===> St-Entry Obj=TstSM_inst,State=TstSM_s111")
expect("@timestamp ===>Tran Obj=TstSM_inst,Sig=H_SIG,State=TstSM_s112->TstSM_s111")
expect("@timestamp Trg-Done QS_RX_EVENT")
#
dispatch("E_SIG")
expect("@timestamp Disp===> Obj=TstSM_inst,Sig=E_SIG,State=TstSM_s111")
expect("===RTC===> St-Exit  Obj=TstSM_inst,State=TstSM_s111")
expect("===RTC===> St-Exit  Obj=TstSM_inst,State=TstSM_s11")
expect("===RTC===> St-Exit  Obj=TstSM_inst,State=TstSM_s1")
expect("===RTC===> St-Entry Obj=TstSM_inst,State=TstSM_s2")
expect("===RTC===> St-Entry Obj=TstSM_inst,State=TstSM_s21")
expect("===RTC===> St-Init  Obj=TstSM_inst,State=TstSM_s21->TstSM_s211")
expect("===RTC===> St-Entry Obj=TstSM_inst,State=TstSM_s211")
expect("@timestamp ===>Tran Obj=TstSM_inst,Sig=E_SIG,State=TstSM_s->TstSM_s211")
expect("@timestamp Trg-Done QS_RX_EVENT")

#-----------------------------------------------------------------------------
test("TUN_QP_qep_hsm_10")
note("guards...")
init()
expect("@timestamp Trg-Done QS_RX_EVENT")
#
glb_filter(GRP_SM, GRP_U0)
dispatch("D_SIG")
expect("@timestamp Disp===> Obj=TstSM_inst,Sig=D_SIG,State=TstSM_s211")
expect("===RTC===> St-Exit  Obj=TstSM_inst,State=TstSM_s211")
expect("===RTC===> St-Exit  Obj=TstSM_inst,State=TstSM_s21")
expect("===RTC===> St-Init  Obj=TstSM_inst,State=TstSM_s2->TstSM_s211")
expect("===RTC===> St-Entry Obj=TstSM_inst,State=TstSM_s21")
expect("===RTC===> St-Entry Obj=TstSM_inst,State=TstSM_s211")
expect("@timestamp ===>Tran Obj=TstSM_inst,Sig=D_SIG,State=TstSM_s21->TstSM_s211")
expect("@timestamp Trg-Done QS_RX_EVENT")
#
dispatch("D_SIG")
expect("@timestamp Disp===> Obj=TstSM_inst,Sig=D_SIG,State=TstSM_s211")
expect("===RTC===> St-Unhnd Obj=TstSM_inst,Sig=D_SIG,State=TstSM_s21")
expect("@timestamp =>Ignore Obj=TstSM_inst,Sig=D_SIG,State=TstSM_s211")
expect("@timestamp Trg-Done QS_RX_EVENT")
#
dispatch("I_SIG")
expect("@timestamp Disp===> Obj=TstSM_inst,Sig=I_SIG,State=TstSM_s211")
expect("@timestamp =>Intern Obj=TstSM_inst,Sig=I_SIG,State=TstSM_s2")
expect("@timestamp Trg-Done QS_RX_EVENT")
#
dispatch("I_SIG")
expect("@timestamp Disp===> Obj=TstSM_inst,Sig=I_SIG,State=TstSM_s211")
expect("===RTC===> St-Unhnd Obj=TstSM_inst,Sig=I_SIG,State=TstSM_s2")
expect("@timestamp =>Intern Obj=TstSM_inst,Sig=I_SIG,State=TstSM_s")
expect("@timestamp Trg-Done QS_RX_EVENT")

#-----------------------------------------------------------------------------
test("TUN_QP_qep_hsm_11")
note("history...")
#
glb_filter(GRP_SM, GRP_U0)
init()
expect("===RTC===> St-Init  Obj=TstSM_inst,State=QHsm_top->TstSM_s2")
expect("===RTC===> St-Entry Obj=TstSM_inst,State=TstSM_s")
expect("===RTC===> St-Entry Obj=TstSM_inst,State=TstSM_s2")
expect("===RTC===> St-Init  Obj=TstSM_inst,State=TstSM_s2->TstSM_s211")
expect("===RTC===> St-Entry Obj=TstSM_inst,State=TstSM_s21")
expect("===RTC===> St-Entry Obj=TstSM_inst,State=TstSM_s211")
expect("@timestamp Init===> Obj=TstSM_inst,State=TstSM_s211")
expect("@timestamp Trg-Done QS_RX_EVENT")
#
note("shallow history")
dispatch("G_SIG")
expect("@timestamp Disp===> Obj=TstSM_inst,Sig=G_SIG,State=TstSM_s211")
expect("===RTC===> St-Hist  Obj=TstSM_inst,State=TstSM_s21->TstSM_s11")
expect("===RTC===> St-Exit  Obj=TstSM_inst,State=TstSM_s211")
expect("===RTC===> St-Exit  Obj=TstSM_inst,State=TstSM_s21")
expect("===RTC===> St-Exit  Obj=TstSM_inst,State=TstSM_s2")
expect("===RTC===> St-Entry Obj=TstSM_inst,State=TstSM_s1")
expect("===RTC===> St-Entry Obj=TstSM_inst,State=TstSM_s11")
expect("===RTC===> St-Init  Obj=TstSM_inst,State=TstSM_s11->TstSM_s111")
expect("===RTC===> St-Entry Obj=TstSM_inst,State=TstSM_s111")
expect("@timestamp ===>Tran Obj=TstSM_inst,Sig=G_SIG,State=TstSM_s21->TstSM_s111")
expect("@timestamp Trg-Done QS_RX_EVENT")
#
command("TSTSM_IS_IN_CMD", 111) # is in state s111
expect("@timestamp COMMAND TSTSM_IS_IN_CMD 1 111")
expect("@timestamp Trg-Done QS_RX_COMMAND")
#
dispatch("C_SIG")
expect("@timestamp Disp===> Obj=TstSM_inst,Sig=C_SIG,State=TstSM_s111")
expect("===RTC===> St-Exit  Obj=TstSM_inst,State=TstSM_s111")
expect("===RTC===> St-Entry Obj=TstSM_inst,State=TstSM_s112")
expect("@timestamp ===>Tran Obj=TstSM_inst,Sig=C_SIG,State=TstSM_s111->TstSM_s112")
expect("@timestamp Trg-Done QS_RX_EVENT")
#
dispatch("C_SIG")
expect("@timestamp Disp===> Obj=TstSM_inst,Sig=C_SIG,State=TstSM_s112")
expect("===RTC===> St-Exit  Obj=TstSM_inst,State=TstSM_s112")
expect("===RTC===> St-Exit  Obj=TstSM_inst,State=TstSM_s11")
expect("===RTC===> St-Exit  Obj=TstSM_inst,State=TstSM_s1")
expect("===RTC===> St-Exit  Obj=TstSM_inst,State=TstSM_s")
expect("===RTC===> St-Entry Obj=TstSM_inst,State=TstSM_t")
expect("@timestamp ===>Tran Obj=TstSM_inst,Sig=C_SIG,State=TstSM_s->TstSM_t")
expect("@timestamp Trg-Done QS_RX_EVENT")
#
note("shallow history")
dispatch("C_SIG")
expect("@timestamp Disp===> Obj=TstSM_inst,Sig=C_SIG,State=TstSM_t")
expect("===RTC===> St-Hist  Obj=TstSM_inst,State=TstSM_t->TstSM_s1")
expect("===RTC===> St-Exit  Obj=TstSM_inst,State=TstSM_t")
expect("===RTC===> St-Entry Obj=TstSM_inst,State=TstSM_s")
expect("===RTC===> St-Entry Obj=TstSM_inst,State=TstSM_s1")
expect("===RTC===> St-Init  Obj=TstSM_inst,State=TstSM_s1->TstSM_s111")
expect("===RTC===> St-Entry Obj=TstSM_inst,State=TstSM_s11")
expect("===RTC===> St-Entry Obj=TstSM_inst,State=TstSM_s111")
expect("@timestamp ===>Tran Obj=TstSM_inst,Sig=C_SIG,State=TstSM_t->TstSM_s111")
expect("@timestamp Trg-Done QS_RX_EVENT")
#
note("deep history")
dispatch("G_SIG")
expect("@timestamp Disp===> Obj=TstSM_inst,Sig=G_SIG,State=TstSM_s111")
expect("===RTC===> St-Hist  Obj=TstSM_inst,State=TstSM_s11->TstSM_s211")
expect("===RTC===> St-Exit  Obj=TstSM_inst,State=TstSM_s111")
expect("===RTC===> St-Exit  Obj=TstSM_inst,State=TstSM_s11")
expect("===RTC===> St-Exit  Obj=TstSM_inst,State=TstSM_s1")
expect("===RTC===> St-Entry Obj=TstSM_inst,State=TstSM_s2")
expect("===RTC===> St-Entry Obj=TstSM_inst,State=TstSM_s21")
expect("===RTC===> St-Entry Obj=TstSM_inst,State=TstSM_s211")
expect("@timestamp ===>Tran Obj=TstSM_inst,Sig=G_SIG,State=TstSM_s11->TstSM_s211")
expect("@timestamp Trg-Done QS_RX_EVENT")
#
dispatch("C_SIG")
expect("@timestamp Disp===> Obj=TstSM_inst,Sig=C_SIG,State=TstSM_s211")
expect("===RTC===> St-Exit  Obj=TstSM_inst,State=TstSM_s211")
expect("===RTC===> St-Entry Obj=TstSM_inst,State=TstSM_s212")
expect("@timestamp ===>Tran Obj=TstSM_inst,Sig=C_SIG,State=TstSM_s211->TstSM_s212")
expect("@timestamp Trg-Done QS_RX_EVENT")
#
dispatch("C_SIG")
expect("@timestamp Disp===> Obj=TstSM_inst,Sig=C_SIG,State=TstSM_s212")
expect("===RTC===> St-Exit  Obj=TstSM_inst,State=TstSM_s212")
expect("===RTC===> St-Exit  Obj=TstSM_inst,State=TstSM_s21")
expect("===RTC===> St-Exit  Obj=TstSM_inst,State=TstSM_s2")
expect("===RTC===> St-Exit  Obj=TstSM_inst,State=TstSM_s")
expect("===RTC===> St-Entry Obj=TstSM_inst,State=TstSM_t")
expect("@timestamp ===>Tran Obj=TstSM_inst,Sig=C_SIG,State=TstSM_s->TstSM_t")
expect("@timestamp Trg-Done QS_RX_EVENT")
#
note("deep history")
dispatch("E_SIG")
expect("@timestamp Disp===> Obj=TstSM_inst,Sig=E_SIG,State=TstSM_t")
expect("===RTC===> St-Hist  Obj=TstSM_inst,State=TstSM_t->TstSM_s212")
expect("===RTC===> St-Exit  Obj=TstSM_inst,State=TstSM_t")
expect("===RTC===> St-Entry Obj=TstSM_inst,State=TstSM_s")
expect("===RTC===> St-Entry Obj=TstSM_inst,State=TstSM_s2")
expect("===RTC===> St-Entry Obj=TstSM_inst,State=TstSM_s21")
expect("===RTC===> St-Entry Obj=TstSM_inst,State=TstSM_s212")
expect("@timestamp ===>Tran Obj=TstSM_inst,Sig=E_SIG,State=TstSM_t->TstSM_s212")
expect("@timestamp Trg-Done QS_RX_EVENT")
