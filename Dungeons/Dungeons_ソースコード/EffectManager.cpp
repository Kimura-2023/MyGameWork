// ===�G�t�F�N�g�Ǘ��N���X����===
// �G�t�F�N�g�Ǘ��N���X������
// Author Kimura
// ==============================

// ===�C���N���[�h===
#include "EffectManager.h"
#include "CameraManager.h"

// ===�O���[�o���ϐ�===
EffectManager* EffectManager::m_pInstance = nullptr;

namespace EffectKind
{
	const char16_t* pass[EFFECT_MAX] = {
		u"data/effect/hit.efk",
		u"data/effect/move.efk",
		u"data/effect/sword.efk",
		u"data/effect/kick.efk",
		u"data/effect/explosion.efk",
		u"data/effect/enemyatk.efk",
		u"data/effect/dead.efk",
		u"data/effect/repel.efk",
		u"data/effect/avoid.efk",
	};
}

// ===�R���X�g���N�^===
EffectManager::EffectManager()
{
	Init();
}

// ===�f�X�g���N�^===
EffectManager::~EffectManager()
{
	EFFECT->Release();
}

// ===����===
void EffectManager::Create()
{
	if (!m_pInstance)
	{
		m_pInstance = new EffectManager;
	}
}

// ===�j��===
void EffectManager::Destroy()
{
	if (m_pInstance)
	{
		delete m_pInstance;
		m_pInstance = nullptr;
	}
}

// ===�擾===
EffectManager* EffectManager::Get()
{
	return m_pInstance;
}

void EffectManager::Init()
{
	// Effekseer������
	m_renderer = EffekseerRendererDX11::Renderer::Create(GetDevice(), GetDeviceContext(), MAX_PARTICLE);
	m_manager = Effekseer::Manager::Create(MAX_PARTICLE);

	// �`�惂�W���[���̐ݒ�
	m_manager->SetSpriteRenderer(m_renderer->CreateSpriteRenderer());
	m_manager->SetRibbonRenderer(m_renderer->CreateRibbonRenderer());
	m_manager->SetRingRenderer(m_renderer->CreateRingRenderer());
	m_manager->SetTrackRenderer(m_renderer->CreateTrackRenderer());
	m_manager->SetModelRenderer(m_renderer->CreateModelRenderer());

	// �e�N�X�`���A���f���A�J�[�u�A�}�e���A�����[�_�[�̐ݒ肷��B
	m_manager->SetTextureLoader(m_renderer->CreateTextureLoader());
	m_manager->SetModelLoader(m_renderer->CreateModelLoader());
	m_manager->SetMaterialLoader(m_renderer->CreateMaterialLoader());
	m_manager->SetCurveLoader(Effekseer::MakeRefPtr<Effekseer::CurveLoader>());

	LoadAll();
}

void EffectManager::Release()
{
	// �S�e�N�X�`�����
	auto it = m_EffectResource.begin();
	while (it != m_EffectResource.end())
	{
		it->Reset();
		++it;
	}

	m_manager.Reset();
	m_renderer.Reset();
}

void EffectManager::Update()
{
	CameraBase* pCamera = CAMERA->GetCurrentCamera();
	if (!pCamera)
		return;

	// �s��
	XMFLOAT3 cameraPos = pCamera->GetPos();
	XMFLOAT3 cameraLook = pCamera->GetLook();
	XMFLOAT3 cameraUp = pCamera->GetUp();

	Effekseer::Vector3D eye  = Effekseer::Vector3D(cameraPos.x, cameraPos.y, cameraPos.z);
	Effekseer::Vector3D look = Effekseer::Vector3D(cameraLook.x, cameraLook.y, cameraLook.z);
	Effekseer::Vector3D up   = Effekseer::Vector3D(cameraUp.x, cameraUp.y, cameraUp.z);

	// ���e�s���ݒ�
	m_renderer->SetProjectionMatrix(Effekseer::Matrix44().PerspectiveFovLH(
		XMConvertToRadians(pCamera->GetFov()), pCamera->GetAspect(), pCamera->GetNearClip(), pCamera->GetFarClip()));
	// �J�����s���ݒ�
	m_renderer->SetCameraMatrix(Effekseer::Matrix44().LookAtLH(eye, look, up));

	m_manager->Update();
}

void EffectManager::Draw()
{
	SetBlendState(BS_ALPHABLEND);

	static int time = 0;
	m_renderer->SetTime(time++ / 60.0f);
	m_renderer->BeginRendering();
	m_manager->Draw();
	m_renderer->EndRendering();
}

void EffectManager::LoadAll()
{
	for (int i = 0; i < EFFECT_MAX; ++i)
	{
		m_EffectResource.push_back(Effekseer::Effect::Create(m_manager, EffectKind::pass[i]));
	}
}

void EffectManager::Play(EFFECT_TYPE eff,const XMFLOAT3& pos)
{
	m_handle = m_manager->Play(m_EffectResource[eff], pos.x, pos.y, pos.z);
}