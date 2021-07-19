
// MFCDialogDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "MFCDialog.h"
#include "MFCDialogDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
public:
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


// CMFCDialogDlg 대화 상자



CMFCDialogDlg::CMFCDialogDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFCDIALOG_DIALOG, pParent),
	m_iStackNum(0),
	m_iOper(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON1);
}

void CMFCDialogDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_Edit);
}

BEGIN_MESSAGE_MAP(CMFCDialogDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_0, IDC_BTN_9, &CMFCDialogDlg::OnNumBnClickedBtn)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BUTTON_PLUS, IDC_BUTTON_DIV, &CMFCDialogDlg::OnOperateBnClickedButton)
	ON_BN_CLICKED(IDC_BUTTON_RESULT, &CMFCDialogDlg::OnBnClickedButtonResult)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, &CMFCDialogDlg::OnBnClickedButtonClear)
END_MESSAGE_MAP()

// CMFCDialogDlg 메시지 처리기

BOOL CMFCDialogDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
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

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	SetWindowText(_T("계 산 기"));

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CMFCDialogDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CMFCDialogDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CMFCDialogDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


// 버튼을 누를 경우
void CMFCDialogDlg::OnNumBnClickedBtn(UINT _iNum)
{
	int iNum = (int)_iNum % 1000;

	// int to string
	CString strNum;
	strNum.Format(_T("%d"), iNum);
	m_strNumber += strNum;
	m_Edit.SetWindowTextW(m_strNumber);
}


// 연산버튼을 누를 경우
void CMFCDialogDlg::OnOperateBnClickedButton(UINT _iOper)
{
	if (m_iStackNum == 0) {
		m_iStackNum = _ttoi(m_strNumber);
		m_strNumber = _T("");
	}
	m_iOper = _iOper;

	if (_T("") != m_strNumber) {
		m_iStackNum = Calculate(m_iStackNum, _ttoi(m_strNumber), m_iOper);
		m_strNumber = _T("");
	}
}


void CMFCDialogDlg::OnBnClickedButtonResult()
{
	m_iStackNum = Calculate(m_iStackNum, _ttoi(m_strNumber), m_iOper);
	m_strNumber.Format(_T("%d"), m_iStackNum);
	m_Edit.SetWindowTextW(m_strNumber);
	m_strNumber = _T("");
}

void CMFCDialogDlg::OnBnClickedButtonClear()
{
	m_iStackNum = 0;
	m_strNumber = _T("");
	m_iOper = 0;
	m_Edit.SetWindowTextW(_T(""));
}

int CMFCDialogDlg::Calculate(int _iFirst, int _iSecond, UINT _iOper)
{
	int iResult = 0;
	switch (_iOper) {
	case IDC_BUTTON_PLUS:
		iResult = _iFirst + _iSecond;
		break;
	case IDC_BUTTON_MINUS:
		iResult = _iFirst - _iSecond;
		break;
	case IDC_BUTTON_MUL:
		iResult = _iFirst * _iSecond;
		break;
	case IDC_BUTTON_DIV:
		iResult = _iFirst / _iSecond;
		break;
	}
	return iResult;
}