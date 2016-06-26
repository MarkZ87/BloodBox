
#ifndef __BLOOD_DB_HPP__
#define __BLOOD_DB_HPP__

extern int16_t Blood_DB_StatCount[Blood_MaxStatus + 1];

extern int32_t Blood_BD_1A2EC4[Blood_MaxSprites];
extern int32_t Blood_BD_19EEC4[Blood_MaxSprites];
extern int32_t Blood_BD_19AEC4[Blood_MaxSprites];

void    Blood_DB_InsertSpriteSect(int16_t SpriteIndex, int16_t SectorIndex);
void    Blood_DB_DeleteSpriteSect(int16_t SpriteIndex);
void    Blood_DB_InsertSpriteStat(int16_t SpriteIndex, int16_t Stat);
void    Blood_DB_DeleteSpriteStat(int16_t SpriteIndex);
void    Blood_DB_InitSprites(void);
int16_t Blood_DB_SpawnSprite(int16_t SectorIndex, int16_t Stat);

#endif /* __BLOOD_DB_HPP__ */