// includes
#include <iostream>
#include <gccore.h>
#include "errors.h"
#include "video.h"
#include "nand.h"
#include "net.h"

int main(void) {
    videoInit();
    printf("(C) 2026 hediibl\nLicensed under the MIT\nRunning on IOS%d v%d\n\n", IOS_GetVersion(), IOS_GetRevision());

#if 1
        printf("Initializing NAND...");
        executeHandler(nandInit);
#endif

    videoWait(55);

    printf("Initializing network...");
    executeHandler(netInit);
    videoWait(55);

    printf("Reading NAND...");
    nandReport report = {};
    executeHandler(nandRead, &report);
    printf("Found %d entries.\n", report.count);
    videoWait(55);

    printf("Uploading report...");
    executeHandler(netUpload, &report);
    printf("Your report is available at:\nhttps://nreader.eu/pages/nand.php?nand=%s", report.serial);
    videoWait(55);
    
    puts("\nThanks you for contributing to Wii history preservation!...\n\nPress RESET, HOME or START to exit.");
    // wait
    while (1) {
        WPAD_ScanPads();
        PAD_ScanPads();

        u32 pressedWii = WPAD_ButtonsDown(0);
        u32 pressedGC = PAD_ButtonsDown(0);

        if ((pressedWii & WPAD_BUTTON_HOME) || (pressedGC & PAD_BUTTON_START) || SYS_ResetButtonDown()) {
            break;
        }

        VIDEO_WaitVSync();
    }

    return 0;

}
