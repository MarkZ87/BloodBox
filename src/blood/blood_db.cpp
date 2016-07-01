
#include <memory.h>
#include "blood.hpp"

void Blood_DB_InsertSpriteSect(int16_t SpriteIndex, int16_t SectorIndex)
{
    if (SpriteIndex < 0 || SpriteIndex >= 4096)
    {
        /* "src\\db.cpp", "nSprite >= 0 && nSprite < kMaxSprites" */
        Blood_ExitError(498, IDA2LinearDSeg04(0x1219f8),
                        IDA2LinearDSeg04(0x121a03));
    }

    if (SectorIndex < 0 || SectorIndex >= 1024)
    {
        /* "src\\db.cpp", "nSector >= 0 && nSector < kMaxSectors" */
        Blood_ExitError(499, IDA2LinearDSeg04(0x121a29),
                        IDA2LinearDSeg04(0x121a34));
    }

    int16_t bx = Blood_HeadSpriteSect[SectorIndex];

    if (bx < 0)
    {
        Blood_PrevSpriteSect[SpriteIndex] = SpriteIndex;
        Blood_HeadSpriteSect[SectorIndex] = SpriteIndex;
        Blood_NextSpriteSect[SpriteIndex] = -1;
    }
    else
    {
        int16_t dx = bx;
        bx = Blood_PrevSpriteSect[dx];
        Blood_PrevSpriteSect[SpriteIndex] = bx;
        Blood_NextSpriteSect[SpriteIndex] = -1;
        int16_t ax = Blood_PrevSpriteSect[dx];
        Blood_PrevSpriteSect[dx] = SpriteIndex;
        Blood_NextSpriteSect[ax] = SpriteIndex;
    }

    Blood_Sprites[SpriteIndex].SectNum = SectorIndex;
}

void Blood_DB_DeleteSpriteSect(int16_t SpriteIndex)
{
    if (SpriteIndex < 0 || SpriteIndex >= 4096)
    {
        /* "src\\db.cpp", "nSprite >= 0 && nSprite < kMaxSprites" */
        Blood_ExitError(523, IDA2LinearDSeg04(0x121a5a),
                        IDA2LinearDSeg04(0x121a65));
    }

    int16_t SectorIndex = Blood_Sprites[SpriteIndex].SectNum;

    if (SectorIndex < 0 || SectorIndex >= 1024)
    {
        /* "src\\db.cpp", "nSector >= 0 && nSector < kMaxSectors" */
        Blood_ExitError(526, IDA2LinearDSeg04(0x121a8b),
                        IDA2LinearDSeg04(0x121a96));
    }

    int16_t NextIndex = Blood_NextSpriteSect[SpriteIndex];
    if (NextIndex < 0)
        NextIndex = Blood_HeadSpriteSect[SectorIndex];

    Blood_PrevSpriteSect[NextIndex] = Blood_PrevSpriteSect[SpriteIndex];

    if (Blood_HeadSpriteSect[SectorIndex] != SpriteIndex)
    {
        Blood_NextSpriteSect[Blood_PrevSpriteSect[SpriteIndex]] =
            Blood_NextSpriteSect[SpriteIndex];
    }
    else
    {
        Blood_HeadSpriteSect[SectorIndex] =
            Blood_NextSpriteSect[SpriteIndex];
    }

    Blood_Sprites[SpriteIndex].SectNum = -1;
}

void Blood_DB_InsertSpriteStat(int16_t SpriteIndex, int16_t Stat)
{
    if (SpriteIndex < 0 || SpriteIndex >= 4096)
    {
        /* "src\\db.cpp", "nSprite >= 0 && nSprite < kMaxSprites" */
        Blood_ExitError(544, IDA2LinearDSeg04(0x121abc),
                        IDA2LinearDSeg04(0x121ac7));
    }

    if (Stat < 0 || Stat > 1024)
    {
        /* "src\\db.cpp", "nStat >= 0 && nStat <= kMaxStatus" */
        Blood_ExitError(545, IDA2LinearDSeg04(0x121aed),
                        IDA2LinearDSeg04(0x121af8));
    }

    int16_t bx = Blood_HeadSpriteStat[Stat];

    if (bx < 0)
    {
        Blood_PrevSpriteStat[SpriteIndex] = SpriteIndex;
        Blood_HeadSpriteStat[Stat] = SpriteIndex;
        Blood_NextSpriteStat[SpriteIndex] = -1;
    }
    else
    {
        int16_t dx = bx;
        bx = Blood_PrevSpriteStat[dx];
        Blood_PrevSpriteStat[SpriteIndex] = bx;
        Blood_NextSpriteStat[SpriteIndex] = -1;
        int16_t ax = Blood_PrevSpriteStat[dx];
        Blood_PrevSpriteStat[dx] = SpriteIndex;
        Blood_NextSpriteStat[ax] = SpriteIndex;
    }

    Blood_Sprites[SpriteIndex].StatNum = Stat;
    Blood_DB_StatCount[Stat]++;
}

void Blood_DB_DeleteSpriteStat(int16_t SpriteIndex)
{
    if (SpriteIndex < 0 || SpriteIndex >= 4096)
    {
        /* "src\\db.cpp", "nSprite >= 0 && nSprite < kMaxSprites" */
        Blood_ExitError(569, IDA2LinearDSeg04(0x121b1a),
                        IDA2LinearDSeg04(0x121b25));
    }

    int16_t Stat = Blood_Sprites[SpriteIndex].StatNum;

    if (Stat < 0 || Stat > 1024)
    {
        /* "src\\db.cpp", "nStat >= 0 && nStat <= kMaxStatus" */
        Blood_ExitError(571, IDA2LinearDSeg04(0x121b4b),
                        IDA2LinearDSeg04(0x121b56));
    }

    int16_t NextIndex = Blood_NextSpriteStat[SpriteIndex];
    if (NextIndex < 0)
        NextIndex = Blood_HeadSpriteStat[Stat];

    Blood_PrevSpriteStat[NextIndex] = Blood_PrevSpriteStat[SpriteIndex];

    if (Blood_HeadSpriteStat[Stat] != SpriteIndex)
    {
        Blood_NextSpriteStat[Blood_PrevSpriteStat[SpriteIndex]] =
            Blood_NextSpriteStat[SpriteIndex];
    }
    else
    {
        Blood_HeadSpriteStat[Stat] =
            Blood_NextSpriteStat[SpriteIndex];
    }

    Blood_DB_StatCount[Stat]--;
    Blood_Sprites[SpriteIndex].StatNum = -1;
}

void Blood_DB_InitSprites(void)
{
    for (int32_t I = 0; I < Blood_MaxStatus + 1; I++)
    {
        Blood_HeadSpriteSect[I] = -1;
        Blood_HeadSpriteStat[I] = -1;
    }

    for (int32_t I = 0; I < Blood_MaxSprites; I++)
    {
        Blood_Sprites[I].SectNum = -1;
        Blood_Sprites[I].XVel = -1;
        Blood_DB_InsertSpriteStat(I, Blood_MaxStatus);
    }

    memset(&Blood_DB_StatCount, 0, sizeof(Blood_DB_StatCount));
}

int16_t Blood_DB_SpawnSprite(int16_t SectorIndex, int16_t Stat)
{
    int16_t SpriteIndex = Blood_HeadSpriteStat[Blood_MaxStatus];

    if (SpriteIndex >= 4096)
    {
        /* "src\\db.cpp", "nSprite < kMaxSprites" */
        Blood_ExitError(620, IDA2LinearDSeg04(0x121b78),
                        IDA2LinearDSeg04(0x121b83));
    }

    if (SpriteIndex < 0)
        return SpriteIndex;

    Blood_DB_DeleteSpriteStat(SpriteIndex);

    Blood_Sprite_t *Sprite = &Blood_Sprites[SpriteIndex];
    memset(Sprite, 0, sizeof(Blood_Sprite_t));

    Blood_DB_InsertSpriteStat(SpriteIndex, Stat);
    Blood_DB_InsertSpriteSect(SpriteIndex, SectorIndex);

    Sprite->CStat = 128;
    Sprite->ClipDist = 32;
    Sprite->YRepeat = 64;
    Sprite->Owner = -1;
    Sprite->Extra = -1;
    Sprite->XVel = SpriteIndex; /* ??? */
    Sprite->XRepeat = Sprite->YRepeat;

    Blood_BD_1A2EC4[SpriteIndex] = 0;
    Blood_BD_19EEC4[SpriteIndex] = 0;
    Blood_BD_19AEC4[SpriteIndex] = 0;

    return SpriteIndex;
}

void Blood_DB_DespawnSprite(int16_t SpriteIndex)
{
    int16_t Extra = Blood_Sprites[SpriteIndex].Extra;

    if (Extra > 0)
    {
        reg_eax = Extra;
        Blood_Call(IDA2LinearCSeg01(0x1f990));
    }

    int16_t Stat = Blood_Sprites[SpriteIndex].StatNum;

    if (Stat < 0 || Stat >= Blood_MaxStatus)
    {
        /* "src\\db.cpp", "sprite[nSprite].statnum >= 0 && sprite[nSprite].statnum < kMaxStatus" */
        Blood_ExitError(667, IDA2LinearDSeg04(0x121b99),
                        IDA2LinearDSeg04(0x121ba4));
    }

    Blood_DB_DeleteSpriteStat(SpriteIndex);

    int16_t SectorIndex = Blood_Sprites[SpriteIndex].SectNum;

    if (SectorIndex < 0 || SectorIndex >= Blood_MaxSectors)
    {
        /* "src\\db.cpp", "sprite[nSprite].sectnum >= 0 && sprite[nSprite].sectnum < kMaxSectors" */
        Blood_ExitError(670, IDA2LinearDSeg04(0x121be9),
                        IDA2LinearDSeg04(0x121bf4));
    }

    Blood_DB_DeleteSpriteSect(SpriteIndex);
    Blood_DB_InsertSpriteStat(SpriteIndex, 1024);
}

int32_t Blood_DB_MoveSpriteSect(int16_t SpriteIndex, int16_t NewSectorIndex)
{
    if (SpriteIndex < 0 || SpriteIndex >= 4096)
    {
        /* "src\\db.cpp", "nSprite >= 0 && nSprite < kMaxSprites" */
        Blood_ExitError(679, IDA2LinearDSeg04(0x121c3a),
                        IDA2LinearDSeg04(0x121c45));
    }

    if (NewSectorIndex < 0 || NewSectorIndex >= 1024)
    {
        /* "src\\db.cpp", "nSector >= 0 && nSector < kMaxSectors" */
        Blood_ExitError(680, IDA2LinearDSeg04(0x121c6b),
                        IDA2LinearDSeg04(0x121c76));
    }

    int16_t Stat = Blood_Sprites[SpriteIndex].StatNum;

    if (Stat < 0 || Stat >= 1024)
    {
        /* "src\\db.cpp", "sprite[nSprite].statnum >= 0 && sprite[nSprite].statnum < kMaxStatus" */
        Blood_ExitError(682, IDA2LinearDSeg04(0x121c9c),
                        IDA2LinearDSeg04(0x121ca7));
    }

    int16_t OldSectorIndex = Blood_Sprites[SpriteIndex].SectNum;

    if (OldSectorIndex < 0 || OldSectorIndex >= 1024)
    {
        /* "src\\db.cpp", "sprite[nSprite].sectnum >= 0 && sprite[nSprite].sectnum < kMaxSectors" */
        Blood_ExitError(683, IDA2LinearDSeg04(0x121cec),
                        IDA2LinearDSeg04(0x121cf7));
    }

    Blood_DB_DeleteSpriteSect(SpriteIndex);
    Blood_DB_InsertSpriteSect(SpriteIndex, NewSectorIndex);

    return 0;
}

int32_t Blood_DB_MoveSpriteStat(int16_t SpriteIndex, int16_t NewStat)
{
    if (SpriteIndex < 0 || SpriteIndex >= 4096)
    {
        /* "src\\db.cpp", "nSprite >= 0 && nSprite < kMaxSprites" */
        Blood_ExitError(694, IDA2LinearDSeg04(0x121d3d),
                        IDA2LinearDSeg04(0x121d48));
    }

    if (NewStat < 0 || NewStat >= 1024)
    {
        /* "src\\db.cpp", "nStatus >= 0 && nStatus < kMaxStatus" */
        Blood_ExitError(695, IDA2LinearDSeg04(0x121d6e),
                        IDA2LinearDSeg04(0x121d79));
    }

    int16_t OldStat = Blood_Sprites[SpriteIndex].StatNum;

    if (OldStat < 0 || OldStat >= 1024)
    {
        /* "src\\db.cpp", "sprite[nSprite].statnum >= 0 && sprite[nSprite].statnum < kMaxStatus" */
        Blood_ExitError(696, IDA2LinearDSeg04(0x121d9e),
                        IDA2LinearDSeg04(0x121da9));
    }

    int16_t SectorIndex = Blood_Sprites[SpriteIndex].SectNum;

    if (SectorIndex < 0 || SectorIndex >= 1024)
    {
        /* "src\\db.cpp", "sprite[nSprite].sectnum >= 0 && sprite[nSprite].sectnum < kMaxSectors" */
        Blood_ExitError(697, IDA2LinearDSeg04(0x121dee),
                        IDA2LinearDSeg04(0x121df9));
    }

    Blood_DB_DeleteSpriteStat(SpriteIndex);
    Blood_DB_InsertSpriteStat(SpriteIndex, NewStat);

    return 0;
}

int16_t Blood_DB_NewXSprite(int16_t SpriteIndex)
{
    int16_t XSpriteIndex = Blood_DB_XSpritesFreeList[0];
    Blood_DB_XSpritesFreeList[0] = Blood_DB_XSpritesFreeList[XSpriteIndex];

    if (!XSpriteIndex)
    {
        /* "src\\db.cpp" */
        Blood_SetErrorInfo(756, IDA2LinearDSeg04(0x121e3f));
        /* "Out of free XSprites" */
        Blood_ExitError1(IDA2LinearDSeg04(0x121e4a));
    }

    memset(&Blood_DB_XSprites[XSpriteIndex], 0, sizeof(Blood_DB_XSprite_t));

    Blood_DB_XSprites[XSpriteIndex].field_0 = (SpriteIndex & 0x3fff);
    Blood_Sprites[SpriteIndex].Extra = XSpriteIndex;

    return XSpriteIndex;
}

void Blood_DB_DelXSprite(int16_t XSpriteIndex)
{
    int32_t SpriteIndex = Blood_DB_XSprites[XSpriteIndex].field_0;
    SpriteIndex &= 0x3fff;

    /* Check if negative */
    if (SpriteIndex & 0x2000)
    {
        /* "src\\db.cpp", "xsprite[nXSprite].reference >= 0" */
        Blood_ExitError(768, IDA2LinearDSeg04(0x121e5f),
                        IDA2LinearDSeg04(0x121e6a));
    }

    if (Blood_Sprites[SpriteIndex].Extra != XSpriteIndex)
    {
        /* "src\\db.cpp", "sprite[xsprite[nXSprite].reference].extra == nXSprite" */
        Blood_ExitError(769, IDA2LinearDSeg04(0x121e8b),
                        IDA2LinearDSeg04(0x121e96));
    }

    Blood_DB_XSpritesFreeList[XSpriteIndex] = Blood_DB_XSpritesFreeList[0];
    Blood_DB_XSpritesFreeList[0] = XSpriteIndex;

    Blood_DB_XSprites[XSpriteIndex].field_0 |= 0x3fff;
    Blood_Sprites[SpriteIndex].Extra = -1;
}

int16_t Blood_DB_NewXWall(int16_t WallIndex)
{
    int16_t XWallIndex = Blood_DB_XWallsFreeList[0];
    Blood_DB_XWallsFreeList[0] = Blood_DB_XWallsFreeList[XWallIndex];

    if (!XWallIndex)
    {
        /* "src\\db.cpp" */
        Blood_SetErrorInfo(782, IDA2LinearDSeg04(0x121ecc));
        /* "Out of free XWalls" */
        Blood_ExitError1(IDA2LinearDSeg04(0x121ed7));
    }

    memset(&Blood_DB_XWalls[XWallIndex], 0, sizeof(Blood_DB_XWall_t));

    Blood_DB_XWalls[XWallIndex].field_0 = (WallIndex & 0x3fff);
    Blood_Walls[WallIndex].Extra = XWallIndex;

    return XWallIndex;
}

void Blood_DB_DelXWall(int16_t XWallIndex)
{
    int16_t WallIndex = Blood_DB_XWalls[XWallIndex].field_0;
    WallIndex &= 0x3fff;

    /* Check if negative */
    if (WallIndex & 0x2000)
    {
        /* "src\\db.cpp", "xwall[nXWall].reference >= 0" */
        Blood_ExitError(794, IDA2LinearDSeg04(0x121eea),
                        IDA2LinearDSeg04(0x121ef5));
    }

    Blood_DB_XWallsFreeList[XWallIndex] = Blood_DB_XWallsFreeList[0];
    Blood_DB_XWallsFreeList[0] = XWallIndex;

    Blood_DB_XWalls[XWallIndex].field_0 |= 0x3fff;
    Blood_Walls[WallIndex].Extra = -1;
}

int16_t Blood_DB_NewXSector(int16_t SectorIndex)
{
    int16_t XSectorIndex = Blood_DB_XSectorsFreeList[0];
    Blood_DB_XSectorsFreeList[0] = Blood_DB_XSectorsFreeList[XSectorIndex];

    if (!XSectorIndex)
    {
        /* "src\\db.cpp" */
        Blood_SetErrorInfo(807, IDA2LinearDSeg04(0x121f12));
        /* "Out of free XSectors" */
        Blood_ExitError1(IDA2LinearDSeg04(0x121f1d));
    }

    memset(&Blood_DB_XSectors[XSectorIndex], 0, sizeof(Blood_DB_XSector_t));

    Blood_DB_XSectors[XSectorIndex].field_0 = (SectorIndex & 0x3fff);
    Blood_Sectors[SectorIndex].Extra = XSectorIndex;

    return XSectorIndex;
}

void Blood_DB_DelXSector(int16_t XSectorIndex)
{
    int16_t SectorIndex = Blood_DB_XSectors[XSectorIndex].field_0;
    SectorIndex &= 0x3fff;

    /* Test if negative */
    if (SectorIndex & 0x2000)
    {
        /* "src\\db.cpp", "xsector[nXSector].reference >= 0" */
        Blood_ExitError(819, IDA2LinearDSeg04(0x121f32),
                        IDA2LinearDSeg04(0x121f3d));
    }

    Blood_DB_XSectorsFreeList[XSectorIndex] = Blood_DB_XSectorsFreeList[0];
    Blood_DB_XSectorsFreeList[0] = XSectorIndex;

    Blood_DB_XSectors[XSectorIndex].field_0 |= 0x3fff;
    Blood_Sectors[SectorIndex].Extra = -1;
}
