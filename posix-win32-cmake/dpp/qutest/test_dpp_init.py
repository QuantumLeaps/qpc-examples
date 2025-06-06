# test-script for QUTest unit testing harness
# see https://www.state-machine.com/qtools/qutest.html

# preamble...
def on_reset():
    expect_pause()
    glb_filter(GRP_ALL)
    continue_test()
    expect("@timestamp AO-Subsc Obj=Philo_inst[0],Sig=EAT_SIG")
    expect("@timestamp AO-Subsc Obj=Philo_inst[0],Sig=TEST_SIG")
    expect("===RTC===> St-Init  Obj=Philo_inst[0],State=QHsm_top->Philo_thinking")
    expect("@timestamp BSP_CALL BSP_random *")
    expect("@timestamp TE0-Arm  Obj=Philo_inst[0].timeEvt,AO=Philo_inst[0],Tim=*,Int=0")
    expect("===RTC===> St-Entry Obj=Philo_inst[0],State=Philo_thinking")
    expect("@timestamp Init===> Obj=Philo_inst[0],State=Philo_thinking")
    expect("@timestamp AO-Subsc Obj=Philo_inst[1],Sig=EAT_SIG")
    expect("@timestamp AO-Subsc Obj=Philo_inst[1],Sig=TEST_SIG")
    expect("===RTC===> St-Init  Obj=Philo_inst[1],State=QHsm_top->Philo_thinking")
    expect("@timestamp BSP_CALL BSP_random *")
    expect("@timestamp TE0-Arm  Obj=Philo_inst[1].timeEvt,AO=Philo_inst[1],Tim=*,Int=0")
    expect("===RTC===> St-Entry Obj=Philo_inst[1],State=Philo_thinking")
    expect("@timestamp Init===> Obj=Philo_inst[1],State=Philo_thinking")
    expect("@timestamp AO-Subsc Obj=Philo_inst[2],Sig=EAT_SIG")
    expect("@timestamp AO-Subsc Obj=Philo_inst[2],Sig=TEST_SIG")
    expect("===RTC===> St-Init  Obj=Philo_inst[2],State=QHsm_top->Philo_thinking")
    expect("@timestamp BSP_CALL BSP_random *")
    expect("@timestamp TE0-Arm  Obj=Philo_inst[2].timeEvt,AO=Philo_inst[2],Tim=*,Int=0")
    expect("===RTC===> St-Entry Obj=Philo_inst[2],State=Philo_thinking")
    expect("@timestamp Init===> Obj=Philo_inst[2],State=Philo_thinking")
    expect("@timestamp AO-Subsc Obj=Philo_inst[3],Sig=EAT_SIG")
    expect("@timestamp AO-Subsc Obj=Philo_inst[3],Sig=TEST_SIG")
    expect("===RTC===> St-Init  Obj=Philo_inst[3],State=QHsm_top->Philo_thinking")
    expect("@timestamp BSP_CALL BSP_random *")
    expect("@timestamp TE0-Arm  Obj=Philo_inst[3].timeEvt,AO=Philo_inst[3],Tim=*,Int=0")
    expect("===RTC===> St-Entry Obj=Philo_inst[3],State=Philo_thinking")
    expect("@timestamp Init===> Obj=Philo_inst[3],State=Philo_thinking")
    expect("@timestamp AO-Subsc Obj=Philo_inst[4],Sig=EAT_SIG")
    expect("@timestamp AO-Subsc Obj=Philo_inst[4],Sig=TEST_SIG")
    expect("===RTC===> St-Init  Obj=Philo_inst[4],State=QHsm_top->Philo_thinking")
    expect("@timestamp BSP_CALL BSP_random *")
    expect("@timestamp TE0-Arm  Obj=Philo_inst[4].timeEvt,AO=Philo_inst[4],Tim=*,Int=0")
    expect("===RTC===> St-Entry Obj=Philo_inst[4],State=Philo_thinking")
    expect("@timestamp Init===> Obj=Philo_inst[4],State=Philo_thinking")
    expect("@timestamp AO-Subsc Obj=Table_inst,Sig=DONE_SIG")
    expect("@timestamp AO-Subsc Obj=Table_inst,Sig=PAUSE_SIG")
    expect("@timestamp AO-Subsc Obj=Table_inst,Sig=SERVE_SIG")
    expect("@timestamp AO-Subsc Obj=Table_inst,Sig=TEST_SIG")
    expect("@timestamp BSP_CALL BSP_displayPhilStat 0 thinking")
    expect("@timestamp BSP_CALL BSP_displayPhilStat 1 thinking")
    expect("@timestamp BSP_CALL BSP_displayPhilStat 2 thinking")
    expect("@timestamp BSP_CALL BSP_displayPhilStat 3 thinking")
    expect("@timestamp BSP_CALL BSP_displayPhilStat 4 thinking")
    expect("===RTC===> St-Init  Obj=Table_inst,State=QHsm_top->Table_serving")
    expect("===RTC===> St-Entry Obj=Table_inst,State=Table_serving")
    expect("@timestamp Init===> Obj=Table_inst,State=Table_serving")
    expect_run()

test("DPP init")
