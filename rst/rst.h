// rst.h : rst DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CrstApp
// �йش���ʵ�ֵ���Ϣ������� rst.cpp
//

class CrstApp : public CWinApp
{
public:
	CrstApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
