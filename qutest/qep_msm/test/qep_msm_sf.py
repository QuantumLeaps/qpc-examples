# test-script for QUTest unit testing harness
# see https://www.state-machine.com/qtools/qutest.html

note("QMsm Safety Functions tests...")

#=============================================================================
# preamble...

#=============================================================================
# tests...

#-----------------------------------------------------------------------------
test("TUN_QP_qep_msm_200")
current_obj(OBJ_SM, "the_TstSM0")
note("no initial transition -> qep_msm:200")
init()
expect("@timestamp =ASSERT= Mod=qep_msm,Loc=200")

#-----------------------------------------------------------------------------
test("TUN_QP_qep_msm_210")
current_obj(OBJ_SM, "the_TstSM1")
note("corrupted state in the ctor -> qep_msm:210")
init()
expect("@timestamp =ASSERT= Mod=qep_msm,Loc=210")

#-----------------------------------------------------------------------------
test("TUN_QP_qep_msm_240")
current_obj(OBJ_SM, "the_TstSM2")
note("no initial transition -> qep_msm:240")
init()
expect("@timestamp =ASSERT= Mod=qep_msm,Loc=240")

#-----------------------------------------------------------------------------
test("TUN_QP_qep_msm_250")
current_obj(OBJ_SM, "the_TstSM3")
note("NULL tran table in top-most initial tran -> qep_msm:250")
init()
expect("@timestamp =ASSERT= Mod=qep_msm,Loc=250")

#-----------------------------------------------------------------------------
test("TUN_QP_qep_msm_280")
current_obj(OBJ_SM, "the_TstSM4")
note("too many levels of in top-most initial tran -> qep_msm:280")
init()
expect("@timestamp =ASSERT= Mod=qep_msm,Loc=280")

#-----------------------------------------------------------------------------
test("TUN_QP_qep_msm_300")
current_obj(OBJ_SM, "the_TstSM5")
#
note("dispatch without init -> qep_msm:300")
dispatch("I_SIG")
expect("@timestamp =ASSERT= Mod=qep_msm,Loc=300")

#-----------------------------------------------------------------------------
test("TUN_QP_qep_msm_310")
current_obj(OBJ_SM, "the_TstSM5")
init()
expect("@timestamp Trg-Done QS_RX_EVENT")
#
note("dispatching NULL event -> qep_msm:310")
command("DISP_NULL_EVT_CMD")
expect("@timestamp =ASSERT= Mod=qep_msm,Loc=310")

#-----------------------------------------------------------------------------
skip(1) # Safety Function available only in SafeQP
test("TUN_QP_qep_msm_321")
current_obj(OBJ_SM, "the_TstSM5")
init()
expect("@timestamp Trg-Done QS_RX_EVENT")
#
note("dispatching corrupted event -> qep_msm:321")
command("DISP_CORR_EVT_CMD")
expect("@timestamp =ASSERT= Mod=qep_msm,Loc=321")

#-----------------------------------------------------------------------------
test("TUN_QP_qep_msm_340")
current_obj(OBJ_SM, "the_TstSM5")
init()
expect("@timestamp Trg-Done QS_RX_EVENT")
#
note("too many levels to tran source -> qep_msm:340")
dispatch("I_SIG")
expect("@timestamp =ASSERT= Mod=qep_msm,Loc=340")

#-----------------------------------------------------------------------------
test("TUN_QP_qep_msm_360")
current_obj(OBJ_SM, "the_TstSM5")
init()
expect("@timestamp Trg-Done QS_RX_EVENT")
#
note("too many nested initial transitions -> qep_msm:360")
dispatch("A_SIG")
expect("@timestamp =ASSERT= Mod=qep_msm,Loc=360")

#-----------------------------------------------------------------------------
test("TUN_QP_qep_msm_400")
current_obj(OBJ_SM, "the_TstSM5")
init()
expect("@timestamp Trg-Done QS_RX_EVENT")
#
note("NULL tran. table -> qep_msm:400")
dispatch("C_SIG")
expect("@timestamp =ASSERT= Mod=qep_msm,Loc=400")

#-----------------------------------------------------------------------------
test("TUN_QP_qep_msm_480")
current_obj(OBJ_SM, "the_TstSM5")
init()
expect("@timestamp Trg-Done QS_RX_EVENT")
#
note("too many steps in tran. table -> qep_msm:480")
dispatch("B_SIG")
expect("@timestamp =ASSERT= Mod=qep_msm,Loc=480")

#-----------------------------------------------------------------------------
skip(1)
test("TUN_QP_qep_msm_580")
note("not achievable")

#-----------------------------------------------------------------------------
test("TUN_QP_qep_msm_610")
current_obj(OBJ_SM, "the_TstSM5")
init()
expect("@timestamp Trg-Done QS_RX_EVENT")
#
dispatch("D_SIG")
expect("@timestamp Trg-Done QS_RX_EVENT")
#
note("too many entry actions after tran to history -> qep_msm:620")
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
note("isIn() too many nesting levels to state -> qep_msm:740")
command("TSTSM5_IS_IN_CMD", 3)
expect("@timestamp =ASSERT= Mod=qep_msm,Loc=740")

#-----------------------------------------------------------------------------
test("TUN_QP_qep_msm_840")
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
note("childState() too many nesting levels to state -> qep_msm:840")
command("TSTSM5_CHILD_CMD", 3)
expect("@timestamp =ASSERT= Mod=qep_msm,Loc=840")

#-----------------------------------------------------------------------------
test("TUN_QP_qep_msm_890")
current_obj(OBJ_SM, "the_TstSM5")
init()
expect("@timestamp Trg-Done QS_RX_EVENT")
#
dispatch("D_SIG")
expect("@timestamp Trg-Done QS_RX_EVENT")
#
note("childState() too many nesting levels to state -> qep_msm:890")
command("TSTSM5_CHILD_CMD", 3)
expect("@timestamp =ASSERT= Mod=qep_msm,Loc=890")
