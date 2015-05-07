// Emit one of these templates for each node in the model

//
// Generated task code (FRODO Code Generator). Do not edit!
// Date: 
//

///////////////// INCLUDES ////////////////////////


#include "peri.h"


#include "InnerLoop_sl.h"
#include "DataHandling.h"


///////////////// LOCAL MESSAGES ////////////////////////


typedef struct {

    MsingleC1C1M InnerLoop_ang_ref;

} OuterLoop_ang_ref_t;
OuterLoop_ang_ref_t OuterLoop_ang_ref, OuterLoop_ang_ref_c;

typedef struct {

    MsingleC1C1M InnerLoop_angle;

    float DataHandling_angle_data_out;

    float DataHandling_pos_data_out2;

} DataHandling_sensor_data_out_t;
DataHandling_sensor_data_out_t DataHandling_sensor_data_out, DataHandling_sensor_data_out_c;

typedef struct {


    unsigned char header;
    MsingleC1C1M InnerLoop_ang_err;
    MsingleC1C1M InnerLoop_Torque;
    MsingleC1C1M InnerLoop_ang_vel;

} InnerLoop_thrust_commands_t;
InnerLoop_thrust_commands_t InnerLoop_thrust_commands, InnerLoop_thrust_commands_c;

/*typedef struct {

    MsingleC1C1M InnerLoop_angle;

    float DataHandling_angle_data_out;

    float DataHandling_pos_data_out2;

} DataHandling_sensor_data_out_t;
*/
DataHandling_sensor_data_out_t DataHandling_sensor_data_out, DataHandling_sensor_data_out_c;

typedef struct {

    float DataHandling_pos_data_out;

    float DataHandling_pos_ref_out;

} DataHandling_pos_msg_t;
DataHandling_pos_msg_t DataHandling_pos_msg, DataHandling_pos_msg_c;

typedef struct {

    unsigned short DataHandling_pos_ref;

} DataHandling_pos_ref_t;
DataHandling_pos_ref_t DataHandling_pos_ref, DataHandling_pos_ref_c;

typedef struct {
    unsigned char header;
    float DataHandling_pos_data;

    float DataHandling_angle_data;

} DataHandling_sensor_data_in_t;
DataHandling_sensor_data_in_t DataHandling_sensor_data_in, DataHandling_sensor_data_in_c;


///////////////////////// C-CODE /////////////////////////

/*typedef void *DataHandling_context;
static void DataHandling_init(DataHandling_context *DataHandling_ctxt ) { }

static void DataHandling_main(
 DataHandling_context *DataHandling_ctxt,
 unsigned short DataHandling_pos_ref,
 float DataHandling_pos_data,
 float DataHandling_angle_data,
 float *DataHandling_pos_data_out,
 float *DataHandling_angle_data_out,
 float *DataHandling_pos_ref_out,
 float *DataHandling_pos_data_out2 ) {
    *pos_ref_out = 5.0 * (((float)pos_ref)/1024.0);  *pos_data_out = pos_data; *pos_data_out2 = pos_data; *angle_data_out = angle_data;
}*/

static InnerLoop_context InnerLoop_ctxt;
static DataHandling_context DataHandling_ctxt;

void app_tasks_init() { 
   
   memset( &DataHandling_sensor_data_in,0,sizeof(DataHandling_sensor_data_in_t) );
   memset( &DataHandling_sensor_data_in_c,0,sizeof(DataHandling_sensor_data_in_t) );
   memset( &DataHandling_pos_ref,0,sizeof(DataHandling_pos_ref_t));
   memset( &DataHandling_pos_ref_c,0,sizeof(DataHandling_pos_ref_t));
   memset( &DataHandling_pos_msg,0,sizeof(DataHandling_pos_msg_t));
   memset( &DataHandling_pos_msg_c,0,sizeof(DataHandling_pos_msg_t));
   memset( &DataHandling_sensor_data_out,0,sizeof(DataHandling_sensor_data_out_t));
   memset( &DataHandling_sensor_data_out_c,0,sizeof(DataHandling_sensor_data_out_t));
   memset( &InnerLoop_thrust_commands,0,sizeof(InnerLoop_thrust_commands_t));
   memset( &InnerLoop_thrust_commands_c,0,sizeof(InnerLoop_thrust_commands_t));
   memset( &OuterLoop_ang_ref,0,sizeof(OuterLoop_ang_ref_t));
   memset( &OuterLoop_ang_ref_c,0,sizeof(OuterLoop_ang_ref_t));

   InnerLoop_thrust_commands.header = 147;
   InnerLoop_thrust_commands.InnerLoop_ang_err = 0.0;
   InnerLoop_thrust_commands.InnerLoop_Torque = 0.0;
   InnerLoop_thrust_commands.InnerLoop_ang_vel = 0.0;

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
      
      DDRC |= 0x40;
      PORTC|= 0x40;

      InnerLoop_main(
       &InnerLoop_ctxt,
       OuterLoop_ang_ref.InnerLoop_ang_ref,
       DataHandling_sensor_data_out.InnerLoop_angle,
       &InnerLoop_thrust_commands.InnerLoop_ang_err,
       &InnerLoop_thrust_commands.InnerLoop_Torque,
       &InnerLoop_thrust_commands.InnerLoop_ang_vel
      );
      
      PORTC &=~0x40;

   }

}


static void DataHandling (void *parameters)
{


   task_entry * te = (task_entry *) parameters;


   for(;;)
   {
      xSemaphoreTake( te->semaphore, portMAX_DELAY );

      DDRC |= 0x40;
      PORTC|= 0x40;

      DataHandling_main(
       &DataHandling_ctxt,
       DataHandling_pos_ref.DataHandling_pos_ref,
       DataHandling_sensor_data_in.DataHandling_pos_data,
       DataHandling_sensor_data_in.DataHandling_angle_data,
       &DataHandling_pos_msg.DataHandling_pos_data_out,
       &DataHandling_sensor_data_out.DataHandling_angle_data_out,
       &DataHandling_pos_msg.DataHandling_pos_ref_out,
       &DataHandling_sensor_data_out.DataHandling_pos_data_out2
      );
      
      PORTC &=~0x40;
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

   { 1, MSG_DIR_SEND, sizeof( DataHandling_pos_msg ), (portCHAR *) & DataHandling_pos_msg, (portCHAR *) & DataHandling_pos_msg_c, 5, pdFALSE},
   { 2, MSG_DIR_RECV, sizeof( OuterLoop_ang_ref ), (portCHAR *) & OuterLoop_ang_ref, (portCHAR *) & OuterLoop_ang_ref_c, 8, pdFALSE},
   { -1, 0, 0, NULL, NULL, 0, 0}
};

per_entry pers[] = {

  {1,"ADC",ADC_EXT_AVCC,0,ADC_PIN1,sizeof(DataHandling_pos_ref_t),(portCHAR*)&DataHandling_pos_ref,(portCHAR*)&DataHandling_pos_ref_c,4,NULL},
  {2,"UART",OUT,1,0,sizeof(InnerLoop_thrust_commands_t),(portCHAR*)&InnerLoop_thrust_commands,(portCHAR*)&InnerLoop_thrust_commands_c,9,NULL},
  {3,"UART",IN,1,0,sizeof(DataHandling_sensor_data_in_t),(portCHAR*)&DataHandling_sensor_data_in,(portCHAR*)&DataHandling_sensor_data_in_c,12,NULL},
  {-1,NULL,0,0,0,0,NULL,NULL,0,NULL},
};
   


