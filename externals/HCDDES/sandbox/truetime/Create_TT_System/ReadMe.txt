How to use the TT network simulation generation package
Last Updated: 4-16-08
Peter Humke

In order to create a TT network from an existing simulink model the following are needed:

Original simulink block diagram:
-The original model must be split into sub-models which correspond to each of the nodes in the network.
 -These models must be named using the following convention:
 -Model name = node name + '_fn'

-Any additional blocks which are not supposed to be run on a node need to be put into another file:
 -This file must be named 'Additional_Blocks'
 -Preferably these blocks should be lined up vertically along the left side or else possible block overlaps will occur during the generation process.

-Finally, in order to actually connect the non-generated blocks with the generated blocks:
 -Create a data structure associated with the additional blocks that has the following format:

 additional_rules	-> array of structs
  from			-> name of node to connect from
  to			-> name of node to connect to

 -These fields contain the names of the non-generated and generated blocks to connect together during the generation process

Schedule:
-The schedule generated for the nodes will encapsulate all of the information required to generate the truetime model, therefore it must have the following format:
 -The schedule contains both network information and node information:

 schedule		-> struct
  name			-> string name of model to be created
  NetworkParameters	-> array of structs (one entry for each network)
 			-> the following identifiers are specific for truetime and more informaiton can be found about them in the truetime documentation
   network_type		-> type of network
   network_number	-> a unique identifier for the network
   num_nodes		-> the number of nodes contained within the network
   data_rate		-> data rate for the network
   min_frame_size	-> the minimum frame size for each node
   loss_prob		-> the probability that a packet is lost on the network
   bandwith_alloc	-> an allocation of the bandwidths for the nodes
   slotsize		-> the slot size for each of the nodes
   cyclic_sched		-> schedule for the nodes
   total_switch_mem	-> total memory available on the switch
  Nodes			-> array of structs (one for each node within the system)
   name			-> name of the node - must be associated with the struct names later
  hyperperiod		-> the hyperperiod size for the nodes
  <node_name>		-> replace <node_name> with each of the names in the previous nodes array, contains information about the node itself
   num_analog_inp	-> the number of analog inputs to the node
   num_analog_outp	-> the number of analog outputs from the node
   node_num		-> a unique identifier for the node within the network
   task_list		-> array of tasks to be run on the node
    task_name		-> name of task to be run
    start		-> start time of the task
    wcet		-> the worst case execution time for the task
   msg_sent_list	-> array of messages sent out by the node to the network
    msg_name		-> the name of the message
    task_name		-> name of task sending the message
    send_time		-> time the message is sent
    length		-> length of the sent message
   msg_rcv_list		-> array of messages received by the node from the network
    msg_name		-> the name of the message
    task_name		-> name of task receiving the message
    send_time		-> time the message is received
    length		-> length of the sent message

 -This is then used to generate the truetime blocks, and attach everything together.



Files contained within the Create_TT_System folder:

 init				-> this initializes the truetime block system and also adds the current directory to the search path so that the user can now simply move into the folder containing their model and run all the code generators from there
 Create_System			-> this generates the entire truetime representation of the requested model
				-> it requires a directory pointing to the locaiton of the treutime files (the env variable created by init)
				-> it requires the schedule file
				-> it requires the additional rules file
 Create_Generic_Code		-> this generates the code which will run on every node and parse the schedule file
 Create_TT_Network		-> this will generate the actual simulink model

Files contained within the example folder:

 schedule			-> this creates the schedule for the given model
 Additional_Rules		-> this creates an array which has the additional 
                           rules for the particular model we are using
			        	-> this should be replaced per model
 Additional_Import		-> this has all the non-generated portions of the 
                           model
 CompassDriver_fn		-> the model to run on the CompassDriver node
 Scaler_fn			-> the model to run on the Scaler node
 ServoDriver_fn			-> the model to run on the ServoDriver node
 Smoothing_fn			-> the model to run on the Smoothing node



Running the entire system:
-Within matlab:
-Switch to the Create_TT_System folder
 -Run init
-Switch to the example folder (or designated model folder)
 -Run Schedule
 -Run Additional_Rules
 -Run Create_TT_System(env, schedule, additional_rules)
