#include "ImGuiTest.h"

ImGuiTest::ImGuiTest()
{

}

ImGuiTest::~ImGuiTest()
{

}

void ImGuiTest::Contents()
{
	ImGui::Begin("config 1", nullptr, ImGuiWindowFlags_MenuBar);

	if (ImGui::BeginMenuBar()) {
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Save")) {

			}
			if (ImGui::MenuItem("Load")) {

			}

			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}

	ImGui::End();
}