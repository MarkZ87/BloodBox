
#ifndef __BLOOD_HPP__
#define __BLOOD_HPP__

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "cpu.h"
#include "mem.h"
#include "paging.h"
#include "regs.h"

#include "blood_build.hpp"
#include "blood_db.hpp"
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
#define Blood_KLAbs(X) (abs(X))

enum
{
    /* blood.cpp */
    Blood_ProcAddress_ExitError           = IDA2LinearCSeg01(0xab040),

    /* blood_build.cpp */
    Blood_ProcAddress_SetAspect           = IDA2LinearCSeg01(0x9bd17),
    Blood_ProcAddress_DoSetAspect         = IDA2LinearCSeg01(0x9bd7c),
    Blood_ProcAddress_Inside              = IDA2LinearCSeg01(0x900b4),
    Blood_ProcAddress_UpdateSector        = IDA2LinearCSeg01(0x99224),
    Blood_ProcAddress_GetZsOfSlope        = IDA2LinearCSeg01(0xa1d0a),
    Blood_ProcAddress_ScanSector          = IDA2LinearCSeg01(0x898e7),
    Blood_ProcAddress_BunchFront          = IDA2LinearCSeg01(0x8a2bd),
    Blood_ProcAddress_DrawWalls           = IDA2LinearCSeg01(0x8a357),
    Blood_ProcAddress_WallMost            = IDA2LinearCSeg01(0xa2482),
    Blood_ProcAddress_GrouScan            = IDA2LinearCSeg01(0xa2df9),
    Blood_ProcAddress_CeilScan            = IDA2LinearCSeg01(0x8b8da),
    Blood_ProcAddress_ParaScan            = IDA2LinearCSeg01(0xa3a83),
    Blood_ProcAddress_FlorScan            = IDA2LinearCSeg01(0x8c04b),
    Blood_ProcAddress_PrepWall            = IDA2LinearCSeg01(0x8b564),
    Blood_ProcAddress_WallScan            = IDA2LinearCSeg01(0x8c7de),
    Blood_ProcAddress_AnimateOffs         = IDA2LinearCSeg01(0x41750),
    Blood_ProcAddress_FakeTimerHandler    = IDA2LinearCSeg01(0x7bffc),
    Blood_ProcAddress_LoadTile            = IDA2LinearCSeg01(0x4168c),
    Blood_ProcAddress_GetPalLookup        = IDA2LinearCSeg01(0x4185c),
    Blood_ProcAddress_SetupVLineAsm       = IDA2LinearCSeg01(0xf0b40),
    Blood_ProcAddress_PreVLineAsm1        = IDA2LinearCSeg01(0xf0bd0),
    Blood_ProcAddress_VLineAsm1           = IDA2LinearCSeg01(0xf0bf0),
    Blood_ProcAddress_VLineAsm4           = IDA2LinearCSeg01(0xf0dc0),
    Blood_ProcAddress_SetPalLookupAddress = IDA2LinearCSeg01(0xf08e0),
    Blood_ProcAddress_SetHLineSizes       = IDA2LinearCSeg01(0xf07e0),
    Blood_ProcAddress_HLine               = IDA2LinearCSeg01(0x8e7d0),
    Blood_ProcAddress_HLineAsm4           = IDA2LinearCSeg01(0xf0950),
    Blood_ProcAddress_GetCeilZOfSlope     = IDA2LinearCSeg01(0xa1b4e),
    Blood_ProcAddress_GetFloorZOfSlope    = IDA2LinearCSeg01(0xa1c2c),
};

enum
{
    BloodInt_Init,
    BloodInt_Return,

    /* blood_build.hpp */
    BloodInt_SetAspect,
    BloodInt_DoSetAspect,
    BloodInt_SetView,
    BloodInt_DrawRooms,
    BloodInt_ScanSector,
    BloodInt_GetZsOfSlope,
    BloodInt_Inside,
    BloodInt_UpdateSector,
    BloodInt_DrawWalls,
    BloodInt_WallScan,
    BloodInt_FlorScan,
    BloodInt_CeilScan,
    BloodInt_ParaScan,
    BloodInt_GrouScan,

    /* blood_db.hpp */
    BloodInt_DB_InsertSpriteSect,
    BloodInt_DB_DeleteSpriteSect,
    BloodInt_DB_InsertSpriteStat,
    BloodInt_DB_DeleteSpriteStat,
    BloodInt_DB_InitSprites,
    BloodInt_DB_SpawnSprite,
    BloodInt_DB_DespawnSprite,
};

int32_t BloodBox_Startup(void);

void Blood_Call(uint32_t Address);
void Blood_DoInterrupt(void);
void Blood_Interrupt(void);

void Blood_ExitError(int32_t LineNum, int32_t FileNameString,
                     int32_t ErrorMsgString);


#endif /* __BLOOD_HPP__ */
