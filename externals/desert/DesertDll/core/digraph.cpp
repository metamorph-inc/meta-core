// -*-C++-*-
// digraph.cpp
// Implementation of graph classes


#include "stdafx.h"
#include "desert.h"
#include "CSetErrDialog.h"
#include "core/digraph.h"
#include "core/dynamic.h"



void
CDiGraph::CreateNode(CDynElement *core)
{
  CGraphNode *node = new CGraphNode(core);
  nodes.AddTail(node);
}

void CDiGraph::linker_force(){return;};

 bool
CDiGraph::ConnectNodes(CDynElement *src, CDynElement *dst)
{

  CGraphNode *srcNode = FindNode(src);
  CGraphNode *dstNode = FindNode(dst);

  // loops not allowed here
  if (dstNode->IsAncestorOf(srcNode))
    return false;

  srcNode->AddChildren(dstNode);
  dstNode->AddAncestor(srcNode);

  return true;
}

 int 
CDiGraph::FindAllPaths()
{
  CGraphNodeList srcs;
  CGraphNodeList dsts;

  FindSources(srcs);
  FindDestinations(dsts);

  POSITION sp = srcs.GetHeadPosition();
  while(sp)
  {
    CGraphNode *src = srcs.GetNext(sp);
    POSITION dp = dsts.GetHeadPosition();
    while(dp)
    {
      CGraphNode *dst = dsts.GetNext(dp);
      FindAllPaths(src, dst);
    }
  }

  return paths.GetCount();
}

 void
CDiGraph::
GetPath(int idx, CDynElementList& list)
{
  if (idx >= paths.GetCount()) return;

  POSITION pos = paths.FindIndex(idx);
  ASSERT_EX( pos!=0, _T("CDiGraph::GetPath"), _T("pos is NULL") );

  CGraphPath *path = paths.GetAt(pos);
  CGraphNodeList& nodes = path->GetNodes();

  pos = nodes.GetHeadPosition();
  while(pos)
  {
    CGraphNode *node = nodes.GetNext(pos);
    CDynElement *core = node->GetCore();
    list.AddTail(core);
  }
}

 int
CDiGraph::
GetFullPath()
/*
 * returns the index of the full path (a path that contains all nodes)
 * returns -1 if no such path
 */
{
  int ret;
  CGraphPath *path = FindLongestPath(ret);
  if (path && path->GetLength() == nodes.GetCount()) return ret;
  return -1;
}

 CGraphNode * 
CDiGraph::FindNode(CDynElement *core)
{
  POSITION pos = nodes.GetHeadPosition();
  while(pos)
  {
    CGraphNode *node = nodes.GetNext(pos);
    if (node->GetCore() == core) return node;
  }
  return 0;
}


 void
CDiGraph::
FindAllPaths(CGraphNode *src, CGraphNode *dst)
{
  CGraphPathList list;
  bool ret = src->FindAllPaths(dst, list);
  if (ret) paths.AddTail(&list);
}

void 
CDiGraph::
FindSources(CGraphNodeList& sources)
{
  POSITION pos = nodes.GetHeadPosition();
  while(pos)
  {
    CGraphNode *node = nodes.GetNext(pos);
    if (node->IsSource()) sources.AddTail(node);
  }
}

void 
CDiGraph::
FindDestinations(CGraphNodeList& destinations)
{
  POSITION pos = nodes.GetHeadPosition();
  while(pos)
  {
    CGraphNode *node = nodes.GetNext(pos);
    if (node->IsDestination()) destinations.AddTail(node);
  }
}

CGraphPath * 
CDiGraph::
FindLongestPath(int &index)
/*
 * find the longest path (length = no. of nodes in path)
 */
{
  CGraphPath *longest = 0;
  int len = 0;
  POSITION pos = paths.GetHeadPosition(); index=-1;
  while(pos)
  {
    CGraphPath *path = paths.GetNext(pos); index++;
    int plen = path->GetLength();
    if ( plen < len) continue;
    len = plen;
    longest = path;
  }
  
  return longest;
}

bool
CGraphNode::
IsAncestorOf(CGraphNode *other)
{
  // find whether this node is an ancestor of other node

  POSITION pos;
  pos = children.GetHeadPosition();
  while(pos)
  {
    CGraphNode *child = children.GetNext(pos);
    if (child == other) return true;
  }

  bool ret=false;
  pos = children.GetHeadPosition();
  while(pos && !ret)
    ret = children.GetNext(pos)->IsAncestorOf(other);

  return ret;
}


bool
CGraphNode::
FindAllPaths(CGraphNode *dst, CGraphPathList& paths)
{
  if (this == dst)
  {
    CGraphPath *path = new CGraphPath();
    path->AddNode(this);
    paths.AddTail(path);
    return true;
  }
  else
  {
    bool hasPath = false;
    POSITION pos = children.GetHeadPosition();
    while(pos)
    {
      CGraphPathList list;
      CGraphNode *child = children.GetNext(pos);
      bool ret = child->FindAllPaths(dst, list);
      if (!ret) continue;

      // to each returned path from the children add yourself
      POSITION lp = list.GetHeadPosition();
      while(lp) list.GetNext(lp)->AddNode(this);

      // add all this paths to the top path list to be returned to the caller
      paths.AddTail(&list);

      // found atleast one path to destination
      hasPath = true;
    }

    return hasPath;
  }
}

// Implementation of graph class inlines

inline 	
CDiGraph::CDiGraph(){}

inline 
CDiGraph::~CDiGraph()
{
  Destroy(nodes);
  Destroy(paths);
}

inline 
CGraphNode::CGraphNode(CDynElement *c) : core(c)
{
}

inline 
CGraphNode::~CGraphNode()
{
}

inline  CDynElement *
CGraphNode::GetCore()
{
  return core;
}

inline  CGraphNodeList&
CGraphNode::GetChildren()
{
  return children;
}

inline  CGraphNodeList&
CGraphNode::GetAncestors()
{
  return ancestors;
}

inline  void
CGraphNode::AddChildren(CGraphNode *node)
{
  children.AddTail(node);
}

inline  void
CGraphNode::AddAncestor(CGraphNode *node)
{
  ancestors.AddTail(node);
}

inline  bool
CGraphNode::IsSource()
{
  return ancestors.IsEmpty() == TRUE;
}

inline  bool
CGraphNode::IsDestination()
{
  return children.IsEmpty() == TRUE;
}

inline  CGraphNodeList&
CGraphPath::GetNodes()
{
  return nodes;
}

inline  void
CGraphPath::AddNode(CGraphNode *node)
{
  ASSERT_EX ( !nodes.Find(node), _T("CGraphPath::AddNode"), _T("!! LOOP !!") );
  nodes.AddHead(node);
}

inline  int
CGraphPath::GetLength()
{
  return nodes.GetCount();
}

