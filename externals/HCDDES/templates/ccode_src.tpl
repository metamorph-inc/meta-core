/*** Included Header Files ***/
#include "{{COMPONENT_NAME}}.h"


/*** Init Function Definition ***/
void {{COMPONENT_NAME}}_init( {{COMPONENT_NAME}}_context* {{COMPONENT_NAME}}_ctxt ) {
	// Nothing to do for now...
}


/*** Main Function Definition ***/
void {{COMPONENT_NAME}}_main(
	{{COMPONENT_NAME}}_context* {{COMPONENT_NAME}}_ctxt{{#INPUT_SIGNAL}},
	{{SIGNAL_TYPE}} {{SIGNAL_NAME}}{{/INPUT_SIGNAL}}{{#OUTPUT_SIGNAL}},
	{{SIGNAL_TYPE}} *{{SIGNAL_NAME}}{{/OUTPUT_SIGNAL}} ) {
	{{CCODE_BODY}}
}

