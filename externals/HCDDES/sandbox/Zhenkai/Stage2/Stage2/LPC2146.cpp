#include <iostream>
#include <string>
using namespace std;

#include <ctemplate/template.h>
#include "Processors.h"


void LPC2146_Variables(ctemplate::TemplateDictionary &mainDictionary)
{
	ctemplate::TemplateDictionary *variableDictionary;
	variableDictionary = mainDictionary.AddSectionDictionary("VARIABLES");
	variableDictionary->SetValue("VARIABLE", "unsigned int processorClockFrequency;");
	variableDictionary = mainDictionary.AddSectionDictionary("VARIABLES");
	variableDictionary->SetValue("VARIABLE", "unsigned int peripheralClockFrequency;");
}

void LPC2146_UART0(ctemplate::TemplateDictionary &mainDictionary)
{
	mainDictionary.SetValue("VARIABLE", "unsigned int divisor0;");
}


