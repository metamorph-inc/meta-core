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

    unsigned char header;
    MsingleC6C1M DataHandler_data2;

} DataHandler_PL_GPS_msg_t;
DataHandler_PL_GPS_msg_t DataHandler_PL_GPS_msg, DataHandler_PL_GPS_msg_c;

typedef struct {

    unsigned char header;
    MsingleC9C1M DataHandler_data1;

} DataHandler_PL_IMU_msg_t;
DataHandler_PL_IMU_msg_t DataHandler_PL_IMU_msg, DataHandler_PL_IMU_msg_c;

typedef struct {

    DataHandler_PL_IMU_msg_t imu_buf;
    DataHandler_PL_GPS_msg_t gps_buf;

} DataHandler_UART_bufs_t;
DataHandler_UART_bufs_t input_bufs;

/* typedef struct {

    MsingleC6C1M DataHandler_att_data;

    MsingleC6C1M InnerLoop_att_data;

} DataHandler_Att_Data_msg_t;
DataHandler_Att_Data_msg_t DataHandler_Att_Data_msg, DataHandler_Att_Data_msg_c;
*/
typedef struct {

    MsingleC4C1M InnerLoop_att_ref;

} OuterLoop_Att_Ref_msg_t;
OuterLoop_Att_Ref_msg_t OuterLoop_Att_Ref_msg, OuterLoop_Att_Ref_msg_c;

typedef struct {

    unsigned char header;
    MsingleC4C1M InnerLoop_Torque;

} InnerLoop_Thrust_Cmd_msg_t;
InnerLoop_Thrust_Cmd_msg_t InnerLoop_Thrust_Cmd_msg, InnerLoop_Thrust_Cmd_msg_c;


///////////////////////// C-CODE /////////////////////////

static DataHandler_context DataHandler_ctxt;
static InnerLoop_context InnerLoop_ctxt;

void app_tasks_init() { 

    memset( &DataHandler_Pos_Data_msg, 0, sizeof(DataHandler_Pos_Data_msg_t));
    memset( &DataHandler_Pos_Data_msg_c, 0, sizeof(DataHandler_Pos_Data_msg_t));
    memset( &DataHandler_Att_Data_msg, 0, sizeof(DataHandler_Att_Data_msg_t));
    memset( &DataHandler_Att_Data_msg_c, 0, sizeof(DataHandler_Att_Data_msg_t));
    memset( &DataHandler_PL_GPS_msg, 0, sizeof(DataHandler_PL_GPS_msg_t));
    memset( &DataHandler_PL_GPS_msg_c, 0, sizeof(DataHandler_PL_GPS_msg_t));
    memset( &DataHandler_PL_IMU_msg, 0, sizeof(DataHandler_PL_IMU_msg_t));
    memset( &DataHandler_PL_IMU_msg_c, 0, sizeof(DataHandler_PL_IMU_msg_t));
    memset( &OuterLoop_Att_Ref_msg, 0, sizeof(OuterLoop_Att_Ref_msg_t));
    memset( &OuterLoop_Att_Ref_msg_c, 0, sizeof(OuterLoop_Att_Ref_msg_t));
    memset( &InnerLoop_Thrust_Cmd_msg, 0, sizeof(InnerLoop_Thrust_Cmd_msg_t));
    memset( &InnerLoop_Thrust_Cmd_msg_c, 0, sizeof(InnerLoop_Thrust_Cmd_msg_t));
    memset( &input_bufs, 0, sizeof(DataHandler_UART_bufs_t));

    // Initialize the message to a recognizable state.
    InnerLoop_Thrust_Cmd_msg.header = 147;
    InnerLoop_Thrust_Cmd_msg.InnerLoop_Torque[0] = -1.1;
    InnerLoop_Thrust_Cmd_msg.InnerLoop_Torque[1] = -0.4;
    InnerLoop_Thrust_Cmd_msg.InnerLoop_Torque[2] = 0.55;
    InnerLoop_Thrust_Cmd_msg.InnerLoop_Torque[3] = 1.1;

    DataHandler_init( &DataHandler_ctxt );
    InnerLoop_init( &InnerLoop_ctxt );
}

///////////////////////// TASKS /////////////////////////



static void DataHandler (void *parameters)
{


   task_entry * te = (task_entry *) parameters;

   DataHandler_init( &DataHandler_ctxt );

   for(;;)
   {     
      xSemaphoreTake( te->semaphore, portMAX_DELAY );
//                  DDRC|=0x80;
//		  PORTC|=0x80;
      DataHandler_main(
       &DataHandler_ctxt,
       DataHandler_PL_IMU_msg.DataHandler_data1,
       DataHandler_PL_GPS_msg.DataHandler_data2,
       &(DataHandler_Att_Data_msg.DataHandler_att_data),
       &(DataHandler_Pos_Data_msg.DataHandler_pos_data)
      );
//             PORTC&=~0x80;
   }

}


static void InnerLoop (void *parameters)
{


   task_entry * te = (task_entry *) parameters;

   InnerLoop_init( &InnerLoop_ctxt );

   for(;;)
   {
      xSemaphoreTake( te->semaphore, portMAX_DELAY );
//                  DDRC|=0x80;
//		  PORTC|=0x80;

      InnerLoop_main(
       &InnerLoop_ctxt,
       OuterLoop_Att_Ref_msg.InnerLoop_att_ref,
       DataHandler_Att_Data_msg.DataHandler_att_data,
       &(InnerLoop_Thrust_Cmd_msg.InnerLoop_Torque)
      ); 


      /* Just copy through some of the input data */ 
      //InnerLoop_Thrust_Cmd_msg.InnerLoop_Torque[0] 
         // = DataHandler_PL_IMU_msg.DataHandler_data1[1];
         // = DataHandler_Att_Data_msg.DataHandler_att_data[0];
      //InnerLoop_Thrust_Cmd_msg.InnerLoop_Torque[1] 
         // = DataHandler_PL_IMU_msg.DataHandler_data1[3];
         // = DataHandler_Att_Data_msg.DataHandler_att_data[1];
      //InnerLoop_Thrust_Cmd_msg.InnerLoop_Torque[2]
         // = DataHandler_PL_GPS_msg.DataHandler_data2[2]; 
         // = DataHandler_Att_Data_msg.DataHandler_att_data[2];
      //InnerLoop_Thrust_Cmd_msg.InnerLoop_Torque[3] 
          //= DataHandler_PL_GPS_msg.DataHandler_data2[4]; 
         // = DataHandler_Att_Data_msg.DataHandler_att_data[3]; 
      
      // First test point -- after the data is received from the UART  
      /* memcpy( ((char *)&(InnerLoop_Thrust_Cmd_msg.InnerLoop_Torque)), ((char *)&(DataHandler_PL_IMU_msg.DataHandler_data1)), 8 );
      memcpy( ((char *)&(InnerLoop_Thrust_Cmd_msg.InnerLoop_Torque))+8, ((char *)&(DataHandler_PL_GPS_msg.DataHandler_data2)), 8 ); */;


      // Second test point -- after the data has been through the DataHandler
      /* memcpy( ((char *)&(InnerLoop_Thrust_Cmd_msg.InnerLoop_Torque)), ((char *)&(DataHandler_Att_Data_msg.DataHandler_att_data)), 8 );
      memcpy( ((char *)&(InnerLoop_Thrust_Cmd_msg.InnerLoop_Torque))+8, ((char *)&(DataHandler_Pos_Data_msg.DataHandler_pos_data)), 8 ); */

      // Third test point -- data returning from I2C
      //memcpy( ((char *)&(InnerLoop_Thrust_Cmd_msg.InnerLoop_Torque)), ((char *)&(DataHandler_Att_Data_msg.DataHandler_att_data)), 8 );
      //memcpy( ((char *)&(InnerLoop_Thrust_Cmd_msg.InnerLoop_Torque))+8, ((char *)&(OuterLoop_Att_Ref_msg.InnerLoop_att_ref)), 8 ); 

      // Fourth test point -- after the Inner loop
      // No intervening copy before transmitting...

      // All at once test:  one data value from each test point
      memcpy( ((char *)&(InnerLoop_Thrust_Cmd_msg.InnerLoop_Torque)), ((char *)&(DataHandler_PL_IMU_msg.DataHandler_data1)), 4 );
      memcpy( ((char *)&(InnerLoop_Thrust_Cmd_msg.InnerLoop_Torque))+4, ((char *)&(DataHandler_Att_Data_msg.DataHandler_att_data)), 4 );
      memcpy( ((char *)&(InnerLoop_Thrust_Cmd_msg.InnerLoop_Torque))+8, ((char *)&(OuterLoop_Att_Ref_msg.InnerLoop_att_ref)), 4 ); 
      

//             PORTC&=~0x80;
   }

}


///////////////////////// SCHEDULE /////////////////////////



portTickType hp_len = 20;

task_entry tasks[] = {

   { DataHandler, "DataHandler", 4, 0},
   { InnerLoop, "InnerLoop", 12, 0},
   {NULL, NULL, 0, 0}
}; 

msg_entry msgs[] = {

   { 1, MSG_DIR_SEND, sizeof( DataHandler_Pos_Data_msg ), (portCHAR *) & DataHandler_Pos_Data_msg, (portCHAR *) & DataHandler_Pos_Data_msg_c, 9, pdFALSE},
   { 2, MSG_DIR_RECV, sizeof( OuterLoop_Att_Ref_msg ), (portCHAR *) & OuterLoop_Att_Ref_msg, (portCHAR *) & OuterLoop_Att_Ref_msg_c, 14, pdFALSE},
   { -1, 0, 0, NULL, NULL, 0, 0}
};

per_entry pers[] = {

   /* This one should take 5.14 ms to transfer, and how long to copy? */
   { 1, "UART", IN, 1, 0, sizeof(DataHandler_PL_IMU_msg_t), (portCHAR *) &DataHandler_PL_IMU_msg, (portCHAR *) &(input_bufs.imu_buf), 2, NULL, NULL, 147},
   /* This one should take 3.48 ms to transfer */
   { 2, "UART", IN, 1, 0, sizeof( DataHandler_PL_GPS_msg_t ), (portCHAR *) &DataHandler_PL_GPS_msg, (portCHAR *) &(input_bufs.gps_buf), 3, NULL, NULL, 108},
   /* This one should take 2.37 ms to transfer */ 
   { 3, "UART", OUT, 1, 0, sizeof(InnerLoop_Thrust_Cmd_msg_t), (portCHAR *) &InnerLoop_Thrust_Cmd_msg, (portCHAR *) &InnerLoop_Thrust_Cmd_msg_c, 17, NULL, NULL, 147},
   { -1, NULL, 0, 0, 0, 0, NULL, NULL, 0, NULL, NULL, 0 }
};


