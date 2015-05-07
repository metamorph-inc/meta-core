#!/bin/bash

echo Running tests with the Debug version of Stage1...

echo -------------------------------------------------
echo Unconnected dataflow input port inside component.  
echo Should give an error in InnerLoop_InnerLoop_InnerLoop.  Ignore the rest
echo  qint_inputport_unconnected_error.mga
echo -------------------------------------------------
Stage1D -f qint_inputport_unconnected_error.mga -o qint_inputport_unconnected_error.xml

echo -------------------------------------------------
echo Unconnected dataflow output port inside component. 
echo Should give a warning in InnerLoop_InnerLoop_InnerLoop.
echo Should create a dummy message structure.
echo  qint_outputport_unconnected_dummy.mga
echo -------------------------------------------------
Stage1D -f qint_outputport_unconnected_dummy.mga -o qint_outputport_unconnected_dummy.xml

echo -------------------------------------------------
echo Unconnected field in an input message. .
echo Should give a warning for port vel of message ang_ref.
echo  qint_inmsgfield_unconnected_warning.mga
echo -------------------------------------------------
Stage1D -f qint_inmsgfield_unconnected_warning.mga -o qint_inmsgfield_unconnected_warning.xml

echo -------------------------------------------------
echo Unconnected field in an output message. 
echo Should give an error for port ang4 of message thrust_commands.
echo  qint_outmsgfield_unconnected_error.mga
echo -------------------------------------------------
Stage1D -f qint_outmsgfield_unconnected_error.mga -o qint_outmsgfield_unconnected_error.xml

echo -------------------------------------------------
echo Unconnected component input message port.
echo Should give errors for OuterLoop/pos_msg and InnerLoop/ang_ref. Ignore the rest.
echo  qint_inputmsg_unconnected_error.mga
echo -------------------------------------------------
Stage1D -f qint_inputmsg_unconnected_error.mga -o qint_inputmsg_unconnected_error.xml 

echo -------------------------------------------------
echo Unconnected component output message port.
echo Should give a warning for output message InnerLoop/thrust_commands.
echo  qint_outputmsg_unconnected_warning.mga
echo -------------------------------------------------
Stage1D -f qint_outputmsg_unconnected_warning.mga -o qint_outputmsg_unconnected_warning.xml

echo -------------------------------------------------
echo Partially connected message port -- CommMapping to a Bus channel, but no dependency.
echo Should give an error for four different CommMapping connections.
echo  qint_inputmsg_partially_connected_error.mga
echo -------------------------------------------------
Stage1D -f qint_inputmsg_partially_connected_error.mga -o qint_inputmsg_partially_connected_error.xml



