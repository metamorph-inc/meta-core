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

    MsingleC12C1M DataHandler_pos_data;

} DataHandler_Pos_Data_msg_t;
DataHandler_Pos_Data_msg_t DataHandler_Pos_Data_msg, DataHandler_Pos_Data_msg_c;

typedef struct {

    MsingleC6C1M DataHandler_att_data;

    MsingleC6C1M InnerLoop_att_data;

} DataHandler_Att_Data_msg_t;
DataHandler_Att_Data_msg_t DataHandler_Att_Data_msg, DataHandler_Att_Data_msg_c;

typedef struct {

    MsingleC6C1M DataHandler_data2;

} DataHandler_PL_GPS_msg_t;
DataHandler_PL_GPS_msg_t DataHandler_PL_GPS_msg, DataHandler_PL_GPS_msg_c;

typedef struct {

    MsingleC9C1M DataHandler_data1;

} DataHandler_PL_IMU_msg_t;
DataHandler_PL_IMU_msg_t DataHandler_PL_IMU_msg, DataHandler_PL_IMU_msg_c;

typedef struct {

    MsingleC6C1M DataHandler_att_data;

    MsingleC6C1M InnerLoop_att_data;

} DataHandler_Att_Data_msg_t;
DataHandler_Att_Data_msg_t DataHandler_Att_Data_msg, DataHandler_Att_Data_msg_c;

typedef struct {

    MsingleC4C1M InnerLoop_att_ref;

} OuterLoop_Att_Ref_msg_t;
OuterLoop_Att_Ref_msg_t OuterLoop_Att_Ref_msg, OuterLoop_Att_Ref_msg_c;

typedef struct {

    MsingleC4C1M InnerLoop_Torque;

} InnerLoop_Thrust_Cmd_msg_t;
InnerLoop_Thrust_Cmd_msg_t InnerLoop_Thrust_Cmd_msg, InnerLoop_Thrust_Cmd_msg_c;


///////////////////////// C-CODE /////////////////////////


void app_tasks_init() { }

///////////////////////// TASKS /////////////////////////


static DataHandler_context DataHandler_ctxt;

static void DataHandler (void *parameters)
{


   task_entry * te = (task_entry *) parameters;

   DataHandler_init( &DataHandler_ctxt );

   for(;;)
   {
      xSemaphoreTake( te->semaphore, portMAX_DELAY );

      DataHandler_main(
       &DataHandler_ctxt,
       DataHandler_PL_IMU_msg.DataHandler_data1,
       DataHandler_PL_GPS_msg.DataHandler_data2,
       &DataHandler_Att_Data_msg.DataHandler_att_data,
       &DataHandler_Pos_Data_msg.DataHandler_pos_data
      );

   }

}

static InnerLoop_context InnerLoop_ctxt;

static void InnerLoop (void *parameters)
{


   task_entry * te = (task_entry *) parameters;

   InnerLoop_init( &InnerLoop_ctxt );

   for(;;)
   {
      xSemaphoreTake( te->semaphore, portMAX_DELAY );

      InnerLoop_main(
       &InnerLoop_ctxt,
       OuterLoop_Att_Ref_msg.InnerLoop_att_ref,
       DataHandler_Att_Data_msg.InnerLoop_att_data,
       &InnerLoop_Thrust_Cmd_msg.InnerLoop_Torque
      );

   }

}


///////////////////////// SCHEDULE /////////////////////////



portTickType hp_len = 20;

task_entry tasks[] = {

   { DataHandler, "DataHandler", 8, 0},
   { InnerLoop, "InnerLoop", 13, 0},
   {NULL, NULL, 0, 0}
};

msg_entry msgs[] = {

   { 1, MSG_DIR_RECV, sizeof( OuterLoop_Att_Ref_msg ), (portCHAR *) & OuterLoop_Att_Ref_msg, (portCHAR *) & OuterLoop_Att_Ref_msg_c, 7, pdFALSE},

   { 2, MSG_DIR_SEND, sizeof( DataHandler_Pos_Data_msg ), (portCHAR *) & DataHandler_Pos_Data_msg, (portCHAR *) & DataHandler_Pos_Data_msg_c, 11, pdFALSE},

   { -1, 0, 0, NULL, NULL, 0, 0}
};

per_entry pers[] = {

   { -1, NULL, 0, 0, 0, 0, NULL, NULL, 0, NULL }
};


