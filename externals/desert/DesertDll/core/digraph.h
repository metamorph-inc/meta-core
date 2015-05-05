// -*-C++-*-
// digraph.h
// Declarations of graph class

#ifndef DIGRAPH_H
#define DIGRAPH_H

#include "core/coredefs.h"
#include "core/templ.h"

class  DESERT_API CDiGraph
{
private:
   CGraphNodeList nodes;
   CGraphPathList paths;


  //exported functions for custom PCM Dlls
public:


	 CDiGraph();
	 ~CDiGraph();

	 int			FindAllPaths();
	 void			GetPath(int idx, CDynElementList& list);
	 int			GetFullPath();
	 void			CreateNode(CDynElement *core);
	 bool			ConnectNodes(CDynElement *src, CDynElement *dst); // will not connect if a loop is formed
	 CGraphNode*	FindNode(CDynElement *core);
	
	 void			linker_force();		//force the linker to load from .lib to .dll

  
  

private:
	 void		FindAllPaths(CGraphNode *src, CGraphNode *dst);
	 void		FindSources(CGraphNodeList &srcs);
	 void		FindDestinations(CGraphNodeList &dsts);
	 CGraphPath*	FindLongestPath(int &index);

  

};



class DESERT_API CGraphNode
{
private:
  CDynElement *core;                    // the element behind this node
  CGraphNodeList children;
  CGraphNodeList ancestors;

public:
		CGraphNode(CDynElement *core);
		~CGraphNode();

public:
		CDynElement *GetCore();
		CGraphNodeList& GetChildren();
		CGraphNodeList& GetAncestors();
		void AddChildren(CGraphNode *);
		void AddAncestor(CGraphNode *);
		bool IsSource();                      // no ancestors
		bool IsDestination();                 // no children

public:
		bool IsChildOf(CGraphNode *);
		bool IsAncestorOf(CGraphNode *);
		bool FindAllPaths(CGraphNode *dst, CGraphPathList& paths);
};

class CGraphPath
{
private:
  CGraphNodeList nodes;

public:
		 CGraphNodeList& GetNodes();
		 void AddNode(CGraphNode *node);
		 int GetLength();
};

#endif // #ifndef DIGRAPH_H
