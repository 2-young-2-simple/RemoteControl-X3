// rst.cpp : ���� DLL �ĳ�ʼ�����̡�
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
#include "rst.h"

#define   WIDTHBYTES(bits) (((bits)+31)/32*4)//����ʹͼ������ռ�ֽ���Ϊ4byte�ı���

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO:  ����� DLL ����� MFC DLL �Ƕ�̬���ӵģ�
//		��Ӵ� DLL �������κε���
//		MFC �ĺ������뽫 AFX_MANAGE_STATE ����ӵ�
//		�ú�������ǰ�档
//
//		����: 
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// �˴�Ϊ��ͨ������
//		}
//
//		�˺������κ� MFC ����
//		������ÿ��������ʮ����Ҫ��  ����ζ��
//		��������Ϊ�����еĵ�һ�����
//		���֣������������ж������������
//		������Ϊ���ǵĹ��캯���������� MFC
//		DLL ���á�
//
//		�й�������ϸ��Ϣ��
//		����� MFC ����˵�� 33 �� 58��
//

// CrstApp

BEGIN_MESSAGE_MAP(CrstApp, CWinApp)
END_MESSAGE_MAP()


// CrstApp ����

CrstApp::CrstApp()
{
	// TODO:  �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CrstApp ����

CrstApp theApp;


// CrstApp ��ʼ��

BOOL CrstApp::InitInstance()
{
	CWinApp::InitInstance();

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	return TRUE;
}



Pic GetImg(unsigned long imgwidth, unsigned long imgheight)
{
	CWnd cwnd;
	CDC *pdeskdc = cwnd.GetDesktopWindow()->GetDC();
	CRect re;
	//��ȡ���ڵĴ�С  
	cwnd.GetDesktopWindow()->GetClientRect(&re);
	CBitmap bmp;
	bmp.CreateCompatibleBitmap(pdeskdc, re.Width(), re.Height());
	//����һ�����ݵ��ڴ滭��  
	CDC memorydc;
	memorydc.CreateCompatibleDC(pdeskdc);

	//ѡ�л���  
	CBitmap *pold = memorydc.SelectObject(&bmp);

	//����ͼ��  
	memorydc.BitBlt(0, 0, re.Width(), re.Height(), pdeskdc, 0, 0, SRCCOPY);

	//��ȡ���λ�ã�Ȼ��������ͼ��  
	CPoint po;
	GetCursorPos(&po);
	HICON hinco = (HICON)GetCursor();
	memorydc.DrawIcon(po.x - 10, po.y - 10, hinco);
	//ѡ��ԭ���Ļ���  
	memorydc.SelectObject(pold);
	BITMAP bit;
	bmp.GetBitmap(&bit);
	//  DWORD size = bit.bmWidth * bit.bmHeight ;  

	//���� ͼ���С����λ��byte��  
	DWORD size = bit.bmWidthBytes * bit.bmHeight;
	LPSTR lpdata = (LPSTR)GlobalAlloc(GPTR, size);

	//�����Ǵ���һ��bmp�ļ��ı����ļ�ͷ����Ҫ�˽���Բο�msdn  

	BITMAPINFOHEADER pbitinfo;
	pbitinfo.biBitCount = 24;
	pbitinfo.biClrImportant = 0;
	pbitinfo.biCompression = BI_RGB;
	pbitinfo.biHeight = bit.bmHeight;
	pbitinfo.biPlanes = 1;
	pbitinfo.biSize = sizeof(BITMAPINFOHEADER);
	pbitinfo.biSizeImage = size;
	pbitinfo.biWidth = bit.bmWidth;
	pbitinfo.biXPelsPerMeter = 0;
	pbitinfo.biYPelsPerMeter = 0;

	GetDIBits(pdeskdc->m_hDC, bmp, 0, pbitinfo.biHeight, lpdata,
		(BITMAPINFO*)&pbitinfo, DIB_RGB_COLORS);

	BITMAPFILEHEADER bfh;
	bfh.bfReserved1 = bfh.bfReserved2 = 0;
	bfh.bfType = ((WORD)('M' << 8) | 'B');
	bfh.bfSize = 54 + size;
	bfh.bfOffBits = 54;
	// ��ʼͼ����
	BITMAPFILEHEADER HeadBuffer;
	BITMAPINFOHEADER InfoHeadBuffer;
	// IMG�߶�
	unsigned long MYDRAW_HEIGHT = imgheight;
	unsigned long MYDRAW_WIDTH = imgwidth;
	BITMAPFILEHEADER writebitHead;
	BITMAPINFOHEADER writebitInfoHead;
	writebitHead = bfh;
	writebitInfoHead = pbitinfo;
	writebitInfoHead.biHeight = MYDRAW_HEIGHT;//Ϊ��ȡ�ļ���дλͼ�߶�    
	writebitInfoHead.biWidth = MYDRAW_WIDTH;//Ϊ��ȡ�ļ���дλͼ���    
	int mywritewidth = WIDTHBYTES(writebitInfoHead.biWidth*writebitInfoHead.biBitCount);//BMPͼ��ʵ��λͼ�������Ŀ��Ϊ4byte�ı������ڴ˼���ʵ�����������    
	writebitInfoHead.biSizeImage = mywritewidth*writebitInfoHead.biHeight;//����λͼʵ����������С    

	writebitHead.bfSize = 54 + writebitInfoHead.biSizeImage;//λͼ�ļ�ͷ��СΪλͼ��������С����54byte
	HeadBuffer = writebitHead;
	InfoHeadBuffer = writebitInfoHead;

	int width = pbitinfo.biWidth;
	int height = pbitinfo.biHeight;
	//�����ڴ�ռ��Դͼ�����ڴ�       
	int l_width = WIDTHBYTES(width*pbitinfo.biBitCount);//����λͼ��ʵ�ʿ�Ȳ�ȷ����Ϊ4byte�ı���    
	int write_width = WIDTHBYTES(writebitInfoHead.biWidth*writebitInfoHead.biBitCount);//����дλͼ��ʵ�ʿ�Ȳ�ȷ����Ϊ4byte�ı���    

	unique_ptr<BYTE>pColorDataMid ((BYTE *)malloc(mywritewidth*MYDRAW_HEIGHT));//�����ڴ�ռ�洢ͼ����֮������    
	memset(&*pColorDataMid, 0, mywritewidth*MYDRAW_HEIGHT);

	long nData = height*l_width;
	long write_nData = mywritewidth*MYDRAW_HEIGHT;//��ȡ��λͼ���������ȶ���
	BYTE* pColorData = (BYTE*)lpdata;
	for (int hnum = 0; hnum < MYDRAW_HEIGHT; hnum++)
		for (int wnum = 0; wnum < MYDRAW_WIDTH; wnum++)
		{
			double d_original_img_hnum = hnum*height / (double)MYDRAW_HEIGHT;
			double d_original_img_wnum = wnum*width / (double)MYDRAW_WIDTH;
			int i_original_img_hnum = d_original_img_hnum;
			int i_original_img_wnum = d_original_img_wnum;
			double distance_to_a_x = d_original_img_wnum - i_original_img_wnum;//��ԭͼ������a���ˮƽ����    
			double distance_to_a_y = d_original_img_hnum - i_original_img_hnum;//��ԭͼ������a��Ĵ�ֱ����    

			int original_point_a = i_original_img_hnum*l_width + i_original_img_wnum * 3;//����λ��ƫ��������Ӧ��ͼ��ĸ����ص�RGB�����,�൱�ڵ�A      
			int original_point_b = i_original_img_hnum*l_width + (i_original_img_wnum + 1) * 3;//����λ��ƫ��������Ӧ��ͼ��ĸ����ص�RGB�����,�൱�ڵ�B    
			int original_point_c = (i_original_img_hnum + 1)*l_width + i_original_img_wnum * 3;//����λ��ƫ��������Ӧ��ͼ��ĸ����ص�RGB�����,�൱�ڵ�C     
			int original_point_d = (i_original_img_hnum + 1)*l_width + (i_original_img_wnum + 1) * 3;//����λ��ƫ��������Ӧ��ͼ��ĸ����ص�RGB�����,�൱�ڵ�D     
			if (i_original_img_hnum + 1 == MYDRAW_HEIGHT - 1)
			{
				original_point_c = original_point_a;
				original_point_d = original_point_b;
			}
			if (i_original_img_wnum + 1 == MYDRAW_WIDTH - 1)
			{
				original_point_b = original_point_a;
				original_point_d = original_point_c;
			}

			int pixel_point = hnum*write_width + wnum * 3;//ӳ��߶ȱ任ͼ������λ��ƫ����    
			(&*pColorDataMid)[pixel_point] =
				pColorData[original_point_a] * (1 - distance_to_a_x)*(1 - distance_to_a_y) +
				pColorData[original_point_b] * distance_to_a_x*(1 - distance_to_a_y) +
				pColorData[original_point_c] * distance_to_a_y*(1 - distance_to_a_x) +
				pColorData[original_point_c] * distance_to_a_y*distance_to_a_x;
			(&*pColorDataMid)[pixel_point + 1] =
				pColorData[original_point_a + 1] * (1 - distance_to_a_x)*(1 - distance_to_a_y) +
				pColorData[original_point_b + 1] * distance_to_a_x*(1 - distance_to_a_y) +
				pColorData[original_point_c + 1] * distance_to_a_y*(1 - distance_to_a_x) +
				pColorData[original_point_c + 1] * distance_to_a_y*distance_to_a_x;
			(&*pColorDataMid)[pixel_point + 2] =
				pColorData[original_point_a + 2] * (1 - distance_to_a_x)*(1 - distance_to_a_y) +
				pColorData[original_point_b + 2] * distance_to_a_x*(1 - distance_to_a_y) +
				pColorData[original_point_c + 2] * distance_to_a_y*(1 - distance_to_a_x) +
				pColorData[original_point_c + 2] * distance_to_a_y*distance_to_a_x;

		}
	/*******************˫���Բ�ֵ******************/
	/*******************ͼ������******************/
	//�ͷ��ڴ�
	GlobalFree(lpdata);
	Pic NxData;
	NxData.imagelong = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + write_nData;
	shared_ptr<char> Bmp(new char[sizeof(BITMAPFILEHEADER)+ sizeof(BITMAPINFOHEADER)+ write_nData]);
	memcpy(&*Bmp, &HeadBuffer, sizeof(BITMAPFILEHEADER));
	memcpy(&*Bmp + sizeof(BITMAPFILEHEADER), &InfoHeadBuffer, sizeof(BITMAPINFOHEADER));
	memcpy(&*Bmp + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER), &*pColorDataMid, write_nData);
	NxData.imagedata = Bmp;
	return move(NxData);
}

#include "../hisfans/ykdef.h"

int sendimg(SOCKET client, shared_ptr<char> _Data, unsigned long long DataLen)
{
	char fBuffer[Pkg_Leng];
	unsigned long long latestpkg = 0ULL;
	for (unsigned long long DataPcs = 0; DataPcs < DataLen;)
	{
		if ((DataPcs + Pkg_Leng) > DataLen)
			latestpkg = DataLen - DataPcs;
		else latestpkg = Pkg_Leng;
		memcpy(fBuffer, &*_Data + DataPcs, latestpkg);
		send(client, &*_Data, latestpkg, 0);
		recv(client, (char*)&latestpkg, sizeof(unsigned long long), 0);
		DataPcs += Pkg_Leng;
	}
	return 1;
}

string Get_Domain_IP()
{
	hostent *phst = gethostbyname(Server_Host);
	in_addr * iddr = (in_addr*)phst->h_addr;
	unsigned long IPUL = iddr->s_addr;
	return move(string(inet_ntoa(*iddr)));
}

extern "C" _declspec (dllexport) BOOL NxRms()
{
	//��ʼ�����绷��  
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		printf("WSAStartup failed\n");
		return 0;
	}
	// ��ʼ����ɣ�����һ��TCP��socket  
	SOCKET sServer = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sServer == INVALID_SOCKET)
	{
		printf("socket failed\n");
		return 0;
	}
	//ָ�����ӵķ������Ϣ  
	SOCKADDR_IN addrServ;
	addrServ.sin_family = AF_INET;
	addrServ.sin_port = htons(S_Port - 1);
	//�ͻ���ֻ��Ҫ����ָ���ķ�������ַ��127.0.0.1�Ǳ����Ļػ���ַ  
	addrServ.sin_addr.S_un.S_addr = inet_addr((Get_Domain_IP()).c_str());

	// ������Bind �ͻ����ǽ�������  
	int ret = connect(sServer, (SOCKADDR*)&addrServ, sizeof(SOCKADDR));//��ʼ����  
	if (SOCKET_ERROR == ret)
	{
		printf("socket connect failed\n");
		WSACleanup();
		closesocket(sServer);
		return 0;
	}
	unsigned long long uxBuffer = 1;
	// YK Start Screen Monitoring
	int cx = GetSystemMetrics(SM_CXSCREEN);
	int cy = GetSystemMetrics(SM_CYSCREEN);
	unsigned long Failed_Counter;
	unsigned long long SleepTime = 0ULL;
	recv(sServer, (char*)&SleepTime, sizeof(unsigned long long), 0);
	while (uxBuffer)
	{
		auto FxImg = GetImg(480UL, 320UL);
		uxBuffer = FxImg.imagelong;
		if (send(sServer, &*(FxImg.imagedata), uxBuffer, 0) < 0)
		{
			Failed_Counter++;
			if (Failed_Counter > 10UL)break;
			continue;
		}
		if (sendimg(sServer, (FxImg.imagedata), uxBuffer) < 0)
		{
			Failed_Counter++;
			if (Failed_Counter > 10UL)break;
			continue;
		}
		else
		{
			Failed_Counter = 0UL;

			recv(sServer, (char*)&uxBuffer, sizeof(unsigned long long), 0);
			if (uxBuffer == 2ULL)
			{
				recv(sServer, (char*)&uxBuffer, sizeof(unsigned long long), 0);
				if (uxBuffer == Mou_Move)
				{
					recv(sServer, (char*)&uxBuffer, sizeof(unsigned long long), 0);
					double fW = double(cx) / Max_WSc * uxBuffer;
					recv(sServer, (char*)&uxBuffer, sizeof(unsigned long long), 0);
					double fH = cy / Max_HSc *uxBuffer;
					SetCursorPos(int(fW), int(fH));
				}
				else if (uxBuffer == Mou_L)
				{
					mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
					mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
				}
				else if (uxBuffer == Mou_R)
				{
					mouse_event(MOUSEEVENTF_RIGHTDOWN, 0, 0, 0, 0);
					mouse_event(MOUSEEVENTF_RIGHTUP, 0, 0, 0, 0);
				}
				else if (uxBuffer == Mou_DbC)
				{
					mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
					mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
					mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
					mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
				}
				else if (uxBuffer)
				{
					keybd_event(uxBuffer, 0, 0, 0);
					keybd_event(uxBuffer, 0, KEYEVENTF_KEYUP, 0);
				}
			}
		}
		Sleep(DWORD(SleepTime));
	}
	closesocket(sServer);
	WSACleanup();
	return 1;
}
