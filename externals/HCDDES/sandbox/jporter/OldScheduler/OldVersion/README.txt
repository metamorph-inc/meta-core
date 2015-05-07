CSched README 6/1/2007

CSched is a simple language for exploring constraint-based generation 
of real-time schedules.  It has many limitations, only some of which are documented here.  

Current limitations:
Task periods are strict (exact spacing between instances of any given task within the schedule).
Latency constraints are not generated from the models.


The file AllInOne.sln currently contains three projects:
1. AllInOne
2. InstancesAndConstraints
3. SolveConstraints

These three interpreters are written using UDM and are based on a 
common code set (found in directory Shared) for different purposes:

AllInOne - single standalone command-line interpreter.  Input is an mga-file containing one or
more System models.  For each of those system models two sets of files are created if the 
system description schedule is feasible:

1. A single Matlab m-file script which will draw the generated schedule if invoked in Matlab.
2. A set of text files (.scd) which contain the schedule for each processor and bus in the 
   system.  Each line has the form: <task or msg name>   <hyperperiod>   <start time (offset from 0)>

Note: The AllInOne interpreter successively invokes the code in the following two interpreters, and then 
   formats the output.  The next two interpreter versions are meant to be run from within GME.

InstancesAndConstraints - from a System model this interpreter creates two new models -- an instances model
   and a contraint model.

   The interpreter code automatically generates the correct number of task and message instances from the hyper-
   period length, the specified period for the tasks, and the inferred relationship for the messages.

   The constraint model is created automatically to capture the dependencies and relationships between
   tasks, messages, and the processor/bus topology on which they live.  The language only captures
   constraints for equality (x=y+constant), inequality (x<=y+constant), and serialization (no two tasks/msgs 
   in a given set may overlap).

SolveConstraints - the interpreter translates the constraint model into calls to the Gecode constraint
   solution library.  The solver is run according to the specified search criteria, and then solutions
   (if any) are written as model entities back to the constraint models.  If a SearchConfig entity is present
   in the model, then its configuration will control the number of solutions created.  The current default is
   10 solutions for the interactive interpreter, but only one for the AllInOne standalone interpreter.


Running things :

   InstancesAndConstraints and SolveContraints can be added using the "Register Components" entry in the GME
   File menu.  

   AllInOne is run directly on the model from the command line:

   > AllInOne.exe my_rt_model.mga

   If the schedule is feasible, all of the files described above will be created in the current working directory.