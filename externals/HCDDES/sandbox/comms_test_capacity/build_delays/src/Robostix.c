// Emit one of these templates for each node in the model

//
// Generated task code (FRODO Code Generator). Do not edit!
// Date: 
//

///////////////// INCLUDES ////////////////////////


#include "peri.h"


#include "DataHandler_sl.h"
#include "InnerLoop_sl.h"


///////////////// LOCAL MESSAGES ////////////////////////


typedef struct {

    A6floatA DataHandler_att_data;

} DataHandler_Att_Data_msg_t;
DataHandler_Att_Data_msg_t DataHandler_Att_Data_msg, DataHandler_Att_Data_msg_c[2];

typedef struct {

    A6floatA DataHandler_data2;

    A9floatA DataHandler_data1;

} DataHandler_PL_IMU_GPS_msg_t;
DataHandler_PL_IMU_GPS_msg_t DataHandler_PL_IMU_GPS_msg, DataHandler_PL_IMU_GPS_msg_c[2];

typedef struct {

    A12floatA OuterLoop_pos_data;

} DataHandler_Pos_Data_msg_t;
DataHandler_Pos_Data_msg_t DataHandler_Pos_Data_msg, DataHandler_Pos_Data_msg_c[2];

typedef struct {

    A4floatA OuterLoop_att_ref;

} OuterLoop_Att_Ref_msg_t;
OuterLoop_Att_Ref_msg_t OuterLoop_Att_Ref_msg, OuterLoop_Att_Ref_msg_c[2];

typedef struct {

    float[4] RefSocketHandler_raw_pos_ref;

} RefSocketHandler_PosRefSocketData_t;
RefSocketHandler_PosRefSocketData_t RefSocketHandler_PosRefSocketData, RefSocketHandler_PosRefSocketData_c[2];

typedef struct {

    A6floatA DataHandler_att_data;

} DataHandler_Att_Data_msg_t;
DataHandler_Att_Data_msg_t DataHandler_Att_Data_msg, DataHandler_Att_Data_msg_c[2];

typedef struct {

    A6floatA DataHandler_data2;

    A9floatA DataHandler_data1;

} DataHandler_PL_IMU_GPS_msg_t;
DataHandler_PL_IMU_GPS_msg_t DataHandler_PL_IMU_GPS_msg, DataHandler_PL_IMU_GPS_msg_c[2];

typedef struct {

    A12floatA OuterLoop_pos_data;

} DataHandler_Pos_Data_msg_t;
DataHandler_Pos_Data_msg_t DataHandler_Pos_Data_msg, DataHandler_Pos_Data_msg_c[2];

typedef struct {

    A4floatA InnerLoop_Torque;

} InnerLoop_Thrust_Cmd_msg_t;
InnerLoop_Thrust_Cmd_msg_t InnerLoop_Thrust_Cmd_msg, InnerLoop_Thrust_Cmd_msg_c[2];

typedef struct {

    A4floatA OuterLoop_att_ref;

} OuterLoop_Att_Ref_msg_t;
OuterLoop_Att_Ref_msg_t OuterLoop_Att_Ref_msg, OuterLoop_Att_Ref_msg_c[2];

typedef struct {

    float[4] RefSocketHandler_raw_pos_ref;

} RefSocketHandler_PosRefSocketData_t;
RefSocketHandler_PosRefSocketData_t RefSocketHandler_PosRefSocketData, RefSocketHandler_PosRefSocketData_c[2];


///////////////////////// INITIALIZATION /////////////////////////



static  DataHandler_context DataHandler_ctxt;
static  InnerLoop_context InnerLoop_ctxt;


void app_tasks_init() { 

   memset( (void *) &(DataHandler_Att_Data_msg), 0, sizeof( DataHandler_Att_Data_msg_t ) );
   memset( (void *) &(DataHandler_Att_Data_msg_c), 0, sizeof( DataHandler_Att_Data_msg_t[2] ) );
   memset( (void *) &(DataHandler_PL_IMU_GPS_msg), 0, sizeof( DataHandler_PL_IMU_GPS_msg_t ) );
   memset( (void *) &(DataHandler_PL_IMU_GPS_msg_c), 0, sizeof( DataHandler_PL_IMU_GPS_msg_t[2] ) );
   memset( (void *) &(DataHandler_Pos_Data_msg), 0, sizeof( DataHandler_Pos_Data_msg_t ) );
   memset( (void *) &(DataHandler_Pos_Data_msg_c), 0, sizeof( DataHandler_Pos_Data_msg_t[2] ) );
   memset( (void *) &(InnerLoop_Thrust_Cmd_msg), 0, sizeof( InnerLoop_Thrust_Cmd_msg_t ) );
   memset( (void *) &(InnerLoop_Thrust_Cmd_msg_c), 0, sizeof( InnerLoop_Thrust_Cmd_msg_t[2] ) );
   memset( (void *) &(OuterLoop_Att_Ref_msg), 0, sizeof( OuterLoop_Att_Ref_msg_t ) );
   memset( (void *) &(OuterLoop_Att_Ref_msg_c), 0, sizeof( OuterLoop_Att_Ref_msg_t[2] ) );
   memset( (void *) &(RefSocketHandler_PosRefSocketData), 0, sizeof( RefSocketHandler_PosRefSocketData_t ) );
   memset( (void *) &(RefSocketHandler_PosRefSocketData_c), 0, sizeof( RefSocketHandler_PosRefSocketData_t[2] ) );



   DataHandler_init( &DataHandler_ctxt );
   InnerLoop_init( &InnerLoop_ctxt );


}

///////////////////////// TASKS /////////////////////////


static void DataHandler (void *parameters)
{


   task_entry * te = (task_entry *) parameters;


   for(;;)
   {
      xSemaphoreTake( te->semaphore, portMAX_DELAY );

      DataHandler_main(
       &DataHandler_ctxt,
       DataHandler_PL_IMU_GPS_msg.DataHandler_data1,
       DataHandler_PL_IMU_GPS_msg.DataHandler_data2,
       &DataHandler_Att_Data_msg.DataHandler_att_data,
       &DataHandler_Pos_Data_msg.DataHandler_pos_data
      );

   }

}

static void InnerLoop (void *parameters)
{


   task_entry * te = (task_entry *) parameters;


   for(;;)
   {
      xSemaphoreTake( te->semaphore, portMAX_DELAY );

      InnerLoop_main(
       &InnerLoop_ctxt,
       OuterLoop_Att_Ref_msg.InnerLoop_att_ref,
       DataHandler_Att_Data_msg.DataHandler_att_data,
       &InnerLoop_Thrust_Cmd_msg.InnerLoop_Torque
      );

   }

}


///////////////////////// SCHEDULE /////////////////////////



portTickType hp_len = 20;

task_entry tasks[] = {

   { DataHandler, "DataHandler", 9, 0},
   { InnerLoop, "InnerLoop", 10, 0},
   {NULL, NULL, 0, 0}
};

msg_entry msgs[] = {

   { 1, MSG_DIR_RECV, sizeof( DataHandler_Pos_Data_msg ), (portCHAR *) & DataHandler_Pos_Data_msg, (portCHAR *) DataHandler_Pos_Data_msg_c, 14, pdFALSE},

   { 2, MSG_DIR_SEND, sizeof( OuterLoop_Att_Ref_msg ), (portCHAR *) & OuterLoop_Att_Ref_msg, (portCHAR *) OuterLoop_Att_Ref_msg_c, 17, pdFALSE},

   { -1, 0, 0, NULL, NULL, 0, 0}
};

per_entry pers[] = {

   { 1, "", , 0, , sizeof( RefSocketHandler_PosRefSocketData ), (portCHAR *) RefSocketHandler_PosRefSocketData, (portCHAR *) & RefSocketHandler_PosRefSocketData_c, 4, NULL, 0, 0 },

   { 2, "UART", IN, 0, 0, sizeof( DataHandler_PL_IMU_GPS_msg ), (portCHAR *) DataHandler_PL_IMU_GPS_msg, (portCHAR *) & DataHandler_PL_IMU_GPS_msg_c, 5, NULL, 0, 0 },

   { 3, "UART", OUT, 0, 0, sizeof( InnerLoop_Thrust_Cmd_msg ), (portCHAR *) InnerLoop_Thrust_Cmd_msg, (portCHAR *) & InnerLoop_Thrust_Cmd_msg_c, 11, NULL, 0, 0 },

   { -1, NULL, 0, 0, 0, 0, NULL, NULL, 0, NULL, 0, 0 }
};


