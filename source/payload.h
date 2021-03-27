#pragma once
#include <tesla.hpp>
#include "element.h"

namespace fastCFWSwitcher {
    class Payload : Element {

        public:
            Payload(std::string name, std::string path) : name(name), path(path){
            }
            ~Payload(){};
            tsl::elm::Element* toListItem(fastCFWSwitcher::PayloadHandler* payloadHandler);
            std::string getPath(){
                return path;
            }

            int getBootPos(){
                return autobootPos;
            }

            std::string getBootId(){
                return autobootID;
            }

            std::string getUmsName(){
                return umsName;
            }

            void setBootPos(int pos){
                this->autobootPos = pos;
            }

            void setBootId(std::string id){
                this->autobootID = id;
            }

            void setUmsName(std::string umsName){
                this->umsName = umsName;
            }

        private:
            std::string name;
            std::string path;
            int autobootPos = -1;
            std::string autobootID;
            std::string umsName;
    };
}
