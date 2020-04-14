#pragma once
#include <tesla.hpp>
#include "payload.h"

namespace fastCFWSwitcher {

    class PayloadHandler{

        public:
            PayloadHandler(tsl::elm::OverlayFrame* frame) : frame(frame){
            }
            ~PayloadHandler(){}
            void rebootToPayload(fastCFWSwitcher::Payload* payload);

        private:
            void setError(std::string errorString);
            void do_iram_dram_copy(void *buf, uintptr_t iram_addr, size_t size, int option);
            void copy_to_iram(uintptr_t iram_addr, void *buf, size_t size);
            void copy_from_iram(void *buf, uintptr_t iram_addr, size_t size) ;
            void clear_iram(void);
            void reboot_to_payload(const char* payloadPath);
            tsl::elm::OverlayFrame* frame;
    };
}