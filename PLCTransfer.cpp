#pragma once

#include "Stdafx.h"
#include "include.h"
#include "CommControl.h"
#include "PLCTransfer.h"

unsigned short __stdcall CRC16(unsigned char* puchMsg, unsigned short usDataLen)
{
	unsigned char uchCRCHi = 0xFF;
	unsigned char uchCRCLo = 0xFF;
	unsigned char uIndex;
	while (usDataLen--)
	{
		uIndex = uchCRCHi ^ *puchMsg++;
		uchCRCHi = uchCRCLo ^ auchCRCHi[uIndex];
		uchCRCLo = auchCRCLo[uIndex];
	}
	return (uchCRCHi << 8 | uchCRCLo);
}
//PLC通信操作
int __stdcall PLCTransfer(HANDLE handle, u8 cmd, u8 index, u8 *data, PLCFrame * fb)
{
	PLCFrame frame;
	int status;

	frame.Head[0] = FRAME_HEAD;
	frame.Head[1] = FRAME_HEAD;
	frame.Naddr = NETADDR;
	frame.Funccode = cmd;
	frame.Index = index;
	memcpy(frame.data, data, 4);
	frame.result = 0xFF;
	frame.lrc = lrc(frame.Head, 10);
	frame.end = END_HEAD;
	
	status = commWrite(handle, frame.Head, 12, 100);
	if(!status)
		return FALSE;
	Sleep(30);
	status = commRead(handle, (u8*)fb, 12,150);
	if(!status)
		return FALSE;
	if(fb->lrc != lrc(fb->Head, 10))
		return FALSE;

	return TRUE;
}
//变频器启动/停止
int __stdcall DanfossAction(HANDLE handle, u16 action)
{
	DanfossFrame t_frame;
	DanfossFrame r_frame;
	int status;
	u16 crc;

	t_frame.d_addr = 0x01;
	t_frame.d_code = WRITEREG;
	t_frame.d_Modbusaddr[0] = 0xC3;
	t_frame.d_Modbusaddr[1] = 0x4F;
	t_frame.d_data[0] = (action >> 8) & 0xFF; 
	t_frame.d_data[1] = action;

	crc = CRC16((u8*)&t_frame, 6);
	t_frame.d_data[2] = (crc >> 8) & 0xFF;
	t_frame.d_data[3] = crc;

	status = commWrite(handle, (u8*)&t_frame, 8, 100);
	if(!status)
		return FALSE;
	Sleep(30);
	status = commRead(handle, (u8*)&r_frame, 8,150);
	if(!status)
		return FALSE;

	crc = CRC16((u8*)&r_frame, 6);

	if((r_frame.d_data[3] != (u8)crc) || (r_frame.d_data[2] != (u8)(crc >> 8)))
		return FALSE;
	return TRUE;
}
//设置频率
int __stdcall DanfossSet(HANDLE handle, short* percent)
{
	DanfossFrame t_frame;
	DanfossFrame r_frame;
	int status;
	u16 crc;

	t_frame.d_addr = 0x01;
	t_frame.d_code = WRITEREG;
	t_frame.d_Modbusaddr[0] = 0x0C;
	t_frame.d_Modbusaddr[1] = 0x1B;
	t_frame.d_data[0] = (*percent >> 8) & 0xFF; 
	t_frame.d_data[1] = *percent;

	crc = CRC16((u8*)&t_frame, 6);
	t_frame.d_data[2] = (crc >> 8) & 0xFF;
	t_frame.d_data[3] = crc;

	status = commWrite(handle, (u8*)&t_frame, 8, 100);
	if(!status)
		return FALSE;
	Sleep(30);
	status = commRead(handle, (u8*)&r_frame, 8,150);
	if(!status)
		return FALSE;

	crc = CRC16((u8*)&r_frame, 6);

	if((r_frame.d_data[3] != (u8)crc) || (r_frame.d_data[2] != (u8)(crc >> 8)))
		return FALSE;

	*percent = r_frame.d_data[1] | ((u16)r_frame.d_data[0] << 8);
	return TRUE;
}
//读频率
int __stdcall DAnfossRead(HANDLE handle, short* percent)
{
	DanfossFrame t_frame;
	DanfossFrame r_frame;
	int status;
	u16 crc;

	t_frame.d_addr = 0x01;
	t_frame.d_code = READREG;
	t_frame.d_Modbusaddr[0] = 0x0C;
	t_frame.d_Modbusaddr[1] = 0x1B;
	t_frame.d_data[0] = 0; 
	t_frame.d_data[1] = 0x01;

	crc = CRC16((u8*)&t_frame, 6);
	t_frame.d_data[2] = (crc >> 8) & 0xFF;
	t_frame.d_data[3] = crc;

	status = commWrite(handle, (u8*)&t_frame, 8, 100);
	if(!status)
		return FALSE;
	Sleep(30);
	status = commRead(handle, (u8*)&r_frame, 7,150);
	if(!status)
		return FALSE;

	crc = CRC16((u8*)&r_frame, 5);

	if((r_frame.d_data[2] != (u8)crc) || (r_frame.d_data[1] != (u8)(crc >> 8)))
	return FALSE;

	*percent = r_frame.d_data[0] | ((u16)r_frame.d_Modbusaddr[1] << 8);
	return TRUE;
}