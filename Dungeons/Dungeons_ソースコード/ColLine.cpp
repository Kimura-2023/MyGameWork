// ===�����蔻��(����)�N���X����===
// �����蔻��(����)�N���X������
// Author:	Kimura
// ==============================

// ===�C���N���[�h===
#include "ColLine.h"
#include "Light.h"
#include "DebugProc.h"
#include "ColSphere.h"
#include "Calc.h"

// ===�ÓI�����o===
Line* CollLine::m_Line = nullptr;				// �f�o�b�O�\���p
int CollLine::m_nCnt = 0;						// ����

// ===�R���X�g���N�^(�I�t�Z�b�g�Ȃ�)===
CollLine::CollLine(Collision3D* col, XMFLOAT3 end)
{
	// ---����������---
	Init();							// ������
	m_vec = CalcUnitVector(end);	// �x�N�g���ݒ�
	m_End = end;					// �I�_���W�ݒ�
	SetParent(col);					// �e���Z�b�g

	m_Line->AddLine(m_Pos, m_End);	// �����ݒ�
}

// ===�R���X�g���N�^(�I�t�Z�b�g����)===
CollLine::CollLine(Collision3D* col, XMFLOAT3 off, XMFLOAT3 end)
{
	// ---����������---
	Init();							// ������
	m_Offset = off;					// �I�t�Z�b�g�ݒ�
	m_Pos.x += m_Offset.x;			// ���S���W(x)�X�V
	m_Pos.y += m_Offset.y;			// ���S���W(y)�X�V
	m_Pos.z += m_Offset.z;			// ���S���W(z)�X�V

	m_vec = CalcUnitVector(end);	// �x�N�g���ݒ�
	m_End = end;					// �I�_���W�ݒ�
	SetParent(col);					// �e���Z�b�g

	m_Line->AddLine(m_Pos, m_End);	// �����ݒ�
}

// ===�f�X�g���N�^===
CollLine::~CollLine()
{
	--m_nCnt;						// �����蔻�葍������
	if (m_nCnt <= 0)
	{
		m_Line->Fin();				// �f�o�b�O�\���I��
		SAFE_DELETE(m_Line);		// ���
	}
}

// ===����������===
void CollLine::Init()
{
	// ---������---
	m_vec = m_End = XMFLOAT3(1.0f, 1.0f, 1.0f);		// �I�_���W�A�x�N�g���Z�b�g
	m_Pos = GetCenter();			// ���S���W�Z�b�g
	m_coltype = COL_LINE;			// �����蔻��^�ݒ�

	if (m_nCnt == 0)
	{
		m_Line = new Line();		// ����
	}
	m_nCnt++;						// �����蔻�葍�����Z
}

// ===�q�b�g�m�F===
bool CollLine::CheckHit(CollisionBase* col)
{
	// ---���O����---
	if (col->GetFollowCollision() == GetFollowCollision())
		return false;
	if (!col->GetEnable())
		return false;

	// ---�q�b�g�m�F---
	XMFLOAT3 pos = col->GetCenter();											// ������W

	switch (col->GetColType())			// ���蓖���蔻��ɂ���ď�����ؑ�
	{
	case COL_SPHERE: {					// ---���E��

		XMFLOAT3 vline = GetVec();					// �����̃x�N�g�����Z�o
		float xa = m_Pos.x - pos.x;
		float ya = m_Pos.y - pos.y;
		float za = m_Pos.z - pos.z;
		float rad = ((CollSphere*)col)->GetRadius();

		float a = vline.x * vline.x + vline.y * vline.y + vline.z * vline.z;
		float b = 2.0f * (vline.x * xa + vline.y * ya + vline.z * za);
		float c = (xa * xa) + (ya * ya) + (za * za) - (rad * rad);

		// ---�ꍇ����������---
		// �ub^2 - 4ac�v�̒l��0�ȉ��̏ꍇ�͏Փ˂Ȃ�
		float d = (b * b) - (4.0f * a * c);			// b^2 - 4ac
		if (d < 0)
		{
			m_bHit = false;
			return false;
		}

		// ---t�̒l(2���)�����߂�---
		d = sqrtf(d);
		float t1 = (-b + d) / (2.0f * a);
		float t2 = (-b - d) / (2.0f * a);

		// ---�����̍Ŋ��(�ŏ��l)��ݒ�---
		float t = 2.0f;
		if (t1 >= 0.0f && t1 <= 1.0f && t1 < t) {
			t = t1;
		}
		if (t2 >= 0.0f && t2 <= 1.0f && t2 < t) {
			t = t2;
		}

		// ---�Փ˂Ȃ�---
		if (t > 1.0f)
		{
			m_bHit = false;
			return false;
		}

		// ---�Փ�---
		CDebugProc::Print("˯�\n");
		m_bHit = true;
		return true;
		break;
	}
	case COL_NONE:
		break;
	default:
		break;
	}
	return false;
}

// ===�f�o�b�O�\��===
void CollLine::Draw()
{
	// ---���O����---
	if (!GetVisible())
		return;

	// ---�`�揈��---
	m_Line->SetWorldMatrix(GetWorld());				// ���[���h�ϊ��s����Z�b�g
	m_Line->SetMaterial(m_Material.m_Diffuse);		// �}�e���A�����Z�b�g
	m_Line->DrawLines();							// �����`��
}

// ===�I�_���W�擾===
XMFLOAT3 CollLine::GetEnd()
{
	return m_End;
}

// ===�x�N�g���擾===
XMFLOAT3 CollLine::GetVec()
{
	// ---�I�_�̍��W���X�V(���S���W�Ɛ����x�N�g������I�_�����߂�)---
	m_End.x = m_Pos.x + m_vec.x;
	m_End.y = m_Pos.y + m_vec.y;
	m_End.z = m_Pos.z + m_vec.z;

	// ---�x�N�g����Ԃ�---
	return XMFLOAT3(m_End.x - m_Pos.x, m_End.y - m_Pos.y, m_End.z - m_Pos.z);
}