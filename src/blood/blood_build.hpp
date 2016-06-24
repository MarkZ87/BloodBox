
#ifndef __BLOOD_BUILD_HPP__
#define __BLOOD_BUILD_HPP__

#include "blood.hpp"

#define Blood_MaxSectors 1024
#define Blood_MaxWalls 8192
#define Blood_MaxSprites 4096

#define Blood_MaxTiles 9216
#define Blood_MaxStatus 1024
#define Blood_MaxPlayers 16
#define Blood_MaxXDim 1600
#define Blood_MaxYDim 1200
#define Blood_MaxPalLookups 256
#define Blood_MaxPSKyTiles 256
#define Blood_MaxSpritesOnScreen 1024

#define Blood_ClipMask0 ((1 << 16) + 1)
#define Blood_ClipMask1 ((256 << 16) + 64)

#define Blood_MaxXDim 1600

#define Blood_MaxClipNum 512
#define Blood_MaxPerms 512
#define Blood_MaxTileFiles 256
#define Blood_MaxYSaves ((Blood_MaxXDim * Blood_MaxSprites) >> 7)
/* Warning: This depends on MaxYSaves & MaxYDim! */
#define Blood_MaxNodesPerLine 42
#define Blood_MaxWallsB 2048
#define Blood_MaxClipDist 1024

typedef uint32_t Blood_Pointer_t;

/*
 * CeilingStat / FloorStat:
 *   Bit 0: 1 = parallaxing, 0 = not                                 "P"
 *   Bit 1: 1 = groudraw, 0 = not
 *   Bit 2: 1 = swap x&y, 0 = not                                    "F"
 *   Bit 3: 1 = double smooshiness                                   "E"
 *   Bit 4: 1 = x-flip                                               "F"
 *   Bit 5: 1 = y-flip                                               "F"
 *   Bit 6: 1 = Align texture to first wall of sector                "R"
 *   Bits 7-8:                                                       "T"
 *          00 = normal floors
 *          01 = masked floors
 *          10 = transluscent masked floors
 *          11 = reverse transluscent masked floors
 *   Bits 9-15: reserved
 * 40 bytes
 */
typedef struct __attribute__((packed))
{
    int16_t WallPtr, WallNum;
    int32_t CeilingZ, FloorZ;
    int16_t CeilingStat, FloorStat;
    int16_t CeilingPicNum, CeilingHeiNum;
    int8_t  CeilingShade;
    int8_t  CeilingPal;
    int8_t  CeilingXPanning, CeilingYPanning;
    int16_t FloorPicNum, FloorHeiNum;
    int8_t  FloorShade;
    int8_t  FloorPal;
    int8_t  FloorXPanning, FloorYPanning;
    int8_t  Visibility, Filler;
    int16_t LoTag, HiTag, Extra;
}
Blood_Sector_t;

/*
 * CStat:
 *   Bit 0: 1 = Blocking wall (use with clipmove, getzrange)         "B"
 *   Bit 1: 1 = bottoms of invisible walls swapped, 0 = not          "2"
 *   Bit 2: 1 = align picture on bottom (for doors), 0 = top         "O"
 *   Bit 3: 1 = x-flipped, 0 = normal                                "F"
 *   Bit 4: 1 = masking wall, 0 = not                                "M"
 *   Bit 5: 1 = 1-way wall, 0 = not                                  "1"
 *   Bit 6: 1 = Blocking wall (use with hitscan / cliptype 1)        "H"
 *   Bit 7: 1 = Transluscence, 0 = not                               "T"
 *   Bit 8: 1 = y-flipped, 0 = normal                                "F"
 *   Bit 9: 1 = Transluscence reversing, 0 = normal                  "T"
 *   Bits 10-15: reserved
 * 32 bytes
 */
typedef struct __attribute__((packed))
{
    int32_t X, Y;
    int16_t Point2, NextWall;
    int16_t NextSector, CStat;
    int16_t PicNum, OverPicNum;
    int8_t  Shade;
    int8_t  Pal;
    int8_t  XRepeat, YRepeat;
    int8_t  XPanning, YPanning;
    int16_t LoTag, HiTag, Extra;
}
Blood_Wall_t;

/*
 * CStat:
 *   bit 0: 1 = Blocking sprite (use with clipmove, getzrange)       "B"
 *   bit 1: 1 = transluscence, 0 = normal                            "T"
 *   bit 2: 1 = x-flipped, 0 = normal                                "F"
 *   bit 3: 1 = y-flipped, 0 = normal                                "F"
 *   bits 5-4: 00 = FACE sprite (default)                            "R"
 *             01 = WALL sprite (like masked walls)
 *             10 = FLOOR sprite (parallel to ceilings&floors)
 *   bit 6: 1 = 1-sided sprite, 0 = normal                           "1"
 *   bit 7: 1 = Real centered centering, 0 = foot center             "C"
 *   bit 8: 1 = Blocking sprite (use with hitscan / cliptype 1)      "H"
 *   bit 9: 1 = Transluscence reversing, 0 = normal                  "T"
 *   bits 10-14: reserved
 *   bit 15: 1 = Invisible sprite, 0 = not invisible
 * 44 bytes
 */
typedef struct __attribute__((packed))
{
    int32_t X, Y, Z;
    int16_t CStat, PicNum;
    int8_t  Shade;
    int8_t  Pal, ClipDist, Filler;
    uint8_t XRepeat, YRepeat;
    int8_t  XOffset, YOffset;
    int16_t SectNum, StatNum;
    int16_t Ang, Owner;
    int16_t XVel, YVel, ZVel;
    int16_t LoTag, HiTag, Extra;
}
Blood_Sprite_t;

extern int32_t Blood_ViewingRange;
extern int32_t Blood_ViewingRangeRecip;
extern int32_t Blood_YXAspect, Blood_XYAspect;
extern int32_t Blood_XDimen; /* = -1 */
extern int32_t Blood_XDimenScale;
extern int32_t Blood_XDimScale;
extern int32_t Blood_WindowX1, Blood_WindowY1;
extern int32_t Blood_WindowX2, Blood_WindowY2;
extern int32_t Blood_WX1, Blood_WY1;
extern int32_t Blood_WX2, Blood_WY2;
extern int32_t Blood_XDim, Blood_YDim;
extern int32_t Blood_HalfXDimen;
extern int32_t Blood_XDimenRecip;
extern int32_t Blood_YDimen;
extern int32_t Blood_ViewOffset;
extern int32_t Blood_BytesPerLine;
extern int32_t Blood_StereoMode;
extern int8_t  Blood_VidOption;
extern int32_t Blood_OStereoPixelWidth; /* = -1 */
extern int32_t Blood_StereoPixelWidth; /* = 28 */
extern int16_t Blood_StartUMost[Blood_MaxXDim];
extern int16_t Blood_StartDMost[Blood_MaxXDim];

extern int32_t Blood_BeforeDrawRooms; /* = 1 */
extern int32_t Blood_TotalArea;
extern int32_t Blood_GlobalPosX;
extern int32_t Blood_GlobalPosY;
extern int32_t Blood_GlobalPosZ;
extern int16_t Blood_GlobalAng;
extern int32_t Blood_GlobalHoriz;
extern int32_t Blood_GlobalUClip;
extern int32_t Blood_GlobalDClip;
extern int32_t Blood_GlobalPisibility;
extern int32_t Blood_GlobalVisibility;
extern int32_t Blood_GlobalHisibility;
extern int32_t Blood_GlobalCisibility;
extern int32_t Blood_ParallaxVisibility;
extern int16_t Blood_GlobalCurSectNum;
extern int32_t Blood_TotalClockLock;
extern int32_t Blood_TotalClock;
extern int32_t Blood_CosGlobalAng;
extern int32_t Blood_SinGlobalAng;
extern int16_t Blood_SinTable[2048];
extern int32_t Blood_CosViewingRangeGlobalAng;
extern int32_t Blood_SinViewingRangeGlobalAng;
extern int32_t Blood_ActivePage;
extern int32_t Blood_StereoWidth; /* = 23040 */
extern int32_t Blood_FramePlace;
/*extern char *  Blood_Screen;*/ /* = 0 */
extern Blood_Pointer_t Blood_Screen; /* = 0 */
extern int32_t Blood_OXYAspect; /* = -1 */
extern int32_t Blood_OXDimen; /* = -1 */
extern int32_t Blood_OViewingRange; /* = -1 */
extern int32_t Blood_FrameOffset;
extern int8_t  Blood_GotSector[(Blood_MaxSectors + 7) >> 3];
extern int16_t Blood_NumSectors;
extern int16_t Blood_UMost[Blood_MaxXDim];
extern int16_t Blood_DMost[Blood_MaxXDim];
extern int16_t Blood_NumHits;
extern int16_t Blood_NumScans;
extern int16_t Blood_NumBunches;
extern int16_t Blood_MaskWallCount;
extern int32_t Blood_SMostWallCount; /* = -1 */
extern int16_t Blood_SMostCount;
extern int32_t Blood_SpriteSortCount;
extern int8_t  Blood_GlobParaCeilClip;
extern int8_t  Blood_GlobParaFlorClip;
extern uint8_t Blood_TempBuf[Blood_MaxWalls];
extern int8_t  Blood_AutoMapping;
extern int16_t Blood_BunchFirst[Blood_MaxWallsB];
extern int16_t Blood_P2[Blood_MaxWallsB];
extern int8_t  Blood_Show2DWall[(Blood_MaxWalls + 7) >> 3];
extern int16_t Blood_TheWall[Blood_MaxWallsB];
extern int8_t  Blood_Pow2Char[8]; /* = {1, 2, 4, 8, 16, 32, 64, 128} */
extern int16_t Blood_BunchLast[Blood_MaxWallsB];
extern int32_t Blood_Visibility;

extern int8_t  Blood_Show2DSector[(Blood_MaxSectors + 7) >> 3];
extern Blood_Sector_t Blood_Sectors[Blood_MaxSectors];
extern Blood_Wall_t Blood_Walls[Blood_MaxWalls];
extern Blood_Sprite_t Blood_Sprites[Blood_MaxSprites];
extern int16_t Blood_SectorBorder[256];
extern int16_t Blood_SectorBorderCount;
extern int16_t Blood_HeadSpriteSect[Blood_MaxSectors + 1];
extern int16_t Blood_NextSpriteSect[Blood_MaxSprites];
extern int8_t  Blood_ShowInvisibility;
extern Blood_Sprite_t Blood_TSprites[Blood_MaxSpritesOnScreen];
extern int32_t Blood_TSpriteSortCount;
extern int16_t Blood_TheSector[Blood_MaxWallsB];
extern int32_t Blood_XB1[Blood_MaxWallsB];
extern int32_t Blood_YB1[Blood_MaxWallsB];
extern int32_t Blood_XB2[Blood_MaxWallsB];
extern int32_t Blood_YB2[Blood_MaxWallsB];
extern int32_t Blood_RX1[Blood_MaxWallsB];
extern int32_t Blood_RX2[Blood_MaxWallsB];
extern int32_t Blood_RY1[Blood_MaxWallsB];
extern int32_t Blood_RY2[Blood_MaxWallsB];

extern int16_t Blood_UPlc[Blood_MaxXDim];
extern int16_t Blood_DPlc[Blood_MaxXDim];

extern uint16_t Blood_SqrtTable[4096];
extern uint16_t Blood_ShLookup[4096 + 256];

extern int16_t Blood_SearchIt;
extern int32_t Blood_SearchX; /* = -1 */
extern int32_t Blood_SearchY;
extern int16_t Blood_SearchSector;
extern int16_t Blood_SearchWall;
extern int16_t Blood_SearchStat;

extern int16_t Blood_SMost[Blood_MaxYSaves];
extern int16_t Blood_SMostStart[Blood_MaxWallsB];
extern int8_t  Blood_SMostWallType[Blood_MaxWallsB];
extern int32_t Blood_SMostWall[Blood_MaxWallsB];

extern int16_t Blood_MaskWall[Blood_MaxWallsB];
extern int16_t Blood_UWall[Blood_MaxXDim];
extern int16_t Blood_DWall[Blood_MaxXDim];
extern int32_t Blood_SWall[Blood_MaxXDim];
extern int32_t Blood_LWall[Blood_MaxXDim + 4];
extern int32_t Blood_GlobalOrientation;
extern int32_t Blood_GlobalXPanning;
extern int32_t Blood_GlobalYPanning;
extern int32_t Blood_GlobalShade;
extern int16_t Blood_GlobalPicNum;
extern int16_t Blood_GlobalShiftVal;
extern int8_t  Blood_PicSiz[Blood_MaxTiles];
extern int32_t Blood_Pow2Long[32];
extern int16_t Blood_TileSizeY[Blood_MaxTiles];
extern int32_t Blood_PicAnm[Blood_MaxTiles];
extern int32_t Blood_GlobVis;
extern int32_t Blood_GlobalPal;
extern int32_t Blood_GlobalYScale;
extern int32_t Blood_GlobalZD;

void    Blood_SetAspect(int32_t daxrange, int32_t daaspect);
void    Blood_SetView(int32_t X1, int32_t Y1, int32_t X2, int32_t Y2);
int32_t Blood_BunchFront(int32_t BunchIndex1, int32_t BunchIndex2);
void    Blood_DrawRooms(int32_t daposx, int32_t daposy, int32_t daposz,
                        int16_t daang, int32_t dahoriz, int16_t dacursectnum);
void    Blood_ScanSector(int16_t SectorIndex);
int32_t Blood_NSqrtAsm(int32_t eax);
void    Blood_GetZsOfSlope(int16_t SectorIndex, int32_t DaX, int32_t DaY,
                           int32_t *CeilingZ, int32_t *FloorZ);
int32_t Blood_Inside(int32_t X, int32_t Y, int16_t SectorIndex);
void    Blood_UpdateSector(int32_t X, int32_t Y, int16_t *SectorIndex);
uint8_t Blood_WallMost(int16_t *MostBuffer, int32_t W, int32_t SectorIndex,
                       char DaStat);
void    Blood_GrouScan(int32_t DaX1, int32_t DaX2, int32_t SectorIndex,
                       char DaStat);
void    Blood_CeilScan(int32_t X1, int32_t X2, int32_t SectorIndex);
void    Blood_ParaScan(int32_t DaX1, int32_t DaX2, int32_t SectorIndex,
                       char DaStat, int32_t BunchIndex);
void    Blood_FlorScan(int32_t X1, int32_t X2, int32_t SectorIndex);
void    Blood_PrepWall(int32_t Z, Blood_Wall_t *Wall);
void    Blood_WallScan(int32_t X1, int32_t X2, int16_t *UWall, int16_t *DWall,
                       int32_t *SWall, int32_t *LWall);
int32_t Blood_AnimateOffs(int16_t TileIndex, int16_t FakeVar);
void    Blood_DrawWalls(int32_t BunchIndex);

#endif /* __BLOOD_BUILD_HPP__ */