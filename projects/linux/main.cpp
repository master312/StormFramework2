#include <StormEngine.h>
INITIALIZE_EASYLOGGINGPP

int main(int argc, char* argv[]) {
    /* Initialize easylogging lib */
    el::Configurations c;
    c.setToDefault();
    c.parseFromText("*GLOBAL:\n FORMAT = %level %msg");

    /* Initialize engine, and start main loop */
    StormEngine::instance()->initialize(STORM_PLATFORM_SDL2);
    StormEngine::instance()->startMainLoop();
    StormEngine::instance()->deinitialize();
    
    return 1;
}