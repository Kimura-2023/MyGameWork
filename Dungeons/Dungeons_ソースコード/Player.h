#pragma once
// ===�v���C���[�N���X��`===
// �v���C���[�N���X���`
// Author:	Kimura
// ==============================

// ===�C���N���[�h===
#include "main.h"
#include "PlayerStrategy.h"
#include "Collision3D.h"
#include "Weapon.h"
#include "Bullet.h"

// ===�񋓒�`===
enum PLAYER_STATE		// ---�v���C���[���
{
	STAY_PLAYER = 0,	// �ҋ@
	MOVE_PLAYER,		// �ړ�
	ATTACK_PLAYER,		// �U��
	ROLL_PLAYER,		// ���
	ATTACK2_PLAYER,		// �U��(��i��)
	REPEL_PLAYER,		// �͂���

	MAX_PLAYER
};
namespace {				// ---�A�j���[�V����
	enum EAnimNo {
		ANIM_IDLE = 0,	// �ҋ@�A�j���[�V����
		ANIM_RUN,		// �ړ��A�j���[�V����
		ANIM_ROLL,		// ����A�j���[�V����
		ANIM_ATTACK,	// �U���A�j���[�V����
		ANIM_ATTACK2,	// 2�i�ڍU���A�j���[�V����
		ANIM_REPEL,		// �e���A�j���[�V����
		MAX_ANIMNO		// �ő�
	};
}

// ===�N���X��`===
class PlayerStrategy;
class Player : public Object3DBase, Collision3D
{
	// ---�����o�֐�---
public:
	Player();			// �R���X�g���N�^
	~Player();			// �f�X�g���N�^

	void Init();		// ����������
	void Update();		// �X�V����
	void Draw();		// �`�揈��

	bool IsUpdate();	// �X�V�L��
	void SetStrategy(PLAYER_STATE player);		// ��Ԑݒ�
	void OnCollision(Object*);					// �Փˏ���
	void ReflectMovement();						// �߂荞�ݗʂ�߂�

	bool GetAtk() { return m_bAtk; }			// �U����Ԏ擾
	void SetAtk(bool flg) { m_bAtk = flg; }		// �U����Ԑݒ�

	float& GetSpeed() { return m_fSpeed; }		// ���x�擾	
	void SetSpeed(const float& speed) { m_fSpeed = speed; }		// ���x�ݒ�

	void PlayerSetAnime(int num) { m_animNo = num; }			// �A�j���[�V�����ԍ��ݒ�
	void PlayerSetBlendAnime(int num) { m_blendNo = num; }		// �A�j���[�V�����u�����h�ݒ�

	int& GetLife() { return m_nLife; }			// �̗͎擾
	float& GetStamina() { return m_nStamina; }	// �X�^�~�i�擾

	int& GetAnimeNo() { return m_animNo; }		// �A�j���[�V�����ԍ��擾
	float& GetAnimTime() { return m_animTime; }	// �A�j���[�V�����Đ����Ԏ擾
	float& GetRate() { return m_rate; }			// �A�j���[�V�����J�ڗ��擾
	bool GetEndAnime();							// �A�j���[�V�����I���擾
	void SetAnimeRate(float rate) { m_rate = rate; }		// �A�j���[�V�����J�ڗ��ݒ�
	void SetAnimeTime(float time) { m_animTime = time; }	// �A�j���[�V�����Đ����Ԑݒ�
	void SetAnimeLoop(bool flg) { m_bLoop = flg; }			// �A�j���[�V�������[�v�ݒ�

	Weapon* GetWeapon() { return m_pWeapon; }	// ����擾

	void DelLife();								// �̗͌���
	void DelStamina(const float& num = 1);		// �X�^�~�i����

	// ---�����o�ϐ�---
private:
	PlayerStrategy* m_pStrategy[MAX_PLAYER];	// �v���C���[
	PLAYER_STATE m_PlayerState;					// �v���C���[���

	float m_fSpeed;								// �ړ����x

	int m_nLife;								// ���C�t
	float m_nStamina;							// �X�^�~�i
	bool m_bAtk;								// �U���t���O
	bool m_bLoop;								// �A�j���[�V�������[�v�t���O

	int m_animNo;								// �A�j���[�V����No.
	int m_blendNo;								// �u�����h�A�j���[�V����
	float m_animTime;							// �Đ�����
	float m_rate;								// �u�����h���邽�߂̃��[�g

	Weapon* m_pWeapon;							// ����
};