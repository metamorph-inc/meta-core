
#ifndef _INSTANCE_MGR_H_
#define _INSTANCE_MGR_H_

#include <map>
#include <set>

#include "CSched.h"

#include "MiscUtil.h"
#include "IndexMgr.h"
#include "ConstraintMgr.h"

using namespace CSched;

class InstanceMgr
{
public:
    virtual ~InstanceMgr();
    InstanceMgr( Udm::DataNetwork* p_backend, TTSystem & sys );

	void CopyProcHierarchy( Udm::DataNetwork* p_backend );
	void CopyBusHierarchy(Udm::DataNetwork * &p_backend);
	void ReviewTasks();
	void CreateTaskInstances();
	void CreateMsgInstances();
	void CreateConstraints();
    bool GetSolutions( const std::string & sysname );

protected:

    TTSystem        _sys;
    InstanceModel   _instModel;
    IndexMgr        _idxMgr;
    ConstraintMgr   _cstrMgr;
};

#endif // _INSTANCE_MGR_H_
