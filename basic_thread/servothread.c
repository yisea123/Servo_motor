#include "servothread.h"   
 
void servo_thread(void const *argument);                             // thread function

osThreadId servo_thread_id;                                          // thread id
osThreadDef (servo_thread, osPriorityNormal, 1, 512);                   // thread object

int Init_servo_thread() {

  servo_thread_id = osThreadCreate (osThread(servo_thread), NULL);
  if (!servo_thread_id) return(-1);
  
  return(0);
}

void servo_thread(void const *argument) {
	servoStatus status;
	uart1_current_send_thread_id = servo_thread_id;
	//status = servo_off(1);
	status = servoMotorInit(1);
	status = setJPO(1);
	//jog_disable(1);	
	//jog_enable(1);
	//jog_forward_run(1);
	
  while(1) 
	{

  }
}

