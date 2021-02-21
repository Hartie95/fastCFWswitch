#define TESLA_INIT_IMPL // If you have more than one file using the tesla header, only define this in the main one
#include <tesla.hpp>    // The Tesla Header
#include "payloadHandler.h"
#include "payload.h"
#include "section.h"
#include "configParser.h"

class FastCFWSwitchBaseGui : public tsl::Gui {
protected:    
    tsl::elm::CustomDrawer* getErrorDrawer(std::string message1){
        return new tsl::elm::CustomDrawer([message1](tsl::gfx::Renderer *renderer, s32 x, s32 y, s32 w, s32 h) {
            renderer->drawString(message1.c_str(), false, x + 3, y + 15, 20, renderer->a(0xF22F));
        });
    }
};

class FastCFWSwitchGui : public FastCFWSwitchBaseGui {
public:
    FastCFWSwitchGui() { }

    // Called when this Gui gets loaded to create the UI
    // Allocate all elements on the heap. libtesla will make sure to clean them up when not needed anymore
    virtual tsl::elm::Element* createUI() override {
        auto frame = new tsl::elm::OverlayFrame(APP_TITLE, APP_VERSION);
        auto list = new tsl::elm::List();

        fastCFWSwitcher::ConfigParser* configParser = new fastCFWSwitcher::ConfigParser(CONFIG_FILE_PATH, list);
        fastCFWSwitcher::PayloadHandler* payloadHandler = new fastCFWSwitcher::PayloadHandler(frame);


        std::list<fastCFWSwitcher::Element*>* payloadList = configParser->getElements();

        if(payloadList!=nullptr){
            for(fastCFWSwitcher::Element* curPayload : *payloadList){
                auto item = curPayload->toListItem(payloadHandler);
                list->addItem(item);
            }
        } else {
            list->addItem(getErrorDrawer("List is emtpy/null\nIs the payload list configured?"), 40);
        }

        // Add the list to the frame for it to be drawn
        frame->setContent(list);
        
        // Return the frame to have it become the top level element of this Gui
        return frame;
    }
};

class FastCFWSwitchErrorGui : public FastCFWSwitchBaseGui {
private:
    std::string errorMessage;
public:
    FastCFWSwitchErrorGui(std::string errorMessage) { 
        this->errorMessage = errorMessage;
    }
    virtual tsl::elm::Element* createUI() override {
        auto frame = new tsl::elm::OverlayFrame(APP_TITLE, APP_VERSION);
        frame->setContent(getErrorDrawer(errorMessage));
        return frame;
    }
};

class FastCFWSwitchOverlay : public tsl::Overlay {
private:
    Result splInitializeResult;
public:
    // libtesla already initialized fs, hid, pl, pmdmnt, hid:sys and set:sys
    virtual void initServices() override {
        splInitializeResult = splInitialize();
    }  // Called at the start to initialize all services necessary for this Overlay
    virtual void exitServices() override {
        splExit();
    }  // Callet at the end to clean up all services previously initialized

    virtual void onShow() override {}    // Called before overlay wants to change from invisible to visible state
    virtual void onHide() override {}    // Called before overlay wants to change from visible to invisible state

    virtual std::unique_ptr<tsl::Gui> loadInitialGui() override {
        if(R_FAILED(splInitializeResult)){
            //unable to init spl, cant reboot this way
            return initially<FastCFWSwitchErrorGui>("Failed to init spl service\nError code: "+std::to_string(splInitializeResult));
        }

        // check if reboot to payload is supported:
        Result rc = 0;
        u64 hardware_type;
        if (R_FAILED(rc = splGetConfig(SplConfigItem_HardwareType, &hardware_type))) {
            return initially<FastCFWSwitchErrorGui>("Failed to get hardware type\nError code: "+std::to_string(rc));
        }
        const bool is_erista = hardware_type == 0 || hardware_type == 1;
        if(!is_erista) {
            // unsupported Switch model, show error
            return initially<FastCFWSwitchErrorGui>("This Switch model is not supported\nReboot to payload is only possible\n\non an Erista Switch");
        } else {
            // create main GUI with payload selection
            return initially<FastCFWSwitchGui>();
        }
    }
};

int main(int argc, char **argv) {
    return tsl::loop<FastCFWSwitchOverlay>(argc, argv);
}
