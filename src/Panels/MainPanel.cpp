#include "MainPanel.h"
#include "TypeRegistry.h"
#include "ErrorReporter.h"
#include "imgui.h"
#include "imgui_stdlib.h"

#include <format>

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

    ImGui::SameLine();
    ImGui::Checkbox("FasScan", &sc.fastScan);

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
			ErrorReporter::warning("Could not rescan");
		}
	}
	if (ImGui::Button("Unknow") && searchInput.size() != 0) {
		try
		{
			types[ui.selectedType].scanUnknown(sc);
		} catch (...)
		{
			ErrorReporter::warning("Could not Unknow");
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
			types[ui.selectedType].rescanSame(sc);
			
		} catch (...) 
		{
			ErrorReporter::warning("Could not rescan Same");
		}
	}

	if (ImGui::Button("Changed") && searchInput.size() != 0) {
		try
		{
			types[ui.selectedType].rescanChanged(sc);
		} catch (...) 
		{
			ErrorReporter::warning("Could not rescan Changed");
		}
	}
   
	ImGui::End();
}
