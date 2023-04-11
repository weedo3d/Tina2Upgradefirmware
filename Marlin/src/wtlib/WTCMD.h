/**
* Copyright (C) 2023 WEEDO3D Perron
*/

#ifndef WTCMD_H
#define WTCMD_H

#include "stdint.h"

#define SPARK_IDLE		0
#define SPARK_PAUSED	1
#define SPARK_PRINTING	2
#define SPARK_LOST		3

#define HOST_WIIBUILDER	0
#define HOST_SPARK		1

enum WT_STATUS : char {
	IDLE = 0, 
	PRINTING = 1,
	PAUSE = 2,
	ABORT = 3,
	FINISH = 4
};

enum WT_MAIN_ACTION : char
{
	WMA_IDLE = 0,
	WMA_PAUSE = 1,
	WMA_RESUME = 2
};

extern WT_STATUS wt_machineStatus;

extern WT_MAIN_ACTION wt_mainloop_action;

extern uint8_t wtvar_hostmode;

void WTCMD_Process();

// 向Spark发送暂停打印请求
void wt_online_pause();

// 向Spark发送恢复打印请求
void wt_online_resume();

// 向Spark发送取消打印请求
void wt_online_cancel();

// 读取主机模式
uint8_t wt_get_hostmode(void);

// 保存主机模式
void wt_set_hostmode(const uint8_t mode);

// restart
void wt_restart();

// get machine status
void GetMachineStatus();

// send host action start message
void host_action_start();

// send host action finish message
void host_action_finish();

// main loop action
void wt_loopaction(void);

// got bootload
void wt_bootload();

void wt_powerloss_set(uint8_t state);

#endif

