# test-script for QUTest unit testing harness
# see https://www.state-machine.com/qtools/qutest.html/qutest.html

note('''
This test group verifies the preemption tests
in the QXK preemptive kernel
''')

# preamble
def on_reset():
    expect_pause()
    # don't call continue_test() yet
    # this will be done by the individual tests
    # after they poke the priorities of threads

#------------------------------------------------------------------------------
SCENARIO("extened->basic")

GIVEN("basic thread priorities: 4,0,0")
current_obj(OBJ_AP, "pspecB")
poke(0, 2, pack("<HHH", 4, 0, 0))
AND("extended thread priorities: 4,0,0")
current_obj(OBJ_AP, "pspecX")
poke(0, 2, pack("<HHH", 1, 0, 0))
continue_test()
expect_run()
glb_filter(GRP_SC, GRP_UA)

WHEN("post TEST0_SIG->thrX[0]")
current_obj(OBJ_AO, "thrX[0]")
post("TEST0_SIG")

THEN("Extpect the following preemptions")
expect("@timestamp Sch-Next Pri=0->1")
expect("@timestamp CONTEXT_SW NULL thrX[0]")
expect("@timestamp TRACE_MSG thrX[0] TEST0 1of2")
expect("@timestamp Sch-Next Pri=1->4")
expect("@timestamp CONTEXT_SW thrX[0] aoB[0]")
expect("@timestamp TRACE_MSG aoB[0] TEST1 1of1")
expect("@timestamp Sch-Next Pri=4->1")
expect("@timestamp CONTEXT_SW aoB[0] thrX[0]")
expect("@timestamp TRACE_MSG thrX[0] TEST0 2of2")
expect("@timestamp TRACE_MSG thrX[0] TEST1 1of2")
expect("@timestamp Sch-Lock Ceil=0->4")
expect("@timestamp Sch-Unlk Ceil=4->0")
expect("@timestamp Sch-Next Pri=1->4")
expect("@timestamp CONTEXT_SW thrX[0] aoB[0]")
expect("@timestamp TRACE_MSG aoB[0] TEST2 1of1")
expect("@timestamp Sch-Next Pri=4->1")
expect("@timestamp CONTEXT_SW aoB[0] thrX[0]")
expect("@timestamp TRACE_MSG thrX[0] TEST1 2of2")
expect("@timestamp TRACE_MSG thrX[0] TEST2 1of1")
expect("@timestamp Sch-Idle Pri=1->0")
expect("@timestamp CONTEXT_SW thrX[0] NULL")
expect("@timestamp Trg-Done QS_RX_EVENT")

#------------------------------------------------------------------------------
SCENARIO("extended->extened->basic")

GIVEN("basic thread priorities: 1,2,3")
current_obj(OBJ_AP, "pspecB")
poke(0, 2, pack("<HHH", 1, 2, 3))
AND("extended thread priorities: 4,5,0")
current_obj(OBJ_AP, "pspecX")
poke(0, 2, pack("<HHH", 4, 5, 0))
continue_test()
expect_run()
glb_filter(GRP_SC, GRP_UA)

WHEN("post TEST0_SIG->thrX[0]")
current_obj(OBJ_AO, "thrX[0]")
post("TEST0_SIG")

THEN("Extpect the following preemptions")
expect("@timestamp Sch-Next Pri=0->4")
expect("@timestamp CONTEXT_SW NULL thrX[0]")
expect("@timestamp TRACE_MSG thrX[0] TEST0 1of2")
expect("@timestamp Sch-Next Pri=4->5")
expect("@timestamp CONTEXT_SW thrX[0] thrX[1]")
expect("@timestamp TRACE_MSG thrX[1] TEST1 1of2")
expect("@timestamp Sch-Lock Ceil=0->5")
expect("@timestamp Sch-Unlk Ceil=5->0")
expect("@timestamp TRACE_MSG thrX[1] TEST1 2of2")
expect("@timestamp TRACE_MSG thrX[1] TEST2 1of1")
expect("@timestamp Sch-Next Pri=5->4")
expect("@timestamp CONTEXT_SW thrX[1] thrX[0]")
expect("@timestamp TRACE_MSG thrX[0] TEST0 2of2")
expect("@timestamp TRACE_MSG thrX[0] TEST1 1of2")
expect("@timestamp Sch-Lock Ceil=0->5")
expect("@timestamp Sch-Unlk Ceil=5->0")
expect("@timestamp Sch-Next Pri=4->5")
expect("@timestamp CONTEXT_SW thrX[0] thrX[1]")
expect("@timestamp TRACE_MSG thrX[1] TEST2 1of1")
expect("@timestamp Sch-Next Pri=5->4")
expect("@timestamp CONTEXT_SW thrX[1] thrX[0]")
expect("@timestamp TRACE_MSG thrX[0] TEST1 2of2")
expect("@timestamp TRACE_MSG thrX[0] TEST2 1of1")
expect("@timestamp TRACE_MSG thrX[0] TEST2 1of1")
expect("@timestamp Sch-Next Pri=4->3")
expect("@timestamp CONTEXT_SW thrX[0] aoB[2]")
expect("@timestamp TRACE_MSG aoB[2] TEST1 1of1")
expect("@timestamp TRACE_MSG aoB[2] TEST2 1of1")
expect("@timestamp TRACE_MSG aoB[2] TEST2 1of1")
expect("@timestamp Sch-Next Pri=3->2")
expect("@timestamp CONTEXT_SW aoB[2] aoB[1]")
expect("@timestamp TRACE_MSG aoB[1] TEST1 1of1")
expect("@timestamp TRACE_MSG aoB[1] TEST2 1of1")
expect("@timestamp TRACE_MSG aoB[1] TEST2 1of1")
expect("@timestamp Sch-Next Pri=2->1")
expect("@timestamp CONTEXT_SW aoB[1] aoB[0]")
expect("@timestamp TRACE_MSG aoB[0] TEST1 1of1")
expect("@timestamp TRACE_MSG aoB[0] TEST2 1of1")
expect("@timestamp TRACE_MSG aoB[0] TEST2 1of1")
expect("@timestamp Sch-Idle Pri=1->0")
expect("@timestamp CONTEXT_SW aoB[0] NULL")
expect("@timestamp Trg-Done QS_RX_EVENT")


#------------------------------------------------------------------------------
SCENARIO("extened->basic->extended")

GIVEN("basic thread priorities: 4,0,0")
current_obj(OBJ_AP, "pspecB")
poke(0, 2, pack("<HHH", 4, 0, 0))
AND("extended thread priorities: 1,5,0")
current_obj(OBJ_AP, "pspecX")
poke(0, 2, pack("<HHH", 1, 5, 0))
continue_test()
expect_run()
glb_filter(GRP_SC, GRP_UA)

WHEN("post TEST0_SIG->thrX[0]")
current_obj(OBJ_AO, "thrX[0]")
post("TEST0_SIG")

THEN("Extpect the following preemptions")
expect("@timestamp Sch-Next Pri=0->1")
expect("@timestamp CONTEXT_SW NULL thrX[0]")
expect("@timestamp TRACE_MSG thrX[0] TEST0 1of2")
expect("@timestamp Sch-Next Pri=1->5")
expect("@timestamp CONTEXT_SW thrX[0] thrX[1]")
expect("@timestamp TRACE_MSG thrX[1] TEST1 1of2")
expect("@timestamp Sch-Lock Ceil=0->5")
expect("@timestamp Sch-Unlk Ceil=5->0")
expect("@timestamp TRACE_MSG thrX[1] TEST1 2of2")
expect("@timestamp TRACE_MSG thrX[1] TEST2 1of1")
expect("@timestamp Sch-Next Pri=5->4")
expect("@timestamp CONTEXT_SW thrX[1] aoB[0]")
expect("@timestamp TRACE_MSG aoB[0] TEST1 1of1")
expect("@timestamp TRACE_MSG aoB[0] TEST2 1of1")
expect("@timestamp Sch-Next Pri=4->1")
expect("@timestamp CONTEXT_SW aoB[0] thrX[0]")
expect("@timestamp TRACE_MSG thrX[0] TEST0 2of2")
expect("@timestamp TRACE_MSG thrX[0] TEST1 1of2")
expect("@timestamp Sch-Lock Ceil=0->5")
expect("@timestamp Sch-Unlk Ceil=5->0")
expect("@timestamp Sch-Next Pri=1->5")
expect("@timestamp CONTEXT_SW thrX[0] thrX[1]")
expect("@timestamp TRACE_MSG thrX[1] TEST2 1of1")
expect("@timestamp Sch-Next Pri=5->4")
expect("@timestamp CONTEXT_SW thrX[1] aoB[0]")
expect("@timestamp TRACE_MSG aoB[0] TEST2 1of1")
expect("@timestamp Sch-Next Pri=4->1")
expect("@timestamp CONTEXT_SW aoB[0] thrX[0]")
expect("@timestamp TRACE_MSG thrX[0] TEST1 2of2")
expect("@timestamp TRACE_MSG thrX[0] TEST2 1of1")
expect("@timestamp TRACE_MSG thrX[0] TEST2 1of1")
expect("@timestamp Sch-Idle Pri=1->0")
expect("@timestamp CONTEXT_SW thrX[0] NULL")
expect("@timestamp Trg-Done QS_RX_EVENT")
