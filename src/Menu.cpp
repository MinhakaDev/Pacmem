#include "Menu.h"
#include "Panels/MemoryScanned.h"
#include "imgui.h"
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
            break;
        case Screen::PROCESS:
            memoryScannedPannel.draw();
            mainPanel.draw();
	    adressListPanel.draw();
            break;
        default:
            break;
    }

    if (ui.menu.process) processPanel.draw();
    



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

    ImGui::StyleColorsDark();
    style();    
    ImGui::GetStyle().ScaleAllSizes(scale);

    //scaling
    io.Fonts->AddFontFromFileTTF("fonts/Roboto-Medium.ttf", 16.0f * scale);

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");

}

Menu::~Menu()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();
}

void Menu::style()
{
    ImGuiStyle& style = ImGui::GetStyle();
    ImVec4* colors = style.Colors;

    // --- 1. Sizing and Spacing (Soft & Modern) ---
    style.WindowPadding = ImVec2(12.0f, 12.0f);
    style.FramePadding = ImVec2(6.0f, 4.0f);
    style.ItemSpacing = ImVec2(8.0f, 6.0f);
    style.ScrollbarSize = 14.0f;
    style.GrabMinSize = 12.0f;

    // --- 2. Borders & Rounding ---
    style.WindowRounding = 8.0f;
    style.FrameRounding = 5.0f;
    style.PopupRounding = 5.0f;
    style.ScrollbarRounding = 12.0f;
    style.GrabRounding = 5.0f;
    style.TabRounding = 5.0f;

    style.WindowBorderSize = 1.0f;
    style.FrameBorderSize = 0.0f; // Minimalist look
    style.PopupBorderSize = 1.0f;

    // --- 3. The Catppuccin Mocha Palette ---
    // Base: #1e1e2e | Mantle: #181825 | Crust: #11111b
    // Text: #cdd6f4 | Subtext0: #a6adc8 | Surface0: #313244
    // Lavender: #b4befe | Sapphire: #74c7ec | Mauve: #cba6f7

    // Text
    colors[ImGuiCol_Text] = ImVec4(0.80f, 0.84f, 0.96f, 1.00f); // Text
    colors[ImGuiCol_TextDisabled] = ImVec4(0.42f, 0.45f, 0.55f, 1.00f); // Surface1

    // Backgrounds
    colors[ImGuiCol_WindowBg] = ImVec4(0.12f, 0.12f, 0.18f, 1.00f); // Base
    colors[ImGuiCol_ChildBg] = ImVec4(0.09f, 0.09f, 0.15f, 1.00f); // Mantle
    colors[ImGuiCol_PopupBg] = ImVec4(0.07f, 0.07f, 0.11f, 0.96f); // Crust

    // Borders
    colors[ImGuiCol_Border] = ImVec4(0.19f, 0.20f, 0.27f, 1.00f); // Surface0
    colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);

    // Frames (Inputs, etc.)
    colors[ImGuiCol_FrameBg] = ImVec4(0.19f, 0.20f, 0.27f, 1.00f); // Surface0
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.25f, 0.26f, 0.35f, 1.00f); // Surface1
    colors[ImGuiCol_FrameBgActive] = ImVec4(0.31f, 0.32f, 0.42f, 1.00f); // Surface2

    // Title Bars
    colors[ImGuiCol_TitleBg] = ImVec4(0.09f, 0.09f, 0.15f, 1.00f); // Mantle
    colors[ImGuiCol_TitleBgActive] = ImVec4(0.12f, 0.12f, 0.18f, 1.00f); // Base
    colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.07f, 0.07f, 0.11f, 1.00f); // Crust

    // Menus
    colors[ImGuiCol_MenuBarBg] = ImVec4(0.09f, 0.09f, 0.15f, 1.00f);

    // Scrollbars
    colors[ImGuiCol_ScrollbarBg] = ImVec4(0.09f, 0.09f, 0.15f, 1.00f);
    colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.32f, 0.42f, 1.00f); // Surface2
    colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.37f, 0.38f, 0.51f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.42f, 0.45f, 0.55f, 1.00f);

    // Interactables
    colors[ImGuiCol_CheckMark] = ImVec4(0.71f, 0.75f, 1.00f, 1.00f); // Lavender
    colors[ImGuiCol_SliderGrab] = ImVec4(0.45f, 0.78f, 0.93f, 1.00f); // Sapphire
    colors[ImGuiCol_SliderGrabActive] = ImVec4(0.45f, 0.78f, 0.93f, 1.00f);
    colors[ImGuiCol_Button] = ImVec4(0.19f, 0.20f, 0.27f, 1.00f);
    colors[ImGuiCol_ButtonHovered] = ImVec4(0.80f, 0.65f, 0.97f, 1.00f); // Mauve
    colors[ImGuiCol_ButtonActive] = ImVec4(0.70f, 0.55f, 0.87f, 1.00f);
    colors[ImGuiCol_Header] = ImVec4(0.19f, 0.20f, 0.27f, 1.00f);
    colors[ImGuiCol_HeaderHovered] = ImVec4(0.25f, 0.26f, 0.35f, 1.00f);
    colors[ImGuiCol_HeaderActive] = ImVec4(0.31f, 0.32f, 0.42f, 1.00f);

    // Tabs
    colors[ImGuiCol_Tab] = ImVec4(0.12f, 0.12f, 0.18f, 1.00f);
    colors[ImGuiCol_TabHovered] = ImVec4(0.31f, 0.32f, 0.42f, 1.00f);
    colors[ImGuiCol_TabActive] = ImVec4(0.19f, 0.20f, 0.27f, 1.00f);
    colors[ImGuiCol_TabUnfocused] = ImVec4(0.09f, 0.09f, 0.15f, 1.00f);
    colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.12f, 0.12f, 0.18f, 1.00f);

    // Misc
    colors[ImGuiCol_PlotLines] = ImVec4(0.94f, 0.72f, 0.42f, 1.00f); // Marigold
    colors[ImGuiCol_TextSelectedBg] = ImVec4(0.31f, 0.32f, 0.42f, 1.00f);
    colors[ImGuiCol_NavHighlight] = ImVec4(0.71f, 0.75f, 1.00f, 1.00f); // Lavender

#ifdef IMGUI_HAS_DOCK
    colors[ImGuiCol_DockingPreview] = ImVec4(0.71f, 0.75f, 1.00f, 0.50f);
    colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.12f, 0.12f, 0.18f, 1.00f);
#endif
}

