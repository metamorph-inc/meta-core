*** DESIGN QUESTIONS TO ANSWER

How do we represent the discretization of messages for the various types of buses?
  Possibilities: Have a callback that can be handed to the msg object that does the right thing.
                 Create a discretization object with a virtual interface that does the right thing.
				 or both
				 * Give each bus a call for each of the two message types that determines the 
				   proper transfer length (in seconds)

AFDXBus - does each sender get one channel, or can we have a single channel that gets the bandwidth
  of multiple channels?  I think it can, in powers of two.  That is not represented yet.  How do we
  model that?  It should be part of the specification for processors on the bus.

TTEBus - how do we know which type of traffic we have?  Currently we have to assume that a message
  is sent over the time-triggered part or the non-time-triggered part.  That needs to be specified.
  Is the granularity of the traffic at the message level or at the processor level?

*** THINGS TO DO (deferred cleanups and features):

REMEMBER: Is that a good idea? == What are the use cases that this touches?

Can we factor the parent <-> child ( w/ callback ) functionality into a 
pair of mix-in classes?  Is that a good idea?

(related to the last one) Can we make Msg have a generic hardware parent
instead of ProcMsg and other msgs being defined separately?  We would have
to use a dynamic cast.  Is that a good idea?  For now we're using redundancy
rather than using this approach.

* Recommend to Whole Tomato to add a refactoring to move a file in the vsproj
  into a different directory, updating the project, solution, and any references
  to it (i.e. adding include paths or changing #include definitions).

* Also "move a class to its own header/implementation file" would be a good refactoring.


**** IDEAS (partial designs that need more thinking):

1.	We may want to create a generic frame for storing abstracted and transformed
	versions of a problem tree:

	1.  We may have different versions of the problem tree (like tt vs partitioned),
	    so the structure needs to be flexible enough to handle the differences.

	2.  Do we need to distinguish between abstractions and transformations?
		Think about it.

	3.	The generic tree is created by traversing the problem tree and instantiating the 
		generic parts.  References back to the appropriate problem elements should be 
		stored in each of the generic elements.

	4.  A single problem tree instance may have several generic trees associated with it,
		so we need to think about the interfaces.  For example, we may have a generic 
		tree holding a discretization model for the problem tree, or an ordering model.
		Those models would include the data to represent each problem element as either
		a discretized space or an ordering, and keep track of instance numbers in the 
		respective finite domain problem.   This may also be where we put together the
		hierarchical scheduling model and its pieces (tranformation).

	5.	We want to be able to set up an atomic interface to a particular generic structure
		for a given problem: 
		1.	create the generic structure from this problem
		2.	traverse the generic structure and do the following operation
		3.	select a particular generic structure from the collection available for this
			particular problem

	6.	We may also want to relate two or more generic structures in some way -- perhaps
		creating a second generic structure linked back to the first one rather than directly
		to the problem.

	7.	Do we need to have the external interface able to look up particular elements and
		values, or will most of the accesses be through traversals and other aggregate 
		operations?

This hearkens back to the work of Don Batory on categorical structures for 
software engineering.  We are actually instantiating mappings between problem structures
and various abstractions and transformations on those structures.  We want those transformations,
structures, and the later operations on those structures, to behave as a single coherent
entity -- we want to have a function that says 'make one of these related structures',
which would perform the transformation, and other functions like 'create a set of constraints
from this generic structure' which when handed a proper container would traverse the whole
generic structure and do it. 



