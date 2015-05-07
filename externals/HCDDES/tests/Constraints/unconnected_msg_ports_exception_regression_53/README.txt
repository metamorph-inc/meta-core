This is a variant of the quad integrator example that tests the unconnected
message ports problem (Trac issue #53).  In the design, the RefHandler 
component has an unconnected input port, which used to cause Stage1 to crash.
Fixing that, then Stage2 would crash.

The desired behavior is to generate the message structure for the unconnected
port, and include it in the call list of the task that includes it.  The data
for the message doesn't come from anywhere (for received messages) or go 
anywhere (for sent messages), but the code is still correct according to the
model.

qint_dh.mdl - Modified Simulink model
qint.mga - GME model
qint.xml - Fully processed ESMoL abstract model
qint.scs - Scheduler config file (1ms resolution)
qint.rslt - Scheduler results file

