#pragma once
#include "resource.h"		// ������
#include "moreopt.h"
class moreopt;
class ChisfooDlg;

// kbpanel �Ի���

class kbpanel : public CDialogEx
{
	DECLARE_DYNAMIC(kbpanel)

public:
	kbpanel(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~kbpanel();
	ChisfooDlg* CallBackEnv = NULL;
	void SetupFunc(unsigned long long Mode);
// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG4 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
};
