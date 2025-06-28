#include "knight.hpp"

void knight::context::initialize()
{
    MH_Initialize();

    logger.initialize();
    config_manager.initialize();
    Java::Init(); 
    SDK::Init();
     
    Menu::Init();
    module_manager.initialize();

    is_running = true;

    while (is_running)
    {
        module_manager.update_modules();
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }

    shutdown();
}

void knight::context::shutdown()
{
    StrayCache::DeleteRefs();
    Java::Kill();
    Menu::Kill();
    logger.shutdown();

    MH_Uninitialize();
}