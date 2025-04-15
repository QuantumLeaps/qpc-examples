# test-script for QUTest unit testing harness
# see https://www.state-machine.com/qtools/qutest.html

note("QMsm Safety Functions tests...")

#=============================================================================
# preamble...

#=============================================================================
# tests...

#-----------------------------------------------------------------------------
test("TUN_QP_qep_msm_200")
note("no initial transition -> qep_msm:200")
current_obj(OBJ_SM, "the_TstSM0")
init()
expect("@timestamp =ASSERT= Mod=qep_msm,Loc=200")

#-----------------------------------------------------------------------------
test("TUN_QP_qep_msm_210")
note("corrupted state in the ctor -> qep_msm:210")
current_obj(OBJ_SM, "the_TstSM1")
init()
expect("@timestamp =ASSERT= Mod=qep_msm,Loc=210")

#-----------------------------------------------------------------------------
test("TUN_QP_qep_msm_240")
note("no initial transition -> qep_msm:240")
current_obj(OBJ_SM, "the_TstSM2")
init()
expect("@timestamp =ASSERT= Mod=qep_msm,Loc=240")

#-----------------------------------------------------------------------------
test("TUN_QP_qep_msm_280")
note("too many levels of in top-most initial tran -> qep_msm:280")
current_obj(OBJ_SM, "the_TstSM3")
init()
expect("@timestamp =ASSERT= Mod=qep_msm,Loc=280")

#-----------------------------------------------------------------------------
test("TUN_QP_qep_msm_300")
note("dispatching NULL event -> qep_msm:300")
current_obj(OBJ_SM, "the_TstSM5")
init()
expect("@timestamp Trg-Done QS_RX_EVENT")
#
command("DISP_NULL_EVT_CMD")
expect("@timestamp =ASSERT= Mod=qep_msm,Loc=300")

#-----------------------------------------------------------------------------
skip(1) # this Safety Function is only available in SafeQP
test("TUN_QP_qep_msm_311")
note("dispatching corrupted event -> qep_msm:311")
current_obj(OBJ_SM, "the_TstSM5")
init()
expect("@timestamp Trg-Done QS_RX_EVENT")
#
command("DISP_CORR_EVT_CMD")
expect("@timestamp =ASSERT= Mod=qep_msm,Loc=311")

#-----------------------------------------------------------------------------
test("TUN_QP_qep_msm_340")
note("too many levels to tran source -> qep_msm:340")
current_obj(OBJ_SM, "the_TstSM5")
init()
expect("@timestamp Trg-Done QS_RX_EVENT")
#
dispatch("I_SIG")
expect("@timestamp =ASSERT= Mod=qep_msm,Loc=340")

#-----------------------------------------------------------------------------
test("TUN_QP_qep_msm_360")
note("too many nested initial transitions -> qep_msm:360")
current_obj(OBJ_SM, "the_TstSM5")
init()
expect("@timestamp Trg-Done QS_RX_EVENT")
#
dispatch("A_SIG")
expect("@timestamp =ASSERT= Mod=qep_msm,Loc=360")

#-----------------------------------------------------------------------------
test("TUN_QP_qep_msm_400")
note("NULL tran. table -> qep_msm:400")
current_obj(OBJ_SM, "the_TstSM5")
init()
expect("@timestamp Trg-Done QS_RX_EVENT")
#
dispatch("C_SIG")
expect("@timestamp =ASSERT= Mod=qep_msm,Loc=400")

#-----------------------------------------------------------------------------
test("TUN_QP_qep_msm_480")
note("too many steps in tran. table -> qep_msm:480")
current_obj(OBJ_SM, "the_TstSM5")
init()
expect("@timestamp Trg-Done QS_RX_EVENT")
#
dispatch("B_SIG")
expect("@timestamp =ASSERT= Mod=qep_msm,Loc=480")

#-----------------------------------------------------------------------------
test("TUN_QP_qep_msm_610")
note("too many entry actions after tran to history -> qep_msm:620")
current_obj(OBJ_SM, "the_TstSM5")
init()
expect("@timestamp Trg-Done QS_RX_EVENT")
#
dispatch("D_SIG")
expect("@timestamp Trg-Done QS_RX_EVENT")
#
dispatch("H_SIG")
expect("@timestamp =ASSERT= Mod=qep_msm,Loc=610")

#-----------------------------------------------------------------------------
test("TUN_QP_qep_msm_620")
note("too many entry actions after tran to history -> qep_msm:620")
current_obj(OBJ_SM, "the_TstSM5")
init()
expect("@timestamp Trg-Done QS_RX_EVENT")
#
dispatch("H_SIG")
expect("@timestamp =ASSERT= Mod=qep_msm,Loc=620")

#-----------------------------------------------------------------------------
test("TUN_QP_qep_msm_740")
note("isIn() too many nesting levels to state -> qep_msm:740")
current_obj(OBJ_SM, "the_TstSM5")
init()
expect("@timestamp Trg-Done QS_RX_EVENT")
#
dispatch("D_SIG")
expect("@timestamp Trg-Done QS_RX_EVENT")
#
dispatch("D_SIG")
expect("@timestamp Trg-Done QS_RX_EVENT")
#
command("TSTSM5_IS_IN_CMD", 3)
expect("@timestamp =ASSERT= Mod=qep_msm,Loc=740")

#-----------------------------------------------------------------------------
test("TUN_QP_qep_msm_800")
note("childState() not found -> qep_msm:890")
current_obj(OBJ_SM, "the_TstSM5")
init()
expect("@timestamp Trg-Done QS_RX_EVENT")
#
dispatch("D_SIG")
expect("@timestamp Trg-Done QS_RX_EVENT")
#
command("TSTSM5_CHILD_CMD", 3)
expect("@timestamp =ASSERT= Mod=qep_msm,Loc=890")

#-----------------------------------------------------------------------------
test("TUN_QP_qep_msm_840")
note("childState() too many nesting levels to state -> qep_msm:840")
current_obj(OBJ_SM, "the_TstSM5")
init()
expect("@timestamp Trg-Done QS_RX_EVENT")
#
dispatch("D_SIG")
expect("@timestamp Trg-Done QS_RX_EVENT")
#
dispatch("D_SIG")
expect("@timestamp Trg-Done QS_RX_EVENT")
#
command("TSTSM5_CHILD_CMD", 3)
expect("@timestamp =ASSERT= Mod=qep_msm,Loc=840")

#-----------------------------------------------------------------------------
test("TUN_QP_qep_msm_890")
note("childState() too many nesting levels to state -> qep_msm:890")
current_obj(OBJ_SM, "the_TstSM5")
init()
expect("@timestamp Trg-Done QS_RX_EVENT")
#
dispatch("D_SIG")
expect("@timestamp Trg-Done QS_RX_EVENT")
#
command("TSTSM5_CHILD_CMD", 3)
expect("@timestamp =ASSERT= Mod=qep_msm,Loc=890")
