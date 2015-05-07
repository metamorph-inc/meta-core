#ifndef TaskTypes_h__
#define TaskTypes_h__

// Define different kinds of tasks

#include "NWModel.h"

#include <iostream>
#include <string>
#include <exception>

class TTask;
typedef boost::shared_ptr< TTask > TTaskPtr;
class PTask;
typedef boost::shared_ptr< PTask > PTaskPtr;
class ATask;
typedef boost::shared_ptr< ATask > ATaskPtr;


class TTask : public Task
{
public:
	TTask() { }
	
	TTask( const std::string & name, ProcPtr parent, 
			double freq, double dur ) : 
		Task( name, parent, freq, dur ) { }
	virtual ~TTask() { }

protected:

	virtual const std::string TaskType() { return std::string( "TTask"); }
	virtual void PrintOtherParameters() { }
	
private:
};

class PTask : public Task
{
public:
	PTask() { }
	PTask( const std::string & name, ProcPtr parent, 
			double freq, double dur, double deadline ) : 
		Task( name, parent, freq, dur ), _deadline( deadline ) { }
	virtual ~PTask() { }

	double getDeadline() { return _deadline; }

protected:

	virtual const std::string TaskType() { return std::string( "PTask"); }
	virtual void PrintOtherParameters();
	
	double _deadline; 

private:
};

class ATask : public Task
{
public:
	ATask();
	ATask( const std::string & name, ProcPtr parent, 
			double freq, double dur, double deadline  ) : 
	Task( name, parent, freq, dur ), _deadline( deadline ) { }
	virtual ~ATask() { }

	double getDeadline() { return _deadline; }

protected:

	virtual const std::string TaskType() { return std::string( "ATask"); }
	virtual void PrintOtherParameters();
	
	double _deadline; 
	
private:
};



#endif // TaskTypes_h__