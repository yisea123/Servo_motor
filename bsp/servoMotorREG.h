#ifndef __SERVOMOTORREG_H
#define __SERVOMOTORREG_H	 

#define PN000 0 //功能选择开关0
#define PN001 1 //功能选择开关1
#define PN002 2 //功能选择开关2
#define PN003 3 //功能选择开关3
#define PN004 4 //功能选择开关4
#define PN005 5 //电机型号
#define PN006 6 //功能选择开关5
#define PN007 7 //功能选择开关6
#define PN008 8 //功能选择开关7

#define PN100 100 //速度环增益
#define PN101 101 //速度环积分时间
#define PN102 102 //位置环增益
#define PN103 103 //负载惯量百分比
#define PN104 104 //第二速度环增益
#define PN105 105 //第二速度环积分时间
#define PN106 106 //第二位置环增益
#define PN107 107 //实时自动调谐设置
#define PN108 108 //负载刚性设定
#define PN109 109 //前馈
#define PN110 110 //前馈滤波
#define PN111 111 //扭矩前馈
#define PN112 112 //扭矩前馈滤波
#define PN113 113 //速度偏置
#define PN114 114 //扭矩指令滤波器常数
#define PN115 115 //第二扭矩指令滤波器常数
#define PN116 116 //P/PI切换条件
#define PN117 117 //扭矩切换阀值
#define PN118 118 //偏差计数器切换阀值
#define PN119 119 //给定加速度切换阀值
#define PN120 120 //给定速度切换阀值
#define PN121 121 //增益切换条件
#define PN122 122 //切换延迟时间
#define PN123 123 //切换门槛水平
#define PN124 124 //实际速度阀值
#define PN125 125 //位置增益切换时间
#define PN126 126 //切换滞环
#define PN127 127 //低速测速滤波
#define PN128 128 //实时调谐时速度增益增加关系
#define PN129 129 //低速校正系数
#define PN130 130 //摩擦负载
#define PN131 131 //摩擦补偿速度滞环区
#define PN132 132 //粘滞摩擦负载

//位置模式
#define PN200 200 //位置控制形态选择开关
#define PN201 201 //16位第一电子齿轮分子
#define PN202 202 //16位电子齿轮分母
#define PN203 203 //16位第二电子齿轮分子
#define PN204 204 //位置指令滤波时间常数
#define PN205 205 //位置指令滤波形式选择
#define PN206 206 //PG分频

//速度模式
#define PN300 300 //速度指令输入增益
#define PN301 301 //内部速度1
#define PN302 302 //内部速度2
#define PN303 303 //内部速度3
#define PN304 304 //JOG速度
#define PN305 305 //软启动加速时间
#define PN306 306 //软启动减速时间
#define PN307 307 //速度滤波时间常数
#define PN308 308 //S曲线上升时间
#define PN309 309 //速度指令曲线形式
#define PN310 310 //S形状选择
#define PN311 311 //模拟速度给定零点偏置
#define PN312 312 //参数速度
#define PN313 313 //通讯点动速度
#define PN319 319 //内部速度4
#define PN320 320 //内部速度5
#define PN321 321 //内部速度6
#define PN322 322 //内部速度7

//转矩模式
#define PN400 400 //扭矩指令增益
#define PN402 402 //正转扭矩内部限制
#define PN403 403 //反转扭矩内部限制
#define PN404 404 //正转外部扭矩限制
#define PN405 405 //反转外部扭矩限制
#define PN406 406 //反接制动扭矩限制
#define PN407 407 //扭矩控制时的速度限制
#define PN408 408 //陷波器1频率
#define PN409 409 //陷波器1深度
#define PN410 410 //陷波器2频率
#define PN411 411 //陷波器2深度
#define PN412 412 //低频抖动频率
#define PN413 413 //低频抖动阻尼
#define PN414 414 //转矩控制延迟时间
#define PN415 415 //转矩控制速度滞环
#define PN416 416 //模拟扭矩给定零点偏置

#define PN500 500 //定位误差
#define PN501 501 //同速误差
#define PN502 502 //零钳位转速
#define PN503 503 //旋转检测转速TGON
#define PN504 504 //偏差计数器溢出报警
#define PN505 505 //伺服ON等待时间
#define PN506 506 //基本的等待流程
#define PN507 507 //制动等待速度
#define PN508 508 //制动等待时间
#define PN509 509 //将输入信号分配到端口
#define PN510 510 //将输入信号分配到端口
#define PN511 511 //将输出信号分配到端口
#define PN512 512 //总线控制输入接点低位使能
#define PN513 513 //总线控制输入接点高位使能
#define PN514 514 //输入端口滤波
#define PN515 515 //报警端口滤波
#define PN516 516 //输入端口信号取反
#define PN517 517 //输入端口信号取反
#define PN518 518 //动态制动时间
#define PN519 519 //串行编码器错误允许时间
#define PN520 520 //到位时间
#define PN525 525 //过载报警阀值
#define PN528 528 //输出端口信号取反
#define PN529 529 //转矩检测输出信号阀值
#define PN530 530 //转矩检测输出信号时间

#define PN600 600 //JPOS0点位控制脉冲高位，单位10000P，范围-9999~9999
#define PN601 601 //JPOS0点位控制脉冲低位,单位1P，范围-9999~9999
//602~631 JPOS1 ~ JPOS15
#define PN632 632 //JPOS0点位控制速度，单位rpm，范围0~4500
//633~647 JPOS1 ~ JPOS15
#define PN648 648 //JPOS0点位控制一次滤波时间，单位0.1ms，范围0~32767
//649~663 JPOS1 ~ JPOS15
#define PN664 664 //JPOS0点位控制停止时间，单位50ms，范围0~300
//665~679 JPOS1 ~ JPOS15
#define PN681 681 //16进制位参
#define PN682 682 //编程方式
#define PN683 683 //编程起始步
#define PN684 684 //编程终止步
#define PN685 685 //位置接点控制下为“搜索行程速度”位置回零控制下为“找参考点时的速度（撞原点信号ORG）”
#define PN686 686 //位置接点控制下为“离开行程开关速度”位置回零控制下为“找参考点时的速度（离开原点信号ORG）”
#define PN687 687 //示教位置脉冲高位
#define PN688 688 //示教位置脉冲低位
#define PN689 689 //回零模式设定
#define PN690 690 //原点回归偏移脉冲数高位
#define PN691 691 //原点回归偏移脉冲数低位

#define PN700 700 //16进制位参
#define PN701 701 //MODBUS轴地址
#define PN703 703 //CAN通讯速率
#define PN704 704 //CAN通讯节点
#define PN705 705 //32位第一电子齿轮分子（H）
#define PN706 706 //32位第一电子齿轮分子（L）
#define PN707 707 //32位电子齿轮分母（H）
#define PN708 708 //32位电子齿轮分母（L）
#define PN709 709 //32位第二电子齿轮分子（H）
#define PN710 710 //32位第二电子齿轮分子（L）

#define ALARM0 0X07F1 //ALARM0-最近一次报警
//0X07F1~0X07FA 10个历史报警，只读

#define S_ZERO_OFFSET 0X07FB //速度指令零点偏移量，可读可写
#define T_ZERO_OFFSET 0X07FC //扭矩指令零点偏移量，可读可写
#define LU_ZERO_OFFSET 0X07FD //lu零点偏移量，只读
#define LV_ZERO_OFFSET 0X07FE //lv零点偏移量，只读

//UN为数据监视，全部只读
#define UN000 0X0806 //电机的实际转速，rpm
#define UN001 0X0807 //输入的速度指令值，rpm
#define UN002 0X0808 //输入的扭矩指令百分比（相对额定扭矩）
#define UN003 0X0809 //内部扭矩的百分比（相对额定扭矩）
#define UN004 0X080A //编码器旋转脉冲数
#define UN005 0X080B //输入信号状态
#define UN006 0X080C //编码器信号状态
#define UN007 0X080D //输出信号状态
#define UN008 0X080E //脉冲给定频率
#define UN009 0X080F //当前位置低位，单位：1P
#define UN010 0X0810 //当前位置高位，单位：10000P
#define UN011 0X0811 //偏差脉冲计数器低16位
#define UN012 0X0812 //偏差脉冲计数器高16位
#define UN013 0X0813 //给定脉冲计数器低位，单位：1P
#define UN014 0X0814 //给定脉冲计数器高位，单位：10000P
#define UN015 0X0815 //负载惯量百分比
#define UN016 0X0816 //电机过载比列
#define UN017 0X0817 //当前报警
#define UN018 0X0818 //母线电压，V
#define UN019 0X0819 //驱动器温度
#define UN020 0X0820 //电机电流，A

#define ModBUS_IO 0X0900 //ModBus通讯IO信号，掉电不保存，可读可写
#define DSP_VERSION 0X090E //DSP软件版本，只读
#define FPGA_VERSION 0X090F //FPGA软件版本，只读

#define ENCODER_17BIT_MULTI 0X1010 //17位编码器多圈信息，单位：1圈
#define ENCODER_17BIT_SINGLE_LOW 0X1011 //17位编码器单圈信息低位，单位：1P
#define ENCODER_17BIT_SINGLE_HIGH 0X1012 //17位编码器单圈信息高位，单位：1P
//只读，限17位编码器，其中多圈数据16bit，单圈数据17bit

#define CLEAR_HISTORY_ALARM 0X1021 //清除历史报警，01：清除，可写
#define CLEAR_PRESNET_ALARM 0X1022 //清除当前报警，01：清除，可写
#define JOG_ENABLE 0X1023 //JOG伺服使能，01，使能，00不使能，可写
#define JOG_FOREWARD 0X1024 //JOG正转，01正转，00停止，可写
#define JOG_INVERSION 0X1025 //JOG反转，01反转，00停止，可写
#define JPO_JOG_FOREWARD 0X1026 //接点位置下JOG正转，01正转，00停止，可写
#define JPO_JOG_INVERSION 0X1027 //接点位置下JOG反转，01反转，00停止，可写
#define JPO_JOG_STOP 0X1028 //接点位置下暂停，01暂停，00取消暂停，可写

#define CLEAR_ENCODER_17BIT_ALARM 0X1040 //清除编码器报警，01：清除，只写
#define CLEAR_ENCODER_17BIT_MULTI 0X1041 //清除编码器多圈数据，01：清除，只写

#define P_TEACHING_ENABLE 0X1070 //位置示教功能，01：启动，只写

#endif

