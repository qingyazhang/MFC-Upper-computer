#include "StdAfx.h"
#include <stdio.h>
#include <math.h>
#include "Algorithms.h"

// short SquareC[68] =
// {
// 	960/2,	966/2,	971/2,	976/2,	982/2,	987/2,	992/2,	997/2,	1002/2,	1006/2,
// 	1011/2,	1016/2,	1020/2,	1025/2,	1029/2,	1034/2,	1038/2,	1042/2,	1046/2,	1050/2,
// 	1054/2,	1058/2,	1062/2,	1066/2,	1069/2,	1073/2,	1076/2,	1080/2,	1083/2,	1087/2,
// 	1090/2,	1093/2,	1096/2,	1099/2,	1102/2,	1105/2,	1108/2,	1111/2,	1113/2,	1116/2,
// 	1119/2,	1121/2,	1124/2,	1126/2,	1128/2,	1131/2,	1133/2,	1135/2,	1137/2,	1139/2,
// 	1141/2,	1143/2,	1145/2,	1147/2,	1149/2,	1151/2,	1152/2,	1154/2,	1155/2,	1157/2,
// 	1158/2,	1160/2,	1161/2,	1163/2,	1164/2,	1165/2,	1166/2,	1167/2
// };
short SquareC[173] = 
{
	207, 208, 209, 209, 210, 210, 211, 211, 212, 213,\
	213, 214, 214, 215, 215, 216, 216, 217, 217, 218,\
	218, 219, 219, 220, 220, 221, 221, 222, 222, 223,\
	223, 224, 224, 225, 225, 225, 226, 226, 227, 227,\
	228, 228, 228, 229, 229, 230, 230, 231, 231, 231,\
	232, 232, 232, 233, 233, 234, 234, 234, 235, 235,\
	235, 236, 236, 236, 237, 237, 237, 238, 238, 238,\
	239, 239, 239, 240, 240, 240, 240, 241, 241, 241,\
	242, 242, 242, 242, 243, 243, 243, 244, 244, 244,\
	244, 244, 245, 245, 245, 245, 246, 246, 246, 246,\
	247, 247, 247, 247, 247, 248, 248, 248, 248, 248,\
	249, 249, 249, 249, 249, 249, 250, 250, 250, 250,\
	250, 250, 250, 251, 251, 251, 251, 251, 251, 251,\
	252, 252, 252, 252, 252, 252, 252, 252, 253, 253,\
	253, 253, 253, 253, 253, 253, 253, 253, 253, 254,\
	254, 254, 254, 254, 254, 254, 254, 254, 254, 254,\
	254, 254, 254, 255, 255, 255, 255, 255, 255, 255,\
	255, 255, 255	};
//short round(double num);

AlgrithmsTest::AlgrithmsTest()
{

}
AlgrithmsTest::~AlgrithmsTest()
{

}
//获取excel的数据;
void AlgrithmsTest::GetData(long n,long FlyTimeDtline,long tmprline)
{
	int i;
	Num = n - 1; //Num个数据
	for(i = 0;i < Num;i++)  
	{	
		dt[i] = GetCellDouble(i + 2 , FlyTimeDtline);		
		tmprwater[i] = GetCellDouble(i + 2 , tmprline);  //获取水表测量的水温;
		v[i]  = GetCellDouble(i + 2, 3) + GetCellDouble(i + 2 , 4);
		kg_weight[i] = GetCellDouble(i + 2 ,2);//称重桶质量;
		ml_meter[i] = GetCellDouble(i + 2,FlyTimeDtline - 1);
	}
}
void AlgrithmsTest::PreDealData()
{
	int i;
	int tmp = 0;
// 	for (i = 0; i< Num;i++)
// 	{
// 		tmp += tmprwater[i];
// 	}
// 	tmp = round(tmp / Num / 4.0);
	for (i = 0;i < 12;i++)
	{
		tmp = round(tmpravg[i] / 4.0 );
		qavg[i] = qavg[i] * 16000 / 3600;
		dtavg[i] = dtavg[i] * SquareC[tmp];			 
	}
	flowpramlsf.Td = round(dtavg[0]);	
	for (i = 0; i < 12;i++)
	{
		dtavg[i] /= 65536;
	}	
}
//计算8对参数
void AlgrithmsTest::Calc8Paras()
{
	int i,j,k;
	double tmp;

	for (i = 0;i < 9;i ++ )
	{
		dtavg[i] = 0.0;
		qavg[i] = 0.0;
		for (j = (5 + i * 30);j < 30 * (i + 1);j++)
		{
			dtavg[i] += dt[j];
			qavg[i] += v[j];
		}
		dtavg[i] /= 25.0;
		qavg[i] /= 25.0;		
	}//计算25个数据的平均值
	flowpramlsf.Td = round(dtavg[0]);
 	for (i = 1;i < 9;i ++)
 	{
 		 dtavg[i] -= dtavg[0];
 	}
	PreDealData();
	flowpramlsf.P01 = round(/*(*//*qavg[0] / dtavg[0] + */qavg[1] / dtavg[1]/*)/2*/);
	flowpramlsf.P11 = round((qavg[1] / dtavg[1] + qavg[2] / dtavg[2])/2);
	flowpramlsf.P21 = round((qavg[2] / dtavg[2] + qavg[3] / dtavg[3])/2);
// 	flowpramlsf.P31 = round((qavg[3] / dtavg[3] + qavg[4] / dtavg[4])/2);
// 	flowpramlsf.P41 = round((qavg[4] / dtavg[4] + qavg[5] / dtavg[5])/2);
// 	flowpramlsf.P51 = round((qavg[5] / dtavg[5] + qavg[6] / dtavg[6])/2);
// 	flowpramlsf.P61 = round((qavg[6] / dtavg[6] + qavg[7] / dtavg[7])/2);
// 	flowpramlsf.P71 = round((qavg[7] / dtavg[7] + qavg[8] / dtavg[8])/2);
	tmp =(/*(qavg[0] - dtavg[0] * flowpramlsf.P01 - qavg[0]) + (*/qavg[1] - dtavg[1] * flowpramlsf.P01 /*- qavg[1]*/)/*)/2*/;
	flowpramlsf.P02 = round(tmp);

	tmp =((qavg[1] - dtavg[1] * flowpramlsf.P11 /*- qavg[1]*/) + (qavg[2] - dtavg[2] * flowpramlsf.P11 /*- qavg[2]*/))/2;
	flowpramlsf.P12 = round(tmp);

	tmp =((qavg[2] - dtavg[2] * flowpramlsf.P21 /*- qavg[2]*/) + (qavg[3] - dtavg[3] * flowpramlsf.P21 /*- qavg[3]*/))/2;
	flowpramlsf.P22 = round(tmp);

// 	tmp =((qavg[3] - dtavg[3] * flowpramlsf.P31/* - qavg[3]*/) + (qavg[4] - dtavg[4] * flowpramlsf.P31 /*- qavg[4]*/))/2;
// 	flowpramlsf.P32 = round(tmp);
// 
// 	tmp =((qavg[4] - dtavg[4] * flowpramlsf.P41 /*- qavg[4]*/) + (qavg[5] - dtavg[5] * flowpramlsf.P41/* - qavg[5]*/))/2;
// 	flowpramlsf.P42 = round(tmp);
// 
// 	tmp =((qavg[5] - dtavg[5] * flowpramlsf.P51/* - qavg[5]*/) + (qavg[6] - dtavg[6] * flowpramlsf.P51 /*- qavg[6]*/))/2;
// 	flowpramlsf.P52 = round(tmp);
// 
// 	tmp =((qavg[6] - dtavg[6] * flowpramlsf.P61 /*- qavg[6]*/) + (qavg[7] - dtavg[7] * flowpramlsf.P61/* - qavg[7]*/))/2;
// 	flowpramlsf.P62 = round(tmp);
// 
// 	tmp =((qavg[7] - dtavg[7] * flowpramlsf.P71/* - qavg[7]*/) + (qavg[8] - dtavg[8] * flowpramlsf.P71 /*- qavg[8]*/))/2;
// 	flowpramlsf.P72 = round(tmp);

}
//拟合polyfit8对参数
void AlgrithmsTest::Fit8Paras()
{
	int i,j,k;
	double tmp;

	for (i = 0;i < 9;i ++ )
	{
		dtavg[i] = 0.0;
		qavg[i] = 0.0;
		for (j = (5 + i * 30);j < 30 * (i + 1);j++)
		{
			dtavg[i] += dt[j];
			qavg[i] += v[j];
		}
		dtavg[i] /= 25.0;
		qavg[i] /= 25.0;		
	}//计算25个数据的平均值
	flowpramlsf.Td = round(dtavg[0]);
	for (i = 0;i < 9;i ++)
	{
		dtavg[i] -= dtavg[0];
	}
	PreDealData();
	flowpramlsf.P01 = round((qavg[1] - qavg[0]) / (dtavg[1] - dtavg[0] ));
	flowpramlsf.P11 = round((qavg[2] - qavg[1]) / (dtavg[2] - dtavg[1] ));
	flowpramlsf.P21 = round((qavg[3] - qavg[2]) / (dtavg[3] - dtavg[2] ));
// 	flowpramlsf.P31 = round((qavg[4] - qavg[3]) / (dtavg[4] - dtavg[3] ));
// 	flowpramlsf.P41 = round((qavg[5] - qavg[4]) / (dtavg[5] - dtavg[4] ));
// 	flowpramlsf.P51 = round((qavg[6] - qavg[5]) / (dtavg[6] - dtavg[5] ));
// 	flowpramlsf.P61 = round((qavg[7] - qavg[6]) / (dtavg[7] - dtavg[6] ));
// 	flowpramlsf.P71 = round((qavg[8] - qavg[7]) / (dtavg[8] - dtavg[7] ));
	tmp =((qavg[0] - dtavg[0] * flowpramlsf.P01) + (qavg[1] - dtavg[1] * flowpramlsf.P01 ))/2;
	flowpramlsf.P02 = round(tmp);

	tmp =((qavg[1] - dtavg[1] * flowpramlsf.P11) + (qavg[2] - dtavg[2] * flowpramlsf.P11 ))/2;
	flowpramlsf.P12 = round(tmp);

	tmp =((qavg[2] - dtavg[2] * flowpramlsf.P21) + (qavg[3] - dtavg[3] * flowpramlsf.P21 ))/2;
	flowpramlsf.P22 = round(tmp);

// 	tmp =((qavg[3] - dtavg[3] * flowpramlsf.P31) + (qavg[4] - dtavg[4] * flowpramlsf.P31 ))/2;
// 	flowpramlsf.P32 = round(tmp);
// 
// 	tmp =((qavg[4] - dtavg[4] * flowpramlsf.P41) + (qavg[5] - dtavg[5] * flowpramlsf.P41 ))/2;
// 	flowpramlsf.P42 = round(tmp);
// 
// 	tmp =((qavg[5] - dtavg[5] * flowpramlsf.P51) + (qavg[6] - dtavg[6] * flowpramlsf.P51 ))/2;
// 	flowpramlsf.P52 = round(tmp);
// 
// 	tmp =((qavg[6] - dtavg[6] * flowpramlsf.P61 ) + (qavg[7] - dtavg[7] * flowpramlsf.P61))/2;
// 	flowpramlsf.P62 = round(tmp);
// 
// 	tmp =((qavg[7] - dtavg[7] * flowpramlsf.P71) + (qavg[8] - dtavg[8] * flowpramlsf.P71 ))/2;
// 	flowpramlsf.P72 = round(tmp);

}
//拟合2对二次曲线参数
void AlgrithmsTest::Fit3Paras()
{
	int i,j,k;
	double tmp;
	int everynums = 55;

	for (i = 0;i < 12;i ++ )
	{
		dtavg[i] = 0.0;
		qavg[i] = 0.0;
		tmpravg[i] = 0;
		for (j = (5 + i * 60);j < 60 * (i + 1);j++)
		{
			dtavg[i] += dt[j];
			qavg[i] += v[j];
			tmpravg[i] += tmprwater[j];
		}
		dtavg[i] /= everynums;
		qavg[i] /= everynums;
		tmpravg[i] /= everynums;
	}//计算55个数据的平均值
	PreDealData();	
	
	tmp = dtavg[0];
	for (i = 0;i < 12;i ++ )
	{	
		dtavg[i] -= tmp;
		dt[i] = dtavg[i];
		v[i] = qavg[i];
	}
	
	GenerateLSF(1,3);
	flowpramlsf.P00 = round(a[2] * 256);
	flowpramlsf.P01 = round(a[1]);
	flowpramlsf.P02 = round(a[0]);
// 	tmp = flowpramlsf.P00 * dtavg[1] *dtavg[1] / 256  + dtavg[1] * flowpramlsf.P01 + flowpramlsf.P02;//计算得到的第3点流量值（双滴/s）
// 	flowpramlsf.P02 += round(qavg[1] - tmp);
	GenerateLSF(4,3);
	flowpramlsf.P10 = round(a[2] * 256);
	flowpramlsf.P11 = round(a[1]);
	flowpramlsf.P12 = round(a[0]);
	tmp = flowpramlsf.P10 * dtavg[4] *dtavg[4] / 256 + dtavg[4] * flowpramlsf.P11 + flowpramlsf.P12;//计算得到的第一点流量值（双滴/s）
	flowpramlsf.P12 += round(qavg[4] - tmp);
	GenerateLSF(7,5);
	flowpramlsf.P20 = round(a[2] * 256);
	flowpramlsf.P21 = round(a[1]);
	flowpramlsf.P22 = round(a[0]);
	tmp = flowpramlsf.P20 * dtavg[8] *dtavg[8] / 256 + dtavg[8] * flowpramlsf.P21 + flowpramlsf.P22;//计算得到的第一点流量值（双滴/s）
	flowpramlsf.P22 += round(qavg[8] - tmp);
}
short AlgrithmsTest:: round(double num)
{
	float tmp = num - (int)num;
	if (tmp >= 0.5)
	{
		return (short)num+1;
	}
	else if (tmp <= -0.5)
	{
		return (short)num-1;
	}
	else
	{
		return short(num);
	}
}
//生成拟合曲线的系数，并计算SSE
void AlgrithmsTest::GenerateLSF(int NumStart,int NumCount)
{	
	int i,j,k,mi;//new_n为数据量
	double tmp,mx;
	for (i = 0;i <= PowerN_PolyNormal;i++)
	{
		a[i] = 0.0;
	}

	if (PowerN_PolyNormal > NumCount )//至少需要1个数据
	{
		FangCheng = _T("数据不够");//m
		SSE=0.0;
	} 
	else
	{	
		NumCount += NumStart;
		for(i = 0;i <= PowerN_PolyNormal;i++) //求矩阵乘数因子
		{ 
			for(j = i;j <= PowerN_PolyNormal;j++)  
			{  
				tmp = 0;  
				for(k = NumStart;k < NumCount;k++) 
				{
					tmp = tmp + pow(dt[k],(i+j)); 
				}
				x[i][j] = tmp;  
				x[j][i] = x[i][j]; 
			}  
		} 
		for(i = 0;i <= PowerN_PolyNormal;i++) //求矩阵因变量
		{  
			tmp = 0; 
			for(k = NumStart;k < NumCount;k++)  
			{
				tmp = tmp + v[k] * pow(dt[k],i); 
			}
			y[i] = tmp; 
		} 
		for(j = 0;j <= PowerN_PolyNormal - 1;j++) //求矩阵因变量和系数的增广矩阵
		{ 
			for(i = j+1,mi = j,mx = fabs(x[j][j]);i <= PowerN_PolyNormal;i++)  
			{
				if(fabs(x[i][j]) > mx) 
				{  
					mi = i;  
					mx = fabs(x[i][j]);  
				} 
				if(j < mi) //矩阵X和Y改变行号
				{   
					tmp = y[j];
					y[j] = y[mi]; 
					y[mi] = tmp;  
					for(k = j;k <= PowerN_PolyNormal;k++) 
					{  
						tmp = x[j][k]; 
						x[j][k] = x[mi][k];  
						x[mi][k] = tmp; 
					}
				}  
			}
			for(i = j+1;i <= PowerN_PolyNormal;i++)  //矩阵变换
			{  
				tmp = -x[i][j] / x[j][j]; 
				y[i] += y[j] * tmp; 
				for(k = j;k <= PowerN_PolyNormal;k++) 
				{
					x[i][k] += x[j][k] * tmp; 
				}
			}
		}  
		a[PowerN_PolyNormal] = y[PowerN_PolyNormal] / x[PowerN_PolyNormal][PowerN_PolyNormal]; 
		for(i = PowerN_PolyNormal-1;i >= 0;i--) 
		{  
			a[i] = y[i];
			for(j = i+1;j <= PowerN_PolyNormal;j++)
			{
				a[i] -= x[i][j] * a[j];
			}
			a[i] /= x[i][i]; 
		}		
		//求该拟合函数的SSE；m
		for (i = NumStart;i < NumCount ;i++)
		{
			double tmp=0;
			for ( j= 0;j <= PowerN_PolyNormal;j++)
			{		
				tmp += a[j] * pow(dt[i],j);
			}

			SSE += v[i] * v[i]-tmp * tmp; 
		}	
	} 
}
//误差计算
void AlgrithmsTest:: ErrorTest(long row)
{
	errnum = (ml_meter[row - 2] / 1000 - kg_weight[row - 2]) / kg_weight[row - 2] * 100;
}

//不计算dt=0和数据错误的点;
// void AlgrithmsTest::DealData(int err_nums,float x1,float x2)
// {
// 	int i,j,k;
// 	double tmp;
// 	for (i=0;i<Num-1;i++)//不计算dt=0的点;
// 	{
// 		tmp=dt[i];
// 		if (!tmp)
// 		{
// 			for(k=i;k<Num-1;k++)
// 			{
// 				dt[k] = dt[k+1];
// 				v[k]  = v[k+1];	
// 			}
// 			i--;
// 			Num--;			
// 		}
// 	}              //前Num-1个点计算基数
// 
// 	tmp=dt[Num-1];//最后一个点
// 	if (!tmp)
// 	{
// 		Num--;
// 	}//最后一个点计算结束
// 
// 
// 	for (i=0;i<err_nums;i++)//不计算值为一半的错误点
// 	{
// 		for (j=1;j<Num-1;j++)
// 		{
// 			if ((dt[j+1]>x1*dt[j])&&(dt[j+1]<x2*dt[j]))//后一个数据小
// 			{
// 				for (k=j+1;k<Num-1;k++)
// 				{
// 					dt[k]=dt[k+1];
// 					v[k] =v[k+1];
// 				}
// 				Num--;
// 			}
// 			else if ((dt[j-1]>x1*dt[j])&&(dt[j-1]<x2*dt[j]))//前一个数据小
// 			{
// 				for (k=j-1;k<Num;k++)
// 				{
// 					dt[k]=dt[k+1];
// 					v[k] =v[k+1];
// 				}
// 				Num--;
// 				j--;
// 			}
// 			else
// 			{	}
// 		}
// 	}
// }

//找出每20个dt数据中的最大值和最小值，并去除，然后求20个数的平均值
// void AlgrithmsTest::DealSelectData(int SeriesN)
// {
// 	int i,j,kmax,kmin,SeriesNTemp=SeriesN;
// 	double mindt,maxdt;
// 	double sumdt,sumv,sumtf;
// 	int NumArray=Num / SeriesN;
// 	int numremain=Num % SeriesN;
// 
// 	if (numremain>2)
// 	{
// 		NumArray++;
// 	}
// 	for (i=0;i<NumArray;i++)
// 	{
// 		mindt=dt[i*SeriesN];
// 		maxdt=dt[i*SeriesN];
// 
// 		sumdt=0;sumv=0;sumtf=0;
// 		if ((numremain>2)&&(i==NumArray-1))
// 		{
// 			SeriesN=numremain;
// 		}
// 		kmax=kmin=0;
// 		for (j=0;j<SeriesN;j++)
// 		{
// 
// 			if (mindt>dt[i*SeriesNTemp+j])
// 			{
// 				mindt=dt[i*SeriesNTemp+j];
// 				kmin=j;			
// 			}
// 			if (maxdt<dt[i*SeriesNTemp+j])
// 			{
// 				maxdt=dt[i*SeriesNTemp+j];
// 				kmax=j;
// 			}
// 			sumdt+=dt[i*SeriesNTemp+j];
// 			sumv+=v[i*SeriesNTemp+j];
// 			sumtf+=tf[i*SeriesNTemp+j];			
// 		}
// 		sumdt=sumdt-maxdt-mindt;
// 		sumv=sumv-v[i*SeriesNTemp+kmax]-v[i*SeriesNTemp+kmin];
// 		sumtf=sumtf-tf[i*SeriesNTemp+kmax]-tf[i*SeriesNTemp+kmin];
// 
// 		dt[i]=sumdt/(SeriesN-2);
// 		v[i]=sumv/(SeriesN-2);
// 		tf[i]=sumtf/(SeriesN-2);
// 	}
// 	Num=NumArray;
// }





// 飞行时间边界转换为流量边界
// float AlgrithmsTest::FlytimeToFlowrate(int t1,int t2)
// {
// 	int i;
// 	float flowrate = 0;
// 	for (i = 0; i < Num; i++)
// 	{
// 		if ((dt[i] > t1) && (dt[i] < t2))
// 		{
// 			if (flowrate < v[i])
// 			{
// 				flowrate = v[i];
// 			}			
// 		}
// 	}
// 	return flowrate;
// }
// 
// 除去误差最大的若干个点后得到剩余点个数
// int AlgrithmsTest::DealA0A1(int num_st,int nums,int errnum)
// {
// 	int i,j,tmp;
// 	if (nums <= errnum)
// 	{
// 		for (i = 0;i < nums;i++)
// 		{
// 			v[num_st + i] = 0;
// 		}
// 	}
// 	else
// 	{
// 		float df2[1200] = {0};
// 		tmp = 0;
// 		for(i = 0;i < nums;i++)
// 		{
// 			df2[num_st + i] = pow(dt[num_st + i] * a[1] + a[0] - v[num_st + i],2);
// 		}
// 		for (i = 0;i < errnum;i++)
// 		{
// 			for (j = 0;j < nums;j++)
// 			{
// 				if (df2[num_st + j] > df2[num_st + tmp])
// 				{
// 					tmp = j;
// 				}
// 			}
// 			for (j = tmp ;j < nums -1;j++ )
// 			{
// 				df2[j] = df2[j+1];
// 				v[j] =v[j+1];
// 				dt[j]=dt[j+1];
// 				tf[j]=tf[j+1];
// 				tft[j]=tft[j+1];
// 			}
// 			nums--;
// 		}
// 	}
// 	return nums;
// }
// 获取计算36个参数时的数据
// void AlgrithmsTest:: GetData36Paras(int dtline,int row)
// {
// 	Num = row - 1;
// 	int i;
// 	for(i = 0;i < Num;i++)  
// 	{
// 		dt[i] = GetCellDouble(i + 2 , dtline );
// 		v[i] = GetCellDouble(i + 2, 3) + GetCellDouble(i + 2 , 4);
// 		tmprwater[i] = GetCellDouble(i + 2, dtline + 2);//水温度
// 	}
// }
// 计算36个参数
// void AlgrithmsTest:: Calc36Paras()
// {
// 	int i,j = 0,k = 0;
// 	double dttmp = 0.0,vtmp = 0.0,tmpr = 0.0;
// 	double tmp[3][37] = {0,};//临时存储临界点数据
// 	for(i = 0;i < Num;i++)  
// 	{	
// 		if (j != 5)
// 		{
// 			dttmp += dt[i];
// 			vtmp += v[i];
// 			tmpr += tmprwater[i];
// 			j++;			
// 		} 
// 		else
// 		{
// 			tmp[0][k] = dttmp / 5.0 * 65536;
// 			tmp[1][k] = vtmp / 5.0 /** 10000 / 3600*/;		
// 			tmp[2][k] = tmpr / 5.0;
// 			dttmp = 0.0;
// 			vtmp = 0.0;
// 			tmpr = 0.0;
// 			j = 0;
// 			k++;
// 			i--;
// 		}		
// 	}
// 
// 	for (i = 0;i < 36;i++)
// 	{
// 		p[0][i] = (tmp[1][i + 1] -tmp[1][i]) / (tmp[0][i + 1] -tmp[0][i]) /*/ SquareC[ (int) (tmp[2][i] / 8)]*/;
// 		p[1][i] = tmp[1][i] - p[0][i] * tmp[0][i];//此函数连续，但是不是准确的在500倍数分界点隔开;
// 	}
// }