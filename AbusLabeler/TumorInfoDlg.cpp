// CMyNewDlg.cpp: 实现文件
//
#include "stdafx.h"
#include "AbusLabeler.h"
#include "TumorInfoDlg.h"
#include "afxdialogex.h"
#include "AbusLabelerDlg.h"


// CMyNewDlg 对话框

IMPLEMENT_DYNAMIC(CTumorInfoDlg, CDialog)

CTumorInfoDlg::CTumorInfoDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_TUMOR_INFO_DLG, pParent)
{

}

CTumorInfoDlg::~CTumorInfoDlg()
{
}


void CTumorInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIOB1, m_info.birads);			// BI-RADS
	DDX_Radio(pDX, IDC_RADIOM1, m_info.morph);			// 形态
	DDX_Radio(pDX, IDC_RADIODE1, m_info.derection);		// 方向

	DDX_Radio(pDX, IDC_RADIO_INNER_ECHO1, m_info.inner_echo);		// 内部回声		

	DDX_Radio(pDX, IDC_RADIO_BE1, m_info.back_echo);				// 后方回声

	DDX_Check(pDX, IDC_CHECK_EDGE1, m_info.boder_type[0]);			// 边界
	DDX_Check(pDX, IDC_CHECK_EDGE2, m_info.boder_type[1]);
	DDX_Check(pDX, IDC_CHECK_EDGE3, m_info.boder_type[2]);
	DDX_Check(pDX, IDC_CHECK_EDGE4, m_info.boder_type[3]);
	DDX_Check(pDX, IDC_CHECK_EDGE5, m_info.boder_type[4]);
	DDX_Check(pDX, IDC_CHECK_EDGE6, m_info.boder_type[5]);

	DDX_Check(pDX, IDC_CHECK_CA1, m_info.cacif_type[0]);			//钙化
	DDX_Check(pDX, IDC_CHECK_CA2, m_info.cacif_type[1]);
	DDX_Check(pDX, IDC_CHECK_CA3, m_info.cacif_type[2]);

	DDX_Check(pDX, IDC_CHECK_CON1, m_info.con_phen.type[0]);		// 伴随现象
	DDX_Check(pDX, IDC_CHECK_CON2, m_info.con_phen.type[1]);
	DDX_Check(pDX, IDC_CHECK_CON3, m_info.con_phen.type[2]);
	DDX_Check(pDX, IDC_CHECK_CON4, m_info.con_phen.type[3]);
	DDX_Check(pDX, IDC_CHECK_CON5, m_info.con_phen.type[4]);

	DDX_Radio(pDX, IDC_RADIO_COR_IMG1, m_info.con_phen.edge_type);			// 冠状面成像

	DDX_Check(pDX, IDC_CHECK_OTHER_CASE1, m_info.other_case.othercase[0]);	// 其他情形
	DDX_Check(pDX, IDC_CHECK_OTHER_CASE2, m_info.other_case.othercase[1]);
	DDX_Check(pDX, IDC_CHECK_OTHER_CASE3, m_info.other_case.othercase[2]);
	DDX_Check(pDX, IDC_CHECK_OTHER_CASE4, m_info.other_case.othercase[3]);
	DDX_Check(pDX, IDC_CHECK_OTHER_CASE5, m_info.other_case.othercase[4]);
	DDX_Check(pDX, IDC_CHECK_OTHER_CASE6, m_info.other_case.othercase[5]);
	DDX_Check(pDX, IDC_CHECK_OTHER_CASE7, m_info.other_case.othercase[6]);
	DDX_Check(pDX, IDC_CHECK_OTHER_CASE8, m_info.other_case.othercase[7]);
	DDX_Check(pDX, IDC_CHECK_OTHER_CASE9, m_info.other_case.othercase[8]);

	DDX_Radio(pDX, IDC_RADIO_VESSEL1, m_info.other_case.type);				// 血管异常
}


BEGIN_MESSAGE_MAP(CTumorInfoDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CTumorInfoDlg::OnBnClickedOk)
	// BI-RADS
	ON_BN_CLICKED(IDC_RADIOB1, &CTumorInfoDlg::OnBnClickedRadioB1)
	ON_BN_CLICKED(IDC_RADIOB2, &CTumorInfoDlg::OnBnClickedRadioB1)
	ON_BN_CLICKED(IDC_RADIOB3, &CTumorInfoDlg::OnBnClickedRadioB1)
	ON_BN_CLICKED(IDC_RADIOB4, &CTumorInfoDlg::OnBnClickedRadioB1)
	ON_BN_CLICKED(IDC_RADIOB5, &CTumorInfoDlg::OnBnClickedRadioB1)
	ON_BN_CLICKED(IDC_RADIOB6, &CTumorInfoDlg::OnBnClickedRadioB1)
	ON_BN_CLICKED(IDC_RADIOB7, &CTumorInfoDlg::OnBnClickedRadioB1)

	// 形态
	ON_BN_CLICKED(IDC_RADIOM1, &CTumorInfoDlg::OnBnClickedRadiom1)
	ON_BN_CLICKED(IDC_RADIOM2, &CTumorInfoDlg::OnBnClickedRadiom1)
	ON_BN_CLICKED(IDC_RADIOM3, &CTumorInfoDlg::OnBnClickedRadiom1)

	// 后方回声
	ON_BN_CLICKED(IDC_RADIO_BE1, &CTumorInfoDlg::OnBnClickedRadioB1)
	ON_BN_CLICKED(IDC_RADIO_BE2, &CTumorInfoDlg::OnBnClickedRadioB1)
	ON_BN_CLICKED(IDC_RADIO_BE3, &CTumorInfoDlg::OnBnClickedRadioB1)
	ON_BN_CLICKED(IDC_RADIO_BE4, &CTumorInfoDlg::OnBnClickedRadioB1)

	// 伴随现象
	ON_BN_CLICKED(IDC_CHECK_CON1, &CTumorInfoDlg::OnBnClickedRadioB1)
	ON_BN_CLICKED(IDC_CHECK_CON2, &CTumorInfoDlg::OnBnClickedRadioB1)
	ON_BN_CLICKED(IDC_CHECK_CON3, &CTumorInfoDlg::OnBnClickedRadioB1)
	ON_BN_CLICKED(IDC_CHECK_CON4, &CTumorInfoDlg::OnBnClickedRadiom1)
	ON_BN_CLICKED(IDC_CHECK_CON5, &CTumorInfoDlg::OnBnClickedRadiom1)

	// 边界
	ON_BN_CLICKED(IDC_CHECK_EDGE1, &CTumorInfoDlg::OnBnClickedRadioB1)
	ON_BN_CLICKED(IDC_CHECK_EDGE2, &CTumorInfoDlg::OnBnClickedRadioB1)
	ON_BN_CLICKED(IDC_CHECK_EDGE3, &CTumorInfoDlg::OnBnClickedRadioB1)
	ON_BN_CLICKED(IDC_CHECK_EDGE4, &CTumorInfoDlg::OnBnClickedRadioB1)
	ON_BN_CLICKED(IDC_CHECK_EDGE5, &CTumorInfoDlg::OnBnClickedRadioB1)
	ON_BN_CLICKED(IDC_CHECK_EDGE6, &CTumorInfoDlg::OnBnClickedRadioB1)

	// 内部回声
	ON_BN_CLICKED(IDC_RADIO_INNER_ECHO1, &CTumorInfoDlg::OnBnClickedRadioB1)
	ON_BN_CLICKED(IDC_RADIO_INNER_ECHO2, &CTumorInfoDlg::OnBnClickedRadioB1)
	ON_BN_CLICKED(IDC_RADIO_INNER_ECHO3, &CTumorInfoDlg::OnBnClickedRadioB1)
	ON_BN_CLICKED(IDC_RADIO_INNER_ECHO4, &CTumorInfoDlg::OnBnClickedRadioB1)
	ON_BN_CLICKED(IDC_RADIO_INNER_ECHO5, &CTumorInfoDlg::OnBnClickedRadioB1)
	ON_BN_CLICKED(IDC_RADIO_INNER_ECHO6, &CTumorInfoDlg::OnBnClickedRadioB1)

	//钙化
	ON_BN_CLICKED(IDC_CHECK_CA1, &CTumorInfoDlg::OnBnClickedRadioB1)
	ON_BN_CLICKED(IDC_CHECK_CA2, &CTumorInfoDlg::OnBnClickedRadioB1)
	ON_BN_CLICKED(IDC_CHECK_CA3, &CTumorInfoDlg::OnBnClickedRadioB1)

	// 冠状面成像
	ON_BN_CLICKED(IDC_RADIO_COR_IMG1, &CTumorInfoDlg::OnBnClickedRadiom1)
	ON_BN_CLICKED(IDC_RADIO_COR_IMG2, &CTumorInfoDlg::OnBnClickedRadiom1)
	ON_BN_CLICKED(IDC_RADIO_COR_IMG3, &CTumorInfoDlg::OnBnClickedRadiom1)

	// 方向
	ON_BN_CLICKED(IDC_RADIODE1, &CTumorInfoDlg::OnBnClickedRadiom1)
	ON_BN_CLICKED(IDC_RADIODE2, &CTumorInfoDlg::OnBnClickedRadiom1)

	// 其他情形
	ON_BN_CLICKED(IDC_CHECK_OTHER_CASE1, &CTumorInfoDlg::OnBnClickedRadioB1)
	ON_BN_CLICKED(IDC_CHECK_OTHER_CASE2, &CTumorInfoDlg::OnBnClickedRadiom1)
	ON_BN_CLICKED(IDC_CHECK_OTHER_CASE3, &CTumorInfoDlg::OnBnClickedRadiom1)
	ON_BN_CLICKED(IDC_CHECK_OTHER_CASE4, &CTumorInfoDlg::OnBnClickedRadiom1)
	ON_BN_CLICKED(IDC_CHECK_OTHER_CASE5, &CTumorInfoDlg::OnBnClickedRadioB1)
	ON_BN_CLICKED(IDC_CHECK_OTHER_CASE6, &CTumorInfoDlg::OnBnClickedRadioB1)
	ON_BN_CLICKED(IDC_CHECK_OTHER_CASE7, &CTumorInfoDlg::OnBnClickedRadioB1)
	ON_BN_CLICKED(IDC_CHECK_OTHER_CASE8, &CTumorInfoDlg::OnBnClickedRadiom1)
	ON_BN_CLICKED(IDC_CHECK_OTHER_CASE9, &CTumorInfoDlg::OnBnClickedRadiom1)
	ON_BN_CLICKED(IDC_RADIO_VESSEL1, &CTumorInfoDlg::OnBnClickedRadiom1)

END_MESSAGE_MAP()

void CTumorInfoDlg::OnBnClickedOk()
{
	UpdateData(TRUE);
	CDialog::OnOK();
}

void CTumorInfoDlg::OnBnClickedRadioB1()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	GetParent()->SendMessage(WM_UPDATE_TUMOR_INFO, (WPARAM)m_tumor_id, 0);
}


void CTumorInfoDlg::OnBnClickedRadiom1()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	GetParent()->SendMessage(WM_UPDATE_TUMOR_INFO, (WPARAM)m_tumor_id, 0);
}



BOOL CTumorInfoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}