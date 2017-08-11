
// MeasureCommdemoDlg.h : 头文件
//

#pragma once
#include "resource.h"
#include "OperateExcelFile.h"
#include "DisplayEdit.h"
#include "Algorithms.h"
#include "ParaMeterTool.h"
#include "afxwin.h"
#include "afxcmn.h"

// CMeasureCommdemoDlg 对话框
class CMeasureCommdemoDlg : public CDialogEx
{
// 构造
public:
	CMeasureCommdemoDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_MEASURECOMMDEMO_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;
	CComboBox m_combo1;
	CComboBox m_combo2;
	CComboBox m_combo3;
	CComboBox m_combo4;
	CComboBox m_combo5;
	CComboBox m_combo6;
	DisplayEdit displayEdit;   
	DisplayEdit displayResult; 
	DisplayEdit displayFlowRate;
	ParaMeterTool pmtdialog;
	
	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnClose();
	afx_msg void OnTimer(UINT nIDEvent);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL ExcelOperation(void);
	
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton6();
	afx_msg void OnBnClickedButton7();

private:
/*OperateExcelFile opexf;*/
	AlgrithmsTest opexf;

public:
	afx_msg void OnBnClickedButton8();
	afx_msg void OnBnClickedButton9();
	afx_msg void OnBnClickedButton10();
	afx_msg void OnBnClickedButton11();
	afx_msg void OnBnClickedButton12();
	afx_msg void OnBnClickedButton13();
	afx_msg void OnBnClickedButton14();
	afx_msg void OnBnClickedButton15();

	afx_msg void OnBnClickedButton17();
	afx_msg void OnCbnSelchangeCombo7();
	afx_msg void OnOK();
	afx_msg void OnCancel();
	afx_msg void OnBnClickedButton18();
	afx_msg void OnBnClickedButton19();
	afx_msg void OnBnClickedButton20();
/*	afx_msg void OnBnClickedButton21();*/
	
// 	afx_msg void OnBnClickedButton16();
// 	afx_msg void OnBnClickedButton22();
	afx_msg void OnBnClickedButton23();
	afx_msg void OnBnClickedButton24();
	
	afx_msg void OnBnClickedButton25();
	afx_msg void OnBnClickedButton26();

	afx_msg void OnBnClickedButton28();
	afx_msg void OnBnClickedButton27();
	afx_msg void OnBnClickedButton29();
	afx_msg void OnCbnSelchangeCombo8();
	CComboBox m_combo7;
	CComboBox m_combo8;
	CComboBox m_combo9_meter6;
	afx_msg void OnBnClickedButton30();
	afx_msg void OnBnClickedButton31();
	afx_msg void OnBnClickedButton32();
	afx_msg void OnBnClickedButton33();
};




