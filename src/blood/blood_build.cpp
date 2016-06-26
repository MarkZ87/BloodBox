
// "Build Engine & Tools" Copyright (c) 1993-1997 Ken Silverman
// Ken Silverman's official web site: "http://www.advsys.net/ken"
// See the included license file "BUILDLIC.TXT" for license info.

#include <math.h>
#include "blood.hpp"

static int32_t Blood_NSqrtAsm(int32_t eax)
{
    int16_t cx;

    if (eax & 0xff000000)
        cx = Blood_ShLookup[(eax >> 24) + 4096];
    else
        cx = Blood_ShLookup[(eax >> 12)];

    eax >>= (cx & 0xff);
    eax = Blood_SqrtTable[eax];
    eax >>= (cx >> 8);

    return eax;
}

static int32_t Blood_MSqrtAsm(int32_t ecx)
{
    int32_t eax = 0x40000000;
    int32_t ebx = 0x20000000;

    do
    {
        if (ecx >= eax)
        {
            ecx -= eax;
            eax += ebx << 2;
        }

        eax = (eax - ebx) >> 1;
        ebx >>= 2;
    }
    while (ebx);

    if (ecx >= eax)
        eax++;
    eax >>= 1;

    return eax;
}

static inline void Blood_SetGotPic(int32_t PicNum)
{
    if (Blood_WallLock[PicNum] < 200)
        Blood_WallLock[PicNum] = 19;
    Blood_GotPic[PicNum >> 3] |= Blood_Pow2Char[PicNum & 7];
}

static void Blood_LoadTile(short TileIndex)
{
    reg_eax = TileIndex;
    Blood_Call(Blood_ProcAddress_LoadTile);
}

static int32_t Blood_GetPalLookup(int32_t DaVis, int32_t DaShade)
{
    int32_t Temp0 = reg_eax;
    int32_t Temp1 = reg_edx;

    reg_eax = DaVis;
    reg_edx = DaShade;
    Blood_Call(Blood_ProcAddress_GetPalLookup);
    int32_t Result = reg_eax;

    reg_eax = Temp0;
    reg_edx = Temp1;

    return Result;
}

static int32_t Blood_BunchFront(int32_t BunchIndex1, int32_t BunchIndex2)
{
    int32_t Temp0 = reg_eax;
    int32_t Temp1 = reg_edx;

    reg_eax = BunchIndex1;
    reg_edx = BunchIndex2;
    Blood_Call(Blood_ProcAddress_BunchFront);
    int32_t Result = reg_eax;

    reg_eax = Temp0;
    reg_edx = Temp1;

    return Result;
}

static uint8_t Blood_WallMost(int16_t *MostBuffer, int32_t W, int32_t SectorIndex,
                              char DaStat)
{
    int32_t Temp[4];
    Temp[0] = reg_eax; Temp[1] = reg_edx;
    Temp[2] = reg_ebx; Temp[3] = reg_ecx;

    reg_eax = Ptr2Offset(MostBuffer);
    reg_edx = W;
    reg_ebx = SectorIndex;
    reg_ecx = DaStat;
    Blood_Call(Blood_ProcAddress_WallMost);
    int32_t Result = reg_eax;

    reg_eax = Temp[0]; reg_edx = Temp[1];
    reg_ebx = Temp[2]; reg_ecx = Temp[3];

    return (uint8_t)Result;
}

static void Blood_GrouScan(int32_t DaX1, int32_t DaX2, int32_t SectorIndex,
                           char DaStat)
{
    int32_t Temp[4];
    Temp[0] = reg_eax; Temp[1] = reg_edx;
    Temp[2] = reg_ebx; Temp[3] = reg_ecx;

    reg_eax = DaX1;
    reg_edx = DaX2;
    reg_ebx = SectorIndex;
    reg_ecx = DaStat;
    Blood_Call(Blood_ProcAddress_GrouScan);

    reg_eax = Temp[0]; reg_edx = Temp[1];
    reg_ebx = Temp[2]; reg_ecx = Temp[3];
}

#if 0
static void Blood_ParaScan(int32_t DaX1, int32_t DaX2, int32_t SectorIndex,
                           char DaStat, int32_t BunchIndex)
{
    int32_t Temp[4];
    Temp[0] = reg_eax; Temp[1] = reg_edx;
    Temp[2] = reg_ebx; Temp[3] = reg_ecx;

    reg_eax = DaX1;
    reg_edx = DaX2;
    reg_ebx = SectorIndex;
    reg_ecx = DaStat;
    CPU_Push32(BunchIndex);
    Blood_Call(Blood_ProcAddress_ParaScan);

    reg_eax = Temp[0]; reg_edx = Temp[1];
    reg_ebx = Temp[2]; reg_ecx = Temp[3];
}
#endif

static void Blood_PrepWall(int32_t Z, Blood_Wall_t *Wall)
{
    int32_t Temp0 = reg_eax;
    int32_t Temp1 = reg_edx;

    reg_eax = Z;
    reg_edx = Ptr2Offset(Wall);
    Blood_Call(Blood_ProcAddress_PrepWall);

    reg_eax = Temp0;
    reg_edx = Temp1;
}

static int32_t Blood_AnimateOffs(int16_t TileIndex, int16_t FakeVar)
{
    int32_t Temp0 = reg_eax;
    int32_t Temp1 = reg_edx;

    reg_eax = TileIndex;
    reg_edx = FakeVar;
    Blood_Call(Blood_ProcAddress_AnimateOffs);
    int32_t Result = reg_eax;

    reg_eax = Temp0;
    reg_edx = Temp1;

    return Result;
}

static void Blood_FakeTimerHandler(void)
{
    Blood_Call(Blood_ProcAddress_FakeTimerHandler);
}

void Blood_SetAspect(int32_t daxrange, int32_t daaspect)
{
    Blood_ViewingRange = daxrange;
    Blood_ViewingRangeRecip = Blood_DivScale32(1, daxrange);

    Blood_YXAspect = daaspect;
    Blood_XYAspect = Blood_DivScale32(1, Blood_YXAspect);
    Blood_XDimenScale = Blood_Scale(Blood_XDimen, Blood_YXAspect, 320);
    Blood_XDimScale = Blood_Scale(320, Blood_XYAspect, Blood_XDimen);
}

void Blood_DoSetAspect(void)
{
    int32_t *HL1Ptr = (int32_t *)&MemBase[Blood_HorizLookup];
    int32_t *HL2Ptr = (int32_t *)&MemBase[Blood_HorizLookup2];

    if (Blood_XYAspect != Blood_OXYAspect)
    {
        Blood_OXYAspect = Blood_XYAspect;
        int32_t J = Blood_XYAspect * 320;

        HL2Ptr[Blood_HorizYCent - 1] = Blood_DivScale26(131072, J);

        for (int32_t I = Blood_YDim * 4 - 1; I >= 0; I--)
        {
            if (I != (Blood_HorizYCent - 1))
            {
                HL1Ptr[I] = Blood_DivScale28(1, I - (Blood_HorizYCent - 1));
                HL2Ptr[I] = Blood_DivScale14(Blood_KLAbs(HL1Ptr[I]), J);
            }
        }
    }

    if ((Blood_XDimen != Blood_OXDimen) ||
        (Blood_ViewingRange != Blood_OViewingRange))
    {
        Blood_OXDimen = Blood_XDimen;
        Blood_OViewingRange = Blood_ViewingRange;

        int32_t XInc = Blood_MulScale32(Blood_ViewingRange * 320,
                                        Blood_XDimenRecip);

        int32_t X = (640 << 16) - Blood_MulScale1(XInc, Blood_XDimen);

        for(int32_t I = 0; I < Blood_XDimen; I++)
        {
            int32_t J = (X & 65535);
            int32_t K = (X >> 16);

            X += XInc;

            if (J != 0)
                J = Blood_MulScale16((int32_t)Blood_RadarAng[K + 1] -
                                     (int32_t)Blood_RadarAng[K], J);

            Blood_RadarAng2[I] =
                (int16_t)(((int32_t)Blood_RadarAng[K] + J) >> 6);
        }

        for (int32_t I = 1; I < 16384; I++)
            Blood_DistRecip[I] = Blood_DivScale20(Blood_XDimen, I);

        Blood_NYTooClose = Blood_XDimen * 2100;
        Blood_NYTooFar = 16384 * 16384 - 1048576;
    }
}

void Blood_SetView(int32_t X1, int32_t Y1,
                   int32_t X2, int32_t Y2)
{
    Blood_WindowX1 = X1; Blood_WX1 = (X1 << 12);
    Blood_WindowY1 = Y1; Blood_WY1 = (Y1 << 12);
    Blood_WindowX2 = X2; Blood_WX2 = ((X2 + 1) << 12);
    Blood_WindowY2 = Y2; Blood_WY2 = ((Y2 + 1) << 12);

    Blood_XDimen = (X2 - X1) + 1;
    Blood_HalfXDimen = (Blood_XDimen >> 1);
    Blood_XDimenRecip = Blood_DivScale32(1, Blood_XDimen);
    Blood_YDimen = (Y2 - Y1) + 1;

    Blood_SetAspect(65536, Blood_DivScale16(Blood_YDim * 320, Blood_XDim * 200));

    for (int32_t i = 0; i < Blood_WindowX1; i++)
    {
        Blood_StartUMost[i] = 1;
        Blood_StartDMost[i] = 0;
    }

    for (int32_t i = Blood_WindowX1; i <= Blood_WindowX2; i++)
    {
        Blood_StartUMost[i] = Blood_WindowY1;
        Blood_StartDMost[i] = Blood_WindowY2 + 1;
    }

    for (int32_t i = Blood_WindowX2 + 1; i < Blood_XDim; i++)
    {
        Blood_StartUMost[i] = 1;
        Blood_StartDMost[i] = 0;
    }

    Blood_ViewOffset = Blood_WindowY1 * Blood_BytesPerLine + Blood_WindowX1;

    if (Blood_StereoMode || (Blood_VidOption == 6))
    {
        Blood_OStereoPixelWidth = Blood_StereoPixelWidth;
        Blood_XDimen = (Blood_WindowX2 - Blood_WindowX1) + 1 +
                       (Blood_StereoPixelWidth << 1);
        Blood_HalfXDimen = (Blood_XDimen >> 1);
        Blood_XDimenRecip = Blood_DivScale32(1, Blood_XDimen);
        Blood_SetAspect(
            Blood_DivScale16(Blood_XDimen, Blood_WindowX2 - Blood_WindowX1 + 1),
            Blood_YXAspect);
    }
}

void Blood_DrawRooms(int32_t daposx, int32_t daposy, int32_t daposz,
                     int16_t daang, int32_t dahoriz, int16_t dacursectnum)
{
    uint32_t Temp[4];

    Blood_BeforeDrawRooms = 0;
    Blood_TotalArea += ((Blood_WindowX2 + 1) - Blood_WindowX1) *
                       ((Blood_WindowY2 + 1) - Blood_WindowY1);

    Blood_GlobalPosX = daposx;
    Blood_GlobalPosY = daposy;
    Blood_GlobalPosZ = daposz;
    Blood_GlobalAng = (daang & 2047);

    Blood_GlobalHoriz = Blood_MulScale16(dahoriz - 100, Blood_XDimenScale) +
        (Blood_YDimen >> 1);
    Blood_GlobalUClip = (0 - Blood_GlobalHoriz) * Blood_XDimScale;
    Blood_GlobalDClip = (Blood_YDimen - Blood_GlobalHoriz) *
        Blood_XDimScale;

    int32_t I = Blood_MulScale16(Blood_XDimenScale, Blood_ViewingRangeRecip);
    Blood_GlobalPisibility = Blood_MulScale16(Blood_ParallaxVisibility, I);
    Blood_GlobalVisibility = Blood_MulScale16(Blood_Visibility, I);
    Blood_GlobalHisibility = Blood_MulScale16(Blood_GlobalVisibility, Blood_XYAspect);
    Blood_GlobalCisibility = Blood_MulScale16(Blood_GlobalHisibility, 320);

    Blood_GlobalCurSectNum = dacursectnum;
    Blood_TotalClockLock = Blood_TotalClock;

    Blood_CosGlobalAng = Blood_SinTable[(Blood_GlobalAng + 512) & 2047];
    Blood_SinGlobalAng = Blood_SinTable[(Blood_GlobalAng & 2047)];

    Blood_CosViewingRangeGlobalAng = Blood_MulScale16(Blood_CosGlobalAng,
                                                      Blood_ViewingRange);
    Blood_SinViewingRangeGlobalAng = Blood_MulScale16(Blood_SinGlobalAng,
                                                      Blood_ViewingRange);

    if ((Blood_StereoMode != 0) || (Blood_VidOption == 6))
    {
        if (Blood_StereoPixelWidth != Blood_OStereoPixelWidth)
        {
            Blood_OStereoPixelWidth = Blood_StereoPixelWidth;
            Blood_XDimen = (Blood_WindowX2 - Blood_WindowX1 + 1) +
                           (Blood_StereoPixelWidth << 1);
            Blood_HalfXDimen = (Blood_XDimen >> 1);
            Blood_XDimenRecip = Blood_DivScale32(1, Blood_XDimen);
            Blood_SetAspect(Blood_DivScale16(Blood_XDimen, Blood_WindowX2 - Blood_WindowX1 + 1),
                            Blood_YXAspect);
        }

        if (!(Blood_ActivePage & 1))
        {
            for (I = Blood_WindowX1;
                 I < Blood_WindowX1 + (Blood_StereoPixelWidth << 1);
                 I++)
            {
                Blood_StartUMost[I] = 1;
                Blood_StartDMost[I] = 0;
            }
            for (; I < Blood_WindowX2 + 1 + (Blood_StereoPixelWidth << 1);
                 I++)
            {
                Blood_StartUMost[I] = Blood_WindowY1;
                Blood_StartDMost[I] = Blood_WindowY2 + 1;
            }
            Blood_ViewOffset = Blood_WindowY1 * Blood_BytesPerLine +
                               Blood_WindowX1 - (Blood_StereoPixelWidth << 1);
            I = Blood_StereoWidth;
        }
        else
        {
            for (I = Blood_WindowX1;
                 I < Blood_WindowX2 + 1;
                 I++)
            {
                Blood_StartUMost[I] = Blood_WindowY1;
                Blood_StartDMost[I] = Blood_WindowY2 + 1;
            }
            for (; I < Blood_WindowX2 + 1 + (Blood_StereoPixelWidth << 1);
                 I++)
            {
                Blood_StartUMost[I] = 1;
                Blood_StartDMost[I] = 0;
            }
            Blood_ViewOffset = Blood_WindowY1 * Blood_BytesPerLine +
                               Blood_WindowX1;
            I = -Blood_StereoWidth;
        }

        Blood_GlobalPosX += Blood_MulScale24(Blood_SinGlobalAng, I);
        Blood_GlobalPosY -= Blood_MulScale24(Blood_CosGlobalAng, I);

        if (Blood_VidOption == 6)
        {
            /* Blood_FramePlace = FP_OFF(Blood_Screen) +
                                  (Blood_ActivePage & 1) * 65536; */
            Blood_FramePlace = Blood_Screen + ((Blood_ActivePage & 1) * 65536);
        }
    }

    if ((Blood_XYAspect != Blood_OXYAspect) ||
        (Blood_XDimen != Blood_OXDimen) ||
        (Blood_ViewingRange != Blood_OViewingRange))
    {
        Blood_DoSetAspect();
    }

    Blood_FrameOffset = Blood_FramePlace + Blood_ViewOffset;

#if 0
Blood_ClearBufferByte((char *)&MemBase[Blood_FramePlace], 64000, 0);
#endif

    Blood_ClearBufferByte(&Blood_GotSector[0],
        ((Blood_NumSectors + 7) >> 3), 0);

    int16_t *ShortPtr1 = &Blood_StartUMost[Blood_WindowX1];
    int16_t *ShortPtr2 = &Blood_StartDMost[Blood_WindowX1];
    I = Blood_XDimen - 1;
    do {
        Blood_UMost[I] = ShortPtr1[I] - Blood_WindowY1;
        Blood_DMost[I] = ShortPtr2[I] - Blood_WindowY1;
        I--;
    } while (I != 0);
    Blood_UMost[0] = ShortPtr1[0] - Blood_WindowY1;
    Blood_DMost[0] = ShortPtr2[0] - Blood_WindowY1;

    Blood_NumHits = Blood_XDimen;
    Blood_NumScans = 0;
    Blood_NumBunches = 0;
    Blood_MaskWallCount = 0;
    Blood_SMostWallCount = 0;
    Blood_SMostCount = 0;
    Blood_SpriteSortCount = 0;

    if (Blood_GlobalCurSectNum >= Blood_MaxSectors)
        Blood_GlobalCurSectNum -= Blood_MaxSectors;
    else
    {
        I = Blood_GlobalCurSectNum;
        Blood_UpdateSector(Blood_GlobalPosX, Blood_GlobalPosY,
                           &Blood_GlobalCurSectNum);
        if (Blood_GlobalCurSectNum < 0)
            Blood_GlobalCurSectNum = I;
    }

    Blood_GlobParaCeilClip = 1;
    Blood_GlobParaFlorClip = 1;

    int32_t cz, fz;
    Blood_GetZsOfSlope(Blood_GlobalCurSectNum, Blood_GlobalPosX,
                       Blood_GlobalPosY, &cz, &fz);
    if (Blood_GlobalPosZ < cz)
        Blood_GlobParaCeilClip = 0;
    if (Blood_GlobalPosZ > fz)
        Blood_GlobParaFlorClip = 0;

    Blood_ScanSector(Blood_GlobalCurSectNum);

    while ((Blood_NumBunches > 0) && (Blood_NumHits > 0))
    {
        Blood_ClearBuffer(&Blood_TempBuf[0], ((Blood_NumBunches + 3) >> 2), 0);
        Blood_TempBuf[0] = 1;

        int32_t Closest = 0;
        for (I = 0; I < Blood_NumBunches; I++)
        {
            int32_t J = Blood_BunchFront(I, Closest);
            if (J < 0) continue;
            Blood_TempBuf[I] = 1;
            if (J == 0) {
                Blood_TempBuf[Closest] = 1;
                Closest = I;
            }
        }

        for (I = 0; I < Blood_NumBunches; I++)
        {
            if (Blood_TempBuf[I]) continue;
            int32_t J = Blood_BunchFront(I, Closest);
            if (J < 0) continue;
            Blood_TempBuf[I] = 1;
            if (J == 0) {
                Blood_TempBuf[Closest] = 1;
                Closest = I;
                I = 0;
            }
        }

        Blood_DrawWalls(Closest);

        if (Blood_AutoMapping)
        {
            for (int32_t Z = Blood_BunchFirst[Closest];
                Z >= 0; Z = Blood_P2[Z])
            {
            Blood_Show2DWall[Blood_TheWall[Z] >> 3] |=
                Blood_Pow2Char[Blood_TheWall[Z] & 7];
            }
        }

        Blood_NumBunches--;
        Blood_BunchFirst[Closest] = Blood_BunchFirst[Blood_NumBunches];
        Blood_BunchLast[Closest] = Blood_BunchLast[Blood_NumBunches];
    }
}

void Blood_ScanSector(int16_t SectorIndex)
{
    if (SectorIndex < 0)
        return;

    if (Blood_AutoMapping)
        Blood_Show2DSector[SectorIndex >> 3] |= Blood_Pow2Char[SectorIndex & 7];

    Blood_SectorBorder[0] = SectorIndex;
    Blood_SectorBorderCount = 1;

    do
    {
        SectorIndex = Blood_SectorBorder[--Blood_SectorBorderCount];

        for (int16_t Z = Blood_HeadSpriteSect[SectorIndex];
             Z >= 0;
             Z = Blood_NextSpriteSect[Z])
        {
            Blood_Sprite_t *Sprite = &Blood_Sprites[Z];

            if ((Sprite->CStat & 0x8000) && !Blood_ShowInvisibility)
                continue;
            if (Sprite->XRepeat <= 0 || Sprite->YRepeat <= 0)
                continue;
            if (Blood_SpriteSortCount >= Blood_MaxSpritesOnScreen)
                continue;

            uint8_t WallOrFloorSprite = Sprite->CStat & 48;

            int32_t XS = Sprite->X - Blood_GlobalPosX;
            int32_t YS = Sprite->Y - Blood_GlobalPosY;
            int32_t Dot = XS * Blood_CosGlobalAng +
                          YS * Blood_SinGlobalAng;

            if (!WallOrFloorSprite && Dot <= 0)
                continue;

            Blood_Sprite_t *TSprite = &Blood_TSprites[Blood_TSpriteSortCount++];
            Blood_CopyBufferByte(Sprite, TSprite, sizeof(Blood_Sprite_t));
            TSprite->Owner = Z;
        }

        Blood_GotSector[SectorIndex >> 3] |= Blood_Pow2Char[SectorIndex & 7];

        int16_t BunchFirst = Blood_NumBunches;
        int16_t NumScansBefore = Blood_NumScans;

        int16_t StartWall = Blood_Sectors[SectorIndex].WallPtr;
        int16_t EndWall = StartWall + Blood_Sectors[SectorIndex].WallNum;
        int16_t ScanFirst = Blood_NumScans;

        int32_t XP1, YP1, XP2, YP2;

        Blood_Wall_t *Wall = &Blood_Walls[StartWall];
        for(int16_t Z = StartWall;
            Z < EndWall;
            Z++, Wall++)
        {
            Blood_Wall_t *Wall2 = &Blood_Walls[Wall->Point2];

            int32_t X1 = Wall->X - Blood_GlobalPosX;
            int32_t Y1 = Wall->Y - Blood_GlobalPosY;
            int32_t X2 = Wall2->X - Blood_GlobalPosX;
            int32_t Y2 = Wall2->Y - Blood_GlobalPosY;

            int16_t NextSectNum = Wall->NextSector;

            uint8_t OneWayWall = Wall->CStat & 32;
            if ((NextSectNum >= 0) && !OneWayWall)
            {
                if (!(Blood_GotSector[NextSectNum >> 3] &
                      Blood_Pow2Char[NextSectNum & 7]))
                {
                    int32_t Cross = X1 * Y2 - X2 * Y1;
                    if (((uint32_t)Cross + 262144) < 524288)
                    {
                        int32_t SquaredLength = (X2 - X1) * (X2 - X1) +
                                                (Y2 - Y1) * (Y2 - Y1);
                        if (Blood_MulScale5(Cross, Cross) <= SquaredLength)
                            Blood_SectorBorder[Blood_SectorBorderCount++] = NextSectNum;
                    }
                }
            }

            if ((Z == StartWall) || (Wall[Z - 1].Point2 !=Z))
            {
                XP1 = Blood_DMulScale6(Y1, Blood_CosGlobalAng,
                                       -X1, Blood_SinGlobalAng);
                YP1 = Blood_DMulScale6(X1, Blood_CosViewingRangeGlobalAng,
                                       Y1, Blood_SinViewingRangeGlobalAng);
            }
            else
            {
                XP1 = XP2;
                YP1 = YP2;
            }

            XP2 = Blood_DMulScale6(Y2, Blood_CosGlobalAng,
                                   -X2, Blood_SinGlobalAng);
            YP2 = Blood_DMulScale6(X2, Blood_CosViewingRangeGlobalAng,
                                   Y2, Blood_SinViewingRangeGlobalAng);

            if ((YP1 < 256) && (YP2 < 256))
                goto skipitaddwall;

            /* If the wall is not facing you */
            if (Blood_DMulScale32(XP1, YP2, -XP2, YP1) >= 0)
                goto skipitaddwall;

            if (XP1 >= -YP1)
            {
                if ((XP1 > YP1) || (YP1 == 0))
                    goto skipitaddwall;

                Blood_XB1[Blood_NumScans] = Blood_HalfXDimen +
                                            Blood_Scale(XP1, Blood_HalfXDimen, YP1);

                if (XP1 >= 0)
                    Blood_XB1[Blood_NumScans]++; /* Fix for signed divide */
                if (Blood_XB1[Blood_NumScans] >= Blood_XDimen)
                    Blood_XB1[Blood_NumScans] = Blood_XDimen - 1;
                Blood_YB1[Blood_NumScans] = YP1;
            }
            else
            {
                if (XP2 < -YP2)
                    goto skipitaddwall;

                Blood_XB1[Blood_NumScans] = 0;

                int32_t TempLong = YP1 - YP2 + XP1 - XP2;
                if (TempLong == 0)
                    goto skipitaddwall;

                Blood_YB1[Blood_NumScans] = YP1 + Blood_Scale(YP2 - YP1,
                                                              XP1 + YP1, TempLong);
            }

            if (Blood_YB1[Blood_NumScans] < 256)
                goto skipitaddwall;

            if (XP2 <= YP2)
            {
                if ((XP2 < -YP2) || (YP2 == 0))
                    goto skipitaddwall;

                Blood_XB2[Blood_NumScans] = Blood_HalfXDimen +
                                            Blood_Scale(XP2, Blood_HalfXDimen, YP2) - 1;
                if (XP2 >= 0)
                    Blood_XB2[Blood_NumScans]++; /* Fix for signed divide */
                if (Blood_XB2[Blood_NumScans] >= Blood_XDimen)
                    Blood_XB2[Blood_NumScans] = Blood_XDimen - 1;
                Blood_YB2[Blood_NumScans] = YP2;
            }
            else
            {
                if (XP1 > YP1)
                    goto skipitaddwall;

                Blood_XB2[Blood_NumScans] = Blood_XDimen - 1;

                int32_t TempLong = XP2 - XP1 + YP1 - YP2;
                if (TempLong == 0)
                    goto skipitaddwall;

                Blood_YB2[Blood_NumScans] = YP1 + Blood_Scale(YP2 - YP1,
                                                              YP1 - XP1, TempLong);
            }
            if ((Blood_YB2[Blood_NumScans] < 256) ||
                (Blood_XB1[Blood_NumScans] > Blood_XB2[Blood_NumScans]))
            {
                goto skipitaddwall;
            }

            Blood_TheSector[Blood_NumScans] = SectorIndex;
            Blood_TheWall[Blood_NumScans] = Z;
            Blood_RX1[Blood_NumScans] = XP1;
            Blood_RY1[Blood_NumScans] = YP1;
            Blood_RX2[Blood_NumScans] = XP2;
            Blood_RY2[Blood_NumScans] = YP2;
            Blood_P2[Blood_NumScans] = ++Blood_NumScans;

            skipitaddwall:

            if ((Blood_Walls[Z].Point2 < Z) && (ScanFirst < Blood_NumScans))
            {
                Blood_P2[Blood_NumScans - 1] = ScanFirst;
                ScanFirst = Blood_NumScans;
            }
        }

        for(int16_t Z = NumScansBefore;
            Z < Blood_NumScans;
            Z++)
        {
            if ((Blood_Walls[Blood_TheWall[Z]].Point2 != Blood_TheWall[Blood_P2[Z]]) ||
                (Blood_XB2[Z] >= Blood_XB1[Blood_P2[Z]]))
            {
                Blood_BunchFirst[Blood_NumBunches++] = Blood_P2[Z];
                Blood_P2[Z] = -1;
            }
        }

        for(int16_t Z = BunchFirst;
            Z < Blood_NumBunches;
            Z++)
        {
            int16_t ZZ;
            for (ZZ = Blood_BunchFirst[Z];
                 Blood_P2[ZZ] >= 0;
                 ZZ = Blood_P2[ZZ])
            {}
            Blood_BunchLast[Z] = ZZ;
        }
    }
    while (Blood_SectorBorderCount > 0);
}

void Blood_GetZsOfSlope(int16_t SectorIndex, int32_t DaX, int32_t DaY,
                        int32_t *CeilingZ, int32_t *FloorZ)
{
    Blood_Sector_t *Sector = &Blood_Sectors[SectorIndex];

    *CeilingZ = Sector->CeilingZ;
    *FloorZ = Sector->FloorZ;

    if ((Sector->CeilingStat | Sector->FloorStat) & 2)
    {
        Blood_Wall_t *Wall1 = &Blood_Walls[Sector->WallPtr];
        Blood_Wall_t *Wall2 = &Blood_Walls[Wall1->Point2];

        int32_t DX = Wall2->X - Wall1->X;
        int32_t DY = Wall2->Y - Wall1->Y;

        int32_t i = (Blood_NSqrtAsm(DX * DX + DY * DY) << 5);
        if (i == 0)
            return;

        int32_t j = Blood_DMulScale3(DX, DaY - Wall1->Y, -DY, DaX - Wall1->X);

        if (Sector->CeilingStat & 2)
            *CeilingZ = (*CeilingZ) + Blood_Scale(Sector->CeilingHeiNum, j, i);
        if (Sector->FloorStat & 2)
            *FloorZ = (*FloorZ) + Blood_Scale(Sector->FloorHeiNum, j, i);
    }
}

int32_t Blood_Inside(int32_t X, int32_t Y, int16_t SectorIndex)
{
    if ((SectorIndex < 0) || (SectorIndex >= Blood_NumSectors))
        return -1;

    uint32_t Cnt = 0;
    Blood_Wall_t *Wall = &Blood_Walls[Blood_Sectors[SectorIndex].WallPtr];
    int32_t I = Blood_Sectors[SectorIndex].WallNum;
    do
    {
        int32_t Y1 = Wall->Y - Y;
        int32_t Y2 = Blood_Walls[Wall->Point2].Y - Y;

        if ((Y1 ^ Y2) < 0)
        {
            int32_t X1 = Wall->X - X;
            int32_t X2 = Blood_Walls[Wall->Point2].X - X;

            if ((X1 ^ X2) >= 0)
                Cnt ^= X1;
            else
                Cnt ^= (X1 * Y2 - X2 * Y1) ^ Y2;
        }
        Wall++; I--;
    }
    while (I);

    return (Cnt >> 31);
}

void Blood_UpdateSector(int32_t X, int32_t Y, int16_t *SectorIndex)
{
    if (Blood_Inside(X, Y, *SectorIndex) == 1)
        return;

    if ((*SectorIndex >= 0) && (*SectorIndex < Blood_NumSectors))
    {
        Blood_Sector_t *Sector = &Blood_Sectors[*SectorIndex];
        Blood_Wall_t *Wall = &Blood_Walls[Sector->WallPtr];
        int32_t J = Sector->WallNum;
        do
        {
            int32_t I = Wall->NextSector;
            if (I >= 0)
            {
                if (Blood_Inside(X, Y, I) == 1)
                {
                    *SectorIndex = I;
                    return;
                }
            }
            Wall++; J--;
        }
        while (J != 0);
    }

    for (int32_t I = Blood_NumSectors - 1; I >= 0; I--)
    {
        if (Blood_Inside(X, Y, I) == 1)
        {
            *SectorIndex = I;
            return;
        }
    }

    *SectorIndex = -1;
}

void Blood_DrawWalls(int32_t BunchIndex)
{
    int32_t Z = Blood_BunchFirst[BunchIndex];
    int32_t SectorIndex = Blood_TheSector[Z];
    Blood_Sector_t *Sector = &Blood_Sectors[SectorIndex];

    uint8_t AndWStat1 = 0xff;
    uint8_t AndWStat2 = 0xff;
    for (; Z >= 0; Z = Blood_P2[Z])
    {
        AndWStat1 &= Blood_WallMost(Blood_UPlc, Z, SectorIndex, 0);
        AndWStat2 &= Blood_WallMost(Blood_DPlc, Z, SectorIndex, 1);
    }

    if ((AndWStat1 & 3) != 3)
    {
        if ((Sector->CeilingStat & 3) == 2)
        {
            Blood_GrouScan(Blood_XB1[Blood_BunchFirst[BunchIndex]],
                           Blood_XB2[Blood_BunchLast[BunchIndex]],
                           SectorIndex, 0);
        }
        else if ((Sector->CeilingStat & 1) == 0)
        {
            Blood_CeilScan(Blood_XB1[Blood_BunchFirst[BunchIndex]],
                           Blood_XB2[Blood_BunchLast[BunchIndex]],
                           SectorIndex);
        }
        else
        {
            Blood_ParaScan(Blood_XB1[Blood_BunchFirst[BunchIndex]],
                           Blood_XB2[Blood_BunchLast[BunchIndex]],
                           SectorIndex, 0, BunchIndex);
        }
    }

    if ((AndWStat2 & 12) != 12)
    {
        if ((Sector->FloorStat & 3) == 2)
        {
            Blood_GrouScan(Blood_XB1[Blood_BunchFirst[BunchIndex]],
                           Blood_XB2[Blood_BunchLast[BunchIndex]],
                           SectorIndex, 1);
        }
        else if ((Sector->FloorStat & 1) == 0)
        {
            Blood_FlorScan(Blood_XB1[Blood_BunchFirst[BunchIndex]],
                           Blood_XB2[Blood_BunchLast[BunchIndex]],
                           SectorIndex);
        }
        else
        {
            Blood_ParaScan(Blood_XB1[Blood_BunchFirst[BunchIndex]],
                           Blood_XB2[Blood_BunchLast[BunchIndex]],
                           SectorIndex, 1, BunchIndex);
        }
    }

    /* Draw walls section */
    for (Z = Blood_BunchFirst[BunchIndex];
         Z >= 0; Z = Blood_P2[Z])
    {
        int32_t X1 = Blood_XB1[Z];
        int32_t X2 = Blood_XB2[Z];

        if (Blood_UMost[X2] >= Blood_DMost[X2])
        {
            int32_t X;
            for(X = X1; X < X2; X++)
            {
                if (Blood_UMost[X] < Blood_DMost[X])
                    break;
            }

            if (X >= X2)
            {
                Blood_SMostWall[Blood_SMostWallCount] = Z;
                Blood_SMostWallType[Blood_SMostWallCount] = 0;
                Blood_SMostWallCount++;
                continue;
            }
        }

        int32_t WallIndex = Blood_TheWall[Z];
        Blood_Wall_t *Wall = &Blood_Walls[WallIndex];

        int32_t NextSectorIndex = Wall->NextSector;
        Blood_Sector_t *NextSector = &Blood_Sectors[NextSectorIndex];

        int32_t GotSWall = 0;
        int32_t StartSMostWallCount = Blood_SMostWallCount;
        int32_t StartSMostCount = Blood_SMostCount;

        if ((Blood_SearchIt == 2) && (Blood_SearchX >= X1) &&
            (Blood_SearchX <= X2))
        {
            /* Ceiling */
            if (Blood_SearchY <= Blood_UPlc[Blood_SearchX])
            {
                Blood_SearchSector = SectorIndex;
                Blood_SearchWall = WallIndex;
                Blood_SearchStat = 1;
                Blood_SearchIt = 1;
            }
            /* Floor */
            else if (Blood_SearchY >= Blood_DPlc[Blood_SearchX])
            {
                Blood_SearchSector = SectorIndex;
                Blood_SearchWall = WallIndex;
                Blood_SearchStat = 2;
                Blood_SearchIt = 1;
            }
        }

        if (NextSectorIndex >= 0)
        {
            int32_t CeilZs[5], FloorZs[5];

            Blood_GetZsOfSlope(SectorIndex, Wall->X, Wall->Y, &CeilZs[0], &FloorZs[0]);
            Blood_GetZsOfSlope(SectorIndex, Blood_Walls[Wall->Point2].X,
                               Blood_Walls[Wall->Point2].Y, &CeilZs[1], &FloorZs[1]);
            Blood_GetZsOfSlope(NextSectorIndex, Wall->X, Wall->Y, &CeilZs[2], &FloorZs[2]);
            Blood_GetZsOfSlope(NextSectorIndex, Blood_Walls[Wall->Point2].X,
                               Blood_Walls[Wall->Point2].Y, &CeilZs[3], &FloorZs[3]);
            Blood_GetZsOfSlope(NextSectorIndex, Blood_GlobalPosX, Blood_GlobalPosY,
                               &CeilZs[4], &FloorZs[4]);

            if ((Wall->CStat & 48) == 16)
                Blood_MaskWall[Blood_MaskWallCount++] = Z;

            if (((Sector->CeilingStat & 1) == 0) || ((NextSector->CeilingStat & 1) == 0))
            {
                if ((CeilZs[2] <= CeilZs[0]) && (CeilZs[3] <= CeilZs[1]))
                {
                    if (Blood_GlobParaCeilClip)
                    {
                        for (int32_t X = X1; X <= X2; X++)
                        {
                            if (Blood_UPlc[X] > Blood_UMost[X])
                            {
                                if (Blood_UMost[X] <= Blood_DMost[X])
                                {
                                    Blood_UMost[X] = Blood_UPlc[X];
                                    if (Blood_UMost[X] > Blood_DMost[X])
                                        Blood_NumHits--;
                                }
                            }
                        }
                    }
                }
                else
                {
                    Blood_WallMost(Blood_DWall, Z, NextSectorIndex, 0);

                    if ((CeilZs[2] > FloorZs[0]) || (CeilZs[3] > FloorZs[1]))
                    {
                        for (int32_t I = X1; I <= X2; I++)
                        {
                            if (Blood_DWall[I] > Blood_DPlc[I])
                                Blood_DWall[I] = Blood_DPlc[I];
                        }
                    }

                    if ((Blood_SearchIt == 2) && (Blood_SearchX >= X1) &&
                        (Blood_SearchX <= X2))
                    {
                        /* Wall */
                        if (Blood_SearchY <= Blood_DWall[Blood_SearchX])
                        {
                            Blood_SearchSector = SectorIndex;
                            Blood_SearchWall = WallIndex;
                            Blood_SearchStat = 0;
                            Blood_SearchIt = 1;
                        }
                    }

                    Blood_GlobalOrientation = Wall->CStat;
                    Blood_GlobalPicNum = Wall->PicNum;

                    if ((uint32_t)Blood_GlobalPicNum >= (uint32_t)Blood_MaxTiles)
                        Blood_GlobalPicNum = 0;

                    Blood_GlobalXPanning = Wall->XPanning;
                    Blood_GlobalYPanning = Wall->YPanning;
                    Blood_GlobalShiftVal = (Blood_PicSize[Blood_GlobalPicNum] >> 4);

                    if (Blood_Pow2Long[Blood_GlobalShiftVal] != Blood_TileSizeY[Blood_GlobalPicNum])
                        Blood_GlobalShiftVal++;

                    Blood_GlobalShiftVal = 32 - Blood_GlobalShiftVal;

                    if (Blood_PicAnm[Blood_GlobalPicNum] & 192)
                        Blood_GlobalPicNum += Blood_AnimateOffs(Blood_GlobalPicNum, WallIndex + 16384);

                    Blood_GlobalShade = Wall->Shade;
                    Blood_GlobVis = Blood_GlobalVisibility;

                    if (Sector->Visibility != 0)
                        Blood_GlobVis = Blood_MulScale4(Blood_GlobVis,
                                                        (int32_t)((uint8_t)(Sector->Visibility + 16)));

                    Blood_GlobalPal = Wall->Pal;
                    Blood_GlobalYScale = (Wall->YRepeat << (Blood_GlobalShiftVal - 19));

                    if ((Blood_GlobalOrientation & 4) == 0)
                        Blood_GlobalZD = (((Blood_GlobalPosZ - NextSector->CeilingZ) * Blood_GlobalYScale) << 8);
                    else
                        Blood_GlobalZD = (((Blood_GlobalPosZ - Sector->CeilingZ) * Blood_GlobalYScale) << 8);

                    Blood_GlobalZD += (Blood_GlobalYPanning << 24);

                    if (Blood_GlobalOrientation & 256)
                    {
                        Blood_GlobalYScale = -Blood_GlobalYScale;
                        Blood_GlobalZD = -Blood_GlobalZD;
                    }

                    if (GotSWall == 0)
                    {
                        GotSWall = 1;
                        Blood_PrepWall(Z, Wall);
                    }

                    Blood_WallScan(X1, X2, Blood_UPlc, Blood_DWall, Blood_SWall, Blood_LWall);

                    if ((CeilZs[2] >= CeilZs[0]) && (CeilZs[3] >= CeilZs[1]))
                    {
                        for(int32_t X = X1; X <= X2; X++)
                        {
                            if (Blood_DWall[X] > Blood_UMost[X])
                            {
                                if (Blood_UMost[X] <= Blood_DMost[X])
                                {
                                    Blood_UMost[X] = Blood_DWall[X];
                                    if (Blood_UMost[X] > Blood_DMost[X])
                                        Blood_NumHits--;
                                }
                            }
                        }
                    }
                    else
                    {
                        for (int32_t X = X1; X <= X2; X++)
                        {
                            if (Blood_UMost[X] <= Blood_DMost[X])
                            {
                                int32_t I = Blood_Max(Blood_UPlc[X], Blood_DWall[X]);
                                if (I > Blood_UMost[X])
                                {
                                    Blood_UMost[X] = I;
                                    if (Blood_UMost[X] > Blood_DMost[X])
                                        Blood_NumHits--;
                                }
                            }
                        }
                    }
                }

                if ((CeilZs[2] < CeilZs[0]) || (CeilZs[3] < CeilZs[1]) ||
                    (Blood_GlobalPosZ < CeilZs[4]))
                {
                    int32_t I = X2 - X1 + 1;
                    if (Blood_SMostCount + I < Blood_MaxYSaves)
                    {
                        Blood_SMostStart[Blood_SMostWallCount] = Blood_SMostCount;
                        Blood_SMostWall[Blood_SMostWallCount] = Z;
                        Blood_SMostWallType[Blood_SMostWallCount] = 1; /* 1 for umost */
                        Blood_SMostWallCount++;
                        Blood_CopyBufferByte(&Blood_UMost[X1], &Blood_SMost[Blood_SMostCount],
                                             I * sizeof(Blood_SMost[0]));
                        Blood_SMostCount += I;
                    }
                }
            }

            if (((Sector->FloorStat & 1) == 0) || ((NextSector->FloorStat & 1) == 0))
            {
                if ((FloorZs[2] >= FloorZs[0]) && (FloorZs[3] >= FloorZs[1]))
                {
                    if (Blood_GlobParaFlorClip)
                    {
                        for (int32_t X = X1; X <= X2; X++)
                        {
                            if (Blood_DPlc[X] < Blood_DMost[X])
                            {
                                if (Blood_UMost[X] <= Blood_DMost[X])
                                {
                                    Blood_DMost[X] = Blood_DPlc[X];
                                    if (Blood_UMost[X] > Blood_DMost[X])
                                        Blood_NumHits--;
                                }
                            }
                        }
                    }
                }
                else
                {
                    Blood_WallMost(Blood_UWall, Z, NextSectorIndex, 1);

                    if ((FloorZs[2] < CeilZs[0]) || (FloorZs[3] < CeilZs[1]))
                    {
                        for (int32_t I = X1; I <= X2; I++)
                        {
                            if (Blood_UWall[I] < Blood_UPlc[I])
                                Blood_UWall[I] = Blood_UPlc[I];
                        }
                    }

                    if ((Blood_SearchIt == 2) && (Blood_SearchX >= X1) &&
                        (Blood_SearchX <= X2))
                    {
                        /* Wall */
                        if (Blood_SearchY >= Blood_UWall[Blood_SearchX])
                        {
                            Blood_SearchSector = SectorIndex;
                            Blood_SearchWall = WallIndex;
                            if ((Wall->CStat & 2) > 0)
                                Blood_SearchWall = Wall->NextWall;
                            Blood_SearchStat = 0;
                            Blood_SearchIt = 1;
                        }
                    }

                    if ((Wall->CStat & 2) > 0)
                    {
                        WallIndex = Wall->NextWall;
                        Wall = &Blood_Walls[WallIndex];

                        Blood_GlobalOrientation = Wall->CStat;
                        Blood_GlobalPicNum = Wall->PicNum;

                        if ((uint32_t)Blood_GlobalPicNum >= (uint32_t)Blood_MaxTiles)
                            Blood_GlobalPicNum = 0;

                        Blood_GlobalXPanning = Wall->XPanning;
                        Blood_GlobalYPanning = Wall->YPanning;

                        if (Blood_PicAnm[Blood_GlobalPicNum] & 192)
                            Blood_GlobalPicNum += Blood_AnimateOffs(Blood_GlobalPicNum, WallIndex + 16384);

                        Blood_GlobalShade = Wall->Shade;
                        Blood_GlobalPal = Wall->Pal;

                        WallIndex = Blood_TheWall[Z];
                        Wall = &Blood_Walls[WallIndex];
                    }
                    else
                    {
                        Blood_GlobalOrientation = Wall->CStat;
                        Blood_GlobalPicNum = Wall->PicNum;

                        if ((uint32_t)Blood_GlobalPicNum >= (uint32_t)Blood_MaxTiles)
                            Blood_GlobalPicNum = 0;

                        Blood_GlobalXPanning = Wall->XPanning;
                        Blood_GlobalYPanning = Wall->YPanning;

                        if (Blood_PicAnm[Blood_GlobalPicNum] & 192)
                            Blood_GlobalPicNum += Blood_AnimateOffs(Blood_GlobalPicNum, WallIndex + 16384);

                        Blood_GlobalShade = Wall->Shade;
                        Blood_GlobalPal = Wall->Pal;
                    }

                    Blood_GlobVis = Blood_GlobalVisibility;

                    if (Sector->Visibility != 0)
                        Blood_GlobVis = Blood_MulScale4(Blood_GlobVis,
                                                        (int32_t)((uint8_t)(Sector->Visibility + 16)));

                    Blood_GlobalShiftVal = (Blood_PicSize[Blood_GlobalPicNum] >> 4);

                    if (Blood_Pow2Long[Blood_GlobalShiftVal] != Blood_TileSizeY[Blood_GlobalPicNum])
                        Blood_GlobalShiftVal++;

                    Blood_GlobalShiftVal = 32 - Blood_GlobalShiftVal;
                    Blood_GlobalYScale = (Wall->YRepeat << (Blood_GlobalShiftVal - 19));

                    if ((Blood_GlobalOrientation & 4) == 0)
                        Blood_GlobalZD = (((Blood_GlobalPosZ - NextSector->FloorZ) * Blood_GlobalYScale) << 8);
                    else
                        Blood_GlobalZD = (((Blood_GlobalPosZ - Sector->CeilingZ) * Blood_GlobalYScale) << 8);

                    Blood_GlobalZD += (Blood_GlobalYPanning << 24);

                    if (Blood_GlobalOrientation & 256)
                    {
                        Blood_GlobalYScale = -Blood_GlobalYScale;
                        Blood_GlobalZD = -Blood_GlobalZD;
                    }

                    if (GotSWall == 0)
                    {
                        GotSWall = 1;
                        Blood_PrepWall(Z, Wall);
                    }

                    Blood_WallScan(X1, X2, Blood_UWall, Blood_DPlc, Blood_SWall, Blood_LWall);

                    if ((FloorZs[2] <= FloorZs[0]) && (FloorZs[3] <= FloorZs[1]))
                    {
                        for (int32_t X = X1; X <= X2; X++)
                        {
                            if (Blood_UWall[X] < Blood_DMost[X])
                            {
                                if (Blood_UMost[X] <= Blood_DMost[X])
                                {
                                    Blood_DMost[X] = Blood_UWall[X];
                                    if (Blood_UMost[X] > Blood_DMost[X])
                                        Blood_NumHits--;
                                }
                            }
                        }
                    }
                    else
                    {
                        for (int32_t X = X1; X <= X2; X++)
                        {
                            if (Blood_UMost[X] <= Blood_DMost[X])
                            {
                                int32_t I = Blood_Min(Blood_DPlc[X], Blood_UWall[X]);
                                if (I < Blood_DMost[X])
                                {
                                    Blood_DMost[X] = I;
                                    if (Blood_UMost[X] > Blood_DMost[X])
                                        Blood_NumHits--;
                                }
                            }
                        }
                    }
                }

                if ((FloorZs[2] > FloorZs[0]) || (FloorZs[3] > FloorZs[1]) ||
                    (Blood_GlobalPosZ > FloorZs[4]))
                {
                    int32_t I = X2 - X1 + 1;
                    if (Blood_SMostCount + I < Blood_MaxYSaves)
                    {
                        Blood_SMostStart[Blood_SMostWallCount] = Blood_SMostCount;
                        Blood_SMostWall[Blood_SMostWallCount] = Z;
                        Blood_SMostWallType[Blood_SMostWallCount] = 2; /* 2 for dmost */
                        Blood_SMostWallCount++;
                        Blood_CopyBufferByte(&Blood_DMost[X1], &Blood_SMost[Blood_SMostCount],
                                             I * sizeof(Blood_SMost[0]));
                        Blood_SMostCount += I;
                    }
                }
            }

            if (Blood_NumHits < 0)
                return;

            if ((!(Wall->CStat & 32)) &&
                ((Blood_GotSector[NextSectorIndex >> 3] &
                 Blood_Pow2Char[NextSectorIndex & 7]) == 0))
            {
                if (Blood_UMost[X2] < Blood_DMost[X2])
                {
                    Blood_ScanSector(NextSectorIndex);
                }
                else
                {
                    int32_t X;
                    for (X = X1; X < X2; X++)
                    {
                        if (Blood_UMost[X] < Blood_DMost[X])
                        {
                            Blood_ScanSector(NextSectorIndex);
                            break;
                        }
                    }

                    /* If cannot see sector beyond, then cancel smost array and just
                       store wall! */
                    if (X == X2)
                    {
                        Blood_SMostWallCount = StartSMostWallCount;
                        Blood_SMostCount = StartSMostCount;
                        Blood_SMostWall[Blood_SMostWallCount] = Z;
                        Blood_SMostWallType[Blood_SMostWallCount] = 0;
                        Blood_SMostWallCount++;
                    }
                }
            }
        }

        /* White or 1-way wall */
        if ((NextSectorIndex < 0) || (Wall->CStat & 32))
        {
            Blood_GlobalOrientation = Wall->CStat;

            if (NextSectorIndex < 0)
                Blood_GlobalPicNum = Wall->PicNum;
            else
                Blood_GlobalPicNum = Wall->OverPicNum;

            if ((uint32_t)Blood_GlobalPicNum >= (uint32_t)Blood_MaxTiles)
                Blood_GlobalPicNum = 0;

            Blood_GlobalXPanning = (int32_t)Wall->XPanning;
            Blood_GlobalYPanning = (int32_t)Wall->YPanning;

            if (Blood_PicAnm[Blood_GlobalPicNum] & 192)
                Blood_GlobalPicNum += Blood_AnimateOffs(Blood_GlobalPicNum, WallIndex + 16384);

            Blood_GlobalShade = Wall->Shade;
            Blood_GlobVis = Blood_GlobalVisibility;

            if (Sector->Visibility != 0)
                Blood_GlobVis = Blood_MulScale4(Blood_GlobVis,
                                                (int32_t)((uint8_t)(Sector->Visibility + 16)));

            Blood_GlobalPal = Wall->Pal;
            Blood_GlobalShiftVal = (Blood_PicSize[Blood_GlobalPicNum] >> 4);

            if (Blood_Pow2Long[Blood_GlobalShiftVal] != Blood_TileSizeY[Blood_GlobalPicNum])
                Blood_GlobalShiftVal++;

            Blood_GlobalShiftVal = 32 - Blood_GlobalShiftVal;
            Blood_GlobalYScale = (Wall->YRepeat << (Blood_GlobalShiftVal - 19));

            if (NextSectorIndex >= 0)
            {
                if ((Blood_GlobalOrientation & 4) == 0)
                    Blood_GlobalZD = Blood_GlobalPosZ - NextSector->CeilingZ;
                else
                    Blood_GlobalZD = Blood_GlobalPosZ - Sector->CeilingZ;
            }
            else
            {
                if ((Blood_GlobalOrientation & 4) == 0)
                    Blood_GlobalZD = Blood_GlobalPosZ - Sector->CeilingZ;
                else
                    Blood_GlobalZD = Blood_GlobalPosZ - Sector->FloorZ;
            }

            Blood_GlobalZD = ((Blood_GlobalZD * Blood_GlobalYScale) << 8) +
                             (Blood_GlobalYPanning << 24);

            if (Blood_GlobalOrientation & 256)
            {
                Blood_GlobalYScale = -Blood_GlobalYScale;
                Blood_GlobalZD = -Blood_GlobalZD;
            }

            if (GotSWall == 0)
            {
                GotSWall = 1;
                Blood_PrepWall(Z, Wall);
            }

            Blood_WallScan(X1, X2, Blood_UPlc, Blood_DPlc, Blood_SWall, Blood_LWall);

            for (int32_t X = X1; X <= X2; X++)
            {
                if (Blood_UMost[X] <= Blood_DMost[X])
                {
                    Blood_UMost[X] = 1;
                    Blood_DMost[X] = 0;
                    Blood_NumHits--;
                }
            }

            Blood_SMostWall[Blood_SMostWallCount] = Z;
            Blood_SMostWallType[Blood_SMostWallCount] = 0;
            Blood_SMostWallCount++;

            if ((Blood_SearchIt == 2) && (Blood_SearchX >= X1) &&
                (Blood_SearchX <= X2))
            {
                Blood_SearchIt = 1;
                Blood_SearchSector = SectorIndex;
                Blood_SearchWall = WallIndex;

                if (NextSectorIndex < 0)
                    Blood_SearchStat = 0;
                else
                    Blood_SearchStat = 4;
            }
        }
    }
}

int32_t Blood_VLine1_LogY;

static void Blood_SetupVLineAsm(int32_t NegLogY)
{
    Blood_VLine1_LogY = NegLogY;
}

static int32_t Blood_VLineAsm1(int32_t VInc, int32_t PalOffs, int32_t Count,
                               uint32_t VPlc, int32_t BufPlc, int32_t P)
{
    uint8_t *Mem = (uint8_t *)MemBase;

    uint8_t *gbuf = Mem + BufPlc;
    uint8_t *gpal = Mem + PalOffs;

    for (; Count >= 0; Count--)
    {
        uint8_t bl = gpal[gbuf[(VPlc >> Blood_VLine1_LogY)]];

        mem_writeb(P, bl);

        P += Blood_BytesPerLine;
        VPlc += VInc;
    }

    return VPlc;
}

void Blood_WallScan(int32_t X1, int32_t X2, int16_t *UWall, int16_t *DWall,
                    int32_t *SWall, int32_t *LWall)
{
    int32_t TileSizeX = Blood_TileSizeX[Blood_GlobalPicNum];
    int32_t TileSizeY = Blood_TileSizeY[Blood_GlobalPicNum];

    Blood_SetGotPic(Blood_GlobalPicNum);

    if ((TileSizeX <= 0) || (TileSizeY <= 0))
        return;
    if ((UWall[X1] > Blood_YDimen) && (UWall[X2] > Blood_YDimen))
        return;
    if ((DWall[X1] < 0) && (DWall[X2] < 0))
        return;

    if (Blood_WallOff[Blood_GlobalPicNum] == 0)
        Blood_LoadTile(Blood_GlobalPicNum);

    int32_t XNice = (Blood_Pow2Long[Blood_PicSize[Blood_GlobalPicNum] & 15] == TileSizeX);
    if (XNice)
        TileSizeX--;

    int32_t YNice = (Blood_Pow2Long[Blood_PicSize[Blood_GlobalPicNum] >> 4] == TileSizeY);
    if (YNice)
        TileSizeY = (Blood_PicSize[Blood_GlobalPicNum] >> 4);

    int32_t FPalLookup = Blood_PalLookup[Blood_GlobalPal];
    /*int32_t FPalLookup = FP_OFF(Blood_PalLookup[Blood_GlobalPal]);*/

    Blood_SetupVLineAsm(Blood_GlobalShiftVal);

    int32_t Y1VE, Y2VE;
    int32_t X = X1;

    for (X = X1; X <= X2; X++)
    {
        Y1VE = Blood_Max(UWall[X], Blood_UMost[X]);
        Y2VE = Blood_Min(DWall[X], Blood_DMost[X]);

        if (Y2VE <= Y1VE)
            continue;

        Blood_PalLookupOffset[0] = FPalLookup +
            (Blood_GetPalLookup(Blood_MulScale16(SWall[X], Blood_GlobVis), Blood_GlobalShade) << 8);

        Blood_BufPlce[0] = LWall[X] + Blood_GlobalXPanning;

        if (Blood_BufPlce[0] >= TileSizeX)
        {
            if (XNice == 0)
                Blood_BufPlce[0] %= TileSizeX;
            else
                Blood_BufPlce[0] &= TileSizeX;
        }

        if (YNice == 0)
            Blood_BufPlce[0] *= TileSizeY;
        else
            Blood_BufPlce[0] <<= TileSizeY;

        Blood_Vince[0] = SWall[X] * Blood_GlobalYScale;
        Blood_VPlce[0] = Blood_GlobalZD + Blood_Vince[0] * (Y1VE - Blood_GlobalHoriz + 1);

        Blood_VLineAsm1(Blood_Vince[0], Blood_PalLookupOffset[0],
                        Y2VE - Y1VE - 1, Blood_VPlce[0],
                        Blood_BufPlce[0] + Blood_WallOff[Blood_GlobalPicNum],
                        X + Blood_FrameOffset + Blood_YLookup[Y1VE]);
    }

    Blood_FakeTimerHandler();
}

static int32_t         Blood_HLine4_BXInc;
static int32_t         Blood_HLine4_BYInc;
static int32_t         Blood_HLine4_LogX;
static int32_t         Blood_HLine4_LogY;
static uint32_t        Blood_HLine4_BufPlc;
static Blood_Pointer_t Blood_HLine4_Pal;

static void Blood_SetHLineSizes(int32_t LogX, int32_t LogY, uint32_t BufPlc)
{
    Blood_HLine4_LogX = LogX;
    Blood_HLine4_LogY = LogY;
    Blood_HLine4_BufPlc = BufPlc;
}

static void Blood_SetPalLookupAddress(Blood_Pointer_t PalAddr)
{
    Blood_HLine4_Pal = PalAddr;
}

static void Blood_SetupHLineAsm4(int32_t BXInc, int32_t BYInc)
{
    Blood_HLine4_BXInc = BXInc;
    Blood_HLine4_BYInc = BYInc;
}

static void Blood_HLineAsm4(int32_t Count, uint32_t PalOffs,
                            uint32_t BY, uint32_t BX, uint32_t P)
{
    uint8_t *PalPtr = (uint8_t *)&MemBase[Blood_HLine4_Pal + PalOffs];
    uint8_t *BufPtr = (uint8_t *)&MemBase[Blood_HLine4_BufPlc];

    for (; Count >= 0; Count--)
    {
        uint32_t BufIndex =
            ((BX >> (32 - Blood_HLine4_LogX)) << Blood_HLine4_LogY) +
             (BY >> (32 - Blood_HLine4_LogY));

        uint8_t Byte = PalPtr[BufPtr[BufIndex]];

        /* *((uint8_t *)P) = byte; */
        mem_writeb(P, Byte);

        BX -= Blood_HLine4_BXInc;
        BY -= Blood_HLine4_BYInc;
        P--;
    }
}

static void Blood_HLine(int32_t XR, int32_t YP)
{
    int32_t XL = Blood_LastX[YP];
    if (XL > XR)
        return;

    uint8_t *Mem = (uint8_t *)MemBase;
    int32_t *Ptr = (int32_t *)(&Mem[Blood_HorizLookup2]);
    int32_t R = Ptr[YP - Blood_GlobalHoriz + Blood_HorizYCent];

    Blood_SetupHLineAsm4(Blood_GlobalX1 * R, Blood_GlobalY2 * R);

    int32_t S = Blood_GetPalLookup(Blood_MulScale16(R, Blood_GlobVis),
                                   Blood_GlobalShade) << 8;

    Blood_HLineAsm4(XR - XL, S, Blood_GlobalX2 * R + Blood_GlobalYPanning,
                    Blood_GlobalY1 * R + Blood_GlobalXPanning,
                    Blood_YLookup[YP] + XR + Blood_FrameOffset);
}

void Blood_FlorScan(int32_t X1, int32_t X2, int32_t SectorIndex)
{
    Blood_Sector_t *Sector = &Blood_Sectors[SectorIndex];

    Blood_GlobalPalWritten = Blood_PalLookup[Sector->FloorPal];
    Blood_SetPalLookupAddress(Blood_GlobalPalWritten);

    Blood_GlobalZD = Blood_GlobalPosZ - Sector->FloorZ;
    if (Blood_GlobalZD > 0)
        return;

    Blood_GlobalPicNum = Sector->FloorPicNum;
    if ((uint32_t)Blood_GlobalPicNum >= (uint32_t)Blood_MaxTiles)
        Blood_GlobalPicNum = 0;

    Blood_SetGotPic(Blood_GlobalPicNum);

    if ((Blood_TileSizeX[Blood_GlobalPicNum] <= 0) ||
        (Blood_TileSizeY[Blood_GlobalPicNum] <= 0))
    {
        return;
    }

    if (Blood_PicAnm[Blood_GlobalPicNum] & 192)
        Blood_GlobalPicNum += Blood_AnimateOffs(Blood_GlobalPicNum, SectorIndex);

    if (Blood_WallOff[Blood_GlobalPicNum] == 0)
        Blood_LoadTile(Blood_GlobalPicNum);

    Blood_GlobalBufPlc = Blood_WallOff[Blood_GlobalPicNum];

    Blood_GlobalShade = Sector->FloorShade;
    Blood_GlobVis = Blood_GlobalCisibility;

    if (Sector->Visibility != 0)
    {
        Blood_GlobVis =
            Blood_MulScale4(Blood_GlobVis,
                            (int32_t)((uint8_t)(Sector->Visibility + 16)));
    }

    Blood_GlobalOrientation = Sector->FloorStat;

    if ((Blood_GlobalOrientation & 64) == 0)
    {
        Blood_GlobalX1 = Blood_SinGlobalAng;
        Blood_GlobalX2 = Blood_SinGlobalAng;
        Blood_GlobalY1 = Blood_CosGlobalAng;
        Blood_GlobalY2 = Blood_CosGlobalAng;
        Blood_GlobalXPanning = (Blood_GlobalPosX << 20);
        Blood_GlobalYPanning = -(Blood_GlobalPosY << 20);
    }
    else
    {
        int32_t J = Sector->WallPtr;
        int32_t OX = Blood_Walls[Blood_Walls[J].Point2].X - Blood_Walls[J].X;
        int32_t OY = Blood_Walls[Blood_Walls[J].Point2].Y - Blood_Walls[J].Y;

        int32_t I = Blood_MSqrtAsm(OX * OX + OY * OY);
        if (I == 0)
            I = 1024;
        else
            I = 1048576 / I;

        Blood_GlobalX1 = Blood_MulScale10(
            Blood_DMulScale10(OX, Blood_SinGlobalAng, -OY, Blood_CosGlobalAng), I);
        Blood_GlobalY1 = Blood_MulScale10(
            Blood_DMulScale10(OX, Blood_CosGlobalAng, OY, Blood_SinGlobalAng), I);
        Blood_GlobalX2 = -Blood_GlobalX1;
        Blood_GlobalY2 = -Blood_GlobalY1;

        OX = ((Blood_Walls[J].X - Blood_GlobalPosX) << 6);
        OY = ((Blood_Walls[J].Y - Blood_GlobalPosY) << 6);
        I = Blood_DMulScale14(OY, Blood_CosGlobalAng, -OX, Blood_SinGlobalAng);
        J = Blood_DMulScale14(OX, Blood_CosGlobalAng, OY, Blood_SinGlobalAng);
        OX = I; OY = J;
        Blood_GlobalXPanning = Blood_GlobalX1 * OX - Blood_GlobalY1 * OY;
        Blood_GlobalYPanning = Blood_GlobalY2 * OX + Blood_GlobalX2 * OY;
    }

    Blood_GlobalX2 = Blood_MulScale16(Blood_GlobalX2, Blood_ViewingRangeRecip);
    Blood_GlobalY1 = Blood_MulScale16(Blood_GlobalY1, Blood_ViewingRangeRecip);
    Blood_GlobalXShift = (8 - (Blood_PicSize[Blood_GlobalPicNum] & 15));
    Blood_GlobalYShift = (8 - (Blood_PicSize[Blood_GlobalPicNum] >> 4));

    if (Blood_GlobalOrientation & 8)
    {
        Blood_GlobalXShift++;
        Blood_GlobalYShift++;
    }

    if ((Blood_GlobalOrientation & 0x4) > 0)
    {
        int32_t I;

        I = Blood_GlobalXPanning;
        Blood_GlobalXPanning = Blood_GlobalYPanning;
        Blood_GlobalYPanning = I;

        I = Blood_GlobalX2;
        Blood_GlobalX2 = -Blood_GlobalY1;
        Blood_GlobalY1 = -I;

        I = Blood_GlobalX1;
        Blood_GlobalX1 = Blood_GlobalY2;
        Blood_GlobalY2 = I;
    }

    if ((Blood_GlobalOrientation & 0x10) > 0)
    {
        Blood_GlobalX1 = -Blood_GlobalX1;
        Blood_GlobalY1 = -Blood_GlobalY1;
        Blood_GlobalXPanning = -Blood_GlobalXPanning;
    }

    if ((Blood_GlobalOrientation & 0x20) > 0)
    {
        Blood_GlobalX2 = -Blood_GlobalX2;
        Blood_GlobalY2 = -Blood_GlobalY2;
        Blood_GlobalYPanning = -Blood_GlobalYPanning;
    }

    Blood_GlobalX1 <<= Blood_GlobalXShift;
    Blood_GlobalY1 <<= Blood_GlobalXShift;
    Blood_GlobalX2 <<= Blood_GlobalYShift;
    Blood_GlobalY2 <<= Blood_GlobalYShift;

    Blood_GlobalXPanning <<= Blood_GlobalXShift;
    Blood_GlobalYPanning <<= Blood_GlobalYShift;

    Blood_GlobalXPanning += (((int32_t)Sector->FloorXPanning) << 24);
    Blood_GlobalYPanning += (((int32_t)Sector->FloorYPanning) << 24);
    Blood_GlobalY1 = (-Blood_GlobalX1 - Blood_GlobalY1) * Blood_HalfXDimen;
    Blood_GlobalX2 = ( Blood_GlobalX2 - Blood_GlobalY2) * Blood_HalfXDimen;

    Blood_SetHLineSizes(Blood_PicSize[Blood_GlobalPicNum] & 15,
                        Blood_PicSize[Blood_GlobalPicNum] >> 4,
                        Blood_GlobalBufPlc);

    Blood_GlobalX2 += Blood_GlobalY2 * (X1 - 1);
    Blood_GlobalY1 += Blood_GlobalX1 * (X1 - 1);
    Blood_GlobalX1 = Blood_MulScale16(Blood_GlobalX1, Blood_GlobalZD);
    Blood_GlobalX2 = Blood_MulScale16(Blood_GlobalX2, Blood_GlobalZD);
    Blood_GlobalY1 = Blood_MulScale16(Blood_GlobalY1, Blood_GlobalZD);
    Blood_GlobalY2 = Blood_MulScale16(Blood_GlobalY2, Blood_GlobalZD);
    Blood_GlobVis = Blood_KLAbs(Blood_MulScale10(Blood_GlobVis, Blood_GlobalZD));

    int32_t Y1 = Blood_Max(Blood_DPlc[X1], Blood_UMost[X1]);
    int32_t Y2 = Y1;

    for (int32_t X = X1; X <= X2; X++)
    {
        int32_t TWall = Blood_Max(Blood_DPlc[X], Blood_UMost[X]) - 1;
        int32_t BWall = Blood_DMost[X];

        if (TWall < BWall - 1)
        {
            if (TWall >= Y2)
            {
                while (Y1 < Y2 - 1)
                    Blood_HLine(X - 1, ++Y1);
                Y1 = TWall;
            }
            else
            {
                while (Y1 < TWall)
                    Blood_HLine(X - 1, ++Y1);
                while (Y1 > TWall)
                    Blood_LastX[Y1--] = X;
            }

            while (Y2 > BWall)
                Blood_HLine(X - 1, --Y2);
            while (Y2 < BWall)
                Blood_LastX[Y2++] = X;
        }
        else
        {
            while (Y1 < Y2 - 1)
                Blood_HLine(X - 1, ++Y1);

            if (X == X2)
            {
                Blood_GlobalX2 += Blood_GlobalY2;
                Blood_GlobalY1 += Blood_GlobalX1;
                break;
            }

            Y1 = Blood_Max(Blood_DPlc[X + 1], Blood_UMost[X + 1]);
            Y2 = Y1;
        }

        Blood_GlobalX2 += Blood_GlobalY2;
        Blood_GlobalY1 += Blood_GlobalX1;
    }

    while (Y1 < Y2 - 1)
        Blood_HLine(X2, ++Y1);

    Blood_FakeTimerHandler();
}

void Blood_CeilScan(int32_t X1, int32_t X2, int32_t SectorIndex)
{
    Blood_Sector_t *Sector = &Blood_Sectors[SectorIndex];
    Blood_GlobalPalWritten = Blood_PalLookup[Sector->CeilingPal];
    Blood_SetPalLookupAddress(Blood_GlobalPalWritten);

    Blood_GlobalZD = Sector->CeilingZ - Blood_GlobalPosZ;
    if (Blood_GlobalZD > 0)
        return;

    Blood_GlobalPicNum = Sector->CeilingPicNum;

    if ((uint32_t)Blood_GlobalPicNum >= (uint32_t)Blood_MaxTiles)
        Blood_GlobalPicNum = 0;

    Blood_SetGotPic(Blood_GlobalPicNum);

    if ((Blood_TileSizeX[Blood_GlobalPicNum] <= 0) ||
        (Blood_TileSizeY[Blood_GlobalPicNum] <= 0))
    {
        return;
    }

    if (Blood_PicAnm[Blood_GlobalPicNum] & 192)
        Blood_GlobalPicNum += Blood_AnimateOffs(Blood_GlobalPicNum, SectorIndex);

    if (Blood_WallOff[Blood_GlobalPicNum] == 0)
        Blood_LoadTile(Blood_GlobalPicNum);
    Blood_GlobalBufPlc = Blood_WallOff[Blood_GlobalPicNum];

    Blood_GlobalShade = Sector->CeilingShade;
    Blood_GlobVis = Blood_GlobalCisibility;

    if (Sector->Visibility != 0)
    {
        Blood_GlobVis =
            Blood_MulScale4(Blood_GlobVis,
                            (int32_t)((uint8_t)(Sector->Visibility + 16)));
    }

    Blood_GlobalOrientation = Sector->CeilingStat;

    if ((Blood_GlobalOrientation & 64) == 0)
    {
        Blood_GlobalX1 = Blood_SinGlobalAng;
        Blood_GlobalX2 = Blood_SinGlobalAng;
        Blood_GlobalY1 = Blood_CosGlobalAng;
        Blood_GlobalY2 = Blood_CosGlobalAng;
        Blood_GlobalXPanning = (Blood_GlobalPosX << 20);
        Blood_GlobalYPanning = -(Blood_GlobalPosY << 20);
    }
    else
    {
        int32_t J = Sector->WallPtr;
        int32_t OX = Blood_Walls[Blood_Walls[J].Point2].X - Blood_Walls[J].X;
        int32_t OY = Blood_Walls[Blood_Walls[J].Point2].Y - Blood_Walls[J].Y;

        int32_t I = Blood_MSqrtAsm(OX * OX + OY * OY);

        if (I == 0)
            I = 1024;
        else I =
            1048576 / I;

        Blood_GlobalX1 = Blood_MulScale16(Blood_DMulScale10(
            OX, Blood_SinGlobalAng, -OY, Blood_CosGlobalAng), I);
        Blood_GlobalY1 = Blood_MulScale16(Blood_DMulScale10(
            OX, Blood_CosGlobalAng, OY, Blood_SinGlobalAng), I);
        Blood_GlobalX2 = -Blood_GlobalX1;
        Blood_GlobalY2 = -Blood_GlobalY1;

        OX = ((Blood_Walls[J].X - Blood_GlobalPosX) << 6);
        OY = ((Blood_Walls[J].Y - Blood_GlobalPosY) << 6);
        I = Blood_DMulScale14(OY, Blood_CosGlobalAng, -OX, Blood_SinGlobalAng);
        J = Blood_DMulScale14(OX, Blood_CosGlobalAng, OY, Blood_SinGlobalAng);
        OX = I; OY = J;
        Blood_GlobalXPanning = Blood_GlobalX1 * OX - Blood_GlobalY1 * OY;
        Blood_GlobalYPanning = Blood_GlobalY2 * OX + Blood_GlobalX2 * OY;
    }

    Blood_GlobalX2 = Blood_MulScale16(Blood_GlobalX2, Blood_ViewingRangeRecip);
    Blood_GlobalY1 = Blood_MulScale16(Blood_GlobalY1, Blood_ViewingRangeRecip);

    Blood_GlobalXShift = (8 - (Blood_PicSize[Blood_GlobalPicNum] & 15));
    Blood_GlobalYShift = (8 - (Blood_PicSize[Blood_GlobalPicNum] >> 4));

    if (Blood_GlobalOrientation & 8)
    {
        Blood_GlobalXShift++;
        Blood_GlobalYShift++;
    }

    if ((Blood_GlobalOrientation & 0x4) > 0)
    {
        int32_t I;

        I = Blood_GlobalXPanning;
        Blood_GlobalXPanning = Blood_GlobalYPanning;
        Blood_GlobalYPanning = I;

        I = Blood_GlobalX2;
        Blood_GlobalX2 = -Blood_GlobalY1;
        Blood_GlobalY1 = -I;

        I = Blood_GlobalX1;
        Blood_GlobalX1 = Blood_GlobalY2;
        Blood_GlobalY2 = I;
    }

    if ((Blood_GlobalOrientation & 0x10) > 0)
    {
        Blood_GlobalX1 = -Blood_GlobalX1;
        Blood_GlobalY1 = -Blood_GlobalY1;
        Blood_GlobalXPanning = -Blood_GlobalXPanning;
    }

    if ((Blood_GlobalOrientation & 0x20) > 0)
    {
        Blood_GlobalX2 = -Blood_GlobalX2;
        Blood_GlobalY2 = -Blood_GlobalY2;
        Blood_GlobalYPanning = -Blood_GlobalYPanning;
    }

    Blood_GlobalX1 <<= Blood_GlobalXShift;
    Blood_GlobalY1 <<= Blood_GlobalXShift;

    Blood_GlobalX2 <<= Blood_GlobalYShift;
    Blood_GlobalY2 <<= Blood_GlobalYShift;
    Blood_GlobalXPanning <<= Blood_GlobalXShift;
    Blood_GlobalYPanning <<= Blood_GlobalYShift;

    Blood_GlobalXPanning += (((int32_t)Sector->CeilingXPanning) << 24);
    Blood_GlobalYPanning += (((int32_t)Sector->CeilingYPanning) << 24);
    Blood_GlobalY1 = (-Blood_GlobalX1 - Blood_GlobalY1) * Blood_HalfXDimen;
    Blood_GlobalX2 = (Blood_GlobalX2 - Blood_GlobalY2) * Blood_HalfXDimen;

    Blood_SetHLineSizes(Blood_PicSize[Blood_GlobalPicNum] & 15,
                        Blood_PicSize[Blood_GlobalPicNum] >> 4,
                        Blood_GlobalBufPlc);

    Blood_GlobalX2 += Blood_GlobalY2 *(X1 - 1);
    Blood_GlobalY1 += Blood_GlobalX1 *(X1 - 1);
    Blood_GlobalX1 = Blood_MulScale16(Blood_GlobalX1, Blood_GlobalZD);
    Blood_GlobalX2 = Blood_MulScale16(Blood_GlobalX2, Blood_GlobalZD);
    Blood_GlobalY1 = Blood_MulScale16(Blood_GlobalY1, Blood_GlobalZD);
    Blood_GlobalY2 = Blood_MulScale16(Blood_GlobalY2, Blood_GlobalZD);
    Blood_GlobVis = Blood_KLAbs(Blood_MulScale10(Blood_GlobVis, Blood_GlobalZD));

    int32_t Y1 = Blood_UMost[X1];
    int32_t Y2 = Y1;

    for (int32_t X = X1; X <= X2; X++)
    {
        int32_t TWall = Blood_UMost[X] - 1;
        int32_t BWall = Blood_Min(Blood_UPlc[X], Blood_DMost[X]);

        if (TWall < BWall - 1)
        {
            if (TWall >= Y2)
            {
                while (Y1 < Y2 - 1)
                    Blood_HLine(X - 1, ++Y1);
                Y1 = TWall;
            }
            else
            {
                while (Y1 < TWall)
                    Blood_HLine(X - 1, ++Y1);
                while (Y1 > TWall)
                    Blood_LastX[Y1--] = X;
            }

            while (Y2 > BWall)
                Blood_HLine(X - 1, --Y2);
            while (Y2 < BWall)
                Blood_LastX[Y2++] = X;
        }
        else
        {
            while (Y1 < Y2 - 1)
                Blood_HLine(X - 1, ++Y1);

            if (X == X2)
            {
                Blood_GlobalX2 += Blood_GlobalY2;
                Blood_GlobalY1 += Blood_GlobalX1;
                break;
            }

            Y1 = Blood_UMost[X + 1];
            Y2 = Y1;
        }

        Blood_GlobalX2 += Blood_GlobalY2;
        Blood_GlobalY1 += Blood_GlobalX1;
    }

    while (Y1 < Y2 - 1)
        Blood_HLine(X2, ++Y1);

    Blood_FakeTimerHandler();
}

void Blood_ParaScan(int32_t DaX1, int32_t DaX2, int32_t SectorIndex,
                    uint8_t DaStat, int32_t BunchIndex)
{
    SectorIndex = Blood_TheSector[Blood_BunchFirst[BunchIndex]];
    Blood_Sector_t *Sector = &Blood_Sectors[SectorIndex];

    int32_t GlobalHorizBak = Blood_GlobalHoriz;

    if (Blood_ParallaxYScale != 65536)
    {
        Blood_GlobalHoriz =
            Blood_MulScale16(Blood_GlobalHoriz - (Blood_YDimen >> 1),
                             Blood_ParallaxYScale) + (Blood_YDimen >> 1);
    }

    Blood_GlobVis = Blood_GlobalPisibility;

    if (Sector->Visibility != 0)
    {
        Blood_GlobVis =
            Blood_MulScale4(Blood_GlobVis,
                            (int32_t)((uint8_t)(Sector->Visibility + 16)));
    }

    int16_t *TopPtr, *BotPtr;

    if (DaStat == 0)
    {
        Blood_GlobalPal = Sector->CeilingPal;
        Blood_GlobalPicNum = Sector->CeilingPicNum;
        Blood_GlobalShade = Sector->CeilingShade;
        Blood_GlobalXPanning = Sector->CeilingXPanning;
        Blood_GlobalYPanning = Sector->CeilingYPanning;
        TopPtr = Blood_UMost;
        BotPtr = Blood_UPlc;
    }
    else
    {
        Blood_GlobalPal = Sector->FloorPal;
        Blood_GlobalPicNum = Sector->FloorPicNum;
        Blood_GlobalShade = Sector->FloorShade;
        Blood_GlobalXPanning = Sector->FloorXPanning;
        Blood_GlobalYPanning = Sector->FloorYPanning;
        TopPtr = Blood_DPlc;
        BotPtr = Blood_DMost;
    }

    if ((uint32_t)Blood_GlobalPicNum >= (uint32_t)Blood_MaxTiles)
        Blood_GlobalPicNum = 0;

    if (Blood_PicAnm[Blood_GlobalPicNum] & 192)
        Blood_GlobalPicNum += Blood_AnimateOffs(Blood_GlobalPicNum, SectorIndex);

    Blood_GlobalShiftVal = (Blood_PicSize[Blood_GlobalPicNum] >> 4);

    if (Blood_Pow2Long[Blood_GlobalShiftVal] !=
        Blood_TileSizeY[Blood_GlobalPicNum])
    {
        Blood_GlobalShiftVal++;
    }

    Blood_GlobalShiftVal = 32 - Blood_GlobalShiftVal;
    Blood_GlobalZD = (((Blood_TileSizeY[Blood_GlobalPicNum] >> 1) + 
                        Blood_ParallaxYOffset) << Blood_GlobalShiftVal) +
                     (Blood_GlobalYPanning << 24);

    Blood_GlobalYScale = (8 << (Blood_GlobalShiftVal - 19));

    int32_t K = 11 - (Blood_PicSize[Blood_GlobalPicNum] & 15) - Blood_PSkyBits;
    int32_t X = -1;

    for (int32_t Z = Blood_BunchFirst[BunchIndex]; Z >= 0; Z = Blood_P2[Z])
    {
        int32_t WallIndex = Blood_TheWall[Z];
        int32_t NextSectorIndex = Blood_Walls[WallIndex].NextSector;

        int32_t J;
        if (DaStat == 0)
            J = Blood_Sectors[NextSectorIndex].CeilingStat;
        else
            J = Blood_Sectors[NextSectorIndex].FloorStat;

        if ((NextSectorIndex < 0) || (Blood_Walls[WallIndex].CStat & 32) ||
            ((J & 1) == 0))
        {
            if (X == -1)
                X = Blood_XB1[Z];

            if (Blood_ParallaxType == 0)
            {
                int32_t N = Blood_MulScale16(Blood_XDimenRecip, Blood_ViewingRange);

                for(J = Blood_XB1[Z]; J <= Blood_XB2[Z]; J++)
                {
                    Blood_LPlc[J] = (((Blood_MulScale32(J - Blood_HalfXDimen, N) +
                                    Blood_GlobalAng) & 2047) >> K);
                }
            }
            else
            {
                for (J = Blood_XB1[Z]; J <= Blood_XB2[Z]; J++)
                {
                    Blood_LPlc[J] = ((((int32_t)Blood_RadarAng2[J] +
                                      Blood_GlobalAng) & 2047) >> K);
                }
            }

            if (Blood_ParallaxType == 2)
            {
                int32_t N = Blood_MulScale16(Blood_XDimScale, Blood_ViewingRange);

                for (J = Blood_XB1[Z]; J <= Blood_XB2[Z]; J++)
                {
                    Blood_SWPlc[J] = Blood_MulScale14(
                        Blood_SinTable[((int32_t)Blood_RadarAng2[J] + 512) & 2047], N);
                }
            }
            else
            {
                Blood_ClearBuffer(&Blood_SWPlc[Blood_XB1[Z]],
                                  Blood_XB2[Z] - Blood_XB1[Z] + 1,
                                  Blood_MulScale16(Blood_XDimScale, Blood_ViewingRange));
            }
        }
        else if (X >= 0)
        {
            int32_t L = Blood_GlobalPicNum;
            int32_t M = (Blood_PicSize[Blood_GlobalPicNum] & 15);

            Blood_GlobalPicNum = L + Blood_PSkyOffset[Blood_LPlc[X] >> M];

            if (((Blood_LPlc[X] ^ Blood_LPlc[Blood_XB1[Z] - 1]) >> M) == 0)
            {
                Blood_WallScan(X, Blood_XB1[Z] - 1, TopPtr, BotPtr, Blood_SWPlc, Blood_LPlc);
            }
            else
            {
                J = X;
                while (X < Blood_XB1[Z])
                {
                    int32_t N = L + Blood_PSkyOffset[Blood_LPlc[X] >> M];
                    if (N != Blood_GlobalPicNum)
                    {
                        Blood_WallScan(J, X - 1, TopPtr, BotPtr, Blood_SWPlc, Blood_LPlc);
                        J = X;
                        Blood_GlobalPicNum = N;
                    }
                    X++;
                }

                if (J < X)
                    Blood_WallScan(J, X - 1, TopPtr, BotPtr, Blood_SWPlc, Blood_LPlc);
            }

            Blood_GlobalPicNum = L;
            X = -1;
        }
    }

    if (X >= 0)
    {
        int32_t L = Blood_GlobalPicNum;
        int32_t M = (Blood_PicSize[Blood_GlobalPicNum] & 15);
        Blood_GlobalPicNum = L + Blood_PSkyOffset[Blood_LPlc[X] >> M];

        if (((Blood_LPlc[X] ^ Blood_LPlc[Blood_XB2[Blood_BunchLast[BunchIndex]]]) >> M) == 0)
        {
            Blood_WallScan(X, Blood_XB2[Blood_BunchLast[BunchIndex]], TopPtr, BotPtr,
                           Blood_SWPlc, Blood_LPlc);
        }
        else
        {
            int32_t J = X;
            while (X <= Blood_XB2[Blood_BunchLast[BunchIndex]])
            {
                int32_t N = L + Blood_PSkyOffset[Blood_LPlc[X] >> M];
                if (N != Blood_GlobalPicNum)
                {
                    Blood_WallScan(J, X - 1, TopPtr, BotPtr, Blood_SWPlc, Blood_LPlc);
                    J = X;
                    Blood_GlobalPicNum = N;
                }
                X++;
            }
            if (J <= X)
                Blood_WallScan(J, X, TopPtr, BotPtr, Blood_SWPlc, Blood_LPlc);
        }
        Blood_GlobalPicNum = L;
    }

    Blood_GlobalHoriz = GlobalHorizBak;
}
