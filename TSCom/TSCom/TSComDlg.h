
// TSComDlg.h : 头文件
//

#pragma once
#include "mscomm.h"
#include "afxwin.h"


// CTSComDlg 对话框
class CTSComDlg : public CDialogEx
{
// 构造
public:
	CTSComDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TSCOM_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CMscomm m_Com;
	CButton m_ButtonOpen;
	CButton m_ButtonClose;
	afx_msg void OnBnClickedOpen();
	afx_msg void OnBnClickedClose();
	CComboBox m_ComboBox;
	afx_msg void OnCbnSelchangeCombo();
	// 串口序号
	int m_ComOrder;
	// 串口参数
	CString m_ComSetting;
	// 要发送的命令
	CString m_Order2Send;
	// 供设置串口号的组合框
	CComboBox m_ComboComOrder;
	// 显示收到的数据的编辑框
	CEdit m_CEditDataR;
	DECLARE_EVENTSINK_MAP()
	void OnCommMscomm1();
	afx_msg void OnCbnSelchangeOrder();
	CButton m_ButtonForward;
	CButton m_ButtonLeft;
	CButton m_ButtonRight;
	CButton m_ButtonBack;
	afx_msg void OnBnClickedForward();
	afx_msg void OnBnClickedLeft();
	afx_msg void OnBnClickedRight();
	afx_msg void OnBnClickedBack();
	void manage_measure_data(CString);

	// 收到的数据
	CString m_RecievedStr;
	afx_msg void OnBnClickedClear();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	// 要控制的设备
	CComboBox mTargets;
	CButton mButton_stop;
	// 当前选中的设备
	int mTarget;
	// 右键键值
	CString command_right[6] = { _T("R"), _T("r"), _T("3"), _T("R"), _T("+"), _T("j") };
	// 左键键值	
	CString command_left[6] = { _T("L"), _T("l"), _T("1"), _T("L"), _T("-"), _T("J") };
	//上键键值
	CString command_up[2] = { _T("U"), _T("f") };
	CString command_down[3] = { _T("D"), _T(""), _T("2") };
	afx_msg void OnTargetChange();
	afx_msg void OnBnClickedStop();
	CEdit mTemerature;
	CEdit mHumidity;
	CEdit mFDistance;
	CEdit mBDistance;
	CButton mbutton_renew_HT;
	CButton mbutton_renew_FB;
	CButton mbutton_auto_renew;
	CButton mbutton_auto_mode;
	CButton mbutton_fine_turning;
	CButton mbutton_s1;
	CButton mbutton_s2;
	CButton mbutton_s3;
	CButton mbutton_s4;
	CButton mbutton_s5;
	// 作为收集信息的容器
	CString mString_MeasureData;
	afx_msg void OnBnClickedButton9();
	afx_msg void OnBnClickedRenewFB();
	afx_msg void OnBnClickedAutoRenew();
	bool auto_renew;
	bool auto_mode;
	afx_msg void OnBnClickedAutoMode();
	afx_msg void OnBnClickedFineTurn();
	// 舵机微调标志
	bool is_fine_turning;
	afx_msg void OnBnClickedL1();
	afx_msg void OnBnClickedL2();
	afx_msg void OnBnClickedL3();
	afx_msg void OnBnClickedL4();
	afx_msg void OnBnClickedL5();
	// 标志是否有按键按下，防止信息重叠发送
	bool has_key_down;
};
