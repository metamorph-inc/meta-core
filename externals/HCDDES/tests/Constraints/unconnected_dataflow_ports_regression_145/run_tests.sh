#!/bin/bash

echo Running tests with the Debug version of Stage1...

echo -------------------------------------------------
echo 1. Unconnected dataflow input port inside component.  
echo Should give an error in InnerLoop_InnerLoop_InnerLoop.  
echo Ignore other warnings.
echo  qint_inputport_unconnected_error.mga
echo -------------------------------------------------
Stage1D -f qint_inputport_unconnected_error.mga -o qint_inputport_unconnected_error.xml | grep -i -e "Warning\|Error\|Exception"

echo -------------------------------------------------
echo 2. Unconnected dataflow output port inside component.  
echo Should give a warning in InnerLoop_InnerLoop_InnerLoop.
echo Should create a dummy message structure.
echo  qint_outputport_unconnected_dummy.mga
echo -------------------------------------------------
Stage1D -f qint_outputport_unconnected_dummy.mga -o qint_outputport_unconnected_dummy.xml | grep -i -e "Warning\|Error\|Exception"
Stage2D -f qint_outputport_unconnected_dummy.xml -o qint_outputport_unconnected_dummy.scs -t Sched -r 1ms
SchedTool -f qint_outputport_unconnected_dummy.scs -o qint_outputport_unconnected_dummy.rslt
SchedResults -r qint_outputport_unconnected_dummy.rslt -a qint_outputport_unconnected_dummy.xml
Stage2D -f qint_outputport_unconnected_dummy.xml -o qint_outputport_unconnected_dummy -p qint_outputport_unconnected_dummy_tt -t TrueTime
Stage2D -f qint_outputport_unconnected_dummy.xml -o qint_outputport_unconnected_dummy -p qint_outputport_unconnected_dummy_frodo -t FRODO


echo -------------------------------------------------
echo 3. Unconnected field in an input message.  
echo Should give a warning for port vel of message ang_ref.
echo  qint_inmsgfield_unconnected_warning.mga
echo -------------------------------------------------
Stage1D -f qint_inmsgfield_unconnected_warning.mga -o qint_inmsgfield_unconnected_warning.xml | grep -i -e "Warning\|Error\|Exception"

echo -------------------------------------------------
echo 4. Unconnected field in an output message.  
echo Should give an error for port ang4 of message thrust_commands.
echo  qint_outmsgfield_unconnected_error.mga
echo -------------------------------------------------
Stage1D -f qint_outmsgfield_unconnected_error.mga -o qint_outmsgfield_unconnected_error.xml | grep -i -e "Warning\|Error\|Exception"

echo -------------------------------------------------
echo 5. Unconnected component input message port. 
echo Should give errors for OuterLoop/pos_msg and InnerLoop/ang_ref. 
echo Ignore other warnings.
echo  qint_inputmsg_unconnected_error.mga
echo -------------------------------------------------
Stage1D -f qint_inputmsg_unconnected_error.mga -o qint_inputmsg_unconnected_error.xml | grep -i -e "Warning\|Error\|Exception"

echo -------------------------------------------------
echo 6. Unconnected component output message port. 
echo Should give a warning for output message InnerLoop/thrust_commands.
echo Ignore other warnings.
echo  qint_outputmsg_unconnected_warning.mga
echo -------------------------------------------------
Stage1D -f qint_outputmsg_unconnected_warning.mga -o qint_outputmsg_unconnected_warning.xml | grep -i -e "Warning\|Error\|Exception"
Stage2D -f qint_outputmsg_unconnected_warning.xml -o qint_outputmsg_unconnected_warning.scs -t Sched -r 1ms
SchedTool -f qint_outputmsg_unconnected_warning.scs -o qint_outputmsg_unconnected_warning.rslt
SchedResults -r qint_outputmsg_unconnected_warning.rslt -a qint_outputmsg_unconnected_warning.xml
Stage2D -f qint_outputmsg_unconnected_warning.xml -o qint_outputmsg_unconnected_warning -p qint_outputmsg_unconnected_warning_tt -t TrueTime
Stage2D -f qint_outputmsg_unconnected_warning.xml -o qint_outputmsg_unconnected_warning -p qint_outputmsg_unconnected_warning_frodo -t FRODO

echo -------------------------------------------------
echo 7. Partially connected message port -- 
echo      CommMapping to a Bus channel, but no dependency.  
echo Should give an error for four different CommMapping connections.
echo  qint_inputmsg_partially_connected_error.mga
echo -------------------------------------------------
Stage1D -f qint_inputmsg_partially_connected_error.mga -o qint_inputmsg_partially_connected_error.xml | grep -i -e "Warning\|Error\|Exception"

echo -------------------------------------------------
echo 8. Unconnected dataflow output port inside component.  
echo Should give a warning in InnerLoop_InnerLoop_InnerLoop, and in
echo  DataHandling_DataHandling_DataHandling.
echo Ignore other warnings.
echo Should create a dummy message structure with two fields.
echo  qint_outputport_unconnected_2dummy.mga
echo -------------------------------------------------
Stage1D -f qint_outputport_unconnected_2dummy.mga -o qint_outputport_unconnected_2dummy.xml | grep -i -e "Warning\|Error\|Exception"
Stage2D -f qint_outputport_unconnected_2dummy.xml -o qint_outputport_unconnected_2dummy.scs -t Sched -r 1ms
SchedTool -f qint_outputport_unconnected_2dummy.scs -o qint_outputport_unconnected_2dummy.rslt
SchedResults -r qint_outputport_unconnected_2dummy.rslt -a qint_outputport_unconnected_2dummy.xml
Stage2D -f qint_outputport_unconnected_2dummy.xml -o qint_outputport_unconnected_2dummy -p qint_outputport_unconnected_2dummy_tt -t TrueTime
Stage2D -f qint_outputport_unconnected_2dummy.xml -o qint_outputport_unconnected_2dummy -p qint_outputport_unconnected_2dummy_frodo -t FRODO

echo -------------------------------------------------
echo 9. Partially connected message port --
echo     No CommMapping or Dependency, but it still has an execinfo object.
echo Should give an error for output message InnerLoop/thrust_commands.
echo Ignore other warnings.
echo  qint_outputmsg_partial_execinfo_error.mga
echo -------------------------------------------------
Stage1D -f qint_outputmsg_partial_execinfo_error.mga -o qint_outputmsg_partial_execinfo_error.xml | grep -i -e "Warning\|Error\|Exception"

