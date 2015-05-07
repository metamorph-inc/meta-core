// -*-C++-*-
// config.cpp

#include "common/config.h"

// -- CConfigurationStore

CConfigurationRoot *
CConfigurationStore::
CreateConfigurationRoot(long id)
{
  CConfigurationRoot *root = new CConfigurationRoot(id);
  ASSERT_EX(root, _T("CConfigurationStore::CreateConfigurationRoot"), _T("new failed"));
  roots.AddTail(root);
  return root;
}

//bool
//CConfigurationStore::
//Store()
//{
//  TRY {
//    CFile file(name, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary);
//    CArchive ar(&file, CArchive::store);
//    Serialize(ar);
//    ar.Close();
//    file.Close();
//  } CATCH_ALL(e) {
//    Error(_T("CConfigurationStore::Store"), _T("Exception: Unable to open file <%s>"), name);
//    return false;
//  } END_CATCH_ALL;
//  return true;
//}
//
//bool
//CConfigurationStore::
//Load()
//{
//  TRY {
//    CFile file(name, CFile::modeRead | CFile::typeBinary);
//    CArchive ar(&file, CArchive::load);
//    Serialize(ar);
//    ar.Close();
//    file.Close();
//  } CATCH_ALL(e) {
//    Error(_T("CConfigurationStore::Load"), _T("Exception: Unable to open file <%s>"), name);
//    return false;
//  } END_CATCH_ALL;
//  return true;
//}

//void
//CConfigurationStore::
//Serialize(CArchive& ar)
//{
//  if (ar.IsStoring())
//  {
//    ar << roots.GetCount();
//    POSITION pos = roots.GetHeadPosition();
//    while(pos) roots.GetNext(pos)->Serialize(ar);
//  } else {
//    int count;
//    ar >> count;
//    for(int i=0; i<count; i++)
//    {
//      long id;
//      ar >> id;
//      CConfigurationRoot *root = CreateConfigurationRoot(id);
//      root->Serialize(ar);
//    }
//  }
//}

const CConfigurationList *
CConfigurationStore::
GetConfigurations(long rootId) const
{
  POSITION pos = roots.GetHeadPosition();
  while(pos)
  {
    CConfigurationRoot *root = roots.GetNext(pos);
    if (root->Id == rootId) return root->GetConfigurations();
  }
  return NULL;
}

// -- CConfigurationRoot

CConfiguration *
CConfigurationRoot::
CreateConfiguration()
{
  CConfiguration *cfg = new CConfiguration(this);
  ASSERT_EX(cfg, _T("CConfigurationRoot::CreateConfiguration"), _T("new failed"));
  configurations.AddTail(cfg);
  return cfg;
}

//void
//CConfigurationRoot::
//Serialize(CArchive& ar)
//{
//  if (ar.IsStoring())
//  {
//    ar << Id;
//    ar << configurations.GetCount();
//    POSITION pos = configurations.GetHeadPosition();
//    while(pos) configurations.GetNext(pos)->Serialize(ar);
//  } else {
//    int count;
//    ar >> count;
//    for(int i=0; i<count; i++)
//    {
//      CConfiguration *cfg = CreateConfiguration();
//      cfg->Serialize(ar);
//    }
//  }
//}

// -- CConfiguration

CComponent *
CConfiguration::
CreateComponent(long id, long assTo)
{
  CComponent *comp = new CComponent(id, assTo);
  ASSERT_EX(comp, _T("CConfiguration::CreateComponent"), _T("new failed"));
  components.AddTail(comp);
  return comp;
}

//void
//CConfiguration::
//Serialize(CArchive& ar)
//{
//  if (ar.IsStoring())
//  {
//    ar << components.GetCount();
//    POSITION pos = components.GetHeadPosition();
//    while(pos) components.GetNext(pos)->Serialize(ar);
//  } else {
//    int count;
//    ar >> count;
//    for(int i=0; i<count; i++)
//    {
//      long id, assTo;
//      ar >> id;
//      ar >> assTo;
//      CComponent *comp = CreateComponent(id, assTo);
//      comp->Serialize(ar);
//    }
//  }
//}

// -- CComponent

//void
//CComponent::
//Serialize(CArchive& ar)
//{
//  if (ar.IsStoring())
//  {
//    ar << Id;
//    ar << assignedTo;
//  } else {
//  }
//}
