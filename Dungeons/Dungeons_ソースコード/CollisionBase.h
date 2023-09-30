#pragma once

// ===�����蔻��N���X��`===
// �����蔻��N���X���`
// Author:	Kimura
// ==============================

// ===�C���N���[�h===
#include "main.h"
#include "Object.h"
#include "Collision3D.h"
#include "Mesh.h"

// ===�����蔻����===
enum COLTYPE
{
	COL_SPHERE,				// ���E��
	COL_AABB,				// AABB
	COL_OBB,				// OBB
	COL_LINE,				// ����
	COL_NONE,
};

// ===�N���X��`===
class Collision3D;
class CollisionBase
{
	// ---�����o�֐�---
public:
	CollisionBase();				// �R���X�g���N�^
	virtual ~CollisionBase();		// �f�X�g���N�^
	virtual void Init();			// ������

	Collision3D* GetFollowCollision(void);		// �e�����蔻��擾
	XMFLOAT3& GetCenter();						// ���S���W�擾
	XMFLOAT4X4& GetWorld();						// ���[���h�s��擾
	bool GetEnable();							// Enable
	bool GetVisible();							// �f�o�b�O�\��
	bool GetHit();								// �Փˏ�Ԏ擾
	COLTYPE GetColType();						// �����蔻���ގ擾

	void Update();								// �X�V����
	virtual void Draw() = 0;					// �`�揈��
	virtual bool CheckHit(CollisionBase *) = 0;	// �����������ǂ���

	void SetParent(Collision3D*);	// �e���Z�b�g
	void SetEnable(bool);			// Enable�Z�b�g
	void SetVisible(bool);			// ���Z�b�g
private:
	void UpdateMatrix();			// ���[���h�}�g���b�N�X�X�V

	// ---�����o�ϐ�---
protected:
	COLTYPE m_coltype;				// �����蔻����
	XMFLOAT3 m_Pos;					// �����蔻��v�Z�p���W
	XMFLOAT3 m_Scale;				// �����蔻��v�Z�p�T�C�Y
	XMFLOAT3 m_Rot;					// �����蔻���]��
	XMFLOAT3 m_Offset;				// �I�u�W�F�N�g���S���W����̂���
	bool m_Enable;					// ����
	bool m_Visible;					// �����
	bool m_bHit;					// �Փˏ��
	CMeshMaterial m_Material;		// �f�o�b�O�\���̃}�e���A�����
	Collision3D* m_pParent;			// �e�����蔻��
private:

	XMFLOAT4X4 m_World;		// ���[���h�s��
};
