
// hisfoo.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// ChisfooApp: 
// �йش����ʵ�֣������ hisfoo.cpp
//

class ChisfooApp : public CWinApp
{
public:
	ChisfooApp();

// ��д
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern ChisfooApp theApp;
