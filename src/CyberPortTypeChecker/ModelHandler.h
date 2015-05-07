#pragma once
//#include <boost\unordered_map.hpp>
#include "Observer.h"
#include "Notifier.h"
#include "UdmConfig.h"

using namespace META_NAMESPACE;

//class PortObjInfo 
//{
//public:
//	Udm::Object	portObj;
//	enum	portType
//	{
//		INPUT_PORT,
//		OUTPUT_PORT
//	};
//};
//
//#define	PortNameObjectMap		boost::unordered_map<CString, PortObjInfo> 
//#define	PortNameObjectMapIter	boost::unordered_map<CString, PortObjInfo>::iterator

class ModelHandler 
	: public Notifier/*, public Observer*/
{
public:
	ModelHandler(void);
	~ModelHandler(void);
	void attachComponent(ModelicaComponent component);
	BOOL generateCompReport();							// Generates the attached component's contained port pairs and fills
														// the private Collection members of the class.
	void attachUI(CWnd* pUI)	{ this->pUI = pUI; }
private:
	CWnd				*pUI;							// pointer to UI Windows Object
	ModelicaComponent	component;
	//PortNameObjectMap	portNameObjectMapSet;			// used to locate port for modifying port in the GME model
	//void modifyPortInfo(LPCTSTR portName);			// to modify port info in the GME model
	//void observe(IMSG* pIMsg);
	BOOL processComponent();
	void notifyNewPortInfo(LPCTSTR name, LPCTSTR type);
	//void notifyModifyPortinfo(LPCTSTR oldPortName, LPCTSTR newPortName, LPCTSTR newPortType);
	void notifyNewLinkInfo(LPCTSTR srcPortName, LPCTSTR dstPortName);
	CString extractPortType(Udm::Object portObj);
};

