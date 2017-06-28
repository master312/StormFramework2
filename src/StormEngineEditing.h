#pragma once
#ifdef _EDITING_SUPPORT
#include "StormEngine.h"
#include "core/graphics/StormRenderer.h"
#include "core/platforms/StormPlatform.h"
#include "core/platforms/StormInputManager.h"
#include "../libs/imgui/imgui.h"

class StormRenderer;
struct ImDrawData;

class StormEngineEditing {
public:
    virtual ~StormEngineEditing();

    static StormEngineEditing* instance();
    
    /* Returns true if edit mode is currently enabled */
    static bool isEditMode();

    void initialize();
    void updateTick(float deltaTime);
    void renderPre(StormRenderer* renderer);
    void renderAfter(StormRenderer* renderer);

    /* Method called from imgui lib to render it. */
    static void renderImgui(ImDrawData* drawData);
private:
    StormEngineEditing();

    float _WindowWidth;
    float _WindowHeight;

    bool _EditModeEnabled;
    spStormTexture _ImGuiFontTexture;

    void handleImguiEvents();
    void initImgui();
};

#endif /* _EDITING_SUPPORT */