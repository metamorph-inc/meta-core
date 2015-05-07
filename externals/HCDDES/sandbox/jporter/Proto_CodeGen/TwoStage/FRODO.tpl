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
#define BAUDRATE 57600
#define PARITY 0
#define STOPBITS 0
#define DATABITS 3

#define ADC_CLOCK_DIV 6

//#define Debug(buff) putstr(buff)
#define Debug(buff)

#include "../peri.h"
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
{{MESSAGE_name}}_t {{MESSAGE_name}}, {{MESSAGE_name}}_c;
{{/MESSAGE_STRUCTURE}}

///////////////////////// C-CODE /////////////////////////
{{#CCODE}}
typedef void *{{TASK_componentName}}_context;
static void {{TASK_componentName}}_init({{TASK_componentName}}_context *{{TASK_componentName}}_ctxt ) { }

static void {{TASK_componentName}}_main(
 {{TASK_componentName}}_context *{{TASK_componentName}}_ctxt{{#INPUT_SIGNALS}},
 {{SIGNAL_type}} {{SIGNAL_name}}{{/INPUT_SIGNALS}}{{#OUTPUT_SIGNALS}},
 {{SIGNAL_type}} *{{SIGNAL_name}}{{/OUTPUT_SIGNALS}} ) {
    {{CCODE_BODY}}
}
{{/CCODE}}

void app_tasks_init() { }

///////////////////////// TASKS /////////////////////////

{{#TASK_SUBSYSTEM}}
static {{TASK_componentName}}_context {{TASK_componentName}}_ctxt;

static void {{TASK_name}} (void *parameters)
{
{{#AVR_START}}

   task_entry * te = (task_entry *) parameters;

   {{TASK_componentName}}_init( &{{TASK_componentName}}_ctxt );

   for(;;)
   {
      xSemaphoreTake( te->semaphore, portMAX_DELAY );
{{/AVR_START}}
      {{TASK_componentName}}_main(
       &{{TASK_componentName}}_ctxt{{#INPUT_SIGNALS}},
       {{SIGNAL_message_name}}.{{SIGNAL_name}}{{/INPUT_SIGNALS}}{{#OUTPUT_SIGNALS}},
       &{{SIGNAL_message_name}}.{{SIGNAL_name}}{{/OUTPUT_SIGNALS}}
      );
{{#AVR_END}}
   }
{{/AVR_END}}
}
{{/TASK_SUBSYSTEM}}

///////////////////////// SCHEDULE /////////////////////////



portTickType hp_len = {{NODE_hyperperiod}};

task_entry tasks[] = {
{{#TASK}}
   { {{TASK_name}}, "{{TASK_name}}", {{TASK_startTime}}, 0},
{{/TASK}}
   {NULL, NULL, 0, 0}
};

msg_entry msgs[] = {
{{#MESSAGE_NAME}}
   { {{MESSAGE_index}}, {{MESSAGE_sendreceive}}, sizeof( {{MESSAGE_name}} ), (portCHAR *) & {{MESSAGE_name}}, (portCHAR *) & {{MESSAGE_name}}_c, {{MESSAGE_startTime}}, pdFALSE},
{{/MESSAGE_NAME}}
   { -1, 0, 0, NULL, NULL, 0, 0}
};

{{#AVR_PER}}
per_entry pers[] = {
{{#PER_NAME}}
   { {{PER_index}}, {{PER_type}}, {{PER_way}}, 0, {{PER_pin_number}}, sizeof( {{PER_name}} ), (portCHAR *) & {{PER_name}}, (portCHAR *) & {{PER_name}}_c, {{PER_startTime}}, NULL},
{{/PER_NAME}}
   { -1, NULL, 0, 0, 0, 0, NULL, NULL, 0, NULL }
};
{{/AVR_PER}}

