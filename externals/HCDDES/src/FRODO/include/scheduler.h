/*** Copyright Vanderbilt ***/


#ifndef __FRODO_SCHEDULER_H__
#define __FRODO_SCHEDULER_H__


#ifdef __cplusplus
extern "C" {
#endif


/*** Included Header Files ***/
#include "arch.h"
#include "error_handler.h"


/*****************************************************************************/


/*** Enums for Restricted Resources ***/
typedef enum {
	NoRestriction				= 0x00000000,
	ResourceA					= 0x00000001,
	ResourceB					= 0x00000002,
	ResourceC					= 0x00000004,
	ResourceD					= 0x00000008,
	ResourceE					= 0x00000010,
	ResourceF					= 0x00000020,
	ResourceG					= 0x00000040,
	ResourceH					= 0x00000080,
	ResourceI					= 0x00000100,
	ResourceJ					= 0x00000200,
	ResourceK					= 0x00000400,
	ResourceL					= 0x00000800,
	ResourceM					= 0x00001000,
	ResourceN					= 0x00002000,
	ResourceO					= 0x00004000,
	ResourceP					= 0x00008000
} SchedRestrictedResources;


/*** enums For Event Types: ***/
 typedef enum {
	 ExecutionStart,
	 ExecutionEnd,
	 HyperperiodStart,
	 HyperperiodEnd,
	 TaskStart,
	 TaskEnd,
	 SporadicJobCreation,
	 SporadicJobRemoval
 } SchedEventType;

 
/*** enums For Scheduler Type: ***/
typedef enum {
	RateMonotonic,
	EarliestDeadlineFirst
} SchedSporadicType;


/*** enums For Thread Priority ***/
typedef enum {
	ErrorThread,
	SchedulerThread,
	AsyncReceiveThread,
	TaskThread
} SchedThreadPriority;


/*****************************************************************************/


/* Structure for each schedulable item */
struct SchedInstance_;
typedef struct {
	char						name[FS_MAXTASKNAMELENGTH];	// Name of the schedulable task
	void*						(*ExecFunc)(void*);			// Pointer to the execution function
	pfloat_t					wcetMS;						// Worst-case execution time in milliseconds
	uint8_t						priority;					// Priority of the task
	uint32_t					frequencyHZ;				// maximum frequency of the task
	uint32_t					restrictions;				// Resource restrictions
	struct SchedInstance_		*execInst;					// Instance for which execution was scheduled (varies)
	pfloat_t					execInit;					// Time at which execution started (varies)
	pfloat_t					execHalt;					// Time at which execution halted (varies)
	Semaphore*					semaphore;					// Semaphore for this schedulable task
	pthread_t					thread;						// Execution thread for the task
} SchedSchedulable;


/* Structure for each instance of a schedulable item */
typedef struct SchedInstance_ {
	SchedSchedulable*			schedulable;				// Pointer to the schedulable structure
	pfloat_t					timevalMS;					// (Periodic:next instance start)(Sporadic: relative deadline)
	struct SchedInstance_*		next;						// Pointer to the next struct in a list
} SchedInstance;


/*** Enums For Error Conditions: ***/
#define FS_ERR_SLEEPDEADLINEMISS			0x0001
#define FS_ERR_TASKWCETOVERRUN				0x0002
#define FS_ERR_SPORADICDEADLINEMISS			0x0003
#define FS_ERR_SPORADICTOOOFTEN				0x0004
#define FS_ERR_PERIODICCREATEFAILURE		0x0005
#define FS_ERR_SPORADICCREATEFAILURE		0x0006
#define FS_ERR_INITFAILURE					0x0007
#define FS_ERR_EXECTIMEOUTSIDEHYPERPERIOD	0x0008
#define FS_ERR_SCHEDFIFOUNAVAILABLE			0x0009
#define FS_ERR_SCHEDPRIORITYNOTSET			0x0010
#define FS_ERR_SETAFFINITYFAIL				0x0011


/*****************************************************************************/


/* Initialize the scheduler
 *	- Arguments: scheduler for sporadic tasks, error handling function, 
 *		hyperperiod duration, user context pointer
 *	- Returns: true on success, false on failure							*/
bool SchedInitialize(
		SchedSporadicType	scheduleType,					// Scheduler type for sporadic tasks
		void (*ErrorHandler)(ErrorCode,void*,pfloat_t),	// Error handler for the node
		pfloat_t			hyperperiodMS,					// Hyperperiod in milliseconds
		uint8_t				maxParallelTasks,				// Number of allowable parallel tasks
		void*				context							// Arbitrary context data
	);


/* Create a periodic task
 *	- Arguments: worst-case execution time, number of scheduled instances, array of the instance times
 *	- Returns: true if success, false if failure							*/
bool SchedCreatePeriodicTask(
		char*				name,							// Name of the task
		void*(*ExecFunc)(void*),							// Function to be called for execution
		SchedSchedulable**	schedulable,					// Double pointer for task schedulable
		pfloat_t			wcetMS,							// Worst-case execution time of the task
		uint32_t			restrictions,					// Resource restrictions for this task
		uint8_t				instanceCount,					// Number of scheduled instances
		pfloat_t*			instances						// Array of floats (in ms) of the instances
	);


/* Creates a sporadic task - to be instantiated later
 *	- Arguments: pointer to task function, WCET, priority and execution frequency
 *	- Returns: true if success, false if failure							*/
bool SchedCreateSporadicTask(
		char*				name,							// Name of the task
		void*(*ExecFunc)(void*),							// Pointer to exec function for this task
		SchedSchedulable**	schedulable,					// Double pointer for task schedulable
		pfloat_t			wcetMS,							// Worst-case execution time in milliseconds
		uint32_t			restrictions,					// Resource restrictions for this task
		uint8_t				priority,						// Priority of the task
		uint16_t			frequencyHZ						// Maximum frequency at which task can execute
	);


/* Create an instance of a sporadic task
 *	Arguments: schedulable to have job created for, deadline for that job
 *	Return: true if job was created, false otherwise						*/
bool SchedCreateJob(
		SchedSchedulable*	schedulable,					// Pointer to the schedulable item (see CreateTask)
		pfloat_t			relativeDeadlineMS				// Deadline in MS (relative to time CreateJob is called)
	);


/* Execute the scheduler for X hyperperiods
 *	- Arguments: number of hyperperiods to execute - 999999 signals no limit
 *	- Return: False if a fatal error was encountered, true otherwise		*/
bool SchedExecute(
		uint32_t			numHyperperiods					// Number of hyperperiods to execute
	);


/* Lets tasks see if they should continue running
 *	- Arguments: schedulable = information for the thread starting execution
 *	- Return: None															*/
void SchedSignalExecution(
		SchedSchedulable*	schedulable						// The schedulable for which to signal execution
	);


/* Lets tasks signal their completion and check to see
 * if they should continue running.
 *	- Arguments: schedulable = information for the thread completing execution
 *	- Return: None															*/
void SchedSignalCompletion(
		SchedSchedulable*	schedulable						// Schedulable for which to signal completion
	);


/* Shutdown the scheduler and all of the outstanding task threads
 *	- Arguments: None
 *	- Return: True if shutdown was successful, false otherwise				*/
bool SchedShutdown( void );


/*****************************************************************************/


#ifdef __cplusplus
}
#endif


#endif // __FRODO_SCHEDULER_H__

