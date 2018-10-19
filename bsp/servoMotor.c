#include "servoMotor.h"

//******************************************************************//
uint16_t crc_gen(uint8_t *data,uint8_t length){
	int j;
	uint16_t crc_reg = 0xffff;
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

servoStatus crc_chk(uint8_t *data,uint8_t length){
	uint16_t crc_gen_result;
	uint16_t data_crc_result = *(data+length-2) + *(data+length-1)*0xff;
	crc_gen_result = crc_gen(data,length-2);
	if(data_crc_result == crc_gen_result){
		return OK;
	}else{
		return CRC_CHECK_ERR;
	}
} 

servoStatus servo_transmission(uint8_t *cmd,uint16_t cmd_length,uint16_t *result){
	servoStatus status = TRANSMISSION_ERR; //δ�յ���Ϣ
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
					case 0x03: //���ɹ�
						status = OK;
						*result = uart_receive_msg->Datas[3] + uart_receive_msg->Datas[4]*0xff; //��ȡ��һ����
						break;
					case 0x06: //д�ɹ�
						status = OK;
						*result = 0;
						break;
					case 0x83: //������
						status = REG_READ_ERR;
						*result = uart_receive_msg->Datas[2]; //���ش�����
						break;
					case 0x86: //д����
						status = REG_WRITE_ERR;
						*result = uart_receive_msg->Datas[2]; //���ش�����
						break;
				}
			}else{
				status = CRC_CHECK_ERR; //crc����
			}
		}
		osPoolFree(uart1_receive_mpool, uart_receive_msg);	//Release the buffer
	}
	return status;
}

uint16_t read_word(uint8_t ctr_addr,uint16_t reg_addr){
	uint8_t temp[8];
	uint16_t crc_result;
	uint16_t result;
	servoStatus status;
	temp[0] = ctr_addr;  //ADR
	temp[1] = 0x03;      //CMD
	temp[2] = (uint8_t)(reg_addr >> 8);
	temp[3] = (uint8_t)reg_addr;
	temp[4] = 0x00;
	temp[5] = 0x01;
	crc_result = crc_gen(temp,6);
	temp[6] = (uint8_t)crc_result;
	temp[7] = (uint8_t)(crc_result >> 8);
	status = servo_transmission(temp,8,&result);
	if(status == OK){
		return result;
	}else{
		//���������Ӷ�status��result�Ĵ������
		return 0xffff;
	}
}

servoStatus write_word(uint8_t ctr_addr,uint16_t reg_addr,uint16_t reg_val){
	uint8_t temp[8];
	uint16_t crc_result;
	uint16_t result;
	servoStatus status;
	temp[0] = ctr_addr;  //ADR
	temp[1] = 0x06;      //CMD
	temp[2] = (uint8_t)(reg_addr >> 8);
	temp[3] = (uint8_t)reg_addr;
	temp[4] = (uint8_t)(reg_val >> 8);
	temp[5] = (uint8_t)reg_val;
	crc_result = crc_gen(temp,6);
	temp[6] = (uint8_t)crc_result;
	temp[7] = (uint8_t)(crc_result >> 8);
	status = servo_transmission(temp,8,&result);
	//���������Ӷ�status��result�Ĵ������
	return status;
}

//******************************************************************//
//������
//�������������
servoStatus clear_encoder_alarm(uint8_t ctr_addr){
	return write_word(ctr_addr,CLEAR_ENCODER_17BIT_ALARM,1);
}
//�����������Ȧ����
servoStatus clear_encoder_multi(uint8_t ctr_addr){
	return write_word(ctr_addr,CLEAR_ENCODER_17BIT_MULTI,1);
}
//��ȡ��ǰλ�ø��ֽ�,��λ10000P
uint16_t get_current_position_high(uint8_t ctr_addr){
	return read_word(ctr_addr,UN010);
}
//��ȡ��ǰλ�õ��ֽ�,��λ1P
uint16_t get_current_position_low(uint8_t ctr_addr){
	return read_word(ctr_addr,UN009);
}
//����λ��ʾ�̹���
servoStatus enable_position_teaching(uint8_t ctr_addr){
	return write_word(ctr_addr,P_TEACHING_ENABLE,1);
}
//�ر�λ��ʾ�̹���
servoStatus disable_position_teaching(uint8_t ctr_addr){
	return write_word(ctr_addr,P_TEACHING_ENABLE,0);
}
//����ʾ��λ�������λ����λ��10000P
servoStatus position_teaching_set_high(uint8_t ctr_addr,uint16_t highbytes){
	return write_word(ctr_addr,PN687,highbytes);
}
//����ʾ��λ�������λ����λ��1P
servoStatus position_teaching_set_low(uint8_t ctr_addr,uint16_t lowbytes){
	return write_word(ctr_addr,PN688,lowbytes);
}
//******************************************************************//
//�����ŷ���Ϊλ������ģʽ(�ӵ�ָ��) 
//P-CON������Ϊ����
//P-CL��N-CL����Ϊ�����ο��������
servoStatus set_servo_pmode(uint8_t ctr_addr){
	uint16_t temp = read_word(ctr_addr,PN000);
	if(temp != 0xffff){
		temp &= 0xff0f;
		return write_word(ctr_addr,PN000,temp |= 0X00C0);
	}else{
		return REG_READ_ERR;
	}
}
//��CCW����Ϊ��ת����
servoStatus set_servo_forward_run(uint8_t ctr_addr){
	uint16_t temp = read_word(ctr_addr,PN000);
	if(temp != 0xffff){
		temp &= 0xfff0;
		return write_word(ctr_addr,PN000,temp |= 0X0000);
	}else{
		return REG_READ_ERR;
	}
}
//��CW����Ϊ��ת����
servoStatus set_servo_inversion_run(uint8_t ctr_addr){
	uint16_t temp = read_word(ctr_addr,PN000);
	if(temp != 0xffff){
		temp &= 0xfff0;
		return write_word(ctr_addr,PN000,temp |= 0X0001);
	}else{
		return REG_READ_ERR;
	}
}

//******************************************************************//
//����������IO����
//�ŷ�ʹ��
servoStatus servo_on(uint8_t ctr_addr){
	return write_word(ctr_addr,PN512,0X01);
}
//�ŷ���ʹ��
servoStatus servo_off(uint8_t ctr_addr){
	return write_word(ctr_addr,PN512,0X00);
}
//�������ALMRST
servoStatus alarm_reset(uint8_t ctr_addr){
	servoStatus status;
	status = write_word(ctr_addr,PN513,0X01);
	if(status == OK){
		return write_word(ctr_addr,PN513,0X00);
	}else{
		return REG_WRITE_ERR;
	}
}
//λ��ƫ���������CLR
servoStatus clear_remain_plus(uint8_t ctr_addr){
	servoStatus status;
	status = write_word(ctr_addr,PN513,0X02);
	if(status == OK){
		return write_word(ctr_addr,PN513,0X00);
	}else{
		return REG_WRITE_ERR;
	}
}

//******************************************************************//
//Status
//�ŷ�����S-RDY
servoStatus get_servo_srdy(uint8_t ctr_addr){
	uint16_t status = read_word(ctr_addr,UN007);
	if(status != 0xffff){
		status &= 0x0004;
		if(status){
			return SER_NOT_READY; //OFF=H ��ƽ,�ŷ�δ׼������
		}else{
			return OK; //ON=L ��ƽ,�ŷ�׼������״̬
		}
	}else{
		return REG_READ_ERR;
	}
}
//�����ȶ�TGON
servoStatus get_servo_tgon(uint8_t ctr_addr){
	uint16_t status = read_word(ctr_addr,UN007);
	if(status != 0xffff){
		status &= 0x0001;
		if(status){
			return SER_NOT_TGON; //OFF=H ��ƽ,�ŷ��������ֹͣ״̬
		}else{
			return OK; //ON=L ��ƽ,�ŷ����������ת
		}
	}else{
		return REG_READ_ERR;
	}
}
//�������ALM
servoStatus get_servo_alm(uint8_t ctr_addr){
	uint16_t status = read_word(ctr_addr,UN007);
	if(status != 0xffff){
		status &= 0x0002;
		if(status){
			return SER_GOT_ALARM; //H ��ƽ������״̬
		}else{
			return OK; //L ��ƽ,����״̬
		}
	}else{
		return REG_READ_ERR;
	}
}
//��λ���COIN
servoStatus get_servo_coin(uint8_t ctr_addr){
	uint16_t status = read_word(ctr_addr,UN007);
	if(status != 0xffff){
		status &= 0x0008;
		if(status){
			return SER_NOT_COIN; // H ��ƽ����λδ���
		}else{
			return OK; //L ��ƽ,��λ���
		}
	}else{
		return REG_READ_ERR;
	}
}
//******************************************************************//
//������Ϣ
//��ȡ��ʷ������Ϣ
//info_index��0-9
//0�����һ�α�����9��ʱ�������Զ
uint16_t get_history_alarm_info(uint8_t ctr_addr,uint8_t info_index){
	if(info_index > 9) return 0xffff;
	return read_word(ctr_addr,ALARM0 + info_index);
}
//��ȡ���������Ϣ
uint16_t get_current_alarm_info(uint8_t ctr_addr){
	return get_history_alarm_info(ctr_addr,0);
}

//******************************************************************//
//λ�ýӵ����
//��ʼ������
//�������У�P-CL�����źţ�N-CL�����Ҳο���
//P-CON������Ҫ�����źţ����������źſ��������ر��ڲ����壬��������ʱ���ص�������ʼ�㹤����
//���������ź����巽ʽ
servoStatus initJPO(uint8_t ctr_addr){
	return write_word(ctr_addr,PN681,0x0131);
}
//���ñ�̷�ʽΪ����ģʽ
servoStatus setJPO_increment_mode(uint8_t ctr_addr){
	return write_word(ctr_addr,PN682,0);
}
//���ñ�̷�ʽΪ����ֵ���ģʽ
servoStatus setJPO_absolute_mode(uint8_t ctr_addr){
	return write_word(ctr_addr,PN682,1);
}
//����JPOSX��Ŀ���λ������
//jpo_index��0~15
//pluse_num_high����λ10000P
//pluse_num_low����λ1P
servoStatus setJPO_pluse_num(uint8_t ctr_addr,uint8_t jpo_index,
	uint16_t pluse_num_high,uint16_t pluse_num_low){
	if(jpo_index > 15) return PARAM_ERR;
	if(pluse_num_high > 9999 || pluse_num_low > 9999) return PARAM_ERR;
	if(write_word(ctr_addr,PN600 + jpo_index*2,pluse_num_high) != OK) return REG_WRITE_ERR;
	return write_word(ctr_addr,PN600 + jpo_index*2 + 1,pluse_num_low);
}
//����JPOSX����ٶ�
//jpo_index��0~15
//speed��0~4500 rpm
servoStatus setJPO_speed(uint8_t ctr_addr,uint8_t jpo_index,uint16_t speed){
	if(jpo_index > 15) return PARAM_ERR;
	if(speed > 4500) return PARAM_ERR;
	return write_word(ctr_addr,PN632 + jpo_index,speed);
}
//����JPOSX���һ���˲�ʱ��
//jpo_index��0~15
//time��0~32767 ��λ��0.1ms
servoStatus setJPO_filter_time(uint8_t ctr_addr,uint8_t jpo_index,uint16_t time){
	if(jpo_index > 15) return PARAM_ERR;
	return write_word(ctr_addr,PN648 + jpo_index,time);
}
//����JPOSX���ֹͣʱ��
//jpo_index��0~15
//time��0~300 ��λ��50ms
servoStatus setJPO_stop_time(uint8_t ctr_addr,uint8_t jpo_index,uint16_t time){
	if(jpo_index > 15) return PARAM_ERR;
	if(time > 300) return PARAM_ERR;
	return write_word(ctr_addr,PN664 + jpo_index,time);
}
//����JPO��λ������ʼ��
//jpo_index��0~15
servoStatus setJPO_start_point(uint8_t ctr_addr,uint8_t jpo_index){
	if(jpo_index > 15) return PARAM_ERR;
	return write_word(ctr_addr,PN683,jpo_index);
}

//����JPO��λ������ֹ��
//jpo_index��0~15
servoStatus setJPO_stop_point(uint8_t ctr_addr,uint8_t jpo_index){
	if(jpo_index > 15) return PARAM_ERR;
	return write_word(ctr_addr,PN684,jpo_index);
}

//******************************************************************//
//JOG������
//JOG �ŷ�ʹ��
servoStatus jog_enable(uint8_t ctr_addr){
	return write_word(ctr_addr,JOG_ENABLE,1);
}
//JOG �ŷ���ʹ��
servoStatus jog_disable(uint8_t ctr_addr){
	return write_word(ctr_addr,JOG_ENABLE,0);
}
//JOG ��ת
servoStatus jog_forward_run(uint8_t ctr_addr){
	return write_word(ctr_addr,JOG_FOREWARD,1);
}
//JOG ֹͣ��ת
servoStatus jog_forward_stop(uint8_t ctr_addr){
	return write_word(ctr_addr,JOG_FOREWARD,0);
}
//JOG ��ת
servoStatus jog_inversion_run(uint8_t ctr_addr){
	return write_word(ctr_addr,JOG_INVERSION,1);
}
//JOG ֹͣ��ת
servoStatus jog_inversion_stop(uint8_t ctr_addr){
	return write_word(ctr_addr,JOG_INVERSION,0);
}
//JOG�ٶ�
servoStatus jog_speed_set(uint8_t ctr_addr,uint16_t speed){
	if(speed > 4500) return PARAM_ERR;
	return write_word(ctr_addr,PN304,speed); 
}
//����������ʱ��
servoStatus jog_run_acceleration_time_set(uint8_t ctr_addr,uint16_t time){
	if(time > 10000) return PARAM_ERR;
	return write_word(ctr_addr,PN305,time); //time:ms
}
//����������ʱ��
servoStatus jog_stop_acceleration_time_set(uint8_t ctr_addr,uint16_t time){
	if(time > 10000) return PARAM_ERR;
	return write_word(ctr_addr,PN306,time); //time:ms
}



