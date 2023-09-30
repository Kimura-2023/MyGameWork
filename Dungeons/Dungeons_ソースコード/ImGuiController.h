#pragma once
#include "main.h"
#include "ImGuiBase.h"
#include <list>

enum IMGUISTATE
{
	IMGUI_TEST,
	IMGUI_MAX,
};

class ImGuiController
{
public:
	static void Init();
	static void Uninit();

	static void Contents();
	static void AddImGui(ImGuiBase*);
	static void ReleaseAll();
	static void Release(ImGuiBase*);

	static void CreateImGui(IMGUISTATE);
private:
	static std::list<ImGuiBase*> m_pImGuiList;
};
