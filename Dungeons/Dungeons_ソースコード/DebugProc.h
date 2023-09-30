//=============================================================================
//
// �f�o�b�O�\���N���X��` [DebugProc.h]
//
//=============================================================================
#pragma once
#include "main.h"
#include "Texture.h"

class CDebugProc
{
private:
	static Texture* m_pTexture;						// �e�N�X�`���ւ̃|�C���^
	static char m_szDebug[8192];					// �f�o�b�O���
	static bool m_bHiragana;						// �������t���O

public:
	static HRESULT Init();
	static void Fin();
	static void Draw();

	static void Start(bool hiragana = false);
	static int Print(const char *fmt, ...);
	static bool SetHiragana(bool hiragana = true);
};
