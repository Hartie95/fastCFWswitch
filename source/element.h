#pragma once
#include <tesla.hpp>
namespace fastCFWSwitcher{
    class PayloadHandler;

    class Element{
        public:
            virtual tsl::elm::Element* toListItem(fastCFWSwitcher::PayloadHandler* payloadHandler) = 0;
    };
}
