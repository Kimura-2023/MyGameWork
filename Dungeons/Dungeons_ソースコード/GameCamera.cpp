// ===�Q�[���J�����N���X����===
// �Q�[���J�����N���X������
// Author:	Kimura
// ==============================

// ===�C���N���[�h===
#include "GameCamera.h"
#include "DebugProc.h"
#include "ModelManager.h"
#include "Calc.h"

// ===�O���[�o���ϐ�===
namespace {
	const float VALUE_MOVE_CAMERA = 0.5f;			// �J�����ړ���
	const float VALUE_ROTATE_CAMERA = 0.5f;			// �J������]��

	const float POS_P_X = 8.0f;						// �O�l�̃J�����̎��_�ʒu(X���W)
	const float POS_P_Y = 18.0f;					// �J�����̎��_�����ʒu(Y���W)
	const float POS_P_Z = -12.0f;					// �J�����̎��_�����ʒu(Z���W)
	const float POS_R_X = 0.0f;						// �O�l�̃J�����̒����_�ʒu(X���W)
	const float POS_R_Y = 0.0f;						// �O�l�̃J�����̒����_�ʒu(Y���W)
	const float POS_R_Z = 0.0f;						// �O�l�̃J�����̒����_�ʒu(Z���W)
}

// ===�R���X�g���N�^===
GameCamera::GameCamera()
{
	// ---����������---
	Init();			// ������
	m_fShakeFrame = 0.0f;		// �U�����Ԃ�0�ɐݒ�
	m_fShakeRad = 0.0f;			// �U���ʒu��0�ɐݒ�
}

// ===����������===
void GameCamera::Init()
{
	CameraBase::Init();					// �J����������

	m_vDestPos = m_vPos;				// ���_�Z�b�g
	m_vDestTarget = m_vTarget;			// �����_�Z�b�g

	m_Target = nullptr;					// �Ǐ]����I�u�W�F�N�g���Z�b�g
}

// ===�X�V===
void GameCamera::Update()
{
	// ---�Ǐ]���邩����---
	if (m_Target != nullptr) {

		XMFLOAT3 p = m_Target->GetTransform().Pos;

		// ---�ړI���W---
		m_vDestPos.x = POS_P_X + p.x;			// ---���_���W�ړ�����Z�o
		m_vDestPos.y = POS_P_Y + p.y;
		m_vDestPos.z = POS_P_Z + p.z;
		m_vDestTarget.x = POS_R_X + p.x;		// ---�����_���W�ړ�����Z�o
		m_vDestTarget.y = POS_R_Y + p.y;
		m_vDestTarget.z = POS_R_Z + p.z;

		// ---���W�X�V---
		m_vPos.x = m_vPos.x * 0.92f + m_vDestPos.x * 0.08f;				// ---���_�����X�ɖړI���W�ɋ߂Â���
		m_vPos.y = m_vPos.y * 0.92f + m_vDestPos.y * 0.08f;
		m_vPos.z = m_vPos.z * 0.92f + m_vDestPos.z * 0.08f;

		m_vTarget.x = m_vTarget.x * 0.92f + m_vDestTarget.x * 0.08f;	// ---�����_���W�����X�Ɉړ���ɋ߂Â���
		m_vTarget.y = m_vTarget.y * 0.92f + m_vDestTarget.y * 0.08f;
		m_vTarget.z = m_vTarget.z * 0.92f + m_vDestTarget.z * 0.08f;
	}

	// ---�Ǐ]���Ȃ��ꍇ---
	else
	{
		m_vPos.x = POS_P_X;			// ---�������W���Z�b�g
		m_vPos.y = POS_P_Y;
		m_vPos.z = POS_P_Z;
	}

	// ---�J�����U������---
	if (m_fShakeFrame > 0.0f)
	{
		--m_fShakeFrame;											// �U�����Ԃ��X�V
		m_fShakeRad += XMConvertToRadians(m_fShakeFrame * 6.0f);		// �h��̑傫������萔�l�ɔ[�߂�

		if (m_fShakeRad > XM_PI * 2.0f)
		{
			m_fShakeRad -= XM_PI * 2.0f;									// 
		}

		m_vPosGap.x = (m_fShakeFrame / 60.0f) * cosf(m_fShakeRad);								// �U����̈ʒu���Z�o

		if (m_fShakeFrame == 0.0f)
		{
			m_fShakeFrame = 0.0f;									// �U������0�Ń��Z�b�g
			m_vPosGap = XMFLOAT3(0.0f, 0.0f, 0.0f);					// ���W��������Z�b�g
		}
	}

	CameraBase::Update();			// �J�����X�V
}

// ===�Ǐ]�I�u�W�F�N�g�ݒ�===
void GameCamera::SetTargetObj(Object* obj)
{
	m_Target = obj;				// �^�[�Q�b�g�ݒ�
	if (!m_Target)
		return;
	m_vTarget = m_Target->GetTransform().Pos;		// �����_��ݒ�
}

// ===�J�����U������===
void GameCamera::Shake(float second)
{
	m_fShakeFrame = second * 60.0f;					// �U�����Ԃ�ݒ�
}