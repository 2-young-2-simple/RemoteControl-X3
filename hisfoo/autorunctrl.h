#pragma once


// autorunctrl �Ի���
#include "moreopt.h"

class autorunctrl : public CDialogEx
{
	DECLARE_DYNAMIC(autorunctrl)

public:
	autorunctrl(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~autorunctrl();
	ChisfooDlg* CallBackEnv = NULL;
	void UseFunction(unsigned long long Mode);

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG5 };
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
