#pragma once
// ===�I�u�W�F�N�g�����N���X��`====
// �I�u�W�F�N�g�������s���N���X���`
// Author Kimura
// ==============================

// ===�C���N���[�h===
#include "Object.h"
#include "main.h"

// ===�N���X��`===
class ObjectFactrory
{
	// ---�����o�֐�---
public:
	static Object* CreateObject(ObjType);		// �I�u�W�F�N�g����
};