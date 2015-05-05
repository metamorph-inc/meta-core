//################################################################################################
//
// Stereo label part class (decorator part)
//	StereoLabelPart.h
//
//################################################################################################

#ifndef __STEREOLABELPART_H_
#define __STEREOLABELPART_H_


#include "StdAfx.h"
#include "TextPart.h"


namespace DecoratorSDK {

//################################################################################################
//
// CLASS : StereoLabelPart
//
//################################################################################################

class StereoLabelPart: public TextPart
{
protected:
	long		m_labelRelYPosition;	// bottom of the text because of the bottom alignment

public:
	StereoLabelPart(PartBase* pPart, CComPtr<IMgaCommonDecoratorEvents>& eventSink);
	virtual ~StereoLabelPart();

	virtual void	Draw						(CDC* pDC, Gdiplus::Graphics* gdip);

	virtual CPoint	GetTextPosition				(CDC* pDC, Gdiplus::Graphics* gdip) const;
	virtual void	SetTextRelYPosition			(long relYPosition) { m_labelRelYPosition = relYPosition; };
	virtual CRect	GetTextLocation				(CDC* pDC, Gdiplus::Graphics* gdip) const;
};

}; // namespace DecoratorSDK

#endif //__STEREOLABELPART_H_
