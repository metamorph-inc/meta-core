#ifndef PrintableElement_h__
#define PrintableElement_h__

class PrintableElement
{
public:
	virtual void Print() = 0;
protected:
	virtual void PrintOtherParameters() = 0;
};

#endif // PrintableElement_h__