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
		if (ImGui::BeginMenu(u8"ウィンドウ"))
		{
			if (ImGui::MenuItem(u8"インスペクター"))
				m_bInspecter = true;
			if (ImGui::MenuItem(u8"シーンオブジェクト"))
				m_bObjectList = true;
			if (ImGui::MenuItem(u8"オブジェクトエディタ"))
				m_bEditerWindow = true;

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu(u8"プレイ"))
		{
			if (ImGui::MenuItem(u8"開始"))
			{
				CameraManager::Get()->RegisterCamera("GameCamera", GAME_CAMERA);
				CameraManager::Get()->SetCamera(CameraManager::Get()->FindCamera("GameCamera"));
				Object* player = SCENE->GetCurrentScene()->CreateGameObject("Player", OT_PLAYER);
				((GameCamera*)(CAMERA->GetCurrentCamera()))->SetTargetObj(player);
				m_pSelectObj = nullptr;
			}
			if (ImGui::MenuItem(u8"終了"))
			{
				SCENE->GetCurrentScene()->DeleteGameObject("Player");
				CameraManager::Get()->SetCamera(CameraManager::Get()->FindCamera("DebugCamera"));
				m_pSelectObj = nullptr;
			}

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu(u8"シーン"))
		{
			if (ImGui::MenuItem(u8"破棄"))
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
		// ---シーン関連
		ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Once);
		ImGui::SetNextWindowSize(ImVec2(300, SCREEN_HEIGHT), ImGuiCond_Once);
		ImGuiWindowFlags wflag = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_HorizontalScrollbar;
		ImGui::Begin(SCENE->GetCurrentScene()->GetSceneName().c_str(), &m_bObjectList, wflag);
		ObjListWnd();
		ImGui::End();
	}

	if (m_bInspecter)
	{
		// ---選択中オブジェクト詳細---
		ImGui::SetNextWindowPos(ImVec2(SCREEN_WIDTH - 380, SCREEN_HEIGHT - 320), ImGuiCond_Once);
		ImGui::SetNextWindowSize(ImVec2(380, 320), ImGuiCond_Once);
		ImGuiWindowFlags wflag = ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_NoResize;
		ImGui::Begin(u8"インスペクター", &m_bInspecter, wflag);
		InspecterWnd();
		ImGui::End();
	}

	if (m_bEditerWindow)
	{
		ImGui::SetNextWindowPos(ImVec2(SCREEN_WIDTH - 380, 100), ImGuiCond_Once);
		ImGui::SetNextWindowSize(ImVec2(380, 300), ImGuiCond_Once);
		ImGuiWindowFlags wflag = ImGuiWindowFlags_HorizontalScrollbar;
		ImGui::Begin(u8"オブジェクト生成", &m_bEditerWindow, wflag);
		ObjEditerWnd();
		ImGui::End();
	}

	// ---キー入力でのオブジェクト操作
	if (!m_pSelectObj || SCENE->GetCurrentScene()->FindGameObject("Player"))
		return;

	if (CInput::GetKeyPress(VK_SPACE))
	{
		// カメラの回転マトリックス取得
		Transform t = m_pSelectObj->GetTransform();						// 選択中のオブジェクトの座標などを取得
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

	if(ImGui::Button(u8"生成", ImVec2(100, 20)))
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
		if (ImGui::BeginMenu(u8"ファイル"))
		{
			if (ImGui::MenuItem(u8"セーブ")) {
				// ---現在配置されているオブジェクトをシーンデータに保存する---

				// ---シーン名からシーンデータファイルを読み込む
				//--- 「ファイルを開く」ダイアログの表示
				std::string work = SCENE->GetCurrentScene()->GetSceneName() + ".dat";
				char fileName[MAX_PATH] = "";
				strcpy(fileName, work.c_str());

				OPENFILENAME ofn;
				ZeroMemory(&ofn, sizeof(ofn));
				ofn.lStructSize = sizeof(ofn);
				ofn.hwndOwner = GetActiveWindow();
				ofn.lpstrFile = fileName;			// 開くで選択したファイルのパスを格納
				ofn.nMaxFile = sizeof(fileName);

				// ダイアログ右下の、ファイル拡張子のフィルター設定
				// [表示名\0拡張子\0] の組み合わせ、\0で区切る
				ofn.lpstrFilter = "All Files\0*.*\0.dat\0*.dat\0";
				ofn.nFilterIndex = 2;	// 最初に指定しているフィルター
				ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR;
				// ダイアログを表示
				if (TRUE == GetSaveFileName(&ofn))
				{
					SCENE->GetCurrentScene()->SaveSceneData(fileName);
				}

			}
			if (ImGui::MenuItem(u8"ロード")) {
				// ---シーン名からシーンデータファイルを読み込む---

				//--- 「ファイルを開く」ダイアログの表示
				char fileName[MAX_PATH] = "";
				OPENFILENAME ofn;
				ZeroMemory(&ofn, sizeof(ofn));
				ofn.lStructSize = sizeof(ofn);
				ofn.hwndOwner = GetActiveWindow();
				ofn.lpstrFile = fileName;			// 開くで選択したファイルのパスを格納
				ofn.nMaxFile = sizeof(fileName);

				// ダイアログ右下の、ファイル拡張子のフィルター設定
				// [表示名\0拡張子\0] の組み合わせ、\0で区切る
				ofn.lpstrFilter = "All Files\0*.*\0.dat\0*.dat\0";
				ofn.nFilterIndex = 2;	// 最初に指定しているフィルター
				ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
				// ダイアログを表示
				if (TRUE == GetOpenFileName(&ofn))
				{
					SCENE->GetCurrentScene()->LoadSceneData(fileName);
				}

			}

			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}

	// ---オブジェクトリスト(ツリー)---
	ImGuiTreeNodeFlags flag = ImGuiTreeNodeFlags_DefaultOpen;
	ImGui::SetNextItemOpen(true, ImGuiCond_Once);
	if (ImGui::TreeNode(u8"オブジェクト"))
	{
		// ---ノードの設定
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

			// ---ノードクリック時---
			if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
			{
				node_clicked = i;
				if (m_pSelectObj != nullptr)
				{
					m_pSelectObj->SetSelect(false);						// 前選択オブジェクトの選択状態を更新する
				}

				// ---選択状態更新---
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

		// ---オブジェクト毎の追記パラメータ---
		m_pSelectObj->ImGuiWindow();

		if (ImGui::Button(u8"消去"))
		{
			if (SCENE->GetCurrentScene()->DeleteGameObject(m_pSelectObj->GetName()))
				m_pSelectObj = nullptr;
		}
	}
}