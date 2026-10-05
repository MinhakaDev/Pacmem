#include "AddressListPanel.h"
#include "Scanner.h"
#include <UIContext.h>
#include <imgui.h>
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
			ImGui::Checkbox("##freeze", &entry.frozen);

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
			ImGui::SetNextItemWidth(-FLT_MIN);
			types[entry.type].renderMemoryValue(sc, entry.memoryAddr);

			ImGui::TableNextColumn();
		        if (ImGui::SmallButton("X"));

			ImGui::PopID();
		}
		ImGui::EndTable();
	}
}




