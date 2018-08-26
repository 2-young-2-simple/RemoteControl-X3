
// DlgProxy.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "hisfoo.h"
#include "DlgProxy.h"
#include "hisfooDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ChisfooDlgAutoProxy

IMPLEMENT_DYNCREATE(ChisfooDlgAutoProxy, CCmdTarget)

ChisfooDlgAutoProxy::ChisfooDlgAutoProxy()
{
	EnableAutomation();
	
	// ΪʹӦ�ó������Զ��������ڻ״̬ʱһֱ���� 
	//	���У����캯������ AfxOleLockApp��
	AfxOleLockApp();

	// ͨ��Ӧ�ó����������ָ��
	//  �����ʶԻ���  ���ô�����ڲ�ָ��
	//  ָ��Ի��򣬲����öԻ���ĺ���ָ��ָ��
	//  �ô���
	ASSERT_VALID(AfxGetApp()->m_pMainWnd);
	if (AfxGetApp()->m_pMainWnd)
	{
		ASSERT_KINDOF(ChisfooDlg, AfxGetApp()->m_pMainWnd);
		if (AfxGetApp()->m_pMainWnd->IsKindOf(RUNTIME_CLASS(ChisfooDlg)))
		{
			m_pDialog = reinterpret_cast<ChisfooDlg*>(AfxGetApp()->m_pMainWnd);
			m_pDialog->m_pAutoProxy = this;
		}
	}
}

ChisfooDlgAutoProxy::~ChisfooDlgAutoProxy()
{
	// Ϊ������ OLE �Զ����������ж������ֹӦ�ó���
	//	������������ AfxOleUnlockApp��
	//  ���������������⣬�⻹���������Ի���
	if (m_pDialog != NULL)
		m_pDialog->m_pAutoProxy = NULL;
	AfxOleUnlockApp();
}

void ChisfooDlgAutoProxy::OnFinalRelease()
{
	// �ͷ��˶��Զ�����������һ�����ú󣬽�����
	// OnFinalRelease��  ���ཫ�Զ�
	// ɾ���ö���  �ڵ��øû���֮ǰ�����������
	// ��������ĸ���������롣

	CCmdTarget::OnFinalRelease();
}

BEGIN_MESSAGE_MAP(ChisfooDlgAutoProxy, CCmdTarget)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(ChisfooDlgAutoProxy, CCmdTarget)
END_DISPATCH_MAP()

// ע��: ��������˶� IID_Ihisfoo ��֧��
//  ��֧������ VBA �����Ͱ�ȫ�󶨡�  �� IID ����ͬ���ӵ� .IDL �ļ��е�
//  ���Ƚӿڵ� GUID ƥ�䡣

// {04A76836-E4B7-4BB8-8FF2-AEC5DD5AA23C}
static const IID IID_Ihisfoo =
{ 0x4A76836, 0xE4B7, 0x4BB8, { 0x8F, 0xF2, 0xAE, 0xC5, 0xDD, 0x5A, 0xA2, 0x3C } };

BEGIN_INTERFACE_MAP(ChisfooDlgAutoProxy, CCmdTarget)
	INTERFACE_PART(ChisfooDlgAutoProxy, IID_Ihisfoo, Dispatch)
END_INTERFACE_MAP()

// IMPLEMENT_OLECREATE2 ���ڴ���Ŀ�� StdAfx.h �ж���
// {E15DCE55-7DFD-418F-8A09-960A02D1E97E}
IMPLEMENT_OLECREATE2(ChisfooDlgAutoProxy, "hisfoo.Application", 0xe15dce55, 0x7dfd, 0x418f, 0x8a, 0x9, 0x96, 0xa, 0x2, 0xd1, 0xe9, 0x7e)


// ChisfooDlgAutoProxy ��Ϣ�������
