#ifndef ERRORS_H
#define ERRORS_H

#include <string.h>
#include <stdarg.h>
#include <stdlib.h>

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

static inline const char* getError(int id) {
    for(int i = 0; i < errorTableLength; ++i) {
        if (id == errorTable[i].code) {
            return errorTable[i].name;
        }
    }

    return "UNKNOWN_ERROR";
}

static inline void doStop(int err) {
    while (1) {
        WPAD_ScanPads();
        PAD_ScanPads();

        u32 pressedWii = WPAD_ButtonsDown(0);
        u32 pressedGC = PAD_ButtonsDown(0);

        if ((pressedWii & WPAD_BUTTON_HOME) || (pressedGC & PAD_BUTTON_START) || SYS_ResetButtonDown()) {
            exit(err); 
        }

        VIDEO_WaitVSync();
    }
}
static inline void crashImpl(const char* fmt, ...) {
    char msg[512];

    va_list args;
    va_start(args, fmt);
    vsnprintf(msg, sizeof(msg), fmt, args);
    va_end(args);

    printf("failed.\n%s\n\nPress RESET, HOME or START to exit.", msg);
    doStop(1);
}

static inline void crashCode(int ret) {
    crashImpl("Stop code: %s (%d)", getError(ret), ret);
}

static inline void doAssert(int ret) {
    if (ret < 0) { // failed.
        crashCode(ret);
    }
}

#define EXECUTE_HANDLER(func, ...) do { \
    doAssert( func(__VA_ARGS__) ); \
    puts("success."); \
} while (0)

#endif
