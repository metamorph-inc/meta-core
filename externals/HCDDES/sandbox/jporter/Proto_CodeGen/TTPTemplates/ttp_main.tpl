/* {{NODENAME}} generated on {{GENTIME}} */

#include "hal.h"
#include "string.h"
#include "TTPos.h"
#include "TTPCftl.h"
#include "ttpc_msg.h"
#include "{{APPNAME}}_sl.h"

extern void init (void);
ubyte4 lStackStart [128];
const ubyte4 lStack = (ubyte4) &lStackStart [127];
ubyte4 user_stack_array [400];
const ubyte4 user_stack = (ubyte4) &user_stack_array [399];
extern char __DATA_ROM [];
extern char __DATA_RAM [];
extern char __DATA_END [];
tt_declare_app_mode (am);

void main (void) 
{
    tt_start_OS (&am);
}

void initcopy (void)
{
    if (__DATA_ROM != __DATA_RAM)
      {
        memcpy(__DATA_RAM,__DATA_ROM,__DATA_END-__DATA_RAM);
      };
}

void tt_startup_hook (void)
{
    init ();
    tt_start_TTP ();
}

#define RESTART_REASONS CNIPROTSM_AE | CNIPROTSM_SE | CNIPROTSM_CB |  CNIPROTSM_MV | CNIPROTSM_SO
void tt_background_hook (void)
{
    if (!TTPC_CONTROLLER_ON() || (TTPC_PROTOCOL_ERROR() & (RESTART_REASONS)))
      {
        tt_hal_set_mode (KERNEL);
        TTPC_SET_CONTROLLER_OFF ();
        tt_hal_reset ();
      };
}

void tt_error_hook
    ( tt_Status error
    , tt_Type_ID task_id
    )
{
    if ((error == TT_E_DEADLINE_VIOLATION) && (error == error) && (task_id == task_id))
      {
        /* set breakpoint here or output task_id over serial link to detect  */
        /* violating task                                                    */
      };
    tt_hal_set_mode (KERNEL);
    TTPC_SET_CONTROLLER_OFF ();
    tt_hal_reset ();
}

// Declare globals
{{#COMP_SECTION}}
static {{COMPNAME}}_{{COMPID}}_context {{COMPNAME}}_{{COMPID}}_ctx;
{{/COMP_SECTION}}

// Initialize everything
void init() {
{{#COMP_SECTION}}
  {{COMPNAME}}_init();
  {{COMPNAME}}_{{COMPID}}_init( & {{COMPNAME}}_{{COMPID}}_ctx );
{{/COMP_SECTION}}
}

// Task code
{{#TASK_SECTION}}
tt_task( {{TASKNAME}}_proc) {

// Declarations
{{#COMP_SECTION}}
{{#PORT_SECTION}}
  {{DATATYPE}} loc_{{PORTNAME}};
{{/PORT_SECTION}}
{{/COMP_SECTION}}

// Get Input Data
{{#COMP_SECTION}}
{{#PORT_SECTION}}
  loc_{{PORTNAME}} = {{MSGNAME}};
{{/PORT_SECTION}}  
{{/COMP_SECTION}}

// Invoke externally and internally defined
// functions which implement the task.
{{#COMP_SECTION}}
{{#EXTRN_SECTION}}
    {{COMPNAME}}(
	    {{#ARG_SECTION}}{{#ARGMOD_SECTION}} & {{/ARGMOD_SECTION}}{{PORTNAME}},{{/ARG_SECTION}}
		{{#ARGMOD_SECTION}} & {{/ARGMOD_SECTION}} {{PORTNAME}}
      );
{{/EXTRN_SECTION}}  
{{#INTRN_SECTION}}
    {{COMPNAME}}_{{COMPID}}_main( & {{COMPNAME}}_{{COMPID}}_ctx
		    {{#ARG_SECTION}}, {{#ARGMOD_SECTION}} & {{/ARGMOD_SECTION}} {{ARGNAME}}{{/ARG_SECTION}}
     );
{{/INTRN_SECTION}}  
{{/COMP_SECTION}}

// Put results
{{#COMP_SECTION}}
{{#PORT_SECTION}
  tt_Raw_Value( {{MSGNAME}} ) = loc_{{PORTNAME}};
{{/PORT_SECTION}}  
{{/COMP_SECTION}}
}
{{/TASK_SECTION}}

