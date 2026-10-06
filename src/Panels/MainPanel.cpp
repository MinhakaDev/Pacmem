#include "MainPanel.h"
#include "TypeRegistry.h"
#include "ErrorReporter.h"
#include "imgui.h"
#include "imgui_stdlib.h"

MainPanel::MainPanel(UIContext& ui, Scanner& sc) 
	:ui(ui),sc(sc)
{

}

void MainPanel::renderToolbar()
{
	ImGui::Combo("Type", &ui.selectedType, ui.types, IM_ARRAYSIZE(ui.types));
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
	ImGui::InputText("Value", &searchInput);
	if (ImGui::Button("Scan") && searchInput.size() != 0) {
		try
		{
			types[ui.selectedType].scanExact(sc,searchInput.c_str());
		} catch (...) {
			ErrorReporter::warning("Could not scan Memory");
		}
	}
	ImGui::SameLine();
	if (ImGui::Button("ReScan") && searchInput.size() != 0) {
		try
		{
			std::println("button rescan clicked");
			types[ui.selectedType].rescanExact(sc,searchInput.c_str());
		} catch (...)
		{
			// invalid input, do nothing
		}
	}
	if (ImGui::Button("Unknow") && searchInput.size() != 0) {
		try
		{
			types[ui.selectedType].scanUnknown(sc);
		} catch (...)
		{
			ErrorReporter::warning("Could not rescan");
		}
	}
	if (ImGui::Button("Lower") && searchInput.size() != 0) {
		try
		{
			std::println("button rescan clicked");
			types[ui.selectedType].rescanLower(sc);
		} catch (...)
		{
			ErrorReporter::warning("Could not rescan Lower");
		}
	}
	if (ImGui::Button("Higher") && searchInput.size() != 0) {
		try
		{
			types[ui.selectedType].rescanGreater(sc);
		} catch (...)
		{
			ErrorReporter::warning("Could not rescan Greater");
		}
	}
	if (ImGui::Button("Same") && searchInput.size() != 0) {
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

	if (ImGui::Button("Changed") && searchInput.size() != 0) {
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
