#include "servoMotor.h"

unsigned int crc_gen(unsigned char *data,unsigned char length){
	int j;
	unsigned int crc_reg = 0xffff;
	while(length--){
		crc_reg ^= *data++;
		for(j=0;j<8;j++){
			if(crc_reg & 0x01){
				crc_reg = (crc_reg >> 1) ^ 0xa001;
			}else{
				crc_reg = crc_reg >> 1;
			}
		}
	}
	return crc_reg;
}

unsigned char crc_chk(unsigned char *data,unsigned char length){
	unsigned int crc_gen_result;
	unsigned int data_crc_result = *(data+length-2) + *(data+length-1)*0xff;
	crc_gen_result = crc_gen(data,length-2);
	if(data_crc_result == crc_gen_result){
		return 1;
	}else{
		return 0;
	}
}

unsigned char servo_transmission(unsigned char *cmd,unsigned int cmd_length,unsigned int *result){
	unsigned char status = 1; //未收到消息
	uart_send_msg_t *uart_send_msg;
	osEvent evt;
	uart_send_msg = (uart_send_msg_t*)osPoolAlloc(uart1_send_mpool);							
	uart_send_msg->Datanum = cmd_length;
	uart_send_msg->Datas = cmd;
	osMessagePut(uart1_send_queue, (uint32_t)uart_send_msg, osWaitForever);			
	osSignalWait(UART_SEND_FINISH_SIGNAL,osWaitForever);	
	evt = osMessageGet(uart1_receive_queue, 500);					
	if (evt.status == osEventMessage) {											
		uart_receive_msg_t *uart_receive_msg = (uart_receive_msg_t*)evt.value.p;
		if(uart_receive_msg->Datanum > 2){
			if(crc_chk(uart_receive_msg->Datas,uart_receive_msg->Datanum)){
				switch(uart_receive_msg->Datas[1]){
					case 0x03: //读成功
						status = 0;
						*result = uart_receive_msg->Datas[3] + uart_receive_msg->Datas[4]*0xff; //读取的一个字
						break;
					case 0x06: //写成功
						status = 0;
						*result = 0;
						break;
					case 0x83: //读错误
						status = 3;
						*result = uart_receive_msg->Datas[2]; //返回错误码
						break;
					case 0x86: //写错误
						status = 4;
						*result = uart_receive_msg->Datas[2]; //返回错误码
						break;
				}
			}else{
				status = 2; //crc错误
			}
		}
		osPoolFree(uart1_receive_mpool, uart_receive_msg);	//Release the buffer
	}
	return status;
}

unsigned int read_word(unsigned char ctr_addr,unsigned int reg_addr){
	unsigned char temp[8];
	unsigned int crc_result;
	unsigned int result;
	unsigned char status;
	temp[0] = ctr_addr;  //ADR
	temp[1] = 0x03;      //CMD
	temp[2] = (unsigned char)(reg_addr >> 8);
	temp[3] = (unsigned char)reg_addr;
	temp[4] = 0x00;
	temp[5] = 0x01;
	crc_result = crc_gen(temp,6);
	temp[6] = (unsigned char)crc_result;
	temp[7] = (unsigned char)(crc_result >> 8);
	status = servo_transmission(temp,8,&result);
	if(status == 0){
		return result;
	}else{
		//这里可以添加对status、result的处理程序
		return 0xffff;
	}
}

unsigned char write_word(unsigned char ctr_addr,unsigned int reg_addr,unsigned int reg_val){
	unsigned char temp[8];
	unsigned int crc_result;
	unsigned int result;
	unsigned char status;
	temp[0] = ctr_addr;  //ADR
	temp[1] = 0x06;      //CMD
	temp[2] = (unsigned char)(reg_addr >> 8);
	temp[3] = (unsigned char)reg_addr;
	temp[4] = (unsigned char)(reg_val >> 8);
	temp[5] = (unsigned char)reg_val;
	crc_result = crc_gen(temp,6);
	temp[6] = (unsigned char)crc_result;
	temp[7] = (unsigned char)(crc_result >> 8);
	status = servo_transmission(temp,8,&result);
	//这里可以添加对status、result的处理程序
	return status;
}

unsigned char clear_encoder_alarm(unsigned char ctr_addr){
	return write_word(ctr_addr,CLEAR_ENCODER_17BIT_ALARM,1);
}

unsigned char jog_enable(unsigned char ctr_addr){
	return write_word(ctr_addr,JOG_ENABLE,1);
}

unsigned char jog_disable(unsigned char ctr_addr){
	return write_word(ctr_addr,JOG_ENABLE,0);
}

unsigned char jog_forward_run(unsigned char ctr_addr){
	return write_word(ctr_addr,JOG_FOREWARD,1);
}

unsigned char jog_forward_stop(unsigned char ctr_addr){
	return write_word(ctr_addr,JOG_FOREWARD,0);
}

unsigned char jog_inversion_run(unsigned char ctr_addr){
	return write_word(ctr_addr,JOG_INVERSION,1);
}

unsigned char jog_inversion_stop(unsigned char ctr_addr){
	return write_word(ctr_addr,JOG_INVERSION,0);
}

unsigned char jog_speed_set(unsigned char ctr_addr,unsigned int speed){
	return write_word(ctr_addr,PN304,speed); //speed 0~4500 rpm
}

unsigned char jog_run_acceleration_time_set(unsigned char ctr_addr,unsigned int time){
	return write_word(ctr_addr,PN305,time); //time:ms
}

unsigned char jog_stop_acceleration_time_set(unsigned char ctr_addr,unsigned int time){
	return write_word(ctr_addr,PN306,time); //time:ms
}




