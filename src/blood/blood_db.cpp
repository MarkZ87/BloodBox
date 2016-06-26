
#include <memory.h>
#include "blood.hpp"

void Blood_DB_InsertSpriteSect(int16_t SpriteIndex, int16_t SectorIndex)
{
    if (SpriteIndex < 0 || SpriteIndex >= 4096)
    {
        /* "src\\db.cpp", "nSprite >= 0 && nSprite < kMaxSprites" */
        Blood_ExitError(498, 0x1219f8, 0x121a03);
    }

    if (SectorIndex < 0 || SectorIndex >= 1024)
    {
        /* "src\\db.cpp", "nSector >= 0 && nSector < kMaxSectors" */
        Blood_ExitError(499, 0x121a29, 0x121a34);
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
        Blood_ExitError(523, 0x121a5a, 0x121a65);
    }

    int16_t SectorIndex = Blood_Sprites[SpriteIndex].SectNum;

    if (SectorIndex < 0 || SectorIndex >= 1024)
    {
        /* "src\\db.cpp", "nSector >= 0 && nSector < kMaxSectors" */
        Blood_ExitError(526, 0x121a8b, 0x121a96);
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
        Blood_ExitError(544, 0x121abc, 0x121ac7);
    }

    if (Stat < 0 || Stat > 1024)
    {
        /* "src\\db.cpp", "nStat >= 0 && nStat <= kMaxStatus" */
        Blood_ExitError(545, 0x121aed, 0x121af8);
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
        Blood_ExitError(569, 0x121b1a, 0x121b25);
    }

    int16_t Stat = Blood_Sprites[SpriteIndex].StatNum;

    if (Stat < 0 || Stat > 1024)
    {
        /* "src\\db.cpp", "nStat >= 0 && nStat <= kMaxStatus" */
        Blood_ExitError(571, 0x121b4b, 0x121b56);
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
        Blood_ExitError(620, 0x121b78, 0x121b83);
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
        Blood_ExitError(667, 0x121b99, 0x121ba4);
    }

    Blood_DB_DeleteSpriteStat(SpriteIndex);

    int16_t SectorIndex = Blood_Sprites[SpriteIndex].SectNum;

    if (SectorIndex < 0 || SectorIndex >= Blood_MaxSectors)
    {
        /* "src\\db.cpp", "sprite[nSprite].sectnum >= 0 && sprite[nSprite].sectnum < kMaxSectors" */
        Blood_ExitError(670, 0x121be9, 0x121bf4);
    }

    Blood_DB_DeleteSpriteSect(SpriteIndex);
    Blood_DB_InsertSpriteStat(SpriteIndex, 1024);
}
