#ifndef __SERVOMOTOR_H
#define __SERVOMOTOR_H	 

#include "servoMotorREG.h"
#include "cmsis_os.h"  
#include "uartthread.h"  

typedef enum servoStatus{
	OK, //״̬����
	PARAM_ERR, //��������
	CRC_CHECK_ERR, //CRCУ��ʧ�ܣ�֡����
	TRANSMISSION_ERR, //�������δ�յ��ظ���Ϣ��
	REG_WRITE_ERR, //�Ĵ���д����
	REG_READ_ERR, //�Ĵ���������
	SER_NOT_READY, //�ŷ���δ����
	SER_NOT_TGON, //�ŷ������δ��ת������ֹͣ״̬
	SER_GOT_ALARM, //�ŷ�������г��ִ���
	SER_NOT_COIN, //�ŷ���λδ���
}servoStatus;

//******************************************************************//
//������
servoStatus clear_encoder_alarm(uint8_t ctr_addr);
servoStatus clear_encoder_multi(uint8_t ctr_addr);
uint16_t get_current_position_high(uint8_t ctr_addr);
uint16_t get_current_position_low(uint8_t ctr_addr);
servoStatus enable_position_teaching(uint8_t ctr_addr);
servoStatus disable_position_teaching(uint8_t ctr_addr);
servoStatus position_teaching_set_high(uint8_t ctr_addr,uint16_t highbytes);
servoStatus position_teaching_set_low(uint8_t ctr_addr,uint16_t lowbytes);
//******************************************************************//
//�����ŷ���Ϊλ������ģʽ(�ӵ�ָ��) 
servoStatus set_servo_pmode(uint8_t ctr_addr);
servoStatus set_servo_forward_run(uint8_t ctr_addr);
servoStatus set_servo_inversion_run(uint8_t ctr_addr);
//******************************************************************//
//����������IO����
servoStatus servo_on(uint8_t ctr_addr);
servoStatus servo_off(uint8_t ctr_addr);
servoStatus alarm_reset(uint8_t ctr_addr);
servoStatus clear_remain_plus(uint8_t ctr_addr);
servoStatus get_servo_srdy(uint8_t ctr_addr);
servoStatus get_servo_tgon(uint8_t ctr_addr);
servoStatus get_servo_alm(uint8_t ctr_addr);
servoStatus get_servo_coin(uint8_t ctr_addr);
//******************************************************************//
//������Ϣ
uint16_t get_history_alarm_info(uint8_t ctr_addr,uint8_t info_index);
uint16_t get_current_alarm_info(uint8_t ctr_addr);

//******************************************************************//
//λ�ýӵ����
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
//JOG������
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
//��ʼ��
servoStatus servoMototInit(uint8_t ctr_addr);
//******************************************************************//
#endif
