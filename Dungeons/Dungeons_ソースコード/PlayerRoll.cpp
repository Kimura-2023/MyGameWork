// ===�v���C���[����N���X����===
// �v���C���[����N���X������
// Author:	Kimura
// ==========================

// ===�C���N���[�h===
#include "PlayerRoll.h"
#include "Input.h"
#include "Sound.h"
#include "GameManager.h"
#include "DefineData.h"
#include "EffectManager.h"
#include "CameraManager.h"

// ===����������===
void PlayerRoll::Init(Player* player)
{
	if (player->GetStamina() <= 10)			// �X�^�~�i�Ŕ���
		return;

	// ---�p�����[�^������---
	player->SetAnimeTime(0.0f);				// �A�j���[�V�������Ԃ�ݒ�
	player->SetAnimeRate(0.0f);				// �A�j���[�V�����J�ڗ���ݒ�
	player->SetAnimeLoop(false);			// ���[�v�ݒ�
	player->DelStamina(10);					// �X�^�~�i����

	CSound::Play(SE_DUSH);					// �_�b�V�����Đ�

	// ---�����v�Z---

	XMFLOAT3 vec = player->GetFront();
	if (CInput::GetKeyTrigger(VK_SPACE))			// --�}�E�X�̏ꍇ
	{
		vec = GAME->MousePos();						// �}�E�X���W���擾
		XMFLOAT3 pos = player->GetTransform().Pos;	// �v���C���[�̍��W���擾
		vec.x -= pos.x;				// ����(x����)���Z�o
		vec.z -= pos.z;				// ����(z����)���Z�o
		vec.y = 0.0f;				// �����͖�������
		Normalize(&vec);			// �x�N�g���𐳋K��
		player->SetFront(vec);		// �v���C���[�̐��ʕ������Z�o�����x�N�g���ɐݒ�
	}
	else											// ----�R���g���[���[
	{
		XMFLOAT2 stick = XMFLOAT2(0.0f, 0.0f);		// �X�e�B�b�N���͕���
		stick.x = CInput::GetStick(CInput::GetJoyX(JOYSTICKID1));			// �X�e�B�b�NX�����̓��͒l���擾
		stick.y = -CInput::GetStick(CInput::GetJoyY(JOYSTICKID1));			// �X�e�B�b�NY�����̓��͒l���擾

		// ---�X�e�B�b�N�l�̕␳---
		if (fabs(stick.x) >= DEAD_STICK_X)			// �f�b�h�]�[���𒴂��Ă���Βl�𔽉f
		{
			Normalize(&stick);		// ���͕����𐳋K��(�X�e�B�b�N�̓|����ő��x��ς������ꍇ�͏C��)
			vec.x = stick.x;		// �i�s�����ɃX�e�B�b�N�̒l(X)�𔽉f
		}
		if (fabs(stick.y) >= DEAD_STICK_Y)			// �f�b�h�]�[���𒴂��Ă���Βl�𔽉f
		{
			Normalize(&stick);		// ���͕����𐳋K��(�X�e�B�b�N�̓|����ő��x��ς������ꍇ�͏C��)
			vec.z = stick.y;		// �i�s�����ɃX�e�B�b�N�̒l(Y)�𔽉f
		}
		Normalize(&vec);

		// ---�����̌v�Z---
		XMFLOAT3 view = CAMERA->GetCurrentCamera()->GetLookVector();	// �J�����̎��_�x�N�g�����擾
		Quaternion q;													// ��]�N�H�[�^�j�I��
		view.y = 0;														// �v���C���[��Y����]���������̂Ŏ��_�x�N�g����Y�̒l�͖�������
		Quaternion::LookRotation(&q, view);								// ���_�����������N�H�[�^�j�I�����Z�o

		Quaternion p(vec);												// ���͕���
		p = -q * p * q;													// ��]�N�H�[�^�j�I���ŋ��ݍ��݁A�i�s�����̕␳������
		vec = XMFLOAT3(p.x, p.y, p.z);									// ���߂�������V�����i�s�����Ƃ���
		XMFLOAT3 input = vec;
		vec.y = player->GetVel().y;												// �i�s���x(Y)���擾
		float speed = sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);		// ���x�����߂�
		if (speed != 0)				// �i�s���x�����������߂��Ă���ꍇ
			player->SetFront(input);				// �v���C���[�̐��ʌ�����i�s�����ɃZ�b�g
	}

	// ---���x�ݒ�---
	player->SetSpeed(PLAYER_SPEED * 6.0f);			// ���x�ݒ�
	vec.x *= player->GetSpeed();					// �i�s���x���Z�o(x����)
	vec.z *= player->GetSpeed();					// �i�s���x���Z�o(y����)
	player->SetVel(vec);							// ���x��ݒ�
	player->SetAcc(XMFLOAT3(player->GetAcc().x,0.0f,player->GetAcc().z));		// �����x�ݒ�

	// ---�A�j���[�V�����ݒ�---
	player->PlayerSetBlendAnime(ANIM_ROLL);			// �A�j���[�V�����J�ڐݒ�
}

// ===�X�V����===
void PlayerRoll::Update(Player* player)
{
	player->SetGravity(false);						// �d�͖�����
	if (player->GetEndAnime())						// �A�j���[�V�����I������
		player->SetStrategy(STAY_PLAYER);			// �v���C���[��ԑJ��
	EFFECT->Play(MOVE_EFFECT, player->GetPos());	// �G�t�F�N�g�Đ�
}

// ===�X�V�L��===
bool PlayerRoll::IsUpdate(Player* player)
{
	return player->GetEndAnime();					// �A�j���[�V�����I������
}