#include "configParser.h"
#include "ini.h"
#include <map>
#include <list>
#include <sstream>

#include "section.h"
#include "payload.h"

using namespace fastCFWSwitcher;

namespace fastCFWSwitcher {
    class ConfigEntry{
        public:
            ConfigEntry(std::string section, int pos) : section(section), pos(pos){
                this->type.assign("payload");
            }
            void setType(const char * type){
                this->type.assign(type);
            }
            void setName(const char * name){
                this->name.assign(name);
            }
            void setPath(const char * path){
                this->path.assign(path);
            }

            Element* toElement(){
                if(type == "section" || path.empty()){
                    return (Element*) new Section(name);
                } else if(type == "payload"){
                    return (Element*) new Payload(name, path);
                }
                return nullptr;
            }

            static bool comparePos(const ConfigEntry* first, const ConfigEntry* second)
            {
                return (first->pos  < second->pos);
            }

            std::string section;
            int pos;
            std::string type;
            std::string name;
            std::string path;
        private:
    };
}

int handler(void *user, const char *section, const char *name, const char *value)

{
    std::map<std::string, fastCFWSwitcher::ConfigEntry*>* elementsMap = (std::map<std::string, fastCFWSwitcher::ConfigEntry*>*) user;

    std::string sectionString(section);
    fastCFWSwitcher::ConfigEntry* configEntry ;
 
    std::map<std::string, fastCFWSwitcher::ConfigEntry*>::iterator  it = elementsMap->find(sectionString);
    if( it != elementsMap->end() ){
        configEntry = it->second;
    } else {
        configEntry = new fastCFWSwitcher::ConfigEntry(sectionString, elementsMap->size());
        elementsMap->insert(std::pair<std::string, fastCFWSwitcher::ConfigEntry*>(sectionString, configEntry));
    }

    if (!strcmp("type", name) ) {
        configEntry->setType(value);
    } else if (!strcmp("name", name) ) {
        configEntry->setName(value);
    } else if (!strcmp("path", name)) {
        configEntry->setPath(value);
    }
    return 1;
}

void ConfigParser::setError(std::string errorString){
    this->list->addItem(new tsl::elm::CategoryHeader(errorString));
}

std::list<Element*>* ConfigParser::getElements(){
    
    // Read settings file

    FsFileSystem fsSdmc;

    if(R_FAILED(fsOpenSdCardFileSystem(&fsSdmc))){
        setError("open sd failed\n");
        return nullptr;
    }

    /* Open config file. */
    FsFile fileConfig;
    if (R_FAILED(fsFsOpenFile(&fsSdmc, this->filePath, FsOpenMode_Read, &fileConfig))){
        setError("open config file failed ");
        fsFsClose(&fsSdmc);
        return nullptr;
    }


    /* Get config file size. */
    s64 configFileSize;
    if (R_FAILED(fsFileGetSize(&fileConfig, &configFileSize))){
        setError("get file size failed\n");
        fsFileClose(&fileConfig);
        fsFsClose(&fsSdmc);
        return nullptr;
    }

    std::vector<char> fileString(configFileSize+1);
    fileString[configFileSize] = 0;

    u64 readSize;
    Result rc = fsFileRead(&fileConfig, 0, &fileString[0], configFileSize, FsReadOption_None, &readSize);
    if (R_FAILED(rc) || readSize != static_cast<u64>(configFileSize)){
        setError("read file failed\n");
        fsFileClose(&fileConfig);
        fsFsClose(&fsSdmc);
        return nullptr;
    }

    fsFileClose(&fileConfig);
    fsFsClose(&fsSdmc);


    //parse ini file
    std::map<std::string, fastCFWSwitcher::ConfigEntry*> elementsMap;

    int config_err = ini_parse_string(&fileString[0], handler, &elementsMap);
    if(config_err<0){
        setError("An error occured.");
        return nullptr;
    } else if(config_err>0){
        std::stringstream ss;
        ss<< std::hex << config_err; // int decimal_value
        std::string res ( ss.str() );
        setError("Bad config file, first error on line "+res);
        return nullptr;
    }
    std::list<fastCFWSwitcher::ConfigEntry*>* elementsList = new std::list<fastCFWSwitcher::ConfigEntry*>();
    for (auto const& x : elementsMap)
    {
        elementsList->push_back(x.second);
    }

    elementsList->sort(fastCFWSwitcher::ConfigEntry::comparePos);

    std::list<fastCFWSwitcher::Element*>* payloadList = new std::list<fastCFWSwitcher::Element*>();

    for (fastCFWSwitcher::ConfigEntry* entry : *elementsList)
    {
        fastCFWSwitcher::Element* element = entry->toElement();
        if(element!=nullptr){
            payloadList->push_back(element);
            //setError(x.second->path);
        } else {
            setError("error "+entry->section);
        }
    }
    return payloadList;
}