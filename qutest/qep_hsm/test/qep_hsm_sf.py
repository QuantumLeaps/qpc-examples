# test-script for QUTest unit testing harness
# see https://www.state-machine.com/qtools/qutest.html

note("Safety Functions tests for qep_hsm.c")

#=============================================================================
# preamble...

#=============================================================================
# tests...

#-----------------------------------------------------------------------------
test("TUN_QP_qep_hsm_200")
note("no initial transition -> qep_hsm:200")
current_obj(OBJ_SM, "the_TstSM0")
init()
expect("@timestamp =ASSERT= Mod=qep_hsm,Loc=200")

#-----------------------------------------------------------------------------
test("TUN_QP_qep_hsm_210")
note("double init -> qep_hsm:210")
current_obj(OBJ_SM, "the_TstSM2")
init()
expect("@timestamp Trg-Done QS_RX_EVENT")
#
init()
expect("@timestamp =ASSERT= Mod=qep_hsm,Loc=210")

#-----------------------------------------------------------------------------
test("TUN_QP_qep_hsm_240")
note("top-most initial tran incorrect -> qep_hsm:240")
current_obj(OBJ_SM, "the_TstSM3")
init()
expect("@timestamp =ASSERT= Mod=qep_hsm,Loc=240")

#-----------------------------------------------------------------------------
test("TUN_QP_qep_hsm_260")
note("circular nesting -> qep_hsm:260")
current_obj(OBJ_SM, "the_TstSM4")
init()
expect("@timestamp =ASSERT= Mod=qep_hsm,Loc=260")

#-----------------------------------------------------------------------------
test("TUN_QP_qep_hsm_300")
note("dispatch NULL event -> qep_hsm:300")
current_obj(OBJ_SM, "the_TstSM5")
init()
expect("@timestamp Trg-Done QS_RX_EVENT")
#
command("DISP_NULL_EVT_CMD")
expect("@timestamp =ASSERT= Mod=qep_hsm,Loc=300")

#-----------------------------------------------------------------------------
skip(1) # Safety Function only present in SafeQP
test("TUN_QP_qep_hsm_311")
note("dispatch corrupted event -> qep_hsm:311")
current_obj(OBJ_SM, "the_TstSM5")
init()
expect("@timestamp Trg-Done QS_RX_EVENT")
#
command("DISP_CORR_EVT_CMD")
expect("@timestamp =ASSERT= Mod=qep_hsm,Loc=311")

#-----------------------------------------------------------------------------
test("TUN_QP_qep_hsm_320")
note("RTC violation -> qep_hsm:320")
current_obj(OBJ_SM, "the_TstSM5")
init()
expect("@timestamp Trg-Done QS_RX_EVENT")
#
dispatch("D_SIG")
expect("@timestamp =ASSERT= Mod=qep_hsm,Loc=320")

#-----------------------------------------------------------------------------
test("TUN_QP_qep_hsm_340")
note("too many nesting levels in a tran. source -> qep_hsm:340")
current_obj(OBJ_SM, "the_TstSM5")
init()
expect("@timestamp Trg-Done QS_RX_EVENT")
#
dispatch("I_SIG")
expect("@timestamp =ASSERT= Mod=qep_hsm,Loc=340")

#-----------------------------------------------------------------------------
test("TUN_QP_qep_hsm_440")
note("incorrect nesting of tran. target -> qep_hsm:440")
current_obj(OBJ_SM, "the_TstSM5")
init()
expect("@timestamp Trg-Done QS_RX_EVENT")
#
dispatch("C_SIG")
expect("@timestamp =ASSERT= Mod=qep_hsm,Loc=440")

#-----------------------------------------------------------------------------
test("TUN_QP_qep_hsm_540")
note("too many nesting levels in tran. target -> qep_hsm:540")
current_obj(OBJ_SM, "the_TstSM5")
init()
expect("@timestamp Trg-Done QS_RX_EVENT")
#
dispatch("E_SIG")
expect("@timestamp Trg-Done QS_RX_EVENT")
#
dispatch("E_SIG")
expect("@timestamp =ASSERT= Mod=qep_hsm,Loc=540")

#-----------------------------------------------------------------------------
test("TUN_QP_qep_hsm_560")
note("too many nesting levels in tran. source -> qep_hsm:560")
current_obj(OBJ_SM, "the_TstSM5")
init()
expect("@timestamp Trg-Done QS_RX_EVENT")
#
dispatch("F_SIG")
expect("@timestamp =ASSERT= Mod=qep_hsm,Loc=560")

#-----------------------------------------------------------------------------
test("TUN_QP_qep_hsm_640")
note("too many initial trans. in target -> qep_hsm:640")
current_obj(OBJ_SM, "the_TstSM5")
init()
expect("@timestamp Trg-Done QS_RX_EVENT")
#
dispatch("E_SIG")
expect("@timestamp Trg-Done QS_RX_EVENT")
#
dispatch("G_SIG")
expect("@timestamp =ASSERT= Mod=qep_hsm,Loc=640")

#-----------------------------------------------------------------------------
test("TUN_QP_qep_hsm_660")
note("too many initial trans. in target -> qep_hsm:660")
current_obj(OBJ_SM, "the_TstSM5")
init()
expect("@timestamp Trg-Done QS_RX_EVENT")
#
dispatch("E_SIG")
expect("@timestamp Trg-Done QS_RX_EVENT")
#
dispatch("F_SIG")
expect("@timestamp =ASSERT= Mod=qep_hsm,Loc=660")

#-----------------------------------------------------------------------------
test("TUN_QP_qep_hsm_680")
note("incorrect superstate in target of initial tran. -> qep_hsm:680")
current_obj(OBJ_SM, "the_TstSM5")
init()
expect("@timestamp Trg-Done QS_RX_EVENT")
#
dispatch("H_SIG")
expect("@timestamp =ASSERT= Mod=qep_hsm,Loc=680")

#-----------------------------------------------------------------------------
test("TUN_QP_qep_hsm_700")
note("uninitialized state machine -> qep_hsm:700")
#
current_obj(OBJ_SM, "the_TstSM5")
command("TSTSM5_IS_IN_CMD", 1) # is in state s1
expect("@timestamp =ASSERT= Mod=qep_hsm,Loc=700")

#-----------------------------------------------------------------------------
test("TUN_QP_qep_hsm_740")
note("QHsm_isIn_() too many levels current..queried -> qep_hsm:740")
current_obj(OBJ_SM, "the_TstSM5")
init()
expect("@timestamp Trg-Done QS_RX_EVENT")
#
note("isIn")
command("TSTSM5_IS_IN_CMD", 1) # is in state s1
expect("@timestamp =ASSERT= Mod=qep_hsm,Loc=740")

#-----------------------------------------------------------------------------
test("TUN_QP_qep_hsm_840")
note("QHsm_childState() too many levels current..queried -> qep_hsm:840")
current_obj(OBJ_SM, "the_TstSM5")
init()
expect("@timestamp Trg-Done QS_RX_EVENT")
#
command("TSTSM5_CHILD_CMD", 1) # is in state s1
expect("@timestamp =ASSERT= Mod=qep_hsm,Loc=840")

#-----------------------------------------------------------------------------
test("TUN_QP_qep_hsm_890")
note("QHsm_childState() child cannot be found -> qep_hsm:890")
current_obj(OBJ_SM, "the_TstSM5")
init()
expect("@timestamp Trg-Done QS_RX_EVENT")
#
dispatch("E_SIG")
expect("@timestamp Trg-Done QS_RX_EVENT")
#
command("TSTSM5_CHILD_CMD", 3) # is in state s3
expect("@timestamp =ASSERT= Mod=qep_hsm,Loc=890")
