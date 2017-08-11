#ifndef _INCLUDE_H_
#define _INCLUDE_H_

extern "C"
{
	typedef unsigned char u8;
	typedef unsigned short u16;
	typedef unsigned long u32;

#pragma pack(1)
	typedef struct
	{
		unsigned char Second;
		unsigned char Minute;
		unsigned char Hour;
		unsigned char Day;
		unsigned char Month;
		unsigned short Year;
	}OprtTime;

	typedef struct
	{
		signed short P00;
		signed short P01;
		signed short P02;
		signed short P10;
		signed short P11;		
		signed short P12;
		signed short P20;
		signed short P21;		
		signed short P22;
		signed short Td;
		unsigned char Xrc;
	}FlowPram;

	typedef struct
	{
		HANDLE mhandle;			//句柄
		unsigned char meterid[7];//表地址
		int	ml;
		short dt;
		unsigned short tf;
		__int64 total_f;
		__int64 total_r;
		FlowPram flowpram;
		signed char twos;
		signed char taos;
		signed short tmpr1;
		signed short tmpr2;
		float errnum;
		short flow;
	}MeterInfo;
	//采样信号;
	typedef struct 
	{
		int valvetype;
		float weight;  //称重桶质量;
		LONG64 time;   //当前时间;
		float lflow;   //瞬时大流;
		float mflow;   //瞬时小流;
		float inpessure;//入口压力;
		float outpesure;//出口压力;
		float intempr;  //入口温度;
		float outtempr; //出口温度;
		float boxtempr; //储水箱水温;
		int meternum;   //表数量;
		MeterInfo minfo[20];//最多处理20只表;
	}SampleInfo;

	typedef struct
	{
		float setflowrate;
		float actualflowrate;
		float acflowratepre;
		float err;
		float err_pre;
		float err_ppre;
		float KP;
		float KI;
		float KD;
		short Hz;
		int valvell;
		int valvels;
		int valvesl;
		int valvess;
	}PID;


#pragma pack()
	/**
	*初始化通信端口
	*@param comm:串口名称
	*@return 成功返回串口句柄，错误为0
	*/
	__declspec(dllexport) HANDLE __stdcall InitComm(WCHAR* comm);
	/**
	*初始化通信端口
	*@param handle:串口句柄
	*@return 成功非负，错误为0
	*/
	__declspec(dllexport) int __stdcall ExitComm(int handle);
	/**
	*读表号 7字节
	*@param handle:串口句柄
	*@param id:返回表号
	*@return 成功非负，错误为0
	*/
	__declspec(dllexport) int __stdcall ReadMeterId(int handle, unsigned char *id);
	/**
	*读累计流量
	*@param handle:串口句柄
	*@param id:表号
	*@return 成功返回累计流量，错误为0
	*/
	__declspec(dllexport) int __stdcall ReadMeterTotal(int handle, unsigned char *id,__int64 * m3_f,__int64 * m3_r);
	/**
	*读瞬时流量
	*@param handle:串口句柄
	*@param id:表号
	*@return 成功返回瞬时流量，错误为0
	*/
	__declspec(dllexport) int __stdcall ReadMeterInstant(int handle, unsigned char *id);
}
#endif