// Emit one of these templates for each node in the model

//
// Generated task code (FRODO Code Generator). Do not edit!
// Date: 
//

///////////////// INCLUDES ////////////////////////

#include "peri.h"

#include "InnerLoop_sl.h"


///////////////// LOCAL MESSAGES ////////////////////////


typedef struct {

    MsingleC1C1M InnerLoop_angle;

} OuterLoop_OL_to_IL_t;
OuterLoop_OL_to_IL_t OuterLoop_OL_to_IL, OuterLoop_OL_to_IL_c;

typedef struct {
   
    unsigned char header;
    MsingleC1C1M InnerLoop_Torque;

} InnerLoop_IL_to_PL_t;
InnerLoop_IL_to_PL_t InnerLoop_IL_to_PL, InnerLoop_IL_to_PL_c;

typedef struct {

    float DataHandler_MsgOut;

} DataHandler_DH_to_OL_t;
DataHandler_DH_to_OL_t DataHandler_DH_to_OL, DataHandler_DH_to_OL_c;

typedef struct {

    unsigned char header;
    float DataHandler_SerialIn;

} DataHandler_PL_to_DH_t;
DataHandler_PL_to_DH_t DataHandler_PL_to_DH, DataHandler_PL_to_DH_c;


///////////////////////// C-CODE /////////////////////////

typedef void *DataHandler_context;
static void DataHandler_init(DataHandler_context *DataHandler_ctxt) { }

static void DataHandler_main(
  DataHandler_context *DataHandler_ctxt,
  float DataHandler_SerialIn,
  float *DataHandler_MsgOut ) {
      *DataHandler_MsgOut = DataHandler_SerialIn;
}

void app_tasks_init() {
  memset( (void*)&OuterLoop_OL_to_IL, 0, sizeof(OuterLoop_OL_to_IL_t) );
  memset( (void*)&OuterLoop_OL_to_IL_c, 0, sizeof(OuterLoop_OL_to_IL_t) );
  memset( (void*)&InnerLoop_IL_to_PL, 0, sizeof(InnerLoop_IL_to_PL_t) );
  memset( (void*)&InnerLoop_IL_to_PL_c, 0, sizeof(InnerLoop_IL_to_PL_t) );
  memset( (void*)&DataHandler_DH_to_OL, 0, sizeof(DataHandler_DH_to_OL_t) );
  memset( (void*)&DataHandler_DH_to_OL_c, 0, sizeof(DataHandler_DH_to_OL_t) );
  memset( (void*)&DataHandler_PL_to_DH, 0, sizeof(DataHandler_PL_to_DH_t) );
  memset( (void*)&DataHandler_PL_to_DH_c, 0, sizeof(DataHandler_PL_to_DH_t) );

  //DataHandler_PL_to_DH.header_c = 147;
  //DataHandler_PL_to_DH.header_c = 147;

  InnerLoop_IL_to_PL.header = 147;

 }

///////////////////////// TASKS /////////////////////////


static InnerLoop_100000008_context InnerLoop_ctxt;

static void InnerLoop (void *parameters)
{
   task_entry * te = (task_entry *) parameters;

   InnerLoop_100000008_init( &InnerLoop_ctxt );

   for(;;)
   {
      xSemaphoreTake( te->semaphore, portMAX_DELAY );                  

                  DDRC|=0x80;
		  PORTC|=0x80;
      InnerLoop_100000008_main(
       &InnerLoop_ctxt,
       OuterLoop_OL_to_IL.InnerLoop_angle,
       &InnerLoop_IL_to_PL.InnerLoop_Torque
      );

             PORTC&=~0x80;
   }

}

static DataHandler_context DataHandler_ctxt;

static void DataHandler (void *parameters)
{
   task_entry * te = (task_entry *) parameters;

   DataHandler_init( &DataHandler_ctxt );

   for(;;)
   {
      xSemaphoreTake( te->semaphore, portMAX_DELAY );


                  DDRC|=0x40;
		  PORTC|=0x40;
      DataHandler_main(
       &DataHandler_ctxt,
       DataHandler_PL_to_DH.DataHandler_SerialIn,
       &DataHandler_DH_to_OL.DataHandler_MsgOut
      );
             PORTC&=~0x40;
   }
}


///////////////////////// SCHEDULE /////////////////////////



portTickType hp_len = 20;

task_entry tasks[] = {

   { DataHandler, "DataHandler", 5, 0},

   { InnerLoop, "InnerLoop", 10, 0},

   {NULL, NULL, 0, 0}
};

msg_entry msgs[] = {
   { 1, MSG_DIR_SEND, sizeof( DataHandler_DH_to_OL ), (portCHAR *) & DataHandler_DH_to_OL, (portCHAR *) & DataHandler_DH_to_OL_c, 6, pdFALSE},

   { 2, MSG_DIR_RECV, sizeof( OuterLoop_OL_to_IL ), (portCHAR *) & OuterLoop_OL_to_IL, (portCHAR *) & OuterLoop_OL_to_IL_c, 12, pdFALSE},

   { -1, 0, 0, NULL, NULL, 0, 0}
};

per_entry pers[] = {

   { 1, "UART", IN, 1, 0, sizeof( DataHandler_PL_to_DH ), (portCHAR *) & DataHandler_PL_to_DH, (portCHAR *) & DataHandler_PL_to_DH_c, 3, NULL},

   { 2, "UART", OUT, 1, 0, sizeof( InnerLoop_IL_to_PL ), (portCHAR *) & InnerLoop_IL_to_PL, (portCHAR *) & InnerLoop_IL_to_PL_c, 15, NULL},

   { -1, NULL, 0, 0, 0, 0, NULL, NULL, 0, NULL }
};

