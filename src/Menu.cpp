#include "Menu.h"
#include "Panels/MemoryScanned.h"
#include "imgui.h"
#include <cstdint>
#include <cstdlib>
#include <print>
// separate everything into each function



bool Menu::update()
{		
	if(glfwWindowShouldClose(window))
	{
		return false;
	}

	
	glfwPollEvents();

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	ImGuiID dock_id = ImGui::DockSpaceOverViewport();
	ImGui::SetNextWindowDockID(dock_id, ImGuiCond_FirstUseEver);

    menuPanel.draw();
    switch (ui.screen)
    {
        case Screen::MAIN:
            processPanel.draw();
            break;
        case Screen::PROCESS:
            memoryScannedPannel.draw();
            mainPanel.draw();
            break;
        default:
            break;
    }



	ImGui::Render();
	int w, h;
	glfwGetFramebufferSize(window, &w, &h);
	glViewport(0, 0, w, h);
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	glfwSwapBuffers(window);


	return true;
}

Menu::Menu()
{
	if (!glfwInit())
	{
		std::println("error creating glwINIT");
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // get the monitor scale BEFORE creating the window
    float scale = ImGui_ImplGlfw_GetContentScaleForMonitor(glfwGetPrimaryMonitor()) * 2;
	Menu::window = glfwCreateWindow(1080*scale, 1920*scale, "pacmem", nullptr, nullptr);
	if (!window) { glfwTerminate();}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    //scaling
    io.Fonts->AddFontFromFileTTF("fonts/Roboto-Medium.ttf", 16.0f * scale);
    ImGui::GetStyle().ScaleAllSizes(scale);

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

