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
	servoStatus status = TRANSMISSION_ERR; //未收到消息
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
						status = OK;
						*result = uart_receive_msg->Datas[3] + uart_receive_msg->Datas[4]*0xff; //读取的一个字
						break;
					case 0x06: //写成功
						status = OK;
						*result = 0;
						break;
					case 0x83: //读错误
						status = REG_READ_ERR;
						*result = uart_receive_msg->Datas[2]; //返回错误码
						break;
					case 0x86: //写错误
						status = REG_WRITE_ERR;
						*result = uart_receive_msg->Datas[2]; //返回错误码
						break;
				}
			}else{
				status = CRC_CHECK_ERR; //crc错误
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
		//这里可以添加对status、result的处理程序
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
	//这里可以添加对status、result的处理程序
	return status;
}

//******************************************************************//
//编码器
//清除编码器报警
servoStatus clear_encoder_alarm(uint8_t ctr_addr){
	return write_word(ctr_addr,CLEAR_ENCODER_17BIT_ALARM,1);
}
//清除编码器多圈数据
servoStatus clear_encoder_multi(uint8_t ctr_addr){
	return write_word(ctr_addr,CLEAR_ENCODER_17BIT_MULTI,1);
}
//获取当前位置高字节,单位10000P
uint16_t get_current_position_high(uint8_t ctr_addr){
	return read_word(ctr_addr,UN010);
}
//获取当前位置低字节,单位1P
uint16_t get_current_position_low(uint8_t ctr_addr){
	return read_word(ctr_addr,UN009);
}
//启动位置示教功能
servoStatus enable_position_teaching(uint8_t ctr_addr){
	return write_word(ctr_addr,P_TEACHING_ENABLE,1);
}
//关闭位置示教功能
servoStatus disable_position_teaching(uint8_t ctr_addr){
	return write_word(ctr_addr,P_TEACHING_ENABLE,0);
}
//设置示教位置脉冲高位，单位：10000P
servoStatus position_teaching_set_high(uint8_t ctr_addr,uint16_t highbytes){
	return write_word(ctr_addr,PN687,highbytes);
}
//设置示教位置脉冲低位，单位：1P
servoStatus position_teaching_set_low(uint8_t ctr_addr,uint16_t lowbytes){
	return write_word(ctr_addr,PN688,lowbytes);
}
//******************************************************************//
//配置伺服器为位置运行模式(接点指令) 
//P-CON：可作为换步
//P-CL、N-CL可作为搜索参考点或启动
servoStatus set_servo_pmode(uint8_t ctr_addr){
	uint16_t temp = read_word(ctr_addr,PN000);
	if(temp != 0xffff){
		temp &= 0xff0f;
		return write_word(ctr_addr,PN000,temp |= 0X00C0);
	}else{
		return REG_READ_ERR;
	}
}
//以CCW方向为正转方向
servoStatus set_servo_forward_run(uint8_t ctr_addr){
	uint16_t temp = read_word(ctr_addr,PN000);
	if(temp != 0xffff){
		temp &= 0xfff0;
		return write_word(ctr_addr,PN000,temp |= 0X0000);
	}else{
		return REG_READ_ERR;
	}
}
//以CW方向为正转方向
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
//控制器输入IO操作
//伺服使能
servoStatus servo_on(uint8_t ctr_addr){
	return write_word(ctr_addr,PN512,0X01);
}
//伺服不使能
servoStatus servo_off(uint8_t ctr_addr){
	return write_word(ctr_addr,PN512,0X00);
}
//报警清除ALMRST
servoStatus alarm_reset(uint8_t ctr_addr){
	servoStatus status;
	status = write_word(ctr_addr,PN513,0X01);
	if(status == OK){
		return write_word(ctr_addr,PN513,0X00);
	}else{
		return REG_WRITE_ERR;
	}
}
//位置偏差脉冲清除CLR
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
//伺服就绪S-RDY
servoStatus get_servo_srdy(uint8_t ctr_addr){
	uint16_t status = read_word(ctr_addr,UN007);
	if(status != 0xffff){
		status &= 0x0004;
		if(status){
			return SER_NOT_READY; //OFF=H 电平,伺服未准备就绪
		}else{
			return OK; //ON=L 电平,伺服准备就绪状态
		}
	}else{
		return REG_READ_ERR;
	}
}
//运行稳定TGON
servoStatus get_servo_tgon(uint8_t ctr_addr){
	uint16_t status = read_word(ctr_addr,UN007);
	if(status != 0xffff){
		status &= 0x0001;
		if(status){
			return SER_NOT_TGON; //OFF=H 电平,伺服电机处于停止状态
		}else{
			return OK; //ON=L 电平,伺服电机正在旋转
		}
	}else{
		return REG_READ_ERR;
	}
}
//故障输出ALM
servoStatus get_servo_alm(uint8_t ctr_addr){
	uint16_t status = read_word(ctr_addr,UN007);
	if(status != 0xffff){
		status &= 0x0002;
		if(status){
			return SER_GOT_ALARM; //H 电平，报警状态
		}else{
			return OK; //L 电平,正常状态
		}
	}else{
		return REG_READ_ERR;
	}
}
//定位完成COIN
servoStatus get_servo_coin(uint8_t ctr_addr){
	uint16_t status = read_word(ctr_addr,UN007);
	if(status != 0xffff){
		status &= 0x0008;
		if(status){
			return SER_NOT_COIN; // H 电平，定位未完成
		}else{
			return OK; //L 电平,定位完成
		}
	}else{
		return REG_READ_ERR;
	}
}
//******************************************************************//
//报警信息
//获取历史报警信息
//info_index：0-9
//0：最近一次报警，9：时间相对最远
uint16_t get_history_alarm_info(uint8_t ctr_addr,uint8_t info_index){
	if(info_index > 9) return 0xffff;
	return read_word(ctr_addr,ALARM0 + info_index);
}
//获取最近报警信息
uint16_t get_current_alarm_info(uint8_t ctr_addr){
	return get_history_alarm_info(ctr_addr,0);
}

//******************************************************************//
//位置接点控制
//初始化配置
//单次运行，P-CL启动信号，N-CL反向找参考点
//P-CON换步，要启动信号，撤销启动信号可以立即关闭内部脉冲，在重启动时，回到程序起始点工作。
//换步输入信号脉冲方式
servoStatus initJPO(uint8_t ctr_addr){
	return write_word(ctr_addr,PN681,0x0131);
}
//设置编程方式为增量模式
servoStatus setJPO_increment_mode(uint8_t ctr_addr){
	return write_word(ctr_addr,PN682,0);
}
//设置编程方式为绝对值编程模式
servoStatus setJPO_absolute_mode(uint8_t ctr_addr){
	return write_word(ctr_addr,PN682,1);
}
//设置JPOSX点的控制位置脉冲
//jpo_index：0~15
//pluse_num_high：单位10000P
//pluse_num_low：单位1P
servoStatus setJPO_pluse_num(uint8_t ctr_addr,uint8_t jpo_index,
	uint16_t pluse_num_high,uint16_t pluse_num_low){
	if(jpo_index > 15) return PARAM_ERR;
	if(pluse_num_high > 9999 || pluse_num_low > 9999) return PARAM_ERR;
	if(write_word(ctr_addr,PN600 + jpo_index*2,pluse_num_high) != OK) return REG_WRITE_ERR;
	return write_word(ctr_addr,PN600 + jpo_index*2 + 1,pluse_num_low);
}
//设置JPOSX点的速度
//jpo_index：0~15
//speed：0~4500 rpm
servoStatus setJPO_speed(uint8_t ctr_addr,uint8_t jpo_index,uint16_t speed){
	if(jpo_index > 15) return PARAM_ERR;
	if(speed > 4500) return PARAM_ERR;
	return write_word(ctr_addr,PN632 + jpo_index,speed);
}
//设置JPOSX点的一次滤波时间
//jpo_index：0~15
//time：0~32767 单位：0.1ms
servoStatus setJPO_filter_time(uint8_t ctr_addr,uint8_t jpo_index,uint16_t time){
	if(jpo_index > 15) return PARAM_ERR;
	return write_word(ctr_addr,PN648 + jpo_index,time);
}
//设置JPOSX点的停止时间
//jpo_index：0~15
//time：0~300 单位：50ms
servoStatus setJPO_stop_time(uint8_t ctr_addr,uint8_t jpo_index,uint16_t time){
	if(jpo_index > 15) return PARAM_ERR;
	if(time > 300) return PARAM_ERR;
	return write_word(ctr_addr,PN664 + jpo_index,time);
}
//设置JPO点位控制起始点
//jpo_index：0~15
servoStatus setJPO_start_point(uint8_t ctr_addr,uint8_t jpo_index){
	if(jpo_index > 15) return PARAM_ERR;
	return write_word(ctr_addr,PN683,jpo_index);
}

//设置JPO点位控制终止点
//jpo_index：0~15
servoStatus setJPO_stop_point(uint8_t ctr_addr,uint8_t jpo_index){
	if(jpo_index > 15) return PARAM_ERR;
	return write_word(ctr_addr,PN684,jpo_index);
}

//******************************************************************//
//JOG试运行
//JOG 伺服使能
servoStatus jog_enable(uint8_t ctr_addr){
	return write_word(ctr_addr,JOG_ENABLE,1);
}
//JOG 伺服不使能
servoStatus jog_disable(uint8_t ctr_addr){
	return write_word(ctr_addr,JOG_ENABLE,0);
}
//JOG 正转
servoStatus jog_forward_run(uint8_t ctr_addr){
	return write_word(ctr_addr,JOG_FOREWARD,1);
}
//JOG 停止正转
servoStatus jog_forward_stop(uint8_t ctr_addr){
	return write_word(ctr_addr,JOG_FOREWARD,0);
}
//JOG 反转
servoStatus jog_inversion_run(uint8_t ctr_addr){
	return write_word(ctr_addr,JOG_INVERSION,1);
}
//JOG 停止反转
servoStatus jog_inversion_stop(uint8_t ctr_addr){
	return write_word(ctr_addr,JOG_INVERSION,0);
}
//JOG速度
servoStatus jog_speed_set(uint8_t ctr_addr,uint16_t speed){
	if(speed > 4500) return PARAM_ERR;
	return write_word(ctr_addr,PN304,speed); 
}
//软启动加速时间
servoStatus jog_run_acceleration_time_set(uint8_t ctr_addr,uint16_t time){
	if(time > 10000) return PARAM_ERR;
	return write_word(ctr_addr,PN305,time); //time:ms
}
//软启动减速时间
servoStatus jog_stop_acceleration_time_set(uint8_t ctr_addr,uint16_t time){
	if(time > 10000) return PARAM_ERR;
	return write_word(ctr_addr,PN306,time); //time:ms
}



