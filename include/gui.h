#pragma once
void ModernDarkTheme() {
    // THEME REFERENCE:
    // https://github.com/AdamHarris-GamesProgrammer/Dear-ImGui-Themes/blob/master/ModernDarkTheme.txt
    auto &style = ImGui::GetStyle();
    style.ChildRounding = 0;
    style.GrabRounding = 0;
    style.FrameRounding = 2;
    style.PopupRounding = 0;
    style.ScrollbarRounding = 0;
    style.TabRounding = 2;
    style.WindowRounding = 0;
    style.FramePadding = {4, 4};

    style.WindowTitleAlign = {0.0, 0.5};
    style.ColorButtonPosition = ImGuiDir_Left;

    ImVec4 *colors = ImGui::GetStyle().Colors;
    colors[ImGuiCol_Text] = {1.0f, 1.0f, 1.0f, 1.00f};             //
    colors[ImGuiCol_TextDisabled] = {0.25f, 0.25f, 0.25f, 1.00f};  //
    colors[ImGuiCol_WindowBg] = {0.09f, 0.09f, 0.09f, 0.94f};      //
    colors[ImGuiCol_ChildBg] = {0.11f, 0.11f, 0.11f, 1.00f};       //
    colors[ImGuiCol_PopupBg] = {0.11f, 0.11f, 0.11f, 0.94f};       //
    colors[ImGuiCol_Border] = {0.07f, 0.08f, 0.08f, 1.00f};
    colors[ImGuiCol_BorderShadow] = {0.00f, 0.00f, 0.00f, 0.00f};
    colors[ImGuiCol_FrameBg] = {0.35f, 0.35f, 0.35f, 0.54f};  //
    colors[ImGuiCol_FrameBgHovered] = {0.31f, 0.29f, 0.27f, 1.00f};
    colors[ImGuiCol_FrameBgActive] = {0.40f, 0.36f, 0.33f, 0.67f};
    colors[ImGuiCol_TitleBg] = {0.1f, 0.1f, 0.1f, 1.00f};
    colors[ImGuiCol_TitleBgActive] = {0.3f, 0.3f, 0.3f, 1.00f};  //
    colors[ImGuiCol_TitleBgCollapsed] = {0.0f, 0.0f, 0.0f, 0.61f};
    colors[ImGuiCol_MenuBarBg] = {0.18f, 0.18f, 0.18f, 0.94f};  //
    colors[ImGuiCol_ScrollbarBg] = {0.00f, 0.00f, 0.00f, 0.16f};
    colors[ImGuiCol_ScrollbarGrab] = {0.24f, 0.22f, 0.21f, 1.00f};
    colors[ImGuiCol_ScrollbarGrabHovered] = {0.31f, 0.29f, 0.27f, 1.00f};
    colors[ImGuiCol_ScrollbarGrabActive] = {0.40f, 0.36f, 0.33f, 1.00f};
    colors[ImGuiCol_CheckMark] = {0.84f, 0.84f, 0.84f, 1.0f};          //
    colors[ImGuiCol_SliderGrab] = {0.8f, 0.8f, 0.8f, 1.0f};            //
    colors[ImGuiCol_SliderGrabActive] = {0.55f, 0.55f, 0.55f, 1.00f};  //
    colors[ImGuiCol_Button] = {0.55f, 0.55f, 0.55f, 0.40f};            //
    colors[ImGuiCol_ButtonHovered] = {0.15f, 0.15f, 0.15f, 0.62f};     //
    colors[ImGuiCol_ButtonActive] = {0.60f, 0.60f, 0.60f, 1.00f};      //
    colors[ImGuiCol_Header] = {0.84f, 0.36f, 0.05f, 0.0f};             //
    colors[ImGuiCol_HeaderHovered] = {0.25f, 0.25f, 0.25f, 0.80f};     //
    colors[ImGuiCol_HeaderActive] = {0.42f, 0.42f, 0.42f, 1.00f};
    colors[ImGuiCol_Separator] = {0.35f, 0.35f, 0.35f, 0.50f};  //
    colors[ImGuiCol_SeparatorHovered] = {0.31f, 0.29f, 0.27f, 0.78f};
    colors[ImGuiCol_SeparatorActive] = {0.40f, 0.36f, 0.33f, 1.00f};
    colors[ImGuiCol_ResizeGrip] = {1.0f, 1.0f, 1.0f, 0.25f};          //
    colors[ImGuiCol_ResizeGripHovered] = {1.00f, 1.0f, 1.0f, 0.4f};   //
    colors[ImGuiCol_ResizeGripActive] = {1.00f, 1.00f, 1.0f, 0.95f};  //
    colors[ImGuiCol_Tab] = {0.18f, 0.18f, 0.18f, 1.0f};               //
    colors[ImGuiCol_TabHovered] = {0.58f, 0.58f, 0.58f, 0.80f};       //
    colors[ImGuiCol_TabActive] = {0.6f, 0.60f, 0.60f, 1.00f};
    colors[ImGuiCol_TabUnfocused] = {0.07f, 0.10f, 0.15f, 0.97f};
    colors[ImGuiCol_TabUnfocusedActive] = {0.14f, 0.26f, 0.42f, 1.00f};
    colors[ImGuiCol_PlotLines] = {0.66f, 0.60f, 0.52f, 1.00f};
    colors[ImGuiCol_PlotLinesHovered] = {0.98f, 0.29f, 0.20f, 1.00f};
    colors[ImGuiCol_PlotHistogram] = {0.60f, 0.59f, 0.10f, 1.00f};
    colors[ImGuiCol_PlotHistogramHovered] = {0.72f, 0.73f, 0.15f, 1.00f};
    colors[ImGuiCol_TextSelectedBg] = {0.27f, 0.52f, 0.53f, 0.35f};
    colors[ImGuiCol_DragDropTarget] = {0.60f, 0.59f, 0.10f, 0.90f};
    colors[ImGuiCol_NavHighlight] = {0.51f, 0.65f, 0.60f, 1.00f};
    colors[ImGuiCol_NavWindowingHighlight] = {1.00f, 1.00f, 1.00f, 0.70f};
    colors[ImGuiCol_NavWindowingDimBg] = {0.80f, 0.80f, 0.80f, 0.20f};
    colors[ImGuiCol_ModalWindowDimBg] = {0.11f, 0.13f, 0.13f, 0.35f};
}

void setImGuiStyle() { ModernDarkTheme(); }

void initImgui() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    globals::io = ImGui::GetIO();
    (void)globals::io;
    // ImGui::StyleColorsDark();
    setImGuiStyle();
    ImGui_ImplGlfw_InitForOpenGL(globals::window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
}

void beginFrame() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void endFrame() {
    ImGui::End();
    ImGui::EndFrame();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void loadTabs() {
    ImVec4 *colors = ImGui::GetStyle().Colors;
    colors[ImGuiCol_Button] = {1.f, 0.55f, 0.55f,
                               0.40f};  // TODO: make set button col function
    if (ImGui::Button("Interaction",
                      ImVec2(ImGui::GetWindowSize().x * 0.35f, 0.0f))) {
        globals::tab = 0;
    }
    ImGui::SameLine();
    if (ImGui::Button("Boids",
                      ImVec2(ImGui::GetWindowSize().x * 0.35f, 0.0f))) {
        globals::tab = 1;
    }
    colors[ImGuiCol_Button] = {0.55f, 0.55f, 0.55f, 0.40f};
    ImGui::SameLine();
}

void renderGeneralSettings() {
    std::string title = "profile - fps: ";
    title += std::to_string(globals::guiFrameRate) + " - ";
    title += "cpu usg: " + std::to_string(globals::guiCPULoad * 100.f) + "%";
    ImGui::TextColored(ImVec4(.7f, .7f, .7f, 1.f), title.c_str());
    std::string bc =
        "count boids: " +
        std::to_string((float)globals::flock->countBoids / 1000.f) + "k";
    ImGui::TextColored(ImVec4(.7f, .7f, .7f, 1.f), bc.c_str());
    bool tg = 1;
    if (ImGui::CollapsingHeader("General Settings",
                                ImGuiTreeNodeFlags_DefaultOpen)) {
        if (ImGui::Button("Reset")) {
            globals::flock->resetFlockProperties();
        }
        ImGui::SameLine();
        ImGui::Checkbox("Play", &globals::simulationState);
        ImGui::SameLine();
        ImGui::Checkbox("Record", &globals::recordEnabled);
        if (globals::recordEnabled) {
            ImGui::SameLine();
            std::string recstr = "recording in prog (";
            recstr += std::to_string(globals::renderCount);
            recstr += ")";
            ImGui::TextColored(ImVec4(1.f, 0.f, 0.f, 1.f), recstr.c_str());
        }

       
        ImGui::Checkbox("Auto Pan", &globals::autoRotate);
        ImGui::Checkbox("Skybox", &globals::engine->skyboxEnabled);
        // globals::engine->terrainEnabled = false;
        ImGui::SameLine();
        ImGui::Checkbox("Terrain", &globals::engine->terrainEnabled);
        // globals::engine->skyboxEnabled = false;

        ImGui::SameLine();
        ImGui::Checkbox("Obstacles", &globals::flock->obstacleCourseEnabled);
        ImGui::SameLine();
        ImGui::Checkbox("Cage",
                        &globals::flock->cageEnabled);  // todo make cageg row
        ImGui::Checkbox("POV Camera", &globals::engine->povModeEnabled);

        ImGui::SliderFloat3("Range Size", &globals::flock->cage->scale[0],
                            100.f, 2500.f);
        globals::flock->cage->translate = globals::flock->cage->scale * -0.5f;
        globals::flock->cage->updateModel();

        ImGui::SliderFloat("Max Speed", &globals::flock->maxSpeed, 0.f, 10.f);
        ImGui::NewLine();
    }
}

void renderEnvControls() {
    // entire flock control props
    if (ImGui::CollapsingHeader("Enviornment Settings")) {
        ImGui::Separator();
        {
            ImGui::Text("Mouse Settings");
            ImGui::Checkbox("Follow Mouse", &globals::flock->followMouse);
            ImGui::SliderFloat("FM Coeff", &globals::flock->followCoeff, 0.f,
                               .001f, "%.5g");
        }
        ImGui::NewLine();

        ImGui::Separator();
        {
            ImGui::Text("Physics Settings");
            ImGui::SliderFloat3("Wind Dir", &globals::flock->steeringVec[0],
                                -1.f, 1.f);
            ImGui::SliderFloat("Wind Coeff", &globals::flock->steeringCoeff,
                               0.f, .005f, "%.5g");
        }
        ImGui::NewLine();
    }
}

void renderFlockControls() {
    if (ImGui::CollapsingHeader("Flock Settings",
                                ImGuiTreeNodeFlags_DefaultOpen)) {
        // boid rule props
        ImGui::Checkbox("Apply Cohesion",
                        &globals::flock->shouldApplyCohesionRule);
        ImGui::SliderFloat("Cohesion", &globals::flock->cohesionCoeff, 0.f,
                           .001f, "%.5g");
        ImGui::SliderFloat("Cohesion Radius", &globals::flock->cohesionRadius,
                           0.f, globals::flock->cageSize);
        ImGui::NewLine();

        ImGui::Checkbox("Apply Adhesion",
                        &globals::flock->shouldApplyAdhesionRule);
        ImGui::SliderFloat("Adhesion", &globals::flock->adhesionCoeff, 0.f,
                           .001f, "%.5g");
        ImGui::SliderFloat("Adhesion Radius", &globals::flock->adhesionRadius,
                           0.f, globals::flock->cageSize);
        ImGui::NewLine();

        ImGui::Checkbox("Apply Seperation",
                        &globals::flock->shouldApplySepRule);
        ImGui::SliderFloat("Seperation", &globals::flock->sepCoeff, 0.f, .001f,
                           "%.5g");
        ImGui::SliderFloat("Seperation Radius", &globals::flock->sepRadius, 0.f,
                           globals::flock->cageSize);
        if (ImGui::CollapsingHeader("Boid Color Picker")) {
            ImGui::ColorEdit3("", &globals::flock->boidColor[0]);
        }
    }
}

void renderObstacleControls() {
    if (ImGui::CollapsingHeader("Obstacle Settings")) {
        ImGui::SliderFloat("Look Ahead", &globals::flock->maxLookAhead, 0.f,
                           globals::flock->cageSize, "%.5g");
        ImGui::SliderFloat("Avoidance Coeff", &globals::flock->collisionCoeff,
                           0.f, 1.f, "%.5g");
        if (ImGui::BeginCombo(
                "Geom # ##combo",
                globals::selectedObstacle
                    .c_str()))  // The second parameter is the label
                                // previewed before opening the combo.
        {
            for (int n = 0; n < globals::flock->countGeoms; n++) {
                if (ImGui::Selectable(std::to_string(n).c_str()))
                    globals::selectedObstacle = std::to_string(n);
            }
            ImGui::EndCombo();
        }
        std::string obtitle = "pos " + globals::selectedObstacle;
        ImGui::SliderFloat3(
            obtitle.c_str(),
            &globals::flock->geoms[std::stoi(globals::selectedObstacle)]
                 ->translate[0],
            -globals::flock->cageSize / 2.f, globals::flock->cageSize / 2.f);
        globals::flock->geoms[std::stoi(globals::selectedObstacle)]
            ->updateModel();
        obtitle = "scl " + globals::selectedObstacle;
        ImGui::SliderFloat3(
            obtitle.c_str(),
            &globals::flock->geoms[std::stoi(globals::selectedObstacle)]
                 ->scale[0],
            -globals::flock->cageSize / 2.f, globals::flock->cageSize / 2.f);
        globals::flock->geoms[std::stoi(globals::selectedObstacle)]
            ->updateModel();
    }
}

void renderPredatorControls() {
    if (ImGui::CollapsingHeader("Predator Settings")) {
        ImGui::Checkbox("Enable Pred/Prey", &globals::flock->predPreyEnabled);

        ImGui::SliderFloat("Avoid Coeff", &globals::flock->xAvoidYCoeff, 0.f,
                           .1f, "%.5g");
        ImGui::SliderFloat("Avoid Radius", &globals::flock->xAvoidYRadius, 0.f,
                           globals::flock->cageSize);

        ImGui::SliderFloat("Pred Coeff", &globals::flock->xAttackYCoeff, 0.f,
                           5.f);
        ImGui::SliderFloat("Pred Radius", &globals::flock->xAttackYRadius, 0.f,
                           globals::flock->cageSize, "%.5g");
    }
}

void renderImguiMenu() {
    beginFrame();
    {
        ImGui::SetNextWindowSize(
            ImVec2(globals::guiWidth, globals::windowHeight));
        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::Begin("Settings", NULL,
                     ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                         ImGuiWindowFlags_NoCollapse);
        renderGeneralSettings();
        renderEnvControls();
        renderFlockControls();
        renderObstacleControls();
        renderPredatorControls();
    }
    endFrame();
}

void closeImgui() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}
