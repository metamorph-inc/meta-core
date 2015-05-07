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

    MsingleC1C1M OuterLoop_vel_err;

    MsingleC1C1M OuterLoop_ang_ref;

} OuterLoop_ang_ref_t;
OuterLoop_ang_ref_t OuterLoop_ang_ref, OuterLoop_ang_ref_c;

typedef struct {

    MsingleC1C1M OuterLoop_pos_ref;

    MsingleC1C1M OuterLoop_pos;

} DataHandling_pos_msg_t;
DataHandling_pos_msg_t DataHandling_pos_msg, DataHandling_pos_msg_c;


///////////////////////// C-CODE /////////////////////////

static OuterLoop_context OuterLoop_ctxt;


void app_tasks_init() { 

    memset( &DataHandling_pos_msg, 0, sizeof(DataHandling_pos_msg_t) );
    memset( &DataHandling_pos_msg_c, 0, sizeof(DataHandling_pos_msg_t) );

    memset( &OuterLoop_ang_ref, 0, sizeof(OuterLoop_ang_ref_t) );
    memset( &OuterLoop_ang_ref_c, 0, sizeof(OuterLoop_ang_ref_t) );


    OuterLoop_init( &OuterLoop_ctxt );

}

///////////////////////// TASKS /////////////////////////



static void OuterLoop (void *parameters)
{

      OuterLoop_main(
       &OuterLoop_ctxt,
       DataHandling_pos_msg.OuterLoop_pos_ref,
       DataHandling_pos_msg.OuterLoop_pos,
       &OuterLoop_ang_ref.OuterLoop_vel_err,
       &OuterLoop_ang_ref.OuterLoop_ang_ref
      );

}


///////////////////////// SCHEDULE /////////////////////////



portTickType hp_len = 20;

task_entry tasks[] = {

   { OuterLoop, "OuterLoop", 14},
   {NULL, NULL, 0, 0}
};

msg_entry msgs[] = {

   { 1, MSG_DIR_RECV, sizeof( DataHandling_pos_msg ), (portCHAR *) & DataHandling_pos_msg, 7 },
   { 2, MSG_DIR_SEND, sizeof( OuterLoop_ang_ref ), (portCHAR *) & OuterLoop_ang_ref, 10},
   { -1, 0, 0, NULL, 0}
};




