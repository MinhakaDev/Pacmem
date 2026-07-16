#include "MemoryScanned.h"
#include "imgui.h"
#include "TypeRegistry.h"
#include "ErrorReporter.h"



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
		ImGui::OpenPopup("edit_value");

	    ImGui::TableSetColumnIndex(1);
	    types[ui.selectedType].renderMemoryValue(sc,memoryAddrList[i]);
	}

	if (ImGui::BeginPopup("edit_value"))
	{
	    ImGui::InputText("New Value", editInput, sizeof(editInput));
	    if (ImGui::Button("Write"))
	    {
		try {
			types[ui.selectedType].writeMemory(sc, selectedIndex, editInput);
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
