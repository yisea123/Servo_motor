#ifndef __SERVOMOTOR_H
#define __SERVOMOTOR_H	 

#include "servoMotorREG.h"
#include "cmsis_os.h"  
#include "uartthread.h"  

typedef enum servoStatus{
	OK, //状态正常
	PARAM_ERR, //参数错误
	CRC_CHECK_ERR, //CRC校验失败（帧错误）
	TRANSMISSION_ERR, //传输错误（未收到回复消息）
	REG_WRITE_ERR, //寄存器写错误
	REG_READ_ERR, //寄存器读错误
	SER_NOT_READY, //伺服还未就绪
	SER_NOT_TGON, //伺服电机还未旋转，处于停止状态
	SER_GOT_ALARM, //伺服电机运行出现错误
	SER_NOT_COIN, //伺服定位未完成
}servoStatus;

//******************************************************************//
//编码器
servoStatus clear_encoder_alarm(uint8_t ctr_addr);
servoStatus clear_encoder_multi(uint8_t ctr_addr);
uint16_t get_current_position_high(uint8_t ctr_addr);
uint16_t get_current_position_low(uint8_t ctr_addr);
servoStatus enable_position_teaching(uint8_t ctr_addr);
servoStatus disable_position_teaching(uint8_t ctr_addr);
servoStatus position_teaching_set_high(uint8_t ctr_addr,uint16_t highbytes);
servoStatus position_teaching_set_low(uint8_t ctr_addr,uint16_t lowbytes);
//******************************************************************//
//配置伺服器为位置运行模式(接点指令) 
servoStatus set_servo_pmode(uint8_t ctr_addr);
servoStatus set_servo_forward_run(uint8_t ctr_addr);
servoStatus set_servo_inversion_run(uint8_t ctr_addr);
//******************************************************************//
//控制器输入IO操作
servoStatus servo_on(uint8_t ctr_addr);
servoStatus servo_off(uint8_t ctr_addr);
servoStatus alarm_reset(uint8_t ctr_addr);
servoStatus clear_remain_plus(uint8_t ctr_addr);
servoStatus get_servo_srdy(uint8_t ctr_addr);
servoStatus get_servo_tgon(uint8_t ctr_addr);
servoStatus get_servo_alm(uint8_t ctr_addr);
servoStatus get_servo_coin(uint8_t ctr_addr);
//******************************************************************//
//报警信息
uint16_t get_history_alarm_info(uint8_t ctr_addr,uint8_t info_index);
uint16_t get_current_alarm_info(uint8_t ctr_addr);

//******************************************************************//
//位置接点控制
servoStatus initJPO(uint8_t ctr_addr);
servoStatus setJPO_increment_mode(uint8_t ctr_addr);
servoStatus setJPO_absolute_mode(uint8_t ctr_addr);
servoStatus setJPO_pluse_num(uint8_t ctr_addr,uint8_t jpo_index,uint16_t pluse_num_high,uint16_t pluse_num_low);
servoStatus setJPO_speed(uint8_t ctr_addr,uint8_t jpo_index,uint16_t speed);
servoStatus setJPO_filter_time(uint8_t ctr_addr,uint8_t jpo_index,uint16_t time);
servoStatus setJPO_stop_time(uint8_t ctr_addr,uint8_t jpo_index,uint16_t time);
servoStatus setJPO_start_point(uint8_t ctr_addr,uint8_t jpo_index);
servoStatus setJPO_stop_point(uint8_t ctr_addr,uint8_t jpo_index);
//******************************************************************//
//JOG试运行
servoStatus jog_enable(uint8_t ctr_addr);
servoStatus jog_disable(uint8_t ctr_addr);
servoStatus jog_forward_run(uint8_t ctr_addr);
servoStatus jog_forward_stop(uint8_t ctr_addr);
servoStatus jog_inversion_run(uint8_t ctr_addr);
servoStatus jog_inversion_stop(uint8_t ctr_addr);
servoStatus jog_speed_set(uint8_t ctr_addr,uint16_t speed);
servoStatus jog_run_acceleration_time_set(uint8_t ctr_addr,uint16_t time);
servoStatus jog_stop_acceleration_time_set(uint8_t ctr_addr,uint16_t time);
//******************************************************************//
//初始化
servoStatus servoMototInit(uint8_t ctr_addr);
//******************************************************************//
#endif
