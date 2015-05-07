// Emit one of these templates for each node in the model

//
// Generated task code (FRODO Code Generator). Do not edit!
// Date: {{now}}
//

///////////////// INCLUDES ////////////////////////
{{#PXA_INCLUDE}}
#include <stdio.h>
#include <stdlib.h>
#include "gumstix-frodo.h"
#include "Log.h"
{{/PXA_INCLUDE}}
{{#AVR_INCLUDE}}
#include "peri.h"
{{/AVR_INCLUDE}}

{{#INCLUDE}}#include "{{NAME}}"
{{/INCLUDE}}

///////////////// LOCAL MESSAGES ////////////////////////

{{#MESSAGE_STRUCTURE}}
typedef struct {
{{#SIGNAL_MEMBER}}
    {{SIGNAL_typeName}} {{SIGNAL_name}};
{{/SIGNAL_MEMBER}}
} {{MESSAGE_name}}_t;
{{MESSAGE_name}}_t {{MESSAGE_name}}, {{MESSAGE_name}}_c[2];
{{/MESSAGE_STRUCTURE}}

///////////////////////// INITIALIZATION /////////////////////////

{{#INITIALIZATION}}
{{#CONTEXT_INIT}}
{{#TASK_CONTEXT}}static  {{TASK_functionName}}_context {{TASK_functionName}}_ctxt;
{{/TASK_CONTEXT}}
{{/CONTEXT_INIT}}
void app_tasks_init() { 

{{#MESSAGE_INIT}}   memset( (void *) &({{MESSAGE_name}}), 0, sizeof( {{MESSAGE_name}}_t ) );
   memset( (void *) &({{MESSAGE_name}}_c), 0, sizeof( {{MESSAGE_name}}_t[2] ) );
{{/MESSAGE_INIT}}

{{#CONTEXT_INIT}}
{{#TASK_CONTEXT}}   {{TASK_functionName}}_init( &{{TASK_functionName}}_ctxt );
{{/TASK_CONTEXT}}
{{/CONTEXT_INIT}}
}{{/INITIALIZATION}}

///////////////////////// TASKS /////////////////////////

{{#TASK_SUBSYSTEM}}
static void {{TASK_name}} (void *parameters)
{
{{#AVR_START}}

   task_entry * te = (task_entry *) parameters;


   for(;;)
   {
      xSemaphoreTake( te->semaphore, portMAX_DELAY );
{{/AVR_START}}
      {{TASK_functionName}}_main(
       &{{TASK_functionName}}_ctxt{{#INPUT_SIGNALS}},
       {{SIGNAL_message_name}}.{{SIGNAL_name}}{{/INPUT_SIGNALS}}{{#OUTPUT_SIGNALS}},
       &{{SIGNAL_message_name}}.{{SIGNAL_name}}{{/OUTPUT_SIGNALS}}
      );
{{#AVR_END}}
   }
{{/AVR_END}}
}
{{/TASK_SUBSYSTEM}}

///////////////////////// SCHEDULE /////////////////////////

{{#SCHEDULE_SECTION}}

portTickType hp_len = {{NODE_hyperperiod}};

{{#PXA_VERSION}}task_entry tasks[] = {
{{#TASK}}
   { {{TASK_name}}, "{{TASK_name}}", {{TASK_startTime}} },{{/TASK}}
   {NULL, NULL, 0, 0}
};

msg_entry msgs[] = {
{{#MESSAGE_NAME}}
   { {{MESSAGE_index}}, {{MESSAGE_sendreceive}}, sizeof( {{MESSAGE_name}} ), (portCHAR *) & {{MESSAGE_name}}, {{MESSAGE_startTime}} },{{/MESSAGE_NAME}}
   { -1, 0, 0, NULL, 0}
};


{{/PXA_VERSION}}{{#AVR_VERSION}}task_entry tasks[] = {
{{#TASK}}
   { {{TASK_name}}, "{{TASK_name}}", {{TASK_startTime}}, 0},{{/TASK}}
   {NULL, NULL, 0, 0}
};

msg_entry msgs[] = {
{{#MESSAGE_NAME}}
   { {{MESSAGE_index}}, {{MESSAGE_sendreceive}}, sizeof( {{MESSAGE_name}} ), (portCHAR *) & {{MESSAGE_name}}, (portCHAR *) {{MESSAGE_name}}_c, {{MESSAGE_startTime}}, pdFALSE},
{{/MESSAGE_NAME}}
   { -1, 0, 0, NULL, NULL, 0, 0}
};

per_entry pers[] = {
{{#PER_NAME}}
   { {{PER_index}}, "{{PER_type}}", {{PER_way}}, 0, {{PER_pin_number}}, sizeof( {{PER_name}} ), (portCHAR *) & {{PER_name}}, (portCHAR *) {{PER_name}}_c, {{PER_startTime}}, NULL, 0, 0 },
{{/PER_NAME}}
   { -1, NULL, 0, 0, 0, 0, NULL, NULL, 0, NULL, 0, 0 }
};
{{/AVR_VERSION}}
{{/SCHEDULE_SECTION}}
