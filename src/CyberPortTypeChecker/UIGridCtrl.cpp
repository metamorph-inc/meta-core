// UIGridCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "UIGridCtrl.h"
#include "UI.h"
#include "MyColors.h"
#include "resource.h"
#include "Boost_Hash_Ext.h"
#include <boost\assign.hpp>
#include "CompatibleDataTypes.h"

// UIGridCtrl
IMPLEMENT_DYNAMIC(UIGridCtrl, CGridCtrl)

UIGridCtrl::UIGridCtrl()
{
	numPorts = 0;
	boost::assign::insert(strictlyCompatibleDataTypes) strictlyCompatibleDataTypeStrings;
	boost::assign::insert(weaklyCompatibleDataTypes) weaklyCompatibleDataTypeStrings;
}

UIGridCtrl::~UIGridCtrl()
{
}

BEGIN_MESSAGE_MAP(UIGridCtrl, CGridCtrl)
	ON_WM_LBUTTONUP()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()



// UIGridCtrl member functions
void UIGridCtrl::insertPortInfo(CString portName, CString portType)
{
	if(!this->findPortIndex(portName))
	{
		// portName not found in the list. Record port info
		this->portNameIndexMapSet.insert(make_pair(portName, ++numPorts));

		PortInfo pi(portName, portType);
		this->indexPortInfoMapSet.insert(make_pair(numPorts, pi));
	}
}

//void UIGridCtrl::modifyPortInfo(CString oldPortName, PortInfo* newPortInfo)
//{
//	UINT index = this->findPortIndex(oldPortName);
//	// if port not found then do nothing
//	if(!index)
//		return;
//	// Set port info
//	if(!this->setPortInfo(index, newPortInfo))
//		return;
//
//	dynamic_cast<UI*>(this->GetParent())->printNormalLog(_T("Modified Port Info! <OLD>") + oldPortName + _T(", <NEW>") + newPortInfo->name + _T(":[TYPE]") + newPortInfo->type);
//	// if the selection is the current port then show the correct info in the PropEditorViewer
//
//	// Check the connection types starting and ending with this port for compatibility
//}

void UIGridCtrl::setConnection(LPCTSTR srcPortName, LPCTSTR dstPortName)
{
	UINT srcPortIndex = this->findPortIndex(srcPortName);
	UINT dstPortIndex = this->findPortIndex(dstPortName);

	if(!srcPortIndex || !dstPortIndex) return;

	this->setConnection(srcPortIndex, dstPortIndex);
}

void UIGridCtrl::setConnection(UINT rowIndex, UINT colIndex)
{
	this->SetItemBkColour(rowIndex, colIndex, MY_BLACK);
	this->setConnectionClr(rowIndex, colIndex);
}

void UIGridCtrl::setConnectionClr(LPCTSTR srcPortName, LPCTSTR dstPortName)
{
	UINT srcPortIndex = this->findPortIndex(srcPortName);
	UINT dstPortIndex = this->findPortIndex(dstPortName);

	if(!srcPortIndex || !dstPortIndex) return;

	this->setConnectionClr(srcPortIndex, dstPortIndex);
}

void UIGridCtrl::setConnectionClr(UINT rowIndex, UINT colIndex)
{
	if(this->GetItemBkColour(rowIndex, colIndex) == MY_BLACK)
	{
		// the connection exists
		PortInfo srcPortInfo;
		PortInfo dstPortInfo;
		// get port info for src and dst ports
		if(!this->getPortInfo(rowIndex, srcPortInfo) || !this->getPortInfo(colIndex, dstPortInfo))
			return;
		// Check if the types are exactly matching
		if(!srcPortInfo.type.Compare(dstPortInfo.type))
		{
			this->SetItemBkColour(rowIndex, colIndex, MY_GREEN);
			return;
		}
		// Check if types are strictly compatible
		if(strictlyCompatibleDataTypes.find(srcPortInfo.type + _T(" -> ") + dstPortInfo.type) != strictlyCompatibleDataTypes.end())
		{
			this->SetItemBkColour(rowIndex, colIndex, MY_GREEN);
			return;
		}
		// Check if types are weakly compatible
		if(weaklyCompatibleDataTypes.find(srcPortInfo.type + _T(" -> ") + dstPortInfo.type) != weaklyCompatibleDataTypes.end())
		{
			this->SetItemBkColour(rowIndex, colIndex, MY_ORANGE);
			dynamic_cast<UI*>(this->GetParent())->printWarningLog(_T("Possible data loss or misinterpretation! between [START_PORT]") + srcPortInfo.name + _T(":[TYPE]") + srcPortInfo.type + _T(", [END_PORT]") + dstPortInfo.name + _T(":[TYPE]") + dstPortInfo.type);
			return;
		}
		// port types do not match
		this->SetItemBkColour(rowIndex, colIndex, MY_RED);
		dynamic_cast<UI*>(this->GetParent())->printErrorLog(_T("Type Mismatch! [START_PORT]") + srcPortInfo.name + _T(":[TYPE]") + srcPortInfo.type + _T(", [END_PORT]") + dstPortInfo.name + _T(":[TYPE]") + dstPortInfo.type);
	}

}

void UIGridCtrl::addConnectionInfo(CString srcPortName, CString dstPortName)
{
	UINT rowIndex, colIndex;

	rowIndex = this->findPortIndex(srcPortName);
	colIndex = this->findPortIndex(dstPortName);

	if(!rowIndex || !colIndex) 
	{
		return;
	}

	this->addConnectionInfo(rowIndex, colIndex);
}

void UIGridCtrl::addConnectionInfo(UINT rowIndex, UINT colIndex)
{
	this->gridCellStatusInfoVector.push_back(make_pair(rowIndex, colIndex));
}

UINT UIGridCtrl::findPortIndex(LPCTSTR portName)
{
	PortNameIndexMapIter it = this->portNameIndexMapSet.find(portName);

	if(it == this->portNameIndexMapSet.end())
		return 0;

	return it->second;
}

BOOL UIGridCtrl::setPortInfo(UINT portIndex, PortInfo* portInfo)
{
	IndexPortInfoMapIter it = this->indexPortInfoMapSet.find(portIndex);
	if(it != this->indexPortInfoMapSet.end())
	{
		// found port info in the record
		it->second = *portInfo;

		return TRUE;
	}

	return FALSE;
}

BOOL UIGridCtrl::getPortInfo(UINT portIndex, PortInfo &portInfo)
{
	IndexPortInfoMapIter it = this->indexPortInfoMapSet.find(portIndex);
	if(it != this->indexPortInfoMapSet.end())
	{
		// found port info in the record
		portInfo = it->second;

		return TRUE;
	}

	return FALSE;
}


void UIGridCtrl::setPortClr(UINT index, const BOOL indexType)
{
	PortInfo pi;
	BOOL fault = FALSE;
	if(!this->getPortInfo(index, pi))
		fault = TRUE;
	else
	{
		// Empty Type
		if(pi.type.GetLength() == 0)
			fault = TRUE;
	}

	// No fault
	if(!fault)
	{
		// A row Port
		if(indexType == this->ROW_INDEX)
		{
			this->SetItemBkColour(index, 0, MY_GREYISHBLUE);
			return;
		}
		// A col port
		this->SetItemBkColour(0, index, MY_GREYISHBLUE);
		return;
	}
	// Faulty index
	if(indexType == this->ROW_INDEX)
	{
		// Row index
		this->SetItemBkColour(index, 0, MY_RED);
		// Print log
		dynamic_cast<UI*>(this->GetParent())->printErrorLog(pi.name + _T(" has empty TYPE string!"));
		return;
	}
	// col index
	this->SetItemBkColour(0, index, MY_RED);
	return;
	
}


void UIGridCtrl::showMatrix()
{
	// Set properties
	this->SetEditable(FALSE);
	this->EnableDragAndDrop(FALSE);
	this->EnableRowHide(FALSE);
	this->EnableColumnHide(FALSE);
	this->EnableDragRowMode(FALSE);
	this->EnableSelection(FALSE);
	this->AllowSelectRowInFixedCol(FALSE);

	// Setting for Matrix display
	this->SetRowCount(numPorts+1);
	this->SetColumnCount(numPorts+1);
	this->SetBkColor(MY_WHITE);
	this->SetItemText(0,0,_T("Start(R)/End(C)"));
	this->SetItemBkColour(0, 0, MY_GRAY);
	this->SetFreezedFrame(1, 1);

	if(!numPorts)
	{
		dynamic_cast<UI*>(this->GetParent())->printNormalLog(_T("Zero Contained Ports found in the selected Modelica Component!"));
		return;
	}

	// Set the COL Labels
	for(UINT i=1; i<=numPorts; ++i)
	{
		CString label;
		label.Format("  PORT %u", i);
		this->SetItemText(i, 0, label);
		this->setPortClr(i, this->ROW_INDEX);
	}
	// Set the ROW Labels
	for(UINT i=1; i<=numPorts; ++i)
	{
		CString label;
		label.Format("  PORT %u", i);
		this->SetItemText(0, i, label);
		this->setPortClr(i, this->COL_INDEX);
	}
	// fill cells
	for(GridCellStatusInfoVectorIter it = this->gridCellStatusInfoVector.begin();
		it != this->gridCellStatusInfoVector.end();
		it++)
	{
		this->setConnection(it->first, it->second);
	}
}

void UIGridCtrl::observe(IMSG* pIMSG)
{
	if(pIMSG->fromCtrlId == MODEL_HANDLER)
	{
		switch(pIMSG->actionType)
		{
			case IMSG::ADD_PORT :
				this->insertPortInfo(pIMSG->srcPortName, pIMSG->srcPortType);
				break;
			
			case IMSG::ADD_PORT_LINK :
				this->addConnectionInfo(pIMSG->srcPortName, pIMSG->dstPortName);
				break;
		}
	}
}

void UIGridCtrl::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	// notify observers about the current cell info
	this->notifyCellInfo(this->m_idCurrentCell);

	__super::OnLButtonUp(nFlags, point);

	// highlight the current cell
	this->highlightCell(this->m_idCurrentCell);
}

void UIGridCtrl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default
	__super::OnKeyDown(nChar, nRepCnt, nFlags);

	// Once the base class function completes and sets currentcell then notify observers about the current cell info
	this->notifyCellInfo(this->m_idCurrentCell);
	// highlight the current cell
	this->highlightCell(this->m_idCurrentCell);
}

void UIGridCtrl::notifyCellInfo(CCellID cell)
{
	IMSG iMsg;
	iMsg.fromCtrlId = this->GetDlgCtrlID();
	
	// if it is a connection cell
	if((cell.col > 0) && (cell.row > 0))
	{
		PortInfo srcPI, dstPI;
		COLORREF cellBkColor = this->GetItemBkColour(cell.row, cell.col);
					
		iMsg.actionType	= iMsg.ADD_PORT_LINK;

		// if the cur cell is depicting an existing connection then send the info abt src and dst ports to the property viewer
		if(cellBkColor == MY_GREEN || 
		   cellBkColor == MY_RED ||
		   cellBkColor == MY_ORANGE)
		{
			if(this->getPortInfo(cell.row, srcPI) && this->getPortInfo(cell.col, dstPI))
			{
				iMsg.srcPortName = srcPI.name;
				iMsg.srcPortType = srcPI.type;
				iMsg.dstPortName = dstPI.name;
				iMsg.dstPortType = dstPI.type;
			}
		}
	}
	else
	{
		iMsg.actionType = iMsg.ADD_PORT;

		{
			// if src port (row Port)
			if(cell.col == 0)
			{
				PortInfo pi;
				if(this->getPortInfo(cell.row, pi))
				{
					iMsg.srcPortName = pi.name;
					iMsg.srcPortType = pi.type;
				}
			}
	
			// if dst port (col Port)
			if(cell.row == 0)
			{
				PortInfo pi;
				if(this->getPortInfo(cell.col, pi))
				{
					iMsg.dstPortName = pi.name;
					iMsg.dstPortType = pi.type;
				}
			}
		}
	}

	this->notify(&iMsg);
}

void UIGridCtrl::highlightCell(CCellID cell)
{
	CRect rect;		// Cell outline Rect
	if (!GetCellRect(this->m_idCurrentCell, rect))
        return;

	CDC* pDC = GetDC();
	if(!pDC)
		return;
	// Since we have erased the background, we will need to redraw the gridlines
    CPen pen;
	pen.CreatePen(PS_SOLID, 3, MY_BLACK);
	// Set DC to new Pen
    CPen* pOldPen = (CPen*) pDC->SelectObject(&pen);
	// Reduce the rect size to accomodate the pens width and prevent from overflowing the outlining 
	rect.left += 3;
	rect.top += 3;
	rect.right -= 3;
	rect.bottom -= 3;
	// draw outlines
	pDC->MoveTo(rect.left,    rect.bottom);
    pDC->LineTo(rect.right, rect.bottom);
    pDC->LineTo(rect.right, rect.top);
    pDC->LineTo(rect.left, rect.top);
    pDC->LineTo(rect.left, rect.bottom);
    // Set DC to old Pen
	pDC->SelectObject(pOldPen);
}
