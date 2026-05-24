#ifndef ERRORS_H
#define ERRORS_H

#include <iostream>
#include <string>
#include <map>
#include <utility>
#include <gccore.h>
#include <wiiuse/wpad.h>
#include "video.h"

typedef struct {
    int code;
    const char* name;
} errorType;

static const errorType errorTable[] = {
    {-1, "UNEXPECTED_MEMORY_ERROR"},
    {-2, "UNEXPECTED_NULL_POINTER"},
    {-101, "AHBPROT_DISABLING_FAILED"},
    {-102, "IOS_PATCHING_FAILED"},
    {-111, "SETTING_OPENING_FAILED"},
    {-112, "SETTING_READING_FAILED"},
    {-113, "SERIAL_EXTRACTING_FAILED"},
    {-121, "TMD_READING_FAILED"},
    {-131, "UID_OPENING_FAILED"},
    {-132, "UID_READING_FAILED"},
    {-200, "NET_INIT_FAILED"},
    {-201, "NET_DHCP_FAILED"},
    {-202, "NET_DNS_FAILED"},
    {-203, "NET_CONNECT_FAILED"},
    {-211, "NET_SEND_FAILED"},
    {-212, "NET_RCV_FAILED"},
    {-213, "NET_SERVER_INTERNAL"},
    {-214, "NET_SERVER_SCRIPT"},
    {-215, "NET_SERVER_API"}
};
static const int errorTableLength = sizeof(errorTable) / sizeof(errorType);

inline const char* getError(int id) {
    for(int i = 0; i < errorTableLength; ++i) {
        if (id == errorTable[i].code) {
            return errorTable[i].name;
        }
    }

    return "UNKNOWN_ERROR";
}

template<typename Func, typename... Args>
inline void executeHandler(Func func, Args&&... args) {
    int ret = func(std::forward<Args>(args)...);
    if (ret < 0) { // failed.
        printf("failed.\nStop code: %s (%d)\n\nPress RESET, HOME or START to exit.", getError(ret), ret);
        while (1) {
            WPAD_ScanPads();
            PAD_ScanPads();

            u32 pressedWii = WPAD_ButtonsDown(0);
            u32 pressedGC = PAD_ButtonsDown(0);

            if ((pressedWii & WPAD_BUTTON_HOME) || (pressedGC & PAD_BUTTON_START) || SYS_ResetButtonDown()) {
                exit(1); 
            }

            VIDEO_WaitVSync();
        }
    }

    puts("success.");
}

#endif
