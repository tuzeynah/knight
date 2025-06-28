#pragma once  

#include "java/java.h"
#include "util/logger.hpp"
#include "menu/menu.h"
#include "module/module_manager.hpp"  
#include "../sdk/sdk.h"               
#include "../ext/minhook/minhook.h"
#include "game_data.h"

#include <thread>
#include <unordered_map>
#include "../sdk/StrayCache.h"     

namespace knight {
    struct context {
        static void initialize();
        static void shutdown();

        static inline bool is_running;
    };

}