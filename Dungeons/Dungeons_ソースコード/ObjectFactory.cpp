// ===�I�u�W�F�N�g�����N���X����====
// �I�u�W�F�N�g�������s���N���X������
// Author Kimura
// ==============================

// ===�C���N���[�h===
#include "ObjectFactory.h"
#include "Title.h"
#include "Player.h"
#include "Ground.h"
#include "Wall.h"
#include "ModelObject.h"
#include "EnterButton.h"
#include "Pillar.h"
#include "Fence.h"
#include "WallPillar.h"
#include "Door.h"
#include "EnemyNormal.h"
#include "EnemyBullet.h"
#include "StartButton.h"
#include "TitleButton.h"
#include "FinButton.h"
#include "PlayerLife.h"
#include "PlayerStamina.h"
#include "GameOver.h"
#include "GameClear.h"
#include "Goal.h"
#include "Score.h"
#include "ScoreEffect.h"

// ===�I�u�W�F�N�g����===
Object* ObjectFactrory::CreateObject(ObjType objtype)
{
	switch (objtype)		// �^�ɂ���Đؑ�
	{
	case OT_PLAYER:				// �v���C���[
		return new Player;
		break;
	case OT_GROUND:				// �n��
		return new Ground;
		break;
	case OT_WALL:				// ��
		return new Wall;
		break;
	case OT_PILLAR:				// ��
		return new Pillar;
		break;
	case BG_3DOBJ:				// �w�i�I�u�W�F�N�g
		return new ModelObject;
		break;
	case BG_TITLE:				// �^�C�g�����S
		return new Title;
		break;
	case BG_ENTER:				// Enter�{�^��
		return new EnterButton;
		break;
	case OT_ENEMY:				// �ʏ�G�l�~�[
		return new EnemyNormal;
		break;
	case OT_FENCE:				// ��
		return new Fence;
		break;
	case OT_WALLPILLAR:			// �ǒ�
		return new WallPillar;
		break;
	case OT_DOOR:				// ��
		return new Door;
		break;
	case OT_ENEMY_BULLET:		// �������G�l�~�[
		return new EnemyBullet;
		break;

	case UI_START_BUTTON:		// �X�^�[�g�{�^��
		return new StartButton;
		break;
	case UI_TITLE_BUTTON:		// �^�C�g���{�^��
		return new TitleButton;
		break;
	case UI_FIN_BUTTON:			// �I���{�^��
		return new FinButton;
		break;
	case UI_LIFE:				// �̗�UI
		return new PlayerLife;
		break;
	case UI_STAMINA:			// �X�^�~�iUI
		return new PlayerStamina;
		break;
	case BG_GAMEOVER:			// �Q�[���I�[�o�[�w�i
		return new GameOver;
		break;

	case BG_GAMECLEAR:			// �Q�[���N���A�w�i
		return new GameClear;
		break;

	case OT_GOAL:
		return new Goal;		// �S�[���I�u�W�F�N�g
		break;

	case UI_SCORE:
		return new Score;		// �X�R�AUI

	case BILL_SCORE:
		return new ScoreEffect;	// �X�R�A�G�t�F�N�g
	}

	return nullptr;
}