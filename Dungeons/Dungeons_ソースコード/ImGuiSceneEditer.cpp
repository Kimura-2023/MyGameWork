#include "ImGuiSceneEditer.h"
#include "SceneManager.h"
#include "CollisionManager.h"
#include "CameraManager.h"
#include "GameCamera.h"
#include "Input.h"
#include "ModelManager.h"
#include "TextureManager.h"

Object* ImGuiSceneEditer::m_pSelectObj = nullptr;

ImGuiSceneEditer::ImGuiSceneEditer()
{
	m_pSelectObj = nullptr;
	m_bObjectList = true;
	m_bInspecter = true;
	m_bEditerWindow = false;
}

ImGuiSceneEditer::~ImGuiSceneEditer()
{

}

void ImGuiSceneEditer::Contents()
{
	ImGui::SetNextWindowPos(ImVec2(SCREEN_WIDTH - 280, 0), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(280, 60), ImGuiCond_Once);
	ImGuiWindowFlags flag = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar;
	ImGui::Begin("Edit", nullptr, flag);
	if (ImGui::BeginMenuBar()) {
		if (ImGui::BeginMenu(u8"�E�B���h�E"))
		{
			if (ImGui::MenuItem(u8"�C���X�y�N�^�["))
				m_bInspecter = true;
			if (ImGui::MenuItem(u8"�V�[���I�u�W�F�N�g"))
				m_bObjectList = true;
			if (ImGui::MenuItem(u8"�I�u�W�F�N�g�G�f�B�^"))
				m_bEditerWindow = true;

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu(u8"�v���C"))
		{
			if (ImGui::MenuItem(u8"�J�n"))
			{
				CameraManager::Get()->RegisterCamera("GameCamera", GAME_CAMERA);
				CameraManager::Get()->SetCamera(CameraManager::Get()->FindCamera("GameCamera"));
				Object* player = SCENE->GetCurrentScene()->CreateGameObject("Player", OT_PLAYER);
				((GameCamera*)(CAMERA->GetCurrentCamera()))->SetTargetObj(player);
				m_pSelectObj = nullptr;
			}
			if (ImGui::MenuItem(u8"�I��"))
			{
				SCENE->GetCurrentScene()->DeleteGameObject("Player");
				CameraManager::Get()->SetCamera(CameraManager::Get()->FindCamera("DebugCamera"));
				m_pSelectObj = nullptr;
			}

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu(u8"�V�[��"))
		{
			if (ImGui::MenuItem(u8"�j��"))
			{
				SCENE->GetCurrentScene()->ReleaseAllGameObject();
				m_pSelectObj = nullptr;
			}

			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}
	int fps = GetFps();
	ImGui::Text("fps:%d",fps);
	ImGui::End();

	if (m_bObjectList)
	{
		// ---�V�[���֘A
		ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Once);
		ImGui::SetNextWindowSize(ImVec2(300, SCREEN_HEIGHT), ImGuiCond_Once);
		ImGuiWindowFlags wflag = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_HorizontalScrollbar;
		ImGui::Begin(SCENE->GetCurrentScene()->GetSceneName().c_str(), &m_bObjectList, wflag);
		ObjListWnd();
		ImGui::End();
	}

	if (m_bInspecter)
	{
		// ---�I�𒆃I�u�W�F�N�g�ڍ�---
		ImGui::SetNextWindowPos(ImVec2(SCREEN_WIDTH - 380, SCREEN_HEIGHT - 320), ImGuiCond_Once);
		ImGui::SetNextWindowSize(ImVec2(380, 320), ImGuiCond_Once);
		ImGuiWindowFlags wflag = ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_NoResize;
		ImGui::Begin(u8"�C���X�y�N�^�[", &m_bInspecter, wflag);
		InspecterWnd();
		ImGui::End();
	}

	if (m_bEditerWindow)
	{
		ImGui::SetNextWindowPos(ImVec2(SCREEN_WIDTH - 380, 100), ImGuiCond_Once);
		ImGui::SetNextWindowSize(ImVec2(380, 300), ImGuiCond_Once);
		ImGuiWindowFlags wflag = ImGuiWindowFlags_HorizontalScrollbar;
		ImGui::Begin(u8"�I�u�W�F�N�g����", &m_bEditerWindow, wflag);
		ObjEditerWnd();
		ImGui::End();
	}

	// ---�L�[���͂ł̃I�u�W�F�N�g����
	if (!m_pSelectObj || SCENE->GetCurrentScene()->FindGameObject("Player"))
		return;

	if (CInput::GetKeyPress(VK_SPACE))
	{
		// �J�����̉�]�}�g���b�N�X�擾
		Transform t = m_pSelectObj->GetTransform();						// �I�𒆂̃I�u�W�F�N�g�̍��W�Ȃǂ��擾
		XMFLOAT3 view = CAMERA->GetCurrentCamera()->GetLookVector();
		Quaternion q;
		Quaternion::LookRotation(&q, view);

		XMFLOAT3 Movement = { 0.0f,0.0f,0.0f };

		if (CInput::GetKeyPress(VK_RIGHT) || CInput::GetKeyPress(VK_D))
			Movement.x += 0.01f;
		if (CInput::GetKeyPress(VK_LEFT) || CInput::GetKeyPress(VK_A))
			Movement.x -= 0.01f;
		if (CInput::GetKeyPress(VK_UP) || CInput::GetKeyPress(VK_W))
			Movement.z += 0.01f;
		if (CInput::GetKeyPress(VK_DOWN) || CInput::GetKeyPress(VK_S))
			Movement.z -= 0.01f;
		if (CInput::GetKeyPress(VK_PRIOR) || CInput::GetKeyPress(VK_E))
			Movement.y += 0.01f;
		if (CInput::GetKeyPress(VK_NEXT) || CInput::GetKeyPress(VK_Q))
			Movement.y -= 0.01f;

		Quaternion p(Movement);
		p = -q * p * q;
		Movement = XMFLOAT3(p.x, p.y, p.z);
		t.Pos.x += Movement.x;
		t.Pos.y += Movement.y;
		t.Pos.z += Movement.z;
		m_pSelectObj->SetTransform(t);
	}
}

void ImGuiSceneEditer::ObjEditerWnd()
{
	static ImGuiComboFlags flags = ImGuiComboFlags_HeightSmall;
	static int item_current_idx = 0;
	static int modelidx = 0;
	static int textureidx = 0;

	const char* combo_preview_value = GetObjectTagName((ObjType)item_current_idx);
	if (ImGui::BeginCombo("Kind", combo_preview_value, flags))
	{
		for (int n = 0; n < MAX_GAMEOBJTYPE; n++)
		{
			const bool is_selected = (item_current_idx == n);
			if (ImGui::Selectable(GetObjectTagName((ObjType)n), is_selected))
				item_current_idx = n;

			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}

	static char CreateName[50] = "Object";
	ImGui::InputText("Name", CreateName,IM_ARRAYSIZE(CreateName));

	static bool EnableTransform = false;
	ImGui::Checkbox("Transform", &EnableTransform);

	static Transform createTransform = Transform();
	if (EnableTransform)
	{
		static float createpos[3] = { 0.0f,0.0f,0.0f };
		static float createrot[3] = { 0.0f,0.0f,0.0f };
		static float createscl[3] = { 1.0f,1.0f,1.0f };

		if (ImGui::DragFloat3(u8"Position", createpos, 0.02f, 0, 0, "%.3f", 0))
		{
			createTransform.Pos = XMFLOAT3(createpos[0], createpos[1], createpos[2]);
		}
		if (ImGui::DragFloat3(u8"Rotation", createrot, 0.02f, 0, 0, "%.3f", 0))
		{
			createTransform.Rot = XMFLOAT3(createrot[0], createrot[1], createrot[2]);
		}
		if (ImGui::DragFloat3(u8"Scale", createscl, 0.02f, 0, 0, "%.3f", 0))
		{
			createTransform.Scale = XMFLOAT3(createscl[0], createscl[1], createscl[2]);
		}
	}

	if (item_current_idx == BG_3DOBJ)
	{
		combo_preview_value = GetModelPass((MODEL_TYPE)modelidx);
		if (ImGui::BeginCombo("Model", combo_preview_value, flags))
		{
			for (int n = 0; n < MAX_MODEL; n++)
			{
				const bool is_selected = (modelidx == n);
				if (ImGui::Selectable(GetModelPass((MODEL_TYPE)n), is_selected))
					modelidx = n;

				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}
	}

	if (item_current_idx == OT_GROUND)
	{
		combo_preview_value = GetTexturePass((TEX_TYPE)textureidx);
		if (ImGui::BeginCombo("Texture", combo_preview_value, flags))
		{
			for (int n = 0; n < MAX_TEX; n++)
			{
				const bool is_selected = (textureidx == n);
				if (ImGui::Selectable(GetTexturePass((TEX_TYPE)n), is_selected))
					textureidx = n;

				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}
	}

	if(ImGui::Button(u8"����", ImVec2(100, 20)))
	{
		Object* pobj = SCENE->GetCurrentScene()->CreateGameObject(CreateName, (ObjType)item_current_idx);
		if (!pobj)
			return;
		if (EnableTransform)
			pobj->SetTransform(createTransform);

		if (item_current_idx == BG_3DOBJ)
		{
			pobj->SetResource(GetModelPass((MODEL_TYPE)modelidx));
		}
		if (item_current_idx == OT_GROUND)
		{
			pobj->SetResource(GetTexturePass((TEX_TYPE)textureidx));
		}
	}

}

void ImGuiSceneEditer::ObjListWnd()
{
	if (ImGui::BeginMenuBar()) {
		if (ImGui::BeginMenu(u8"�t�@�C��"))
		{
			if (ImGui::MenuItem(u8"�Z�[�u")) {
				// ---���ݔz�u����Ă���I�u�W�F�N�g���V�[���f�[�^�ɕۑ�����---

				// ---�V�[��������V�[���f�[�^�t�@�C����ǂݍ���
				//--- �u�t�@�C�����J���v�_�C�A���O�̕\��
				std::string work = SCENE->GetCurrentScene()->GetSceneName() + ".dat";
				char fileName[MAX_PATH] = "";
				strcpy(fileName, work.c_str());

				OPENFILENAME ofn;
				ZeroMemory(&ofn, sizeof(ofn));
				ofn.lStructSize = sizeof(ofn);
				ofn.hwndOwner = GetActiveWindow();
				ofn.lpstrFile = fileName;			// �J���őI�������t�@�C���̃p�X���i�[
				ofn.nMaxFile = sizeof(fileName);

				// �_�C�A���O�E���́A�t�@�C���g���q�̃t�B���^�[�ݒ�
				// [�\����\0�g���q\0] �̑g�ݍ��킹�A\0�ŋ�؂�
				ofn.lpstrFilter = "All Files\0*.*\0.dat\0*.dat\0";
				ofn.nFilterIndex = 2;	// �ŏ��Ɏw�肵�Ă���t�B���^�[
				ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR;
				// �_�C�A���O��\��
				if (TRUE == GetSaveFileName(&ofn))
				{
					SCENE->GetCurrentScene()->SaveSceneData(fileName);
				}

			}
			if (ImGui::MenuItem(u8"���[�h")) {
				// ---�V�[��������V�[���f�[�^�t�@�C����ǂݍ���---

				//--- �u�t�@�C�����J���v�_�C�A���O�̕\��
				char fileName[MAX_PATH] = "";
				OPENFILENAME ofn;
				ZeroMemory(&ofn, sizeof(ofn));
				ofn.lStructSize = sizeof(ofn);
				ofn.hwndOwner = GetActiveWindow();
				ofn.lpstrFile = fileName;			// �J���őI�������t�@�C���̃p�X���i�[
				ofn.nMaxFile = sizeof(fileName);

				// �_�C�A���O�E���́A�t�@�C���g���q�̃t�B���^�[�ݒ�
				// [�\����\0�g���q\0] �̑g�ݍ��킹�A\0�ŋ�؂�
				ofn.lpstrFilter = "All Files\0*.*\0.dat\0*.dat\0";
				ofn.nFilterIndex = 2;	// �ŏ��Ɏw�肵�Ă���t�B���^�[
				ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
				// �_�C�A���O��\��
				if (TRUE == GetOpenFileName(&ofn))
				{
					SCENE->GetCurrentScene()->LoadSceneData(fileName);
				}

			}

			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}

	// ---�I�u�W�F�N�g���X�g(�c���[)---
	ImGuiTreeNodeFlags flag = ImGuiTreeNodeFlags_DefaultOpen;
	ImGui::SetNextItemOpen(true, ImGuiCond_Once);
	if (ImGui::TreeNode(u8"�I�u�W�F�N�g"))
	{
		// ---�m�[�h�̐ݒ�
		static ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
		static bool align_label_with_current_x_position = false;
		static bool test_drag_and_drop = false;
		static int selection_mask = (1 << 2);
		int node_clicked = -1;

		auto map = SCENE->GetCurrentScene()->GetAllObjects();
		auto it = map->begin();

		int i = 0;

		while (it != map->end())
		{
			ImGuiTreeNodeFlags node_flags = base_flags;
			const bool is_selected = (selection_mask & (1 << i)) != 0;
			if (is_selected)
				node_flags |= ImGuiTreeNodeFlags_Selected;

			node_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
			ImGui::TreeNodeEx((void*)(intptr_t)i, node_flags, it->first.c_str());

			// ---�m�[�h�N���b�N��---
			if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
			{
				node_clicked = i;
				if (m_pSelectObj != nullptr)
				{
					m_pSelectObj->SetSelect(false);						// �O�I���I�u�W�F�N�g�̑I����Ԃ��X�V����
				}

				// ---�I����ԍX�V---
				m_pSelectObj = it->second;
				m_pSelectObj->SetSelect(true);
			}
			++it;
			++i;
		}
		if (node_clicked != -1)
		{
			selection_mask = (1 << node_clicked);           // Click to single-select
		}

		if (align_label_with_current_x_position)
			ImGui::Indent(ImGui::GetTreeNodeToLabelSpacing());
		ImGui::TreePop();
	}
}

void ImGuiSceneEditer::InspecterWnd()
{
	if (m_pSelectObj != nullptr)
	{
		ImGui::Text(m_pSelectObj->GetName().c_str());

		ImGuiTreeNodeFlags flag = ImGuiTreeNodeFlags_DefaultOpen;
		ImGui::SetNextItemOpen(true, ImGuiCond_Once);
		if (ImGui::TreeNode("Transform"))
		{
			Transform work = m_pSelectObj->GetTransform();
			float pos[3] = { work.Pos.x,work.Pos.y,work.Pos.z };
			float rot[3] = { work.Rot.x,work.Rot.y,work.Rot.z };
			float scl[3] = { work.Scale.x,work.Scale.y,work.Scale.z };
			if (ImGui::DragFloat3(u8"Position", pos, 0.02f, 0, 0, "%.3f", 0))
			{
				m_pSelectObj->SetPos(XMFLOAT3(pos[0], pos[1], pos[2]));
			}
			if (ImGui::DragFloat3(u8"Rotation", rot, 0.02f, 0, 0, "%.3f", 0))
			{
				m_pSelectObj->SetRot(XMFLOAT3(rot[0], rot[1], rot[2]));
			}
			if (ImGui::DragFloat3(u8"Scale", scl, 0.02f, 0, 0, "%.3f", 0))
			{
				m_pSelectObj->SetScl(XMFLOAT3(scl[0], scl[1], scl[2]));
			}
			ImGui::TreePop();
		}

		bool enable = m_pSelectObj->GetEnable();
		bool Static = m_pSelectObj->GetStatic();
		ImGui::Checkbox("Enable", &enable);
		ImGui::SameLine();
		ImGui::Checkbox("Static", &Static);
		m_pSelectObj->SetEnable(enable);
		m_pSelectObj->SetStatic(Static);

		// ---�I�u�W�F�N�g���̒ǋL�p�����[�^---
		m_pSelectObj->ImGuiWindow();

		if (ImGui::Button(u8"����"))
		{
			if (SCENE->GetCurrentScene()->DeleteGameObject(m_pSelectObj->GetName()))
				m_pSelectObj = nullptr;
		}
	}
}