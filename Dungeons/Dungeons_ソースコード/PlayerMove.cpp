// ===�v���C���[�ړ��N���X����===
// �v���C���[�ړ��N���X������
// Author:	Kimura
// ==========================

// ===�C���N���[�h===
#include "PlayerMove.h"
#include "Input.h"
#include "DefineData.h"
#include "CameraManager.h"

// ===����������===
void PlayerMove::Init(Player* player)
{
	player->SetAnimeRate(0.1f);					// �A�j���[�V�����J�ڗ�������
	player->PlayerSetBlendAnime(ANIM_RUN);		// �A�j���[�V�����𑖂�ɃZ�b�g(�u�����h�Đ�)
	player->SetAnimeLoop(true);					// �A�j���[�V�������[�v

	// ---�R���g���[���[����---
	if (CInput::GetJoyCount() > 0)				// �R���g���[���[�ڑ�����1��ȏ゠���
		m_bController = true;
	else
		m_bController = false;

}

// ===�X�V����===
void PlayerMove::Update(Player* player)
{
	// ---�ϐ��錾---
	XMFLOAT3 vel = { 0,0,0 };					// �i�s����
	XMFLOAT2 stick = XMFLOAT2(0.0f,0.0f);		// �X�e�B�b�N���͕���

	// ---�R���g���[���[����---
	if (m_bController)				// �R���g���[���[�ڑ�����1��ȏ゠���
	{
		stick.x = CInput::GetStick(CInput::GetJoyX(JOYSTICKID1));			// �X�e�B�b�NX�����̓��͒l���擾
		stick.y = -CInput::GetStick(CInput::GetJoyY(JOYSTICKID1));			// �X�e�B�b�NY�����̓��͒l���擾
	}

	// ---�L�[����---
	if (CInput::GetKeyPress(VK_A)) vel.x = -1.0f;							// ������
	if (CInput::GetKeyPress(VK_W)) vel.z =  1.0f;							// ������
	if (CInput::GetKeyPress(VK_S)) vel.z = -1.0f;							// �����
	if (CInput::GetKeyPress(VK_D)) vel.x =  1.0f;							// �E����

	// ---�X�e�B�b�N�l�̕␳---
	if (fabs(stick.x) >= DEAD_STICK_X)			// �f�b�h�]�[���𒴂��Ă���Βl�𔽉f
	{
		Normalize(&stick);		// ���͕����𐳋K��(�X�e�B�b�N�̓|����ő��x��ς������ꍇ�͏C��)
		vel.x = stick.x;		// �i�s�����ɃX�e�B�b�N�̒l(X)�𔽉f
	}
	if (fabs(stick.y) >= DEAD_STICK_Y)			// �f�b�h�]�[���𒴂��Ă���Βl�𔽉f
	{
		Normalize(&stick);		// ���͕����𐳋K��(�X�e�B�b�N�̓|����ő��x��ς������ꍇ�͏C��)
		vel.z = stick.y;		// �i�s�����ɃX�e�B�b�N�̒l(Y)�𔽉f
	}

	// ---��O����---
	if (vel.x == 0.0f && vel.z == 0.0f)
	{
		player->SetVel(vel);
		player->SetStrategy(STAY_PLAYER);		// �ҋ@��ԂɑJ��
		return;
	}
	Normalize(&vel);							// �i�s�����x�N�g���𐳋K��

	// ---�����̌v�Z---
	XMFLOAT3 view = CAMERA->GetCurrentCamera()->GetLookVector();	// �J�����̎��_�x�N�g�����擾
	Quaternion q;													// ��]�N�H�[�^�j�I��
	view.y = 0;														// �v���C���[��Y����]���������̂Ŏ��_�x�N�g����Y�̒l�͖�������
	Quaternion::LookRotation(&q,view);								// ���_�����������N�H�[�^�j�I�����Z�o

	Quaternion p(vel);												// ���͕���
	p = -q * p * q;													// ��]�N�H�[�^�j�I���ŋ��ݍ��݁A�i�s�����̕␳������
	vel = XMFLOAT3(p.x, p.y, p.z);									// ���߂�������V�����i�s�����Ƃ���
	vel.x *= PLAYER_SPEED * 3.0f;									// �i�s���x(X)�����߂�
	vel.z *= PLAYER_SPEED * 3.0f;									// �i�s���x(Z)�����߂�

	XMFLOAT3 input = vel;											// Y�����̑��x�𖳎������l���i�[

	vel.y = player->GetVel().y;										// �i�s���x(Y)���擾
	player->SetVel(vel);											// �i�s���x���v���C���[�ɃZ�b�g

	float speed = sqrt(vel.x * vel.x + vel.y * vel.y + vel.z * vel.z);		// ���x�����߂�
	if (speed != 0)				// �i�s���x�����������߂��Ă���ꍇ
		player->SetFront(input);				// �v���C���[�̐��ʌ�����i�s�����ɃZ�b�g

	player->DelStamina(-0.5);					// �X�^�~�i����
}

// ===�X�V��Ԏ擾===
bool PlayerMove::IsUpdate(Player* player)
{
	return 	true;
}