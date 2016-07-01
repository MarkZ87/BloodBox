
#ifndef __BLOOD_DB_HPP__
#define __BLOOD_DB_HPP__

/* 56 bytes */
typedef struct __attribute__((packed))
{
    int32_t field_0;
    uint8_t Unknown[52];
}
Blood_DB_XSprite_t;

/* 24 bytes */
typedef struct __attribute__((packed))
{
    int32_t field_0;
    uint8_t Unknown[20];
}
Blood_DB_XWall_t;

/* 60 bytes */
typedef struct __attribute__((packed))
{
    int32_t field_0;
    uint8_t Unknown[56];
}
Blood_DB_XSector_t;

extern int16_t Blood_DB_StatCount[Blood_MaxStatus + 1];
extern int32_t Blood_BD_1A2EC4[Blood_MaxSprites];
extern int32_t Blood_BD_19EEC4[Blood_MaxSprites];
extern int32_t Blood_BD_19AEC4[Blood_MaxSprites];

extern int16_t Blood_DB_XSpritesFreeList[]; /* TODO: How many? */
extern Blood_DB_XSprite_t Blood_DB_XSprites[]; /* TODO How many? */

extern int16_t Blood_DB_XWallsFreeList[]; /* TODO: How many? */
extern Blood_DB_XWall_t Blood_DB_XWalls[]; /* TODO: How many? */

extern int16_t Blood_DB_XSectorsFreeList[]; /* TODO: How many? */
extern Blood_DB_XSector_t Blood_DB_XSectors[]; /* TODO: How many? */

void    Blood_DB_InsertSpriteSect(int16_t SpriteIndex, int16_t SectorIndex);
void    Blood_DB_DeleteSpriteSect(int16_t SpriteIndex);
void    Blood_DB_InsertSpriteStat(int16_t SpriteIndex, int16_t Stat);
void    Blood_DB_DeleteSpriteStat(int16_t SpriteIndex);
void    Blood_DB_InitSprites(void);
int16_t Blood_DB_SpawnSprite(int16_t SectorIndex, int16_t Stat);
void    Blood_DB_DespawnSprite(int16_t SpriteIndex);
int32_t Blood_DB_MoveSpriteSect(int16_t SpriteIndex, int16_t NewSectorIndex);
int32_t Blood_DB_MoveSpriteStat(int16_t SpriteIndex, int16_t NewStat);
int16_t Blood_DB_NewXSprite(int16_t SpriteIndex);
void    Blood_DB_DelXSprite(int16_t XSpriteIndex);
int16_t Blood_DB_NewXWall(int16_t WallIndex);
void    Blood_DB_DelXWall(int16_t XWallIndex);
int16_t Blood_DB_NewXSector(int16_t SectorIndex);
void    Blood_DB_DelXSector(int16_t XSectorIndex);

#endif /* __BLOOD_DB_HPP__ */
