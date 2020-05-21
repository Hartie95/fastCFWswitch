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

            int getBootPos(){
                return autobootPos;
            }

            std::string getBootId(){
                return autobootID;
            }

            void setBootPos(int pos){
                this->autobootPos = pos;
            }

            void setBootId(std::string id){
                this->autobootID = id;
            }

        private:
            std::string name;
            std::string path;
            int autobootPos;
            std::string autobootID;
    };
}
