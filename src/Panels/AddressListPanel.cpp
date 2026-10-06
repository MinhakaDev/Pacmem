#include "AddressListPanel.h"
#include "Scanner.h"
#include <UIContext.h>
#include <imgui.h>
#include <print>
#include "imgui_stdlib.h"
#include "TypeRegistry.h"



AdressListPanel::AdressListPanel(UIContext& ui, Scanner& sc): sc(sc), ui(ui)
{
}

void AdressListPanel::draw()
{
	ImGui::Begin("Address List");
	renderTable();
	ImGui::End();
}

void AdressListPanel::renderTable()
{
    int toDelete = -1;
	if (ImGui::BeginTable("addr_list", 6, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_Resizable | ImGuiTableFlags_ScrollY))
	{
		ImGui::TableSetupScrollFreeze(0, 1);
		ImGui::TableSetupColumn("Freeze", ImGuiTableColumnFlags_WidthFixed);
		ImGui::TableSetupColumn("Description");
		ImGui::TableSetupColumn("Type");
		ImGui::TableSetupColumn("Address");
		ImGui::TableSetupColumn("Value");
		ImGui::TableSetupColumn("##delete", ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_NoResize);
		ImGui::TableHeadersRow();
		for(int i = 0; i < ui.adressList.size(); i++)
		{
			AdressEntry& entry = ui.adressList[i];
			ImGui::PushID(i);
			ImGui::TableNextRow();

            ImGui::TableNextColumn();
			if(ImGui::Checkbox("##freeze", &entry.frozen))
            {
                entry.value = types[entry.type].valueToString(sc,entry.memoryAddr);
            }
            if (entry.frozen)
            {
                types[entry.type].writeMemory(sc,entry.memoryAddr,entry.value.c_str());
            }

			ImGui::TableNextColumn();
			ImGui::SetNextItemWidth(-FLT_MIN);
            ImGui::InputText("##desc", &entry.description);

			ImGui::TableNextColumn();
			ImGui::SetNextItemWidth(-FLT_MIN);
			ImGui::Combo("Type", &entry.type, ui.types, IM_ARRAYSIZE(ui.types));

			ImGui::TableNextColumn();
			ImGui::SetNextItemWidth(-FLT_MIN);
			ImGui::Text("0x%lX", entry.memoryAddr);

			ImGui::TableNextColumn();

            ImGui::SetCursorPosY(ImGui::GetCursorPosY() - ImGui::GetStyle().FramePadding.y); 
            std::string valueText = types[entry.type].valueToString(sc, entry.memoryAddr);

            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(ImGui::GetStyle().ItemSpacing.x, ImGui::GetStyle().CellPadding.y * 2.0f));
            bool clicked = ImGui::Selectable((valueText + "###value").c_str(), false, ImGuiSelectableFlags_AllowDoubleClick, ImVec2(0, ImGui::GetFrameHeight()));
            ImGui::PopStyleVar();

            if (clicked && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
            {
                editInput = valueText;          
                ImGui::OpenPopup("edit_value");
            }

            renderPopUp(entry);

			ImGui::TableNextColumn();
		        if (ImGui::SmallButton("X"))
                    toDelete = i;

			ImGui::PopID();
		}
		ImGui::EndTable();
        if (toDelete != -1)
            ui.adressList.erase(ui.adressList.begin() + toDelete);
	}
}

void AdressListPanel::renderPopUp(AdressEntry& entry)
{
    if (ImGui::BeginPopup("edit_value"))
        {
            ImGui::InputText("New Value", &editInput);
            if (ImGui::Button("Write"))
            {
            try {
                types[entry.type].writeMemory(sc, entry.memoryAddr, editInput.c_str());
            } catch (...) {ErrorReporter::warning("Could Not Write to memory");}
            ImGui::CloseCurrentPopup();
            }
            ImGui::SameLine();
            if (ImGui::Button("Cancel")) ImGui::CloseCurrentPopup();
            ImGui::EndPopup();
        }
}




