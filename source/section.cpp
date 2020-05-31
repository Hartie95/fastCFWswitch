#include "section.h"
#include "element.h"

using namespace fastCFWSwitcher;
using namespace tsl::elm;

 tsl::elm::Element* Section::toListItem(fastCFWSwitcher::PayloadHandler* PayloadHandler){
    return new CategoryHeader(this->name);
}