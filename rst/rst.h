// rst.h : rst DLL ����ͷ�ļ�
//

#pragma once

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
