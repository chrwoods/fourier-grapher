#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl2.h"
#include <stdio.h>
#include "fourier.cpp"
#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#endif
#include <GLFW/glfw3.h>

#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

#define TIME_STEP 0.01f

//error handler for GLFW
static void glfw_error_callback(int error, const char* description) {
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

int main(int argc, char* argv[]) {
  //fill data
  vector<array<double, 2>> data;
  for(int i = 0; i < 40; i++){
    array<double, 2> arr;
    arr[0] = i * 2;
    arr[1] = i * 2 + 1;
    data.push_back(arr);
  }
  
  //fill fourier
  vector<FourierElement> fouriers;
  for(int i = 0; i < 40; i++){
    fouriers.push_back(fourier(data, i + 1));
  }
  
    // Setup window
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit()) return -1;
    GLFWwindow* window = glfwCreateWindow(1280, 720, "Fourier Grapher", NULL, NULL);
    if (window == NULL) return -1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls

    // Choose Dear ImGui style
    //ImGui::StyleColorsDark();
    ImGui::StyleColorsClassic();

    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL2_Init();

    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    bool show_demo_window = false;
    ImVec4 clear_color = ImVec4(0.50f, 0.50f, 0.50f, 1.00f); //background color

    // Main loop
    float time = 0;
    while (!glfwWindowShouldClose(window))
    {
      if(time >= TAU) time = 0;
        // Poll and handle events (inputs, window resize, etc.)
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
        glfwPollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL2_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
        if (show_demo_window) ImGui::ShowDemoWindow(&show_demo_window);

        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
        {
	  ImGui::Begin("Fourier Graphing Window"); //Create window

	  static float sz = 36.0f;
	  static ImVec4 ccol = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	  static ImVec4 lcol = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
	  static ImVec4 gcol = ImVec4(0.0f, 1.0f, 1.0f, 1.0f);
	  
	  //ImGui::DragFloat("Size", &sz, 0.2f, 2.0f, 72.0f, "%.0f");

	  ImGui::ColorEdit3("Background Color", (float*)&clear_color); // Edit 3 floats representing a color
	  ImGui::ColorEdit4("Circle Color", &ccol.x);
	  ImGui::ColorEdit4("Line Color", &lcol.x);
	  ImGui::ColorEdit4("Graph Color", &gcol.x);
	  
	  const ImVec2 p = ImGui::GetCursorScreenPos();
	  float x = p.x + 4.0f, y = p.y + 16.0f, spacing = 8.0f;
	  float th = 1.0f;
	  ImGui::GetWindowDrawList()->AddCircle(ImVec2(x + sz*0.5f, y + sz*0.5f), sz*0.5f, ImColor(ccol), 20, th); x += sz + spacing;  // Circle
	  
	  ImGui::Text("Current time: %.3f", time);               // Display some text (you can use a format strings too)

	  ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

	  float offx = 640.0f + p.x; //x offset
	  float offy = 400.0f + p.y; //y offset
	  
	  for(unsigned int i = 0; i < fouriers.size(); i++){
	    FourierElement fe = fouriers[i];
	    float centx = offx + fe.x;
	    float centy = offy + fe.y;
	    float destx = centx + fe.amp * cos(TAU * fe.freq * time + fe.phase);
	    float desty = centy + fe.amp * sin(TAU * fe.freq * time + fe.phase);
	    ImGui::GetWindowDrawList()->AddCircle(ImVec2(centx, centy), fe.amp*0.5f, ImColor(ccol), i + 5, 1.0f); 
	    ImGui::GetWindowDrawList()->AddLine(ImVec2(centx, centy), ImVec2(destx, desty), ImColor(lcol), 1.0f);
	  }
	  
	  ImGui::End();
        }

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);

        // If you are using this code with non-legacy OpenGL header/contexts (which you should not, prefer using imgui_impl_opengl3.cpp!!), 
        // you may need to backup/reset/restore current shader using the commented lines below.
        //GLint last_program; 
        //glGetIntegerv(GL_CURRENT_PROGRAM, &last_program);
        //glUseProgram(0);
        ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
        //glUseProgram(last_program);

        glfwMakeContextCurrent(window);
        glfwSwapBuffers(window);

	//increment time
	time += TIME_STEP;
    }

    // Cleanup
    ImGui_ImplOpenGL2_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
