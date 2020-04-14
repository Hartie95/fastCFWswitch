

#include <sstream>

#include "payload.h"
#include "payloadHandler.h"

using namespace fastCFWSwitcher;

#define IRAM_PAYLOAD_MAX_SIZE 0x2F000
#define IRAM_PAYLOAD_BASE 0x40010000

alignas(0x1000) static u8 g_ff_page[0x1000];
alignas(0x1000) static u8 g_work_page[0x1000];
alignas(0x1000) static  u8 g_reboot_payload[IRAM_PAYLOAD_MAX_SIZE];


void PayloadHandler::rebootToPayload(fastCFWSwitcher::Payload* payload){
    reboot_to_payload(payload->getPath().c_str());
}

void PayloadHandler::do_iram_dram_copy(void *buf, uintptr_t iram_addr, size_t size, int option) {
    memcpy(g_work_page, buf, size);
    
    SecmonArgs args = {0};
    args.X[0] = 0xF0000201;             /* smcAmsIramCopy */
    args.X[1] = (uintptr_t)g_work_page;  /* DRAM Address */
    args.X[2] = iram_addr;              /* IRAM Address */
    args.X[3] = size;                   /* Copy size */
    args.X[4] = option;                 /* 0 = Read, 1 = Write */
    svcCallSecureMonitor(&args);
    
    memcpy(buf, g_work_page, size);
}

void PayloadHandler::copy_to_iram(uintptr_t iram_addr, void *buf, size_t size) {
    do_iram_dram_copy(buf, iram_addr, size, 1);
}
void PayloadHandler::copy_from_iram(void *buf, uintptr_t iram_addr, size_t size) {
    do_iram_dram_copy(buf, iram_addr, size, 0);
}


void PayloadHandler::clear_iram(void) {
    memset(g_ff_page, 0xFF, sizeof(g_ff_page));
    for (size_t i = 0; i < IRAM_PAYLOAD_MAX_SIZE; i += sizeof(g_ff_page)) {
        copy_to_iram(IRAM_PAYLOAD_BASE + i, g_ff_page, sizeof(g_ff_page));
    }
}

void PayloadHandler::setError(std::string errorString){
    this->frame->setSubtitle(errorString);
}

//todo better error handling
void PayloadHandler::reboot_to_payload(const char* payloadPath) {
    smInitialize();
    Result splResult = splInitialize();
    smExit();
    if (R_FAILED(splResult)){
        std::stringstream ss;
        ss<< std::hex << splResult; // int decimal_value
        std::string res ( ss.str() );
        setError("splInitFailed: "+res);
        return ;
    }

    FsFileSystem fsSdmc;
    FsFile fileConfig;

    if(R_FAILED(fsOpenSdCardFileSystem(&fsSdmc))){
        setError("open sd failed\n");
        splExit();
        return ;
    }

    /* Open config file. */
    if (R_FAILED(fsFsOpenFile(&fsSdmc, payloadPath, FsOpenMode_Read, &fileConfig))){
        setError("open file failed\n");
        fsFsClose(&fsSdmc);
        splExit();
        return ;
    }


    /* Get config file size. */
    s64 configFileSize;
    if (R_FAILED(fsFileGetSize(&fileConfig, &configFileSize))){
        setError("get file size failed\n");
        fsFileClose(&fileConfig);
        fsFsClose(&fsSdmc);
        splExit();
        return ;
    }

    if(configFileSize>sizeof(g_reboot_payload)){
        setError("to big\n");
        fsFileClose(&fileConfig);
        fsFsClose(&fsSdmc);
        splExit();
        return;
    }

    memset(g_reboot_payload, 0xFF, sizeof(g_reboot_payload));
    u64 readSize;
    Result rc = fsFileRead(&fileConfig, 0, g_reboot_payload, configFileSize, FsReadOption_None, &readSize);
    if (R_FAILED(rc) || readSize != static_cast<u64>(configFileSize)){
        setError("read file failed\n");
        fsFileClose(&fileConfig);
        fsFsClose(&fsSdmc);
        splExit();
        return ;
    }

    fsFileClose(&fileConfig);
    fsFsClose(&fsSdmc);
    clear_iram();
    
    for (size_t i = 0; i < IRAM_PAYLOAD_MAX_SIZE; i += 0x1000) {
        copy_to_iram(IRAM_PAYLOAD_BASE + i, &g_reboot_payload[i], 0x1000);
    }

    //check for corectness
    int result=1;
    for (size_t i = 0; i < IRAM_PAYLOAD_MAX_SIZE; i += 0x1000) {
        copy_from_iram(&g_ff_page, IRAM_PAYLOAD_BASE + i, 0x1000);
        result = memcmp(&g_ff_page,&g_reboot_payload[i], 0x1000);
        if(result){
            result=i;
            break;
        }
    }
    if(result){
        setError("cmp failed"+std::to_string(result));
    } else{
        splSetConfig((SplConfigItem)65001, 2);
    }

    splExit();
}
