// ===�f�o�b�O�p�V�[���N���X����===
// �f�o�b�O�p�V�[���N���X������
// ---Author kimura
// =================================

// ===�C���N���[�h===
#include "DebugScene.h"
#include "SceneManager.h"
#include "DebugProc.h"
#include "Input.h"
#include "GameManager.h"
#include "GameCamera.h"
#include "TimeController.h"
#include "Calc.h"
#include "MapManager.h"
#include "CameraManager.h"
#include "ImGuiTest.h"
#include "ImGuiSceneEditer.h"
#include "CollisionManager.h"
#include "Fade.h"
#include "Sound.h"

// ===�\���̒�`===
struct ViewSetting					// ---�J�����ݒ�
{
	DirectX::XMFLOAT3 camPos;		// �J�������W
	DirectX::XMFLOAT3 camLook;		// �J���������_
	DirectX::XMFLOAT3 camUp;		// �J�����̃A�b�v�x�N�g��
	char SceneName[50];				// �V�[����
};

// ===�R���X�g���N�^===
DebugScene::DebugScene()
{
	m_Name = "DebugScene";
	m_HitZ = 0.0f;

	MapManager::Create();
	CameraManager::Get()->RegisterCamera("DebugCamera", DEBUG_CAMERA);
	CameraBase* pCamera = CameraManager::Get()->FindCamera("DebugCamera");

	ViewSetting setting =
	{
		pCamera->GetPos(),
		pCamera->GetLook(),
		pCamera->GetUp()
	};

	strcpy(setting.SceneName, m_Name.c_str());

	CameraManager::Get()->SetCamera(CameraManager::Get()->FindCamera("DebugCamera"));

	std::vector<std::vector<Room*>> pRooms = MAP->GetRooms();

	new ImGuiSceneEditer;
	LoadSceneData();

	CSound::SetVolume(BGM_GAME, 0.05f);
	CSound::SetVolume(SE_SWORD, 0.05f);
	CSound::SetVolume(SE_EXPLOSION, 0.05f);
	CSound::SetVolume(SE_DEFEND, 0.05f);
	CSound::SetVolume(SE_GAMEOVER, 0.05f);
	CSound::SetVolume(SE_SHOT, 0.05f);
	CSound::SetVolume(SE_REPEL, 0.05f);
	CSound::SetVolume(SE_AVOID, 0.05f);
}

// ===�f�X�g���N�^===
DebugScene::~DebugScene()
{
	MapManager::Destroy();		// �}�b�v�j��
	SaveSceneData();			// �V�[���f�[�^��ۑ�
}

// ===�X�V����===
void DebugScene::Update()
{
	// ---�}�b�v�N���A---
	if (CInput::GetKeyTrigger(VK_RETURN)) MAP->SetClear(true);

	// ---�J�������W�f�[�^�ۑ�---
	if (CInput::GetKeyPress(VK_CONTROL))
	{
		if(CInput::GetKeyTrigger(VK_S))
		{
			CameraBase* pCamera = CameraManager::Get()->GetCurrentCamera();		// �J�����擾

			// ---�J�������擾---
			ViewSetting setting =
			{
				pCamera->GetPos(),		// ���W
				pCamera->GetLook(),		// �����_
				pCamera->GetUp()		// ����x�N�g��
			};

			strcpy(setting.SceneName, m_Name.c_str());			// �V�[�������R�s�[
			FILE* fp;
			fopen_s(&fp, "data/camera/setting.dat", "wb");		// �t�@�C���I�[�v��
			if (fp)
			{
				fwrite(&setting, sizeof(ViewSetting), 1, fp);	// �t�@�C����������
				fclose(fp);										// �t�@�C���N���[�Y
			}
		}
	}

	CheckSelectObject();		// �I�u�W�F�N�g�I���X�V
}

// ===�`�揈��===
void DebugScene::Draw()
{
	SceneBase::Draw();		// �ʏ�`��
}

// ===�I���I�u�W�F�N�g����===
void DebugScene::CheckSelectObject()
{
	// ---�ϐ��錾---
	XMFLOAT3	PQ, P, V;
	XMVECTOR	vPQ, vV;
	float		Zval, PQv, d, PQd, Vd;

	// ---���W�擾---
	POINT mouse = GAME->GetMousePoint();					// �J�[�\���ʒu(�X�N���[����)�擾
	CameraBase* pCamera = CAMERA->GetCurrentCamera();		// ���݂̃J�������擾

	XMFLOAT3& E = pCamera->GetPos();						// �J�����̈ʒu���擾
	Object* pObj = ImGuiSceneEditer::m_pSelectObj;			// �I�𒆂̃I�u�W�F�N�g���擾
	Object* pNear = nullptr;								// �}�E�X�J�[�\���ŐG��Ă���I�u�W�F�N�g(��ɔ��f���邽�ߏ�����)

	if (pCamera)
	{
		// �}�E�X�J�[�\���̈ʒu�����[���h���W��(�x�N�g���Z�o)
		XMStoreFloat3(&P, XMVector3Unproject(XMVectorSet((float)mouse.x, (float)mouse.y, 0.0f, 1.0f),
			GetViewPort().TopLeftX, GetViewPort().TopLeftY, GetViewPort().Width, GetViewPort().Height, GetViewPort().MinDepth, GetViewPort().MaxDepth,
			XMLoadFloat4x4(&pCamera->GetProjMatrix()), XMLoadFloat4x4(&pCamera->GetViewMatrix()),
			XMMatrixIdentity()));
	}

	// ---�I�u�W�F�N�g�擾---
	auto objmap = GetAllObjects();				// �V�[����̃I�u�W�F�N�g�����ׂĎ擾
	auto it = objmap->begin();					// �C�e���[�^�[������

	// ---����---
	while (it != objmap->end())
	{
		// ---���O����---
		if (!it->second->GetVisible())			// ����Ԃ��ǂ���
		{
			++it;
			continue;
		}

		// ---�J�������_���猩���I�u�W�F�N�g�Ƃ̋����Z�o---
		XMFLOAT3 Q = it->second->GetTransform().Pos;		// �I�u�W�F�N�g�̈ʒu���擾
		XMStoreFloat(&Zval, XMVector3Length(XMVectorSet(Q.x - E.x, Q.y - E.y, Q.z - E.z, 1.0f)));		// �������O�哃

		// ---���ɐݒ肳��Ă���ڕW����낾�����ꍇ---
		if (pNear != nullptr && Zval >= m_HitZ) {
			++it;											// �I��ΏۂɂȂ�Ȃ�
			continue;
		}

		// ---�}�E�X�ʒu�ւ̃x�N�g���Z�o---
		V.x = P.x - E.x;
		V.y = P.y - E.y;
		V.z = P.z - E.z;

		// ---�_���璼���܂ł̋������Z�o---
		PQ.x = Q.x - P.x;
		PQ.y = Q.y - P.y;
		PQ.z = Q.z - P.z;

		vPQ = XMLoadFloat3(&PQ);
		vV = XMLoadFloat3(&V);
		XMStoreFloat(&PQv, XMVector3Dot(vPQ, vV));
		XMStoreFloat(&PQd, XMVector3Dot(vPQ, vPQ));
		XMStoreFloat(&Vd, XMVector3Dot(vV, vV));
		d = sqrtf(PQd - PQv * PQv / Vd);

		// ---�I�u�W�F�N�g���a��菬����(�J�[�\�����I�u�W�F�N�g�ɍ����Ă���)---
		if (d < it->second->GetRad()) {
			m_HitZ = Zval;				// ���_����̋������X�V
			pNear = it->second;			// �߂��̃I�u�W�F�N�g���X�V


			// ---����---
			if (CInput::GetMouseTrigger(MOUSEBUTTON_R))		// �E�N���b�N��
			{
				// ---�I�����Ă���I�u�W�F�N�g�����邩���f---
				if (pObj)									// �I�𒆂̃I�u�W�F�N�g������ꍇ
				{
					pObj->SetSelect(false);
					// �I���I�u�W�F�N�g�X�V
					pObj = it->second;
					pObj->SetSelect(true);
					ImGuiSceneEditer::m_pSelectObj = pObj;
				}
				else										// �Ȃ��ꍇ
				{
					pObj = it->second;						// �I���I�u�W�F�N�g�X�V
					pObj->SetSelect(true);
					ImGuiSceneEditer::m_pSelectObj = pObj;	// �G�f�B�^�ɂ��X�V�𔽉f
				}
			}

			if (CInput::GetKeyPress(VK_SHIFT) || CInput::GetKeyPress(VK_CONTROL))
			{
				if (CInput::GetMouseTrigger(MOUSEBUTTON_L))		// ���N���b�N��
				{
					// ---�I�����Ă���I�u�W�F�N�g�����邩���f---
					if (pObj)									// �I�𒆂̃I�u�W�F�N�g������ꍇ
					{
						pObj->SetSelect(false);
						// �I���I�u�W�F�N�g�X�V
						pObj = it->second;
						pObj->SetSelect(true);
						ImGuiSceneEditer::m_pSelectObj = pObj;
					}
					else										// �Ȃ��ꍇ
					{
						pObj = it->second;						// �I���I�u�W�F�N�g�X�V
						pObj->SetSelect(true);
						ImGuiSceneEditer::m_pSelectObj = pObj;	// �G�f�B�^�ɂ��X�V�𔽉f
					}
				}
			}
		}
		++it;
	}

	// ---�}�E�X�J�[�\�����ǂ̃I�u�W�F�N�g�ɂ��ڒn���Ă��Ȃ��ꍇ---
	if (pNear)
		return;

	// ---����---
	if (CInput::GetMouseTrigger(MOUSEBUTTON_R))				// �E�N���b�N��
	{
		if (pObj)											// �Z���N�g��Ԃ̃I�u�W�F�N�g�������
		{
			pObj->SetSelect(false);							// �I������
			// �I���I�u�W�F�N�g�X�V
			ImGuiSceneEditer::m_pSelectObj = nullptr;		// �G�f�B�^��ł��I�����͂���
		}
	}
	if (CInput::GetKeyPress(VK_SHIFT) || CInput::GetKeyPress(VK_CONTROL))		// ���N���b�N�̓E�B���h�E��G��Ƃ��Əd�����邱�Ƃ�����̂ŃL�[���͂��͂���
	{
		if (CInput::GetMouseTrigger(MOUSEBUTTON_L))			// ���N���b�N��
		{
			if (pObj)										// �Z���N�g��Ԃ̃I�u�W�F�N�g�������
			{
				pObj->SetSelect(false);						// �I��������
				// �I���I�u�W�F�N�g�X�V
				ImGuiSceneEditer::m_pSelectObj = nullptr;	// �G�f�B�^��őI�����͂���
			}
		}
	}
}