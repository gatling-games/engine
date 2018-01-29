#include "EditorManager.h"

#include <imgui.h>
#include <imgui\examples\opengl3_example\imgui_impl_glfw_gl3.h>

#include <GLFW\glfw3.h>

#include <filesystem>
namespace fs = std::experimental::filesystem::v1;

EditorManager::EditorManager(GLFWwindow* window, bool setupGLFWCallbacks)
    : glfwWindow_(window),
    mainWindow_()
{
    // First initilise the imgui GUI library
    ImGui_ImplGlfwGL3_Init(window, setupGLFWCallbacks);

    // Adjust the ui scale based on monitor DPI
    // This allows the ui to work properly with hidpi monitors.
    // Adapted from http://www.glfw.org/docs/latest/monitor_guide.html
    int monitorCount;
    GLFWmonitor** monitors = glfwGetMonitors(&monitorCount);
    if (monitorCount > 0)
    {
        const GLFWvidmode* mode = glfwGetVideoMode(monitors[0]);

        int widthMM, heightMM;
        glfwGetMonitorPhysicalSize(monitors[0], &widthMM, &heightMM);
        const float dpi = mode->width / (widthMM / 25.4f);

        // Set to 1.0 scaling at 110DPI (dont go below 1.0)
        // Aka 2.0 scaling on a 15" macbook pro retina
        ImGui::GetIO().FontGlobalScale = (dpi < 110.0f ? 1.0f : dpi / 110.0f);
    }
    
    // Pick a nicer font for the window, if avaliable use segoeui semibold
    const std::string customFontPath = "C:/Windows/Fonts/seguisb.ttf";
    if (fs::exists(customFontPath))
    {
        ImGuiIO& io = ImGui::GetIO();
        io.Fonts->AddFontFromFileTTF(customFontPath.c_str(), 16.0f);
    }

    // Remove ImGui component rounding styles
    ImGui::GetStyle().WindowRounding = 0.0f;
    ImGui::GetStyle().FrameRounding = 0.0f;
    ImGui::GetStyle().ScrollbarRounding = 0.0f;
    ImGui::GetStyle().GrabRounding = 0.0f;

    // Make the ImGui scrollbar wider
    ImGui::GetStyle().ScrollbarSize = 20.0f;

    // ------- WARNING - Auto-generated by IMGUI.
    // ------- WARNING - Do not modify by hand. Use the test window instead.
    ImVec4* colors = ImGui::GetStyle().Colors;
    colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    colors[ImGuiCol_TextDisabled] = ImVec4(0.33f, 0.33f, 0.33f, 1.00f);
    colors[ImGuiCol_WindowBg] = ImVec4(0.02f, 0.02f, 0.02f, 1.00f);
    colors[ImGuiCol_ChildWindowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_PopupBg] = ImVec4(0.00f, 0.00f, 0.01f, 0.90f);
    colors[ImGuiCol_Border] = ImVec4(0.46f, 0.46f, 0.46f, 0.40f);
    colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_FrameBg] = ImVec4(0.16f, 0.39f, 0.75f, 0.30f);
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.79f, 0.61f, 0.61f, 0.40f);
    colors[ImGuiCol_FrameBgActive] = ImVec4(0.79f, 0.39f, 0.39f, 0.45f);
    colors[ImGuiCol_TitleBg] = ImVec4(0.06f, 0.06f, 0.06f, 1.00f);
    colors[ImGuiCol_TitleBgActive] = ImVec4(0.08f, 0.08f, 0.08f, 1.00f);
    colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.03f, 0.03f, 0.03f, 1.00f);
    colors[ImGuiCol_MenuBarBg] = ImVec4(0.06f, 0.06f, 0.06f, 1.00f);
    colors[ImGuiCol_ScrollbarBg] = ImVec4(0.06f, 0.06f, 0.06f, 0.60f);
    colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.40f, 0.40f, 0.40f, 0.30f);
    colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.19f, 0.19f, 0.19f, 0.40f);
    colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.68f, 0.68f, 0.68f, 0.40f);
    colors[ImGuiCol_ComboBg] = ImVec4(0.03f, 0.03f, 0.03f, 0.99f);
    colors[ImGuiCol_CheckMark] = ImVec4(0.79f, 0.79f, 0.79f, 0.50f);
    colors[ImGuiCol_SliderGrab] = ImVec4(0.53f, 0.53f, 0.53f, 0.30f);
    colors[ImGuiCol_SliderGrabActive] = ImVec4(0.87f, 0.87f, 0.87f, 1.00f);
    colors[ImGuiCol_Button] = ImVec4(0.02f, 0.11f, 0.33f, 1.00f);
    colors[ImGuiCol_ButtonHovered] = ImVec4(0.12f, 0.16f, 0.22f, 1.00f);
    colors[ImGuiCol_ButtonActive] = ImVec4(0.33f, 0.33f, 0.33f, 1.00f);
    colors[ImGuiCol_Header] = ImVec4(0.11f, 0.11f, 0.11f, 0.45f);
    colors[ImGuiCol_HeaderHovered] = ImVec4(0.34f, 0.34f, 0.34f, 0.80f);
    colors[ImGuiCol_HeaderActive] = ImVec4(0.17f, 0.17f, 0.17f, 0.80f);
    colors[ImGuiCol_Separator] = ImVec4(0.22f, 0.22f, 0.22f, 1.00f);
    colors[ImGuiCol_SeparatorHovered] = ImVec4(0.33f, 0.33f, 0.46f, 1.00f);
    colors[ImGuiCol_SeparatorActive] = ImVec4(0.46f, 0.46f, 0.79f, 1.00f);
    colors[ImGuiCol_ResizeGrip] = ImVec4(1.00f, 1.00f, 1.00f, 0.30f);
    colors[ImGuiCol_ResizeGripHovered] = ImVec4(1.00f, 1.00f, 1.00f, 0.60f);
    colors[ImGuiCol_ResizeGripActive] = ImVec4(1.00f, 1.00f, 1.00f, 0.90f);
    colors[ImGuiCol_CloseButton] = ImVec4(0.22f, 0.22f, 0.79f, 0.50f);
    colors[ImGuiCol_CloseButtonHovered] = ImVec4(0.46f, 0.46f, 0.79f, 0.60f);
    colors[ImGuiCol_CloseButtonActive] = ImVec4(0.46f, 0.46f, 0.46f, 1.00f);
    colors[ImGuiCol_PlotLines] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.79f, 0.46f, 0.00f, 1.00f);
    colors[ImGuiCol_PlotHistogram] = ImVec4(0.79f, 0.46f, 0.00f, 1.00f);
    colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.33f, 0.00f, 1.00f);
    colors[ImGuiCol_TextSelectedBg] = ImVec4(0.00f, 0.00f, 1.00f, 0.35f);
    colors[ImGuiCol_ModalWindowDarkening] = ImVec4(0.00f, 0.00f, 0.00f, 0.60f);

    // Set the window size correctly
    int width;
    int height;
    glfwGetWindowSize(window, &width, &height);
    mainWindow_.resize(width, height);
}

EditorManager::~EditorManager()
{
    // Shutdown ImGui
    ImGui_ImplGlfwGL3_Shutdown();
}

void EditorManager::addModuleToDebugPanel(ApplicationModule* module)
{
    mainWindow_.debugPanel()->addModule(module);
}

void EditorManager::resize(int width, int height)
{
    mainWindow_.resize(width, height);
}

void EditorManager::render()
{
    // Set up imgui for the new frame
    ImGui_ImplGlfwGL3_NewFrame();

    // Create the draw lists for the main window.
    mainWindow_.repaint();

    // Switch to rendering directly to the window
    Framebuffer::backbuffer()->use();

    // Ensure the screen is cleared to black so that any 1px 
    // gaps between panels don't stand out.
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
	
    // Now actually render the glfw draw list.
    ImGui::Render();
}