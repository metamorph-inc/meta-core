/* 
 * C++ Code header for Module {{MODNAME}}
 * Generated on {{DATETIME}}
 */

#ifndef _BIP_CPP_{{MODNAME}}_H_
#define _BIP_CPP_{{MODNAME}}_H_

{{#PN_SECTION}}
{{#STRUCT_SECTION}}
class {{PN_NAME}}
{
private:
{{#STATE_SECTION}}    unsigned int {{STATE_NAME}};
{{/STATE_SECTION}}
{{#GLOBAL_VAR_SECTION}}    {{VAR_TYPE}} {{VAR_NAME}};{{/GLOBAL_VAR_SECTION}}
{{/STRUCT_SECTION}}
public:
{{#SETUP_SECTION}}    void {{PN_NAME}}( {{#GLOBAL_VAR_SECTION}}{{VAR_TYPE}} {{VAR_NAME}}_val{{#GLOBAL_VAR_SECTION_separator}}, {{/GLOBAL_VAR_SECTION_separator}}{{/GLOBAL_VAR_SECTION}} );
    void reset();
{{/SETUP_SECTION}}{{#TRANS_SECTION}}
{{#TRANS_GUARD_SECTION}}    bool {{TRANS_NAME}}_check( {{#VAR_SECTION}}{{VAR_TYPE}} {{VAR_NAME}}_val{{#VAR_SECTION_separator}}, {{/VAR_SECTION_separator}}{{/VAR_SECTION}} );{{/TRANS_GUARD_SECTION}}
{{#TRANS_ACTION_SECTION}}    bool {{TRANS_NAME}}_exec( {{#VAR_SECTION}}{{VAR_TYPE}} {{VAR_NAME}}_val{{#VAR_SECTION_separator}}, {{/VAR_SECTION_separator}}{{/VAR_SECTION}} );{{/TRANS_ACTION_SECTION}}
{{#TRANS_RECV_SECTION}}    bool {{TRANS_NAME}}( {{#VAR_SECTION}}{{VAR_TYPE}} {{VAR_NAME}}_val{{#VAR_SECTION_separator}}, {{/VAR_SECTION_separator}}{{/VAR_SECTION}} );{{/TRANS_RECV_SECTION}}
{{#TRANS_SEND_SECTION}}    bool {{TRANS_NAME}}_send( {{#VAR_SECTION}}{{VAR_TYPE}} {{VAR_NAME}}_val{{#VAR_SECTION_separator}}, {{/VAR_SECTION_separator}}{{/VAR_SECTION}} );{{/TRANS_SEND_SECTION}}
{{/TRANS_SECTION}}
};
{{/PN_SECTION}}

#endif /* _BIP_{{MODNAME}}_H_*/
