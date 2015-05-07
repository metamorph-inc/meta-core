// Emit one of these templates for each node in the model

//
// Generated task code (FRODO Code Generator). Do not edit!
// Date: 
//

///////////////// INCLUDES ////////////////////////


#include "gumstix-frodo.h"

#include "OuterLoop_sl.h"

#include "Log.h"


///////////////// LOCAL MESSAGES ////////////////////////


typedef struct {

    MsingleC1C1M OuterLoop_ang_ref;

} OuterLoop_OL_to_IL_t;
OuterLoop_OL_to_IL_t OuterLoop_OL_to_IL, OuterLoop_OL_to_IL_c;

typedef struct {

    MsingleC1C1M OuterLoop_pos;

} DataHandler_DH_to_OL_t;
DataHandler_DH_to_OL_t DataHandler_DH_to_OL, DataHandler_DH_to_OL_c;

static OuterLoop_100000011_context OuterLoop_ctxt;

///////////////////////// C-CODE /////////////////////////


void app_tasks_init() {
   memset( (void*)&OuterLoop_OL_to_IL , 0, sizeof(OuterLoop_OL_to_IL_t ) );
   memset( (void*)&OuterLoop_OL_to_IL , 0, sizeof(OuterLoop_OL_to_IL_t ) );
   memset( (void*)&DataHandler_DH_to_OL , 0, sizeof(DataHandler_DH_to_OL_t ) );
   memset( (void*)&DataHandler_DH_to_OL , 0, sizeof(DataHandler_DH_to_OL_t ) );

   OuterLoop_100000011_init( &OuterLoop_ctxt );
 }

///////////////////////// TASKSLED_TOGGLE( YELLOW ); /////////////////////////



static void OuterLoop (void *parameters)
{
      static float prev = 0.0;

      OuterLoop_100000011_main(
       &OuterLoop_ctxt,
       DataHandler_DH_to_OL.OuterLoop_pos,
       &OuterLoop_OL_to_IL.OuterLoop_ang_ref
      );

      float diff = OuterLoop_OL_to_IL.OuterLoop_ang_ref - (0.5) * prev;
      if ( diff < 0.0 ) diff = -diff;

      if ( diff > 0.05 )
         Log( "prev %f cur %f\n", prev, OuterLoop_OL_to_IL.OuterLoop_ang_ref );

      prev = DataHandler_DH_to_OL.OuterLoop_pos;
}


///////////////////////// SCHEDULE /////////////////////////



portTickType hp_len = 20;

task_entry tasks[] = {

   { OuterLoop, "OuterLoop", 10},

   {NULL, NULL, 0, 0}
};

msg_entry msgs[] = {

   { 1, MSG_DIR_RECV, sizeof( DataHandler_DH_to_OL ), (portCHAR *) & DataHandler_DH_to_OL, 7 },

   { 2, MSG_DIR_SEND, sizeof( OuterLoop_OL_to_IL ), (portCHAR *) & OuterLoop_OL_to_IL, 13 },

   { -1, 0, 0, NULL, 0 }
};



