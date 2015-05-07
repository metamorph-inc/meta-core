Starmac Model Files

File Status as of 4/26/2008:

Models:
test_harness.mdl -- simple dual-quadrotor model.  The top instance is the original model,
and the bottom instance is the system under test.  The bottom instance is discretized, and
there some model changes to support that.  After a run, the Error Scope (top level) will
show the difference between the states.  The changelog file describes the change process
and observed results.  The discretized model still has a few issues, and has only been
tested on one input set.  It's a work in progress.  This model will not work with xPC 
target, so don't try to generate code for it.

discrete_starmac.mdl -- this is an extracted model from the harness, and is the one that
gets deployed to the xPC target.  The main difference is in the solver settings.  The 
test harness model uses variable-step methods to support the continuous-time original model.
The xPC model has to be fixed-step.  The current version has dummy (constant) inputs.  This
model is ready for xPC target.

commblock.mdl -- this is the original UDP comms test model.  The comms message has been
factored into a single subsystem block so it can be easily copied into other models.  This
model is ready for xPC target.

The *.m files support the Starmac model, setting global constants and so forth.
The harness model will invoke a callback that invokes them directly.  The other 
models do not have this, so you will need to run 'init_simulation' from the Matlab
command line before running or generating code from the model.

xPC Configuration:

From the Matlab command prompt, type 'xpcexplr'.  The resulting window allows configuration
of the xPC target.  Look in the Matlab help for basic setup.  Your default target 
configuration needs to have the following communications data for our setup:

Host target communication: TCP/IP
Target PC IP address: 129.59.129.37
TCP/IP target port: 22222
LAN subnet mask address: 255.255.255.0
TCP/IP gateway address: 129.59.129.1

The other target defaults should be sensible.  Right-click the TargetPC1, and choose
'connect'.

From your Simulink model, select 
Tools -> Real-Time Workshop -> Build Model

Wait a while, then look at your TargetPC configuration in the xPC explorer window.  You
should see a representation of your loaded model.  If it didn't work for some reason, check
the Matlab command-line window.  

Once the model is loaded, you can browse to it in the explorer.  You can set parameters for
the simulation run time, etc... (use 'Inf' for an endless loop).  Note: If you change 
sample rate, you must go back to the original Simulink model and re-build.  You may see a 
warning to that effect when you build the model.  

Here's the right way to change sample rate:

1. Open 'constants.m' in the editor.
2. Find the assignment 'plant_int_period'.  
3. Please do not change the formula, rather add a scaling to adjust the sample rate.
   (e.g. 0.5 * t_s/tau_motor or 2 * t_s/tau_motor).  This is period, not rate, so bigger
   is slower.  Changing to an absolute time will mess up rate transitions, all of which
   have to be integer multiple changes.  RTW will then kindly fail to generate your model.
4. Run constants.m from the command-line to update the workspace.
5. Use Tools -> Real-Time Workshop -> Build Model to re-generate the xPC model.
6. After testing, make a note in the changelog with your observations and the sample
   rate(s) that you used.
7. If you check your changes back in, make sure to check in the changelog as well.


