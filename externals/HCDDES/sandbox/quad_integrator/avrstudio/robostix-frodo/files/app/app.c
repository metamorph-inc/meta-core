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

    unsigned char header;
    A1A9singleAA InnerLoop_Torque;

} InnerLoop_thrust_commands_t;
InnerLoop_thrust_commands_t InnerLoop_thrust_commands, InnerLoop_thrust_commands_c[2];

typedef struct {

    unsigned char header;
    single InnerLoop_Torque;

} InnerLoop_thrust_commands2_t;
InnerLoop_thrust_commands2_t InnerLoop_thrust_commands2, InnerLoop_thrust_commands2_c[2];

typedef struct {

    A5singleA InnerLoop_ang_ref;

} OuterLoop_ang_ref_t;
OuterLoop_ang_ref_t OuterLoop_ang_ref, OuterLoop_ang_ref_c[2];

typedef struct {

    single DataHandling_ang_out;

} DataHandling_ang_msg_t;
DataHandling_ang_msg_t DataHandling_ang_msg, DataHandling_ang_msg_c[2];

typedef struct {

    single DataHandling_pos_out;

} DataHandling_pos_msg_t;
DataHandling_pos_msg_t DataHandling_pos_msg, DataHandling_pos_msg_c[2];

typedef struct {

    
    unsigned char header;
    single DataHandling_ang_in;

    single DataHandling_pos_in;

} DataHandling_sensor_data_in_t;
DataHandling_sensor_data_in_t DataHandling_sensor_data_in, DataHandling_sensor_data_in_c[2];


///////////////////////// INITIALIZATION /////////////////////////



static  InnerLoop_context InnerLoop_ctxt;
static  DataHandler_context DataHandler_ctxt;


void app_tasks_init() { 

   memset( (void *) &(DataHandling_sensor_data_in), 0, sizeof( DataHandling_sensor_data_in_t ) );
   memset( (void *) &(DataHandling_sensor_data_in_c), 0, sizeof( DataHandling_sensor_data_in_t ) );
   memset( (void *) &(OuterLoop_ang_ref), 0, sizeof( OuterLoop_ang_ref_t ) );
   memset( (void *) &(OuterLoop_ang_ref_c), 0, sizeof( OuterLoop_ang_ref_t ) );
   memset( (void *) &(DataHandling_pos_msg), 0, sizeof( DataHandling_pos_msg_t ) );
   memset( (void *) &(DataHandling_pos_msg_c), 0, sizeof( DataHandling_pos_msg_t ) );
   memset( (void *) &(InnerLoop_thrust_commands), 0, sizeof( InnerLoop_thrust_commands_t ) );
   memset( (void *) &(InnerLoop_thrust_commands_c), 0, sizeof( InnerLoop_thrust_commands_t ) );

   InnerLoop_thrust_commands.header = 147;
   InnerLoop_thrust_commands2.header = 147;

   InnerLoop_init( &InnerLoop_ctxt );
   DataHandler_init( &DataHandler_ctxt );


}

///////////////////////// TASKS /////////////////////////


static void InnerLoop (void *parameters)
{
   //float *msg_data_ptr = &(InnerLoop_thrust_commands.InnerLoop_Torque[1]);
   float *msg_data_ptr = &(InnerLoop_thrust_commands2.InnerLoop_Torque);

   task_entry * te = (task_entry *) parameters;

   for(;;)
   {
      xSemaphoreTake( te->semaphore, portMAX_DELAY );

      DDRC|=0x10;
      PORTC|=0x10;
      InnerLoop_main(
       &InnerLoop_ctxt,
       OuterLoop_ang_ref.InnerLoop_ang_ref,
       DataHandling_ang_msg.DataHandling_ang_out,
       &InnerLoop_thrust_commands.InnerLoop_Torque
      );
      PORTC&=~0x10;

      memcpy( msg_data_ptr, &(DataHandling_sensor_data_in.DataHandling_ang_in),
              sizeof( float ) );
      //memcpy( msg_data_ptr+1, &(DataHandling_sensor_data_in_c[0].DataHandling_ang_in),
      //        sizeof( float ) );
      //memcpy( msg_data_ptr+2, &(DataHandling_sensor_data_in_c[1].DataHandling_ang_in),
      //        sizeof( float ) );
      

      //memcpy(msg_data_ptr+1,&(DataHandling_sensor_data_in.DataHandling_pos_in),
      //        sizeof( float ) );
      //memcpy(msg_data_ptr+2,&(OuterLoop_ang_ref.InnerLoop_ang_ref[0]), 
      //        sizeof( float ) );
   }

}

static void DataHandling (void *parameters)
{


   task_entry * te = (task_entry *) parameters;


   for(;;)
   {
      xSemaphoreTake( te->semaphore, portMAX_DELAY );
      

      DDRC|=0x10;
      PORTC|=0x10;
      DataHandler_main(
       &DataHandler_ctxt,
       DataHandling_sensor_data_in.DataHandling_ang_in,
       DataHandling_sensor_data_in.DataHandling_pos_in,
       &DataHandling_ang_msg	.DataHandling_ang_out,
       &DataHandling_pos_msg.DataHandling_pos_out
      );
      PORTC&=~0x10;
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

   { 1, MSG_DIR_RECV, sizeof( OuterLoop_ang_ref ), (portCHAR *) & OuterLoop_ang_ref, (portCHAR *) OuterLoop_ang_ref_c, 7, 0, 0},

   { 2, MSG_DIR_SEND, sizeof( DataHandling_pos_msg ), (portCHAR *) & DataHandling_pos_msg, (portCHAR *) DataHandling_pos_msg_c, 11, 0, 0},

   { -1, 0, 0, NULL, NULL, 0, 0, 0}
};

per_entry pers[] = {

   { 1, "UART", IN, 0, 0, sizeof( DataHandling_sensor_data_in ), (portCHAR *) & DataHandling_sensor_data_in, (portCHAR *) DataHandling_sensor_data_in_c, 2, NULL, 0, 0},

   //{ 2, "UART", OUT, 0, 0, sizeof( InnerLoop_thrust_commands ), (portCHAR *) & InnerLoop_thrust_commands, (portCHAR *) InnerLoop_thrust_commands_c, 14, NULL, 0, 0},
   { 2, "UART", OUT, 0, 0, sizeof( InnerLoop_thrust_commands2 ), (portCHAR *) & InnerLoop_thrust_commands2, (portCHAR *) InnerLoop_thrust_commands2_c, 14, NULL, 0, 0},

   { -1, NULL, 0, 0, 0, 0, NULL, NULL, 0, NULL, 0, 0 }
};


