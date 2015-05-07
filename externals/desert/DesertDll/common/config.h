// -*-C++-*-
// config.h

#ifndef CONFIG_H
#define CONFIG_H

#include <afxtempl.h>
#include "common/error.h"

class CConfigurationStore;
class CConfigurationRoot;
class CConfiguration;
class CComponent;
typedef CTypedPtrList<CPtrList, CConfigurationRoot *> CConfigurationRootList;
typedef CTypedPtrList<CPtrList, CConfiguration *> CConfigurationList;
typedef CTypedPtrList<CPtrList, CComponent *> CComponentList;


class CConfigurationStore
{
private:
  CString name;
  CConfigurationRootList roots;

public:
  CConfigurationStore(const TCHAR *name);
  virtual ~CConfigurationStore();
  CConfigurationRoot *CreateConfigurationRoot(long id);
//  bool Store();
 // bool Load();
  const CConfigurationList *GetConfigurations(long rootId) const;

private:
 // void Serialize(CArchive& ar);
};

class CConfigurationRoot
{
  friend class CConfigurationStore;
private:
  long Id;
  CConfigurationList configurations;

public:
  CConfigurationRoot(long id);
  virtual ~CConfigurationRoot();
  CConfiguration *CreateConfiguration();
  //void Serialize(CArchive& ar);
  const CConfigurationList *GetConfigurations() const;
};

class CConfiguration
{
private:
  CComponentList components;
  CConfigurationRoot *configRoot;

public:
  CConfiguration(CConfigurationRoot *root);
  virtual ~CConfiguration();
  CComponent *CreateComponent(long id, long assignedTo);
  void Serialize(CArchive& ar);
  const CComponentList *GetComponents() const;
};

class CComponent
{
private:
  long Id;
  long assignedTo;

public:
  CComponent(long Id, long assignedTo);
  //void Serialize(CArchive& ar);
  long GetId() const;
  long GetAssignedTo() const;
};

inline
CConfigurationStore::
CConfigurationStore(const TCHAR *n) : name(n)
{
}

inline
CConfigurationStore::
~CConfigurationStore()
{
  POSITION pos = roots.GetHeadPosition(); while(pos) delete roots.GetNext(pos);
}

inline
CConfigurationRoot::
CConfigurationRoot(long id) : Id(id)
{
}

inline
CConfigurationRoot::
~CConfigurationRoot()
{
  POSITION pos = configurations.GetHeadPosition(); while(pos) delete configurations.GetNext(pos);
}

inline const CConfigurationList *
CConfigurationRoot::
GetConfigurations() const
{
  return &configurations;
}

inline
CConfiguration::
CConfiguration(CConfigurationRoot *root) : configRoot(root)
{
}

inline
CConfiguration::
~CConfiguration()
{
  POSITION pos = components.GetHeadPosition(); while(pos) delete components.GetNext(pos);
}

inline const CComponentList *
CConfiguration::
GetComponents() const
{
  return &components;
}

inline
CComponent::
CComponent(long id, long assTo) : Id(id), assignedTo(assTo)
{
}

inline long
CComponent::
GetId() const
{
  return Id;
}

inline long
CComponent::
GetAssignedTo() const
{
  return assignedTo;
}


#endif // #ifndef CONFIG_H
