
// DlgProxy.h: ͷ�ļ�
//

#pragma once

class ChisfooDlg;


// ChisfooDlgAutoProxy ����Ŀ��

class ChisfooDlgAutoProxy : public CCmdTarget
{
	DECLARE_DYNCREATE(ChisfooDlgAutoProxy)

	ChisfooDlgAutoProxy();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��

// ����
public:
	ChisfooDlg* m_pDialog;

// ����
public:

// ��д
	public:
	virtual void OnFinalRelease();

// ʵ��
protected:
	virtual ~ChisfooDlgAutoProxy();

	// ���ɵ���Ϣӳ�亯��

	DECLARE_MESSAGE_MAP()
	DECLARE_OLECREATE(ChisfooDlgAutoProxy)

	// ���ɵ� OLE ����ӳ�亯��

	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};

