#include "MainPanel.h"
#include "TypeRegistry.h"
#include "ErrorReporter.h"
#include "imgui.h"
#include "imgui_stdlib.h"
#include <sstream>
#include <chrono>
#include <format>

// "121010, 10, 28"  or  "0x121010 0x10 0x28"  →  {0x121010, 0x10, 0x28}
static std::vector<uintptr_t> parseHexList(const std::string& text)
{
    std::string cleaned = text;
    for (char& c : cleaned)
        if (c == ',') c = ' ';

    std::vector<uintptr_t> out;
    std::istringstream ss(cleaned);
    std::string token;
    while (ss >> token)
        out.push_back(std::stoull(token, nullptr, 16));   // base 16 accepts "0x" too
    return out;
}

// {0x121010, 0x10, 0x28}  →  "121010, 10, 28"
static std::string formatHexList(const std::vector<uintptr_t>& values)
{
    std::string s;
    for (size_t i = 0; i < values.size(); i++)
        s += std::format("{}{:X}", i ? ", " : "", values[i]);
    return s;
}

void MainPanel::renderTest()
{
    if (!ImGui::CollapsingHeader("Pointer test"))
        return;

    // ───── 1. Resolve a chain you already know ─────
    ImGui::SeparatorText("Resolve chain");
    ImGui::InputText("Module path", &chainPath);
    ImGui::InputText("Offsets (hex)", &chainOffsets);
    ImGui::TextDisabled("first = offset from module base, e.g.  121010, 10, 28");

    if (ImGui::Button("Resolve + add to list"))
    {
        try
        {
            std::vector<uintptr_t> offsets = parseHexList(chainOffsets);
            std::optional<uintptr_t> addr = mlp.getAdress(chainPath, offsets);

            if (addr)
            {
                ui.adressList.emplace_back(*addr, ui.selectedType);
                ui.adressList.back().description = chainPath + " -> " + formatHexList(offsets);
                resolveStatus = std::format("Resolved to 0x{:X}", *addr);
            }
            else
            {
                resolveStatus = "Chain broken (null pointer or module not found)";
            }
        }
        catch (...)
        {
            resolveStatus = "Invalid offsets";
        }
    }
    if (!resolveStatus.empty())
        ImGui::TextUnformatted(resolveStatus.c_str());

    // ───── 2. Find a chain for an address ─────
    ImGui::SeparatorText("Pointer scan");
    ImGui::InputText("Target address", &scanTarget);
    ImGui::SliderInt("Max depth", &scanDepth, 1, 5);

    if (ImGui::Button("Find pointer"))
    {
        try
        {
            uintptr_t target = std::stoull(scanTarget, nullptr, 16);

            auto t0 = std::chrono::steady_clock::now();
            lastScan = mlp.test(target, scanDepth);
            lastScan = mlp.getMultilevelPointer(target, 0, scanDepth, 0);
            double secs = std::chrono::duration<double>(
                              std::chrono::steady_clock::now() - t0).count();

            scanStatus = lastScan.found
                ? std::format("Found in {:.2f}s", secs)
                : std::format("Nothing found ({:.2f}s), try a bigger depth", secs);
        }
        catch (...)
        {
            scanStatus = "Invalid address";
            lastScan = {};
        }
    }
    if (!scanStatus.empty())
        ImGui::TextUnformatted(scanStatus.c_str());

    if (lastScan.found)
    {
        ImGui::Text("Module:  %s", lastScan.path.c_str());
        ImGui::Text("Offsets: %s", formatHexList(lastScan.offset).c_str());

        if (ImGui::Button("Use in resolver"))
        {
            chainPath    = lastScan.path;
            chainOffsets = formatHexList(lastScan.offset);
        }
    }
}



MainPanel::MainPanel(UIContext& ui, Scanner& sc) 
	:ui(ui),sc(sc),mlp(sc)
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
    renderTest();
	ImGui::End();
}
