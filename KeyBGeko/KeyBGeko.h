// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
// ��ı�׼�������� DLL �е������ļ��������������϶���� KEYBGEKO_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
// �κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ
// KEYBGEKO_API ������Ϊ�Ǵ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�
#ifdef KEYBGEKO_EXPORTS
#define KEYBGEKO_API __declspec(dllexport)
#else
#define KEYBGEKO_API __declspec(dllimport)
#endif

// �����Ǵ� KeyBGeko.dll ������
class KEYBGEKO_API CKeyBGeko {
public:
	CKeyBGeko(void);
	// TODO:  �ڴ�������ķ�����
};

extern KEYBGEKO_API int nKeyBGeko;

KEYBGEKO_API int fnKeyBGeko(void);
