#include "Object.h"

// ===�I�u�W�F�N�g�^����`===
namespace ObjectTag
{
	LPCSTR tag[MAX_GAMEOBJTYPE] = {					// �G�f�B�^��ŕ\������I�u�W�F�N�g��ޖ�
		"GameObject",		// �^�Ȃ�
		"Player",			// �v���C���[
		"Enemy",			// �G�l�~�[
		"Floor",			// �n��
		"Wall",				// ��
		"Pillar",			// ��
		"3DModel",			// �w�i�I�u�W�F�N�g
		"2DGameObject",		// 2D�I�u�W�F�N�g
		"TitleBG",			// �^�C�g�����S
		"EnterButton",		// �{�^��
		"Fence",			// �t�F���X
		"None",				// �v���C���[����
		"WallPillar",		// �ǒ�
		"Bullet",			// �e
		"Door",				// �h�A
		"BulletEnemy",		// �������G�l�~�[
		"StartButton",		// �X�^�[�g�{�^��
		"TitleButton",		// �^�C�g���{�^��
		"FinButton",		// �I���{�^��
		"Life",				// ���C�t
		"Stamina",			// �X�^�~�i
		"GameOver",			// �Q�[���I�[�o�[���
		"GameClear",		// �Q�[���N���A���
		"Goal",				// �S�[���I�u�W�F�N�g
		"Score",			// �X�R�AUI
		"ScoreEff",			// �X�R�A�G�t�F�N�g
	};
}

// ===�R���X�g���N�^===
Object::Object() : m_name("�I�u�W�F�N�g"),m_ResourcePass("")
{
	// ---����������---
	m_id = OT_OBJ;			// �I�u�W�F�N�gid

	m_transform = Transform();								// ���W��������
	m_qRot = Quaternion(XMFLOAT3(0.0f, 1.0f, 0.0f), 0.0f);	// ��]
	m_vVel = XMFLOAT3(0.0f, 0.0f, 0.0f);					// ���x
	m_vAcc = XMFLOAT3(0.0f, 0.0f, 0.0f);					// �����x

	m_bGravity = false;										// �d�̓t���O
	m_bEnable = false;										// �X�V�E�`��L��(false�ōX�V�A�`���~)
	m_bVisible = false;										// �����邩�ǂ���(true�ŉ�)
	m_col = false;											// �����蔻�����邩�ǂ���
	m_bStatic = false;										// �ÓI�I�u�W�F�N�g���ǂ���
	m_bSelect = false;										// �I������Ă��邩�ǂ���

	m_fRadius = 1.0f;										// ���E���T�C�Y

	XMStoreFloat4x4(&m_mWorld, XMMatrixIdentity());			// ���[���h�}�g���b�N�X������
	m_nInvincibleTime = 0;									// ���G����
}

// ===�f�X�g���N�^===
Object::~Object()
{
}

// ===�X�V�E�`��L���ݒ�===
void Object::SetEnable(bool flg)
{
	m_bEnable = flg;
}

// ===�d�͉ېݒ�===
void Object::SetGravity(bool flg)
{
	m_bGravity = flg;
}

// ===�����蔻��ېݒ�===
void Object::SetCol(bool flg)
{
	m_col = flg;
}

// ===�I�u�W�F�N�g���W�ݒ�===
void Object::SetPos(XMFLOAT3 pos)
{
	m_transform.Pos = pos;
}

// ===�I�u�W�F�N�g��]�ݒ�===
void Object::SetRot(XMFLOAT3 rot)
{
	m_transform.Rot = rot;
}

// ===�I�u�W�F�N�g�g�k�ݒ�===
void Object::SetScl(XMFLOAT3 scl)
{
	m_transform.Scale = scl;
}

// ===�I�u�W�F�N�g���x�ݒ�===
void Object::SetVel(XMFLOAT3 vel)
{
	m_vVel = vel;
}

// ===�I�u�W�F�N�g�����x�ݒ�===
void Object::SetAcc(XMFLOAT3 acc)
{
	m_vAcc = acc;
}

// ===�I�u�W�F�N�g���E���T�C�Y�ݒ�===
void Object::SetRad(float r)
{
	m_fRadius = r;
}

// ===�I�u�W�F�N�g���ݒ�===
void Object::SetName(std::string name)
{
	m_name = name;
}

// ===�I�u�W�F�N�g�I����Ԑݒ�===
void Object::SetSelect(bool flg)
{
	m_bSelect = flg;
}

// ===�ÓI�I�u�W�F�N�g�ݒ�===
void Object::SetStatic(bool flg)
{
	m_bStatic = flg;
}

// ===���W�E��]�E�g�k�ݒ�===
void Object::SetTransform(Transform transform)
{
	m_transform = transform;
}

// ===���[���h�ϊ��s��ݒ�===
void Object::SetWorld(const XMFLOAT4X4& mtx)
{
	m_mWorld = mtx;
}

// ===�I�u�W�F�N�g�^���擾===
LPCSTR GetObjectTagName(ObjType type)
{
	return ObjectTag::tag[type];
}

// ===�I�u�W�F�N�g���G���Ԑݒ�===
void Object::SetInvincible(const int& time)
{
	m_nInvincibleTime = time;
}