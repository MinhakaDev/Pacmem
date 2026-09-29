#include "MenuPanel.h"
#include <imgui.h>
#include <print>

void MenuPanel::draw()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Open"))
			{
				std::println("Open Clicado");
			}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
}
