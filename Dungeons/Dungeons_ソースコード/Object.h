#pragma once
// ===�I�u�W�F�N�g�x�[�X�N���X��`===
// �I�u�W�F�N�g�x�[�X�N���X���`
// Author:	Kimura
// ==============================

// ===�C���N���[�h===
#include "main.h"
#include "Calc.h"
#include <string>
#include "Line.h"

// ===�I�u�W�F�N�g�̃^�O��`===
//	���t�@�C���Ƀ^�O�̏����f���o�����߁A�I�u�W�F�N�g��ǉ�����ۂ͉��ɑ����Ă���(�ԂɃ^�O���ǉ������ƌ��X�̒�`���炻�̕����ɂ���Ă��܂�)
enum ObjType {			// �I�u�W�F�N�g�^
	OT_OBJ = 0,			// �^����
	OT_PLAYER,			// �v���C���[
	OT_ENEMY,			// �G
	OT_GROUND,			// �n��
	OT_WALL,			// ��
	OT_PILLAR,			// ��
	BG_3DOBJ,			// �w�i�I�u�W�F�N�g
	TEST_2DOBJ,			// �e�X�g�I�u�W�F�N�g
	BG_TITLE,			// �^�C�g�����S
	BG_ENTER,			// UI
	OT_FENCE,			// �t�F���X
	PLAYER_ATACK,		// �v���C���[�̍U�������蔻��
	OT_WALLPILLAR,		// �ʉ���
	ENEMY_ATTACK,		// �G�̍U������
	OT_DOOR,			// �h�A
	OT_ENEMY_BULLET,	// �������G
	UI_START_BUTTON,	// �{�^���e�X�g
	UI_TITLE_BUTTON,	// �^�C�g���{�^��
	UI_FIN_BUTTON,		// �I���{�^��
	UI_LIFE,			// �̗�UI
	UI_STAMINA,			// �X�^�~�iUI
	BG_GAMEOVER,		// �Q�[���I�[�o�[�w�i
	BG_GAMECLEAR,		// �Q�[���N���A�w�i
	OT_GOAL,			// �S�[���I�u�W�F�N�g
	UI_SCORE,			// �X�R�AUI
	BILL_SCORE,			// �X�R�A�G�t�F�N�g

	MAX_GAMEOBJTYPE
};

// ===�N���X��`===
class Object
{
	// ---�����o�֐�---
public:
	Object();				// �R���X�g���N�^
	virtual ~Object();		// �f�X�g���N�^

	virtual void DrawShadow() { ; }		// �e�`��

	// ---�l�̃Z�b�g---
	void SetEnable(bool);		// �X�V�̗L�����Z�b�g
	void SetGravity(bool);		// �d�̗͂L�����Z�b�g
	void SetCol(bool);
	void SetPos(XMFLOAT3);
	void SetRot(XMFLOAT3);
	void SetScl(XMFLOAT3);
	void SetVel(XMFLOAT3);
	void SetAcc(XMFLOAT3);
	void SetRad(float);
	void SetName(std::string);
	void SetStatic(bool);
	void SetSelect(bool);
	void SetTransform(Transform);
	void SetWorld(const XMFLOAT4X4&);

	void SetInvincible(const int& time);

	virtual bool IsUpdate() = 0;	// �X�V�������ǂ���
	virtual void SetResource(std::string) = 0;
	virtual void OnCollision(Object*) { ; }

	// ---�l�̎擾---
	Transform& GetTransform() { return m_transform; }				// �I�u�W�F�N�g���W�����擾
	XMFLOAT3& GetPos() { return m_transform.Pos; }					// ���W�擾
	XMFLOAT3& GetScale() { return m_transform.Scale; }				// �g�k�擾
	XMFLOAT3& GetRotate() { return m_transform.Rot; }				// ��]�擾
	Quaternion& GetQuaternion() { return m_qRot; }					// ��](�N�H�[�^�j�I���擾)
	bool& GetEnable() { return m_bEnable; }							// �擾
	bool& GetGravity() { return m_bGravity; }						// �d�͎擾
	bool& GetVisible() { return m_bVisible; }						// ����Ԏ擾
	bool& GetCol() { return m_col; }								// �����蔻���
	bool& GetStatic() { return m_bStatic; }							// �I�u�W�F�N�g��Ԏ擾
	bool& GetSelect() { return m_bSelect; }							// �Z���N�g��Ԏ擾
	float& GetRad() { return m_fRadius; }							// ���E�����a�擾
	XMFLOAT3& GetVel() { return m_vVel; }							// ���x�擾
	XMFLOAT3& GetAcc() { return m_vAcc; }							// �����x�擾
	ObjType GetObjID() { return m_id; }								// �I�u�W�F�N�g�^�C�v�擾
	XMFLOAT4X4& GetWorld() { return m_mWorld; }						// ���[���h�}�g���b�N�X�擾
	std::string GetName() { return m_name; }						// ���O�擾
	std::string GetResourcePass() { return m_ResourcePass; }		// ���\�[�X�ւ̃p�X���擾

	bool GetInvincible() { return m_nInvincibleTime > 0; }			// ���G���Ԓ����ǂ���
	int& GetInvincibleTime() { return m_nInvincibleTime; }

	virtual void UpdateMatrix() = 0;								// ���[���h�}�g���N�X�X�V

	virtual void ImGuiWindow() { ; }

	// ---�����o�ϐ�---
protected:
	ObjType m_id;					// �I�u�W�F�N�gid

	Transform m_transform;			// ���W�A�g�k�A��]
	Quaternion m_qRot;
	XMFLOAT3 m_vVel;				// ���x
	XMFLOAT3 m_vAcc;				// �����x

	bool m_bGravity;				// �d�̓t���O
	bool m_bEnable;					// ���݂��邩(false�ōX�V�A�`���~)
	bool m_bVisible;				// �����邩�ǂ���(true�ŉ�)
	bool m_col;						// �����蔻�����邩�ǂ���
	bool m_bStatic;					// �����I�u�W�F�N�g���ǂ���
	bool m_bSelect;					// �G�f�B�^��őI������Ă��邩�ǂ���

	float m_fRadius;				// ���E���T�C�Y

	XMFLOAT4X4 m_mWorld;			// ���[���h�ϊ��}�g���b�N�X

	std::string m_name;				// �I�u�W�F�N�g��
	std::string m_ResourcePass;		// ���f����摜�Ȃǂ̃��\�[�X�p�X

	int m_nInvincibleTime;			// ���G����
};

// ===�֐���`===
LPCSTR GetObjectTagName(ObjType);	// �G�f�B�^��ŕ\������I�u�W�F�N�g�̌^���擾