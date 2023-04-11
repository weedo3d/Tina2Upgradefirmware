/**
* Copyright (C) 2023 WEEDO3D Perron
*/

/**
* WEEDO 自用指令
* 主板发往打印机的指令，以W开头，后跟1至3位数字
*
* 联机打印相关指令
*
* W1 联机打印开始
* W2 联机打印结束
* W3 联机打印中止
* W4 联机打印暂停
* W5 联机打印恢复
* W6 SPARK已连接
* W7 SPARK已断开
*
* GUI相关指令
*
* W100 欢迎界面1 欢迎使用 xxxx
* W101 欢迎界面2 第1步 载入料丝
* W102 欢迎界面3 第2步 插入TF卡
* W103 欢迎界面4 第3步 选择文件打印
* W104 启用调试菜单
* W105 禁用调试菜单
* W106 显示正在下载界面
* W107 关闭正在下载界面
* W108 显示下载错误界面
* W109 重启DGUS
* W110 显示DGUS启动界面
* W111 显示DGUS主界面
* W112 显示DGUS SD界面
* W113 显示DGUS准备界面
* W114 显示DGUS设置界面
* W116 显示DGUS料丝载入界面
* W117 显示DGUS料丝卸载界面
* W118 显示DGUS打印界面
* W119 显示DGUS错误信息界面
* W120 显示DGUS提示信息界面
* W121 显示DGUS调平界面
* W122 显示DGUS加热界面
* W123 显示DGUS版本信息界面
* W124 显示DGUS高级设置界面
* W125 显示DGUS WIFI设置界面
* W126 显示DGUS打印结束界面
* W127 显示DGUS自动关机界面
* W128 显示DGUS打印设置界面
* W129 显示DGUS保存关机界面
* W130 显示DGUS恢复打印界面
* W131 显示DGUS恢复出厂设置界面
* W132 显示带返回的信息显示界面 
* W133 显示无返回的信息显示界面
* W134 显示ZOFFSET界面
* W135 显示工程测试界面
* W136 显示开机自检界面
*
* 打印功能相关指令
*
* W200 查询文件是否存在，存在返回true，不存在返回false, W200 test.gco
* W201 查询打印机状态，返回 打印机状态, 正在打印的文件， 已打印时间， 打印的文件字节量
* W202 打开现有文件并添加数据，暂时无效
* W203 打印机立即重启
* W204 返回ESP32的IP地址字符串，如未连接网络，则返回Disconnected
* W205 返回ESP32的序列号字符串
* W206 返回ESP32的固件版本字符串
* W207 返回ESP32的设备名称字符串
* W208 返回ESP32状态信息字符串
* W209 取消打印
* W210 连接工厂默认WIFI网
* W211 打开喷头0风扇
* W212 关闭喷头0风扇
* W213 WEEDO云状态 
* W214 打开空滤风扇
* W215 关闭空滤风扇
* W216 保存参数至SD卡gcode文件
* W217 从SD卡gcode文件中读取参数并保存至主板内存
* W218 已连接AP名
* W219 AP信号强度
* W220 接收jpg数据
* W221 接收jpg数据开始
* W222 接收jpg数据结束
* W223 解析GCode信息
* W224 断丝暂停
* W225 更新SoftEndstop
* W226 关闭喷头XY偏置粗调与精调参数
* W227 关闭喷头XY偏置精调参数
* W228 恢复喷头XY偏置粗调与精调参数
* W230 接收文件数据
* W231 接收文件开始
* W232 接收文件结束
* W233 切换喷头
* W240 显示已打印时间和次数
* W299 关闭电源
*
* 网络功能相关指令
*
* W300 设置WIFI网ssid
* W301 设置WIFI网pwd
* W302 连接WIFI网络
* W303 返回NDisk文件页数
* W304 NDisk列表开始
* W305 NDisk文件名
* W306 NDisk文件列表结束
* W307 NDisk文件机型名
* W308 NDisk文件层高
* W309 NDisk文件填充率
* W310 NDisk文件材料
* W311 NDisk文件料丝用量
* W312 NDisk文件温度
* W313 NDisk文件打印时间
* W314 NDisk文件信息结束
* W315 NDisk列表有更新
* W316 正在打印文件名
* W317 NDisk MinX
* W318 NDisk MaxX
* W320 AP列表开始
* W321 AP名
* W322 AP列表结束
* W323 AP已连接
* W324 AP已断开
* W330 显示WIFICAM消息
* 
* EPC相关指令
*
* W400 读取EPC读卡器数据
* W401 读取EPC芯片信息
*
* WIFICAM相关指令
*
* W999 WIFICAM启动
*
*
* 打印机发往主板的指令，以WTCMD:开头，后跟大写英文单词
* WTCMD:IP			读取ESP32的IP地址
* WTCMD:RESTART		ESP32立即重启
* WTCMD:VERSION		读取ESP32的硬件和固件版本
* WTCMD:OTADEFAULT	从默认网址OTA升级
* WTCMD:OTAURI		从指定网址OTA升级
* WTCMD:OTAFORCE	从指定网络OTA至指定版本
* WTCMD:WIFIDEFAULT	连接工厂默认WIFI网
* WTCMD:JOIN		连接到指定的WIFI网
*
*/

#include "../MarlinCore.h"
#include "../gcode/parser.h"
#include "../gcode/gcode.h"
#include "../HAL/STM32F1/watchdog.h"
#include "../sd/cardreader.h"
#include "../libs/nozzle.h"
#include "../libs/duration_t.h"
#include "../libs/buzzer.h"
#include "../module/planner.h"
#include "../module/temperature.h"
#include "../module/configuration_store.h"
#include "../module/printcounter.h"
#include "../module/probe.h"
#include "../module/stepper.h"
#include "../feature/pause.h"
#include "../feature/host_actions.h"
#include "../lcd/ultralcd.h"

#include "WTCMD.h"
#include "WTUtilty.h"
#include "WTPacket.h"
#include "WTGcodeinfo.h"

#if ENABLED(POWER_LOSS_RECOVERY)
  #include "../feature/powerloss.h"
#endif

#define SD_CONFIG_FILE		"config.sav"

#define ACTION_ON_PAUSE		"pause"
#define ACTION_ON_PAUSED	"paused"
#define ACTION_ON_RESUME	"resume"
#define ACTION_ON_CANCEL	"cancel"
#define ACTION_ON_DISCONNECT	"disconnect"
#define ACTION_ON_START       "start"
#define ACTION_ON_FINISH      "finish"

#define EEPROM_HOST_MODE				1000

#define STATUS_CHECK_TIME_INTERVAL		1000
#define STATUS_TOGGLE_TIME_INTERVAL		5000

extern void wtlcd_welcome1();
extern void wtlcd_welcome2();
extern void wtlcd_welcome3();
extern void wtlcd_welcome4();
extern void wtlcd_WifiOTAMenu();
extern void menu_action_submenu(screenFunc_t func);
extern void wtlcd_ShowUploadingScreen();
extern void wtlcd_ShowUploadFailScreen();
extern void wt_restart();
extern void lcd_sdcard_stop();
extern void lcd_return_to_status();
extern void lcd_main_menu();
extern CardReader card;

WT_STATUS wt_machineStatus = WT_STATUS::IDLE;

WT_MAIN_ACTION wt_mainloop_action = WT_MAIN_ACTION::WMA_IDLE;
uint8_t wt_onlineprinting = SPARK_IDLE;		// 0 = not printing, 1 = paused, 2 = printing, 3 = lost


char ipAddress[16];
char esp32_name[12];
char esp32_hardware[8];
char esp32_version[8];
char esp32_message[22];
char esp32_serial[16];
bool getIPAddress = false;
bool isOTAUpdating = false;
char uploadFilename[FILENAME_LENGTH + 2];
char wifi_ssid[30];
char wifi_pwd[30];
char parsedString[30];
uint8_t wtvar_hostmode = HOST_WIIBUILDER;
char tb_state[20];
uint8_t wt_active_extruder;

extern xyze_pos_t resume_position;

uint32_t wt_status_check = STATUS_CHECK_TIME_INTERVAL;
uint32_t wt_status_togle = STATUS_TOGGLE_TIME_INTERVAL;

// 欢迎界面1 欢迎使用 xxxx
void Show_WelcomeScreen1()
{
	defer_return_to_status = true;
	lcd_goto_screen(wtlcd_welcome1);
}

// 欢迎界面2 第1步 载入料丝
void Show_WelcomeScreen2()
{
	defer_return_to_status = true;
	lcd_goto_screen(wtlcd_welcome2);
}

// 欢迎界面3 第2步 插入TF卡
void Show_WelcomeScreen3()
{
	defer_return_to_status = true;
	lcd_goto_screen(wtlcd_welcome3);
}

// 欢迎界面4 第3步 选择文件打印
void Show_WelcomeScreen4()
{
	defer_return_to_status = true;
	lcd_goto_screen(wtlcd_welcome4);
}

void GetMachineStatus()
{
	SERIAL_ECHOPGM("MStatus:");
	SERIAL_ECHO(wt_machineStatus);

	SERIAL_ECHOPGM(" MFile:");
	#ifdef SDSUPPORT
	card.printFilename();
	#endif

	char buffer[21];
	duration_t elapsed = print_job_timer.duration();
	elapsed.toString(buffer);

	SERIAL_ECHOPGM(" MTime:");
	SERIAL_ECHO(buffer);

	SERIAL_ECHOPGM(" MPercent:");
	#ifdef SDSUPPORT
	card.report_status();
	#endif

	SERIAL_EOL();

}

void wt_CloseUploadScreen()
{
	buzzer.tone(30, 1000);
	lcd_return_to_status();
}

void wt_spark_begin()
{
	wt_onlineprinting = SPARK_PRINTING;
	print_job_timer.start();
	lcd_setstatusPGM(MMSG_PRINTING[wtvar_language], -1);
	lcd_return_to_status();
	wt_machineStatus = WT_STATUS::PRINTING;
}

void wt_spark_end()
{
	wt_machineStatus = WT_STATUS::FINISH;
	wt_onlineprinting = SPARK_IDLE;
	print_job_timer.stop();
	// queue.enqueue_one_P(PSTR("M18"));
	queue.enqueue_one_P(PSTR("M18"));
	if (print_job_timer.duration() > 60)
		queue.enqueue_one_P(PSTR("M31"));
	else
		lcd_reset_status();
}

void wt_spark_cancel()
{
	lcd_sdcard_stop();
	while (1);
}

void wt_spark_pause()
{
#if ENABLED(PARK_HEAD_ON_PAUSE)
	queue.enqueue_one_P(PSTR("M125"));
#endif
	print_job_timer.pause();

	wt_machineStatus = WT_STATUS::PAUSE;
	wt_onlineprinting = SPARK_PAUSED;

	lcd_setstatusPGM(MMSG_PRINT_PAUSED[wtvar_language], -1);
}

void wt_spark_resume()
{
#if ENABLED(PARK_HEAD_ON_PAUSE)
	resume_print(0, 0, 0, 0);
#endif
	print_job_timer.start();

	planner.synchronize();

	wt_machineStatus = WT_STATUS::PRINTING;
	wt_onlineprinting = SPARK_PRINTING;
	lcd_setstatusPGM(MMSG_PRINTING[wtvar_language], -1);
	lcd_return_to_status();
}

void wt_reset_param(void)
{
	wtvar_gohome = 0;
	wtvar_showWelcome = 1;
	wtvar_hostmode = 0;
	(void)settings.save();

	safe_delay(200);
	wt_restart();
}

// 向Spark发送暂停打印请求
void wt_online_pause()
{
	SERIAL_ECHOLNPGM_0("//action:" ACTION_ON_PAUSE);
}

// 向Spark发送恢复打印请求
void wt_online_resume()
{
	SERIAL_ECHOLNPGM_0("//action:" ACTION_ON_RESUME);
}

// 向Spark发送取消打印请求
void wt_online_cancel()
{
	SERIAL_ECHOLNPGM_0("//action:" ACTION_ON_CANCEL);
}

extern uint8_t lcd_status_update_delay;
// W Command Process
void WTCMD_Process()
{
	switch (parser.codenum)
	{
	case 1:		// 联机打印开始
		wt_spark_begin();
		SERIAL_ECHOLNPGM_0("Online printing: begin");
		break;

	case 2:		// 联机打印结束
		wt_spark_end();
		SERIAL_ECHOLNPGM_0("Online printing: end");
		break;

	case 3:		// 联机打印中止
		wt_spark_cancel();
		SERIAL_ECHOLNPGM_0("Online printing: cancel");
		break;

	case 4:		// 联机打印暂停
		wt_spark_pause();
		SERIAL_ECHOLNPGM_0("Online printing: pause");
		break;

	case 5:		// 联机打印恢复
		wt_spark_resume();
		SERIAL_ECHOLNPGM_0("Online printing: resume");
		break;

	case 6:		// SPARK已连接
		lcd_setstatusPGM(MMSG_OCTOPRINT_OPEN, -1);
		break;

	case 7:		// SPARK已断开
		lcd_setstatusPGM(MMSG_OCTOPRINT_CLOSE, -1);
		break;

	case 100:	// 欢迎界面1 欢迎使用 xxxx
		Show_WelcomeScreen1();
		break;

	case 101:	// 欢迎界面2 第1步 载入料丝
		Show_WelcomeScreen2();
		break;

	case 102:	// 欢迎界面3 第2步 插入TF卡
		Show_WelcomeScreen3();
		break;

	case 103:	// 欢迎界面4 第3步 选择文件打印
		Show_WelcomeScreen4();
		break;

	case 201:	// 查询打印机状态，返回 打印机状态, 正在打印的文件， 已打印时间， 打印的文件字节量
		GetMachineStatus();
		break;

	case 203:	// 打印机立即重启
		wt_restart();
		while (1);
		break;

	case 209:		// 取消打印
		lcd_sdcard_stop();
		break;

	case 211:		// 自动进丝
		queue.enqueue_one_P(PSTR("M104 T0 S210"));
		queue.enqueue_one_P(PSTR("M701 T0"));
		queue.enqueue_one_P(PSTR("M104 S0"));
		break;

	case 212:		// 自动退丝
		queue.enqueue_one_P(PSTR("M104 T0 S210"));
		queue.enqueue_one_P(PSTR("M702 T0"));
		queue.enqueue_one_P(PSTR("M104 S0"));
		break;

	case 230:		// 恢复默认参数并重启
		wt_reset_param();
		break;

	case 240: // 显示已打印时间和次数
		wt_lcd_show_printer_stats();
		break;

	}

}

// restart
void wt_restart()
{
	SERIAL_ECHOLNPGM_0("//action:" ACTION_ON_DISCONNECT);
	safe_delay(500);
	nvic_sys_reset();
};


// 保存主机模式
void wt_set_hostmode(const uint8_t mode)
{
	if (mode != wtvar_hostmode)
	{
		wtvar_hostmode = mode;
		(void)settings.save();
		wt_restart();
	}
}

void calc_local_printing_time(void)
{
	float p = (float)progress_bar_percent * 0.01;
	uint32_t elapsed = print_job_timer.duration();

	long est_total;
	if (gcodeinfo.info.i_totaltime > 0)
	{
		if (p > 0)
			est_total = (long)((float)elapsed / p);
		else
			est_total = gcodeinfo.info.i_totaltime;

		long est_last = est_total - elapsed;
		long slicing_last = gcodeinfo.info.i_totaltime - elapsed;
		if (slicing_last < 0) slicing_last = 0;

		gcodeinfo.info.i_lefttime = (long)((float)est_last * p + (float)slicing_last * (1 - p));
	}
	else
	{
		if (p > 0)
			est_total = (long)((float)elapsed / p);
		else
			est_total = 0;

		gcodeinfo.info.i_lefttime = est_total - elapsed;
		if (gcodeinfo.info.i_lefttime < 0) gcodeinfo.info.i_lefttime = 0;
	}
}

// main loop action
void wt_loopaction(void)
{
	if (wt_mainloop_action == WMA_IDLE) return;

	if (wt_mainloop_action == WMA_PAUSE)
	{

		if (queue.length > 0) return;

		queue.enqueue_now_P(PSTR("M125"));
		wt_mainloop_action = WMA_IDLE;

		return;
	}

	if (wt_mainloop_action == WMA_RESUME)
	{

		if (queue.length > 0) return;

		queue.enqueue_one_P(PSTR("M24"));
		wt_mainloop_action = WMA_IDLE;
	}
}


// got bootload
void wt_bootload()
{
        // 写入更新标识
    uint16_t working_crc = 0;
    int pos = 0;
    uint8_t signal = 0x5A;
    persistentStore.access_start();
    persistentStore.write_data(pos, &signal, 1, &working_crc); 
    persistentStore.access_finish();
    wt_restart();
}

void host_action_start()
{
	SERIAL_ECHOLNPGM("//action:" ACTION_ON_START);
}

void host_action_finish()
{
	SERIAL_ECHOLNPGM("//action:" ACTION_ON_FINISH);
}

void wt_powerloss_set(uint8_t state)
{
	wtvar_enablepowerloss = state;
	recovery.enable(wtvar_enablepowerloss);
	(void)settings.save();
}