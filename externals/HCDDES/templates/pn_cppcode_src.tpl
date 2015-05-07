//
// C++ Code for Module {{MODNAME}}
// Generated on {{DATETIME}}
//

#include "{{MODNAME}}.h"

#include <cstring>

using namespace std;

{{#PN_SECTION}}{{#SETUP_SECTION}}
void {{PN_NAME}}::{{PN_NAME}}({{#GLOBAL_VAR_SECTION}}{{VAR_TYPE}} {{VAR_NAME}}_val{{#GLOBAL_VAR_SECTION_separator}}, {{/GLOBAL_VAR_SECTION_separator}}{{/GLOBAL_VAR_SECTION}} ) {

{{#GLOBAL_VAR_SECTION}}    {{VAR_NAME}} = {{VAR_NAME}}_val;{{/GLOBAL_VAR_SECTION}}

    reset();
}

void {{PN_NAME}}::reset() {

{{#INITIAL_STATES_SECTION}}    {{STATE_NAME}} = 1;{{/INITIAL_STATES_SECTION}}
{{#NONINITIAL_STATES_SECTION}}    {{STATE_NAME}} = 0;{{/NONINITIAL_STATES_SECTION}}
}
{{/SETUP_SECTION}}
{{#TRANS_SECTION}}{{#TRANS_RECV_SECTION}}
bool {{TRANS_NAME}}( {{#VAR_SECTION}}{{VAR_TYPE}} {{VAR_NAME}}_val{{#VAR_SECTION_separator}}, {{/VAR_SECTION_separator}}{{/VAR_SECTION}} ) {

{{#VAR_SECTION}}    {{VAR_NAME}} = {{VAR_NAME}}_val;{{/VAR_SECTION}}
    if ( {{TRANS_NAME}}_check( {{#VAR_SECTION}}{{VAR_NAME}}_val{{#VAR_SECTION_separator}}, {{/VAR_SECTION_separator}}{{/VAR_SECTION}} ) 
       {{#TRANS_SOURCE_SECTION}} && {{SRC_STATE_NAME}} {{/TRANS_SOURCE_SECTION}})
    {
{{#TRANS_SOURCE_SECTION}}        {{SRC_STATE_NAME}} = 0;{{/TRANS_SOURCE_SECTION}}
{{#TRANS_DEST_SECTION}}        {{DST_STATE_NAME}} = 1;{{/TRANS_DEST_SECTION}}

		{{TRANS_NAME}}_action( {{#VAR_SECTION}}{{VAR_NAME}}_val{{#VAR_SECTION_separator}}, {{/VAR_SECTION_separator}}{{/VAR_SECTION}} );

        return 1;
    }

    return 0;
}{{/TRANS_RECV_SECTION}}
{{#TRANS_SEND_SECTION}}
bool {{PN_NAME}}::{{TRANS_NAME}}_send( {{#VAR_SECTION}}{{VAR_NAME}}_val{{#VAR_SECTION_separator}}, {{/VAR_SECTION_separator}}{{/VAR_SECTION}} ) {

 // TODO: We may need a callback pointer to do this correctly --
 // Declare a generic callback pointer type that all modules can use.
 // Each receiver should declare and implement a callback function that
 // gets registered with the sender.  Then the send transition uses it
 // at the end to notify the other end of the change.  We DO NOT want
 // this to be dynamically configurable -- it all has to be available at
 // initial setup.  However, specifics are not known until model 
 // generation time, so we can't include it here directly (only part of it).
 //
    if ( {{TRANS_NAME}}_check( {{#VAR_SECTION}}{{VAR_NAME}}_val{{#VAR_SECTION_separator}}, {{/VAR_SECTION_separator}}{{/VAR_SECTION}} ) 
       {{#TRANS_SOURCE_SECTION}} && {{SRC_STATE_NAME}} {{/TRANS_SOURCE_SECTION}})
    {
{{#TRANS_SOURCE_SECTION}}        {{SRC_STATE_NAME}} = 0;{{/TRANS_SOURCE_SECTION}}
{{#TRANS_DEST_SECTION}}        {{DST_STATE_NAME}} = 1;{{/TRANS_DEST_SECTION}}
		
{{#VAR_SECTION}}        {{VAR_NAME}} = {{VAR_NAME}}_val;{{/VAR_SECTION}}

        {{TRANS_NAME}}_action( {{#VAR_SECTION}}{{VAR_NAME}}_val{{#VAR_SECTION_separator}}, {{/VAR_SECTION_separator}}{{/VAR_SECTION}} );

 // TODO: The notification would go here.

    }    
} {{/TRANS_SEND_SECTION}}
{{#TRANS_GUARD_SECTION}}
unsigned int {{PN_NAME}}::{{TRANS_NAME}}_check( {{#VAR_SECTION}}{{VAR_NAME}}_val{{#VAR_SECTION_separator}}, {{/VAR_SECTION_separator}}{{/VAR_SECTION}} )
{
	return {{TRANSLATED_EXPRESSION}};
} {{/TRANS_GUARD_SECTION}}
{{#TRANS_ACTION_SECTION}}
void {{PN_NAME}}::{{TRANS_NAME}}_exec( {{#VAR_SECTION}}{{VAR_NAME}}_val{{#VAR_SECTION_separator}}, {{/VAR_SECTION_separator}}{{/VAR_SECTION}} )
{
    {{TRANSLATED_ACTIONS}}
} {{/TRANS_ACTION_SECTION}}
{{/TRANS_SECTION}}
{{/PN_SECTION}}

