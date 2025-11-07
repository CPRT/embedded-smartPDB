/*
 * can_block.h
 *
 *  Created on: Oct 19, 2025
 *      Author: Jonathon Walker
 */

#ifndef INC_CAN_BLOCK_H_
#define INC_CAN_BLOCK_H_
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <assert.h>
#include "stm32f1xx_hal.h"

typedef struct ctxStruct{
	CAN_HandleTypeDef hcan;
    CAN_RxHeaderTypeDef RxHeader;
	uint32_t TxMailbox;
	uint32_t RxMailbox;
	uint8_t RxID;
    uint8_t RxData;
}ctx_t;



void can_send(uint32_t msg_id, uint8_t* data, size_t length);

void can_init();
void can_denit();

ctx_t* setupctx();





#endif /* INC_CAN_BLOCK_H_ */
