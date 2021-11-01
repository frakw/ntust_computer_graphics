// dear imgui: standalone example application for GLFW + OpenGL 3, using programmable pipeline
// If you are new to dear imgui, see examples/README.txt and documentation at the top of imgui.cpp.
// (GLFW is a cross-platform general purpose library for handling windows, inputs, OpenGL/Vulkan graphics context creation, etc.)

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <ctime>

// About OpenGL function loaders: modern OpenGL doesn't have a standard header file and requires individual function pointers to be loaded manually.
// Helper libraries are often used for this purpose! Here we are supporting a few common ones: gl3w, glew, glad.
// You may use another loader/header of your choice (glext, glLoadGen, etc.), or chose to manually implement your own.
#if defined(IMGUI_IMPL_OPENGL_LOADER_GL3W)
#include <GL/gl3w.h>    // Initialize with gl3wInit()
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLEW)
#include <GL/glew.h>    // Initialize with glewInit()
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD)
#include <glad/glad.h>  // Initialize with gladLoadGL()
#else
#include IMGUI_IMPL_OPENGL_LOADER_CUSTOM
#endif

// Include glfw3.h after our OpenGL definitions
#include <GLFW/glfw3.h>

// [Win32] Our example includes a copy of glfw3.lib pre-compiled with VS2010 to maximize ease of testing and compatibility with old VS compilers.
// To link with VS2010-era libraries, VS2015+ requires linking with legacy_stdio_definitions.lib, which we do using this pragma.
// Your own project should not be affected, as you are likely to link with a newer binary of GLFW that is adequate for your version of Visual Studio.
#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif


#define MINIAUDIO_STATIC
#define MINIAUDIO_IMPLEMENTATION
#include <miniaudio.h>
#include "texture.h"
#include "reversi.h"

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}
void data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount)
{
    ma_decoder* pDecoder = (ma_decoder*)pDevice->pUserData;
    if (pDecoder == NULL) {
        return;
    }

    ma_decoder_read_pcm_frames(pDecoder, pOutput, frameCount);

    (void)pInput;
}
void loop_play_data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount)
{
    ma_bool32 isLooping = MA_TRUE;

    ma_decoder* pDecoder = (ma_decoder*)pDevice->pUserData;
    if (pDecoder == NULL) {
        return;
    }

    /*
    A decoder is a data source which means you can seemlessly plug it into the ma_data_source API. We can therefore take advantage
    of the "loop" parameter of ma_data_source_read_pcm_frames() to handle looping for us.
    */
    ma_data_source_read_pcm_frames(pDecoder, pOutput, frameCount, NULL, isLooping);

    (void)pInput;
}


void display()
{
    glColor3b(100,100,100);
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_QUADS);
    glVertex2f(-0.5, -0.5);
    glVertex2f(-0.5, 0.5);
    glVertex2f(0.5, 0.5);
    glVertex2f(0.5, -0.5);
    glEnd();

    glFlush();
}
#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" ) 
int main(int, char**)
{
    srand(time(NULL));
    // Setup window
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;

    // Decide GL+GLSL versions
#if __APPLE__
    // GL 3.2 + GLSL 150
    const char* glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
#else
    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif

    // Create window with graphics context
    GLFWwindow* window = glfwCreateWindow(1280, 720, "REVERSI GAME", NULL, NULL);
    if (window == NULL)
        return 1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync
    glfwSetWindowSizeLimits(window, 1280, 720, GLFW_DONT_CARE, GLFW_DONT_CARE);
    // Initialize OpenGL loader
#if defined(IMGUI_IMPL_OPENGL_LOADER_GL3W)
    bool err = gl3wInit() != 0;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLEW)
    bool err = glewInit() != GLEW_OK;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD)
    bool err = gladLoadGL() == 0;
#else
    bool err = false; // If you use IMGUI_IMPL_OPENGL_LOADER_CUSTOM, your loader is likely to requires some form of initialization.
#endif
    if (err)
    {
        fprintf(stderr, "Failed to initialize OpenGL loader!\n");
        return 1;
    }


    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version); //glsl_version可以使用字符串"#version 150"替代

    bool show_demo_window = true;
    bool show_another_window = false;
    bool settings = false;
    bool game_board = false;
    bool game_lobby = true;
    bool winner_window = false;

    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    ma_result bg_result;
    ma_decoder bg_decoder;
    ma_device_config bg_deviceConfig;
    ma_device bg_device;   
    bg_result = ma_decoder_init_file(u8"audio/The Best Hand.mp3", NULL, &bg_decoder);
    if (bg_result != MA_SUCCESS) {
        return -2;
    }
    bg_deviceConfig = ma_device_config_init(ma_device_type_playback);
    bg_deviceConfig.playback.format = bg_decoder.outputFormat;
    bg_deviceConfig.playback.channels = bg_decoder.outputChannels;
    bg_deviceConfig.sampleRate = bg_decoder.outputSampleRate;
    bg_deviceConfig.dataCallback = loop_play_data_callback;
    bg_deviceConfig.pUserData = &bg_decoder;
    if (ma_device_init(NULL, &bg_deviceConfig, &bg_device) != MA_SUCCESS) {
        printf("Failed to open playback device.\n");
        ma_decoder_uninit(&bg_decoder);
        return -3;
    }
    bg_device.masterVolumeFactor = 0.5f;
    if (ma_device_start(&bg_device) != MA_SUCCESS) {
        printf("Failed to start playback device.\n");
        ma_device_uninit(&bg_device);
        ma_decoder_uninit(&bg_decoder);
        return -4;
    }


    ma_result fall_result;
    ma_decoder fall_decoder;
    ma_device_config fall_deviceConfig;
    ma_device fall_device;
    fall_result = ma_decoder_init_file(u8"audio/chess_fall.mp3", NULL, &fall_decoder);    
    if (fall_result != MA_SUCCESS) {
        return -2;
    }
    fall_deviceConfig = ma_device_config_init(ma_device_type_playback);
    fall_deviceConfig.playback.format = fall_decoder.outputFormat;
    fall_deviceConfig.playback.channels = fall_decoder.outputChannels;
    fall_deviceConfig.sampleRate = fall_decoder.outputSampleRate;
    fall_deviceConfig.dataCallback = loop_play_data_callback;
    fall_deviceConfig.pUserData = &fall_decoder;
    if (ma_device_init(NULL, &fall_deviceConfig, &fall_device) != MA_SUCCESS) {
        printf("Failed to open playback device.\n");
        ma_decoder_uninit(&fall_decoder);
        return -3;
    }
    fall_device.masterVolumeFactor = 0.5f;
    clock_t fall_time = -1;
    

    ImFont* font = io.Fonts->AddFontFromFileTTF("font/NotoSansHans-Regular.ttf", 15.0f, NULL, io.Fonts->GetGlyphRangesChineseFull());
    io.Fonts->GetGlyphRangesChineseSimplifiedCommon();
    Reversi reversi;
    reversi.set_chess_texture("image/Reversi_Black.png","image/Reversi_White.png","image/empty.png");
    Reversi_Color* winner = nullptr;
    
    std::vector<std::pair<int, int> > all_flip;
    int flip_index = 0;
    unsigned int flip_id[8];
    for (int i = 0;i < 8;i++) {
        flip_id[i] = TextureFromFile(string("image/flip/Reversi_flip" + std::to_string(i) + ".png").c_str());
    }
    clock_t last_flip_time = 0;

    unsigned int game_logo = TextureFromFile("image/game_logo.png");
    unsigned int bg_image = TextureFromFile("image/background.png");
    ImGuiStyle& style = ImGui::GetStyle();
    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        //ImGui::GetFrameHeight()

        // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
        //if (show_demo_window)
        //    ImGui::ShowDemoWindow(&show_demo_window);

        int main_window_width, main_window_height;
        glfwGetWindowSize(window, &main_window_width, &main_window_height);

        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
        {
            ImGui::SetNextWindowPos(ImVec2(0, 0), 0, ImVec2(0, 0));
            //设置窗口的大小
            ImGui::SetNextWindowSize(ImVec2(io.DisplaySize.x, io.DisplaySize.y));
            //设置窗口为透明
            ImGui::SetNextWindowBgAlpha(0);
            //设置窗口的padding为0是图片控件充满窗口
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
            //设置窗口为无边框
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
            //创建窗口使其固定在一个位置
            ImGui::Begin(u8"背景", NULL, ImGuiWindowFlags_NoMove |
                ImGuiWindowFlags_NoTitleBar |
                ImGuiWindowFlags_NoBringToFrontOnFocus |
                //ImGuiWindowFlags_NoInputs |
                ImGuiWindowFlags_NoCollapse |
                ImGuiWindowFlags_NoResize |
                ImGuiWindowFlags_NoScrollbar);
            ImGui::Image(ImTextureID(bg_image), ImGui::GetContentRegionAvail());
            ImGui::SameLine((ImGui::GetWindowWidth() / 2) - (189 / 2));//set image in middle position 
            //ImGui::NewLine();
            ImGui::Image(ImTextureID(game_logo), ImVec2(189, 127));

            ImGui::End();
            ImGui::PopStyleVar(2);


            
            ImGui::SetNextWindowPos(ImVec2(main_window_width/2 - 189 / 2, 150));
            ImGui::SetNextWindowSize(ImVec2(189, main_window_height));
            ImGui::Begin(u8"Games Lobby遊戲大廳", &game_lobby,ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar);
            
            ImGui::SameLine((ImGui::GetWindowWidth() / 2) - (160 / 2));//set image in middle position
            if (game_board) {
                if (ImGui::Button(u8"重新開始", ImVec2(160, 90))) {
                    reversi.reset();
                    winner_window = false;
                }
            }
            else if (ImGui::Button(u8"單機雙人", ImVec2(160, 90))) {
                game_board = true;
            }
            ImGui::NewLine();
            ImGui::SameLine((ImGui::GetWindowWidth() / 2) - (160 / 2));//set image in middle position

 /*           if (!reversi.ai_open) {
                if (ImGui::Button(u8"開啟AI", ImVec2(160, 90))) {
                    reversi.reset();
                    winner_window = false;
                    reversi.ai_open = true;
                }

            }
            else if (ImGui::Button(u8"關閉AI", ImVec2(160, 90))) {
                reversi.ai_open = false;
            }*/
            ImGui::NewLine();
            ImGui::SameLine((ImGui::GetWindowWidth() / 2) - (160 / 2));//set image in middle position

            //ImGui::Button(u8"網路對戰", ImVec2(160, 90));
            //ImGui::NewLine();
            //ImGui::SameLine((ImGui::GetWindowWidth() / 2) - (160 / 2));//set image in middle position

            if (ImGui::Button("settings", ImVec2(160, 90))) {
                settings = true;
            }
            ImGui::NewLine();
            ImGui::SameLine((ImGui::GetWindowWidth() / 2) - (160 / 2));//set image in middle position

            if (ImGui::Button(u8"離開遊戲", ImVec2(160, 90))) {
                break;
            }
            //ImGui::NewLine();
            //ImGui::SameLine((ImGui::GetWindowWidth() / 2) - (160 / 2));//set image in middle position

            if (winner_window) {
                if (!winner) {
                    ImGui::Text(u8"平手", ImVec2(160, 90));
                    ImGui::NewLine();
                    ImGui::SameLine((ImGui::GetWindowWidth() / 2) - (180 / 2));//set image in middle position
                    ImGui::Image(ImTextureID(reversi.tex_black_id), ImVec2(90, 90));
                    ImGui::SameLine();
                    ImGui::Image(ImTextureID(reversi.tex_white_id), ImVec2(90, 90));
                }
                else {

                    if (*winner == Reversi_Color::BLACK) {
                        ImGui::Text(u8"\t黑棋勝", ImVec2(160, 90));
                        ImGui::NewLine();
                        ImGui::SameLine((ImGui::GetWindowWidth() / 2) - (90 / 2));//set image in middle position

                        ImGui::Image(ImTextureID(reversi.tex_black_id), ImVec2(90, 90));
                    }
                    else {
                        ImGui::Text(u8"\t白棋勝", ImVec2(160, 90));
                        ImGui::NewLine();
                        ImGui::SameLine((ImGui::GetWindowWidth() / 2) - (90 / 2));//set image in middle position
                        ImGui::Image(ImTextureID(reversi.tex_white_id), ImVec2(90, 90));
                    }
                }
                ImGui::NewLine();
                ImGui::SameLine((ImGui::GetWindowWidth() / 2) - (160 / 2));//set image in middle position

                //if (ImGui::Button(u8"重新遊玩", ImVec2(160, 90))) {
                //    reversi.reset();
                //    winner_window = false;
                //}
            }

            ImGui::End();
            
            static float  grid_size = 50.0f;
            static int  flip_delay = 10;
            static int  ai_delay = 1000;
            if (settings) {
                ImGui::SetNextWindowPos(ImVec2(0,0));
                ImGui::SetNextWindowSize(ImVec2(main_window_width / 2 - 189 / 2, main_window_height));
                ImGui::Begin("settings", &settings, ImGuiWindowFlags_MenuBar);
                ImGui::SliderFloat(u8"格子大小", &grid_size, 0.0f, 100.0f);
                ImGui::SliderInt(u8"翻轉延遲", &flip_delay, 0, 100);
                ImGui::SliderInt(u8"AI延遲", &ai_delay, 0, 5000);
                ImGui::SliderFloat(u8"背景音量", (float*)&bg_device.masterVolumeFactor, 0.0f, 1.0f);
                ImGui::SliderFloat(u8"下棋音量", (float*)&fall_device.masterVolumeFactor, 0.0f, 1.0f);
                ImGui::ColorEdit4(u8"視窗背景", (float*)&style.Colors[ImGuiCol_WindowBg], ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_None);
                //ImGui::ShowStyleEditor();
                ImGui::End();
            }

            if (game_board) {
                ImGui::SetNextWindowPos(ImVec2(main_window_width / 2 + 189 / 2, 0));
                ImGui::SetNextWindowSize(ImVec2(main_window_width / 2 - 189 / 2, main_window_height));

                ImGui::Begin("Game Board", &game_board, ImGuiWindowFlags_MenuBar);
                if (ImGui::BeginMenuBar())
                {
                    if (ImGui::BeginMenu("File"))
                    {
                        if (ImGui::MenuItem("Open..", "Ctrl+O")) { /* Do stuff */ }
                        if (ImGui::MenuItem("Save", "Ctrl+S")) { /* Do stuff */ }
                        if (ImGui::MenuItem("Close", "Ctrl+W")) { game_board = false; }
                        ImGui::EndMenu();
                    }
                    ImGui::EndMenuBar();
                }

                //grid_size = (main_window_width / 2 - 189 / 2) / 10.5f;
                //grid_size = ImGui::GetWindowWidth() / 8.0f;

                //static float my_color;
                //// Edit a color (stored as ~4 floats)
                //ImGui::ColorEdit4("Color", &my_color);

                //// Plot some values
                //const float my_values[] = { 0.2f, 0.1f, 1.0f, 0.5f, 0.9f, 2.2f };
                //ImGui::PlotLines("Frame Times", my_values, IM_ARRAYSIZE(my_values));

                //// Display contents in a scrolling region
                //ImGui::TextColored(ImVec4(1, 1, 0, 1), "Important Stuff");
                //ImGui::BeginChild("Scrolling");
                //for (int n = 0; n < 50; n++)
                //    ImGui::Text("%04d: Some text", n);
                //ImGui::EndChild();
                reversi.find_available_grid();
                ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.0f, 0.0f, 0.0f));
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0.0f, 0.0f, 0.0f));
                ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0.0f, 0.0f, 0.0f));
                ImGui::PushStyleColor(ImGuiCol_Text, (ImVec4)ImColor::HSV(0.0f, 0.0f, 1.0f));
                string tmp = u8"黑棋數量:";tmp += std::to_string(reversi.black_amount());
                ImGui::Button(tmp.c_str(), ImVec2(ImGui::GetWindowSize().x / 2 - 8, 30));ImGui::SameLine();
                ImGui::PopStyleColor(4);

                ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.0f, 0.0f, 1.0f));
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0.0f, 0.0f, 1.0f));
                ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0.0f, 0.0f, 1.0f));
                ImGui::PushStyleColor(ImGuiCol_Text, (ImVec4)ImColor::HSV(0.0f, 0.0f, 0.0f));
                       tmp = u8"白棋數量:";tmp += std::to_string(reversi.white_amount());
                ImGui::Button(tmp.c_str(), ImVec2(ImGui::GetWindowSize().x / 2 - 8, 30));
                ImGui::PopStyleColor(4);
;
                // ImGui::AlignTextToFramePadding();
                //ImGui::SetWindowFontScale(1.5);

                ImGui::SetWindowFontScale(1);
                
                if (ImGui::Button("pass",ImVec2(ImGui::GetWindowSize().x / 2 - 8,30)) && !winner_window) {
                    reversi.next_round();
                }
                ImGui::SameLine();
                if (ImGui::Button(u8"悔棋",ImVec2(ImGui::GetWindowSize().x / 2 - 8, 30))) {
                    
                }

                ImGui::NewLine();
                ImGui::SameLine(ImGui::GetWindowSize().x / 2 - 90 / 2);
                //ImGui::Text(u8"本回合:");

                if (!winner_window) {
                    if (reversi.this_round == Reversi_Color::BLACK) {
                        ImGui::Image(ImTextureID(reversi.tex_black_id), ImVec2(90, 90));
                    }
                    else {
                        ImGui::Image(ImTextureID(reversi.tex_white_id), ImVec2(90, 90));
                    }
                }
                int id = 0;

                for (int i = 0;i < reversi.H();i++) {
                    ImGui::NewLine();
                    for (int j = 0;j < reversi.W();j++) {

                        ImGui::PushID(id++);
                        if (all_flip.size()) {
                            if (std::find(all_flip.begin(), all_flip.end(), make_pair(j, i)) != all_flip.end()) {
                                if (reversi[i][j].is_white()) {//black to white
                                    ImGui::ImageButton(ImTextureID(flip_id[flip_index]), ImVec2(grid_size, grid_size));
                                }
                                else {//white to black
                                    ImGui::ImageButton(ImTextureID(flip_id[7 - flip_index]), ImVec2(grid_size, grid_size));
                                }
                                ImGui::SameLine();
                                ImGui::PopID();
                                continue;
                            }
                        }


                        if (reversi[i][j].is_black()) {
                            ImGui::ImageButton(ImTextureID(reversi.tex_black_id), ImVec2(grid_size, grid_size));
                        }
                        else if (reversi[i][j].is_white()) {
                            ImGui::ImageButton(ImTextureID(reversi.tex_white_id), ImVec2(grid_size, grid_size));
                        }
                        else if (reversi[i][j].available()) {
                            if (ImGui::ImageButton(ImTextureID(reversi.tex_empty_id), ImVec2(grid_size, grid_size), ImVec2(0, 0), ImVec2(1, 1), -1, ImVec4(0.5, 0.5, 0.5, 50)) && !winner_window ) {
                                all_flip = reversi.add_chess(j, i);
                                ma_device_start(&fall_device);
                                fall_time = clock();
                                last_flip_time = clock();
                            }

                            if (ImGui::IsItemHovered()) {
                                ImGui::SetTooltip("flip count:%d", reversi[i][j].flip_count);
                            }

                        }
                        else {
                            ImGui::ImageButton(ImTextureID(reversi.tex_empty_id), ImVec2(grid_size, grid_size));
                        }
                        ImGui::SameLine();
                        ImGui::PopID();
                    }
                }
                ImGui::End();
                if (fall_time != -1 && clock() - fall_time > 130) {
                    ma_device_stop(&fall_device);
                    fall_time = -1;
                }
                if (all_flip.size()) {
                    if (clock() - last_flip_time > flip_delay) {
                        if (flip_index == 7) {
                            all_flip.clear();
                            flip_index = 0;
                        }
                        else {
                            ++flip_index;
                            last_flip_time = clock();
                        }
                    }
                }

                if (reversi.game_end() && !winner_window) {
                    winner = reversi.winner();
                    winner_window = true;

                }
            }


        }

        // 3. Show another simple window.
        if (show_another_window)
        {
            ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
            ImGui::Text("Hello from another window!");
            if (ImGui::Button("Close Me"))
                show_another_window = false;
            ImGui::End();
        }

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwMakeContextCurrent(window);
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        //glRectf(-0.5f, -0.5f, 0.5f, 0.5f);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwMakeContextCurrent(window);
        glfwSwapBuffers(window);
        glUseProgram(0);
        display();

    }
    cout << "go out";
    ma_device_uninit(&bg_device);
    ma_decoder_uninit(&bg_decoder);
    ma_device_uninit(&fall_device);
    ma_decoder_uninit(&fall_decoder);
    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    return 0;
}

