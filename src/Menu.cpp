#include "Menu.h"
#include "Panels/MemoryScanned.h"
#include "imgui.h"
#include <cstdint>
#include <cstdlib>
#include <print>
// separate everything into each function



void Menu::renderToolbar()
{
	const char* types[] = { "int32", "int64", "float", "uintptr_t" };

	ImGui::Combo("Type", &selectedType, types, IM_ARRAYSIZE(types));

}

bool Menu::update()
{
	
	glfwPollEvents();

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	ImGuiID dock_id = ImGui::DockSpaceOverViewport();
	ImGui::SetNextWindowDockID(dock_id, ImGuiCond_FirstUseEver);

	memoryScannedPannel.draw();

	ImGui::Begin("Pacmem");
	renderToolbar();
	ImGui::Separator();
	ImGui::Separator();
	//deletar dps
	ImGui::InputText("Value", searchInput, sizeof(searchInput));
	if (ImGui::Button("Scan") && searchInput[0] != '\0') {
		try {
			std::println("button scan clicked");
			switch (selectedType) {
				case 0: sc.scanExact<int32_t>(std::stoi(searchInput)); break;
				case 1: sc.scanExact<int64_t>(std::stoll(searchInput)); break;
				case 2: sc.scanExact<float>(std::stof(searchInput)); break;
				case 3: sc.scanExact<uintptr_t>(std::stoull(searchInput, nullptr, 16)); break;

			}
		} catch (...) {
			// invalid input, do nothing
		}
	}
	ImGui::SameLine();
	if (ImGui::Button("ReScan") && searchInput[0] != '\0') {
		try {
			std::println("button rescan clicked");
			switch (selectedType) {
				case 0: sc.rescanExact<int32_t>(std::stoi(searchInput)); break;
				case 1: sc.rescanExact<int64_t>(std::stoll(searchInput)); break;
				case 2: sc.rescanExact<float>(std::stof(searchInput)); break;
				case 3: sc.rescanExact<uintptr_t>(std::stoull(searchInput, nullptr, 16)); break;
			}
		} catch (...) {
			// invalid input, do nothing
		}
	}
	if (ImGui::Button("Unknow") && searchInput[0] != '\0') {
		try {
			std::println("button rescan clicked");
			switch (selectedType) {
				case 0: sc.scanUnknown<int32_t>(); break;
				case 1: sc.scanUnknown<int64_t>(); break;
				case 2: sc.scanUnknown<float>(); break;
			}
		} catch (...) {
			// invalid input, do nothing
		}
	}
	if (ImGui::Button("Lower") && searchInput[0] != '\0') {
		try {
			std::println("button rescan clicked");
			switch (selectedType) {
				case 0: sc.rescanLower<int32_t>(); break;
				case 1: sc.rescanLower<int64_t>(); break;
				case 2: sc.rescanLower<float>(); break;
			}
		} catch (...) {
			// invalid input, do nothing
		}
	}
	if (ImGui::Button("Higher") && searchInput[0] != '\0') {
		try {
			std::println("Higher");
			switch (selectedType) {
				case 0: sc.rescanGreater<int32_t>(); break;
				case 1: sc.rescanGreater<int64_t>(); break;
				case 2: sc.rescanGreater<float>(); break;
			}
		} catch (...) {
			// invalid input, do nothing
		}
	}
	if (ImGui::Button("Same") && searchInput[0] != '\0') {
		try {
			std::println("Same");
			switch (selectedType) {
				case 0: sc.rescanSame<int32_t>(); break;
				case 1: sc.rescanSame<int64_t>(); break;
				case 2: sc.rescanSame<float>(); break;
			}
		} catch (...) {
			// invalid input, do nothing
		}
	}

	if (ImGui::Button("Changed") && searchInput[0] != '\0') {
		try {
			switch (selectedType) {
				case 0: sc.rescanChanged<int32_t>(); break;
				case 1: sc.rescanChanged<int64_t>(); break;
				case 2: sc.rescanChanged<float>(); break;
			}
		} catch (...) {
			// invalid input, do nothing
		}
	}



	ImGui::End();

	ImGui::Render();
	int w, h;
	glfwGetFramebufferSize(window, &w, &h);
	glViewport(0, 0, w, h);
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	glfwSwapBuffers(window);

	if(glfwWindowShouldClose(window))
	{
		return false;
	}
	return true;
}

Menu::Menu()
{
	searchInput[0] = '\0';
	if (!glfwInit())
	{
		std::println("error creating glwINIT");
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	Menu::window = glfwCreateWindow(800, 600, "pacmem", nullptr, nullptr);
	if (!window) { glfwTerminate();}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");
	ImGui::StyleColorsDark();
}

Menu::~Menu()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();
}

