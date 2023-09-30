#pragma once
// ===�G�x�[�X�N���X Enemy.h
// --Author kimura
#include "main.h"
#include "Animation2DBase.h"
#include "EnemyState.h"
#include "Collision3D.h"

// ===�\���̒�`===
enum ENEMY2D_STATE		// �G���
{
	MOVING_ENEMY = 0,	// �ړ�
	DAMAGE_ENEMY,		// �_���[�W

	MAX_ENEMY
};

class EnemyState;
// ---�N���X�錾
class Enemy : public Animation2DBase, Collision3D
{
public:
	Enemy();
	Enemy(XMFLOAT3);

	~Enemy();

	void Init();
	void Update();
	void Draw();

	bool IsUpdate();
	void SetState(ENEMY2D_STATE state);
	void OnCollision(Object*);
private:
	EnemyState* m_pState[MAX_ENEMY];		// �v���C���[
	ENEMY2D_STATE m_EnemyState;					// �v���C���[���
	XMFLOAT3 oldPos;

	int m_nLife;								// ���C�t
	bool m_bAtk;								// �U���t���O
};