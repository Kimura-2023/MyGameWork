// ===�V�[���x�[�X�N���X����===
// �V�[���x�[�X�N���X������
// Author:	Kimura
// ==========================

// ===�C���N���[�h===
#include "SceneBase.h"
#include <string>
#include "ObjectFactory.h"
#include "CameraManager.h"
#include "GameCamera.h"
#include "ImGuiController.h"
#include "TextureFactory.h"
#include "RenderTarget.h"
#include "ShaderList.h"
#include "Door.h"
#include "MapManager.h"

// ===�萔��`===
#define MAX_SCENE_OBJECT (1000)				// �V�[���ő�I�u�W�F�N�g��

// ===�\���̒�`===
struct SceneObjData
{
	ObjType id;					// �I�u�W�F�N�g�̌^
	Transform transform;		// ���W��
	char name[50];				// �ۑ��������O
	char pass[100];				// �f�ނ̃p�X
	bool enable;				// �X�V�A�`��̗L��
	bool sta;					// �ÓI�ȃI�u�W�F�N�g���ǂ���(�`��̂�)
};
#pragma pack()

// ===�R���X�g���N�^===
SceneBase::SceneBase() : m_Name("Scene"),m_pCBCamera(nullptr),m_pCBColor(nullptr),m_pCBlight(nullptr),m_LightDir(0.3f,-1.0f,0.3f,0.0f),Color(0.0f,0.0f,0.0f,1.0f)
{
	// ---�J�����p�萔�o�b�t�@�쐬---
	m_pCBCamera = new ConstantBuffer;
	m_pCBCamera->Create(sizeof(XMFLOAT4));

	// ---�F�p�萔�o�b�t�@�쐬---
	m_pCBColor = new ConstantBuffer;
	m_pCBColor->Create(sizeof(XMFLOAT4));

	// ---���C�g�p�萔�o�b�t�@�쐬---
	m_pCBlight = new ConstantBuffer;
	m_pCBlight->Create(sizeof(XMFLOAT4));

	// ---�e�p�萔�o�b�t�@�쐬---
	m_pCBSun = new ConstantBuffer;
	m_pCBSun->Create(sizeof(XMFLOAT4X4) * 2);

	// ---���[���h�ϊ��s��p�o�b�t�@�쐬---
	m_pWVP = new ConstantBuffer;
	m_pWVP->Create(sizeof(XMFLOAT4X4) * 3);

	m_pCBColor->Write(&Color);			// �֊s���F���Z�b�g

	// ---�V���h�E�}�b�v�����p---
	m_pCanvas = TextureFactory::CreateRenderTarget(DXGI_FORMAT_R32_FLOAT,1280.0f, 720.0f);								// �e�N�X�`������
	m_pDepth = reinterpret_cast<DepthStencil*>(TextureFactory::CreateDepthStencil(SCREEN_WIDTH, SCREEN_HEIGHT, true));	// �f�v�X�X�e���V������

	// ---�v���C���[���s�`��p---
	m_pCanvasObject = TextureFactory::CreateRenderTarget(DXGI_FORMAT_R32_FLOAT, 1280.0f, 720.0f);
}

// ===�f�X�g���N�^===
SceneBase::~SceneBase()
{
	// ---�I�u�W�F�N�g���---
	ReleaseAllGameObject();
	ImGuiController::ReleaseAll();

	// ---�e�N�X�`�����---
	delete m_pCanvasObject;
	delete m_pDepth;
	delete m_pCanvas;

	// ---�萔�o�b�t�@���---
	delete m_pWVP;
	delete m_pCBSun;
	delete m_pCBlight;
	delete m_pCBColor;
	delete m_pCBCamera;
}


// ===�I�u�W�F�N�g�T��===
Object* SceneBase::FindGameObject(std::string name)
{
	std::map<std::string, Object*>::iterator it;		// ���[�N�p
	it = m_pObjectList.find(name);		// �I�u�W�F�N�g���X�g(�A�z�z��)���疼�O���L�[�ɒT��
	if (it != m_pObjectList.end())	// ---���������ꍇ
	{
		return it->second;			// ���̃I�u�W�F�N�g��Ԃ�
	}
	else							// ---������Ȃ������ꍇ
		return nullptr;				// nullptr��Ԃ�
}

// ===�I�u�W�F�N�g�T��(�^�O����)===
Object* SceneBase::FindObjectWithTag(ObjType id)
{
	auto it = m_pObjectList.begin();		// �I�u�W�F�N�g�̐擪�C�e���[�^�擾
	while (it != m_pObjectList.end())	// �I�[�܂ŌJ��Ԃ�
	{
		if (it->second->GetObjID() == id)	// �����̃^�O�ƈ�v�����ꍇ
			return it->second;			// �q�b�g�����I�u�W�F�N�g��Ԃ�

		++it;		// �C�e���[�^�����炷
	}

	return nullptr;		// �I�[�܂ł��ǂ蒅���Ă���̂Ń^�O����v����I�u�W�F�N�g�Ȃ�
}

// ===�I�u�W�F�N�g���X�g�T��(�^�O����)===
std::vector<Object*> SceneBase::FindObjectListWithTag(ObjType id)
{
	std::vector<Object*> Objects;			// �I�u�W�F�N�g�i�[�p���I�z��
	auto it = m_pObjectList.begin();		// �I�u�W�F�N�g���X�g�̐擪�C�e���[�^�擾
	while (it != m_pObjectList.end())	// �I�[�܂ŌJ��Ԃ�
	{
		if (it->second->GetObjID() == id)	// �����̃^�O�ƈ�v����ꍇ
			Objects.push_back(it->second);	// �Ή��I�u�W�F�N�g���X�g�ɒǉ�
		++it;			// �C�e���[�^�����炷
	}
	return Objects;			// �i�[�����I�u�W�F�N�g��Ԃ�
}

// ===�I�u�W�F�N�g����===
Object* SceneBase::CreateGameObject(std::string name, ObjType type,int num)
{
	std::string ReName = name;		// ������
	// ---�I�u�W�F�N�g����������̃I�u�W�F�N�g���ǂ������f����---
	int index = num;		// ��
	if (index != 0)
	{
		ReName = name + std::to_string(num);	// ���O�ɔԍ���ǉ�
		++index;								// �ԍ���1���₷(��)Object0�Ƃ������O�̃I�u�W�F�N�g�������Ԃ�Object0�Ƃ������O�Ő��������Object1�Ƃ������O�Ƀ��l�[��������
	}

	Object* pObj = nullptr;
	std::map<std::string, Object*>::iterator it = m_pObjectList.begin();

	// ---�����ς݂Ŏg�p���Ă��Ȃ��I�u�W�F�N�g���ė��p����---
	while (it != m_pObjectList.end())
	{
		// ---��������I�u�W�F�N�g�̃^�O����v����I�u�W�F�N�g��T��---
		if(type != it->second->GetObjID())	// ��v���Ȃ��ꍇ�͎��̃I�u�W�F�N�g��T��
		{
			++it;		// �C�e���[�^�����炷
			continue;	// ���[�v
		}

		// ---�^�O����v����I�u�W�F�N�g�𔭌�������A�X�V�E�`������Ă��Ȃ��I�u�W�F�N�g���ǂ����𔻒肷��---
		if (!it->second->GetEnable())		// ---�X�V�E�`������Ă��Ȃ��I�u�W�F�N�g�̏ꍇ
		{
			pObj = it->second;				// �����������ɂ��̃I�u�W�F�N�g���ė��p����
			break;		// �ė��p����I�u�W�F�N�g�����܂����̂Ń��[�v���甲����
		}
		++it;			// �C�e���[�^�����炷
	}

	// ---�I�u�W�F�N�g�𐶐�---
	if (it == m_pObjectList.end())						// �ė��p����I�u�W�F�N�g��������Ȃ������ꍇ�V�K�ŃI�u�W�F�N�g�𐶐�����
	{
		while (FindGameObject(ReName))					// �����ς݂̖��O���ǂ�����T��
		{
			ReName = name + std::to_string(index);		// ���l�[�����邽�߂ɃI�u�W�F�N�g���ɔԍ���t������
			++index;									// �ԍ������Z
		}

		pObj = ObjectFactrory::CreateObject(type);								// �I�u�W�F�N�g�𐶐�
		pObj->SetName(ReName);													// ���l�[��
		m_pObjectList.insert(std::pair<std::string, Object*>(ReName, pObj));	// �I�u�W�F�N�g���X�g(�A�z�z��)�ɓo�^����
	}

	// ---��O����---
	if (!pObj)
		return nullptr;			// �I�u�W�F�N�g�������ł��Ȃ������̂�nullptr��Ԃ�

	pObj->SetEnable(true);		// �X�V�E�`����J�n������
	return pObj;				// ���������I�u�W�F�N�g��Ԃ�
}

// ===�I�u�W�F�N�g�j��===
bool SceneBase::DeleteGameObject(std::string name)
{
	// --- �I�u�W�F�N�g���X�g����T��---
	auto it = m_pObjectList.find(name);

	// ---�I�u�W�F�N�g���V�[���ɑ��݂��Ă��Ȃ��ꍇ---
	if (it == m_pObjectList.end())
		return false;		// �j�����s

	// ---�I�u�W�F�N�g���J�����̒Ǐ]�I�u�W�F�N�g�Ώۂ������ꍇ---
	if (it->second == ((GameCamera*)CAMERA->GetCurrentCamera())->GetTargetObj())
	{
		((GameCamera*)CAMERA->GetCurrentCamera())->SetTargetObj(nullptr);			// ���̃J�����̒Ǐ]�I�u�W�F�N�g��nullptr���Z�b�g����
	}

	// ---�I�u�W�F�N�g�폜---
	delete it->second;				// ���������
	it->second = nullptr;
	m_pObjectList.erase(name);		// ���X�g���珜�O

	return true;			// �j������
}

// ===�S�I�u�W�F�N�g���===
void SceneBase::ReleaseAllGameObject()
{
	// ---�S�I�u�W�F�N�g�j��---
	std::map<std::string, Object*>::iterator it = m_pObjectList.begin();
	while (it != m_pObjectList.end())
	{
		// ---�I�u�W�F�N�g�����������ۂɃJ�����̒Ǐ]�I�u�W�F�N�g�Ώۂ������ꍇ---
		if (it->second == ((GameCamera*)CAMERA->GetCurrentCamera())->GetTargetObj())
		{
			((GameCamera*)CAMERA->GetCurrentCamera())->SetTargetObj(nullptr);			// ���̃J�����̒Ǐ]�I�u�W�F�N�g��nullptr���Z�b�g����
		}

		delete it->second;				// �I�u�W�F�N�g���
		it = m_pObjectList.erase(it);	// �C�e���[�^�����炷
		continue;
	}
}

// ===�͈͓��̃I�u�W�F�N�g�̐��𐔂���===
int& SceneBase::RangeObjectCount(XMFLOAT3& min, XMFLOAT3& max)
{
	int cnt = 0;							// �J�E���g��
	auto it = m_pObjectList.begin();		// �I�u�W�F�N�g���X�g�擪�C�e���[�^�擾
	while (it != m_pObjectList.end())	// �I�[�܂�
	{
		// ---����---
		if (min.x < it->second->GetTransform().Pos.x && it->second->GetTransform().Pos.x <= max.x)
		{
			if (min.y < it->second->GetTransform().Pos.y && it->second->GetTransform().Pos.y <= max.y)
			{
				if (min.z < it->second->GetTransform().Pos.z && it->second->GetTransform().Pos.y <= max.z)
				{
					cnt++;		// �J�E���g����
				}
			}
		}

		++it;	// �C�e���[�^�����炷
	}
	return cnt;		// �J�E���g����Ԃ�
}

// ===�V�[���f�[�^�ۑ�===
void SceneBase::SaveSceneData(std::string filename)
{
	// ---�I�u�W�F�N�g�f�[�^�擾---
	std::vector<SceneObjData> Objects;			// �I�u�W�F�N�g�ۑ��p�z��
	SceneObjData work;							// �ϊ��p
	auto it = m_pObjectList.begin();			// �I�u�W�F�N�g���X�g�̐擪�C�e���[�^�擾
	while (it != m_pObjectList.end())		// �I�[�܂ŌJ��Ԃ�
	{
		work.id = it->second->GetObjID();								// �^�O���擾
		work.transform = it->second->GetTransform();					// ���W�E��]�E�g�k���擾
		strcpy(work.name,it->first.c_str());							// ���O���擾
		strcpy(work.pass, it->second->GetResourcePass().c_str());		// �f�ނ̃p�X���擾
		work.enable = it->second->GetEnable();							// �X�V�E�`��ۂ��擾
		work.sta = it->second->GetStatic();								// �ÓI�I�u�W�F�N�g���ǂ����擾
		Objects.push_back(work);										// �I�u�W�F�N�g�ۑ��p�z��ɒǉ�
		++it;											// �C�e���[�^�����炷
	}

	// ---�t�@�C������---
	FILE* fp;
	fopen_s(&fp, filename.c_str(), "wb");			// �t�@�C���I�[�v��

	if (fp)
	{
		fwrite(&Objects[0], sizeof(SceneObjData), Objects.size() , fp);		// �t�@�C���ɏ�������
	}
	fclose(fp);										// �t�@�C���N���[�Y
}

// ===�V�[���f�[�^�ۑ�===
void SceneBase::SaveSceneData()
{
	// ---�I�u�W�F�N�g�f�[�^�擾---
	std::vector<SceneObjData> Objects;			// �I�u�W�F�N�g�ۑ��p�z��
	SceneObjData work;							// �ϊ��p
	auto it = m_pObjectList.begin();			// �I�u�W�F�N�g���X�g�̐擪�C�e���[�^�擾
	while (it != m_pObjectList.end())		// �I�[�܂ŌJ��Ԃ�
	{
		work.id = it->second->GetObjID();				// �^�O���擾
		work.transform = it->second->GetTransform();	// ���W�E��]�E�g�k���擾
		strcpy(work.name, it->first.c_str());			// ���O���擾
		strcpy(work.pass, it->second->GetResourcePass().c_str());		// �f�ނ̃p�X���擾
		work.enable = it->second->GetEnable();			// �X�V�E�`��ۂ��擾
		work.sta = it->second->GetStatic();				// �ÓI�I�u�W�F�N�g���ǂ����擾
		Objects.push_back(work);						// �I�u�W�F�N�g�ۑ��p�z��ɒǉ�
		++it;											// �C�e���[�^�����炷
	}

	// ---�t�@�C������---
	FILE* fp;
	std::string file;
	file = "data/scene/" + m_Name + ".dat";
	fopen_s(&fp, file.c_str(), "wb");				// �t�@�C���I�[�v��

	if (fp)
	{
		fwrite(&Objects[0], sizeof(SceneObjData), Objects.size(), fp);		// �t�@�C���ɏ�������
	}
	fclose(fp);										// �t�@�C���N���[�Y
}

// ===�V�[���f�[�^�Ǎ�===
bool SceneBase::LoadSceneData()
{
	FILE* fp;
	std::string file;
	file = "data/scene/" + m_Name + ".dat";

	// ---�t�@�C�����J��---
	fopen_s(&fp, file.c_str(), "rb");

	// ---�Ǎ����s---
	if (!fp)
		return false;

	// �t�@�C���̖����Ɉʒu�w��q���ړ�---
	fseek(fp, 0, SEEK_END);
	// �t�@�C���̖����̈ʒu���擾---
	size_t size = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	size /= sizeof(SceneObjData);		// �I�u�W�F�N�g�����擾

	// ---��O����---
	if (size > MAX_SCENE_OBJECT)
		return false;

	// ---�f�[�^�ǂݍ���---
	SceneObjData* op = new SceneObjData[size];
	fread(&op[0], sizeof(SceneObjData), size, fp);

	// ---�ǂݍ��񂾃f�[�^����I�u�W�F�N�g�𐶐�---
	for (int i = 0; i < size; i++)
	{
		Object* p = CreateGameObject(op[i].name, op[i].id);		// �I�u�W�F�N�g����

		// ---��O����
		if (p == nullptr)
			continue;

		p->SetTransform(op[i].transform);			// �ǂݍ��񂾍��W�A��]�A�g�k���Z�b�g
		p->SetEnable(op[i].enable);					// �X�V�E�`��̗L�����Z�b�g
		p->SetStatic(op[i].sta);					// �ÓI�I�u�W�F�N�g���ǂ������Z�b�g
		p->SetResource(op[i].pass);					// �f�ނ̃p�X���Z�b�g
	}

	// ---�t�@�C�������
	fclose(fp);
	delete[] op;
	return true;
}

// ===�V�[���f�[�^�Ǎ�===
bool SceneBase::LoadSceneData(std::string filename)
{
	FILE* fp;
	fopen_s(&fp, filename.c_str(), "rb");		// �t�@�C���I�[�v��
	// ---�Ǎ����s---
	if (!fp)
		return false;

	// �t�@�C���̖����Ɉʒu�w��q���ړ�
	fseek(fp, 0, SEEK_END);
	// �t�@�C���̖����̈ʒu���擾
	size_t size = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	size /= sizeof(SceneObjData);		// �I�u�W�F�N�g�����擾

	// ---��O����---
	if (size > MAX_SCENE_OBJECT)
		return false;

	// ---�t�@�C���Ǎ�---
	SceneObjData* op = new SceneObjData[size];
	fread(&op[0], sizeof(SceneObjData), size, fp);

	// ---�ǂݍ��񂾃f�[�^����I�u�W�F�N�g�𐶐�---
	for (int i = 0; i < size; i++)
	{
		Object* p = CreateGameObject(op[i].name, op[i].id);		// �I�u�W�F�N�g����

		// ---��O����---
		if (p == nullptr)
			continue;

		p->SetTransform(op[i].transform);			// �ǂݍ��񂾍��W�A��]�A�g�k���Z�b�g
		p->SetEnable(op[i].enable);					// �X�V�E�`��̗L�����Z�b�g
		p->SetStatic(op[i].sta);					// �ÓI�I�u�W�F�N�g���ǂ������Z�b�g
		p->SetResource(op[i].pass);					// �f�ނ̃p�X���Z�b�g
	}

	// ---�I������---
	fclose(fp);
	delete[] op;
	return true;
}

void SceneBase::LoadStageData(std::string filename, float rot , int level)
{
	// ---���݂̃I�u�W�F�N�g����U�X�V�A�`�斳���̏�Ԃɂ���---
	auto it = m_pObjectList.begin();
	while (it != m_pObjectList.end())
	{
		it->second->SetEnable(false);
		++it;
	}

	//---�t�@�C����ǂݍ���---
	FILE* fp;
	fopen_s(&fp, filename.c_str(), "rb");
	// ---�Ǎ����s---
	if (!fp)
		return;

	// �t�@�C���̖����Ɉʒu�w��q���ړ�
	fseek(fp, 0, SEEK_END);
	// �t�@�C���̖����̈ʒu���擾
	size_t size = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	size /= sizeof(SceneObjData);		// �I�u�W�F�N�g�����擾

	// ---��O����
	if (size > MAX_SCENE_OBJECT)
		return;

	SceneObjData* op = new SceneObjData[size];

	// ---�t�@�C���Ǎ�
	fread(&op[0], sizeof(SceneObjData), size, fp);

	// ---�t�@�C�������
	int a = fclose(fp);

	// ---�ǂݍ��񂾃f�[�^����I�u�W�F�N�g�𐶐�---
	for (int i = 0; i < size; i++)
	{
		Object* p = CreateGameObject(op[i].name, op[i].id);		// �I�u�W�F�N�g����

		// ---��O����---
		if (p == nullptr)
			continue;

		// ---�X�e�[�W��]---
		TransformRotate(&op[i].transform, XMFLOAT3(0.0f, -1.0f, 0.0f), rot);		// �A�����Ă��镔���̕����ɂ���ĉ�]������

		p->SetTransform(op[i].transform);			// �ǂݍ��񂾍��W�A��]�A�g�k���Z�b�g
		p->SetEnable(op[i].enable);					// �X�V�E�`��̗L�����Z�b�g
		p->SetStatic(op[i].sta);					// �ÓI�I�u�W�F�N�g���ǂ������Z�b�g
		p->SetResource(op[i].pass);					// �f�ނ̃p�X���Z�b�g

		if (p->GetObjID() == OT_DOOR)			// ���������I�u�W�F�N�g���h�A�������ꍇ
			((Door*)p)->SetDirection();			// �������Z�b�g
	}

	delete[] op;		// ���������

	// ---�G�L��������---
	if (!MAP->GetClear() && MAP->GetCurrentRoom()->type != ROOM_GOAL)						// �X�e�[�W���N���A��ԂɂȂ��Ă��Ȃ��ꍇ�͐���
	{
		int num = Random<int>(1, 4 + level);	// �������������_���ɐU�蕪��
		for (int i = 0; i < num; i++)
		{
			Object* enemy;						// �����p
			if (Random<int>(1, 5) <= 3)			// 3/5�̊m���Œʏ�G
			{
				enemy = CreateGameObject("Enemy", OT_ENEMY);		// �ʏ�G����
			}
			else								// ����ȊO�͉������G
			{
				enemy = CreateGameObject("BulletEnemy", OT_ENEMY_BULLET);		// �������G����
			}
			enemy->SetRot(XMFLOAT3(0.0f, 0.0f, 0.0f));							// �����𐳖ʕ�����
			enemy->SetPos(XMFLOAT3(Random<float>(-9.0f, 9.0f), 1.0f, Random<float>(-9.0f, 9.0f)));		// ���W�������_���ɃZ�b�g
		}
	}

	// ---�v���C���[�Đ���---
	Object* player = FindObjectWithTag(OT_PLAYER);		// �v���C���[�擾
	if(player)
		player->SetEnable(true);
	CreateGameObject("Life", UI_LIFE);			// ���C�tUI�Đ���
	CreateGameObject("Stamina", UI_STAMINA);	// �X�^�~�iUI�Đ���
	CreateGameObject("Score", UI_SCORE);		// �X�R�AUI�Đ���
}

// ===�S�I�u�W�F�N�g�擾===
std::map<std::string, Object*>* SceneBase::GetAllObjects()
{
	return &m_pObjectList;
}

// ===�`�揈��===
void SceneBase::Draw()
{
	// ---��O����---
	auto it = m_pObjectList.begin();
	if (it == m_pObjectList.end())
		return;

	// =====�e�e�N�X�`���ɕ`�悷��=====
	DirectX::XMFLOAT4X4 mat[3];
	DirectX::XMStoreFloat4x4(&mat[0], DirectX::XMMatrixIdentity());

	// ---�������ݐ�̕ύX---
	Texture* pCanvas = m_pCanvas;
	Texture* pMask = m_pCanvasObject;

	DepthStencil* pDepth = m_pDepth;

	// ---���߃e�N�X�`���`��---
	RenderTarget* pRTVCanvas = reinterpret_cast<RenderTarget*>(pMask);
	ID3D11RenderTargetView* pView = pRTVCanvas->GetView();
	GetDeviceContext()->OMSetRenderTargets(1, &pView, pDepth->GetView());

	// ---�`���̃N���A---
	float color[4] = { 1.0f,1.0f,1.0f,1.0f };					// ���s��r�̂��߂ɁA��ԉ���1�������l�Őݒ�
	float disa[4] = { 0.0f,0.0f,0.0f,0.0f };
	GetDeviceContext()->ClearRenderTargetView(pView, disa);
	GetDeviceContext()->ClearDepthStencilView(pDepth->GetView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	// ---�V�F�[�_�[�Z�b�g---
	GetVS(VS_DEPTHWRITE)->Bind();
	GetPS(PS_DEPTHWRITE)->Bind();

	// ---�v���C���[���e�N�X�`���ɏ�������---
	std::vector<Object*> Objects = FindObjectListWithTag(OT_ENEMY);
	std::vector<Object*> bullets = FindObjectListWithTag(OT_ENEMY_BULLET);
	Object* player = FindObjectWithTag(OT_PLAYER);

	for (auto bullet : bullets)
	{
		Objects.push_back(bullet);
	}
	if(player)
		Objects.push_back(player);
	
	for(auto obj : Objects)
	{
		XMStoreFloat4x4(&mat[0], XMMatrixTranspose(XMLoadFloat4x4(&obj->GetWorld())));								// ���[���h�ϊ��s��
		XMStoreFloat4x4(&mat[1], XMMatrixTranspose(XMLoadFloat4x4(&CAMERA->GetCurrentCamera()->GetViewMatrix())));		// �r���[�s��
		XMStoreFloat4x4(&mat[2], XMMatrixTranspose(XMLoadFloat4x4(&CAMERA->GetCurrentCamera()->GetProjMatrix())));		// �r���[�s��

		m_pWVP->Write(mat);							// �萔�o�b�t�@�ɏ�������
		m_pWVP->BindVS(0);							// ���_�V�F�[�_�\�ɃZ�b�g
		obj->DrawShadow();
	}

	// ---�V���h�E�}�b�v����---
	pRTVCanvas = reinterpret_cast<RenderTarget*>(pCanvas);
	pView = pRTVCanvas->GetView();

	GetDeviceContext()->OMSetRenderTargets(1, &pView, pDepth->GetView());

	// ---�`���̃N���A---
	GetDeviceContext()->ClearRenderTargetView(pView, color);
	GetDeviceContext()->ClearDepthStencilView(pDepth->GetView(),D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	XMFLOAT3 look = CAMERA->GetCurrentCamera()->GetLook();

	// ---���z���猩�����_�̐ݒ�---(���z�̈ʒu�ɃJ����������Ƒz�肵�āA�e�N�X�`���ɏ�������)
	DirectX::XMMATRIX sunView = DirectX::XMMatrixLookAtLH(			//   �J������������x�������ʒu��ݒ肷��
		DirectX::XMVectorSet(-300.0f, 1000.0f, -1000.0f, 0.0f),
		DirectX::XMVectorSet(look.x, look.y, look.z ,0.0f),
		DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));

	DirectX::XMMATRIX sunProj = DirectX::XMMatrixOrthographicLH(50, (16 / 9) * 50, 0.2f, 2000.0f);

	// ---�]�u�v�Z---
	DirectX::XMStoreFloat4x4(&mat[1], DirectX::XMMatrixTranspose(sunView));
	DirectX::XMStoreFloat4x4(&mat[2], DirectX::XMMatrixTranspose(sunProj));

	// ---�I�u�W�F�N�g���e�N�X�`���ɏ�������---
	while (it != m_pObjectList.end())
	{
		XMMATRIX m = XMLoadFloat4x4(&it->second->GetWorld());				// �I�u�W�F�N�g�̃��[���h�ϊ��}�g���b�N�X���擾
		DirectX::XMStoreFloat4x4(&mat[0], DirectX::XMMatrixTranspose(m));	// �V�F�[�_�[�ɓn�����߂ɓ]�u
		m_pWVP->Write(mat);							// �萔�o�b�t�@�ɏ�������
		m_pWVP->BindVS(0);							// ���_�V�F�[�_�\�ɃZ�b�g
		if(it->second->GetObjID() != OT_WALL)		// ---�ǃI�u�W�F�N�g�͉e���������܂Ȃ�(�Â��Ȃ肷���邽��)
			it->second->DrawShadow();				// �`��

		++it;		// ���̃I�u�W�F�N�g
	}

	// ---�`�������̉�ʂɖ߂�---
	ResetRenderTarget();
	m_pCBSun->Write(&mat[1]);
	m_pCBSun->BindVS(2);

	GetVS(VS_OBJECT)->Bind();
	GetPS(PS_LAMBERT)->Bind();
	XMFLOAT3 Pos = CAMERA->GetCurrentCamera()->GetPos();
	XMFLOAT4 buf = XMFLOAT4(Pos.x, Pos.y, Pos.z, 0.0f);
	m_pCBCamera->Write(&buf);
	m_pCBCamera->BindVS(1);									// �J�����萔���V�F�[�_�[�ɓn��
	m_pCBCamera->BindPS(1);

	m_pCBlight->Write(&m_LightDir);							// ���C�g�萔��n��
	m_pCBlight->BindPS(0);

	m_pCBColor->Write(&Color);
	m_pCBColor->BindPS(2);									// �F�萔��n��

	SetTexturePS(pCanvas->GetResource(), 2);				// �������񂾃e�N�X�`����n��
	SetTexturePS(pMask->GetResource(), 3);					// �������񂾃e�N�X�`����n��
}

void SceneBase::SetColorBuffer(XMFLOAT4* color)
{
	 m_pCBColor->Write(color); 			// �萔�o�b�t�@�ɐF���Z�b�g(�֊s���Ȃ�)
	 m_pCBColor->BindPS(2);
}