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

    float RefSocketHandler_pos_ref[4];

} RefSocketHandler_Pos_Ref_msg_t;

RefSocketHandler_Pos_Ref_msg_t RefSocketHandler_Pos_Ref_msg, RefSocketHandler_Pos_Ref_msg_c;

/*
typedef struct {

    float RefSocketHandler_pos_ref[4];

} RefSocketHandler_Pos_Ref_msg_t;
RefSocketHandler_Pos_Ref_msg_t RefSocketHandler_Pos_Ref_msg, RefSocketHandler_Pos_Ref_msg_c;
*/
/* typedef struct {

     RefSocketHandler_raw_pos_ref;

} RefSocketHandler_PosRefSocketData_t;
RefSocketHandler_PosRefSocketData_t RefSocketHandler_PosRefSocketData, RefSocketHandler_PosRefSocketData_c; */

///////////////////////// C-CODE /////////////////////////

static OuterLoop_context OuterLoop_ctxt;

void app_tasks_init() {

   memset( &DataHandler_Pos_Data_msg, 0, sizeof( DataHandler_Pos_Data_msg_t ) );
   memset( &DataHandler_Pos_Data_msg_c, 0, sizeof(DataHandler_Pos_Data_msg_c ) );


   memset( &OuterLoop_Att_Ref_msg, 0, sizeof(OuterLoop_Att_Ref_msg_t ) );
   memset( &OuterLoop_Att_Ref_msg_c, 0, sizeof(OuterLoop_Att_Ref_msg_t ) );

   memset( &RefSocketHandler_Pos_Ref_msg, 0, sizeof( RefSocketHandler_Pos_Ref_msg_t ) );
   memset( &RefSocketHandler_Pos_Ref_msg_c, 0, sizeof( RefSocketHandler_Pos_Ref_msg_t ) );

   RefSocketHandler_Pos_Ref_msg.RefSocketHandler_pos_ref[0] = 1.0;
   RefSocketHandler_Pos_Ref_msg.RefSocketHandler_pos_ref[1] = 1.0;
   RefSocketHandler_Pos_Ref_msg.RefSocketHandler_pos_ref[2] = 1.0;
   RefSocketHandler_Pos_Ref_msg.RefSocketHandler_pos_ref[3] = 1.0;

   RefSocketHandler_Pos_Ref_msg_c.RefSocketHandler_pos_ref[0] = 1.0;
   RefSocketHandler_Pos_Ref_msg_c.RefSocketHandler_pos_ref[1] = 1.0;
   RefSocketHandler_Pos_Ref_msg_c.RefSocketHandler_pos_ref[2] = 1.0;
   RefSocketHandler_Pos_Ref_msg_c.RefSocketHandler_pos_ref[3] = 1.0;

   OuterLoop_init( &OuterLoop_ctxt );
}

///////////////////////// TASKS /////////////////////////


static void OuterLoop (void *parameters)
{

      OuterLoop_main(
       &OuterLoop_ctxt,
       DataHandler_Pos_Data_msg.OuterLoop_pos_ref,
       RefSocketHandler_Pos_Ref_msg.RefSocketHandler_pos_ref,
       &OuterLoop_Att_Ref_msg.OuterLoop_att_ref
      );

}

/*
static RefSocketHandler_context RefSocketHandler_ctxt;

static void RefSocketHandler (void *parameters)
{

      RefSocketHandler_main(
       &RefSocketHandler_ctxt,
       RefSocketHandler_PosRefSocketData.RefSocketHandler_raw_pos_ref,
       &RefSocketHandler_Pos_Ref_msg.RefSocketHandler_pos_ref
      );

}
*/

///////////////////////// SCHEDULE /////////////////////////



portTickType hp_len = 20;

task_entry tasks[] = {

  /* { RefSocketHandler, "RefSocketHandler", 5 }, */
   { OuterLoop, "OuterLoop", 12 },
   {NULL, NULL, 0, 0}
};

msg_entry msgs[] = {

   { 1, MSG_DIR_RECV, sizeof( DataHandler_Pos_Data_msg ), (portCHAR *) & DataHandler_Pos_Data_msg, 9 },
   { 2, MSG_DIR_SEND, sizeof( OuterLoop_Att_Ref_msg ), (portCHAR *) & OuterLoop_Att_Ref_msg, 14 },
   { -1, 0, 0, NULL, 0}
};




