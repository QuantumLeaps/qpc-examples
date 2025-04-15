# test-script for QUTest unit testing harness
# see https://www.state-machine.com/qtools/qutest.html

note("Safety Functions tests for qep_hsm.c")

#=============================================================================
# preamble...

#=============================================================================
# tests...

#-----------------------------------------------------------------------------
test("TUN_QP_qep_hsm_200")
current_obj(OBJ_SM, "the_TstSM6")
init()
expect("@timestamp Trg-Done QS_RX_EVENT")
#
note("double init -> qep_hsm:200")
init()
expect("@timestamp =ASSERT= Mod=qep_hsm,Loc=200")

#-----------------------------------------------------------------------------
test("TUN_QP_qep_hsm_210")
note("no initial transition -> qep_hsm:210")
current_obj(OBJ_SM, "the_TstSM0")
init()
expect("@timestamp =ASSERT= Mod=qep_hsm,Loc=210")

#-----------------------------------------------------------------------------
test("TUN_QP_qep_hsm_240")
note("top-most initial tran incorrect -> qep_hsm:240")
current_obj(OBJ_SM, "the_TstSM3")
init()
expect("@timestamp =ASSERT= Mod=qep_hsm,Loc=240")

#-----------------------------------------------------------------------------
test("TUN_QP_qep_hsm_250")
current_obj(OBJ_SM, "the_TstSM2")
note("top-most initial tran to NULL -> qep_hsm:250")
init()
expect("@timestamp =ASSERT= Mod=qep_hsm,Loc=250")

#-----------------------------------------------------------------------------
test("TUN_QP_qep_hsm_260")
current_obj(OBJ_SM, "the_TstSM4")
note("circular nesting -> qep_hsm:260")
init()
expect("@timestamp =ASSERT= Mod=qep_hsm,Loc=260")

#-----------------------------------------------------------------------------
test("TUN_QP_qep_hsm_270")
note("NULL superstate in top-most initial tran. -> qep_hsm:270")
current_obj(OBJ_SM, "the_TstSM5")
init()
expect("@timestamp =ASSERT= Mod=qep_hsm,Loc=270")

#-----------------------------------------------------------------------------
test("TUN_QP_qep_hsm_300A")
current_obj(OBJ_SM, "the_TstSM6")
#
note("dispatch without init -> qep_hsm:300")
dispatch("A_SIG")
expect("@timestamp =ASSERT= Mod=qep_hsm,Loc=300")

#-----------------------------------------------------------------------------
test("TUN_QP_qep_hsm_300B")
current_obj(OBJ_SM, "the_TstSM6")
init()
expect("@timestamp Trg-Done QS_RX_EVENT")
#
note("RTC violation -> qep_hsm:300")
dispatch("D_SIG")
expect("@timestamp =ASSERT= Mod=qep_hsm,Loc=300")

#-----------------------------------------------------------------------------
test("TUN_QP_qep_hsm_310")
current_obj(OBJ_SM, "the_TstSM6")
init()
expect("@timestamp Trg-Done QS_RX_EVENT")
#
note("dispatch NULL event -> qep_hsm:310")
command("DISP_NULL_EVT_CMD")
expect("@timestamp =ASSERT= Mod=qep_hsm,Loc=310")

#-----------------------------------------------------------------------------
skip(1) # Safety Function available only in SafeQP
test("TUN_QP_qep_hsm_321")
current_obj(OBJ_SM, "the_TstSM6")
init()
expect("@timestamp Trg-Done QS_RX_EVENT")
#
note("dispatch corrupted event -> qep_hsm:321")
command("DISP_CORR_EVT_CMD")
expect("@timestamp =ASSERT= Mod=qep_hsm,Loc=321")

#-----------------------------------------------------------------------------
test("TUN_QP_qep_hsm_340")
current_obj(OBJ_SM, "the_TstSM6")
init()
expect("@timestamp Trg-Done QS_RX_EVENT")
#
note("too many nesting levels in a tran. source -> qep_hsm:340")
dispatch("I_SIG")
expect("@timestamp =ASSERT= Mod=qep_hsm,Loc=340")

#-----------------------------------------------------------------------------
test("TUN_QP_qep_hsm_350")
current_obj(OBJ_SM, "the_TstSM6")
init()
expect("@timestamp Trg-Done QS_RX_EVENT")
#
note("tran to NULL state -> qep_hsm:340")
dispatch("G_SIG")
expect("@timestamp =ASSERT= Mod=qep_hsm,Loc=350")

#-----------------------------------------------------------------------------
test("TUN_QP_qep_hsm_440")
current_obj(OBJ_SM, "the_TstSM6")
init()
expect("@timestamp Trg-Done QS_RX_EVENT")
#
note("incorrect nesting of tran. target -> qep_hsm:440")
dispatch("C_SIG")
expect("@timestamp =ASSERT= Mod=qep_hsm,Loc=440")

#-----------------------------------------------------------------------------
test("TUN_QP_qep_hsm_450")
current_obj(OBJ_SM, "the_TstSM6")
init()
expect("@timestamp Trg-Done QS_RX_EVENT")
#
dispatch("E_SIG")
expect("@timestamp Trg-Done QS_RX_EVENT")
#
note("tran to state nested in NULL -> qep_hsm:450")
dispatch("D_SIG")
expect("@timestamp =ASSERT= Mod=qep_hsm,Loc=450")

#-----------------------------------------------------------------------------
test("TUN_QP_qep_hsm_540")
current_obj(OBJ_SM, "the_TstSM6")
init()
expect("@timestamp Trg-Done QS_RX_EVENT")
#
dispatch("E_SIG")
expect("@timestamp Trg-Done QS_RX_EVENT")
#
note("too many nesting levels in tran. target -> qep_hsm:540")
dispatch("E_SIG")
expect("@timestamp =ASSERT= Mod=qep_hsm,Loc=540")

#-----------------------------------------------------------------------------
test("TUN_QP_qep_hsm_560")
current_obj(OBJ_SM, "the_TstSM6")
init()
expect("@timestamp Trg-Done QS_RX_EVENT")
#
note("too many nesting levels in tran. source -> qep_hsm:560")
dispatch("F_SIG")
expect("@timestamp =ASSERT= Mod=qep_hsm,Loc=560")

#-----------------------------------------------------------------------------
test("TUN_QP_qep_hsm_640")
current_obj(OBJ_SM, "the_TstSM6")
init()
expect("@timestamp Trg-Done QS_RX_EVENT")
#
dispatch("E_SIG")
expect("@timestamp Trg-Done QS_RX_EVENT")
#
note("too many initial trans. in target -> qep_hsm:640")
dispatch("G_SIG")
expect("@timestamp =ASSERT= Mod=qep_hsm,Loc=640")

#-----------------------------------------------------------------------------
test("TUN_QP_qep_hsm_650")
current_obj(OBJ_SM, "the_TstSM6")
init()
expect("@timestamp Trg-Done QS_RX_EVENT")
#
dispatch("E_SIG")
expect("@timestamp Trg-Done QS_RX_EVENT")
#
dispatch("H_SIG")
expect("@timestamp =ASSERT= Mod=qep_hsm,Loc=650")

#-----------------------------------------------------------------------------
test("TUN_QP_qep_hsm_660")
current_obj(OBJ_SM, "the_TstSM6")
init()
expect("@timestamp Trg-Done QS_RX_EVENT")
#
dispatch("E_SIG")
expect("@timestamp Trg-Done QS_RX_EVENT")
#
note("too many initial trans. in target -> qep_hsm:660")
dispatch("F_SIG")
expect("@timestamp =ASSERT= Mod=qep_hsm,Loc=660")

#-----------------------------------------------------------------------------
test("TUN_QP_qep_hsm_680")
current_obj(OBJ_SM, "the_TstSM6")
init()
expect("@timestamp Trg-Done QS_RX_EVENT")
#
note("incorrect superstate in target of initial tran. -> qep_hsm:680")
dispatch("H_SIG")
expect("@timestamp =ASSERT= Mod=qep_hsm,Loc=680")

#-----------------------------------------------------------------------------
test("TUN_QP_qep_hsm_700")
current_obj(OBJ_SM, "the_TstSM6")
#
note("uninitialized state machine -> qep_hsm:700")
command("TSTSM6_IS_IN_CMD", 1) # is in state s1
expect("@timestamp =ASSERT= Mod=qep_hsm,Loc=700")

#-----------------------------------------------------------------------------
test("TUN_QP_qep_hsm_740")
current_obj(OBJ_SM, "the_TstSM6")
init()
expect("@timestamp Trg-Done QS_RX_EVENT")
#
note("QHsm_isIn_() too many levels current..queried -> qep_hsm:740")
command("TSTSM6_IS_IN_CMD", 1) # is in state s1
expect("@timestamp =ASSERT= Mod=qep_hsm,Loc=740")

#-----------------------------------------------------------------------------
test("TUN_QP_qep_hsm_840")
current_obj(OBJ_SM, "the_TstSM6")
init()
expect("@timestamp Trg-Done QS_RX_EVENT")
#
note("QHsm_childState() too many levels current..queried -> qep_hsm:840")
command("TSTSM6_CHILD_CMD", 1) # is in state s1
expect("@timestamp =ASSERT= Mod=qep_hsm,Loc=840")

#-----------------------------------------------------------------------------
test("TUN_QP_qep_hsm_890")
current_obj(OBJ_SM, "the_TstSM6")
init()
expect("@timestamp Trg-Done QS_RX_EVENT")
#
dispatch("E_SIG")
expect("@timestamp Trg-Done QS_RX_EVENT")
#
note("QHsm_childState() child cannot be found -> qep_hsm:890")
command("TSTSM6_CHILD_CMD", 3) # is in state s3
expect("@timestamp =ASSERT= Mod=qep_hsm,Loc=890")
