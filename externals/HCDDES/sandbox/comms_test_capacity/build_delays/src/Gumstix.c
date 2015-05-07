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
#include "CCode.h"


///////////////// LOCAL MESSAGES ////////////////////////


typedef struct {

    A12floatA OuterLoop_pos_data;

} DataHandler_Pos_Data_msg_t;
DataHandler_Pos_Data_msg_t DataHandler_Pos_Data_msg, DataHandler_Pos_Data_msg_c[2];

typedef struct {

    A4floatA OuterLoop_att_ref;

} OuterLoop_Att_Ref_msg_t;
OuterLoop_Att_Ref_msg_t OuterLoop_Att_Ref_msg, OuterLoop_Att_Ref_msg_c[2];

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


///////////////////////// INITIALIZATION /////////////////////////



static  OuterLoop_context OuterLoop_ctxt;
static  CCode_context CCode_ctxt;


void app_tasks_init() { 

   memset( (void *) &(DataHandler_Pos_Data_msg), 0, sizeof( DataHandler_Pos_Data_msg_t ) );
   memset( (void *) &(DataHandler_Pos_Data_msg_c), 0, sizeof( DataHandler_Pos_Data_msg_t[2] ) );
   memset( (void *) &(OuterLoop_Att_Ref_msg), 0, sizeof( OuterLoop_Att_Ref_msg_t ) );
   memset( (void *) &(OuterLoop_Att_Ref_msg_c), 0, sizeof( OuterLoop_Att_Ref_msg_t[2] ) );
   memset( (void *) &(RefSocketHandler_PosRefSocketData), 0, sizeof( RefSocketHandler_PosRefSocketData_t ) );
   memset( (void *) &(RefSocketHandler_PosRefSocketData_c), 0, sizeof( RefSocketHandler_PosRefSocketData_t[2] ) );



   OuterLoop_init( &OuterLoop_ctxt );
   CCode_init( &CCode_ctxt );


}

///////////////////////// TASKS /////////////////////////


static void OuterLoop (void *parameters)
{

      OuterLoop_main(
       &OuterLoop_ctxt,
       RefSocketHandler_Pos_Ref_msg.RefSocketHandler_pos_ref,
       DataHandler_Pos_Data_msg.OuterLoop_pos_data,
       &OuterLoop_Att_Ref_msg.OuterLoop_att_ref
      );

}

static void RefSocketHandler (void *parameters)
{

      CCode_main(
       &CCode_ctxt,
       RefSocketHandler_PosRefSocketData.RefSocketHandler_raw_pos_ref,
       &RefSocketHandler_Pos_Ref_msg.RefSocketHandler_pos_ref
      );

}


///////////////////////// SCHEDULE /////////////////////////



portTickType hp_len = 20;

task_entry tasks[] = {

   { RefSocketHandler, "RefSocketHandler", 7 },
   { OuterLoop, "OuterLoop", 12 },
   {NULL, NULL, 0, 0}
};

msg_entry msgs[] = {

   { 1, MSG_DIR_RECV, sizeof( DataHandler_Pos_Data_msg ), (portCHAR *) & DataHandler_Pos_Data_msg, 14 },
   { 2, MSG_DIR_SEND, sizeof( OuterLoop_Att_Ref_msg ), (portCHAR *) & OuterLoop_Att_Ref_msg, 17 },
   { -1, 0, 0, NULL, 0}
};




