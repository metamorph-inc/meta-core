// Emit one of these templates for each node in the model

//
// Generated task code (FRODO Code Generator). Do not edit!
// Date: 
//

///////////////// INCLUDES ////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include "gumstix-frodo.h"
#include "Log.h"



#include "OuterLoop_sl.h"


///////////////// LOCAL MESSAGES ////////////////////////


typedef struct {

    MsingleC4C1M OuterLoop_pos_ref;

} DataHandler_Pos_Data_msg_t;
DataHandler_Pos_Data_msg_t DataHandler_Pos_Data_msg, DataHandler_Pos_Data_msg_c;

typedef struct {

    MsingleC4C1M OuterLoop_att_ref;

} OuterLoop_Att_Ref_msg_t;
OuterLoop_Att_Ref_msg_t OuterLoop_Att_Ref_msg, OuterLoop_Att_Ref_msg_c;

typedef struct {

    float[4] RefSocketHandler_pos_ref;

} RefSocketHandler_Pos_Ref_msg_t;
RefSocketHandler_Pos_Ref_msg_t RefSocketHandler_Pos_Ref_msg, RefSocketHandler_Pos_Ref_msg_c;

typedef struct {

    float[4] RefSocketHandler_pos_ref;

} RefSocketHandler_Pos_Ref_msg_t;
RefSocketHandler_Pos_Ref_msg_t RefSocketHandler_Pos_Ref_msg, RefSocketHandler_Pos_Ref_msg_c;

typedef struct {

     RefSocketHandler_raw_pos_ref;

} RefSocketHandler_PosRefSocketData_t;
RefSocketHandler_PosRefSocketData_t RefSocketHandler_PosRefSocketData, RefSocketHandler_PosRefSocketData_c;


///////////////////////// C-CODE /////////////////////////


void app_tasks_init() { }

///////////////////////// TASKS /////////////////////////


static OuterLoop_context OuterLoop_ctxt;

static void OuterLoop (void *parameters)
{

      OuterLoop_main(
       &OuterLoop_ctxt,
       DataHandler_Pos_Data_msg.OuterLoop_pos_ref,
       RefSocketHandler_Pos_Ref_msg.OuterLoop_pos_data,
       &OuterLoop_Att_Ref_msg.OuterLoop_att_ref
      );

}

static RefSocketHandler_context RefSocketHandler_ctxt;

static void RefSocketHandler (void *parameters)
{

      RefSocketHandler_main(
       &RefSocketHandler_ctxt,
       RefSocketHandler_PosRefSocketData.RefSocketHandler_raw_pos_ref,
       &RefSocketHandler_Pos_Ref_msg.RefSocketHandler_pos_ref
      );

}


///////////////////////// SCHEDULE /////////////////////////



portTickType hp_len = 0;

task_entry tasks[] = {

   { RefSocketHandler, "RefSocketHandler", 5 },
   { OuterLoop, "OuterLoop", 10 },
   {NULL, NULL, 0, 0}
};

msg_entry msgs[] = {

   { 1, MSG_DIR_SEND, sizeof( OuterLoop_Att_Ref_msg ), (portCHAR *) & OuterLoop_Att_Ref_msg, 7 },
   { 2, MSG_DIR_RECV, sizeof( DataHandler_Pos_Data_msg ), (portCHAR *) & DataHandler_Pos_Data_msg, 11 },
   { -1, 0, 0, NULL, 0}
};




