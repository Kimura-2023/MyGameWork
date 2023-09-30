#pragma once
// ===�G�x�[�X�N���X Enemy.h
// --Author kimura
#include "main.h"
#include "Object3DBase.h"
#include "EnemyStrategy.h"
#include <vector>

// ===�\���̒�`===
enum ENEMY_STATE		// �G���
{
	STAY_ENEMY = 0,
	MOVE_ENEMY,
	ATTACK_ENEMY,
	MAX_ENEMY
};

class EnemyStrategy;
// ---�N���X�錾
class EnemyBase : public Object3DBase
{
	// ---�����o�֐�---
public:
	EnemyBase();				// �R���X�g���N�^
	~EnemyBase();				// �f�X�g���N�^

	virtual void Init() = 0;	// ����������
	virtual void Update();		// �X�V����
	virtual void Draw();		// �`�揈��

	void SetNear(bool flg) { bNearFlg = flg; }				// �ߋ�����Ԃ��Z�b�g
	void SetInterval(const float& time) { if (m_AttackInterval <= 0.0f) m_AttackInterval = time * 60.0f; }			// �U���Ԋu���Z�b�g

	bool IsUpdate();						// �X�V�ێ擾
	void SetStrategy(ENEMY_STATE enemy);	// ��ԑJ��
	void DelLife(const int& num = 1) { m_nLife -= num; }		// ���C�t����
	float& GetView() { return m_view; }							// �G���F�����擾
	float& GetInterval() { return m_AttackInterval; }			// �U���Ԋu���Ԏ擾

	// ---�����o�ϐ�---
protected:
	EnemyStrategy* m_pStrategy[MAX_ENEMY];		// �G
	ENEMY_STATE m_State;						// �G���
	int m_nLife;								// �G���C�t
	float m_view;								// ���E�T�C�Y(���E�����a)
	bool bNearFlg;								// �ߋ����t���O
	float m_AttackInterval;						// �U���^�C�}�[
};
