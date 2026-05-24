#ifndef NAND_H
#define NAND_H

#ifdef __cplusplus
extern "C" {
#endif

#include <gccore.h>

typedef struct {
    char id[32];
    char title[32];
    char ticket[8];
} nandEntry;

typedef struct {
    char serial[17];
    char username[33];
    u32 count;
    nandEntry *entries;
} nandReport;

int nandInit(void);
int nandRead(nandReport *dump);

#ifdef __cplusplus
}
#endif

#endif
