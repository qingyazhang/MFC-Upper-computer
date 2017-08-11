#include "Stdafx.h"
#include "include.h"
#include "CommControl.h"

#define RERROR				0
#define METER_PRECURSOR		0xFE	//前导符
#define METER_FRAME_HEAD	0x68	//帧头
#define METER_FRAME_TAIL	0x16	//帧尾
#define MAX_BUFFER_LEN		64

// 自定义控制码
// #define CUSTOM_READ			0x21
//自定义数据标识
#define RD_INF				0x0082
#define RD_PURSE_USE		0x0083
#define RD_PURSE			0x4083
#define RD_PULSE_CONST		0x8083
#define RD_PWR_TYPE			0x8084
#define RD_SW_VER			0x8085
#define RD_VALVE_CNT		0x8086
#define RD_INTERFE_CNT		0x8087

#define CUSTOM_WRITE		0x25
#define WR_SET_INF			0x0082
#define WR_PULSE_CONST		0x0083
#define WR_RUN_STA			0x8183
#define WR_OFF_USE			0x9d28
#define WR_PWR_TYPE			0x9d29
#define WR_ADDR_BROAD		0x9d2a
#define WR_INIT_MERTER		0x9d2b
#define JUMP_BOODLOADER		0x9981

#define RD_INSTANT			0x0090

#define AMT_RADR_C			0x03	//读地址
#define AMT_RADR_DI			0x0A81	
#define AMT_FLY_C			0x31	//读超声波飞行时间
#define AMT_FLY_DI			0x3B71	
#define AMT_State_C			0x35	//使水表进入/退出校表状态
#define AMT_State_DI		0x7763
#define AMT_WFlowP_C		0x35	//写流量曲线参数
#define AMT_WFlowP_DI		0x4763
#define AMT_RFlowP_C		0x31	//写流量曲线参数
#define AMT_RFlowP_DI		0x4771
#define AMT_RStTmpr_C		0x31	//读校表时温度相关量
#define AMT_RStTmpr_DI		0x3371
#define AMT_RTmpr_C			0x31	//读温度偏移量参数
#define AMT_RTmpr_DI		0x5871
#define AMT_WTmpr_C			0x31	//写温度偏移量参数
#define AMT_WTmpr_DI		0x5863
#define AMT_RTotal_C		0x31	//读累积流量
#define AMT_RTotal_DI		0x3771
#define AMT_RFlow_C			0x31	//读累积流量
#define AMT_RFlow_DI			0x3571

typedef struct
{
	HANDLE handle;
	OVERLAPPED commOverlapped;
}EventStruct;

static EventStruct evt[20];
static unsigned char BroatcastAddr[7] = {0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa};

static unsigned char PrepareData188(unsigned char *tx_buf, unsigned char *addr, unsigned char cmd,\
	unsigned short di, unsigned char *dat, unsigned char len, unsigned char ser);
static unsigned short Parse188(unsigned char *data, unsigned char *buffer);

#pragma pack(1)
typedef struct
{
	unsigned char FrameHead;
	unsigned char Type;
	unsigned char Addr[7];
	unsigned char CtlCode;
	unsigned char DataLen;
	unsigned short DI;
	unsigned char ser;
	unsigned char Dat[100];
}struct_protocol_188;
#pragma 

HANDLE __stdcall InitComm(WCHAR* comm)
{
	HANDLE hdev;
	CString wcomm;
	CString prefix = _T("\\\\.\\");

	wcomm.Format(_T("%s%s"), prefix, comm);
	hdev = CreateFile(comm, GENERIC_READ|GENERIC_WRITE, NULL,NULL,
				OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);
	if(hdev == INVALID_HANDLE_VALUE)
	{
		hdev = CreateFile(comm, GENERIC_READ|GENERIC_WRITE, NULL,NULL,
			OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL);
		if(hdev == INVALID_HANDLE_VALUE)
		{
			return FALSE;
		}
	}

	SetupComm(hdev,128,128); //缓冲区
	SetCommMask(hdev, EV_RXCHAR|EV_TXEMPTY );//设置事件驱动的类型 

	DCB dcb;
	GetCommState(hdev, &dcb);
	BuildCommDCB(_T("115200,e,8,1 "), &dcb);
	SetCommState(hdev, &dcb);

	PurgeComm(hdev, PURGE_RXCLEAR|PURGE_TXCLEAR|PURGE_RXABORT|PURGE_TXABORT);//清空缓存

	return hdev;
}

int __stdcall ExitComm(int handle)
{
	if(!CloseHandle((HANDLE)handle))
	{
		CloseHandle((HANDLE)handle);
	}
	return TRUE;
}
//读表号
int __stdcall ReadMeterId(int handle, unsigned char *id)
{
	unsigned char dat_buff[256];
	unsigned char length; 
	unsigned char t_data[23];
	int status;

	length = PrepareData188(dat_buff, BroatcastAddr, AMT_RADR_C, AMT_RADR_DI, NULL, 0, rand());
	status = commWrite((HANDLE)handle, dat_buff, length, 100);
	if(!status)
		return FALSE;

	//Sleep(100);
	status = commRead((HANDLE)handle, dat_buff, 17,150);
	if(!status)
		return FALSE;

	status = Parse188(dat_buff, t_data);
	memcpy(id, dat_buff + 4, 7);
// 	if(!status)
// 		return FALSE;
	return TRUE;
}
//读累积流量
int __stdcall ReadMeterTotal(int handle, unsigned char *id,__int64 *m3_f,__int64 *m3_r)
{
	unsigned char dat_buff[256];
	unsigned char length; 
	int status;
	unsigned char t_data[23];

	length = PrepareData188(dat_buff, id, AMT_RTotal_C, AMT_RTotal_DI, NULL, 0, rand());
	status = commWrite((HANDLE)handle, dat_buff, length, 100);
	if(!status)
		return FALSE;

	Sleep(100);
	status = commRead((HANDLE)handle, dat_buff, 40,150);
	if(!status)
		return FALSE;

	status = Parse188(dat_buff, t_data);
	if(!status)
		return FALSE;
	*m3_f = (t_data[0] & 0x0f) 					+ (t_data[0] >> 4) * 10 + \
		   (t_data[1] & 0x0f) * 100				+ (t_data[1] >> 4) * 1000 + \
		   (t_data[2] & 0x0f) * 10000			+ (t_data[2] >> 4) * 100000 + \
		   (t_data[3] & 0x0f) * 1000000			+ (t_data[3] >> 4) * 10000000 +\
		   (t_data[4] & 0x0f) * 100000000		+ (t_data[4] >> 4) * 1000000000LL +\
		   (t_data[5] & 0x0f) * 10000000000LL	+ (t_data[5] >> 4) * 100000000000LL;
	*m3_r = (t_data[7] & 0x0f)					+ (t_data[7] >> 4) * 10 + \
		   (t_data[8] & 0x0f) * 100			+ (t_data[8] >> 4) * 1000 + \
		   (t_data[9] & 0x0f) * 10000			+ (t_data[9] >> 4) * 100000 + \
		   (t_data[10] & 0x0f)  * 1000000		+ (t_data[10] >> 4)  * 10000000 + \
		   (t_data[11] & 0x0f)  * 100000000		+ (t_data[11] >> 4)  * 1000000000LL +\
		   (t_data[12] & 0x0f)  * 10000000000LL	+ (t_data[12] >> 4)  * 100000000000LL;
	
	return TRUE;
}

int __stdcall ReadMeterInstant(int handle, unsigned char *id)
{
	unsigned char dat_buff[256];
	unsigned char length; 
	int status;
	int instant;

	length = PrepareData188(dat_buff, id, 0x31, RD_INSTANT, NULL, 0, rand());
	status = commWrite((HANDLE)handle, dat_buff, length, 100);
	if(!status)
		return FALSE;

	Sleep(30);
	status = commRead((HANDLE)handle, dat_buff, 21,150);
	if(!status)
		return FALSE;

	status = Parse188(dat_buff, (unsigned char*)&instant);
	if(!status)
		return FALSE;
	return instant;
}
//读表飞行时间相关参数
int __stdcall ReadMeterFly(int handle, unsigned char *id, unsigned char *ml, unsigned char *dt, unsigned char *tf)
{
	unsigned char dat_buff[256];
	unsigned char length; 
	unsigned char t_data[11];
	int status;

	length = PrepareData188(dat_buff, id, AMT_FLY_C, AMT_FLY_DI, NULL, 0, rand());
	status = commWrite((HANDLE)handle, dat_buff, length, 150);
	if(!status)
		return FALSE;

	Sleep(30);
	status = commRead((HANDLE)handle, dat_buff, 28,300);
	if(!status)
		return FALSE;

	status = Parse188(dat_buff, t_data);
	if(!status)
		return FALSE;
	memcpy(ml, t_data, 4);
	memcpy(dt, t_data + 4, 2);
	memcpy(tf, t_data + 6, 2);
	return TRUE;
}
//写校表状态
int __stdcall WriteMeterState(int handle, unsigned char *id,unsigned short statetime)
{
	unsigned char dat_buff[256];
	unsigned char length; 
	//unsigned char t_data[11];
	int status;
	unsigned char timebcd[4];
	
	timebcd[0] = (statetime /10 % 10 ) << 4 | statetime % 10 ;
	
	timebcd[1] = ( statetime /1000 ) << 4 | statetime /100 % 10;
	timebcd[2] = ~timebcd[0];
	timebcd[3] = ~timebcd[1];

	length = PrepareData188(dat_buff, id, AMT_State_C, AMT_State_DI, timebcd, 4, rand());
	status = commWrite((HANDLE)handle, dat_buff, length, 150);
	if(!status)
		return FALSE;

	Sleep(30);
	status = commRead((HANDLE)handle, dat_buff, 17,300);
	if(!status)
		return FALSE;

	//status = Parse188(dat_buff, t_data);
	//if(!status)
	//	return FALSE;
	return TRUE;
}
//读流量参数
int __stdcall ReadFlowP(int handle, unsigned char *id,unsigned char *p)
{
	unsigned char dat_buff[256];
	unsigned char length; 
	unsigned char t_data[20];
	int status;	

	length = PrepareData188(dat_buff, id, AMT_RFlowP_C, AMT_RFlowP_DI, NULL, 0, rand());
	status = commWrite((HANDLE)handle, dat_buff, length, 150);
	if(!status)
		return FALSE;

	Sleep(30);
	status = commRead((HANDLE)handle, dat_buff, 38,300);
	if(!status)
		return FALSE;

	status = Parse188(dat_buff, t_data);
	if(!status)
		return FALSE;
	memcpy(p+1, t_data, 1);
	memcpy(p,   t_data + 1, 1);
	memcpy(p+3, t_data + 2, 1);
	memcpy(p+2, t_data + 3, 1);
	memcpy(p+5, t_data + 4, 1);
	memcpy(p+4, t_data + 5, 1);
	memcpy(p+7, t_data + 6, 1);
	memcpy(p+6, t_data + 7, 1);
	memcpy(p+9, t_data + 8, 1);
	memcpy(p+8, t_data + 9, 1);
	memcpy(p+11, t_data + 10, 1);
	memcpy(p+10, t_data + 11, 1);
	memcpy(p+13, t_data + 12, 1);
	memcpy(p+12, t_data + 13, 1);
	memcpy(p+15, t_data + 14, 1);
	memcpy(p+14, t_data + 15, 1);
	memcpy(p+17, t_data + 16, 1);
	memcpy(p+16, t_data + 17, 1);
	memcpy(p+19, t_data + 18, 1);
	memcpy(p+18, t_data + 19, 1);

	return TRUE;
}
//写流量参数
int __stdcall WriteFlowP(int handle, unsigned char *id,unsigned char *p,unsigned char *OprtTime)
{
	unsigned char dat_buff[256];
	unsigned char length; 
	unsigned char t_data[7];
	unsigned char xrc = 0;
	int i;
	int status;	

	memcpy(dat_buff + 1, p , 1);
	memcpy(dat_buff , p + 1 , 1);
	memcpy(dat_buff + 3, p + 2 , 1);
	memcpy(dat_buff + 2, p + 3, 1);
	memcpy(dat_buff + 5, p + 4, 1);
	memcpy(dat_buff + 4, p + 5, 1);
	memcpy(dat_buff + 7, p + 6, 1);
	memcpy(dat_buff + 6, p + 7, 1);
	memcpy(dat_buff + 9, p + 8, 1);
	memcpy(dat_buff + 8, p + 9, 1);
	memcpy(dat_buff + 11, p + 10, 1);
	memcpy(dat_buff + 10, p + 11, 1);
	memcpy(dat_buff + 13, p + 12, 1);
	memcpy(dat_buff + 12, p + 13, 1);
	memcpy(dat_buff + 15, p + 14, 1);
	memcpy(dat_buff + 14, p + 15, 1);
	memcpy(dat_buff + 17, p + 16, 1);
	memcpy(dat_buff + 16, p + 17, 1);
	memcpy(dat_buff + 19, p + 18, 1);
	memcpy(dat_buff + 18, p + 19, 1);
	memcpy(p, dat_buff, 20);
	for(i = 0; i < 20; i++)
	{
		xrc ^= *(p + i);
	}
	*(p + i) = xrc;
	length = PrepareData188(dat_buff, id, AMT_WFlowP_C, AMT_WFlowP_DI, p, 21, rand());
	status = commWrite((HANDLE)handle, dat_buff, length, 150);
	if(!status)
		return FALSE;
	Sleep(80);
	status = commRead((HANDLE)handle, dat_buff, 25,400);
	if(!status)
		return FALSE;

	status = Parse188(dat_buff, t_data);
	if(!status)
		return FALSE;
	memcpy(OprtTime, t_data, 1);
	memcpy(OprtTime+1, t_data + 1, 1);
	memcpy(OprtTime+2, t_data + 2, 1);
	memcpy(OprtTime+3, t_data + 3, 1);
	memcpy(OprtTime+4, t_data + 4, 1);
	memcpy(OprtTime+5, t_data + 5, 2);

	return TRUE;
}
//读校表时测量温度相关量
int __stdcall ReadStateTmpr(int handle,unsigned char *id, signed short *T1,signed short *T2,unsigned short *timeremain)
{
	unsigned char dat_buff[256];
	unsigned char length; 
	unsigned char t_data[11];
	int status;

	length = PrepareData188(dat_buff, id, AMT_RStTmpr_C, AMT_RStTmpr_DI, NULL, 0, rand());
	status = commWrite((HANDLE)handle, dat_buff, length, 150);
	if(!status)
		return FALSE;

	Sleep(30);
	status = commRead((HANDLE)handle, dat_buff, 24,300);//合计2个字节的温度
	if(!status)
		return FALSE;

	status = Parse188(dat_buff, t_data);
	if(!status)
		return FALSE;
	memcpy(T1, t_data, 2);
	memcpy(T2, t_data + 2, 2);
	memcpy(timeremain, t_data + 4, 2);
	return TRUE;
}
//读温度偏移量参数
int __stdcall ReadTmpr(int handle, unsigned char *id, signed char *twos,signed char *taos)
{
	unsigned char dat_buff[256];
	unsigned char length; 
	unsigned char t_data[2];
	unsigned char xrc = 0;
	int status;	

	length = PrepareData188(dat_buff, id, AMT_RTmpr_C, AMT_RTmpr_DI, NULL, 0, rand());
	status = commWrite((HANDLE)handle, dat_buff, length, 150);
	if(!status)
		return FALSE;

	Sleep(30);
	status = commRead((HANDLE)handle, dat_buff, 20,400);
	if(!status)
		return FALSE;

	status = Parse188(dat_buff, t_data);
	if(!status)
		return FALSE;
	memcpy(twos, t_data, 1);
	memcpy(taos, t_data + 1, 1);

	return TRUE;
}
//写温度偏移量参数
int __stdcall WriteTmpr(int handle, unsigned char *id, signed char twos,signed char taos,unsigned char *OprtTime)
{
	unsigned char dat_buff[256];
	unsigned char length; 
	unsigned char t_data[3];
	unsigned char xrc = 0;
	int status;	

	t_data[0] = twos;
	t_data[1] = taos;
	xrc ^= twos;
	xrc ^= taos;
	t_data[2] = xrc;
	
	length = PrepareData188(dat_buff, id, AMT_WFlowP_C, AMT_WFlowP_DI, t_data, 3, rand());
	status = commWrite((HANDLE)handle, dat_buff, length, 150);
	if(!status)
		return FALSE;

	Sleep(30);
	status = commRead((HANDLE)handle, dat_buff, 25,400);
	if(!status)
		return FALSE;

	status = Parse188(dat_buff, t_data);
	if(!status)
		return FALSE;
	memcpy(OprtTime, t_data, 1);
	memcpy(OprtTime+1, t_data + 1, 1);
	memcpy(OprtTime+2, t_data + 2, 1);
	memcpy(OprtTime+3, t_data + 3, 1);
	memcpy(OprtTime+4, t_data + 4, 1);
	memcpy(OprtTime+5, t_data + 5, 2);

	return TRUE;
}
//读流速        
int __stdcall ReadFlow_meter(int handle, unsigned char *id,signed char *p)
{
	unsigned char dat_buff[256];
	unsigned char length; 
	unsigned char t_data[2];
	int status;	

	length = PrepareData188(dat_buff, id, AMT_RFlow_C, AMT_RFlow_DI, NULL, 0, rand());
	status = commWrite((HANDLE)handle, dat_buff, length, 150);
	if(!status)
		return FALSE;

	Sleep(30);
	status = commRead((HANDLE)handle, dat_buff, 19,300);
	if(!status)
		return FALSE;

	status = Parse188(dat_buff, t_data);
	if(!status)
		return FALSE;
	memcpy(p, t_data, 2);
		
	return TRUE;
}

unsigned char PrepareData188(unsigned char *tx_buf, unsigned char *addr, unsigned char cmd,\
						unsigned short di, unsigned char *dat, unsigned char len, unsigned char ser)
{
	struct_protocol_188 *ptr;
	unsigned char sendlen;

	tx_buf[0] = METER_PRECURSOR;
	tx_buf[1] = METER_PRECURSOR;
	ptr = (struct_protocol_188 *)&tx_buf[2];
	ptr->FrameHead = METER_FRAME_HEAD;
	ptr->Type = 0x10; 
	memcpy(ptr->Addr, addr, 7);
	ptr->CtlCode = cmd;
	ptr->DataLen = 3 + len;
	ptr->DI = di;
	ptr->ser = ser;
	if(dat != NULL && len > 0)
	{
		memcpy(ptr->Dat, dat, len);
	}
	sendlen = 2 + 11 + ptr->DataLen;
	tx_buf[sendlen] = (unsigned char)lrc(&ptr->FrameHead, sendlen-2);
	sendlen++;
	tx_buf[sendlen++] = METER_FRAME_TAIL;
	return sendlen;
}

unsigned short Parse188(unsigned char *data, unsigned char *buffer)
{
	unsigned char len;

	data += 2;
	if(*data != 0x68)
	{
		return FALSE;
	}

	len = *(data + 10);
	if((*(data + 11 + len) != lrc(data, len + 11)) ||( len < 3))//len <= 3读表地址错误
	{
		return FALSE;
	}
	memcpy(buffer, data + 14, len - 3);

	return len - 3;
}

unsigned char __stdcall lrc(unsigned char *dat, unsigned char len)
{
	unsigned char lrc = 0x00;

	while(len--)
	{
		lrc += *dat++;
	}

	return lrc;
}

int __stdcall commRead(HANDLE comm, unsigned char* buffer, DWORD r_len,DWORD timeout)
{
	OVERLAPPED comoverlapped;
	unsigned char readbuffer[256];
	DWORD recvBytes = 64;
	BOOL result;
	int lastError;

	memset(&comoverlapped,0,sizeof(OVERLAPPED));
	comoverlapped.hEvent = CreateEvent(NULL,TRUE,FALSE,NULL);

	if(comm == INVALID_HANDLE_VALUE) //如果读句柄无效
	{
		return FALSE;
	}
	else  //否则，句柄有效
	{
		result = ReadFile(comm, readbuffer, r_len, &recvBytes, (LPOVERLAPPED)&comoverlapped);
	}
	if(!result)
	{
		lastError = GetLastError();
		if(lastError == ERROR_IO_PENDING || ERROR_IO_PENDING == ERROR_SUCCESS)
		{
			result = WaitForSingleObject(comoverlapped.hEvent, timeout);
			if(result == WAIT_OBJECT_0)
			{
				GetOverlappedResult(comm, &comoverlapped, &recvBytes, TRUE);
			}
			else
			{
				PurgeComm(comm, PURGE_RXCLEAR|PURGE_TXCLEAR|PURGE_RXABORT|PURGE_TXABORT);
				return FALSE;
			}
			//ReleaseMutex(comoverlapped.hEvent);
		}
		else
		{
			PurgeComm(comm,  PURGE_RXCLEAR|PURGE_TXCLEAR|PURGE_RXABORT|PURGE_TXABORT);
			return FALSE;
		}
	}

	memcpy(buffer, readbuffer, recvBytes);
	PurgeComm(comm, PURGE_RXCLEAR|PURGE_TXCLEAR|PURGE_RXABORT|PURGE_TXABORT);

	return recvBytes;		
}

int __stdcall commWrite(HANDLE comm, unsigned char* buffer, int len, DWORD timeout)
{
	OVERLAPPED comoverlapped;
	DWORD sendBytes;
	BOOL result;
	int lastError;

	memset(&comoverlapped,0,sizeof(OVERLAPPED));
	comoverlapped.hEvent = CreateEvent(NULL,TRUE,TRUE,NULL);

	if(comm == INVALID_HANDLE_VALUE) //如果读句柄无效
	{
		return FALSE;
	}
	else  //否则，句柄有效
	{
		result = WriteFile(comm, buffer, len, &sendBytes,(LPOVERLAPPED)&comoverlapped);
	}
	if(!result)
	{
		lastError = GetLastError();
		if(lastError == ERROR_IO_PENDING || ERROR_IO_PENDING == ERROR_SUCCESS)
		{
			result = WaitForSingleObject(comoverlapped.hEvent, timeout);
			if(result == WAIT_OBJECT_0)
			{
				GetOverlappedResult(comm, &comoverlapped, &sendBytes, TRUE);
			}
			else
			{
				PurgeComm(comm, PURGE_RXCLEAR|PURGE_TXCLEAR|PURGE_RXABORT|PURGE_TXABORT);
				return FALSE;
			}
		}
		else
		{
			PurgeComm(comm, PURGE_RXCLEAR|PURGE_TXCLEAR|PURGE_RXABORT|PURGE_TXABORT);
			return FALSE;
		}
	}

	PurgeComm(comm, PURGE_RXCLEAR|PURGE_TXCLEAR|PURGE_RXABORT|PURGE_TXABORT);

	return sendBytes;		
}