
#define PRO_USE_VAR_ARGS

/*--------------------------------------------------------------------*\
Pro/TOOLKIT includes
\*--------------------------------------------------------------------*/
#include <ProToolkit.h>
#include <ProMessage.h>
#include <ProUtil.h>

/*--------------------------------------------------------------------*\
System includes
\*--------------------------------------------------------------------*/
#include <stdarg.h>

/*--------------------------------------------------------------------*\
Application includes
\*--------------------------------------------------------------------*/
#include "UtilMessage.h"
#include "UtilString.h"

/*====================================================================*\
    FUNCTION :  ProUtilMsgPrint()
    PURPOSE  :  Generalized form of ProMessageDisplay()
\*====================================================================*/
void ProUtilMsgPrint(
    const char *ftype,
    const char *title,
    ...)
{
    int i;
    char file[PRO_NAME_SIZE];
    wchar_t wstr[PRO_NAME_SIZE];
    void *a[10];
    va_list ap;

    ProUtilstrcpy(file, (const char *)ftype);
    ProUtilstrcat(file,"_msg.txt");

    va_start(ap, title);
    for(i=0;i<10;i++)
	a[i] = va_arg(ap, void*);

    ProMessageDisplay(ProStringToWstring(wstr, file),
		 (char*)title,
		 a[0], a[1], a[2], a[3], a[4], a[5], a[6], a[7], a[8], a[9]);
    va_end(ap);
}

/*====================================================================*\
    FUNCTION :  ProUtilYesnoGet()
    PURPOSE  :  Ask for a yes/no answer from the user.
\*====================================================================*/
int ProUtilYesnoGet(
    char *def)      /* INPUT : default answer */
                    /* Return: 1 = yes, 0 = no */
{
    wchar_t wreply[4];
    char reply[4];

    while(1==1)
    {
        if(ProMessageStringRead(4, wreply))
            ProUtilstrcpy(reply,(const char *) def);
        else
            ProWstringToString(reply,wreply);
        ProUtilStringUpper(reply,reply);
        if(reply[0] == 'Y')
            return(1);
        if(reply[0] == 'N')
            return(0);
        ProUtilMsgPrint("gen","UTIL Please answer Y or N : ");
    }
    return (0);
}

/*====================================================================*\
    FUNCTION :	ProUtilIntGet()
    PURPOSE  :	Read a integer from the keyboard, given a default
\*====================================================================*/
int ProUtilIntGet(
    int range[2],	/* I - Min and Max values */
    int *def,		/* I - Default value - if NULL, default is QUIT */
    int *input)		/* O - The value read */
			/* Return 1 - a value was read,
				  0 - default was NULL and a <CR> was entered */
{
    int i;

    if(ProMessageIntegerRead(range, &i))
    {
	if(def == NULL)
	    return(0);

	*input = *def;
	return(1);
    }

    *input = i;
    return(1);
}

/*====================================================================*\
    FUNCTION :	ProUtilDoubleGet()
    PURPOSE  :	Read a double from the keyboard, given a default
\*====================================================================*/
int ProUtilDoubleGet(
    double range[2],	/* I - Min and Max values */
    double *def,	/* I - Default value - if NULL, default is QUIT */
    double *input)	/* O - The value read */
			/* Return 1 - a value was read,
				  0 - default was NULL and a <CR> was entered */
{
    double d;

    if(ProMessageDoubleRead(range, &d)!=0)
    {
	if(def == NULL)
	    return(0);

	*input = *def;
	return(1);
    }

    *input = d;
    return(1);
}


/*====================================================================*\
FUNCTION :  ProUtilStringGet
PURPOSE  :  Read a string from the keyboard, given a default
\*====================================================================*/
int ProUtilStringGet(wchar_t *p_wstr, wchar_t *p_default,  int max_len)
{
    wchar_t str[PRO_PATH_SIZE];

    if(ProMessageStringRead(max_len, str)!=0)
    {
        if (p_default == NULL)
    	    return (0);
	
    	ProUtilWstrcpy(p_wstr, p_default);
	return (1);
    }
    
    ProUtilWstrcpy(p_wstr, str);
    return (1);
}
