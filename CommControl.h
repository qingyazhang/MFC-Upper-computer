#pragma once

unsigned char __stdcall lrc(unsigned char *dat, unsigned char len);
int __stdcall commRead(HANDLE comm, unsigned char* buffer, DWORD r_len,DWORD timeout);
int __stdcall commWrite(HANDLE comm, unsigned char* buffer, int len, DWORD timeout);
int __stdcall ReadMeterFly(int handle, unsigned char *id, unsigned char *ml, unsigned char *dt, unsigned char *tf);

int __stdcall WriteMeterState(int handle, unsigned char *id,unsigned short statetime);
int __stdcall WriteFlowP(int handle, unsigned char *id,unsigned char *p,unsigned char *OprtTime);
int __stdcall ReadFlowP(int handle, unsigned char *id,unsigned char *p);
int __stdcall ReadStateTmpr(int handle, unsigned char *id, signed short *T1,signed short *T2,unsigned short *timeremain);
int __stdcall ReadTmpr(int handle, unsigned char *id, signed char *twos,signed char *taos);
int __stdcall WriteTmpr(int handle, unsigned char *id, signed char twos,signed char taos,unsigned char *OprtTime);
int __stdcall ReadFlow_meter(int handle, unsigned char *id,signed char *p);


