#pragma once

#include "resource.h"		// ������
#include "hisfooDlg.h"
#include "filepcs.h"
#include "kbpanel.h"
#include "autorunctrl.h"
#include "ykshow.h"
class kbpanel;
class filepcs;
class ChisfooDlg;
class autorunctrl;
class ykshow;

// moreopt �Ի���

class moreopt : public CDialogEx
{
	DECLARE_DYNAMIC(moreopt)

public:
	moreopt(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~moreopt();
	ChisfooDlg* CallBackEnv = NULL;

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG2 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton4();
};
