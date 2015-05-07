#ifndef DiscTaskTypes_h__
#define DiscTaskTypes_h__

#include "DiscNWModel.h"
#include "TaskTypes.h"

class DiscTTask;
typedef boost::shared_ptr< DiscTTask > DiscTTaskPtr;
class DiscPTask;
typedef boost::shared_ptr< DiscPTask > DiscPTaskPtr;
class DiscATask;
typedef boost::shared_ptr< DiscATask > DiscATaskPtr;

class DiscTTask : public DiscTask
{
public:

	DiscTTask() { }
	DiscTTask( TTaskPtr t, DiscProcPtr parent ) : 
		DiscTask( t, parent ), _ttask_ref( t ) { }
	virtual ~DiscTTask() { }

	virtual void Print();
	virtual void DiscretizeTask( double res );

protected:

	TTaskPtr _ttask_ref;
};

class DiscPTask : public DiscTask
{
public:

	DiscPTask() { }
	DiscPTask( PTaskPtr t, DiscProcPtr parent ) : 
		DiscTask( t, parent ), _ptask_ref( t ) { }
	virtual ~DiscPTask() { }

	virtual Ticks getDeadlineTicks() { return _deadline_ticks; }

	virtual void Print();
	virtual void DiscretizeTask( double res );

protected:

	PTaskPtr	_ptask_ref;
	Ticks		_deadline_ticks;
};

class DiscATask : public DiscTask
{
public:

	DiscATask() { }
	DiscATask( ATaskPtr t, DiscProcPtr parent ) : 
		DiscTask( t, parent ), _atask_ref( t ) { }
	virtual ~DiscATask() { }

	virtual Ticks getDeadlineTicks() { return _deadline_ticks; }

	virtual void Print();
	virtual void DiscretizeTask( double res );

protected:

	ATaskPtr _atask_ref;
	Ticks _deadline_ticks;
};

#endif // DiscTaskTypes_h__