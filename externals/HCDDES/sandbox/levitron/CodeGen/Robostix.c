//
//	Generated task code (FRODO Code Generator). Do not edit!
//	Date: 2008.10.14 - 09:58:40
//

///////////////////////// MESSAGES /////////////////////////

tpedef struct tagNavMessage {
	float RefPosition;
} NavMessage_t;

NavMessage_t NavMessage, NavMessage_c;

tpedef struct tagPosMessage {
	float Position;
} PosMessage_t;

PosMessage_t PosMessage, PosMessage_c;

tpedef struct tagForceMessage {
	float Force;
} ForceMessage_t;

ForceMessage_t ForceMessage, ForceMessage_c;

///////////////////////// TASKS /////////////////////////

static PD Controller_100000004_context PD Controller_100000004_ctx;

static void StabTask(void *parameters)
{
	task_entry* te = ( task_entry* ) parameters;

	PD Controller_100000004_init(&PD Controller_100000004_data);

	for( ;; )
	{ 
		xSemaphoreTake(  te->semaphore, portMAX_DELAY ); 
		PD Controller_100000004_main(&PD Controller_100000004_ctx,
				 NavMessage.RefPosition,
				 PosMessage.Position,
				 &ForceMessage.Force);
	}
}


///////////////////////// SCHEDULE /////////////////////////

static portTickType hp_len =0.02;

static task_entry tasks[] = {
	{StabTask, "StabTask", 5, 0},
	{StabTask, "StabTask", 25, 0},
	{StabTask, "StabTask", 45, 0},
	{StabTask, "StabTask", 65, 0},
	{StabTask, "StabTask", 85, 0},
	{StabTask, "StabTask", 105, 0},
	{StabTask, "StabTask", 125, 0},
	{StabTask, "StabTask", 145, 0},
	{StabTask, "StabTask", 165, 0},
	{StabTask, "StabTask", 185, 0},
	{NULL, NULL, 0, 0},
};

static msg_entry msgs[] = {
	{0, MSG_DIR_RECV, sizeof(NavMessage), (portCHAR*)&NavMessage, (portCHAR*)&NavMessage_c, 99, pdFALSE},
	{-1, 0, 0, NULL, NULL, 0, 0},
};


