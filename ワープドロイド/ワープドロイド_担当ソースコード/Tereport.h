/*
�쐬�� : KimuraYusaku

�X�V�� : Osaki Noriaki

�X�V����
		2021/12/11		�쐬
		2021/12/18		�����ύX�̂��ߖ߂�l�ύX
		2022/1/3		�N���X���A�摜�ݒ�
*/
#pragma once

#include "OriginalType.h"
#include <list>
#include "ObjectBase2D.h"
#include "EnemyManager.h"
#include "ArrowManager.h"

class Tereport : public ObjectBase2D
{
public:
	Tereport();
	~Tereport();

	void Update();

	static OFLOAT3 Do(OFLOAT3 *);/*, std::list<Enemy*>, OFLOAT2*/				// �e���|�[�g�֐�
	static bool GetTereport(void);											// �e���|�[�g�t���O�擾
	static void SetTereport(bool);											// �e���|�[�g�t���O�ݒ�
	static void SetTarget(void);

private:
	ObjectBase2D* m_pWave;
	void UpdateWave();



	Enemy* Serch(OFLOAT3, std::list<Enemy*>, OFLOAT2);
	static Enemy* m_pActive;
	OFLOAT2 m_input;
	EnemyManager *m_pEM;

	ArrowManager *m_pAM;
};