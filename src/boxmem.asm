
section .bss

global MemArray
MemArray:

    resb 0x210c06
global Blood_FixChain1A
Blood_FixChain1A:
    resb 0x8
global Blood_Mach3A
Blood_Mach3A:
    resb 0x3
global Blood_FixChain1B
Blood_FixChain1B:
    resb 0x21cb7
global Blood_Screen
Blood_Screen:
    resb 0x1b
global Blood_TotalArea
Blood_TotalArea:
    resb 0x4
global Blood_BeforeDrawRooms
Blood_BeforeDrawRooms:
    resb 0x4
global Blood_OXDimen
Blood_OXDimen:
    resb 0x4
global Blood_OViewingRange
Blood_OViewingRange:
    resb 0x4
global Blood_OXYAspect
Blood_OXYAspect:
    resb 0x4
global Blood_StereoWidth
Blood_StereoWidth:
    resb 0x4
global Blood_StereoPixelWidth
Blood_StereoPixelWidth:
    resb 0x4
global Blood_OStereoPixelWidth
Blood_OStereoPixelWidth:
    resb 0x4
global Blood_StereoMode
Blood_StereoMode:
    resb 0x15
global Blood_Pow2Char
Blood_Pow2Char:
    resb 0x8
global Blood_Pow2Long
Blood_Pow2Long:
    resb 0x80
global Blood_SMostWallCount
Blood_SMostWallCount:
    resb 0x4
global Blood_XDimen
Blood_XDimen:
    resb 0xda
global Blood_SearchX
Blood_SearchX:
    resb 0x36416
global Blood_DB_XSprites
Blood_DB_XSprites:
    resb 0x1c000
global Blood_DB_XWalls
Blood_DB_XWalls:
    resb 0x3000
global Blood_DB_XSectors
Blood_DB_XSectors:
    resb 0x7800
global Blood_DB_XSpritesFreeList
Blood_DB_XSpritesFreeList:
    resb 0x1000
global Blood_DB_XWallsFreeList
Blood_DB_XWallsFreeList:
    resb 0x400
global Blood_DB_XSectorsFreeList
Blood_DB_XSectorsFreeList:
    resb 0x490
global Blood_BD_19AEC4
Blood_BD_19AEC4:
    resb 0x4000
global Blood_BD_19EEC4
Blood_BD_19EEC4:
    resb 0x4000
global Blood_BD_1A2EC4
Blood_BD_1A2EC4:
    resb 0x4000
global Blood_DB_StatCount
Blood_DB_StatCount:
    resb 0x120ab4
global Blood_SectorBorder
Blood_SectorBorder:
    resb 0x200
global Blood_LastX
Blood_LastX:
    resb 0x12c0
global Blood_PalLookupOffset
Blood_PalLookupOffset:
    resb 0x10
global Blood_VIncE
Blood_VIncE:
    resb 0x10
global Blood_VPlcE
Blood_VPlcE:
    resb 0x10
global Blood_BufPlce
Blood_BufPlce:
    resb 0x50
global Blood_LWall
Blood_LWall:
    resb 0x1930
global Blood_LPlc
Blood_LPlc:
    resb 0x1920
global Blood_SWall
Blood_SWall:
    resb 0x1980
global Blood_SMost
Blood_SMost:
    resb 0x19000
global Blood_TheWall
Blood_TheWall:
    resb 0x1000
global Blood_UWall
Blood_UWall:
    resb 0xc80
global Blood_MaskWall
Blood_MaskWall:
    resb 0x3000
global Blood_UMost
Blood_UMost:
    resb 0xc80
global Blood_DPlc
Blood_DPlc:
    resb 0x1900
global Blood_UPlc
Blood_UPlc:
    resb 0xc80
global Blood_SMostWall
Blood_SMostWall:
    resb 0x2000
global Blood_BunchFirst
Blood_BunchFirst:
    resb 0x1000
global Blood_SWPlc
Blood_SWPlc:
    resb 0x1900
global Blood_DWall
Blood_DWall:
    resb 0xc80
global Blood_BunchLast
Blood_BunchLast:
    resb 0x1c80
global Blood_SMostStart
Blood_SMostStart:
    resb 0x2000
global Blood_SMostWallType
Blood_SMostWallType:
    resb 0x800
global Blood_TheSector
Blood_TheSector:
    resb 0x1000
global Blood_DMost
Blood_DMost:
    resb 0x1480
global Blood_ShLookup
Blood_ShLookup:
    resb 0x2200
global Blood_RadarAng2
Blood_RadarAng2:
    resb 0xc80
global Blood_XB2
Blood_XB2:
    resb 0x2000
global Blood_XB1
Blood_XB1:
    resb 0x2000
global Blood_YB2
Blood_YB2:
    resb 0x2000
global Blood_YB1
Blood_YB1:
    resb 0x2000
global Blood_RX2
Blood_RX2:
    resb 0x2000
global Blood_RX1
Blood_RX1:
    resb 0x2000
global Blood_SqrtTable
Blood_SqrtTable:
    resb 0x2000
global Blood_RY2
Blood_RY2:
    resb 0x2000
global Blood_RY1
Blood_RY1:
    resb 0x4400
global Blood_RadarAng
Blood_RadarAng:
    resb 0xa00
global Blood_P2
Blood_P2:
    resb 0x7080
global Blood_TempBuf
Blood_TempBuf:
    resb 0x2000
global Blood_SlopePalLookup
Blood_SlopePalLookup:
    resb 0x5d80
global Blood_PicSize
Blood_PicSize:
    resb 0x2800
global Blood_DistRecip
Blood_DistRecip:
    resb 0x168c0
global Blood_Show2DSector
Blood_Show2DSector:
    resb 0x80
global Blood_PicAnm
Blood_PicAnm:
    resb 0x6000
global Blood_Show2DWall
Blood_Show2DWall:
    resb 0x400
global Blood_TileSizeX
Blood_TileSizeX:
    resb 0x3000
global Blood_TileSizeY
Blood_TileSizeY:
    resb 0x3000
global Blood_GotSector
Blood_GotSector:
    resb 0x80
global Blood_WallLock
Blood_WallLock:
    resb 0x1800
global Blood_GotPic
Blood_GotPic:
    resb 0x300
global Blood_NextSpriteStat
Blood_NextSpriteStat:
    resb 0x2000
global Blood_WallOff
Blood_WallOff:
    resb 0x6200
global Blood_PSkyOffset
Blood_PSkyOffset:
    resb 0x200
global Blood_PalLookup
Blood_PalLookup:
    resb 0x400
global Blood_SinTable
Blood_SinTable:
    resb 0x1000
global Blood_StartUMost
Blood_StartUMost:
    resb 0xc80
global Blood_PrevSpriteSect
Blood_PrevSpriteSect:
    resb 0x2000
global Blood_StartDMost
Blood_StartDMost:
    resb 0xc80
global Blood_NextSpriteSect
Blood_NextSpriteSect:
    resb 0x2000
global Blood_PrevSpriteStat
Blood_PrevSpriteStat:
    resb 0x2300
global Blood_Sprites
Blood_Sprites:
    resb 0x2c200
global Blood_Walls
Blood_Walls:
    resb 0x40000
global Blood_TSprites
Blood_TSprites:
    resb 0xb800
global Blood_Sectors
Blood_Sectors:
    resb 0xa020
global Blood_TotalClockLock
Blood_TotalClockLock:
    resb 0xc
global Blood_SearchY
Blood_SearchY:
    resb 0x488
global Blood_GlobalZ
Blood_GlobalZ:
    resb 0x8
global Blood_GlobalX
Blood_GlobalX:
    resb 0x4
global Blood_GlobalY
Blood_GlobalY:
    resb 0x208
global Blood_GlobalX1
Blood_GlobalX1:
    resb 0x4
global Blood_GlobalX3
Blood_GlobalX3:
    resb 0x4
global Blood_GlobalX2
Blood_GlobalX2:
    resb 0x4
global Blood_GlobalOrientation
Blood_GlobalOrientation:
    resb 0x4
global Blood_GlobalShade
Blood_GlobalShade:
    resb 0x4
global Blood_GlobalY1
Blood_GlobalY1:
    resb 0x4
global Blood_GlobalY3
Blood_GlobalY3:
    resb 0x4
global Blood_GlobalY2
Blood_GlobalY2:
    resb 0x4
global Blood_GlobalZD
Blood_GlobalZD:
    resb 0x4
global Blood_GlobalZX
Blood_GlobalZX:
    resb 0x4
global Blood_GlobalBufPlc
Blood_GlobalBufPlc:
    resb 0x4
global Blood_Asm2
Blood_Asm2:
    resb 0x4
global Blood_Asm3
Blood_Asm3:
    resb 0x8
global Blood_GlobalYScale
Blood_GlobalYScale:
    resb 0x4
global Blood_GlobalXPanning
Blood_GlobalXPanning:
    resb 0x4
global Blood_GlobalYPanning
Blood_GlobalYPanning:
    resb 0x4
global Blood_XYAspect
Blood_XYAspect:
    resb 0x4
global Blood_CosGlobalAng
Blood_CosGlobalAng:
    resb 0x4
global Blood_GlobalUClip
Blood_GlobalUClip:
    resb 0x4
global Blood_GlobalCisibility
Blood_GlobalCisibility:
    resb 0x4
global Blood_SinViewingRangeGlobalAng
Blood_SinViewingRangeGlobalAng:
    resb 0x4
global Blood_GlobalPosX
Blood_GlobalPosX:
    resb 0x4
global Blood_GlobalHisibility
Blood_GlobalHisibility:
    resb 0x4
global Blood_GlobalPosY
Blood_GlobalPosY:
    resb 0x4
global Blood_GlobalPosZ
Blood_GlobalPosZ:
    resb 0x8
global Blood_CosViewingRangeGlobalAng
Blood_CosViewingRangeGlobalAng:
    resb 0x4
global Blood_GlobalPisibility
Blood_GlobalPisibility:
    resb 0x4
global Blood_ViewingRangeRecip
Blood_ViewingRangeRecip:
    resb 0x4
global Blood_GlobalPal
Blood_GlobalPal:
    resb 0x4
global Blood_HorizYCent
Blood_HorizYCent:
    resb 0x4
global Blood_Asm1
Blood_Asm1:
    resb 0x4
global Blood_GlobalHoriz
Blood_GlobalHoriz:
    resb 0x4
global Blood_GlobVis
Blood_GlobVis:
    resb 0x4
global Blood_GlobalPalWritten
Blood_GlobalPalWritten:
    resb 0x4
global Blood_SinGlobalAng
Blood_SinGlobalAng:
    resb 0x4
global Blood_GlobalDClip
Blood_GlobalDClip:
    resb 0x4
global Blood_XDimenScale
Blood_XDimenScale:
    resb 0x4
global Blood_HorizLookup
Blood_HorizLookup:
    resb 0x4
global Blood_XDimenRecip
Blood_XDimenRecip:
    resb 0x4
global Blood_HalfXDimen
Blood_HalfXDimen:
    resb 0x4
global Blood_ViewOffset
Blood_ViewOffset:
    resb 0x4
global Blood_WX2
Blood_WX2:
    resb 0x4
global Blood_WX1
Blood_WX1:
    resb 0x4
global Blood_XDimScale
Blood_XDimScale:
    resb 0x4
global Blood_WY2
Blood_WY2:
    resb 0x4
global Blood_WY1
Blood_WY1:
    resb 0x4
global Blood_FrameOffset
Blood_FrameOffset:
    resb 0x4
global Blood_HorizLookup2
Blood_HorizLookup2:
    resb 0x4
global Blood_YDimen
Blood_YDimen:
    resb 0x1010
global Blood_ActivePage
Blood_ActivePage:
    resb 0x428
global Blood_NYTooClose
Blood_NYTooClose:
    resb 0x4
global Blood_NYTooFar
Blood_NYTooFar:
    resb 0x4
global Blood_BytesPerLine
Blood_BytesPerLine:
    resb 0x18
global Blood_FramePlace
Blood_FramePlace:
    resb 0x28
global Blood_ParallaxYOffset
Blood_ParallaxYOffset:
    resb 0x4
global Blood_ParallaxYScale
Blood_ParallaxYScale:
    resb 0x4
global Blood_WindowY2
Blood_WindowY2:
    resb 0x4
global Blood_WindowY1
Blood_WindowY1:
    resb 0x8
global Blood_WindowX2
Blood_WindowX2:
    resb 0x4
global Blood_WindowX1
Blood_WindowX1:
    resb 0x4
global Blood_GlobalVisibility
Blood_GlobalVisibility:
global Blood_ParallaxVisibility
Blood_ParallaxVisibility:
    resb 0x4
global Blood_Visibility
Blood_Visibility:
    resb 0x4
global Blood_YLookup
Blood_YLookup:
    resb 0x12c4
global Blood_YXAspect
Blood_YXAspect:
    resb 0xc
global Blood_SpriteSortCount
Blood_SpriteSortCount:
global Blood_TSpriteSortCount
Blood_TSpriteSortCount:
    resb 0x4
global Blood_TotalClock
Blood_TotalClock:
    resb 0x4
global Blood_ViewingRange
Blood_ViewingRange:
    resb 0x4
global Blood_YDim
Blood_YDim:
    resb 0x4
global Blood_XDim
Blood_XDim:
    resb 0xa
global Blood_HeadSpriteStat
Blood_HeadSpriteStat:
    resb 0x802
global Blood_HeadSpriteSect
Blood_HeadSpriteSect:
    resb 0x804
global Blood_SearchStat
Blood_SearchStat:
    resb 0x2
global Blood_SearchSector
Blood_SearchSector:
    resb 0x2
global Blood_SearchWall
Blood_SearchWall:
    resb 0x2
global Blood_SearchIt
Blood_SearchIt:
    resb 0x4
global Blood_NumScans
Blood_NumScans:
    resb 0x2
global Blood_NumBunches
Blood_NumBunches:
    resb 0x2
global Blood_NumHits
Blood_NumHits:
    resb 0x8
global Blood_SectorBorderCount
Blood_SectorBorderCount:
    resb 0xc
global Blood_GlobalPicNum
Blood_GlobalPicNum:
    resb 0x2
global Blood_GlobalShiftVal
Blood_GlobalShiftVal:
    resb 0x2
global Blood_GlobalAng
Blood_GlobalAng:
    resb 0x2
global Blood_GlobalCurSectNum
Blood_GlobalCurSectNum:
    resb 0x2
global Blood_MaskWallCount
Blood_MaskWallCount:
    resb 0x2
global Blood_SMostCount
Blood_SMostCount:
    resb 0x8
global Blood_PSkyBits
Blood_PSkyBits:
    resb 0x6
global Blood_NumSectors
Blood_NumSectors:
    resb 0x7f
global Blood_GlobalXShift
Blood_GlobalXShift:
    resb 0x1
global Blood_GlobalYShift
Blood_GlobalYShift:
    resb 0x1
global Blood_GlobParaCeilClip
Blood_GlobParaCeilClip:
    resb 0x1
global Blood_GlobParaFlorClip
Blood_GlobParaFlorClip:
    resb 0x9
global Blood_AutoMapping
Blood_AutoMapping:
    resb 0x1
global Blood_ParallaxType
Blood_ParallaxType:
    resb 0x1
global Blood_ShowInvisibility
Blood_ShowInvisibility:
    resb 0x1
global Blood_VidOption
Blood_VidOption:
    resb 0x4
global Blood_ErrorFileName
Blood_ErrorFileName:
    resb 0x4
global Blood_ErrorLineNum
Blood_ErrorLineNum:
    resb 0x172aee4

