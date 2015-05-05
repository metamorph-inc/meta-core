#ifndef DESERTTHREAD_H
#define DESERTTHREAD_H

#include <string>
#include "DesertStatusDlg.h"

typedef unsigned int UINT;

class CDesertStatusDlg;

class BaseNotify {
public:
  volatile bool m_quit;
  volatile bool m_cancel;
  volatile bool m_fail;
  CString m_invalidConstraint;
  UINT m_maxPrg;
  
public:

  BaseNotify(UINT maxPrg) : 
      m_maxPrg(maxPrg),
      m_quit(false),
	  m_cancel(false),
	  m_fail(false){}

	virtual void finished() { }

	virtual void reportStatus(StatusID s_id) { }
	virtual void reportProgress(const std::string &progress) { }
	virtual bool quit()
	{
		return m_quit;
	}
};

class Notify : public BaseNotify {
public:
  CDesertStatusDlg& m_dlg;
  CString m_invalidConstraint;
  UINT m_maxPrg;
  
public:

  Notify(CDesertStatusDlg& dlg, UINT maxPrg ) : 
	  BaseNotify(maxPrg),
      m_dlg(dlg)
	{}

  void finished();

  void reportStatus(StatusID s_id); 
  void reportProgress(const std::string &progress); 
};

//===================
class DesertThread
{
public: 
	/*DesertThread();*/
	DesertThread(DesertIface::DesertSystem &dsystem, 
		         const std::string &constraints, 
				 UdmDesertMap &des_map,
				 DesertUdmMap &inv_des_map,
				 BaseNotify* notify, 
				 int stage, long& configCount);
	DesertThread(DesertIfaceBack::DesertBackSystem &dbacksystem, 
		         UdmDesertMap &des_map,
				 DesertUdmMap &inv_des_map,
				 BaseNotify* notify, 
				 int stage, long& configCount);

  void operator()();
  void DesertThread::RunDesert();
private:
	DesertIface::DesertSystem m_ds;
	DesertIfaceBack::DesertBackSystem m_dbs;
	std::string m_constraints;
	BaseNotify*  m_notify;
	int m_stage;
	UdmDesertMap &m_des_map;
	DesertUdmMap &m_inv_des_map;
	long& m_configCount;
};

#endif