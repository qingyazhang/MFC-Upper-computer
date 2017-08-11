// ParaMeterTool.cpp : 实现文件
//

#include "stdafx.h"
#include "ParaMeterTool.h"
#include "afxdialogex.h"
#include "CommControl.h"
#include "include.h"
#include "string.h"
#include "Algorithms.h"

extern HANDLE Meterhdev;
extern SampleInfo sinfo;
FlowPram flowpramread;
AlgrithmsTest opexf;
OprtTime oprttime;
SampleInfo sinfocalp;
extern unsigned short timeremain;

long sample_n = 1;
CString ReverseMeterID();
CString str_ppp;

int Td_meter[10]={0,};
float flow_meter[10]={0,};
int para12_meter[10]={0,};
int para11_meter[10]={0,};
int para10_meter[10]={0,};
int tf_thr[746] = {\
	28422,28412,28401,28391,28381,28371,28361,28351,28341,28331,28321,28311,28301,28291,28281,28270,28262,28252,28242,28232,\
	28222,28212,28204,28194,28185,28175,28167,28157,28147,28139,28129,28119,28111,28101,28091,28081,28073,28063,28054,28046,\
	28036,28026,28018,28008,28000,27991,27983,27973,27965,27955,27947,27940,27930,27922,27912,27904,27897,27887,27879,27869,\
	27861,27854,27844,27836,27828,27819,27811,27803,27795,27786,27778,27770,27760,27753,27745,27735,27728,27720,27712,27703,\
	27695,27687,27680,27672,27664,27655,27647,27639,27632,27624,27616,27609,27601,27593,27586,27578,27570,27563,27555,27548,\
	27540,27532,27525,27517,27509,27502,27496,27489,27481,27473,27466,27458,27451,27445,27438,27430,27422,27415,27409,27402,\
	27394,27387,27379,27373,27366,27358,27351,27343,27338,27330,27323,27315,27310,27302,27297,27289,27282,27276,27269,27263,\
	27256,27248,27242,27235,27229,27222,27216,27209,27203,27196,27190,27183,27177,27170,27164,27157,27152,27144,27139,27131,\
	27126,27120,27113,27107,27100,27094,27089,27081,27076,27069,27063,27058,27050,27045,27039,27032,27026,27021,27015,27008,\
	27003,26997,26992,26986,26981,26973,26968,26962,26957,26951,26946,26940,26935,26928,26922,26917,26911,26906,26899,26893,\
	26888,26882,26877,26871,26866,26861,26855,26850,26844,26839,26833,26828,26823,26817,26812,26806,26803,26797,26792,26787,\
	26781,26776,26770,26765,26760,26754,26751,26745,26740,26734,26729,26724,26718,26715,26709,26704,26699,26693,26690,26684,\
	26679,26674,26670,26665,26659,26654,26650,26645,26640,26636,26631,26626,26622,26617,26613,26608,26602,26599,26594,26590,\
	26585,26579,26576,26570,26567,26562,26556,26553,26547,26544,26539,26533,26530,26525,26521,26516,26512,26507,26503,26498,\
	26495,26491,26486,26482,26477,26473,26470,26465,26461,26456,26452,26449,26444,26440,26437,26431,26428,26424,26421,26416,\
	26412,26409,26403,26400,26396,26391,26388,26384,26381,26375,26372,26368,26365,26360,26356,26353,26349,26346,26340,26337,\
	26333,26330,26327,26321,26318,26314,26311,26307,26302,26299,26295,26292,26288,26285,26281,26278,26274,26271,26268,26264,\
	26261,26257,26254,26250,26247,26243,26240,26236,26233,26230,26226,26223,26219,26216,26212,26209,26205,26202,26198,26195,\
	26192,26188,26185,26183,26180,26176,26173,26169,26168,26164,26161,26157,26154,26152,26149,26145,26142,26138,26137,26133,\
	26130,26126,26123,26121,26118,26114,26111,26108,26106,26102,26099,26096,26094,26090,26087,26084,26082,26078,26075,26073,\
	26070,26067,26065,26061,26060,26056,26053,26051,26048,26046,26043,26039,26038,26034,26032,26029,26026,26024,26021,26019,\
	26015,26012,26010,26007,26005,26002,26000,25997,25995,25992,25990,25988,25985,25983,25980,25978,25977,25973,25971,25968,\
	25966,25965,25961,25960,25956,25955,25953,25949,25948,25944,25943,25941,25938,25936,25933,25931,25929,25926,25924,25921,\
	25919,25917,25916,25912,25911,25909,25907,25906,25902,25901,25899,25897,25894,25892,25890,25887,25885,25884,25882,25879,\
	25877,25875,25874,25872,25870,25867,25865,25864,25862,25860,25859,25857,25855,25852,25850,25848,25847,25845,25842,25840,\
	25838,25837,25835,25833,25832,25830,25828,25827,25825,25823,25822,25820,25818,25817,25815,25812,25810,25808,25807,25805,\
	25803,25802,25800,25798,25797,25795,25795,25793,25792,25790,25788,25787,25785,25783,25782,25780,25780,25778,25777,25775,\
	25773,25772,25770,25768,25767,25765,25765,25763,25762,25760,25758,25757,25755,25755,25753,25752,25750,25748,25748,25747,\
	25745,25743,25742,25742,25740,25738,25737,25735,25735,25733,25732,25730,25730,25728,25727,25725,25725,25723,25722,25722,\
	25720,25718,25718,25717,25715,25713,25713,25712,25710,25710,25708,25707,25707,25705,25705,25703,25702,25702,25700,25700,\
	25698,25697,25697,25695,25695,25693,25692,25692,25690,25690,25689,25687,25687,25685,25685,25684,25684,25682,25682,25680,\
	25680,25679,25679,25677,25677,25675,25675,25674,25674,25672,25672,25670,25670,25669,25669,25667,25667,25665,25665,25664,\
	25664,25664,25662,25662,25660,25660,25660,25659,25659,25657,25657,25657,25655,25655,25655,25654,25654,25654,25654,25652,\
	25652,25652,25650,25650,25650,25649,25649,25649,25649,25647,25647,25647,25646,25646,25646,25644,25644,25644,25644,25642,\
	25642,25642,25642,25641,25641,25641,25641,25641,25639,25639,25639,25639,25639,25637,25637,25637,25637,25637,25636,25636,\
	25636,25636,25636,25634,25634,25634,25634,25634,25632,25632,25632,25632,25632,25632,25632,25631,25631,25631,25631,25631,\
	25631,25631,25631,25631,25631,25631,25631,25631,25631,25631,25631,25631,25631,25631,25631,25629,25629,25629,25629,25629,\
	25629,25629,25629,25629,25629,25629};

// ParaMeterTool 对话框

IMPLEMENT_DYNAMIC(ParaMeterTool, CDialogEx)

	ParaMeterTool::ParaMeterTool(CWnd* pParent /*=NULL*/)
	: CDialogEx(ParaMeterTool::IDD, pParent)
{

}

ParaMeterTool::~ParaMeterTool()
{
}

void ParaMeterTool::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_combo1);
	DDX_Control(pDX, IDC_EDIT17,displaycalc36p);	
}


BEGIN_MESSAGE_MAP(ParaMeterTool, CDialogEx)

	ON_BN_CLICKED(IDC_BUTTON1, &ParaMeterTool::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &ParaMeterTool::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &ParaMeterTool::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &ParaMeterTool::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &ParaMeterTool::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON7, &ParaMeterTool::OnBnClickedButton7)
	/*ON_BN_CLICKED(IDC_BUTTON8, &ParaMeterTool::OnBnClickedButton8)*/
	ON_BN_CLICKED(IDC_BUTTON34, &ParaMeterTool::OnBnClickedButton34)
	ON_BN_CLICKED(IDC_BUTTON35, &ParaMeterTool::OnBnClickedButton35)
	ON_BN_CLICKED(IDC_BUTTON36, &ParaMeterTool::OnBnClickedButton36)
	ON_BN_CLICKED(IDC_BUTTON37, &ParaMeterTool::OnBnClickedButton37)
	ON_BN_CLICKED(IDC_BUTTON38, &ParaMeterTool::OnBnClickedButton38)
	ON_BN_CLICKED(IDC_BUTTON40, &ParaMeterTool::OnBnClickedButton40)
	ON_BN_CLICKED(IDC_BUTTON39, &ParaMeterTool::OnBnClickedButton39)
END_MESSAGE_MAP()


// ParaMeterTool 消息处理程序
void ParaMeterTool::OnOK()
{

}

BOOL ParaMeterTool::StrToChar(CString str, UCHAR *dat)
{
	UCHAR count;

	for(count = 0; count < str.GetLength(); count++)
	{
		if(str[count] >= '0' && str[count] <= '9')
		{
			if(count % 2)
			{
				*dat += str[count] - '0';
				dat++;
			}
			else
			{
				*dat = (str[count] - '0') * 0x10;
			}
		}
		else if(str[count] >= 'A' && str[count] <= 'F')
		{
			if(count % 2)
			{
				*dat += str[count] - 'A' + 10;
				dat++;
			}
			else
			{
				*dat = (str[count] - 'A' + 10) * 0x10;
			}
		}
		else
		{
			return FALSE;
		}
	}
	return TRUE;
}

CString ParaMeterTool::CharToStr(UCHAR *dat, USHORT len)
{
	CString temp;
	UCHAR buffer[10];
	CString str;

	str = "";
	while(len--)
	{
		temp = itoa(*dat, (char *)buffer, 16);
		if(temp.GetLength() == 1)
		{
			temp = _T("0") + temp;
		}
		str += temp;
		dat++;
	}
	str.MakeUpper();
	return str;
}

CString ParaMeterTool::ReverseMeterID(int meternum)
{
	char t_str[14];
	t_str[12] = (sinfo.minfo[meternum].meterid[0]>>4) + '0';
	t_str[13] = (sinfo.minfo[meternum].meterid[0]&0x0f) + '0';
	t_str[10] = (sinfo.minfo[meternum].meterid[1]>>4) + '0';
	t_str[11] = (sinfo.minfo[meternum].meterid[1]&0x0f) + '0';
	t_str[8] = (sinfo.minfo[meternum].meterid[2]>>4) + '0';
	t_str[9] = (sinfo.minfo[meternum].meterid[2]&0x0f) + '0';
	t_str[6] = (sinfo.minfo[meternum].meterid[3]>>4) + '0';
	t_str[7] = (sinfo.minfo[meternum].meterid[3]&0x0f) + '0';
	t_str[4] = (sinfo.minfo[meternum].meterid[4]>>4) + '0';
	t_str[5] = (sinfo.minfo[meternum].meterid[4]&0x0f) + '0';
	t_str[2] = (sinfo.minfo[meternum].meterid[5]>>4) + '0';
	t_str[3] = (sinfo.minfo[meternum].meterid[5]&0x0f) + '0';
	t_str[0] = (sinfo.minfo[meternum].meterid[6]>>4) + '0';
	t_str[1] = (sinfo.minfo[meternum].meterid[6]&0x0f) + '0';

	CString str = (CString)t_str;
	return str.Left(14);
}


//初始化
BOOL ParaMeterTool::OnInitDialog()
{
	CString str;
	CDialogEx::OnInitDialog();
	m_combo1.AddString(_T("所有表"));
	m_combo1.SetCurSel(0);

	SetDlgItemInt(IDC_EDIT1,0);
	SetDlgItemInt(IDC_EDIT2,0);
	SetDlgItemInt(IDC_EDIT3,0);
	SetDlgItemInt(IDC_EDIT4,0);
	SetDlgItemInt(IDC_EDIT5,0);
	SetDlgItemInt(IDC_EDIT6,0);
	SetDlgItemInt(IDC_EDIT7,0);
	SetDlgItemInt(IDC_EDIT8,0);
	SetDlgItemInt(IDC_EDIT9,0);
// 	SetDlgItemInt(IDC_EDIT18,0);
// 	SetDlgItemInt(IDC_EDIT19,0);
// 	SetDlgItemInt(IDC_EDIT20,0);
// 	SetDlgItemInt(IDC_EDIT21,0);
// 	SetDlgItemInt(IDC_EDIT22,0);
// 	SetDlgItemInt(IDC_EDIT23,0);
// 	SetDlgItemInt(IDC_EDIT24,0);
	SetDlgItemInt(IDC_EDIT25,0);
	SetDlgItemInt(IDC_EDIT23,10);
	SetDlgItemInt(IDC_EDIT12,sample_n);

	return TRUE;
}

//读参数
void ParaMeterTool::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	int i;
	int item;
	CString com;
	CString AllMeter;
	AllMeter = _T("所有表");
	
	item = m_combo1.GetCurSel();
	m_combo1.GetLBText(item, com);

	if (com == AllMeter)
	{
		AfxMessageBox(_T("请读单只表!")); 
		return;
	}
	else 
	{		
		for (i = 0;i < sinfo.meternum;i++)
		{
			AllMeter = ReverseMeterID(i);
			if (com == AllMeter)
			{
				//WriteMeterState((int)sinfo.minfo[i].mhandle,sinfo.minfo[i].meterid,3600);
				if(!ReadFlowP((int)sinfo.minfo[i].mhandle, sinfo.minfo[i].meterid,(unsigned char*)&flowpramread))
				{
					AllMeter += _T("读取失败!");
					AfxMessageBox(AllMeter); 
					return;
				}
				SetDlgItemInt(IDC_EDIT1, flowpramread.P00);
				SetDlgItemInt(IDC_EDIT2, flowpramread.P01);
				SetDlgItemInt(IDC_EDIT3, flowpramread.P02);
				SetDlgItemInt(IDC_EDIT4, flowpramread.P10);
				SetDlgItemInt(IDC_EDIT5, flowpramread.P11);
				SetDlgItemInt(IDC_EDIT6, flowpramread.P12);	
 				SetDlgItemInt(IDC_EDIT7, flowpramread.P20);
 				SetDlgItemInt(IDC_EDIT8, flowpramread.P21);
 				SetDlgItemInt(IDC_EDIT9, flowpramread.P22);
// 				SetDlgItemInt(IDC_EDIT18, flowpramread.P42);
// 				SetDlgItemInt(IDC_EDIT19, flowpramread.P51);
// 				SetDlgItemInt(IDC_EDIT20, flowpramread.P52);	
// 				SetDlgItemInt(IDC_EDIT21, flowpramread.P61);
// 				SetDlgItemInt(IDC_EDIT22, flowpramread.P62);
// 				SetDlgItemInt(IDC_EDIT23, flowpramread.P71);
// 				SetDlgItemInt(IDC_EDIT24, flowpramread.P72);
				SetDlgItemInt(IDC_EDIT25, flowpramread.Td);
			}
		}
	}
}

//写数据
void ParaMeterTool::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	int i;
	int item;
	CString com;
	CString AllMeter;
	AllMeter = _T("所有表");

	item = m_combo1.GetCurSel();
	m_combo1.GetLBText(item, com);

	if (com == AllMeter)
	{
		for (i = 0;i < sinfo.meternum;i++)
		{
			BOOL flag;
			sinfo.minfo[i].flowpram.P00 = GetDlgItemInt(IDC_EDIT1,&flag,TRUE);
			sinfo.minfo[i].flowpram.P01 = GetDlgItemInt(IDC_EDIT2,&flag,TRUE);
			sinfo.minfo[i].flowpram.P02 = GetDlgItemInt(IDC_EDIT3,&flag,TRUE);
			sinfo.minfo[i].flowpram.P10 = GetDlgItemInt(IDC_EDIT4,&flag,TRUE);
			sinfo.minfo[i].flowpram.P11 = GetDlgItemInt(IDC_EDIT5,&flag,TRUE);
			sinfo.minfo[i].flowpram.P12 = GetDlgItemInt(IDC_EDIT6,&flag,TRUE);
 			sinfo.minfo[i].flowpram.P20 = GetDlgItemInt(IDC_EDIT7,&flag,TRUE);
 			sinfo.minfo[i].flowpram.P21 = GetDlgItemInt(IDC_EDIT8,&flag,TRUE);
 			sinfo.minfo[i].flowpram.P22 = GetDlgItemInt(IDC_EDIT9,&flag,TRUE);
// 			sinfo.minfo[i].flowpram.P42 = GetDlgItemInt(IDC_EDIT18,&flag,TRUE);
// 			sinfo.minfo[i].flowpram.P51 = GetDlgItemInt(IDC_EDIT19,&flag,TRUE);
// 			sinfo.minfo[i].flowpram.P52 = GetDlgItemInt(IDC_EDIT20,&flag,TRUE);
// 			sinfo.minfo[i].flowpram.P61 = GetDlgItemInt(IDC_EDIT21,&flag,TRUE);
// 			sinfo.minfo[i].flowpram.P62 = GetDlgItemInt(IDC_EDIT22,&flag,TRUE);
// 			sinfo.minfo[i].flowpram.P71 = GetDlgItemInt(IDC_EDIT23,&flag,TRUE);
// 			sinfo.minfo[i].flowpram.P72 = GetDlgItemInt(IDC_EDIT24,&flag,TRUE);
 			sinfo.minfo[i].flowpram.Td  = GetDlgItemInt(IDC_EDIT25,&flag,TRUE);

			if (!WriteFlowP((int)sinfo.minfo[i].mhandle,sinfo.minfo[i].meterid,(unsigned char*)&sinfo.minfo[i].flowpram,(unsigned char*)&oprttime))
			{	
				AllMeter = CharToStr(sinfo.minfo[i].meterid,7);
				AllMeter += _T("写失败！");
				AfxMessageBox(AllMeter); 
				return;
			}
			else
			{
				Sleep(500);
				ReadFlowP((int)sinfo.minfo[i].mhandle, sinfo.minfo[i].meterid,(unsigned char*)&flowpramread);
				unsigned char* pt_tmp1;
				unsigned char* pt_tmp2;
				pt_tmp1 = (unsigned char*)&flowpramread;
				pt_tmp2 = (unsigned char*)&sinfo.minfo[i].flowpram;
				if (pt_tmp1[0]!= pt_tmp2[1] || pt_tmp1[1] != pt_tmp2[0]||\
					pt_tmp1[2]!= pt_tmp2[3] || pt_tmp1[3] != pt_tmp2[2]||\
					pt_tmp1[4]!= pt_tmp2[5] || pt_tmp1[5] != pt_tmp2[4]||\
					pt_tmp1[6]!= pt_tmp2[7] || pt_tmp1[7] != pt_tmp2[6]||\
					pt_tmp1[8]!= pt_tmp2[9] || pt_tmp1[9] != pt_tmp2[8]||\
					pt_tmp1[10]!= pt_tmp2[11] || pt_tmp1[11] != pt_tmp2[10]||\
					pt_tmp1[18]!= pt_tmp2[19] || pt_tmp1[19] != pt_tmp2[18]					
				// 						pt_tmp1[12]!= pt_tmp2[13] || pt_tmp1[13] != pt_tmp2[12]||\
				// 						pt_tmp1[14]!= pt_tmp2[15] || pt_tmp1[15] != pt_tmp2[14]||\
				// 						pt_tmp1[16]!= pt_tmp2[17] || pt_tmp1[17] != pt_tmp2[16]||
				)
				{
					AllMeter += _T("写错误！");
					AfxMessageBox(AllMeter); 
					return;
				}
				else
				{
					AfxMessageBox(_T("写操作完成"));
				}
			}					
		}		
	}
	else
	{		
		for (i = 0;i < sinfo.meternum;i++)
		{	
			AllMeter = ReverseMeterID(i);
			if (AllMeter == com)
			{			
				BOOL flag;
				sinfo.minfo[i].flowpram.P00 = GetDlgItemInt(IDC_EDIT1,&flag,TRUE);
				sinfo.minfo[i].flowpram.P01 = GetDlgItemInt(IDC_EDIT2,&flag,TRUE);
				sinfo.minfo[i].flowpram.P02 = GetDlgItemInt(IDC_EDIT3,&flag,TRUE);
				sinfo.minfo[i].flowpram.P10 = GetDlgItemInt(IDC_EDIT4,&flag,TRUE);
				sinfo.minfo[i].flowpram.P11 = GetDlgItemInt(IDC_EDIT5,&flag,TRUE);
				sinfo.minfo[i].flowpram.P12 = GetDlgItemInt(IDC_EDIT6,&flag,TRUE);
				sinfo.minfo[i].flowpram.P20 = GetDlgItemInt(IDC_EDIT7,&flag,TRUE);
				sinfo.minfo[i].flowpram.P21 = GetDlgItemInt(IDC_EDIT8,&flag,TRUE);
				sinfo.minfo[i].flowpram.P22 = GetDlgItemInt(IDC_EDIT9,&flag,TRUE);
// 				sinfo.minfo[i].flowpram.P42 = GetDlgItemInt(IDC_EDIT18,&flag,TRUE);
// 				sinfo.minfo[i].flowpram.P51 = GetDlgItemInt(IDC_EDIT19,&flag,TRUE);
// 				sinfo.minfo[i].flowpram.P52 = GetDlgItemInt(IDC_EDIT20,&flag,TRUE);
// 				sinfo.minfo[i].flowpram.P61 = GetDlgItemInt(IDC_EDIT21,&flag,TRUE);
// 				sinfo.minfo[i].flowpram.P62 = GetDlgItemInt(IDC_EDIT22,&flag,TRUE);
// 				sinfo.minfo[i].flowpram.P71 = GetDlgItemInt(IDC_EDIT23,&flag,TRUE);
// 				sinfo.minfo[i].flowpram.P72 = GetDlgItemInt(IDC_EDIT24,&flag,TRUE);
				sinfo.minfo[i].flowpram.Td  = GetDlgItemInt(IDC_EDIT25,&flag,TRUE);

				if (!WriteFlowP((int)sinfo.minfo[i].mhandle,sinfo.minfo[i].meterid,(unsigned char*)&sinfo.minfo[i].flowpram,(unsigned char*)&oprttime))
				{	
					AllMeter = ReverseMeterID(i);
					AllMeter += _T("写失败！");
					AfxMessageBox(AllMeter); 
					return;
				}
				else
				{
					Sleep(500);
					ReadFlowP((int)sinfo.minfo[i].mhandle, sinfo.minfo[i].meterid,(unsigned char*)&flowpramread);
					unsigned char* pt_tmp1;
					unsigned char* pt_tmp2;
					pt_tmp1 = (unsigned char*)&flowpramread;
					pt_tmp2 = (unsigned char*)&sinfo.minfo[i].flowpram;
					if (pt_tmp1[0]!= pt_tmp2[1] || pt_tmp1[1] != pt_tmp2[0]||\
						pt_tmp1[2]!= pt_tmp2[3] || pt_tmp1[3] != pt_tmp2[2]||\
						pt_tmp1[4]!= pt_tmp2[5] || pt_tmp1[5] != pt_tmp2[4]||\
						pt_tmp1[6]!= pt_tmp2[7] || pt_tmp1[7] != pt_tmp2[6]||\
						pt_tmp1[8]!= pt_tmp2[9] || pt_tmp1[9] != pt_tmp2[8]||\
						pt_tmp1[10]!= pt_tmp2[11] || pt_tmp1[11] != pt_tmp2[10]||\
						pt_tmp1[18]!= pt_tmp2[19] || pt_tmp1[19] != pt_tmp2[18]					
// 						pt_tmp1[12]!= pt_tmp2[13] || pt_tmp1[13] != pt_tmp2[12]||\
// 						pt_tmp1[14]!= pt_tmp2[15] || pt_tmp1[15] != pt_tmp2[14]||\
// 						pt_tmp1[16]!= pt_tmp2[17] || pt_tmp1[17] != pt_tmp2[16]||
					)
// 					if (flowpramread.P00 != EndianConvert16((unsigned char*)&sinfo.minfo[i].flowpram.P00) ||\
// 						flowpramread.P01 != EndianConvert16((unsigned char*)&sinfo.minfo[i].flowpram.P01) ||\
// 						flowpramread.P02 != EndianConvert16((unsigned char*)&sinfo.minfo[i].flowpram.P02) ||\
// 						flowpramread.P10 != EndianConvert16((unsigned char*)&sinfo.minfo[i].flowpram.P10) ||\
// 						flowpramread.P11 != EndianConvert16((unsigned char*)&sinfo.minfo[i].flowpram.P11) ||\
// 						flowpramread.P12 != EndianConvert16((unsigned char*)&sinfo.minfo[i].flowpram.P12) ||\
// 						flowpramread.P20 != EndianConvert16((unsigned char*)&sinfo.minfo[i].flowpram.P20) ||\
// 						flowpramread.P21 != EndianConvert16((unsigned char*)&sinfo.minfo[i].flowpram.P21) ||\
// 						flowpramread.P22 != EndianConvert16((unsigned char*)&sinfo.minfo[i].flowpram.P22) ||\
// 						flowpramread.Td  != EndianConvert16((unsigned char*)&sinfo.minfo[i].flowpram.Td)
// 						// 					||flowpramread.P51 != sinfo.minfo[i].flowpram.P51 || flowpramread.P52 != sinfo.minfo[i].flowpram.P52\
// 						// 					||flowpramread.P61 != sinfo.minfo[i].flowpram.P61 || flowpramread.P62 != sinfo.minfo[i].flowpram.P62\
// 						// 					||flowpramread.P71 != sinfo.minfo[i].flowpram.P71 || flowpramread.P72 != sinfo.minfo[i].flowpram.P72
// 						)
					{
						AllMeter += _T("写错误！");
						AfxMessageBox(AllMeter); 
						return;
					}	
					else
					{
						AfxMessageBox(_T("写操作完成"));
					}
				}				
			}			
		}
	}
}

//计算参数
void ParaMeterTool::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	int i,j;
	int item;
	CString com;
	CString AllMeter;
	
	AllMeter = _T("所有表");
	sample_n = GetDlgItemInt(IDC_EDIT12);

	opexf.InitExcel();
	opexf.OpenWithoutSheet(_T("\\sample.xls"));
	opexf.LoadSheet(sample_n,TRUE);

	item = m_combo1.GetCurSel();
	m_combo1.GetLBText(item, com);

	Sleep(2000);
	
	long row = opexf.GetRowCount();

	if (com == AllMeter)
	{
		CString str,str_t;
		for(i = 0; i < sinfo.meternum; i++)
		{
 			opexf.GetData(row,13 + i * 5,15 + i * 5);
			/*opexf.Calc8Paras();*/
			opexf.Fit3Paras();

 			sinfo.minfo[i].flowpram=opexf.flowpramlsf;	
			AllMeter = opexf.GetCellString(1,13 + i * 5);
			str_t.Format(_T("%s %s %s"),AllMeter.Left(14), _T("的参数为："),_T("\r\n"));
			str +=  str_t ;
			str_t.Format(_T("%d + %d + %d %s"),sinfo.minfo[i].flowpram.P00,sinfo.minfo[i].flowpram.P01,sinfo.minfo[i].flowpram.P02,_T("\r\n"));
			str += str_t;
			str_t.Format(_T("%d + %d + %d %s"),sinfo.minfo[i].flowpram.P10,sinfo.minfo[i].flowpram.P11,sinfo.minfo[i].flowpram.P12,_T("\r\n"));
			str += str_t;
			str_t.Format(_T("%d + %d + %d %s"),sinfo.minfo[i].flowpram.P20,sinfo.minfo[i].flowpram.P21,sinfo.minfo[i].flowpram.P22,_T("\r\n"));
			str += str_t;
		}
		SetDlgItemText(IDC_EDIT17,str);
	}
	else
	{
		for (i = 0;i < sinfo.meternum;i++)
		{	
			AllMeter = opexf.GetCellString(1,13 + i * 5);	
			if (AllMeter.Left(14) == com)
			{
				opexf.GetData(row,13 + i * 5,15 + i * 5);
				/*opexf.Calc8Paras();*/
				opexf.Fit3Paras();
				sinfo.minfo[i].flowpram=opexf.flowpramlsf;
				SetDlgItemInt(IDC_EDIT1, sinfo.minfo[i].flowpram.P00);
				SetDlgItemInt(IDC_EDIT2, sinfo.minfo[i].flowpram.P01);
				SetDlgItemInt(IDC_EDIT3, sinfo.minfo[i].flowpram.P02);
				SetDlgItemInt(IDC_EDIT4, sinfo.minfo[i].flowpram.P10);
				SetDlgItemInt(IDC_EDIT5, sinfo.minfo[i].flowpram.P11);
				SetDlgItemInt(IDC_EDIT6, sinfo.minfo[i].flowpram.P12);	
 				SetDlgItemInt(IDC_EDIT7, sinfo.minfo[i].flowpram.P20);
 				SetDlgItemInt(IDC_EDIT8, sinfo.minfo[i].flowpram.P21);
 				SetDlgItemInt(IDC_EDIT9, sinfo.minfo[i].flowpram.P22);
// 				SetDlgItemInt(IDC_EDIT18, sinfo.minfo[i].flowpram.P42);
// 				SetDlgItemInt(IDC_EDIT19, sinfo.minfo[i].flowpram.P51);
// 				SetDlgItemInt(IDC_EDIT20, sinfo.minfo[i].flowpram.P52);
// 				SetDlgItemInt(IDC_EDIT21, sinfo.minfo[i].flowpram.P61);
// 				SetDlgItemInt(IDC_EDIT22, sinfo.minfo[i].flowpram.P62);	
// 				SetDlgItemInt(IDC_EDIT23, sinfo.minfo[i].flowpram.P71);
// 				SetDlgItemInt(IDC_EDIT24, sinfo.minfo[i].flowpram.P72);
				SetDlgItemInt(IDC_EDIT25, sinfo.minfo[i].flowpram.Td);
			}			
		}
	}
	opexf.SaveExcel();
	opexf.CloseExcelFile();
	opexf.ReleaseExcel();
}

//误差计算
void ParaMeterTool::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码
	int i;
	int item;
	CString com;
	CString AllMeter;

	AllMeter = _T("所有表");
	sample_n = GetDlgItemInt(IDC_EDIT12);

	opexf.InitExcel();
	opexf.OpenWithoutSheet(_T("\\sample.xls"));
	opexf.LoadSheet(sample_n,TRUE);

	item = m_combo1.GetCurSel();
	m_combo1.GetLBText(item, com);

	Sleep(2000);

	long row = opexf.GetRowCount();

	if (com == AllMeter)
	{
		CString str,str_t;
		for(i = 0; i< sinfo.meternum; i++)
		{
			opexf.GetData(row,13 + i * 5,15 + i * 5);
			opexf.ErrorTest(row);
			AllMeter = opexf.GetCellString(1,13 + i * 5);
			str_t.Format(_T("%s %s %s"),AllMeter.Left(14), _T("的误差为："),_T("\r\n"));
			str +=  str_t ;
			str_t.Format(_T("%.2f "),opexf.errnum);
			str += str_t;
			str_t.Format(_T(" %s"),_T("\r\n"));
			str += str_t;
		}
		SetDlgItemText(IDC_EDIT17,str);
	}
	else
	{
		for (i = 0; i< sinfo.meternum; i++)
		{
			CString str;
			AllMeter = opexf.GetCellString(1,13 + i * 5);
			if (com == AllMeter.Left(14))
			{
				opexf.GetData(row,13 + i * 5,15 + i * 5);
				opexf.ErrorTest(row);
				str.Format(_T("%.2f"),opexf.errnum);
				SetDlgItemText(IDC_EDIT13,str);
			}
		}
	}
	opexf.SaveExcel();
	opexf.CloseExcelFile();
	opexf.ReleaseExcel();
}

//进入校表
void ParaMeterTool::OnBnClickedButton5()
{
	// TODO: 在此添加控件通知处理程序代码
	int i;
	int item;
	CString com;
	CString AllMeter;

	AllMeter = _T("所有表");

	item = m_combo1.GetCurSel();
	m_combo1.GetLBText(item, com);

	if (com == AllMeter)
	{
		for(i = 0; i < sinfo.meternum; i++)
		{			
			if(WriteMeterState((int)sinfo.minfo[i].mhandle,sinfo.minfo[i].meterid,3600))
			{					
			}
			else
			{
				AllMeter = ReverseMeterID(i);
				AllMeter += _T("未进入校表!");
				AfxMessageBox(AllMeter);
			}
		}
	}		
	else //单只表
	{
		for (i = 0; i< sinfo.meternum; i++)
		{
			AllMeter = ReverseMeterID(i);
			if (com == AllMeter)
			{
				if(WriteMeterState((int)sinfo.minfo[i].mhandle,sinfo.minfo[i].meterid,3600))
				{
				}
				else
				{
					AllMeter += _T("未进入校表!");
					AfxMessageBox(AllMeter);
				}
			}
		}		
	}		
}
//表号加载
void ParaMeterTool::OnBnClickedButton7()
{
	// TODO: 在此添加控件通知处理程序代码
	int i,j;
	CString str;
	char t_str[14];
	m_combo1.Clear();
	int item_str = m_combo1.GetCount();
	for (i = item_str;i > 0 ;i--)
	{
		m_combo1.DeleteString(i);
	}
	
	for (i = 0;i < sinfo.meternum;i++)
	{
		str = ReverseMeterID(i);		
		m_combo1.AddString(str);
	}
	m_combo1.SetCurSel(0);
}

//读流速
void ParaMeterTool::OnBnClickedButton34()
{
	// TODO: 在此添加控件通知处理程序代码
	int i;
	int item;
	CString com;
	CString AllMeter;
	AllMeter = _T("所有表");

	item = m_combo1.GetCurSel();
	m_combo1.GetLBText(item, com);

	if (com == AllMeter)
	{
		AfxMessageBox(_T("请读单只表!")); 
		return;
	}
	else 
	{		
		for (i = 0;i < sinfo.meternum;i++)
		{
			AllMeter = ReverseMeterID(i);
			if (com == AllMeter)
			{
				if(!ReadFlow_meter((int)sinfo.minfo[i].mhandle, sinfo.minfo[i].meterid,(signed char*)&(sinfo.minfo[i].flow)))
				{
					AllMeter += _T("读取失败!");
					AfxMessageBox(AllMeter); 
					return;
				}
				else
				{
					SetDlgItemInt(IDC_EDIT19,sinfo.minfo[i].flow * 0.36);
				}				
	
			}
		}
	}
}

//计算P13及Td自动取值计算第一波飞行时间tf0,即P13
void ParaMeterTool::OnBnClickedButton35()
{
	// TODO: 在此添加控件通知处理程序代码
	int i,j,k;
	int sample_n_tmp;
	int tf_meter[10]={0,};
	int tmpr_meter[10]={0,};
	int dt_meter[10]={0,};
	CString t_str_ppp;
	sample_n_tmp = GetDlgItemInt(IDC_EDIT23);

	for (i = 0;i < sample_n_tmp;i++)
	{
		for (j = 0;j < sinfo.meternum;j++)
		{			
			if(!ReadMeterFly((int)sinfo.minfo[j].mhandle,sinfo.minfo[j].meterid, \
				(unsigned char*)&(sinfo.minfo[j].ml), (unsigned char*)&(sinfo.minfo[j].dt), (unsigned char*)&(sinfo.minfo[j].tf)))
			{
				i--;
				for (k =0;k < sinfo.meternum;k++)
				{
					sinfo.minfo[k].dt = 0;
					sinfo.minfo[k].tf = 0;
					sinfo.minfo[k].tmpr1 = 0;
				}
				break;
			}
			else if (!ReadStateTmpr((int)sinfo.minfo[j].mhandle,sinfo.minfo[j].meterid,\
				(signed short*)&(sinfo.minfo[j].tmpr1),(signed short*)&(sinfo.minfo[j].tmpr2),(unsigned short*)&timeremain))
			{
				i--;
				for (k =0;k < sinfo.meternum;k++)
				{
					sinfo.minfo[k].dt = 0;
					sinfo.minfo[k].tf = 0;
					sinfo.minfo[k].tmpr1 = 0;
				}				
				break;
			}
			else
			{
				
			}				
		}
		for (k = 0;k < sinfo.meternum;k++)
		{
			dt_meter[k] +=sinfo.minfo[k].dt;
			tf_meter[k] += sinfo.minfo[k].tf;
			tmpr_meter[k] += sinfo.minfo[k].tmpr1;
		}
		Sleep(1000);
	}
	
	for (j = 0;j < sinfo.meternum;j++)
	{
		Td_meter[j] = (int)(dt_meter[j] /sample_n_tmp * 256);
		tf_meter[j] = (int)(tf_meter[j] /sample_n_tmp);
		tmpr_meter[j] = (int)(tmpr_meter[j] /sample_n_tmp);
		para12_meter[j] = tf_meter[j] - tf_thr[tmpr_meter[j]] + 25638;
	}
	//显示全部计算得到的参数
	GetDlgItemText(IDC_EDIT17,str_ppp);
	t_str_ppp.Format(_T("%s	%s	%s%s"),_T("表          号"),_T("P12"),_T("Td"),_T("\r\n"));
	str_ppp += t_str_ppp;
	for (i = 0;i<sinfo.meternum;i++)
	{
		t_str_ppp.Format(_T("%s	%d	%d%s"),ReverseMeterID(i),para12_meter[i],Td_meter[i],_T("\r\n"));
		str_ppp += t_str_ppp;
	}
	SetDlgItemText(IDC_EDIT17,str_ppp);
	i = ((CEdit*)GetDlgItem(IDC_EDIT17))->GetLineCount();
	((CEdit*)GetDlgItem(IDC_EDIT17))->LineScroll(i,0);
}

//计算P12
void ParaMeterTool::OnBnClickedButton36()
{
	// TODO: 在此添加控件通知处理程序代码
	int i,j,k;
	int sample_n_tmp;
	sample_n_tmp = GetDlgItemInt(IDC_EDIT23);
	float flow_tmp=0;
	float flow_calmeter=0;
	CString t_str_ppp;
	
	for (j = 0;j < sinfo.meternum;j++)
	{
		flow_meter[j] = 0;
	}	
	for (i = 0;i < sample_n_tmp;i++)
	{
		flow_tmp = sinfo.mflow + sinfo.lflow;
		if (flow_tmp ==0)
		{
			i--;
			continue;
		}
		for (j = 0;j < sinfo.meternum;j++)
		{			
			if(!ReadFlow_meter((int)sinfo.minfo[j].mhandle, sinfo.minfo[j].meterid,(signed char*)&(sinfo.minfo[j].flow)))
			{
				i--;
				for (k = 0;k < sinfo.meternum;k++)
				{
					sinfo.minfo[k].flow = 0;
				}
				flow_tmp = 0;
				break;
			}			
			else 
			{
				
			}			
		}
		flow_calmeter += flow_tmp;
		for (k = 0;k < sinfo.meternum;k++)
		{
			flow_meter[k]  += sinfo.minfo[k].flow * 0.36;			
		}
		Sleep(1000);	
	}
	flow_calmeter = flow_calmeter/sample_n_tmp;
	//获取当前参数
	for (j = 0;j < sinfo.meternum;j++)
	{
		if(!ReadFlowP((int)sinfo.minfo[j].mhandle, sinfo.minfo[j].meterid,(unsigned char*)&sinfo.minfo[j].flowpram))
		{
			j--;
		}
		else
		{
			flow_meter[j] = flow_meter[j] /sample_n_tmp;			
			if (flow_meter[j]!=0)
			{
				para11_meter[j] = (int)((float)sinfo.minfo[j].flowpram.P11/(flow_meter[j] / flow_calmeter));
			}
			else
			{
				para11_meter[j] = 0;
			}			
		}		
	}
	//显示全部计算得到的参数
	GetDlgItemText(IDC_EDIT17,str_ppp);
	t_str_ppp.Format(_T("%s	%s%s"),_T("表          号"),_T("P11"),_T("\r\n"));
	str_ppp += t_str_ppp;
	for (i = 0;i<sinfo.meternum;i++)
	{
		t_str_ppp.Format(_T("%s	%d%s"),ReverseMeterID(i),para11_meter[i],_T("\r\n"));
		str_ppp += t_str_ppp;
	}
	SetDlgItemText(IDC_EDIT17,str_ppp);
	i = ((CEdit*)GetDlgItem(IDC_EDIT17))->GetLineCount();
	((CEdit*)GetDlgItem(IDC_EDIT17))->LineScroll(i,0);
}

//获取P13及Td(将采样计算得到的P13填入数据参数中)
void ParaMeterTool::OnBnClickedButton37()
{
	// TODO: 在此添加控件通知处理程序代码
	int i;
	int item;
	CString com;
	CString AllMeter;
	AllMeter = _T("所有表");

	item = m_combo1.GetCurSel();
	m_combo1.GetLBText(item, com);

	if (com == AllMeter)
	{
		AfxMessageBox(_T("请选择单只表!")); 
		return;
	}
	else 
	{		
		for (i = 0;i < sinfo.meternum;i++)
		{
			AllMeter = ReverseMeterID(i);
			if (com == AllMeter)
			{
				SetDlgItemInt(IDC_EDIT6, para12_meter[i]);
				SetDlgItemInt(IDC_EDIT19, Td_meter[i]);
			}
		}
	}
}

//获取P12(将采样计算得到的P13填入数据参数中)
void ParaMeterTool::OnBnClickedButton38()
{
	// TODO: 在此添加控件通知处理程序代码
	int i;
	int item;
	CString com;
	CString AllMeter;
	AllMeter = _T("所有表");

	item = m_combo1.GetCurSel();
	m_combo1.GetLBText(item, com);

	if (com == AllMeter)
	{
		AfxMessageBox(_T("请选择单只表!")); 
		return;
	}
	else 
	{		
		for (i = 0;i < sinfo.meternum;i++)
		{
			AllMeter = ReverseMeterID(i);
			if (com == AllMeter)
			{
				SetDlgItemInt(IDC_EDIT5, para11_meter[i]);	
			}
		}
	}	
}

//计算P11
void ParaMeterTool::OnBnClickedButton40()
{
	// TODO: 在此添加控件通知处理程序代码
	int i,j,k;
	int sample_n_tmp;
	sample_n_tmp = GetDlgItemInt(IDC_EDIT23);
	float flow_tmp=0;
	float flow_calmeter=0;
	CString t_str_ppp;

	for (j = 0;j < sinfo.meternum;j++)
	{
		flow_meter[j] = 0;
	}	
	for (i = 0;i < sample_n_tmp;i++)
	{
		flow_tmp = sinfo.mflow + sinfo.lflow;
		if (flow_tmp ==0)
		{
			i--;
			continue;
		}
		for (j = 0;j < sinfo.meternum;j++)
		{			
			if(!ReadFlow_meter((int)sinfo.minfo[j].mhandle, sinfo.minfo[j].meterid,(signed char*)&(sinfo.minfo[j].flow)))
			{
				i--;
				for (k = 0;k < sinfo.meternum;k++)
				{
					sinfo.minfo[k].flow = 0;
				}
				flow_tmp = 0;
				break;
			}			
			else 
			{

			}			
		}
		flow_calmeter += flow_tmp;
		for (k = 0;k < sinfo.meternum;k++)
		{
			flow_meter[k]  += sinfo.minfo[k].flow * 0.36;			
		}
		Sleep(1000);	
	}
	flow_calmeter = flow_calmeter/sample_n_tmp;
	//获取当前参数
	for (j = 0;j < sinfo.meternum;j++)
	{
		if(!ReadFlowP((int)sinfo.minfo[j].mhandle, sinfo.minfo[j].meterid,(unsigned char*)&sinfo.minfo[j].flowpram))
		{
			j--;
		}
		else
		{
			flow_meter[j] = flow_meter[j] /sample_n_tmp;			
			if (flow_meter[j]!=0)
			{
				//第一种,直接加上相差的数据,结果是ml的16倍
				para10_meter[j] = (int)((float)sinfo.minfo[j].flowpram.P10-(flow_meter[j] - flow_calmeter)/3.6*16);
			}
			else
			{
				para10_meter[j] = 0;
			}			
		}		
	}
	//显示全部计算得到的参数
	GetDlgItemText(IDC_EDIT17,str_ppp);
	t_str_ppp.Format(_T("%s	%s%s"),_T("表		      号"),_T("P10"),_T("\r\n"));
	str_ppp += t_str_ppp;
	for (i = 0;i<sinfo.meternum;i++)
	{
		t_str_ppp.Format(_T("%s	%d%s"),ReverseMeterID(i),para10_meter[i],_T("\r\n"));
		str_ppp += t_str_ppp;
	}
	SetDlgItemText(IDC_EDIT17,str_ppp);
	i = ((CEdit*)GetDlgItem(IDC_EDIT17))->GetLineCount();
	((CEdit*)GetDlgItem(IDC_EDIT17))->LineScroll(i,0);
}

//获取P11
void ParaMeterTool::OnBnClickedButton39()
{
	// TODO: 在此添加控件通知处理程序代码
	int i;
	int item;
	CString com;
	CString AllMeter;
	AllMeter = _T("所有表");

	item = m_combo1.GetCurSel();
	m_combo1.GetLBText(item, com);

	if (com == AllMeter)
	{
		AfxMessageBox(_T("请选择单只表!")); 
		return;
	}
	else 
	{		
		for (i = 0;i < sinfo.meternum;i++)
		{
			AllMeter = ReverseMeterID(i);
			if (com == AllMeter)
			{
				SetDlgItemInt(IDC_EDIT4, para10_meter[i]);	
			}
		}
	}	
}
