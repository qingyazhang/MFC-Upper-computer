
#include "OperateExcelFile.h"
#include "include.h"

#define PowerN_PolyNormal 2

class AlgrithmsTest:public OperateExcelFile
{
public:
	AlgrithmsTest(void);
	~AlgrithmsTest(void);

public:
	double dt[1500],v[1500],kg_weight[1500],ml_meter[1500],tmprwater[1500];
	double x[PowerN_PolyNormal+1][PowerN_PolyNormal+1],y[PowerN_PolyNormal+1],\
			a[PowerN_PolyNormal+1];
	double p[2][36];//存储36个参数
	double qavg[13],dtavg[13],tmpravg[13];
	float errnum;	
public:
	CString FangCheng;
	double SSE;
	int Num;
public:
	FlowPram flowpramlsf;
public:
	void GetData(long n,long FlyTimeDtline,long tmprline );
	void Calc8Paras();
	void PreDealData();
	void Fit8Paras();
	void Fit3Paras();

//	void DealData(int err_nums,float x1,float x2);
//	void DataForMeterv();  //将流量L/h转换为滴数/s
//	void DataForMetera(int NumStart,int NumCount);  //将系数乘以第一次飞行时间
void GenerateLSF(int NumStart,int NumCount);
//	void LSFPrint();
//	void DealSelectData(int SeriesN);
//public:
//	void SortArray();
//	void DeleteError_0();
//	void GenerateLSF_3line(int t1,int t2,int errnum1,int errnum2,int errnum3);
	
	/*float FlytimeToFlowrate(int t1,int t2);
	int DealA0A1(int v1_num,int v2_num,int errnum);*/
public:
	/*void Calc36Paras();
	void GetData36Paras(int dtline,int row);*/
	short round(double num);
	void ErrorTest(long row);
};

