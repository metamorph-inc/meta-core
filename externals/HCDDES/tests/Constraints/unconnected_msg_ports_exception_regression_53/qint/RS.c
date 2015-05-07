// Emit one of these templates for each node in the model

//
// Generated task code (FRODO Code Generator). Do not edit!
// Date: 
//

///////////////// INCLUDES ////////////////////////


#include "peri.h"


#include "InnerLoop_sl.h"
#include "DataHandler_sl.h"


///////////////// LOCAL MESSAGES ////////////////////////


typedef struct {

    MsingleC1C1M InnerLoop_ang_err;

    MsingleC1C1M InnerLoop_Torque;

    MsingleC1C1M InnerLoop_ang_vel;

} InnerLoop_thrust_commands_t;
InnerLoop_thrust_commands_t InnerLoop_thrust_commands, InnerLoop_thrust_commands_c;

typedef struct {

    MsingleC1C1M InnerLoop_ang_ref;

} OuterLoop_ang_ref_t;
OuterLoop_ang_ref_t OuterLoop_ang_ref, OuterLoop_ang_ref_c;

typedef struct {

    MsingleC1C1M DataHandling_ang_out;

} DataHandling_ang_msg_t;
DataHandling_ang_msg_t DataHandling_ang_msg, DataHandling_ang_msg_c;

typedef struct {

    MsingleC1C1M DataHandling_pos_out;

} DataHandling_pos_msg_t;
DataHandling_pos_msg_t DataHandling_pos_msg, DataHandling_pos_msg_c;

typedef struct {

    MsingleC1C1M DataHandling_ang_in;

    MsingleC1C1M DataHandling_pos_in;

} DataHandling_sensor_data_in_t;
DataHandling_sensor_data_in_t DataHandling_sensor_data_in, DataHandling_sensor_data_in_c;


///////////////////////// INITIALIZATION /////////////////////////



static  InnerLoop_context InnerLoop_ctxt;
static  DataHandling_context DataHandling_ctxt;


void app_tasks_init() { 

   memset( (void *) &(DataHandling_sensor_data_in), 0, sizeof( DataHandling_sensor_data_in_t ) );
   memset( memset( (void *) &(DataHandling_sensor_data_in_c), 0, sizeof( DataHandling_sensor_data_in_t ) );
   memset( (void *) &(DataHandling_pos_msg), 0, sizeof( DataHandling_pos_msg_t ) );
   memset( memset( (void *) &(DataHandling_pos_msg_c), 0, sizeof( DataHandling_pos_msg_t ) );
   memset( (void *) &(OuterLoop_ang_ref), 0, sizeof( OuterLoop_ang_ref_t ) );
   memset( memset( (void *) &(OuterLoop_ang_ref_c), 0, sizeof( OuterLoop_ang_ref_t ) );
   memset( (void *) &(InnerLoop_thrust_commands), 0, sizeof( InnerLoop_thrust_commands_t ) );
   memset( memset( (void *) &(InnerLoop_thrust_commands_c), 0, sizeof( InnerLoop_thrust_commands_t ) );



   InnerLoop_init( &InnerLoop_ctxt );
   DataHandling_init( &DataHandling_ctxt );


}

///////////////////////// TASKS /////////////////////////


static void InnerLoop (void *parameters)
{


   task_entry * te = (task_entry *) parameters;


   for(;;)
   {
      xSemaphoreTake( te->semaphore, portMAX_DELAY );

      InnerLoop_main(
       &InnerLoop_ctxt,
       OuterLoop_ang_ref.InnerLoop_ang_ref,
       DataHandling_ang_msg.DataHandling_ang_out,
       &InnerLoop_thrust_commands.InnerLoop_ang_err,
       &InnerLoop_thrust_commands.InnerLoop_Torque,
       &InnerLoop_thrust_commands.InnerLoop_ang_vel
      );

   }

}

static void DataHandling (void *parameters)
{


   task_entry * te = (task_entry *) parameters;


   for(;;)
   {
      xSemaphoreTake( te->semaphore, portMAX_DELAY );

      DataHandling_main(
       &DataHandling_ctxt,
       DataHandling_sensor_data_in.DataHandling_ang_in,
       DataHandling_sensor_data_in.DataHandling_pos_in,
       &DataHandling_ang_msg.DataHandling_ang_out,
       &DataHandling_pos_msg.DataHandling_pos_out
      );

   }

}


///////////////////////// SCHEDULE /////////////////////////



portTickType hp_len = 20;

task_entry tasks[] = {

   { DataHandling, "DataHandling", 4, 0},
   { InnerLoop, "InnerLoop", 9, 0},
   {NULL, NULL, 0, 0}
};

msg_entry msgs[] = {

   { 1, MSG_DIR_RECV, sizeof( DataHandling_sensor_data_in ), (portCHAR *) & DataHandling_sensor_data_in, (portCHAR *) & DataHandling_sensor_data_in_c, 1, pdFALSE},

   { 2, MSG_DIR_SEND, sizeof( DataHandling_pos_msg ), (portCHAR *) & DataHandling_pos_msg, (portCHAR *) & DataHandling_pos_msg_c, 5, pdFALSE},

   { 3, MSG_DIR_RECV, sizeof( OuterLoop_ang_ref ), (portCHAR *) & OuterLoop_ang_ref, (portCHAR *) & OuterLoop_ang_ref_c, 8, pdFALSE},

   { 4, MSG_DIR_SEND, sizeof( InnerLoop_thrust_commands ), (portCHAR *) & InnerLoop_thrust_commands, (portCHAR *) & InnerLoop_thrust_commands_c, 14, pdFALSE},

   { -1, 0, 0, NULL, NULL, 0, 0}
};

per_entry pers[] = {

   { 1, "ADC", ADC_EXT_AVCC, 0, ADC_PIN1, sizeof( DataHandling_sensor_data_in ), (portCHAR *) & DataHandling_sensor_data_in, (portCHAR *) & DataHandling_sensor_data_in_c, 1, NULL},

   { 2, "UART", OUT, 0, 0, sizeof( InnerLoop_thrust_commands ), (portCHAR *) & InnerLoop_thrust_commands, (portCHAR *) & InnerLoop_thrust_commands_c, 14, NULL},

   { -1, NULL, 0, 0, 0, 0, NULL, NULL, 0, NULL }
};


