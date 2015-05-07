#pragma once
#include "GridCtrl.h"
#include "GridCellBase.h"
#include "Notifier.h"
#include <boost\unordered_map.hpp>
#include <boost\unordered_set.hpp>
#include <set>
#include <vector>
// UIGridCtrl command target
class PortInfo
{
public:
	CString name;
	CString type;
	PortInfo(){};
	PortInfo(CString portName, CString portType)
		: name(portName), type(portType)
	{}
	void operator=(const PortInfo pi)
	{
		this->name = pi.name;
		this->type = pi.type;
	}
};

#define PortNameIndexMap				boost::unordered_map<CString /*PortName*/, UINT/*Grid Row/Col Index*/>
#define PortNameIndexMapIter			boost::unordered_map<CString /*PortName*/, UINT/*Grid Row/Col Index*/>::iterator
#define IndexPortInfoMap				boost::unordered_map<UINT/*Grid Row/Col Index*/, PortInfo>
#define IndexPortInfoMapIter			boost::unordered_map<UINT/*Grid Row/Col Index*/, PortInfo>::iterator
#define GridCellStatusInfoVector		vector<pair<UINT /*rowIndex*/, UINT /*colIndex*/>>
#define GridCellStatusInfoVectorIter	vector<pair<UINT /*rowIndex*/, UINT /*colIndex*/>>::iterator

class UIGridCtrl : public CGridCtrl, public Notifier, public Observer
{
	DECLARE_DYNAMIC(UIGridCtrl)

public:
	UIGridCtrl();
	virtual ~UIGridCtrl();

protected:
	DECLARE_MESSAGE_MAP()

public:
	static const BOOL ROW_INDEX			=	FALSE;
	static const BOOL COL_INDEX			=	TRUE;

	void showMatrix();
	void observe(IMSG* pIMSG);

private:
	UINT								numPorts;
	PortNameIndexMap					portNameIndexMapSet;	// collection for port info
	IndexPortInfoMap					indexPortInfoMapSet;
	GridCellStatusInfoVector			gridCellStatusInfoVector;
	boost::unordered_set<CString>		strictlyCompatibleDataTypes;
	boost::unordered_set<CString>		weaklyCompatibleDataTypes;

	void insertPortInfo(CString portName, CString portType);
	//void modifyPortInfo(CString oldPortName, PortInfo* newPortInfo);
	void addConnectionInfo(CString srcPortName, CString dstPortName);	// adds the connection info pair in the cellstatusinfo vector
	void addConnectionInfo(UINT rowIndex, UINT colIndex);
	void setConnection(LPCTSTR srcPortName, LPCTSTR dstPortName);
	void setConnection(UINT rowIndex, UINT colIndex);
	void setConnectionClr(LPCTSTR srcPortName, LPCTSTR dstPortName);
	void setConnectionClr(UINT rowIndex, UINT colIndex);
	void setPortClr(UINT index, const BOOL rowIndex);
	UINT findPortIndex(LPCTSTR portName);		// Returns row/col index/num in grid OR -1
	BOOL setPortInfo(UINT portIndex, PortInfo* portInfo);
	BOOL getPortInfo(UINT portIndex, PortInfo &portInfo);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	void notifyCellInfo(CCellID cell);
	void highlightCell(CCellID cell);
};

