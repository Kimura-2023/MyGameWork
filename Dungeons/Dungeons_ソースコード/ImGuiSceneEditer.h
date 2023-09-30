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
	static Object* m_pSelectObj;		// �E�B���h�E��őI�𒆂̃I�u�W�F�N�g
private:
	bool m_bObjectList;					// �V�[���I�u�W�F�N�g
	bool m_bInspecter;					// �C���X�y�N�^�[
	bool m_bEditerWindow;				// �I�u�W�F�N�g�����E�B���h�E
};