#pragma once
// CMyNewDlg 对话框
#include "stdafx.h"
// 伴随征象
typedef struct _phe
{
	int edge_type;
	BOOL type[5];
}ConPhenomenon;
// 其他情况
typedef struct 
{
	int type;
	BOOL othercase[9];
}oth;

typedef struct
{
	int pesudo_color;
	int birads;
	int morph;
	int derection;
	int inner_echo;
	int back_echo;
	ConPhenomenon con_phen;
	BOOL boder_type[6];
	BOOL cacif_type[3];
	oth other_case;
}TumorInfo;


class CTumorInfoDlg : public CDialog
{
	DECLARE_DYNAMIC(CTumorInfoDlg)

public:
	CTumorInfoDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CTumorInfoDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TUMOR_INFO_DLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	DECLARE_MESSAGE_MAP()
public:
	TumorInfo m_info;
	int m_tumor_id;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedRadioB1();
	afx_msg void OnBnClickedRadiom1();
	virtual BOOL OnInitDialog();
};
