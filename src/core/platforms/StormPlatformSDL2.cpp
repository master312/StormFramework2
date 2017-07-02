#ifdef STORM_BUILD_PLATFORM_SDL2
#include "StormPlatformSDL2.h"
#include <SDL2/SDL_opengl.h>
#include "../StormCommon.h"

StormPlatformSDL2::StormPlatformSDL2() {
    _Window = nullptr;
    _PlatformType = STORM_PLATFORM_SDL2;
}

StormPlatformSDL2::~StormPlatformSDL2() {
    deinitialize();
}

int StormPlatformSDL2::initialize() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        LOG(FATAL) << "Could not initialize SDL. " << SDL_GetError();
        return -1;
    }
    _IsInitialized = true;
    _InputManager = new StormInputManager();
    LOG(INFO) << "SDL initialized";
    return 1;
}

void StormPlatformSDL2::deinitialize() {
    if (!_IsInitialized) {
        return;
    }
    if (_Window) {
        SDL_DestroyWindow(_Window);
        _Window = nullptr;
    }
    SDL_Quit();
    _IsInitialized = false;
    LOG(INFO) << "SDL deinitialized";
}

int StormPlatformSDL2::createWindow(StormWindowSettings settings) {
    _WindowSettings = settings;

    uint32_t flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;
    if (_WindowSettings.isFullscreen) {
        flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
    }

    /* Setup OpenGL version parameters */
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    /* Create actual sdl window */
    _Window = SDL_CreateWindow(_WindowSettings.title.c_str(), 
                            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
                            _WindowSettings.width, _WindowSettings.height, flags);
    if (!_Window) {
        LOG(FATAL) << "Could create SDL window. " << SDL_GetError();
        return -1;
    }

    _InputManager->setRealWindowSize(Vector2((float)_WindowSettings.width,
                                             (float)_WindowSettings.height));

    LOG(INFO) << "SDL Window created";

    /* Create openGL context */
    SDL_GLContext glContext = SDL_GL_CreateContext(_Window);
    if (!glContext) {
        LOG(FATAL) << "Could not create SDL gl context. " << SDL_GetError();
        return -1;
    }

    if (_WindowSettings.useVsync) {
        SDL_GL_SetSwapInterval(1);
    } else {
        SDL_GL_SetSwapInterval(0);
    }

    LOG(INFO) << "SDL gl context created. Version: " << glGetString(GL_VERSION);

    return 1;
}

void StormPlatformSDL2::windowSwapBuffers() {
    SDL_GL_SwapWindow(_Window);
}

void StormPlatformSDL2::processEvents() {
    SDL_Event event;
    _InputManager->begin();
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                _ShouldQuit = true;
                return;
            case SDL_KEYDOWN:
                if (event.key.repeat == 0) {
                    _InputManager->processKeyEvent(true, (StormKey)event.key.keysym.sym);
                }
                break;
            case SDL_KEYUP:
                if (event.key.repeat == 0) {
                    _InputManager->processKeyEvent(false, (StormKey)event.key.keysym.sym);
                }
                break;
            case SDL_MOUSEBUTTONDOWN:
                switch (event.button.button) {
                    case SDL_BUTTON_LEFT:
                        _InputManager->processPointerButton(S_POINTER_LEFT, true);
                        break;
                    case SDL_BUTTON_RIGHT:
                        _InputManager->processPointerButton(S_POINTER_RIGHT, true);
                        break;
                    case SDL_BUTTON_MIDDLE:
                        _InputManager->processPointerButton(S_POINTER_MIDDLE, true);
                        break;
                }
                break;
            case SDL_MOUSEBUTTONUP:
                switch (event.button.button) {
                    case SDL_BUTTON_LEFT:
                        _InputManager->processPointerButton(S_POINTER_LEFT, false);
                        break;
                    case SDL_BUTTON_RIGHT:
                        _InputManager->processPointerButton(S_POINTER_RIGHT, false);
                        break;
                    case SDL_BUTTON_MIDDLE:
                        _InputManager->processPointerButton(S_POINTER_MIDDLE, false);
                        break;
                }
                break;
            case SDL_MOUSEMOTION:
                _InputManager->processPointerMotion(1, event.motion.x, event.motion.y);
                break;
        }
    }
}

uint32_t StormPlatformSDL2::getRunningTime() {
    return (uint32_t)SDL_GetTicks();
}

#endif /* STORM_BUILD_PLATFORM_QT */
