#include "MenuPanel.h"
#include "UIContext.h"
#include <imgui.h>
#include <print>


MenuPanel::MenuPanel(UIContext& ui): ui(ui)
{
}


void MenuPanel::draw()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Open"))
			{
                ui.screen = Screen::MultiLevelPointer;
			}
			ImGui::EndMenu();
		}
        if (ImGui::BeginMenu("Process"))
        {
            ui.menu.process = true;
			ImGui::EndMenu();
        }

		ImGui::EndMainMenuBar();
	}
}
