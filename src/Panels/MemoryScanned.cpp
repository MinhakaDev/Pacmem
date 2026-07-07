#include "MemoryScanned.h"
#include "imgui.h"



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
	    switch (selectedType)
	    {
		case 0: ImGui::Text("%d",  sc.getMemoryValue<int32_t>(memoryAddrList[i])); break;
		case 1: ImGui::Text("%lld", sc.getMemoryValue<int64_t>(memoryAddrList[i])); break;
		case 2: ImGui::Text("%f",  sc.getMemoryValue<float>(memoryAddrList[i])); break;
		case 3: ImGui::Text("0x%llX", sc.getMemoryValue<uintptr_t>(memoryAddrList[i])); break;
	    }
	}

	if (ImGui::BeginPopup("edit_value"))
	{
	    ImGui::InputText("New Value", editInput, sizeof(editInput));
	    if (ImGui::Button("Write"))
	    {
		try {
		    switch (selectedType) {
			case 0: sc.write<int32_t>(selectedIndex, std::stoi(editInput)); break;
			case 1: sc.write<int64_t>(selectedIndex, std::stoll(editInput)); break;
			case 2: sc.write<float>(selectedIndex, std::stof(editInput)); break;
			case 3: sc.write<uintptr_t>(selectedIndex, std::stof(editInput)); break;
		    }
		} catch (...) {}
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
