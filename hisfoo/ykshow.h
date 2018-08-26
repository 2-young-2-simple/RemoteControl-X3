#pragma once

#include "resource.h"		// ������
#include "hisfooDlg.h"
#include "filepcs.h"
#include "kbpanel.h"
#include "autorunctrl.h"
#include <stack>
class kbpanel;
class filepcs;
class ChisfooDlg;
class autorunctrl;
#define SHOW_NORMAL            1
#define SHOW_CENTER            2
#define SHOW_STRETCH        3
#define SHOW_ISOSTRETCH        4

// ykshow �Ի���

class ykshow : public CDialogEx
{
	DECLARE_DYNAMIC(ykshow)

public:
	ykshow(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~ykshow();
	// Main Window
	ChisfooDlg* CallBackEnv = NULL;
	int  ShowDib(HDC hdc, BITMAPFILEHEADER * pbmfh, int cxClient, int cyClient, int wShow)
	{
		if (NULL == pbmfh)
			return 0;
		return ShowDib(hdc, (BITMAPINFO*)(pbmfh + 1), (BYTE*)pbmfh + (pbmfh->bfOffBits), cxClient, cyClient, wShow);
	}
	int ShowDib(HDC hdc, BITMAPINFO * pbmi, BYTE * pBits, int cxClient, int cyClient, int wShow)
	{
		if (NULL == hdc || NULL == pbmi || NULL == pBits || wShow<SHOW_NORMAL || wShow>SHOW_ISOSTRETCH)
			return 0;
		int cxDib, cyDib;
		int BltMode, ret;
		if (pbmi->bmiHeader.biSize == sizeof(BITMAPCOREHEADER))
		{
			cxDib = ((BITMAPCOREHEADER *)pbmi)->bcWidth;
			cyDib = ((BITMAPCOREHEADER *)pbmi)->bcHeight;
		}
		else
		{
			cxDib = pbmi->bmiHeader.biWidth;
			cyDib = abs(pbmi->bmiHeader.biHeight);
		}
		switch (wShow)
		{
		case SHOW_NORMAL:
			return SetDIBitsToDevice(hdc, 0, 0, cxDib, cyDib, 0, 0,
				0, cyDib, pBits, pbmi, DIB_RGB_COLORS);

		case SHOW_CENTER:
			return SetDIBitsToDevice(hdc, (cxClient - cxDib) / 2, (cyClient - cyDib) / 2,
				cxDib, cyDib, 0, 0, 0, cyDib, pBits, pbmi, DIB_RGB_COLORS);

		case SHOW_STRETCH:
			BltMode = SetStretchBltMode(hdc, COLORONCOLOR);
			ret = StretchDIBits(hdc, 0, 0, cxClient, cyClient,
				0, 0, cxDib, cyDib,
				pBits, pbmi, DIB_RGB_COLORS, SRCCOPY);
			SetStretchBltMode(hdc, BltMode);
			return ret;

		case SHOW_ISOSTRETCH:
			BltMode = SetStretchBltMode(hdc, COLORONCOLOR);
			if (cxDib*cyClient > cyDib*cxClient)//ͼ����
				ret = StretchDIBits(hdc, 0,
				(cyClient - (int)((float)cyDib / (float)cxDib*(float)cxClient)) / 2,
					cxClient, (int)((float)cyDib / (float)cxDib*(float)cxClient),
					0, 0, cxDib, cyDib, pBits, pbmi, DIB_RGB_COLORS, SRCCOPY);
			else
				ret = StretchDIBits(hdc,
				(cxClient - (int)((float)cxDib / (float)cyDib*(float)cyClient)) / 2, 0,
					(int)((float)cxDib / (float)cyDib*(float)cyClient), cyClient,
					0, 0, cxDib, cyDib, pBits, pbmi, DIB_RGB_COLORS, SRCCOPY);
			SetStretchBltMode(hdc, BltMode);
			return ret;
		}
		return 0;
	}
	shared_ptr<unsigned long> ShowFlag;
	unsigned long long DoBuffer = 0ULL;
	unsigned long long Moux = 0ULL;
	unsigned long long Mouy = 0ULL;
	unsigned long KeyBoardHook = 0;
	void ShowThread();
	unsigned long Working = 0UL;
	unsigned long GetPicShot = 0UL;
	unsigned long long PicSize = 0ULL;
	stack<thread> NxThread;
	bool InitNetEnv()
	{
		// �������绷���ĳ�ʼ������  
		WSADATA wsa;
		if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		{
			printf("WSAStartup failed\n");
			return false;
		}
		return true;
	}
	BOOL OnInitDialog();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG6 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
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
	SOCKET slisten;
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

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnStnClickedPicctrl();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton8();
	afx_msg void OnBnClickedButton6();
	afx_msg void OnBnClickedButton7();
};

#define INVALID_MSG      -1   // ��Ч����Ϣ��ʶ  
#define MSG_FILENAME     1   // �ļ�������  
#define MSG_FILELENGTH     2   // �����ļ��ĳ���  
#define MSG_CLIENT_READY    3   // �ͻ���׼�������ļ�  
#define MSG_FILE      4   // �����ļ�  
#define MSG_SENDFILESUCCESS    5   // �����ļ��ɹ�  
#define MSG_OPENFILE_ERROR    10   // ���ļ�ʧ��,�������ļ�·�������Ҳ����ļ���ԭ��  
#define MSG_FILEALREADYEXIT_ERROR 11   // Ҫ������ļ��Ѿ�������
class CCSDef
{
public:
#pragma pack(1)      // ʹ�ṹ������ݰ���1�ֽ�������,ʡ�ռ�  
	// ��Ϣͷ  
	struct TMSG_HEADER
	{
		char    cMsgID;    // ��Ϣ��ʶ  
		TMSG_HEADER(char MsgID = INVALID_MSG)
			: cMsgID(MsgID)
		{
		}
	};
	// �����͵��ļ���  
	// �ͻ��˴����������˵���ȫ·������  
	// ���������ظ��ͻ��˵����ļ���  
	struct TMSG_FILENAME : public TMSG_HEADER
	{
		char szFileName[256];   // �����ļ������ַ�����  
		TMSG_FILENAME()
			: TMSG_HEADER(MSG_FILENAME)
		{
		}
	};
	// �����ļ�����  
	struct TMSG_FILELENGTH : public TMSG_HEADER
	{
		long lLength;
		TMSG_FILELENGTH(long length)
			: TMSG_HEADER(MSG_FILELENGTH), lLength(length)
		{
		}
	};
	// Client���Ѿ�׼������,Ҫ��Server�˿�ʼ�����ļ�  
	struct TMSG_CLIENT_READY : public TMSG_HEADER
	{
		TMSG_CLIENT_READY()
			: TMSG_HEADER(MSG_CLIENT_READY)
		{
		}
	};
	// �����ļ�  
	struct TMSG_FILE : public TMSG_HEADER
	{
		union     // ����union��֤�����ݰ��Ĵ�С������MAX_PACKET_SIZE * sizeof(char)  
		{
			char szBuff[Pkg_Leng];
			struct
			{
				int nStart;
				int nSize;
				char szBuff[Pkg_Leng - 2 * sizeof(int)];
			}tFile;
		};
		TMSG_FILE()
			: TMSG_HEADER(MSG_FILE)
		{
		}
	};
	// �����ļ��ɹ�  
	struct TMSG_SENDFILESUCCESS : public TMSG_HEADER
	{
		TMSG_SENDFILESUCCESS()
			: TMSG_HEADER(MSG_SENDFILESUCCESS)
		{
		}
	};
	// ���ͳ�����Ϣ,����:  
	// MSG_OPENFILE_ERROR:���ļ�ʧ��  
	// MSG_FILEALREADYEXIT_ERROR:Ҫ������ļ��Ѿ�������  
	struct TMSG_ERROR_MSG : public TMSG_HEADER
	{
		TMSG_ERROR_MSG(char cErrorMsg)
			: TMSG_HEADER(cErrorMsg)
		{
		}
	}; 
#pragma pack()
};