#pragma once
#include <tesla.hpp>
#include "element.h"

namespace fastCFWSwitcher {
    class Section : Element {

        public:
            Section(std::string name) : name(name){
            }
            ~Section(){}
            tsl::elm::ListItem* toListItem(fastCFWSwitcher::PayloadHandler* PayloadHandler);

        private:
            std::string name;
    };
}
