
// hisfooDlg.h : ͷ�ļ�
//

#pragma once
#include "Ccmdline.h"
#include "../hisfans/ykdef.h"
#include "afxwin.h"
#include <map>
#include "moreopt.h"
#include "filepcs.h"
class ChisfooDlgAutoProxy;
class Ccmdline;
class moreopt;
class filepcs;

// ChisfooDlg �Ի���
class ChisfooDlg : public CDialogEx
{
	DECLARE_DYNAMIC(ChisfooDlg);
	friend class ChisfooDlgAutoProxy;
public:
	typedef int BOOL;
	typedef unsigned char byte;
	class CLData
	{
	public:
		unsigned long long Method;
		list<shared_ptr<byte>> Data;
	};
	map<unsigned long long, list<CLData>>WorkMap;
	BOOL Work_Pcs(SOCKET iSock, unsigned long long UID);

	class UserData
	{
	public:
		chrono::system_clock::time_point _Last_Time;
		unsigned long long CID;
	};

	BOOL CopyToClipboard(string pszData, unsigned long long nDataLen)
	{
		if (::OpenClipboard(NULL))
		{
			::EmptyClipboard();
			HGLOBAL clipbuffer;
			char* buffer;
			clipbuffer = ::GlobalAlloc(GMEM_DDESHARE, nDataLen);
			buffer = (char *)::GlobalLock(clipbuffer);
			memcpy(buffer, pszData.c_str(), nDataLen);
			::GlobalUnlock(clipbuffer);
			::SetClipboardData(CF_TEXT, clipbuffer);
			::CloseClipboard();
			return TRUE;
		}
		return FALSE;
	}

public:
	SOCKET slisten;
	atomic_short Listen_Port = S_Port;
	timed_mutex _NetLock;
	atomic_uint _Thr_Flag = 0U;
	atomic_uint Pcs_Flag = 1U;
	unsigned long long _Cli_Count = 1ULL;
	list<thread> _Thr_Pcs;
	void NetWork_Pcs();
	void Offline_Pcs();
	list<CLData> Task_Pcs;
	list<UserData>userList;
	shared_ptr<Ccmdline> tbox;
	shared_ptr<unsigned int> _Shell_Call;
	shared_ptr<filepcs> _file_trs_dlg;
	void _VFUNC(unsigned long long CID);
	void _GoShell(unsigned long long CID, string Shell);
	void MessagePcs(string Msg);
	void UserOperate(unsigned long long UserID, unsigned long long Mode);
	void UserOperate(unsigned long long UserID, unsigned long long Mode, sockaddr_in& sCli);
// ����
public:
	ChisfooDlg(CWnd* pParent = NULL);	// ��׼���캯��
	virtual ~ChisfooDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_HISFOO_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

public:
	//��ȡ�ض���ʽ���ļ���  
	void GetAllFormatFiles(string path, vector<string>& files, string format)
	{
		//�ļ����    
		long   hFile = 0;
		//�ļ���Ϣ    
		struct _finddata_t fileinfo;
		string p;
		if ((hFile = _findfirst(p.assign(path).append("\\*" + format).c_str(), &fileinfo)) != -1)
		{
			do
			{
				if ((fileinfo.attrib &  _A_SUBDIR))
				{
					if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
					{
						files.push_back(p.assign(path).append("\\").append(fileinfo.name) );  
						GetAllFormatFiles(p.assign(path).append("\\").append(fileinfo.name), files, format);
					}
				}
				else
				{
					files.push_back(p.assign(path).append("\\").append(fileinfo.name));
				}
			} while (_findnext(hFile, &fileinfo) == 0);

			_findclose(hFile);
		}
	}
// ʵ��
protected:
	ChisfooDlgAutoProxy* m_pAutoProxy;
	HICON m_hIcon;

	BOOL CanExit();

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnClose();
	virtual void OnOK();
	virtual void OnCancel();
	DECLARE_MESSAGE_MAP()
public:
	// �⼦�б�
	CListBox puserlist;
	// ��Ϣ������
	CListBox pmsgbox;
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedCancel();

	POINT old;    //����ԭ��Ļ�ͻ�����С���������ڴ�С��  
				  //�������ڴ�С�����湤������ͬ
	void AdjustWindowsSize(void)
	{
		CRect rect;
		GetClientRect(&rect);     //ȡ�ͻ�����С    
		old.x = rect.right - rect.left;
		old.y = rect.bottom - rect.top;
		int cx = GetSystemMetrics(SM_CXFULLSCREEN);
		int cy = GetSystemMetrics(SM_CYFULLSCREEN);
		CRect rt;
		SystemParametersInfo(SPI_GETWORKAREA, 0, &rt, 0);  //������湤������С�����������������Ĵ�С
		cy = rt.bottom;
		MoveWindow(0, 0, cx, cy);
	}

	//������ǰ���������пؼ���С������Ӧ��ǰ��Ļ�ֱ���
	void ReSize(void)
	{
		float fsp[2];
		POINT Newp; //��ȡ���ڶԻ���Ĵ�С  
		CRect recta;
		GetClientRect(&recta);     //ȡ�ͻ�����С    
		Newp.x = recta.right - recta.left;
		Newp.y = recta.bottom - recta.top;
		fsp[0] = (float)Newp.x / old.x;
		fsp[1] = (float)Newp.y / old.y;
		CRect Rect;
		int woc;
		CPoint OldTLPoint, TLPoint; //���Ͻ�  
		CPoint OldBRPoint, BRPoint; //���½�  
		HWND  hwndChild = ::GetWindow(m_hWnd, GW_CHILD);  //�г����пؼ�    
		while (hwndChild)
		{
			woc = ::GetDlgCtrlID(hwndChild);//ȡ��ID  
			GetDlgItem(woc)->GetWindowRect(Rect);
			ScreenToClient(Rect);
			OldTLPoint = Rect.TopLeft();
			TLPoint.x = long(OldTLPoint.x*fsp[0]);
			TLPoint.y = long(OldTLPoint.y*fsp[1]);
			OldBRPoint = Rect.BottomRight();
			BRPoint.x = long(OldBRPoint.x *fsp[0]);
			BRPoint.y = long(OldBRPoint.y *fsp[1]);
			Rect.SetRect(TLPoint, BRPoint);
			GetDlgItem(woc)->MoveWindow(Rect, TRUE);
			hwndChild = ::GetWindow(hwndChild, GW_HWNDNEXT);
		}
		old = Newp;
	}
	afx_msg void OnBnClickedButton4();
};
