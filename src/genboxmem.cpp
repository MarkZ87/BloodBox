
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    const char * Name;
    uint32_t     Address;
} Label_t;

static uint32_t TotalSize = (28 << 20);
static uint32_t LabelCount;
static Label_t  Labels[4096];

#define CSeg01(X) (0x130010+(X)-0x010000)
#define DSeg02(X) (0x215fc0+(X)-0x100000)
#define DSeg03(X) (0x215ff0+(X)-0x110000)
#define DSeg04(X) (0x216060+(X)-0x120000)

static void AddLabel(const char *Name, uint32_t Address)
{
    Label_t *Label = &Labels[LabelCount++];

    Label->Name = Name;
    Label->Address = Address;
}

static int CompareFunc(const void *A, const void *B)
{
    Label_t *LabelA = (Label_t *)A;
    Label_t *LabelB = (Label_t *)B;

    return LabelA->Address - LabelB->Address;
}

int main(void)
{
    /* blood_build.hpp */
    AddLabel("Blood_ViewingRange", DSeg04(0x3ddfd4));
    AddLabel("Blood_ViewingRangeRecip", DSeg04(0x3db804));
    AddLabel("Blood_YXAspect", DSeg04(0x3ddfc0));
    AddLabel("Blood_XYAspect", DSeg04(0x3db7d4));
    AddLabel("Blood_XDimen", DSeg04(0x13c944));
    AddLabel("Blood_XDimenScale", DSeg04(0x3db828));
    AddLabel("Blood_XDimScale", DSeg04(0x3db844));
    AddLabel("Blood_WindowX1", DSeg04(0x3dccf0));
    AddLabel("Blood_WindowY1", DSeg04(0x3dcce4));
    AddLabel("Blood_WindowX2", DSeg04(0x3dccec));
    AddLabel("Blood_WindowY2", DSeg04(0x3dcce0));
    AddLabel("Blood_WX1", DSeg04(0x3db840));
    AddLabel("Blood_WY1", DSeg04(0x3db84c));
    AddLabel("Blood_WX2", DSeg04(0x3db83c));
    AddLabel("Blood_WY2", DSeg04(0x3db848));
    AddLabel("Blood_XDim", DSeg04(0x3ddfdc));
    AddLabel("Blood_YDim", DSeg04(0x3ddfd8));
    AddLabel("Blood_HalfXDimen", DSeg04(0x3db834));
    AddLabel("Blood_XDimenRecip", DSeg04(0x3db830));
    AddLabel("Blood_YDimen", DSeg04(0x3db858));
    AddLabel("Blood_ViewOffset", DSeg04(0x3db838));
    AddLabel("Blood_BytesPerLine", DSeg04(0x3dcc98));
    AddLabel("Blood_StereoMode", DSeg04(0x13c8a3));
    AddLabel("Blood_VidOption", DSeg04(0x3df0b4));
    AddLabel("Blood_OStereoPixelWidth", DSeg04(0x13c89f));
    AddLabel("Blood_StereoPixelWidth", DSeg04(0x13c89b));
    AddLabel("Blood_StartUMost", DSeg04(0x351ac8));
    AddLabel("Blood_StartDMost", DSeg04(0x354748));
    AddLabel("Blood_BeforeDrawRooms", DSeg04(0x13c887));
    AddLabel("Blood_TotalArea", DSeg04(0x13c883));
    AddLabel("Blood_GlobalPosX", DSeg04(0x3db7e8));
    AddLabel("Blood_GlobalPosY", DSeg04(0x3db7f0));
    AddLabel("Blood_GlobalPosZ", DSeg04(0x3db7f4));
    AddLabel("Blood_GlobalAng", DSeg04(0x3df012));
    AddLabel("Blood_GlobalHoriz", DSeg04(0x3db814));
    AddLabel("Blood_GlobalUClip", DSeg04(0x3db7dc));
    AddLabel("Blood_GlobalDClip", DSeg04(0x3db824));
    AddLabel("Blood_GlobalPisibility", DSeg04(0x3db800));
    AddLabel("Blood_GlobalVisibility", DSeg04(0x3dccf4));
    AddLabel("Blood_GlobalHisibility", DSeg04(0x3db7ec));
    AddLabel("Blood_GlobalCisibility", DSeg04(0x3db7e0));
    AddLabel("Blood_ParallaxVisibility", DSeg04(0x3dccf4));
    AddLabel("Blood_GlobalCurSectNum", DSeg04(0x3df014));
    AddLabel("Blood_TotalClockLock", DSeg04(0x3db0e8));
    AddLabel("Blood_TotalClock", DSeg04(0x3ddfd0));
    AddLabel("Blood_CosGlobalAng", DSeg04(0x3db7d8));
    AddLabel("Blood_SinGlobalAng", DSeg04(0x3db820));
    AddLabel("Blood_SinTable", DSeg04(0x350ac8));
    AddLabel("Blood_CosViewingRangeGlobalAng", DSeg04(0x3db7fc));
    AddLabel("Blood_SinViewingRangeGlobalAng", DSeg04(0x3db7e4));
    AddLabel("Blood_ActivePage", DSeg04(0x3dc868));
    AddLabel("Blood_StereoWidth", DSeg04(0x13c897));
    AddLabel("Blood_FramePlace", DSeg04(0x3dccb0));
    AddLabel("Blood_Screen", DSeg04(0x13c868));
    AddLabel("Blood_OXYAspect", DSeg04(0x13c893));
    AddLabel("Blood_OXDimen", DSeg04(0x13c88b));
    AddLabel("Blood_OViewingRange", DSeg04(0x13c88f));
    AddLabel("Blood_FrameOffset", DSeg04(0x3db850));
    AddLabel("Blood_GotSector", DSeg04(0x346748));
    AddLabel("Blood_NumSectors", DSeg04(0x3df026));
    AddLabel("Blood_UMost", DSeg04(0x2eb708));
    AddLabel("Blood_DMost", DSeg04(0x2f9308));
    AddLabel("Blood_NumHits", DSeg04(0x3deffa));
    AddLabel("Blood_NumScans", DSeg04(0x3deff6));
    AddLabel("Blood_NumBunches", DSeg04(0x3deff8));
    AddLabel("Blood_MaskWallCount", DSeg04(0x3df016));
    AddLabel("Blood_SMostWallCount", DSeg04(0x13c940));
    AddLabel("Blood_SMostCount", DSeg04(0x3df018));
    AddLabel("Blood_SpriteSortCount", DSeg04(0x3ddfcc));
    AddLabel("Blood_GlobParaCeilClip", DSeg04(0x3df0a7));
    AddLabel("Blood_GlobParaFlorClip", DSeg04(0x3df0a8));
    AddLabel("Blood_TempBuf", DSeg04(0x319488));
    AddLabel("Blood_AutoMapping", DSeg04(0x3df0b1));
    AddLabel("Blood_BunchFirst", DSeg04(0x2f0908));
    AddLabel("Blood_P2", DSeg04(0x312408));
    AddLabel("Blood_Show2DWall", DSeg04(0x340348));
    AddLabel("Blood_TheWall", DSeg04(0x2e6a88));
    AddLabel("Blood_Pow2Char", DSeg04(0x13c8b8));
    AddLabel("Blood_BunchLast", DSeg04(0x2f3e88));
    AddLabel("Blood_Visibility", DSeg04(0x3dccf8));
    AddLabel("Blood_Show2DSector", DSeg04(0x33a2c8));
    AddLabel("Blood_Sectors", DSeg04(0x3d10c8));
    AddLabel("Blood_Walls", DSeg04(0x3858c8));
    AddLabel("Blood_Sprites", DSeg04(0x3596c8));
    AddLabel("Blood_SectorBorder", DSeg04(0x2c7978));
    AddLabel("Blood_SectorBorderCount", DSeg04(0x3df002));
    AddLabel("Blood_ShowInvisibility", DSeg04(0x3df0b3));
    AddLabel("Blood_TSprites", DSeg04(0x3c58c8));
    AddLabel("Blood_TSpriteSortCount", DSeg04(0x3ddfcc));
    AddLabel("Blood_TheSector", DSeg04(0x2f8308));
    AddLabel("Blood_XB1", DSeg04(0x2ff608));
    AddLabel("Blood_YB1", DSeg04(0x303608));
    AddLabel("Blood_XB2", DSeg04(0x2fd608));
    AddLabel("Blood_YB2", DSeg04(0x301608));
    AddLabel("Blood_RX1", DSeg04(0x307608));
    AddLabel("Blood_RX2", DSeg04(0x305608));
    AddLabel("Blood_RY1", DSeg04(0x30d608));
    AddLabel("Blood_RY2", DSeg04(0x30b608));
    AddLabel("Blood_UPlc", DSeg04(0x2edc88));
    AddLabel("Blood_DPlc", DSeg04(0x2ec388));
    AddLabel("Blood_SqrtTable", DSeg04(0x309608));
    AddLabel("Blood_ShLookup", DSeg04(0x2fa788));
    AddLabel("Blood_SearchIt", DSeg04(0x3deff2));
    AddLabel("Blood_SearchX", DSeg04(0x13ca1e));
    AddLabel("Blood_SearchY", DSeg04(0x3db0f4));
    AddLabel("Blood_SearchSector", DSeg04(0x3defee));
    AddLabel("Blood_SearchWall", DSeg04(0x3deff0));
    AddLabel("Blood_SearchStat", DSeg04(0x3defec));
    AddLabel("Blood_SMost", DSeg04(0x2cda88));
    AddLabel("Blood_SMostStart", DSeg04(0x2f5b08));
    AddLabel("Blood_SMostWallType", DSeg04(0x2f7b08));
    AddLabel("Blood_SMostWall", DSeg04(0x2ee908));
    AddLabel("Blood_MaskWall", DSeg04(0x2e8708));
    AddLabel("Blood_UWall", DSeg04(0x2e7a88));
    AddLabel("Blood_DWall", DSeg04(0x2f3208));
    AddLabel("Blood_SWall", DSeg04(0x2cc108));
    AddLabel("Blood_LWall", DSeg04(0x2c8eb8));
    AddLabel("Blood_GlobalOrientation", DSeg04(0x3db79c));
    AddLabel("Blood_GlobalXPanning", DSeg04(0x3db7cc));
    AddLabel("Blood_GlobalYPanning", DSeg04(0x3db7d0));
    AddLabel("Blood_GlobalShade", DSeg04(0x3db7a0));
    AddLabel("Blood_GlobalPicNum", DSeg04(0x3df00e));
    AddLabel("Blood_GlobalShiftVal", DSeg04(0x3df010));
    AddLabel("Blood_PicSize", DSeg04(0x321208));
    AddLabel("Blood_Pow2Long", DSeg04(0x13c8c0));
    AddLabel("Blood_TileSizeY", DSeg04(0x343748));
    AddLabel("Blood_PicAnm", DSeg04(0x33a348));
    AddLabel("Blood_GlobVis", DSeg04(0x3db818));
    AddLabel("Blood_GlobalPal", DSeg04(0x3db808));
    AddLabel("Blood_GlobalYScale", DSeg04(0x3db7c8));
    AddLabel("Blood_GlobalZD", DSeg04(0x3db7b0));
    AddLabel("Blood_WallLock", DSeg04(0x3467c8));
    AddLabel("Blood_GotPic", DSeg04(0x347fc8));
    AddLabel("Blood_TileSizeX", DSeg04(0x340748));
    AddLabel("Blood_PalLookup", DSeg04(0x3506c8));
    AddLabel("Blood_BufPlce", DSeg04(0x2c8e68));
    AddLabel("Blood_Vince", DSeg04(0x2c8e48));
    AddLabel("Blood_VPlce", DSeg04(0x2c8e58));
    AddLabel("Blood_YLookup", DSeg04(0x3dccfc));
    AddLabel("Blood_WallOff", DSeg04(0x34a2c8));
    AddLabel("Blood_PalLookupOffset", DSeg04(0x2c8e38));
    AddLabel("Blood_FixChain1A", CSeg01(0xf0bf4 + 2));
    AddLabel("Blood_FixChain1B", CSeg01(0xf0bff + 2));
    AddLabel("Blood_Mach3A", CSeg01(0xf0bfc + 2));
    AddLabel("Blood_GlobalPalWritten", DSeg04(0x3db81c));
    AddLabel("Blood_GlobalBufPlc", DSeg04(0x3db7b8));
    AddLabel("Blood_GlobalX1", DSeg04(0x3db790));
    AddLabel("Blood_GlobalX2", DSeg04(0x3db798));
    AddLabel("Blood_GlobalY1", DSeg04(0x3db7a4));
    AddLabel("Blood_GlobalY2", DSeg04(0x3db7ac));
    AddLabel("Blood_GlobalXShift", DSeg04(0x3df0a5));
    AddLabel("Blood_GlobalYShift", DSeg04(0x3df0a6));
    AddLabel("Blood_LastX", DSeg04(0x2c7b78));
    AddLabel("Blood_Asm1", DSeg04(0x3db810));
    AddLabel("Blood_Asm2", DSeg04(0x3db7bc));
    AddLabel("Blood_HorizYCent", DSeg04(0x3db80c));
    AddLabel("Blood_HorizLookup", DSeg04(0x3db82c));
    AddLabel("Blood_HorizLookup2", DSeg04(0x3db854));
    AddLabel("Blood_RadarAng", DSeg04(0x311a08));
    AddLabel("Blood_RadarAng2", DSeg04(0x2fc988));
    AddLabel("Blood_DistRecip", DSeg04(0x323a08));
    AddLabel("Blood_NYTooClose", DSeg04(0x3dcc90));
    AddLabel("Blood_NYTooFar", DSeg04(0x3dcc94));
    AddLabel("Blood_HeadSpriteSect", DSeg04(0x3de7e8));
    AddLabel("Blood_PrevSpriteSect", DSeg04(0x352748));
    AddLabel("Blood_NextSpriteSect", DSeg04(0x3553c8));
    AddLabel("Blood_HeadSpriteStat", DSeg04(0x3ddfe6));
    AddLabel("Blood_PrevSpriteStat", DSeg04(0x3573c8));
    AddLabel("Blood_NextSpriteStat", DSeg04(0x3482c8));

    AddLabel("Blood_ParallaxType", DSeg04(0x3df0b2));
    AddLabel("Blood_ParallaxYOffset", DSeg04(0x3dccd8));
    AddLabel("Blood_ParallaxYScale", DSeg04(0x3dccdc));
    AddLabel("Blood_PSkyOffset", DSeg04(0x3504c8));
    AddLabel("Blood_PSkyBits", DSeg04(0x3df020));
    AddLabel("Blood_LPlc", DSeg04(0x2ca7e8));
    AddLabel("Blood_SWPlc", DSeg04(0x2f1908));

    /* blood_db.hpp */
    AddLabel("Blood_DB_StatCount", DSeg04(0x1a6ec4));
    AddLabel("Blood_BD_1A2EC4", DSeg04(0x1a2ec4));
    AddLabel("Blood_BD_19EEC4", DSeg04(0x19eec4));
    AddLabel("Blood_BD_19AEC4", DSeg04(0x19aec4));

    qsort(Labels, LabelCount, sizeof(Label_t), CompareFunc);

    FILE *fp = fopen("boxmem.asm", "wb");

    fprintf(fp, "\nsection .bss\n\n");
    fprintf(fp, "global MemArray\n");
    fprintf(fp, "MemArray:\n\n");

    uint32_t Address = 0;
    for (uint32_t Index = 0;
         Index < LabelCount; Index++)
    {
        Label_t *Label = &Labels[Index];

        if (Label->Address > Address) {
            uint32_t Count = Label->Address - Address;
            Address = Label->Address;
            fprintf(fp, "    resb 0x%x\n", Count);
        }

        fprintf(fp, "global %s\n", Label->Name);
        fprintf(fp, "%s:\n", Label->Name);
    }

    if (Address < TotalSize)
        fprintf(fp, "    resb 0x%x\n", TotalSize - Address);

    fprintf(fp, "\n");
    fclose(fp);

    return 0;
}
