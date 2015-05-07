/* 
 * C Code for Module {{MODNAME}}
 * Generated on {{DATETIME}}
 */

#include "{{MODNAME}}.h"

#include "string.h"

{{#EXTERN_SECTION}}{{#SEND_SECTION}}
extern void {{PN_NAME}}_{{TRANS_NAME}}_notify( {{#VAR_SECTION}}{{VAR_TYPE}} {{VAR_NAME}}{{#VAR_SECTION_SEPARATOR}}, {{/VAR_SECTION_SEPARATOR}}{{/VAR_SECTION}});
{{/SEND_SECTION}}{{/EXTERN_SECTION}}

{{#PN_SECTION}}{{#SETUP_SECTION}}
void {{PN_NAME}}_init( {{PN_NAME}}_context_type * ctxt )
{
{{#GLOBAL_VAR_SECTION}}    memset( &(ctxt->{{VAR_NAME}}), 0, sizeof( {{VAR_TYPE}} ) );{{/GLOBAL_VAR_SECTION}}

    {{PN_NAME}}_reset( ctxt );
}

void {{PN_NAME}}_reset( {{PN_NAME}}_context_type * ctxt )
{
{{#INITIAL_STATES_SECTION}}    ctxt->{{STATE_NAME}} = 1;{{/INITIAL_STATES_SECTION}}
{{#NONINITIAL_STATES_SECTION}}    ctxt->{{STATE_NAME}} = 0;{{/NONINITIAL_STATES_SECTION}}
}
{{/SETUP_SECTION}}
{{#TRANS_SECTION}}{{#TRANS_RECV_SECTION}}
unsigned int {{PN_NAME}}_{{TRANS_NAME}}( {{PN_NAME}}_context_type * ctxt {{#VAR_SECTION}}, {{VAR_TYPE}} {{VAR_NAME}}{{/VAR_SECTION}} )
{
{{#VAR_SECTION}}    ctxt->{{VAR_NAME}} = {{VAR_NAME}};{{/VAR_SECTION}}
    if ( {{PN_NAME}}_{{TRANS_NAME}}_check( ctxt ) 
       {{#TRANS_SOURCE_SECTION}} && ctxt->{{SRC_STATE_NAME}} {{/TRANS_SOURCE_SECTION}})
    {
{{#TRANS_SOURCE_SECTION}}        ctxt->{{SRC_STATE_NAME}} = 0;{{/TRANS_SOURCE_SECTION}}
{{#TRANS_DEST_SECTION}}        ctxt->{{DST_STATE_NAME}} = 1;{{/TRANS_DEST_SECTION}}

		{{PN_NAME}}_{{TRANS_NAME}}_action( ctxt );

        return 1;
    }

    return 0;
}{{/TRANS_RECV_SECTION}}
{{#TRANS_SEND_SECTION}}
unsigned int {{PN_NAME}}_{{TRANS_NAME}}_send( {{PN_NAME}}_context_type * ctxt )
{

    if ( {{PN_NAME}}_{{TRANS_NAME}}_check( ctxt ) 
       {{#TRANS_SOURCE_SECTION}} && ctxt->{{SRC_STATE_NAME}} {{/TRANS_SOURCE_SECTION}})
    {
{{#TRANS_SOURCE_SECTION}}        ctxt->{{SRC_STATE_NAME}} = 0;{{/TRANS_SOURCE_SECTION}}
{{#TRANS_DEST_SECTION}}        ctxt->{{DST_STATE_NAME}} = 1;{{/TRANS_DEST_SECTION}}
		
        {{PN_NAME}}_{{TRANS_NAME}}_action( ctxt );

 // Notify the consumer.
		{{PN_NAME}}_{{TRANS_NAME}}_notify( 
			{{#VAR_SECTION}}ctxt->{{VAR_NAME}}{{#VAR_SECTION_SEPARATOR}}, {{/VAR_SECTION_SEPARATOR}}{{/VAR_SECTION}}        
			    );

    }    
} {{/TRANS_SEND_SECTION}}
{{#TRANS_GUARD_SECTION}}
unsigned int {{PN_NAME}}_{{TRANS_NAME}}_check( {{PN_NAME}}_context_type * ctxt )
{
	return {{TRANSLATED_EXPRESSION}};
} {{/TRANS_GUARD_SECTION}}
{{#TRANS_ACTION_SECTION}}
void {{PN_NAME}}_{{TRANS_NAME}}_exec( {{PN_NAME}}_context_type * ctxt )
{
    {{TRANSLATED_ACTIONS}}
} {{/TRANS_ACTION_SECTION}}
{{/TRANS_SECTION}}
{{/PN_SECTION}}

#endif /* _BIP_{{MODNAME}}_H_*/
