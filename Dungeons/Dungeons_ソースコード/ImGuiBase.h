#pragma once

#include "imGui/imgui_impl_dx11.h"
#include "imGui/imgui_impl_win32.h"

class ImGuiBase
{
public:
	ImGuiBase();
	virtual ~ImGuiBase();
	virtual void Contents() { }
};