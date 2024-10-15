# test-script for QUTest unit testing harness
# see https://www.state-machine.com/qtools/qutest.html

# preamble...
def on_reset():
    expect_run()
    glb_filter(GRP_UA)
    current_obj(OBJ_SM, "the_sm")

# tests...
#==========================================================
test("TstSM_init")
init()
expect("@timestamp BSP_DISPLAY top-INIT;")
expect("@timestamp BSP_DISPLAY s-ENTRY;")
expect("@timestamp BSP_DISPLAY s2-ENTRY;")
expect("@timestamp BSP_DISPLAY s2-INIT;")
expect("@timestamp BSP_DISPLAY s21-ENTRY;")
expect("@timestamp BSP_DISPLAY s211-ENTRY;")
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
expect("@timestamp BSP_DISPLAY s21-A;")
expect("@timestamp BSP_DISPLAY s211-EXIT;")
expect("@timestamp BSP_DISPLAY s21-EXIT;")
expect("@timestamp BSP_DISPLAY s21-ENTRY;")
expect("@timestamp BSP_DISPLAY s21-INIT;")
expect("@timestamp BSP_DISPLAY s211-ENTRY;")
expect("@timestamp Trg-Done QS_RX_EVENT")

dispatch("B_SIG")
expect("@timestamp BSP_DISPLAY s21-B;")
expect("@timestamp BSP_DISPLAY s211-EXIT;")
expect("@timestamp BSP_DISPLAY s211-ENTRY;")
expect("@timestamp Trg-Done QS_RX_EVENT")

dispatch("C_SIG")
expect("@timestamp BSP_DISPLAY s211-C;")
expect("@timestamp BSP_DISPLAY s211-EXIT;")
expect("@timestamp BSP_DISPLAY s212-ENTRY;")
expect("@timestamp Trg-Done QS_RX_EVENT")

dispatch("D_SIG")
expect("@timestamp BSP_DISPLAY s21-D;")
expect("@timestamp BSP_DISPLAY s212-EXIT;")
expect("@timestamp BSP_DISPLAY s21-EXIT;")
expect("@timestamp BSP_DISPLAY s2-INIT;")
expect("@timestamp BSP_DISPLAY s21-ENTRY;")
expect("@timestamp BSP_DISPLAY s211-ENTRY;")
expect("@timestamp Trg-Done QS_RX_EVENT")

dispatch("E_SIG")
expect("@timestamp BSP_DISPLAY s2-E;")
expect("@timestamp BSP_DISPLAY s211-EXIT;")
expect("@timestamp BSP_DISPLAY s21-EXIT;")
expect("@timestamp BSP_DISPLAY s22-ENTRY;")
expect("@timestamp Trg-Done QS_RX_EVENT")

dispatch("F_SIG")
expect("@timestamp BSP_DISPLAY s2-F;")
expect("@timestamp BSP_DISPLAY s22-EXIT;")
expect("@timestamp BSP_DISPLAY s2-EXIT;")
expect("@timestamp BSP_DISPLAY s1-ENTRY;")
expect("@timestamp BSP_DISPLAY s11-ENTRY;")
expect("@timestamp BSP_DISPLAY s11-INIT;")
expect("@timestamp BSP_DISPLAY s111-ENTRY;")
expect("@timestamp Trg-Done QS_RX_EVENT")

dispatch("C_SIG")
expect("@timestamp BSP_DISPLAY s111-C;")
expect("@timestamp BSP_DISPLAY s111-EXIT;")
expect("@timestamp BSP_DISPLAY s112-ENTRY;")
expect("@timestamp Trg-Done QS_RX_EVENT")

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

#==========================================================
test("TstSM guards")
init()
expect("@timestamp BSP_DISPLAY top-INIT;")
expect("@timestamp BSP_DISPLAY s-ENTRY;")
expect("@timestamp BSP_DISPLAY s2-ENTRY;")
expect("@timestamp BSP_DISPLAY s2-INIT;")
expect("@timestamp BSP_DISPLAY s21-ENTRY;")
expect("@timestamp BSP_DISPLAY s211-ENTRY;")
expect("@timestamp Trg-Done QS_RX_EVENT")

dispatch("D_SIG")
expect("@timestamp BSP_DISPLAY s21-D;")
expect("@timestamp BSP_DISPLAY s211-EXIT;")
expect("@timestamp BSP_DISPLAY s21-EXIT;")
expect("@timestamp BSP_DISPLAY s2-INIT;")
expect("@timestamp BSP_DISPLAY s21-ENTRY;")
expect("@timestamp BSP_DISPLAY s211-ENTRY;")
expect("@timestamp Trg-Done QS_RX_EVENT")

dispatch("D_SIG")
expect("@timestamp Trg-Done QS_RX_EVENT")

dispatch("I_SIG")
expect("@timestamp BSP_DISPLAY s2-I;")
expect("@timestamp Trg-Done QS_RX_EVENT")

dispatch("I_SIG")
expect("@timestamp BSP_DISPLAY s-I;")
expect("@timestamp Trg-Done QS_RX_EVENT")

#==========================================================
test("TstSM history")
init()
expect("@timestamp BSP_DISPLAY top-INIT;")
expect("@timestamp BSP_DISPLAY s-ENTRY;")
expect("@timestamp BSP_DISPLAY s2-ENTRY;")
expect("@timestamp BSP_DISPLAY s2-INIT;")
expect("@timestamp BSP_DISPLAY s21-ENTRY;")
expect("@timestamp BSP_DISPLAY s211-ENTRY;")
expect("@timestamp Trg-Done QS_RX_EVENT")

note("shallow history")
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
command(1, 111) # is in state s111
expect("@timestamp CMD 1 111")
expect("@timestamp Trg-Done QS_RX_COMMAND")

dispatch("C_SIG")
expect("@timestamp BSP_DISPLAY s111-C;")
expect("@timestamp BSP_DISPLAY s111-EXIT;")
expect("@timestamp BSP_DISPLAY s112-ENTRY;")
expect("@timestamp Trg-Done QS_RX_EVENT")

dispatch("C_SIG")
expect("@timestamp BSP_DISPLAY s-C;")
expect("@timestamp BSP_DISPLAY s112-EXIT;")
expect("@timestamp BSP_DISPLAY s11-EXIT;")
expect("@timestamp BSP_DISPLAY s1-EXIT;")
expect("@timestamp BSP_DISPLAY s-EXIT;")
expect("@timestamp BSP_DISPLAY t-ENTRY;")
expect("@timestamp Trg-Done QS_RX_EVENT")
command(1, 0) # is in state s
expect("@timestamp CMD 0 0") # NO
expect("@timestamp Trg-Done QS_RX_COMMAND")

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

dispatch("C_SIG")
expect("@timestamp BSP_DISPLAY s211-C;")
expect("@timestamp BSP_DISPLAY s211-EXIT;")
expect("@timestamp BSP_DISPLAY s212-ENTRY;")
expect("@timestamp Trg-Done QS_RX_EVENT")

dispatch("C_SIG")
expect("@timestamp BSP_DISPLAY s-C;")
expect("@timestamp BSP_DISPLAY s212-EXIT;")
expect("@timestamp BSP_DISPLAY s21-EXIT;")
expect("@timestamp BSP_DISPLAY s2-EXIT;")
expect("@timestamp BSP_DISPLAY s-EXIT;")
expect("@timestamp BSP_DISPLAY t-ENTRY;")
expect("@timestamp Trg-Done QS_RX_EVENT")

note("deep history")
dispatch("E_SIG")
expect("@timestamp BSP_DISPLAY t-E->H*-s2;")
expect("@timestamp BSP_DISPLAY t-EXIT;")
expect("@timestamp BSP_DISPLAY s-ENTRY;")
expect("@timestamp BSP_DISPLAY s2-ENTRY;")
expect("@timestamp BSP_DISPLAY s21-ENTRY;")
expect("@timestamp BSP_DISPLAY s212-ENTRY;")
expect("@timestamp Trg-Done QS_RX_EVENT")
