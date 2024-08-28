# test-script for QUTest unit testing harness
# see https://www.state-machine.com/qtools/qutest.html

# preamble...
def on_reset():
    expect_run()
    glb_filter(GRP_UA)
    current_obj(OBJ_SM, "the_sm")

# tests...
#==========================================================
test("QMsmTst_init")
init()
expect("@timestamp BSP_DISPLAY top-INIT;")
expect("@timestamp BSP_DISPLAY s-ENTRY;")
expect("@timestamp BSP_DISPLAY sub-ENTRY;")
expect("@timestamp BSP_DISPLAY sub-INIT;")
expect("@timestamp BSP_DISPLAY sub1-ENTRY;")
expect("@timestamp BSP_DISPLAY sub11-ENTRY;")
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
expect("@timestamp BSP_DISPLAY sub1-A;")
expect("@timestamp BSP_DISPLAY sub11-EXIT;")
expect("@timestamp BSP_DISPLAY sub1-EXIT;")
expect("@timestamp BSP_DISPLAY sub1-ENTRY;")
expect("@timestamp BSP_DISPLAY sub1-INIT;")
expect("@timestamp BSP_DISPLAY sub11-ENTRY;")
expect("@timestamp Trg-Done QS_RX_EVENT")

dispatch("B_SIG")
expect("@timestamp BSP_DISPLAY sub1-B;")
expect("@timestamp BSP_DISPLAY sub11-EXIT;")
expect("@timestamp BSP_DISPLAY sub11-ENTRY;")
expect("@timestamp Trg-Done QS_RX_EVENT")

dispatch("C_SIG")
expect("@timestamp BSP_DISPLAY sub11-C;")
expect("@timestamp BSP_DISPLAY sub11-EXIT;")
expect("@timestamp BSP_DISPLAY sub12-ENTRY;")
expect("@timestamp Trg-Done QS_RX_EVENT")

dispatch("D_SIG")
expect("@timestamp BSP_DISPLAY sub1-D;")
expect("@timestamp BSP_DISPLAY sub12-EXIT;")
expect("@timestamp BSP_DISPLAY sub1-EXIT;")
expect("@timestamp BSP_DISPLAY sub-INIT;")
expect("@timestamp BSP_DISPLAY sub1-ENTRY;")
expect("@timestamp BSP_DISPLAY sub11-ENTRY;")
expect("@timestamp Trg-Done QS_RX_EVENT")

dispatch("E_SIG")
expect("@timestamp BSP_DISPLAY sub-E;")
expect("@timestamp BSP_DISPLAY sub11-EXIT;")
expect("@timestamp BSP_DISPLAY sub1-EXIT;")
expect("@timestamp BSP_DISPLAY sub2-ENTRY;")
expect("@timestamp Trg-Done QS_RX_EVENT")

dispatch("F_SIG")
expect("@timestamp BSP_DISPLAY s2-F;")
expect("@timestamp BSP_DISPLAY sub2-EXIT;")
expect("@timestamp BSP_DISPLAY sub-EXIT;")
expect("@timestamp BSP_DISPLAY sub-ENTRY;")
expect("@timestamp BSP_DISPLAY sub-ep1;")
expect("@timestamp BSP_DISPLAY sub1-ENTRY;")
expect("@timestamp BSP_DISPLAY sub1-INIT;")
expect("@timestamp BSP_DISPLAY sub11-ENTRY;")
expect("@timestamp Trg-Done QS_RX_EVENT")

dispatch("C_SIG")
expect("@timestamp BSP_DISPLAY sub11-C;")
expect("@timestamp BSP_DISPLAY sub11-EXIT;")
expect("@timestamp BSP_DISPLAY sub12-ENTRY;")
expect("@timestamp Trg-Done QS_RX_EVENT")

dispatch("H_SIG")
expect("@timestamp BSP_DISPLAY sub12-H->xpH;")
expect("@timestamp BSP_DISPLAY sub12-EXIT;")
expect("@timestamp BSP_DISPLAY sub1-EXIT;")
expect("@timestamp BSP_DISPLAY sub-EXIT;")
expect("@timestamp BSP_DISPLAY s-INIT;")
expect("@timestamp BSP_DISPLAY sub-ENTRY;")
expect("@timestamp BSP_DISPLAY sub-INIT;")
expect("@timestamp BSP_DISPLAY sub1-ENTRY;")
expect("@timestamp BSP_DISPLAY sub11-ENTRY;")
expect("@timestamp Trg-Done QS_RX_EVENT")

#==========================================================
test("QMsmTst guards")
init()
expect("@timestamp BSP_DISPLAY top-INIT;")
expect("@timestamp BSP_DISPLAY s-ENTRY;")
expect("@timestamp BSP_DISPLAY sub-ENTRY;")
expect("@timestamp BSP_DISPLAY sub-INIT;")
expect("@timestamp BSP_DISPLAY sub1-ENTRY;")
expect("@timestamp BSP_DISPLAY sub11-ENTRY;")
expect("@timestamp Trg-Done QS_RX_EVENT")

dispatch("D_SIG")
expect("@timestamp BSP_DISPLAY sub1-D;")
expect("@timestamp BSP_DISPLAY sub11-EXIT;")
expect("@timestamp BSP_DISPLAY sub1-EXIT;")
expect("@timestamp BSP_DISPLAY sub-INIT;")
expect("@timestamp BSP_DISPLAY sub1-ENTRY;")
expect("@timestamp BSP_DISPLAY sub11-ENTRY;")
expect("@timestamp Trg-Done QS_RX_EVENT")

dispatch("D_SIG")
expect("@timestamp Trg-Done QS_RX_EVENT")

dispatch("I_SIG")
expect("@timestamp BSP_DISPLAY sub-I;")
expect("@timestamp Trg-Done QS_RX_EVENT")

dispatch("I_SIG")
expect("@timestamp BSP_DISPLAY s-I;")
expect("@timestamp Trg-Done QS_RX_EVENT")

#==========================================================
test("QMsmTst history")
init()
expect("@timestamp BSP_DISPLAY top-INIT;")
expect("@timestamp BSP_DISPLAY s-ENTRY;")
expect("@timestamp BSP_DISPLAY sub-ENTRY;")
expect("@timestamp BSP_DISPLAY sub-INIT;")
expect("@timestamp BSP_DISPLAY sub1-ENTRY;")
expect("@timestamp BSP_DISPLAY sub11-ENTRY;")
expect("@timestamp Trg-Done QS_RX_EVENT")

dispatch("G_SIG")
expect("@timestamp BSP_DISPLAY sub1-G->xpG;")
expect("@timestamp BSP_DISPLAY sub11-EXIT;")
expect("@timestamp BSP_DISPLAY sub1-EXIT;")
expect("@timestamp BSP_DISPLAY sub-EXIT;")
expect("@timestamp BSP_DISPLAY sub-ENTRY;")
expect("@timestamp BSP_DISPLAY sub1-ENTRY;")
expect("@timestamp BSP_DISPLAY sub11-ENTRY;")
expect("@timestamp Trg-Done QS_RX_EVENT")
command(1, 111) # is in state s111
expect("@timestamp CMD 1 111")
expect("@timestamp Trg-Done QS_RX_COMMAND")

dispatch("C_SIG")
expect("@timestamp BSP_DISPLAY sub11-C;")
expect("@timestamp BSP_DISPLAY sub11-EXIT;")
expect("@timestamp BSP_DISPLAY sub12-ENTRY;")
expect("@timestamp Trg-Done QS_RX_EVENT")

dispatch("C_SIG")
expect("@timestamp BSP_DISPLAY s-C;")
expect("@timestamp BSP_DISPLAY sub12-EXIT;")
expect("@timestamp BSP_DISPLAY sub1-EXIT;")
expect("@timestamp BSP_DISPLAY sub-EXIT;")
expect("@timestamp BSP_DISPLAY s-EXIT;")
expect("@timestamp BSP_DISPLAY t-ENTRY;")
expect("@timestamp Trg-Done QS_RX_EVENT")
command(1, 0) # is in state s
expect("@timestamp CMD 0 0") # NO
expect("@timestamp Trg-Done QS_RX_COMMAND")

dispatch("C_SIG")
expect("@timestamp BSP_DISPLAY t-C->H-s;")
expect("@timestamp BSP_DISPLAY t-EXIT;")
expect("@timestamp BSP_DISPLAY s-ENTRY;")
expect("@timestamp BSP_DISPLAY sub-ENTRY;")
expect("@timestamp BSP_DISPLAY sub-INIT;")
expect("@timestamp BSP_DISPLAY sub1-ENTRY;")
expect("@timestamp BSP_DISPLAY sub11-ENTRY;")
expect("@timestamp Trg-Done QS_RX_EVENT")

dispatch("G_SIG")
expect("@timestamp BSP_DISPLAY sub1-G->xpG;")
expect("@timestamp BSP_DISPLAY sub11-EXIT;")
expect("@timestamp BSP_DISPLAY sub1-EXIT;")
expect("@timestamp BSP_DISPLAY sub-EXIT;")
expect("@timestamp BSP_DISPLAY sub-ENTRY;")
expect("@timestamp BSP_DISPLAY sub1-ENTRY;")
expect("@timestamp BSP_DISPLAY sub11-ENTRY;")
expect("@timestamp Trg-Done QS_RX_EVENT")

