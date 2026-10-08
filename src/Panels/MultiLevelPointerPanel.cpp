#include "MultiLevelPointerPanel.h"
#include "UIContext.h"
#include <cstdint>
#include <imgui.h>
#include <imgui_stdlib.h>
#include <sys/types.h>
#include <vector>
#include <MultiLevelPointer.h>
#include "TypeRegistry.h"



MultiLevelPointerPanel::MultiLevelPointerPanel(Scanner& sc, UIContext& ui): sc(sc), mpl(sc), ui(ui)
{
}

void MultiLevelPointerPanel::draw()
{
    ImGui::Begin("Multi Level Pointer");
    ImGui::InputText("Target address", &scanTarget);
    ImGui::SliderInt("Max depth", &scanDepth, 1, 5);
    if (ImGui::Button("scan"))
    {
        std::uintptr_t address = static_cast<std::uintptr_t>(std::stoull(scanTarget, nullptr, 0));
        nodes = mpl.test(address, scanDepth - 1);
    }
    ImGui::SameLine();
    if(ImGui::Button("Add To List"))
    {
        for (PointerNode& pn : nodes)
        {
            std::optional<uintptr_t> result = mpl.getAdress(pn.path, pn.offset);
            if (result)
            {
                uintptr_t memoryAddr = *result;
                AdressEntry entry(memoryAddr,0);
                ui.adressList.push_back(entry);
            }
        }
    }
     ImGui::SameLine();
     if (ImGui::Button("clear"))
    {
        ui.adressList.clear();
    }  
     ImGui::SameLine();
   if (ImGui::Button("sort"))
    {
        // Read every value once, so the sort works on a stable snapshot
        for (AdressEntry& e : ui.adressList)
            e.value = types[e.type].valueToString(sc, e.memoryAddr);

        std::sort(ui.adressList.begin(), ui.adressList.end(),
            [](const AdressEntry& a, const AdressEntry& b)
            {
                return std::stoi(a.value) < std::stoi(b.value);
            });
    }
    renderTable(scanDepth, nodes);
    ImGui::End();
}

void MultiLevelPointerPanel::renderTable(int depth, std::vector<PointerNode>& pointers)
{
	if (ImGui::BeginTable("MultiLevelPointers", depth + 1, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_Resizable | ImGuiTableFlags_ScrollY))
    {
         
		ImGui::TableSetupScrollFreeze(0, 1);
		ImGui::TableSetupColumn("Path");
        for (int i = 0; i < depth; i++)
        {
            std::string label = "offset " + std::to_string(i);
            ImGui::TableSetupColumn(label.c_str());
        }
        ImGui::TableHeadersRow();

        for (int i = 0; i < pointers.size(); i++)
        {
            const PointerNode& pointerNode = pointers[i];
			ImGui::PushID(i);
			ImGui::TableNextRow();

            ImGui::TableNextColumn();
			ImGui::SetNextItemWidth(-FLT_MIN);
			ImGui::TextUnformatted(pointerNode.path.c_str());

            for (int j = 0; j < pointerNode.offset.size(); j++)
            {
                ImGui::TableNextColumn();
                ImGui::SetNextItemWidth(-FLT_MIN);
                ImGui::Text("0x%lX", pointerNode.offset[j]);
            }
            ImGui::PopID();
        }
            ImGui::EndTable();
    }
}
