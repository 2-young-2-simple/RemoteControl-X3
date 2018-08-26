// filepcs.cpp : ʵ���ļ�
//

/*
*                    GNU GENERAL PUBLIC LICENSE
*                       Version 3, 29 June 2007
*
* Copyright (C) 2007 Free Software Foundation, Inc. <http://fsf.org/>
* Everyone is permitted to copy and distribute verbatim copies
* of this license document, but changing it is not allowed.
* 
* Saurik Remote Control Software V3 is under a GPLv3 Licence. You can share or change those code under licence.
*/

#include "stdafx.h"
#include "hisfoo.h"
#include "filepcs.h"
#include "afxdialogex.h"


// filepcs �Ի���

IMPLEMENT_DYNAMIC(filepcs, CDialogEx)

filepcs::filepcs(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG3, pParent)
{

}

filepcs::~filepcs()
{
}

void filepcs::Change_File_Data(vector<string> FileList, vector<string> DirList)
{
	folderpathlist.ResetContent();
	for (auto NxDocL = DirList.begin(); NxDocL != DirList.end(); NxDocL++)
	{
		folderpathlist.AddString(NxDocL->c_str());
	}
	filelistbox.ResetContent();
	for (auto NxDocL = FileList.begin(); NxDocL != FileList.end(); NxDocL++)
	{
		filelistbox.AddString(NxDocL->c_str());
	}
	return;
}

void filepcs::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST2, folderpathlist);
	DDX_Control(pDX, IDC_LIST3, filelistbox);
}


BEGIN_MESSAGE_MAP(filepcs, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &filepcs::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &filepcs::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &filepcs::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON5, &filepcs::OnBnClickedButton5)
	ON_LBN_DBLCLK(IDC_LIST2, &filepcs::OnLbnDblclkList2)
	ON_LBN_SELCHANGE(IDC_LIST3, &filepcs::OnLbnSelchangeList3)
	ON_LBN_DBLCLK(IDC_LIST3, &filepcs::OnLbnDblclkList3)
	ON_BN_CLICKED(IDC_BUTTON6, &filepcs::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON4, &filepcs::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON7, &filepcs::OnBnClickedButton7)
END_MESSAGE_MAP()


// filepcs ��Ϣ�������


void filepcs::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	CString CSTRBuf2;
	GetDlgItemTextA(IDC_EDIT1, CSTRBuf2);
	SStr = CSTRBuf2.GetString();
	ChisfooDlg::CLData SzCDATA;
	SzCDATA.Method = File_Download;
	shared_ptr<byte>_pThisPath(new byte[SStr.size() + 1]);
	strcpy_s(reinterpret_cast<char*>(&*_pThisPath), SStr.size() + 1, SStr.c_str());
	SzCDATA.Data.push_back(_pThisPath);
	CString CSTRBuf3;
	GetDlgItemTextA(IDC_EDIT2, CSTRBuf3);
	SStr = CSTRBuf3.GetString();
	shared_ptr<byte>_pRemotePath(new byte[SStr.size() + 1]);
	strcpy_s(reinterpret_cast<char*>(&*_pRemotePath), SStr.size() + 1, SStr.c_str());
	SzCDATA.Data.push_back(_pRemotePath);
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
	CallBackEnv->MessagePcs("�ɹ������������...");
}


void filepcs::OnBnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	CString CSTRBuf2;
	GetDlgItemTextA(IDC_EDIT1, CSTRBuf2);
	SStr = CSTRBuf2.GetString();
	ChisfooDlg::CLData SzCDATA;
	SzCDATA.Method = File_Upload;
	shared_ptr<byte>_pThisPath(new byte[SStr.size() + 1]);
	strcpy_s(reinterpret_cast<char*>(&*_pThisPath), SStr.size() + 1, SStr.c_str());
	SzCDATA.Data.push_back(_pThisPath);
	CString CSTRBuf3;
	GetDlgItemTextA(IDC_EDIT2, CSTRBuf3);
	SStr = CSTRBuf3.GetString();
	shared_ptr<byte>_pRemotePath(new byte[SStr.size() + 1]);
	strcpy_s(reinterpret_cast<char*>(&*_pRemotePath), SStr.size() + 1, SStr.c_str());
	SzCDATA.Data.push_back(_pRemotePath);
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
	CallBackEnv->MessagePcs("�ɹ�����ϴ�����...");
}


void filepcs::OnBnClickedButton3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	char szFilters[] =
		"��������õ�����Դ���ļ�(*.h;*,hpp;*.cpp)\0*.h;*.hpp;*.cpp\0"\
		"�ı��ļ�(*.txt)\0*.txt\0"\
		"�����ļ�(*.*)\0*.*\0" \
		"��������򵥵�����Դ���ļ�(*.lua)\0*.lua\0"\
		"\0";

	CFileDialog OpenDlg(TRUE);
	OpenDlg.m_ofn.lpstrTitle = "Open File";
	OpenDlg.m_ofn.lpstrFilter = szFilters;
	if (OpenDlg.DoModal() == IDOK)
	{
		SetDlgItemTextA(IDC_EDIT1, OpenDlg.GetPathName());
	}
	
}


void filepcs::OnBnClickedButton5()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	CString CSTRBuf2;
	GetDlgItemTextA(IDC_EDIT2, CSTRBuf2);
	SStr = CSTRBuf2.GetString();
	ChisfooDlg::CLData SzCDATA;
	SzCDATA.Method = File_List;
	shared_ptr<byte>_pThisPath(new byte[SStr.size() + 1]);
	strcpy_s(reinterpret_cast<char*>(&*_pThisPath), SStr.size() + 1, SStr.c_str());
	SzCDATA.Data.push_back(_pThisPath);
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
	CallBackEnv->MessagePcs("�ɹ���ӻ�ȡԶ��Ŀ¼����...");
}


void filepcs::OnLbnDblclkList2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (folderpathlist.GetCurSel() < 0)return;
	CString   strText;
	folderpathlist.GetText(folderpathlist.GetCurSel(), strText);
	CString cStr2;
	GetDlgItemTextA(IDC_EDIT2, cStr2);
	cStr2 += "\\";
	cStr2 += strText;
	SetDlgItemTextA(IDC_EDIT2, cStr2.GetString());
	OnBnClickedButton5();
}


void filepcs::OnLbnSelchangeList3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}


void filepcs::OnLbnDblclkList3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (MessageBoxA("���أ�", "����ȷ��", MB_OKCANCEL) == IDOK)
	{
		if (filelistbox.GetCurSel() < 0)return;
		CString   strText;
		filelistbox.GetText(filelistbox.GetCurSel(), strText);
		CString cStr2;
		GetDlgItemTextA(IDC_EDIT2, cStr2);
		cStr2 += "\\";
		cStr2 += strText;
		SetDlgItemTextA(IDC_EDIT2, cStr2.GetString());
		OnBnClickedButton1();
	}
}

void filepcs::_GoShell(unsigned long long CID, string Shell)
{
	auto Pcs_Com = [&](unsigned long long UID, ChisfooDlg::CLData userData)
	{
		if (CallBackEnv->WorkMap.find(UID) == CallBackEnv->WorkMap.end())
		{
			CallBackEnv->WorkMap.insert(make_pair(UID, list<ChisfooDlg::CLData>()));
		}
		CallBackEnv->WorkMap[UID].push_back(userData);
	};
	ChisfooDlg::CLData SzCDATA;
	SzCDATA.Method = Shell_Method;
	shared_ptr<byte>_pLen(new byte[sizeof(unsigned long long)]);
	unsigned long long ScLen = strlen(Shell.c_str()) + 1ULL;
	memcpy(&*_pLen, &ScLen, sizeof(unsigned long long));
	shared_ptr<byte>_pCom(new byte[strlen(Shell.c_str()) + 1]);
	memcpy(&*_pCom, Shell.c_str(), strlen(Shell.c_str()) + 1);
	SzCDATA.Data.push_back(_pLen);
	SzCDATA.Data.push_back(_pCom);
	if (CID == 0ULL)
	{
		for (auto UListTor = CallBackEnv->userList.begin(); UListTor != CallBackEnv->userList.end(); UListTor++)
		{
			Pcs_Com(UListTor->CID, SzCDATA);
		}
	}
	else
	{
		Pcs_Com(CID, SzCDATA);
	}
}

void filepcs::OnBnClickedButton6()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	CString CSTRBuf2;
	GetDlgItemTextA(IDC_EDIT2, CSTRBuf2);
	CString CSTRBuf3 = "md ";
	CSTRBuf3 += CSTRBuf2;
	_GoShell(SzNID, CSTRBuf3.GetString());
}

void getFiles(string path, vector<string> &ownname, unsigned long Mode)
{
#ifdef _WIN32
	/*files�洢�ļ���·��������(eg.   C:\Users\WUQP\Desktop\test_devided\data1.txt)
	ownnameֻ�洢�ļ�������(eg.     data1.txt)*/

	//�ļ����  
	long long   hFile = 0;
	//�ļ���Ϣ  
	struct _finddata_t fileinfo;
	string p;
	if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)
	{
		do
		{
			//�����Ŀ¼,����֮  
			//�������,�����б�  
			if ((fileinfo.attrib &  _A_SUBDIR) && Mode == 0)
			{  /*
			   if(strcmp(fileinfo.name,".") != 0  &&  strcmp(fileinfo.name,"..") != 0)
			   getFiles( p.assign(path).append("\\").append(fileinfo.name), files, ownname ); */
				ownname.push_back(fileinfo.name);
			}
			else if (Mode && !(fileinfo.attrib &  _A_SUBDIR))
			{
				//ownname.push_back(p.assign(path).append("\\").append(fileinfo.name));
				ownname.push_back(fileinfo.name);
			}
		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
	}
#endif
}

void filepcs::OnBnClickedButton4()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	vector<string> storeBuf;
	CString CSTRBuf2;
	GetDlgItemTextA(IDC_EDIT1, CSTRBuf2);
	getFiles(CSTRBuf2.GetString(), storeBuf, 1);
	auto PcsData_Ser = [&](string thisFile, string RemoteFile) -> void
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
		SzCDATA.Method = File_Upload;
		shared_ptr<byte>_pThisPath(new byte[thisFile.size() + 1]);
		strcpy_s(reinterpret_cast<char*>(&*_pThisPath), thisFile.size() + 1, thisFile.c_str());
		SzCDATA.Data.push_back(_pThisPath);
		shared_ptr<byte>_pRemotePath(new byte[RemoteFile.size() + 1]);
		strcpy_s(reinterpret_cast<char*>(&*_pRemotePath), RemoteFile.size() + 1, RemoteFile.c_str());
		SzCDATA.Data.push_back(_pRemotePath);
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
		CallBackEnv->MessagePcs("������һ���ļ�...");
	};
	CString CSTRBufEx;
	GetDlgItemTextA(IDC_EDIT2, CSTRBufEx);
	string pRemotePath = CSTRBufEx.GetString();
	pRemotePath += "\\";
	string pServerPath = CSTRBuf2.GetString();
	pServerPath += "\\";
	for (auto DimPath = storeBuf.begin(); DimPath != storeBuf.end(); DimPath++)
	{
		string WhoDim = pRemotePath + *DimPath;
		string AmIDim = pServerPath + *DimPath;
		PcsData_Ser(AmIDim, WhoDim);
	}
	return;
}


void filepcs::OnBnClickedButton7()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CallBackEnv->tbox.reset(new Ccmdline);
	CallBackEnv->_Shell_Call.reset(new unsigned);
	*(CallBackEnv->_Shell_Call) = 1;
	CallBackEnv->tbox->tDlg = CallBackEnv;
	CString CSTEX;
	CallBackEnv->GetDlgItemTextA(IDC_EDIT1, CSTEX);
	stringstream Nxss;
	Nxss << CSTEX.GetString();
	unsigned long long NIDE;
	Nxss >> NIDE;
	CallBackEnv->tbox->CID = NIDE;
	CallBackEnv->tbox->DoModal();
}
