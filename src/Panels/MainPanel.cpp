#include "MainPanel.h"
#include "TypeRegistry.h"
#include "ErrorReporter.h"
#include "imgui.h"

MainPanel::MainPanel(UIContext& ui, Scanner& sc) 
	:ui(ui),sc(sc)
{

}

void MainPanel::renderToolbar()
{
	const char* types[] = { "int32", "int64", "float", "uintptr_t" };
	ImGui::Combo("Type", &ui.selectedType, types, IM_ARRAYSIZE(types));

}

void MainPanel::renderScanCombo()
{
	const char* types[] = { "Exact", "Lower", "Greater", "Same" };
}


void MainPanel::draw()
{


	ImGui::Begin("Pacmem");
	renderToolbar();
	ImGui::Separator();
	ImGui::Separator();
	//deletar dps
	ImGui::InputText("Value", searchInput, sizeof(searchInput));
	if (ImGui::Button("Scan") && searchInput[0] != '\0') {
		try
		{
			types[ui.selectedType].scanExact(sc,searchInput);
		} catch (...) {
			ErrorReporter::warning("Could not scan Memory");
		}
	}
	ImGui::SameLine();
	if (ImGui::Button("ReScan") && searchInput[0] != '\0') {
		try
		{
			std::println("button rescan clicked");
			switch (selectedType) {
				case 0: sc.rescanExact<int32_t>(std::stoi(searchInput)); break;
				case 1: sc.rescanExact<int64_t>(std::stoll(searchInput)); break;
				case 2: sc.rescanExact<float>(std::stof(searchInput)); break;
				case 3: sc.rescanExact<uintptr_t>(std::stoull(searchInput, nullptr, 16)); break;
			}
		} catch (...)
		{
			// invalid input, do nothing
		}
	}
	if (ImGui::Button("Unknow") && searchInput[0] != '\0') {
		try
		{
			types[ui.selectedType].scanUnknown(sc);
		} catch (...)
		{
			ErrorReporter::warning("Could not rescan");
		}
	}
	if (ImGui::Button("Lower") && searchInput[0] != '\0') {
		try
		{
			std::println("button rescan clicked");
			types[ui.selectedType].rescanLower(sc);
		} catch (...)
		{
			ErrorReporter::warning("Could not rescan Lower");
		}
	}
	if (ImGui::Button("Higher") && searchInput[0] != '\0') {
		try
		{
			types[ui.selectedType].rescanGreater(sc);
		} catch (...)
		{
			ErrorReporter::warning("Could not rescan Greater");
		}
	}
	if (ImGui::Button("Same") && searchInput[0] != '\0') {
		try
		{
			std::println("Same");
			switch (selectedType) {
				case 0: sc.rescanSame<int32_t>(); break;
				case 1: sc.rescanSame<int64_t>(); break;
				case 2: sc.rescanSame<float>(); break;
			}
		} catch (...) 
		{
			// invalid input, do nothing
		}
	}

	if (ImGui::Button("Changed") && searchInput[0] != '\0') {
		try
		{
			switch (selectedType) {
				case 0: sc.rescanChanged<int32_t>(); break;
				case 1: sc.rescanChanged<int64_t>(); break;
				case 2: sc.rescanChanged<float>(); break;
			}
		} catch (...) 
		{
			// invalid input, do nothing
		}
	}
	ImGui::End();
}
