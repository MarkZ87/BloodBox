
#ifndef __BLOOD_HPP__
#define __BLOOD_HPP__

#include <stdint.h>
#include <stdio.h>

#include "cpu.h"
#include "mem.h"
#include "paging.h"
#include "regs.h"

#include "blood_build.hpp"
#include "blood_pragmas.hpp"

#define IDA2LinearCSeg01(X) (0x130010+(X)-0x010000)
#define IDA2LinearDSeg02(X) (0x215fc0+(X)-0x100000)
#define IDA2LinearDSeg03(X) (0x215ff0+(X)-0x110000)
#define IDA2LinearDSeg04(X) (0x216060+(X)-0x120000)

#define CSeg01Ptr(X) (&MemBase[IDA2LinearCSeg01(X)])
#define DSeg02Ptr(X) (&MemBase[IDA2LinearDSeg02(X)])
#define DSeg03Ptr(X) (&MemBase[IDA2LinearDSeg03(X)])
#define DSeg04Ptr(X) (&MemBase[IDA2LinearDSeg04(X)])

#define Ptr2Offset(X) ((uint32_t)((intptr_t)(X) - (intptr_t)(MemBase)))

#define Blood_Min(A, B) (((A) < (B)) ? (A) : (B))
#define Blood_Max(A, B) (((A) > (B)) ? (A) : (B))

enum
{
    BloodInt_Init,
    BloodInt_Return,
    BloodInt_SetView,
    BloodInt_SetAspect,
    BloodInt_DrawRooms,
    BloodInt_ScanSector,
    BloodInt_DrawWalls,
    BloodInt_GetZsOfSlope,
    BloodInt_Inside,
    BloodInt_UpdateSector,
};

int32_t BloodBox_Startup(void);

void Blood_Call(uint32_t Address);
void Blood_DoInterrupt(void);
void Blood_Interrupt(void);

#endif /* __BLOOD_HPP__ */