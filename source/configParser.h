#pragma once
#include <tesla.hpp>
#include "element.h"
#include <list>

#define CONFIG_FILE_PATH "/config/fastCFWSwitch/config.ini"

namespace fastCFWSwitcher{
    class ConfigParser{
        public:
            ConfigParser(const char* filePath, tsl::elm::List* list) : filePath(filePath), list(list){
            };
            ~ConfigParser(){
            }
            std::list<fastCFWSwitcher::Element*>* getElements();
            void setError(std::string errorString);

        private:
            const char* filePath;
            tsl::elm::List* list;
    };
}
