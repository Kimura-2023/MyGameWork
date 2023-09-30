// ===ボタン選択管理クラス実装===
// ボタン選択管理クラスを実装
// Author kimura
// ======================

// ===インクルード===
#include "SelectButtonManager.h"
#include "Input.h"

// ===静的メンバ===
SelectButtonManager* SelectButtonManager::m_pInstance = nullptr;			// インスタンス

// ===コンストラクタ===
SelectButtonManager::SelectButtonManager()
{
	// ---初期化処理---
	m_nButtonCnt = 0;					// 選択番号
	m_pCurrentSelect = nullptr;			// 現在選択ボタン
}

// ===デストラクタ===
SelectButtonManager::~SelectButtonManager()
{
}

// ===インスタンス生成===
void SelectButtonManager::Create()
{
	if (!m_pInstance)
	{
		m_pInstance = new SelectButtonManager;		// インスタンスを生成
	}
}

// ===インスタンス取得===
SelectButtonManager* SelectButtonManager::Get()
{
	return m_pInstance;						// インスタンス解放
}

// ===インスタンス破棄===
void SelectButtonManager::Destroy()
{
	delete m_pInstance;						// インスタンス解放
}

// ===更新処理===
void SelectButtonManager::Update()
{
	// ---選択中ボタンが無ければ何もしない---
	if (!m_pCurrentSelect)
		return;

	// ---キー入力での選択更新---
	if (CInput::GetKeyRepeat(VK_DOWN) || CInput::GetKeyRepeat(VK_S) || CInput::GetStickLTrigger(JOYSTICKID1,DR_DOWN,20))			// 下方向
	{
		int index = (m_pCurrentSelect->GetIndex() + 1) % m_nButtonCnt;		// 選択番号更新
		SetSelect(index);													// 選択ボタンを設定
	}
	if (CInput::GetKeyRepeat(VK_UP) || CInput::GetKeyRepeat(VK_W) || CInput::GetStickLTrigger(JOYSTICKID1, DR_UP, 20))				// 上方向
	{
		int index = m_pCurrentSelect->GetIndex() - 1;						// 選択番号更新
		if (index < 0)
			index = m_nButtonCnt - 1;										// 選択番号補正
		SetSelect(index);													// 選択ボタンを設定
	}

	// ---ボタン更新可否で判定---
	if (!m_pCurrentSelect->GetEnable())
		return;

	// ---ボタン押す処理---
	if (CInput::GetKeyTrigger(VK_RETURN) || CInput::GetMouseTrigger(MOUSEBUTTON_L) || CInput::GetJoyTrigger(JOYSTICKID1,VB_A))
		m_pCurrentSelect->OnPushed();				// ボタン押下処理
}

// ===選択ボタン設定===
void SelectButtonManager::SetSelect(int index)
{
	m_pCurrentSelect->SetSelected(false);			// 前のボタンを非選択状態に
	m_pCurrentSelect = m_pButtonList[index];		// ボタン更新
	m_pCurrentSelect->SetSelected(true);			// ボタンの選択状態更新
}

// ===リスト追加===
int SelectButtonManager::AddButtonManager(UIButton* button)
{
	if (button)
	{
		m_pButtonList.push_back(button);	// リストに追加
		++m_nButtonCnt;						// 総ボタン数を加算
	}

	// ---ボタンがセットされていない場合---
	if (!m_pCurrentSelect)
	{
		m_pCurrentSelect = button;			// 選択状態のボタンに設定
	}

	// ---追加後処理---
	return m_nButtonCnt - 1;				// インデックスを返す
}

// ===ボタン解放===
void SelectButtonManager::Release(UIButton* button)
{
	// ---終了処理---
	if (m_pCurrentSelect == button)
		m_pCurrentSelect = nullptr;					// 選択している物であれば更新

	auto it = m_pButtonList.begin();				// ボタンリストの先頭イテレータ取得
	while (*it != button)							// 一致するまで繰り返し
	{
		++it;										// イテレータをずらす
	}
	m_pButtonList.erase(it);						// リストから除外
	--m_nButtonCnt;									// ボタン数減少
}