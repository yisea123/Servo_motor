#ifndef __SERVOMOTOR_H
#define __SERVOMOTOR_H	 

#include "servoMotorREG.h"
#include "cmsis_os.h"  
#include "uartthread.h"  
			   
unsigned char clear_encoder_alarm(unsigned char ctr_addr);
unsigned char jog_enable(unsigned char ctr_addr);
unsigned char jog_disable(unsigned char ctr_addr);
unsigned char jog_forward_run(unsigned char ctr_addr);
unsigned char jog_forward_stop(unsigned char ctr_addr);
unsigned char jog_inversion_run(unsigned char ctr_addr);
unsigned char jog_inversion_stop(unsigned char ctr_addr);
unsigned char jog_speed_set(unsigned char ctr_addr,unsigned int speed);
unsigned char jog_run_acceleration_time_set(unsigned char ctr_addr,unsigned int time);
unsigned char jog_stop_acceleration_time_set(unsigned char ctr_addr,unsigned int time);
	
#endif
