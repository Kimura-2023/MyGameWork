// ===�v���C���[�N���X����===
// �v���C���[�N���X������
// Author:	Kimura
// ==========================

// ===�C���N���[�h===
#include "Player.h"
#include "DebugProc.h"
#include "ModelManager.h"
#include "PlayerStay.h"
#include "PlayerMove.h"
#include "PlayerAttack.h"
#include "PlayerAttack2.h"
#include "PlayerRoll.h"
#include "PlayerRepel.h"
#include "Input.h"
#include "GameManager.h"
#include "DefineData.h"
#include "CollisionManager.h"
#include "Calc.h"
#include "Sound.h"
#include "ShaderList.h"
#include "TextureManager.h"
#include "TimeController.h"
#include "Door.h"
#include "MapManager.h"
#include "EffectManager.h"
#include "Fade.h"
#include "ScoreManager.h"
#include "CameraManager.h"

// ===�R���X�g���N�^===
Player::Player() : Collision3D(this)
{
	// ---����������---
	m_PlayerState = STAY_PLAYER;						// ��Ԑݒ�

	m_id = OT_PLAYER;									// �I�u�W�F�N�g�^�ݒ�
	m_transform.Pos = XMFLOAT3(0.0f, 0.0f, 0.0f);		// �������W
	m_transform.Rot = XMFLOAT3(0.0f, 0.0f, 0.0f);		// ������]
	m_transform.Scale = XMFLOAT3(1.0f, 1.0f, 1.0f);		// �����g�k
	m_fRadius = 2.5f;									// ���E�����a
	m_bEnable = true;				// �X�V�E�`���
	m_col = true;					// �����蔻���
	m_bAtk = false;					// �U�����
	m_bGravity = true;				// �d��
	m_fSpeed = 0.0f;				// �ړ����x
	m_bLoop = false;				// �A�j���[�V�������[�v

	// ===���f���֘A������===
#ifdef _DEBUG
	m_ResourcePass = ("data/model/Character.fbx");
	m_Model = MODEL->Load("data/model/Character.fbx");
#else
	m_ResourcePass = ("data/model/Character.fbx");
	m_Model = MODEL->Load("data/model/Character.fbx");
#endif

	// ===�A�j���[�V�����ǉ�===
	m_Model->AddAnime("data/model/Idle.fbx");				// �ҋ@�A�j���[�V����
	m_Model->AddAnime("data/model/Run.fbx");				// ����A�j���[�V����
	m_Model->AddAnime("data/model/Roll.fbx");				// ����A�j���[�V����
	m_Model->AddAnime("data/model/Attack1.fbx");			// �U��1�A�j���[�V����
	m_Model->AddAnime("data/model/Attack2.fbx");			// �U��2�A�j���[�V����
	m_Model->AddAnime("data/model/guard.fbx");				// �e���A�j���[�V����

	// ---�A�j���[�V�����֘A�p�����[�^������---
	m_animNo = ANIM_IDLE;			// �A�j���[�V�����������
	m_animTime = 0;					// �A�j���[�V�����Đ�����
	m_blendNo = 0;					// �u�����h�Đ�����A�j���[�V����
	m_rate = 0.0f;					// �J�ڔ䗦

	// ---�X�e�[�^�X������
	m_nLife = 10;					// �̗�
	m_nStamina = 50.0f;				// �X�^�~�i
	AddCollisionOBB(XMFLOAT3(0, 1.0f, 0), XMFLOAT3(0.25f,1.0f,0.25f));		// �����蔻��ǉ�
	Init();							// �v���C���[��ԏ�����

	m_pPS = GetPS(PS_TOON);			// �s�N�Z���V�F�[�_�[�Z�b�g
	m_pVS = GetVS(VS_SKINMESH);		// ���_�V�F�[�_�\�Z�b�g
	m_pWeapon = new Weapon(this);	// ���퐶��

	// COLLISION->SwitchVisibleCollision(this);
}

// ===�v���C���[��ԏ�����===
void Player::Init()
{
	m_pStrategy[STAY_PLAYER] = new PlayerStay;			// �ҋ@��Ԑ���
	m_pStrategy[MOVE_PLAYER] = new PlayerMove;			// �ړ���Ԑ���
	m_pStrategy[ATTACK_PLAYER] = new PlayerAttack;		// �U���@��Ԑ���
	m_pStrategy[ROLL_PLAYER] = new PlayerRoll;			// �����Ԑ���
	m_pStrategy[ATTACK2_PLAYER] = new PlayerAttack2;	// �U���A��Ԑ���
	m_pStrategy[REPEL_PLAYER] = new PlayerRepel;		// �e����Ԑ���
}

// ===�f�X�g���N�^===
Player::~Player()
{
	// ---�I������---
	delete m_pWeapon;			// ����j��
	// ---��Ԕj��---
	for (int i = 0; i < MAX_PLAYER; i++)
	{
		delete m_pStrategy[i];	// ��Ԕj��
	}
}

// ===�X�V===
void Player::Update()
{
	if (!m_bEnable)				// �X�V��
		return;

	// ---�����␳---
	if (m_transform.Pos.y <= -3.0f)
	{
		m_transform.Pos.y = 3.0f;
		m_vVel.y = 0.0f;
	}

	// ---�̗�0�ŃQ�[���I�[�o�[---
	if (m_nLife <= 0)
	{
		SCENE->SetScene(SCENE_GAMEOVER);	// �V�[���ؑ�
		return;
	}

	// ---�Q�[���p�b�h�X�e�B�b�N�l�擾---
	XMFLOAT2 joyinput = XMFLOAT2(0.0f,0.0f);						// ������
	if (CInput::GetJoyCount() > 0)
	{
		joyinput.x = CInput::GetStick(CInput::GetJoyX(JOYSTICKID1));	// X������
		joyinput.y = CInput::GetStick(CInput::GetJoyY(JOYSTICKID1));	// Y������
	}

	// ---��ԍX�V---
	if (m_pStrategy[m_PlayerState]->IsUpdate(this) && SCENE->GetCurrentScene()->GetSceneName() != "TitleScene")
	{
		// ---�L�[���͎擾(����)
		if (CInput::GetKeyRelease(VK_S) || CInput::GetKeyRelease(VK_D) || CInput::GetKeyRelease(VK_A) || CInput::GetKeyRelease(VK_W))
			SetStrategy(STAY_PLAYER);		// ��Ԃ�ҋ@�ɃZ�b�g

		// ---�ړ�����
		if (m_PlayerState == STAY_PLAYER)
		{
			// ---�L�[����
			if (CInput::GetKeyPress(VK_A) || CInput::GetKeyPress(VK_W) || CInput::GetKeyPress(VK_S) || CInput::GetKeyPress(VK_D)) 
				SetStrategy(MOVE_PLAYER);	// ��Ԃ��ړ��ɃZ�b�g

			// ---�Q�[���p�b�h����
			if (fabs(joyinput.x) >= DEAD_STICK_X || fabs(joyinput.y) >= DEAD_STICK_Y)
				SetStrategy(MOVE_PLAYER);
		}

		// ---�ړ��ȊO�̃A�N�V����
		if (m_PlayerState == STAY_PLAYER || m_PlayerState == MOVE_PLAYER)
		{
			if (CInput::GetKeyTrigger(VK_SPACE) || CInput::GetJoyTrigger(JOYSTICKID1,VB_A))
				SetStrategy(ROLL_PLAYER);	// �����Ԃ��Z�b�g

			else if (CInput::GetMouseTrigger(MOUSEBUTTON_L) || CInput::GetJoyButton(JOYSTICKID1,VB_R))
				SetStrategy(ATTACK_PLAYER);	// �U����Ԃ��Z�b�g

			else if (CInput::GetMouseTrigger(MOUSEBUTTON_R) || CInput::GetJoyTrigger(JOYSTICKID1,VB_L))
				SetStrategy(REPEL_PLAYER);	// �e����Ԃ��Z�b�g
		}
	}

	// ---�X�V����---
	m_pStrategy[m_PlayerState]->Update(this);		// ��Ԃ��Ƃ̍X�V����
	Object3DBase::Update();							// ���W���X�V

	m_bGravity = true;								// �d�̓Z�b�g

	// ---�A�j���[�V�����̍X�V---
	if (m_rate > 1.0f)				// �J�ڔ䗦��1.0�𒴂��Ă���ꍇ�͑J�ڏI��
	{
		m_blendNo = -1;				// �u�����h�A�j���[�V�����Ȃ�
		m_rate = 1.0f;				// �䗦��1.0�ɔ[�߂�
	}
	else
		m_rate += 0.2f;				// �J�ڔ䗦�X�V

	if (m_animTime * m_Model->GetAnimTick(m_animNo) / 60.0f >= m_Model->GetAnimDuration(m_animNo))		// �A�j���[�V�����Đ����Ԃ��ő厞�Ԃ𒴂��Ă���ꍇ�̏���
	{
		if (m_bLoop && m_blendNo == -1)		// ���[�v�Đ��̏ꍇ�̓��Z�b�g
			m_animTime = 0.0f;				// �Đ����Ԃ�0�ɃZ�b�g
	}
	else
	{
		if (m_blendNo == -1)				// �A�j���[�V�����u�����h�Ȃ��̏ꍇ
			m_animTime += 1.0f;				// �Đ����Ԃ��X�V
	}

	if (m_Model)
	{
		// ---�p���X�V---
		if (m_blendNo != -1)				// �u�����h�L�̏ꍇ
			m_Model->PlayBlendAnime(m_animTime, &m_animNo, m_blendNo, m_rate);		// �u�����h�Đ�
		else
			m_Model->PlayAnime(m_animTime, &m_animNo);		// �ʏ�Đ�
		if (m_animNo == m_blendNo)
			m_blendNo = -1;							// �u�����h�A�j���[�V�����ƌ��ݍĐ����̃A�j���[�V��������v���Ă�����u�����h�Ȃ��ɃZ�b�g
	}
}

// ===�`�揈��===
void Player::Draw()
{
	// ---���f���`��---
	SetCullMode(CULLMODE_CCW);	// �w�ʃJ�����O
	ID3D11ShaderResourceView* pTex = TEX->Load("data/texture/ramp.png");	// �~���e�N�X�`���擾
	SetTexturePS(pTex, 1);		// �~���e�N�X�`���Z�b�g
	Object3DBase::Draw();		// �`��

	// ---�֊s���`��---
	SetCullMode(CULLMODE_CW);	// �O�ʃJ�����O
	m_pPS = GetPS(PS_OUTLINE);	// �֊s���`��V�F�[�_�[�ɐؑ�
	m_pVS = GetVS(VS_OUTLINE);	// �֊s���`��V�F�[�_�[�ɐؑ�
	Object3DBase::Draw();		// �`��

	// ---�`��㏈��---
	m_pPS = GetPS(PS_TOON);		// �s�N�Z���V�F�[�_�[�Z�b�g
	m_pVS = GetVS(VS_SKINMESH);	// ���_�V�F�[�_�\�Z�b�g
	SetCullMode(CULLMODE_CCW);	// �w�ʃJ�����O
}

// ===�X�V�L���̎擾===
bool Player::IsUpdate()
{
	return true;
}

// ===��Ԃ��Z�b�g===
void Player::SetStrategy(PLAYER_STATE player)
{
	m_PlayerState = player;
	m_pStrategy[m_PlayerState]->Init(this);
}

// ===�Փˏ���===
void Player::OnCollision(Object* obj)
{
	// ---�n�ʂƐڐG�����ꍇ�̏���
	if (obj->GetObjID() == OT_GROUND)
	{
		m_bGravity = false;		// �d�͂Ȃ�
		m_vAcc.y = 0.0f;		// y�������x���Z�b�g
		m_vVel.y = 0.0f;		// y�����x���Z�b�g
	}

	// ---�S�[���I�u�W�F�N�g�ƐڐG�����ꍇ�̏���
	if (obj->GetObjID() == OT_GOAL)
	{
		SCENE->SetScene(SCENE_LOAD);
	}

	// ---�h�A�ƐڐG�����ꍇ�̏���
	if (obj->GetObjID() == OT_DOOR)
	{
		// ---��O����---
		if (!MAP->GetClear())	// �N���A���Ă��Ȃ���Ԃ̏ꍇ�͉����N����Ȃ�
			return;

		// ---�X�e�[�W�̐ؑ�---
		int dir = ((Door*)obj)->GetDirection();			// ���������h�A�̕������擾
		MAP->ChangeRoom(dir);							// ���̕����̕����ɐؑ�
		dir = dir ^ 0x2;								// 2�r�b�g�ڂ𔽓](���̃h�A�ɐG�ꂽ�ꍇ�͎��̕����̎�O�̃h�A�Ɉړ����邽��)
		m_bEnable = true;								// �ēx�\��

		// ---�X�e�[�W�ؑ֌�Ƀv���C���[�̍��W���Z�b�g---
		std::vector<Object*> doors = SCENE->GetCurrentScene()->FindObjectListWithTag(OT_DOOR);		// �V�[���ɔz�u���ꂽ�h�A���擾
		auto it = doors.begin();						// �h�A�̐擪�C�e���[�^

		while (it != doors.end())		// �I�[�C�e���[�^�[�܂�
		{
			if (((Door*)*it)->GetDirection() == dir)	// �ڐG�����h�A�̕����ƈ�v����ꍇ�͍��W�����̃h�A�̖ڂ̑O�ɃZ�b�g
			{
				m_transform.Pos = ((Door*)*it)->GetEntrance();	// ���W���h�A�̑O�ɃZ�b�g
				break;
			}
			++it;		// �C�e���[�^�[�����炷
		}
	}
}

// ===�߂荞�ݗʂ�߂�===
void Player::ReflectMovement()
{
	XMFLOAT3 movement = GetMovement();		// �ړ��ʂ��擾

	m_transform.Pos.x += movement.x;		// x�����ړ�
	m_transform.Pos.y += movement.y;		// y�����ړ�
	m_transform.Pos.z += movement.z;		// z�����ړ�
}

// ===�A�j���[�V�����I����Ԏ擾===
bool Player::GetEndAnime()
{
	if (m_animTime * m_Model->GetAnimTick(m_animNo) / 60.0f >= m_Model->GetAnimDuration(m_animNo))		// �Đ����Ԃ��ő厞�Ԃ𒴂��Ă�����I��
		return true;
	return false;
}

// ===���C�t�X�V===
void Player::DelLife()
{
	if (m_PlayerState != ROLL_PLAYER)	// �_���[�W
	{
		m_nLife--;														// ���C�t����
		m_nInvincibleTime = 100;										// ���G���ԃZ�b�g
		TIME->SetStop(0.3f);											// �q�b�g�X�g�b�v
		CSound::Play(SE_DEFEND);										// �_���[�WSE�Đ�
		EFFECT->Play(EXPLOSION_EFFECT, GetHitPos());					// �G�t�F�N�g���Փ˂����ʒu�ɍĐ�
		((GameCamera*)CAMERA->GetCurrentCamera())->Shake(0.5f);			// �J�����U��
	}
	else								// ���
	{
		m_nInvincibleTime = 10;						// ���G���ԃZ�b�g
		TIME->SetSlow(0.3f);						// �X���[���o
		CSound::Play(SE_AVOID);						// ���SE�Đ�
		EFFECT->Play(AVOID_EFFECT, GetHitPos());	// �G�t�F�N�g���Փ˂����ʒu�ɍĐ�
	}
}

// ===�X�^�~�i�X�V===
void Player::DelStamina(const float& num)
{
	m_nStamina -= num;			// �X�^�~�i�X�V
	if (m_nStamina >= 50)		// �l��͈͓��ɔ[�߂�
		m_nStamina = 50;		// �ő�X�^�~�i�̒l�ɂ���
	if (m_nStamina <= 0)		// �l��͈͓��ɔ[�߂�
		m_nStamina = 0;			// 0�ȉ��̏ꍇ��0
}