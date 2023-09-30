#pragma once
// ===�V�[���x�[�X�N���X��`===
// �V�[���x�[�X�N���X���`
// Author kimura
// =================================

// ===�C���N���[�h===
#include "main.h"
#include "Object.h"
#include <map>
#include "DepthStencil.h"

// ===�N���X��`===
class SceneBase
{
	// ---�����o�֐�---
public:
	SceneBase();					// �R���X�g���N�^
	virtual ~SceneBase();			// �f�X�g���N�^
	virtual void Update() = 0;		// �X�V����
	virtual void Draw();			// �`�揈��

	// **�V�[���f�[�^�֘A�֐�**
	Object* FindGameObject(std::string);								// �I�u�W�F�N�g�T��
	Object* CreateGameObject(std::string, ObjType, int num = 0);		// �I�u�W�F�N�g����
	bool DeleteGameObject(std::string);									// �I�u�W�F�N�g����
	void ReleaseAllGameObject();										// �S�I�u�W�F�N�g���

	int& RangeObjectCount(XMFLOAT3&, XMFLOAT3&);						// ����͈͓��̃I�u�W�F�N�g�������J�E���g

	void SaveSceneData();							// �V�[���f�[�^��ۑ�
	void SaveSceneData(std::string);				// �V�[���f�[�^��ۑ�(�t�@�C�����w��)
	bool LoadSceneData();							// �V�[���f�[�^�Ǎ�
	bool LoadSceneData(std::string);				// �V�[���f�[�^�Ǎ�(�t�@�C�����w��)

	void LoadStageData(std::string,float rot,int level);		// �X�e�[�W�f�[�^�Ǎ�

	std::string GetSceneName() { return m_Name; }		// �V�[�����擾
	std::map<std::string, Object*>* GetAllObjects();	// �S�I�u�W�F�N�g�擾

	Object* FindObjectWithTag(ObjType);						// �I�u�W�F�N�g�^�O����I�u�W�F�N�g�T���E�擾
	std::vector<Object*> FindObjectListWithTag(ObjType);	// �I�u�W�F�N�g�^�O����I�u�W�F�N�g���X�g�擾

	void SetColorBuffer(XMFLOAT4* color);				// �֊s���`��F�Z�b�g

	ConstantBuffer* GetSunBuffer() { return m_pCBSun; }	// DirectionalLight�o�b�t�@�擾

	// ---�����o�ϐ�---
protected:
	std::string m_Name;								// �V�[����
	XMFLOAT4 m_LightDir;							// ���C�g(���z��)
	XMFLOAT4 Color;									// �֊s���F
private:
	std::map<std::string, Object*> m_pObjectList;	// �V�[�����̃I�u�W�F�N�g���X�g

	// **�e�̕`��p**
	Texture* m_pCanvas;
	DepthStencil* m_pDepth;

	// **�v���C���[�����\���p**
	Texture* m_pCanvasObject;

	// **�萔�o�b�t�@**
	ConstantBuffer* m_pCBlight;						// ���C�g���p�萔�o�b�t�@
	ConstantBuffer* m_pCBCamera;					// �J�����p�萔�o�b�t�@
	ConstantBuffer* m_pCBColor;						// �F���p�o�b�t�@
	ConstantBuffer* m_pCBSun;						// �e�p�̑��z�p�o�b�t�@
	ConstantBuffer* m_pWVP;							// �e���e�N�X�`���ɏ������ނ��߂̃o�b�t�@
};