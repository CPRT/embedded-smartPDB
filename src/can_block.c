/*
 * can_block.c
 *
 *  Created on: Oct 19, 2025
 *      Author: Jonathon Walker
 *
 */


#include "can_block.h"


static CAN_TxHeaderTypeDef TxHeader = {.ExtId = 0, .IDE = CAN_ID_STD, .RTR = CAN_RTR_DATA, .TransmitGlobalTime = DISABLE};
static ctx_t * ctx;       // declare context structure for non static variables



void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan) //RX interrupt ** CHANGE to return data format for daughter buffer**
{
	HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0,&(ctx->RxHeader),&(ctx->RxData));   //Retrieve message from RX que (FIFO0)
                                                                                //verify/test fourth argument

    /*
    format data for daughter to be returned by function
    */

}

void can_send(uint32_t msg_id, uint8_t* data, size_t length)
{
	CAN_TxHeaderTypeDef TxConfig;      // use to assign local TxHeader configuration
	TxConfig = TxHeader;

	//if (length <determine length for CAN frame> )
	
		TxConfig.StdId = msg_id;
		TxConfig.DLC = length;
		HAL_CAN_AddTxMessage(&(ctx->hcan),&TxConfig,data,&(ctx->TxMailbox));
	return;
}

void can_init(){
	ctx = setupctx();
	HAL_CAN_Start(&(ctx->hcan));
	HAL_CAN_ActivateNotification(&(ctx->hcan),CAN_IT_RX_FIFO0_MSG_PENDING);
	return;
}

void can_denit(){
	if (ctx != NULL){
		free(ctx);
		ctx = NULL;
	}
	return;
}

ctx_t* setupctx(){
	ctx_t* p = malloc(sizeof(ctx_t));
	assert(p!=NULL);
	return p;
}




