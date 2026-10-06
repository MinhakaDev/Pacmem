#include "MemoryScanned.h"
#include "imgui.h"
#include "TypeRegistry.h"
#include "ErrorReporter.h"
#include "imgui_stdlib.h"
#include <algorithm>



MemoryScanned::MemoryScanned(UIContext& ui, Scanner& sc) 
	:ui(ui),sc(sc)
{}

void MemoryScanned::draw()
{
	ImGui::Begin("Memory Table");

	std::vector<uintptr_t> memoryAddrList = sc.getMemoryAddrList();

	if (ImGui::BeginTable("results", 2, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg))
	{
	ImGui::TableSetupColumn("Address");
	ImGui::TableSetupColumn("Value");
	ImGui::TableHeadersRow();



	int count = std::min<size_t>(memoryAddrList.size(), 50);
	for (int i = 0; i < count; i++)
	{
	    ImGui::TableNextRow();
	    ImGui::TableSetColumnIndex(0);

	    bool selected = (selectedIndex == i);
	    char label[32];
	    snprintf(label, sizeof(label), "0x%llX", memoryAddrList[i]);

	    if (ImGui::Selectable(label, selected, ImGuiSelectableFlags_SpanAllColumns))
		selectedIndex = i;

	    if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0))
	    {
		ImGui::OpenPopup("edit_value");
		if (!std::ranges::contains(ui.adressList,memoryAddrList[i], &AdressEntry::memoryAddr))
		{
			ui.adressList.push_back(AdressEntry(memoryAddrList[i], ui.selectedType));
		}
	    }

	    ImGui::TableSetColumnIndex(1);
	    types[ui.selectedType].renderMemoryValue(sc,memoryAddrList[i]);
	}

	if (ImGui::BeginPopup("edit_value"))
	{
	    ImGui::InputText("New Value", &editInput);
	    if (ImGui::Button("Write"))
	    {
		try {
			types[ui.selectedType].writeMemory(sc, memoryAddrList[selectedIndex], editInput.c_str());
		} catch (...) {ErrorReporter::warning("Could Not Write to memory");}
		ImGui::CloseCurrentPopup();
	    }
	    ImGui::SameLine();
	    if (ImGui::Button("Cancel")) ImGui::CloseCurrentPopup();
	    ImGui::EndPopup();
	}

	ImGui::EndTable();
	}
	ImGui::End();


}
