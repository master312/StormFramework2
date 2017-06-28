#ifdef _EDITING_SUPPORT
#include "StormEngineEditing.h"

StormEngineEditing::StormEngineEditing() {
    _WindowWidth = 0;
    _WindowHeight = 0;
    _ImGuiFontTexture = nullptr;
    _EditModeEnabled = false;
}

StormEngineEditing::~StormEngineEditing() {

}

StormEngineEditing* StormEngineEditing::instance() {
    static StormEngineEditing shared;
    return &shared;
}

bool StormEngineEditing::isEditMode() {
    return instance()->_EditModeEnabled;
}

void StormEngineEditing::initialize() {
    _WindowWidth = StormEngine::instance()->getPlatform()->getWindowSettings().width;
    _WindowHeight = StormEngine::instance()->getPlatform()->getWindowSettings().height;

    initImgui();
}

void StormEngineEditing::updateTick(float deltaTime) {
    if (StormEngine::instance()->getPlatform()->getInputManager()->isKeyPressed(S_KEY_F4)) {
        /* Toggle edit mode using F4 key */
        _EditModeEnabled = !_EditModeEnabled;
    }
    if (!_EditModeEnabled) {
        return;
    }
    _WindowWidth = StormEngine::instance()->getPlatform()->getWindowSettings().width;
    _WindowHeight = StormEngine::instance()->getPlatform()->getWindowSettings().height;

    /* Start imgui frame */
    ImGuiIO& io = ImGui::GetIO();
    handleImguiEvents();
	io.DeltaTime = deltaTime;
	io.DisplaySize = ImVec2(_WindowWidth, _WindowHeight);
    io.FontGlobalScale = 1.2f;
    ImGui::NewFrame();

    ImGui::ShowTestWindow();
}

void StormEngineEditing::renderPre(StormRenderer* renderer) {
    if (!_EditModeEnabled) {
        return;
    }
}

void StormEngineEditing::renderAfter(StormRenderer* renderer) {
    if (!_EditModeEnabled) {
        return;
    }

    /* Render imgui windows */
    renderer->begin(S_RENDER_TRIANGLES);
    renderer->bindTexture(_ImGuiFontTexture.get());
    ImGui::Render();
}

void StormEngineEditing::initImgui() {
    ImGuiIO& io = ImGui::GetIO();

    io.DisplaySize = ImVec2(_WindowWidth, _WindowHeight);

    io.Fonts->AddFontDefault();
    unsigned char* pixels;
    int width, height, bpp;
    io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height, &bpp);

    ImGui::GetStyle().Colors[ImGuiCol_FrameBg] = ImColor(50, 110, 200, 255);
    ImGui::GetStyle().Colors[ImGuiCol_WindowBg] = ImColor(20, 20, 30, 240);

    _ImGuiFontTexture = new StormTexture();
    _ImGuiFontTexture->loadFromBuffer(pixels, width, height, bpp);

    io.RenderDrawListsFn = StormEngineEditing::renderImgui;
}

void StormEngineEditing::handleImguiEvents() {
    ImGuiIO& io = ImGui::GetIO();

    StormInputManager* inputManager = StormEngine::instance()->getPlatform()->getInputManager();

    Vector2 pointer = inputManager->getPointerPosition();
    io.MousePos.x = pointer.x;
    io.MousePos.y = pointer.y;
    io.MouseDown[0] = inputManager->isPointerDown(S_POINTER_LEFT);
    io.MouseDown[1] = inputManager->isPointerDown(S_POINTER_RIGHT);
}

void StormEngineEditing::renderImgui(ImDrawData* drawData) {
    StormVertex triangle[3];
    int triangleCount = 0;

    for (int n = 0; n < drawData->CmdListsCount; n++) {
        const ImDrawList* cmd_list = drawData->CmdLists[n];
        const ImDrawVert* vtx_src = cmd_list->VtxBuffer.Data;

        for (int i = 0; i < cmd_list->IdxBuffer.size(); i++) {
            ImVec4 color = ImGui::ColorConvertU32ToFloat4(vtx_src[cmd_list->IdxBuffer[i]].col);

            triangle[triangleCount].position.x = vtx_src[cmd_list->IdxBuffer[i]].pos.x;
            triangle[triangleCount].position.y = vtx_src[cmd_list->IdxBuffer[i]].pos.y;
            triangle[triangleCount].uv.x = vtx_src[cmd_list->IdxBuffer[i]].uv.x;
            triangle[triangleCount].uv.y = vtx_src[cmd_list->IdxBuffer[i]].uv.y;
            triangle[triangleCount].color.r = (uint8_t)color.x;
            triangle[triangleCount].color.g = (uint8_t)color.y;
            triangle[triangleCount].color.b = (uint8_t)color.z;
            triangle[triangleCount].color.a = (uint8_t)color.w;
            
            triangleCount++;

            if (triangleCount == 3) {
                /* We have a triangle */
                triangleCount = 0;
                StormEngine::instance()->getRenderer()->bindVertexData(triangle, 3);
                StormEngine::instance()->getRenderer()->draw();
                
            }
        }
    }
}

#endif /* _EDITING_SUPPORT */