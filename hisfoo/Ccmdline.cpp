// Ccmdline.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "hisfoo.h"
#include "Ccmdline.h"
#include "afxdialogex.h"


// Ccmdline �Ի���

IMPLEMENT_DYNAMIC(Ccmdline, CDialogEx)

Ccmdline::Ccmdline(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG1, pParent)
{

}

Ccmdline::~Ccmdline()
{
	if (is_ok)
		*is_ok = 0U;
}

void Ccmdline::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT2, pedit);
}


BEGIN_MESSAGE_MAP(Ccmdline, CDialogEx)
	ON_COMMAND(IDD_DIALOG1, &Ccmdline::OnIddDialog1)
	ON_UPDATE_COMMAND_UI(IDD_DIALOG1, &Ccmdline::OnUpdateIddDialog1)
	ON_BN_CLICKED(IDC_BUTTON1, &Ccmdline::OnBnClickedButton1)
END_MESSAGE_MAP()


// Ccmdline ��Ϣ�������


void Ccmdline::OnIddDialog1()
{
	// TODO: �ڴ���������������
}


void Ccmdline::OnUpdateIddDialog1(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
}

void Ccmdline::AddStr(string _str)
{
	CString _pstr;
	GetDlgItemText(IDC_EDIT2, _pstr);
	string _sstr = _pstr.GetString();
	_sstr += "\r\n";
	_sstr += _str;
	SetDlgItemText(IDC_EDIT2, _sstr.c_str());
	SendDlgItemMessage(IDC_EDIT2, WM_VSCROLL, SB_BOTTOM, 0);
}


void Ccmdline::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString _szstr;
	GetDlgItemText(IDC_EDIT1, _szstr);
	tDlg->_GoShell(CID, _szstr.GetString());
	tDlg->_VFUNC(CID);
	AddStr(CString("�Ѿ�Ҫ��ִ��" + CString(_szstr + "�������ĵȺ���")).GetString());
}
