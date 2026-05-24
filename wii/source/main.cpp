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
    
    puts("\nThanks you for contributing to Wii history preservation!\nExiting...");
    videoWait(275);
    return 0;

}
