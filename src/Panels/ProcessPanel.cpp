#include "ProcessPanel.h"
#include "Scanner.h"
#include "imgui.h"
#include <cstddef>
#include <print>
#include <ErrorReporter.h>
#include "UIContext.h"


ProcessPanel::ProcessPanel(Scanner& sc, UIContext& ui) : sc(sc),ui(ui)
{
}

void ProcessPanel::draw()
{
    ImGui::SetNextWindowSize(ImVec2(500,450), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetWorkCenter(),ImGuiCond_FirstUseEver);

    ImGui::Begin("Select Process", &ui.menu.process, ImGuiWindowFlags_NoDocking);
    std::vector<std::string> names  = sc.getProcessNames();
    if (ImGui::BeginTable("Process List", 1, ImGuiTableFlags_ScrollY, ImVec2(400,300)))
    {
        ImGui::TableSetupScrollFreeze(0, 1);   // header stays visible
        ImGui::TableSetupColumn("Name");
        ImGui::TableHeadersRow();

        std::vector<std::string> names  = sc.getProcessNames();
        for (int i = 0; i < names.size(); i++)
        {
            if (names[i] == "pacmem") continue;
            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::PushID(i);
            
            if (ImGui::Selectable(names[i].c_str(),true, ImGuiSelectableFlags_AllowDoubleClick))
            {
                selectedIndex = i;
                if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
                {
                    sc.processConnect(names[selectedIndex]);
                    ui.screen = Screen::PROCESS;
                }
            }
            ImGui::PopID();
        }
        ImGui::EndTable();
    }
    if (ImGui::Button("Connect"))
    {
        sc.processConnect(names[selectedIndex]);
        ui.screen = Screen::PROCESS;
    }
    ImGui::SameLine(0,20);
    if (ImGui::Button("Update"))
    {
        sc.updateProcessNames();
    }

    ImGui::End();
}
