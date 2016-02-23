
// TSComDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "TSCom.h"
#include "TSComDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CTSComDlg 对话框



CTSComDlg::CTSComDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_TSCOM_DIALOG, pParent)
	, m_ComOrder(0)
	, m_ComSetting(_T(""))
	, m_Order2Send(_T(""))
	, m_RecievedStr(_T(""))
	, mTarget(0)
	, mString_MeasureData(_T(""))
	, auto_renew(false)
	, auto_mode(false)
	, is_fine_turning(false)
	, has_key_down(false)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTSComDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MSCOMM1, m_Com);
	DDX_Control(pDX, IDC_BUTTON1, m_ButtonOpen);
	DDX_Control(pDX, IDC_BUTTON2, m_ButtonClose);
	DDX_Control(pDX, IDC_COMBO1, m_ComboBox);
	DDX_Control(pDX, IDC_COMBO2, m_ComboComOrder);
	DDX_Control(pDX, IDC_EDIT1, m_CEditDataR);
	DDX_Control(pDX, IDC_BUTTON3, m_ButtonForward);
	DDX_Control(pDX, IDC_BUTTON4, m_ButtonLeft);
	DDX_Control(pDX, IDC_BUTTON5, m_ButtonRight);
	DDX_Control(pDX, IDC_BUTTON6, m_ButtonBack);
	DDX_Control(pDX, IDC_COMBO3, mTargets);
	DDX_Control(pDX, IDC_BUTTON8, mButton_stop);
	DDX_Control(pDX, IDC_EDIT2, mTemerature);
	DDX_Control(pDX, IDC_EDIT3, mHumidity);
	DDX_Control(pDX, IDC_EDIT4, mFDistance);
	DDX_Control(pDX, IDC_EDIT5, mBDistance);
	DDX_Control(pDX, IDC_BUTTON9, mbutton_renew_HT);
	DDX_Control(pDX, IDC_BUTTON10, mbutton_renew_FB);
	DDX_Control(pDX, IDC_BUTTON11, mbutton_auto_renew);
	DDX_Control(pDX, IDC_BUTTON12, mbutton_auto_mode);
	DDX_Control(pDX, IDC_BUTTON21, mbutton_fine_turning);
	DDX_Control(pDX, IDC_BUTTON13, mbutton_s1);
	DDX_Control(pDX, IDC_BUTTON14, mbutton_s2);
	DDX_Control(pDX, IDC_BUTTON15, mbutton_s3);
	DDX_Control(pDX, IDC_BUTTON16, mbutton_s4);
	DDX_Control(pDX, IDC_BUTTON20, mbutton_s5);
}

BEGIN_MESSAGE_MAP(CTSComDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CTSComDlg::OnBnClickedOpen)
	ON_BN_CLICKED(IDC_BUTTON2, &CTSComDlg::OnBnClickedClose)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CTSComDlg::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO2, &CTSComDlg::OnCbnSelchangeOrder)
	ON_BN_CLICKED(IDC_BUTTON3, &CTSComDlg::OnBnClickedForward)
	ON_BN_CLICKED(IDC_BUTTON4, &CTSComDlg::OnBnClickedLeft)
	ON_BN_CLICKED(IDC_BUTTON5, &CTSComDlg::OnBnClickedRight)
	ON_BN_CLICKED(IDC_BUTTON6, &CTSComDlg::OnBnClickedBack)
	ON_BN_CLICKED(IDC_BUTTON7, &CTSComDlg::OnBnClickedClear)
	ON_CBN_SELCHANGE(IDC_COMBO3, &CTSComDlg::OnTargetChange)
	ON_BN_CLICKED(IDC_BUTTON8, &CTSComDlg::OnBnClickedStop)
	ON_BN_CLICKED(IDC_BUTTON9, &CTSComDlg::OnBnClickedButton9)
	ON_BN_CLICKED(IDC_BUTTON10, &CTSComDlg::OnBnClickedRenewFB)
	ON_BN_CLICKED(IDC_BUTTON11, &CTSComDlg::OnBnClickedAutoRenew)
	ON_BN_CLICKED(IDC_BUTTON12, &CTSComDlg::OnBnClickedAutoMode)
	ON_BN_CLICKED(IDC_BUTTON21, &CTSComDlg::OnBnClickedFineTurn)
	ON_BN_CLICKED(IDC_BUTTON13, &CTSComDlg::OnBnClickedL1)
	ON_BN_CLICKED(IDC_BUTTON14, &CTSComDlg::OnBnClickedL2)
	ON_BN_CLICKED(IDC_BUTTON15, &CTSComDlg::OnBnClickedL3)
	ON_BN_CLICKED(IDC_BUTTON16, &CTSComDlg::OnBnClickedL4)
	ON_BN_CLICKED(IDC_BUTTON20, &CTSComDlg::OnBnClickedL5)
END_MESSAGE_MAP()


// CTSComDlg 消息处理程序

BOOL CTSComDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	//添加波特率选项
	m_ComboBox.AddString(_T("19200"));
	m_ComboBox.AddString(_T("9600"));
	m_ComboBox.AddString(_T("4800"));
	m_ComboBox.AddString(_T("2400"));
	//添加端口号选项
	m_ComboComOrder.AddString(_T("com1"));
	m_ComboComOrder.AddString(_T("com2"));
	m_ComboComOrder.AddString(_T("com3"));
	m_ComboComOrder.AddString(_T("com4"));
	//添加目标设备项目
	mTargets.AddString(_T("电机组"));
	mTargets.AddString(_T("前舵机"));
	mTargets.AddString(_T("后舵机"));
	//波特率组合框默认设置
	m_ComboBox.SetCurSel(3);
	m_ComSetting = "9600,n,8,1";
	//端口号默认设置
	m_ComboComOrder.SetCurSel(3);
	m_ComOrder = 4;
	//默认目标设备设置
	mTargets.SetCurSel(0);
	mTarget = 0;
	//按钮设置
	m_ButtonClose.EnableWindow(FALSE);
	m_ButtonOpen.EnableWindow(TRUE);
	m_ButtonForward.EnableWindow(FALSE);
	m_ButtonBack.EnableWindow(FALSE);
	m_ButtonLeft.EnableWindow(FALSE);
	m_ButtonRight.EnableWindow(FALSE);
	mButton_stop.EnableWindow(FALSE);
	mbutton_auto_mode.EnableWindow(FALSE);
	mbutton_auto_renew.EnableWindow(FALSE);
	mbutton_fine_turning.EnableWindow(FALSE);
	mbutton_renew_FB.EnableWindow(FALSE);
	mbutton_renew_HT.EnableWindow(FALSE);
	mbutton_s1.EnableWindow(FALSE);
	mbutton_s2.EnableWindow(FALSE);
	mbutton_s3.EnableWindow(FALSE);
	mbutton_s4.EnableWindow(FALSE);
	mbutton_s5.EnableWindow(FALSE);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CTSComDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CTSComDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CTSComDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CTSComDlg::OnBnClickedOpen()
{
	// TODO: 在此添加控件通知处理程序代码
	int i = 0, t_usable[4] = { 0, 0, 0, 0 }, sum = 0;
	HANDLE hcom = INVALID_HANDLE_VALUE;
	CString coms[] = { _T("COM1"), _T("COM2"), _T("COM3"), _T("COM4") };
	while (i < 4)
	{
		hcom = CreateFile(coms[i], GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hcom != INVALID_HANDLE_VALUE)
		{
			CloseHandle(hcom);
			t_usable[i] = 1;
			sum++;
		}
		i++;
	}
	if (sum == 0)
	{
		MessageBox(_T("未发现可用串行口，打开失败！"));
		return;
	}
	else
	{
		if (t_usable[m_ComOrder - 1] == 0)
		{
			CString t1("串口"),t2;
			i = 0;
			while (i < 4)
			{
				if (t_usable[i] == 1)
				{
					t2.Format(_T("%d，"), i + 1);
					t1 += t2;
				}
				i++;
			}
			
			MessageBox(_T("当前串行口不可用，打开失败！"));
			if (t1 != "串口")
			{
				MessageBox(t1 + _T("可用！"));
			}
			return;
		}
	}
	m_Com.CloseWindow();
	if (m_Com.get_PortOpen())
	{
		m_Com.put_PortOpen(FALSE);
	}
	m_Com.put_CommPort(m_ComOrder);
	if (!m_Com.get_PortOpen())
	{
		m_Com.put_PortOpen(TRUE);
	}
	else
	{
		MessageBox(_T("未能打开串口！"));
		return;
	}
	m_Com.put_Settings(m_ComSetting);
	m_Com.put_RThreshold(1);
	m_Com.put_InputMode(1);
	m_Com.put_InputLen(0);
	m_Com.get_Input();
	m_ComboBox.EnableWindow(FALSE);
	m_ComboComOrder.EnableWindow(FALSE);
	m_ButtonClose.EnableWindow(TRUE);
	m_ButtonOpen.EnableWindow(FALSE);
	m_ButtonForward.EnableWindow(TRUE);
	m_ButtonBack.EnableWindow(TRUE);
	m_ButtonLeft.EnableWindow(TRUE);
	m_ButtonRight.EnableWindow(TRUE);
	mButton_stop.EnableWindow(TRUE);
	mTargets.EnableWindow(TRUE);
	mButton_stop.EnableWindow(TRUE);
	mbutton_auto_mode.EnableWindow(TRUE);
	mbutton_auto_renew.EnableWindow(TRUE);
	mbutton_fine_turning.EnableWindow(TRUE);
	mbutton_renew_FB.EnableWindow(TRUE);
	mbutton_renew_HT.EnableWindow(TRUE);
	mbutton_s1.EnableWindow(TRUE);
	mbutton_s2.EnableWindow(TRUE);
	mbutton_s3.EnableWindow(TRUE);
	mbutton_s4.EnableWindow(TRUE);
	mbutton_s5.EnableWindow(TRUE);
	MessageBox(_T("打开串口成功！"));
	m_Com.put_Output(COleVariant(_T("S")));//发送连接通知
}


void CTSComDlg::OnBnClickedClose()
{
	// TODO: 在此添加控件通知处理程序代码
	m_Com.put_Output(COleVariant(_T("O")));//发送连接中断通知
	m_Com.put_PortOpen(FALSE);
	m_ComboBox.EnableWindow(TRUE);
	m_ComboComOrder.EnableWindow(TRUE);
	m_ButtonClose.EnableWindow(FALSE);
	m_ButtonOpen.EnableWindow(TRUE);
	m_ButtonForward.EnableWindow(FALSE);
	m_ButtonBack.EnableWindow(FALSE);
	m_ButtonLeft.EnableWindow(FALSE);
	m_ButtonRight.EnableWindow(FALSE);
	mButton_stop.EnableWindow(FALSE);
	mTargets.EnableWindow(FALSE);
	mButton_stop.EnableWindow(FALSE);
	mbutton_auto_mode.EnableWindow(FALSE);
	mbutton_auto_renew.EnableWindow(FALSE);
	mbutton_fine_turning.EnableWindow(FALSE);
	mbutton_renew_FB.EnableWindow(FALSE);
	mbutton_renew_HT.EnableWindow(FALSE);
	mbutton_s1.EnableWindow(FALSE);
	mbutton_s2.EnableWindow(FALSE);
	mbutton_s3.EnableWindow(FALSE);
	mbutton_s4.EnableWindow(FALSE);
	mbutton_s5.EnableWindow(FALSE);
	MessageBox(_T("串口已经关闭！"));
}


void CTSComDlg::OnCbnSelchangeCombo()
{
	// TODO: 在此添加控件通知处理程序代码
	int sel = m_ComboBox.GetCurSel();
	switch (sel)
	{
	case 0:
		m_ComSetting = "19200,n,8,1";
		break;
	case 1:
		m_ComSetting = "2400,n,8,1";
		break;
	case 2:
		m_ComSetting = "4800,n,8,1";
		break;
	case 3:
		m_ComSetting = "9600,n,8,1";
		break;
	default:
		break;
	}
}
BEGIN_EVENTSINK_MAP(CTSComDlg, CDialogEx)
	ON_EVENT(CTSComDlg, IDC_MSCOMM1, 1, CTSComDlg::OnCommMscomm1, VTS_NONE)
END_EVENTSINK_MAP()


void CTSComDlg::OnCommMscomm1()
{
	// TODO: 在此处添加消息处理程序代码
	VARIANT variant_inp;
	COleSafeArray safearray_inp;
	LONG len, k;
	BYTE rxdata[2048];
	CString temp_str;
	if (2 == m_Com.get_CommEvent())
	{
		variant_inp = m_Com.get_Input();
		safearray_inp = variant_inp;
		len = safearray_inp.GetOneDimSize();
		for (k = 0; k < len; k++)
		{
			safearray_inp.GetElement(&k, rxdata + k);
		}
		for (k = 0; k < len; k++)
		{
			BYTE bt = *(char*)(rxdata + k);
			temp_str.Format(_T("%c"), bt);
			m_RecievedStr += temp_str;
		}
		m_RecievedStr += "\r\n";
	}
	//获取时间
	char timE[12];
	time_t rawtime = time(0);
	struct tm timeinfo;
	localtime_s(&timeinfo, &rawtime);
	strftime(timE, sizeof(timE), "%X", &timeinfo);
	//分辨命令
	char type = (char)m_RecievedStr[0];
	switch (type)
	{
	case '#':
		m_Com.put_Output(COleVariant(_T("&")));//发送确认符
		if (mString_MeasureData.GetLength() > 0)
		{
			manage_measure_data(mString_MeasureData);
			mString_MeasureData = "";
		}
		else
		{
			m_CEditDataR.SetSel(1000000, 1000000);
			m_CEditDataR.ReplaceSel(CString(timE) + _T("：意外的终止符\r\n"));
			UpdateData(FALSE);
		}
		m_RecievedStr = "";
		break;
	case '$':
		m_Com.put_Output(COleVariant(_T("&")));//发送确认符
		m_CEditDataR.SetSel(1000000, 1000000);
		m_CEditDataR.ReplaceSel(_T("MCU is uploading data...\r\n"));
		UpdateData(FALSE);
		m_RecievedStr = "";
		break;
	default:
		m_Com.put_Output(COleVariant(_T("&")));//发送确认符
		mString_MeasureData.AppendChar(type);//收集测量信息
		//打印日志
		m_CEditDataR.SetSel(1000000, 1000000);
		m_CEditDataR.ReplaceSel(CString(timE) + _T("：收到：") + m_RecievedStr);
		UpdateData(FALSE);
		m_RecievedStr = "";
		break;
	}
}


void CTSComDlg::OnCbnSelchangeOrder()
{
	// TODO: 在此添加控件通知处理程序代码
	int sel = m_ComboComOrder.GetCurSel();
	switch (sel)
	{
	case 0:
		m_ComOrder = 1;
		break;
	case 1:
		m_ComOrder = 2;
		break;
	case 2:
		m_ComOrder = 3;
		break;
	case 3:
		m_ComOrder = 4;
		break;
	default:
		break;
	}
}




void CTSComDlg::OnBnClickedForward()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!m_ButtonForward.IsWindowEnabled())
	{
		return;
	}
	m_Com.put_Output(COleVariant(command_up[mTarget]));
	//打印日志
	char timE[12];
	time_t rawtime = time(0);
	struct tm timeinfo;
	localtime_s(&timeinfo,&rawtime);
	strftime(timE, sizeof(timE), "%X", &timeinfo);
	m_CEditDataR.SetSel(1000000, 1000000);
	m_CEditDataR.ReplaceSel(CString(timE) + _T("：发送：") + command_up[mTarget] + CString(_T("\r\n")));
	UpdateData(FALSE);
}


void CTSComDlg::OnBnClickedLeft()
{
	// TODO: 在此添加控件通知处理程序代码
	if (is_fine_turning)
	{
		m_Com.put_Output(COleVariant(command_left[mTarget + 3]));
	}
	else
	{
		m_Com.put_Output(COleVariant(command_left[mTarget]));
	}
	//打印日志
	char timE[12];
	time_t rawtime = time(0);
	struct tm timeinfo;
	localtime_s(&timeinfo,&rawtime);
	strftime(timE, sizeof(timE), "%X", &timeinfo);
	m_CEditDataR.SetSel(1000000, 1000000);
	m_CEditDataR.ReplaceSel(CString(timE) + _T("：发送：") + command_left[mTarget] + CString(_T("\r\n")));
	UpdateData(FALSE);
}


void CTSComDlg::OnBnClickedRight()
{
	// TODO: 在此添加控件通知处理程序代码
	if (is_fine_turning)
	{
		m_Com.put_Output(COleVariant(command_right[mTarget + 3]));
	}
	else
	{
		m_Com.put_Output(COleVariant(command_right[mTarget]));
	}
	//打印日志
	char timE[12];
	time_t rawtime = time(0);
	struct tm timeinfo;
	localtime_s(&timeinfo,&rawtime);
	strftime(timE, sizeof(timE), "%X", &timeinfo);
	m_CEditDataR.SetSel(1000000, 1000000);
	m_CEditDataR.ReplaceSel(CString(timE) + _T("：发送：") + command_right[mTarget] + CString(_T("\r\n")));
	UpdateData(FALSE);
}


void CTSComDlg::OnBnClickedBack()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!m_ButtonBack.IsWindowEnabled())
	{
		return;
	}
	m_Com.put_Output(COleVariant(command_down[mTarget]));
	//打印日志
	char timE[12];
	time_t rawtime = time(0);
	struct tm timeinfo;
	localtime_s(&timeinfo,&rawtime);
	strftime(timE, sizeof(timE), "%X", &timeinfo);
	m_CEditDataR.SetSel(1000000, 1000000);
	m_CEditDataR.ReplaceSel(CString(timE) + _T("：发送：") + CString(_T("D\r\n")));
	UpdateData(FALSE);
}


void CTSComDlg::OnBnClickedClear()
{
	// TODO: 在此添加控件通知处理程序代码
	m_CEditDataR.SetSel(0,2000000);
	m_CEditDataR.ReplaceSel(_T(""));
	UpdateData(FALSE);
}

BOOL CTSComDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN && !has_key_down)
	{
		switch (pMsg->wParam)
		{
		case 37:
			OnBnClickedLeft();
			break;
		case 38:
			OnBnClickedForward();
			break;
		case 39:
			OnBnClickedRight();
			break;
		case 40:
			if (m_ButtonBack.IsWindowEnabled())
			{
				OnBnClickedBack();
			}
			break;
		case 49:
			OnBnClickedL1();
			break;
		case 50:
			OnBnClickedL2();
			break;
		case 51:
			OnBnClickedL3();
			break;
		case 52:
			OnBnClickedL4();
			break;
		case 53:
			OnBnClickedL5();
			break;
		default:
			break;
		}
		has_key_down = true;
	}
	else if (pMsg->message == WM_KEYUP || pMsg->wParam == 32)
	{
		if (mButton_stop.IsWindowEnabled())
		{
			OnBnClickedStop();
		}
		has_key_down = false;
	}
	if ((pMsg->wParam > 36 && pMsg->wParam < 41) || (pMsg->wParam > 48 && pMsg->wParam < 54))
	{
		return TRUE;
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CTSComDlg::OnTargetChange()
{
	// TODO: 在此添加控件通知处理程序代码
	int sel = mTargets.GetCurSel();
	switch (sel)
	{
	case 0:
		mTarget = 0;
		m_ButtonForward.EnableWindow(TRUE);
		m_ButtonBack.EnableWindow(TRUE);
		break;
	case 1:
		mTarget = 2;
		m_ButtonBack.EnableWindow(TRUE);
		m_ButtonForward.EnableWindow(FALSE);
		OnBnClickedStop();
		break;
	case 2:
		mTarget = 1;
		m_ButtonBack.EnableWindow(FALSE);
		m_ButtonForward.EnableWindow(TRUE);
		OnBnClickedStop();
		break;
	default:
		break;
	}
}


void CTSComDlg::OnBnClickedStop()
{
	// TODO: 在此添加控件通知处理程序代码
	m_Order2Send = 's';
	m_Com.put_Output(COleVariant(m_Order2Send));
	//打印日志
	char timE[12];
	time_t rawtime = time(0);
	struct tm timeinfo;
	localtime_s(&timeinfo,&rawtime);
	strftime(timE, sizeof(timE), "%X", &timeinfo);
	m_CEditDataR.SetSel(1000000, 1000000);
	m_CEditDataR.ReplaceSel(CString(timE) + _T("：发送：") + CString(_T("s\r\n")));
	UpdateData(FALSE);
}

void CTSComDlg::manage_measure_data(CString data)
{
	if (data.GetLength() == 0)
	{
		return;
	}
	char type = (char)data[0];
	switch (type)
	{
	case 'H':
		mHumidity.SetWindowTextW(data.Right(data.GetLength() - 1));
		break;
	case 'T':
		mTemerature.SetWindowTextW(data.Right(data.GetLength() - 1));
		break;
	case 'D':
		mFDistance.SetWindowTextW(data.Right(data.GetLength() - 1));
		break;
	case 'd':
		mBDistance.SetWindowTextW(data.Right(data.GetLength() - 1));
		break;
	case 'M':
		m_CEditDataR.SetSel(1000000, 1000000);
		m_CEditDataR.ReplaceSel(_T("消息：") + data + CString(_T("\r\n")));
		UpdateData(FALSE);
		break;
	default:
		m_CEditDataR.SetSel(1000000, 1000000);
		m_CEditDataR.ReplaceSel(_T("误码：") + data + CString(_T("\r\n")));
		UpdateData(FALSE);
		break;
	}
}

void CTSComDlg::OnBnClickedButton9()//更新温湿度
{
	// TODO: 在此添加控件通知处理程序代码
	m_Com.put_Output(COleVariant(_T("S")));//发送连接通知
	m_Com.put_Output(COleVariant(_T("n")));
	//打印日志
	char timE[12];
	time_t rawtime = time(0);
	struct tm timeinfo;
	localtime_s(&timeinfo, &rawtime);
	strftime(timE, sizeof(timE), "%X", &timeinfo);
	m_CEditDataR.SetSel(1000000, 1000000);
	m_CEditDataR.ReplaceSel(CString(timE) + _T("：发送：") + CString(_T("S + n\r\n")));
	UpdateData(FALSE);
}


void CTSComDlg::OnBnClickedRenewFB()
{
	// TODO: 在此添加控件通知处理程序代码
	m_Com.put_Output(COleVariant(_T("S")));//发送连接通知
	m_Com.put_Output(COleVariant(_T("N")));
	//打印日志
	char timE[12];
	time_t rawtime = time(0);
	struct tm timeinfo;
	localtime_s(&timeinfo, &rawtime);
	strftime(timE, sizeof(timE), "%X", &timeinfo);
	m_CEditDataR.SetSel(1000000, 1000000);
	m_CEditDataR.ReplaceSel(CString(timE) + _T("：发送：") + CString(_T("S + N\r\n")));
	UpdateData(FALSE);
}


void CTSComDlg::OnBnClickedAutoRenew()
{
	// TODO: 在此添加控件通知处理程序代码
	if (auto_renew)
	{
		m_Com.put_Output(COleVariant(_T("$")));
		mbutton_auto_renew.SetWindowTextW(_T("手动更新"));
		auto_renew = false;
		//打印日志
		char timE[12];
		time_t rawtime = time(0);
		struct tm timeinfo;
		localtime_s(&timeinfo, &rawtime);
		strftime(timE, sizeof(timE), "%X", &timeinfo);
		m_CEditDataR.SetSel(1000000, 1000000);
		m_CEditDataR.ReplaceSel(CString(timE) + _T("：发送：") + CString(_T("$\r\n")));
		UpdateData(FALSE);
		return;
	}
	m_Com.put_Output(COleVariant(_T("S")));//发送连接通知
	m_Com.put_Output(COleVariant(_T("#")));
	mbutton_auto_renew.SetWindowTextW(_T("自动更新"));
	auto_renew = true;
	//打印日志
	char timE[12];
	time_t rawtime = time(0);
	struct tm timeinfo;
	localtime_s(&timeinfo, &rawtime);
	strftime(timE, sizeof(timE), "%X", &timeinfo);
	m_CEditDataR.SetSel(1000000, 1000000);
	m_CEditDataR.ReplaceSel(CString(timE) + _T("：发送：") + CString(_T("S + #\r\n")));
	UpdateData(FALSE);
}


void CTSComDlg::OnBnClickedAutoMode()
{
	// TODO: 在此添加控件通知处理程序代码
	if (auto_mode)
	{
		OnBnClickedStop();
		mbutton_auto_mode.SetWindowTextW(_T("手动控制"));
		auto_mode = false;
		return;
	}
	m_Com.put_Output(COleVariant(_T("P")));
	mbutton_auto_mode.SetWindowTextW(_T("自动运行"));
	auto_mode = true;
	//打印日志
	char timE[12];
	time_t rawtime = time(0);
	struct tm timeinfo;
	localtime_s(&timeinfo, &rawtime);
	strftime(timE, sizeof(timE), "%X", &timeinfo);
	m_CEditDataR.SetSel(1000000, 1000000);
	m_CEditDataR.ReplaceSel(CString(timE) + _T("：发送：") + CString(_T("P\r\n")));
	UpdateData(FALSE);
}


void CTSComDlg::OnBnClickedFineTurn()
{
	// TODO: 在此添加控件通知处理程序代码
	if (is_fine_turning)
	{
		mbutton_fine_turning.SetWindowTextW(_T("舵机微调"));
		is_fine_turning = false;
		return;
	}
	mbutton_fine_turning.SetWindowTextW(_T("关闭微调"));
	is_fine_turning = true;
}


void CTSComDlg::OnBnClickedL1()
{
	// TODO: 在此添加控件通知处理程序代码
	m_Com.put_Output(COleVariant(_T("A")));
	//打印日志
	char timE[12];
	time_t rawtime = time(0);
	struct tm timeinfo;
	localtime_s(&timeinfo, &rawtime);
	strftime(timE, sizeof(timE), "%X", &timeinfo);
	m_CEditDataR.SetSel(1000000, 1000000);
	m_CEditDataR.ReplaceSel(CString(timE) + _T("：发送：") + CString(_T("A\r\n")));
	UpdateData(FALSE);
}


void CTSComDlg::OnBnClickedL2()
{
	// TODO: 在此添加控件通知处理程序代码
	m_Com.put_Output(COleVariant(_T("B")));
	//打印日志
	char timE[12];
	time_t rawtime = time(0);
	struct tm timeinfo;
	localtime_s(&timeinfo, &rawtime);
	strftime(timE, sizeof(timE), "%X", &timeinfo);
	m_CEditDataR.SetSel(1000000, 1000000);
	m_CEditDataR.ReplaceSel(CString(timE) + _T("：发送：") + CString(_T("B\r\n")));
	UpdateData(FALSE);
}


void CTSComDlg::OnBnClickedL3()
{
	// TODO: 在此添加控件通知处理程序代码
	m_Com.put_Output(COleVariant(_T("C")));
	//打印日志
	char timE[12];
	time_t rawtime = time(0);
	struct tm timeinfo;
	localtime_s(&timeinfo, &rawtime);
	strftime(timE, sizeof(timE), "%X", &timeinfo);
	m_CEditDataR.SetSel(1000000, 1000000);
	m_CEditDataR.ReplaceSel(CString(timE) + _T("：发送：") + CString(_T("C\r\n")));
	UpdateData(FALSE);
}


void CTSComDlg::OnBnClickedL4()
{
	// TODO: 在此添加控件通知处理程序代码
	m_Com.put_Output(COleVariant(_T("E")));
	//打印日志
	char timE[12];
	time_t rawtime = time(0);
	struct tm timeinfo;
	localtime_s(&timeinfo, &rawtime);
	strftime(timE, sizeof(timE), "%X", &timeinfo);
	m_CEditDataR.SetSel(1000000, 1000000);
	m_CEditDataR.ReplaceSel(CString(timE) + _T("：发送：") + CString(_T("E\r\n")));
	UpdateData(FALSE);
}


void CTSComDlg::OnBnClickedL5()
{
	// TODO: 在此添加控件通知处理程序代码
	m_Com.put_Output(COleVariant(_T("F")));
	//打印日志
	char timE[12];
	time_t rawtime = time(0);
	struct tm timeinfo;
	localtime_s(&timeinfo, &rawtime);
	strftime(timE, sizeof(timE), "%X", &timeinfo);
	m_CEditDataR.SetSel(1000000, 1000000);
	m_CEditDataR.ReplaceSel(CString(timE) + _T("：发送：") + CString(_T("F\r\n")));
	UpdateData(FALSE);
}
