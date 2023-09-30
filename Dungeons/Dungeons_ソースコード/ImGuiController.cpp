#include "ImGuiController.h"
#include "GameManager.h"
#include "SceneManager.h"

std::list<ImGuiBase*> ImGuiController::m_pImGuiList;

void ImGuiController::Init()
{
	m_pImGuiList.resize(0);
}

void ImGuiController::Uninit()
{
	ReleaseAll();
}

void ImGuiController::Release(ImGuiBase* pImgui)
{
}

void ImGuiController::ReleaseAll()
{
	auto it = m_pImGuiList.begin();
	while (it != m_pImGuiList.end())
	{
		delete *it;
		*it = nullptr;
		++it;
	}
}

void ImGuiController::AddImGui(ImGuiBase* pImgui)
{
	m_pImGuiList.push_back(pImgui);
}

void ImGuiController::Contents()
{
	if (!GAME)
		return;
	if (!SCENE)
		return;
	if (!SCENE->GetCurrentScene())
		return;

	ImGui_ImplWin32_NewFrame();
	ImGui_ImplDX11_NewFrame();
	ImGui::NewFrame();

	auto it = m_pImGuiList.begin();
	while (it != m_pImGuiList.end())
	{
		if (*it != nullptr)
		{
			(*it)->Contents();
		}

		++it;
	}
}

void ImGuiController::CreateImGui(IMGUISTATE state)
{
	switch (state)
	{

	}
}