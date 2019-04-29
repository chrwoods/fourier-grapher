#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl2.h"
#include <stdio.h>
#include "fourier.cpp"
#include "read_data.cpp"
#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#endif
#include <GLFW/glfw3.h>
#include <bits/stdc++.h> //for sorting

#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

//error handler for GLFW
static void glfw_error_callback(int error, const char* description) {
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

bool compareFourierElements(FourierElement f1, FourierElement f2){
  return (f1.amp > f2.amp);
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

  //data = get_data_from_file("peace.txt");
  data = get_data_from_file("diag_line.txt");
  
  //fill fourier
  vector<FourierElement> fouriers;
  for(int i = -1 * data.size(); i <= (signed int)data.size(); i++){
    if(i == 0) continue; //skips 0 so we don't have a circle that sits still
    fouriers.push_back(fourier(data, i));
  }

  //sort fourier
  sort(fouriers.begin(), fouriers.end(), compareFourierElements);
  
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
    //ImVec2 prev_point(0, 0);
    vector<ImVec2> points;
    
    while (!glfwWindowShouldClose(window))
    {
      //if(time >= TAU) time = 0;
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

	  static ImVec4 ccol = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	  static ImVec4 lcol = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
	  static ImVec4 gcol = ImVec4(0.0f, 1.0f, 1.0f, 1.0f);
	  static float time_step = 0.0005f;
	  //time_step = TAU / (float)fouriers.size();
	  static int depth = fouriers.size() > 30 ? 30 : fouriers.size();

	  ImGui::DragInt("Depth", &depth, 1.0f, 1, fouriers.size(), "%d");
	  ImGui::DragFloat("Speed", &time_step, 0.0001f, 0.0f, 0.01f, "%.4f");

	  static float scalar = 0.07f; //scales down the grapher
	  ImGui::DragFloat("Scale", &scalar, 0.01f, 0.01f, 1.0f, "%.2f");
	  
	  ImGui::ColorEdit3("Background Color", (float*)&clear_color); // Edit 3 floats representing a color
	  ImGui::ColorEdit4("Circle Color", &ccol.x);
	  ImGui::ColorEdit4("Line Color", &lcol.x);
	  ImGui::ColorEdit4("Graph Color", &gcol.x);
	  if(ImGui::Button("Reset Time")){
	    time = 0;
	    points.clear();
	  }
	  if(ImGui::Button("Clear Graph")){
	    points.clear();
	  }
	  
	  const ImVec2 p = ImGui::GetCursorScreenPos();
	  
	  ImGui::Text("Current time: %.3f", time);               // Display some text (you can use a format strings too)

	  ImGui::Text("Running at %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

	  float centx = 570.0f + p.x; //center x
	  float centy = 200.0f + p.y; //center y
	  float destx;
	  float desty;
	  
	  for(unsigned int i = 0; i < (unsigned int)depth; i++){
	    FourierElement fe = fouriers[i];
	    destx = centx + fe.amp * scalar * cos(TAU * fe.freq * time + fe.phase);
	    desty = centy + fe.amp * scalar * sin(TAU * fe.freq * time + fe.phase);
	    ImGui::GetWindowDrawList()->AddCircle(ImVec2(centx, centy), fe.amp * scalar, ImColor(ccol), 50, 1.0f); 
	    ImGui::GetWindowDrawList()->AddLine(ImVec2(centx, centy), ImVec2(destx, desty), ImColor(lcol), 1.0f);
	    centx = destx;
	    centy = desty;
	  }

	  /*if(prev_point.x != 0 && prev_point.y != 0){
	    ImGui::GetWindowDrawList()->AddLine(prev_point, ImVec2(destx, desty), ImColor(gcol), 1.0f);
	  }
	  prev_point = ImVec2(destx, desty);*/
	  points.push_back(ImVec2(destx, desty));
	  for(unsigned int i = 1; i < points.size(); i++){
	    ImGui::GetWindowDrawList()->AddLine(points[i - 1], points[i], ImColor(gcol), 1.0f);
	  }
	  
	  //increment time
	  time += time_step;
	  if(time > TAU){
	    time -= TAU;
	    points.clear();
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
    }

    // Cleanup
    ImGui_ImplOpenGL2_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
