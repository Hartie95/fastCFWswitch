#pragma once
#include <tesla.hpp>
#include "element.h"

namespace fastCFWSwitcher {
    class Payload : Element {

        public:
            Payload(std::string name, std::string path) : name(name), path(path){
            }
            ~Payload(){};
            tsl::elm::ListItem* toListItem(fastCFWSwitcher::PayloadHandler* payloadHandler);
            std::string getPath(){
                return path;
            }

        private:
            std::string name;
            std::string path;
    };
}
