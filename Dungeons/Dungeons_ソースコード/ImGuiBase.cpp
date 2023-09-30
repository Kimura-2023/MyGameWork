#include "ImGuiBase.h"
#include "ImGuiController.h"

ImGuiBase::ImGuiBase()
{
	ImGuiController::AddImGui(this);
}

ImGuiBase::~ImGuiBase()
{

}