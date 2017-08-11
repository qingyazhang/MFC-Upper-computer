#pragma once
#include "resource.h"
#include "afxwin.h"
#include "DisplayEdit.h"

// ParaMeterTool 对话框

class ParaMeterTool : public CDialogEx
{
	DECLARE_DYNAMIC(ParaMeterTool)

public:
	ParaMeterTool(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~ParaMeterTool();

// 对话框数据
	enum { IDD = IDD_PARAMETERTOOl };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	DisplayEdit displaycalc36p; 
	DECLARE_MESSAGE_MAP()
public:
	CString CharToStr(UCHAR *dat, USHORT len);
	BOOL StrToChar(CString str, UCHAR *dat);
	CString ReverseMeterID(int meternum);
	
	afx_msg void OnBnClickedButton1();
	afx_msg void OnCbnEditchangeCombo1();
private:
	CComboBox m_combo1;

public:
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnOK();
	//afx_msg void OnCancel();
	afx_msg void OnBnClickedButton7();
	afx_msg void OnBnClickedButton8();
	afx_msg void OnBnClickedButton34();
	afx_msg void OnBnClickedButton35();
	afx_msg void OnBnClickedButton36();
	afx_msg void OnBnClickedButton37();
	afx_msg void OnBnClickedButton38();
	afx_msg void OnBnClickedButton40();
	afx_msg void OnBnClickedButton39();
};


