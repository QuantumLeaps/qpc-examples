# test-script for QUTest unit testing harness
# see https://www.state-machine.com/qtools/qutest.html

note('''
This test group demonstrates testing of the start sequence
by applying the BDD-style testing (alternative #1).
Specifically, a "Gizmo" AO needs to start either in the state
"on_mains", when the AO detects at runtime that main power
is available, or in the state "on_battery", when it is not.
However, the state machine semantics does not allow guard
conditions on the initial transition itself. A workaround is
to self-post a START event, which then triggers a transition
with a guard.
''')

def on_reset():
    expect_pause()

#------------------------------------------------------------------------------
SCENARIO('''
QActive_start(AO_Gizmo) with status==1,
should go to state "on_mains"
''')
GIVEN("status=1")
current_obj(OBJ_AP, 'status')
poke(0, 1, pack('<B', 1)) # status:=1

WHEN("Test sequence contunues AO_Gizmo is started")
glb_filter(GRP_ALL)
continue_test()
expect("===RTC===> St-Init  Obj=AO_Gizmo,State=QHsm_top->Gizmo_initialize")
expect("@timestamp AO-Post  Sdr=AO_Gizmo,Obj=AO_Gizmo,Evt<Sig=START_SIG,*")
expect("===RTC===> St-Entry Obj=AO_Gizmo,State=Gizmo_initialize")
expect("@timestamp Init===> Obj=AO_Gizmo,State=Gizmo_initialize")

THEN('''After QF_run(), AO_Gizmo receives START event,
which triggers transition to Gizmo_on_mains''')
expect_run()
expect("@timestamp AO-GetL  Obj=AO_Gizmo,Evt<Sig=START_SIG,*")
expect("@timestamp Disp===> Obj=AO_Gizmo,Sig=START_SIG,State=Gizmo_initialize")
expect("@timestamp USER+005 START")
expect("@timestamp ===>Tran Obj=AO_Gizmo,Sig=START_SIG,State=Gizmo_initialize->Gizmo_on_mains")

#------------------------------------------------------------------------------
SCENARIO('''
QACTIVE_START(AO_Gizmo) with status==0,
should go to state "on_battery"
''')
GIVEN("status=0")
current_obj(OBJ_AP, 'status')
poke(0, 1, pack('<B', 0))  # status:=0

WHEN("Test sequence contunues AO_Gizmo is started")
glb_filter(GRP_ALL)
continue_test()
expect("===RTC===> St-Init  Obj=AO_Gizmo,State=QHsm_top->Gizmo_initialize")
expect("@timestamp AO-Post  Sdr=AO_Gizmo,Obj=AO_Gizmo,Evt<Sig=START_SIG,*")
expect("===RTC===> St-Entry Obj=AO_Gizmo,State=Gizmo_initialize")
expect("@timestamp Init===> Obj=AO_Gizmo,State=Gizmo_initialize")

THEN('''After QF_run(), AO_Gizmo receives START event,
which triggers transition to Gizmo_on_battery''')
expect_run()
expect("@timestamp AO-GetL  Obj=AO_Gizmo,Evt<Sig=START_SIG,*")
expect("@timestamp Disp===> Obj=AO_Gizmo,Sig=START_SIG,State=Gizmo_initialize")
expect("@timestamp USER+005 START")
expect("@timestamp ===>Tran Obj=AO_Gizmo,Sig=START_SIG,State=Gizmo_initialize->Gizmo_on_battery")
