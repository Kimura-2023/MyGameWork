#pragma once

#include "ImGuiBase.h"
#include "main.h"
#include "Object.h"

class ImGuiSceneEditer : public ImGuiBase
{
public:
	ImGuiSceneEditer();
	~ImGuiSceneEditer();
	void Contents();

private:
	void ObjEditerWnd();
	void ObjListWnd();
	void InspecterWnd();
public:
	static Object* m_pSelectObj;		// ウィンドウ上で選択中のオブジェクト
private:
	bool m_bObjectList;					// シーンオブジェクト
	bool m_bInspecter;					// インスペクター
	bool m_bEditerWindow;				// オブジェクト生成ウィンドウ
};