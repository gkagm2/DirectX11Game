
// MFCDialogDlg.h: 헤더 파일
//

#pragma once
#include <string.h>
#include <tchar.h>
#include <stack>
using std::stack;

// CMFCDialogDlg 대화 상자
class CMFCDialogDlg : public CDialogEx
{
private:
	int m_iStackNum;
	CString m_strNumber;
	
	UINT m_iOper;

// 생성입니다.
public:
	CMFCDialogDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFCDIALOG_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.

public:
	int Calculate(int _iFirst, int _iSecond, UINT _iOper);

// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CEdit m_Edit;
	afx_msg void OnNumBnClickedBtn(UINT _iNum);
	afx_msg void OnOperateBnClickedButton(UINT _iOper);
	afx_msg void OnBnClickedButtonResult();
	afx_msg void OnBnClickedButtonClear();
};
