// autorunctrl.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "hisfoo.h"
#include "autorunctrl.h"
#include "afxdialogex.h"


// autorunctrl �Ի���

IMPLEMENT_DYNAMIC(autorunctrl, CDialogEx)

autorunctrl::autorunctrl(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG5, pParent)
{

}

autorunctrl::~autorunctrl()
{
}

void autorunctrl::UseFunction(unsigned long long Mode)
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
	SzCDATA.Method = Auto_Run;
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

void autorunctrl::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(autorunctrl, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &autorunctrl::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &autorunctrl::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &autorunctrl::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &autorunctrl::OnBnClickedButton4)
END_MESSAGE_MAP()


// autorunctrl ��Ϣ�������


void autorunctrl::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UseFunction(1ULL);
}


void autorunctrl::OnBnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UseFunction(2ULL);
}


void autorunctrl::OnBnClickedButton3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UseFunction(3ULL);
}


void autorunctrl::OnBnClickedButton4()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	MessageBoxA("ʹ��˵��:\n"\
"����ͻ��˴�δ��װ������Ҫ�Ȱ�װ��\n"\
"�����װ��������δ�������״ΰ�װ����Զ���������������������\n"\
"���������Ҫ������������ж�ط���", "Ĥ������", 0U);
}
