#include "payload.h"
#include "payloadHandler.h"
#include "element.h"

using namespace fastCFWSwitcher;
using namespace tsl::elm;




 tsl::elm::Element* Payload::toListItem(PayloadHandler* payloadHandler){
    auto item = new ListItem(this->name);
    std::function<bool(u64)> onClick = [item, payloadHandler, this](u64 keys) { 
        if(keys & KEY_A){
            item->setText("rebooting");
            payloadHandler->rebootToPayload(this);
            return true; 
        }
        return false;
    };
    item->setClickListener(onClick);
    return item;
}