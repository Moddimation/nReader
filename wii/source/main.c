// includes
#include <stdio.h>
#include <gccore.h>
#include <fat.h>
#include <sdcard/wiisd_io.h>

#include "errors.h"
#include "video.h"
#include "nand.h"
#include "net.h"

const DISC_INTERFACE* sd = &__io_wiisd;
void init () {
    videoInit();

    printf("Version 2.6\n(C) 2026 hediibl, moddimation\nLicensed under the MIT\nRunning on IOS%d v%d\n\n", IOS_GetVersion(), IOS_GetRevision());

    // check for sd:/apps/nReader
    if (!fatInitDefault()) {
        crashImpl("Cannot Init SD Card!");
    } else if (!sd->startup() || !fatMountSimple("sd", sd)) {
        crashImpl("Cannot Mount SD Card!");
    }
}

int main(void) {
    init();

#if 1
        printf("Initializing NAND...");
        EXECUTE_HANDLER(nandInit);
#endif

    videoWait(55);

    printf("Initializing network...");
    EXECUTE_HANDLER(netInit);
    videoWait(55);

    printf("Reading NAND...");
    nandReport report = {};
    EXECUTE_HANDLER(nandRead, &report);
    printf("Found %d entries.\n", report.count);
    videoWait(55);

    printf("Uploading report...");
    EXECUTE_HANDLER(netUpload, &report);
    printf("Your report is available at:\nhttps://nreader.eu/pages/nand.php?nand=%s", report.serial);
    videoWait(55);
    
    puts("\nThanks you for contributing to Wii history preservation!...\n\nPress RESET, HOME or START to exit.");

    fatUnmount(0);

    // wait
    doStop(0);

    return 0;

}
