#define TESLA_INIT_IMPL // If you have more than one file using the tesla header, only define this in the main one
#include <tesla.hpp>    // The Tesla Header
#include "payloadHandler.h"
#include "payload.h"
#include "section.h"
#include "configParser.h"


class GuiTest : public tsl::Gui {
public:
    GuiTest() { }

    // Called when this Gui gets loaded to create the UI
    // Allocate all elements on the heap. libtesla will make sure to clean them up when not needed anymore
    virtual tsl::elm::Element* createUI() override {
        auto frame = new tsl::elm::OverlayFrame("Switch CFW", "v1.1.0");
        auto list = new tsl::elm::List();

        fastCFWSwitcher::ConfigParser* configParser = new fastCFWSwitcher::ConfigParser(CONFIG_FILE_PATH, list);
        fastCFWSwitcher::PayloadHandler* payloadHandler = new fastCFWSwitcher::PayloadHandler(frame);


        std::list<fastCFWSwitcher::Element*>* payloadList = configParser->getElements();

        /*fastCFWSwitcher::Element* payload = (fastCFWSwitcher::Element*) new fastCFWSwitcher::Payload("atmosphere", "/atmosphere/reboot_payload.bin");
        payloadList.push_back(payload);

        payload = (fastCFWSwitcher::Element*) new fastCFWSwitcher::Payload("SXOS", "/sxos/reboot_payload.bin");
        payloadList.push_back(payload);

        payload = (fastCFWSwitcher::Element*) new fastCFWSwitcher::Section("Tools");
        payloadList.push_back(payload);
        payload = (fastCFWSwitcher::Element*) new fastCFWSwitcher::Payload("Hekate", "/bootloader/reboot_payload.bin");
        payloadList.push_back(payload);
*/

        if(payloadList!=nullptr){
            for(fastCFWSwitcher::Element* curPayload : *payloadList){
                auto item = curPayload->toListItem(payloadHandler);
                list->addItem(item);
            }
        } else {
            list->addItem(new tsl::elm::CategoryHeader("list is null"));
        }

        // Add the list to the frame for it to be drawn
        frame->setContent(list);
        
        // Return the frame to have it become the top level element of this Gui
        return frame;
    }

    // Called once every frame to update values
    virtual void update() override {

    }

    // Called once every frame to handle inputs not handled by other UI elements
    virtual bool handleInput(u64 keysDown, u64 keysHeld, touchPosition touchInput, JoystickPosition leftJoyStick, JoystickPosition rightJoyStick) override {
        return false;   // Return true here to singal the inputs have been consumed
    }

};

class FastCFWSwitchOverlay : public tsl::Overlay {
public:
                                             // libtesla already initialized fs, hid, pl, pmdmnt, hid:sys and set:sys
    virtual void initServices() override {}  // Called at the start to initialize all services necessary for this Overlay
    virtual void exitServices() override {}  // Callet at the end to clean up all services previously initialized

    virtual void onShow() override {}    // Called before overlay wants to change from invisible to visible state
    virtual void onHide() override {}    // Called before overlay wants to change from visible to invisible state

    virtual std::unique_ptr<tsl::Gui> loadInitialGui() override {
        return initially<GuiTest>();  // Initial Gui to load. It's possible to pass arguments to it's constructor like this
    }
};

int main(int argc, char **argv) {
    return tsl::loop<FastCFWSwitchOverlay>(argc, argv);
}
