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
