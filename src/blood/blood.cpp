
#include "blood.hpp"

static uint32_t BloodBox_CRC32Table[256] =
{
    0x00000000, 0x77073096, 0xee0e612c, 0x990951ba, 0x076dc419, 0x706af48f,
    0xe963a535, 0x9e6495a3, 0x0edb8832, 0x79dcb8a4, 0xe0d5e91e, 0x97d2d988,
    0x09b64c2b, 0x7eb17cbd, 0xe7b82d07, 0x90bf1d91, 0x1db71064, 0x6ab020f2,
    0xf3b97148, 0x84be41de, 0x1adad47d, 0x6ddde4eb, 0xf4d4b551, 0x83d385c7,
    0x136c9856, 0x646ba8c0, 0xfd62f97a, 0x8a65c9ec, 0x14015c4f, 0x63066cd9,
    0xfa0f3d63, 0x8d080df5, 0x3b6e20c8, 0x4c69105e, 0xd56041e4, 0xa2677172,
    0x3c03e4d1, 0x4b04d447, 0xd20d85fd, 0xa50ab56b, 0x35b5a8fa, 0x42b2986c,
    0xdbbbc9d6, 0xacbcf940, 0x32d86ce3, 0x45df5c75, 0xdcd60dcf, 0xabd13d59,
    0x26d930ac, 0x51de003a, 0xc8d75180, 0xbfd06116, 0x21b4f4b5, 0x56b3c423,
    0xcfba9599, 0xb8bda50f, 0x2802b89e, 0x5f058808, 0xc60cd9b2, 0xb10be924,
    0x2f6f7c87, 0x58684c11, 0xc1611dab, 0xb6662d3d, 0x76dc4190, 0x01db7106,
    0x98d220bc, 0xefd5102a, 0x71b18589, 0x06b6b51f, 0x9fbfe4a5, 0xe8b8d433,
    0x7807c9a2, 0x0f00f934, 0x9609a88e, 0xe10e9818, 0x7f6a0dbb, 0x086d3d2d,
    0x91646c97, 0xe6635c01, 0x6b6b51f4, 0x1c6c6162, 0x856530d8, 0xf262004e,
    0x6c0695ed, 0x1b01a57b, 0x8208f4c1, 0xf50fc457, 0x65b0d9c6, 0x12b7e950,
    0x8bbeb8ea, 0xfcb9887c, 0x62dd1ddf, 0x15da2d49, 0x8cd37cf3, 0xfbd44c65,
    0x4db26158, 0x3ab551ce, 0xa3bc0074, 0xd4bb30e2, 0x4adfa541, 0x3dd895d7,
    0xa4d1c46d, 0xd3d6f4fb, 0x4369e96a, 0x346ed9fc, 0xad678846, 0xda60b8d0,
    0x44042d73, 0x33031de5, 0xaa0a4c5f, 0xdd0d7cc9, 0x5005713c, 0x270241aa,
    0xbe0b1010, 0xc90c2086, 0x5768b525, 0x206f85b3, 0xb966d409, 0xce61e49f,
    0x5edef90e, 0x29d9c998, 0xb0d09822, 0xc7d7a8b4, 0x59b33d17, 0x2eb40d81,
    0xb7bd5c3b, 0xc0ba6cad, 0xedb88320, 0x9abfb3b6, 0x03b6e20c, 0x74b1d29a,
    0xead54739, 0x9dd277af, 0x04db2615, 0x73dc1683, 0xe3630b12, 0x94643b84,
    0x0d6d6a3e, 0x7a6a5aa8, 0xe40ecf0b, 0x9309ff9d, 0x0a00ae27, 0x7d079eb1,
    0xf00f9344, 0x8708a3d2, 0x1e01f268, 0x6906c2fe, 0xf762575d, 0x806567cb,
    0x196c3671, 0x6e6b06e7, 0xfed41b76, 0x89d32be0, 0x10da7a5a, 0x67dd4acc,
    0xf9b9df6f, 0x8ebeeff9, 0x17b7be43, 0x60b08ed5, 0xd6d6a3e8, 0xa1d1937e,
    0x38d8c2c4, 0x4fdff252, 0xd1bb67f1, 0xa6bc5767, 0x3fb506dd, 0x48b2364b,
    0xd80d2bda, 0xaf0a1b4c, 0x36034af6, 0x41047a60, 0xdf60efc3, 0xa867df55,
    0x316e8eef, 0x4669be79, 0xcb61b38c, 0xbc66831a, 0x256fd2a0, 0x5268e236,
    0xcc0c7795, 0xbb0b4703, 0x220216b9, 0x5505262f, 0xc5ba3bbe, 0xb2bd0b28,
    0x2bb45a92, 0x5cb36a04, 0xc2d7ffa7, 0xb5d0cf31, 0x2cd99e8b, 0x5bdeae1d,
    0x9b64c2b0, 0xec63f226, 0x756aa39c, 0x026d930a, 0x9c0906a9, 0xeb0e363f,
    0x72076785, 0x05005713, 0x95bf4a82, 0xe2b87a14, 0x7bb12bae, 0x0cb61b38,
    0x92d28e9b, 0xe5d5be0d, 0x7cdcefb7, 0x0bdbdf21, 0x86d3d2d4, 0xf1d4e242,
    0x68ddb3f8, 0x1fda836e, 0x81be16cd, 0xf6b9265b, 0x6fb077e1, 0x18b74777,
    0x88085ae6, 0xff0f6a70, 0x66063bca, 0x11010b5c, 0x8f659eff, 0xf862ae69,
    0x616bffd3, 0x166ccf45, 0xa00ae278, 0xd70dd2ee, 0x4e048354, 0x3903b3c2,
    0xa7672661, 0xd06016f7, 0x4969474d, 0x3e6e77db, 0xaed16a4a, 0xd9d65adc,
    0x40df0b66, 0x37d83bf0, 0xa9bcae53, 0xdebb9ec5, 0x47b2cf7f, 0x30b5ffe9,
    0xbdbdf21c, 0xcabac28a, 0x53b39330, 0x24b4a3a6, 0xbad03605, 0xcdd70693,
    0x54de5729, 0x23d967bf, 0xb3667a2e, 0xc4614ab8, 0x5d681b02, 0x2a6f2b94,
    0xb40bbe37, 0xc30c8ea1, 0x5a05df1b, 0x2d02ef8d,
};

uint32_t BloodBox_CRC32(uint32_t In, uint8_t *Data, uint32_t Size)
{
    uint32_t Checksum = In ^ 0xffffffff;
    for (uint32_t i = 0; i < Size; i++)
        Checksum = (Checksum >> 8) ^ BloodBox_CRC32Table[(Checksum ^ Data[i]) & 0xff];
    return Checksum ^ 0xffffffff;
}

int32_t BloodBox_Startup(void)
{
    FILE *fp;
    uint32_t Size, Checksum;
    uint32_t OkayToGo;
    uint8_t *Buffer;

    OkayToGo = 1;
    Buffer = new uint8_t[1442615];

    fp = fopen("dos32a.exe", "rb");
    if (fp)
    {
        fseek(fp, 0, SEEK_END);
        Size = ftell(fp);
        if (Size == 27504)
        {
            fseek(fp, 0, SEEK_SET);
            fread(Buffer, 1, Size, fp);
            Checksum = BloodBox_CRC32(0x424c5544, Buffer, Size);
            if (Checksum != 0x35e4fff0)
            {
                LOG_MSG("ERROR: Wrong CRC32 checksum of dos32a.exe\n");
                OkayToGo = 0;
            }
        }
        else
        {
            LOG_MSG("ERROR: dos32a.exe is of the wrong size\n");
            OkayToGo = 0;
        }
        fclose(fp);
    }
    else
    {
        LOG_MSG("ERROR: Could not find dos32a.exe\n");
        OkayToGo = 0;
    }

    fp = fopen("BLOOD.EXE", "rb");
    if (fp)
    {
        fseek(fp, 0, SEEK_END);
        Size = ftell(fp);
        if (Size == 1442615)
        {
            fseek(fp, 0, SEEK_SET);
            fread(Buffer, 1, Size, fp);
            Checksum = BloodBox_CRC32(0x424c5544, Buffer, Size);
            if (Checksum != 0x6f7f04ab)
            {
                LOG_MSG("ERROR: Wrong CRC32 checksum of BLOOD.EXE\n");
                OkayToGo = 0;
            }
        }
        else
        {
            LOG_MSG("ERROR: BLOOD.EXE is of the wrong size\n");
            OkayToGo = 0;
        }
        fclose(fp);
    }
    else
    {
        LOG_MSG("ERROR: Could not find BLOOD.EXE\n");
        OkayToGo = 0;
    }

    delete Buffer;
    return OkayToGo;
}

static void Blood_WriteInt(uint8_t *Ptr, uint32_t ID,
                           uint8_t Returns, uint16_t RetCount,
                           uint8_t NopCount)
{
    *Ptr++ = 0x50; /* push eax */
    *Ptr++ = 0xb8; /* mov eax, id */
    *Ptr++ = (ID      ) & 0xff;
    *Ptr++ = (ID >>  8) & 0xff;
    *Ptr++ = (ID >> 16) & 0xff;
    *Ptr++ = (ID >> 24);
    *Ptr++ = 0xcd; /* int 0xbd */
    *Ptr++ = 0xbd;

    if (Returns)
    {
        if (!RetCount) {
            *Ptr++ = 0xc3; /* retn */
        } else {
            *Ptr++ = 0xc2; /* retn x */
            *Ptr++ = (RetCount     ) & 0xff;
            *Ptr++ = (RetCount >> 8) & 0xff;
        }
    }

    while (NopCount--)
        *Ptr++ = 0x90; /* nop */
}

static void Blood_Init(void)
{
    /*
     * We're now in dos32a.exe, about to jump to BLOOD.EXE's
     * entry point
     *
     * 0x66 0x33 0xc0 - xor eax, eax
     * 0x66 0x33 0xdb - xor ebx, ebx
    */

    reg_eax = 0;
    reg_ebx = 0;

    /* blood_build.hpp */
    Blood_WriteInt(CSeg01Ptr(0x9bd17), BloodInt_SetAspect, 1, 0, 0);
    Blood_WriteInt(CSeg01Ptr(0x9bd7c), BloodInt_DoSetAspect, 1, 0, 0);
    Blood_WriteInt(CSeg01Ptr(0x9bb50), BloodInt_SetView, 1, 0, 0);

    /*
     * 0x50                     - push eax
     * 0xb8 0x01 0x00 0x00 0x00 - mov eax, 1 (BloodInt_Return)
     * 0xcd 0xbd                - int 0xbd
     * 0xeb 0xfe                - jmp $-2 (label: jmp label)
     *
     * See Blood_Call above
    */
    uint8_t *Ptr = (uint8_t *)CSeg01Ptr(0x9bb60);
    *Ptr++ = 0x50; *Ptr++ = 0xb8; *Ptr++ = 0x01; *Ptr++ = 0x00;
    *Ptr++ = 0x00; *Ptr++ = 0x00; *Ptr++ = 0xcd; *Ptr++ = 0xbd;
    *Ptr++ = 0xeb; *Ptr++ = 0xfe;

    Blood_WriteInt(CSeg01Ptr(0x8923e), BloodInt_DrawRooms, 1, 8, 0);
    Blood_WriteInt(CSeg01Ptr(0x898e7), BloodInt_ScanSector, 1, 0, 0);
    Blood_WriteInt(CSeg01Ptr(0xa1d0a), BloodInt_GetZsOfSlope, 1, 4, 0);
    Blood_WriteInt(CSeg01Ptr(0x900b4), BloodInt_Inside, 1, 0, 0);
    Blood_WriteInt(CSeg01Ptr(0x99224), BloodInt_UpdateSector, 1, 0, 0);
    Blood_WriteInt(CSeg01Ptr(0x8a357), BloodInt_DrawWalls, 1, 0, 0);
    Blood_WriteInt(CSeg01Ptr(0x8c7de), BloodInt_WallScan, 1, 8, 0);
    Blood_WriteInt(CSeg01Ptr(0x8c04b), BloodInt_FlorScan, 1, 0, 0);
    Blood_WriteInt(CSeg01Ptr(0x8b8da), BloodInt_CeilScan, 1, 0, 0);
    Blood_WriteInt(CSeg01Ptr(0xa3a83), BloodInt_ParaScan, 1, 4, 0);
    Blood_WriteInt(CSeg01Ptr(0xa2df9), BloodInt_GrouScan, 1, 0, 0);

    /* blood_db.hpp */
    Blood_WriteInt(CSeg01Ptr(0x1f174), BloodInt_DB_InsertSpriteSect, 1, 0, 0);
    Blood_WriteInt(CSeg01Ptr(0x1f248), BloodInt_DB_DeleteSpriteSect, 1, 0, 0);
    Blood_WriteInt(CSeg01Ptr(0x1f330), BloodInt_DB_InsertSpriteStat, 1, 0, 0);
    Blood_WriteInt(CSeg01Ptr(0x1f40c), BloodInt_DB_DeleteSpriteStat, 1, 0, 0);
    Blood_WriteInt(CSeg01Ptr(0x1f50c), BloodInt_DB_InitSprites, 1, 0, 0);
    Blood_WriteInt(CSeg01Ptr(0x1f5a4), BloodInt_DB_SpawnSprite, 1, 0, 0);
    Blood_WriteInt(CSeg01Ptr(0x1f65c), BloodInt_DB_DespawnSprite, 1, 0, 0);
    Blood_WriteInt(CSeg01Ptr(0x1f710), BloodInt_DB_MoveSpriteSect, 1, 0, 0);
    Blood_WriteInt(CSeg01Ptr(0x1f7e0), BloodInt_DB_MoveSpriteStat, 1, 0, 0);
    Blood_WriteInt(CSeg01Ptr(0x1f8dc), BloodInt_DB_NewXSprite, 1, 0, 0);
    Blood_WriteInt(CSeg01Ptr(0x1f990), BloodInt_DB_DelXSprite, 1, 0, 0);
    Blood_WriteInt(CSeg01Ptr(0x1fa74), BloodInt_DB_NewXWall, 1, 0, 0);
    Blood_WriteInt(CSeg01Ptr(0x1fb24), BloodInt_DB_DelXWall, 1, 0, 0);
    Blood_WriteInt(CSeg01Ptr(0x1fbb0), BloodInt_DB_NewXSector, 1, 0, 0);
    Blood_WriteInt(CSeg01Ptr(0x1fc64), BloodInt_DB_DelXSector, 1, 0, 0);
}

/* See Normal_Loop() in dosbox.cpp */
uint32_t Blood_Status;
static int32_t Blood_ProcID;
int32_t Blood_OldCycles;
static uint32_t ReturnStackIndex;
static uint32_t ReturnStack[4096];

void Blood_Call(uint32_t Address)
{
    ReturnStack[ReturnStackIndex++] = reg_eip;

    CPU_Push32(IDA2LinearCSeg01(0x9bb60));
    reg_eip = Address;
    DOSBOX_RunMachine();

    reg_eip = ReturnStack[--ReturnStackIndex];
}

void Blood_DoInterrupt(void)
{
    int32_t *Stack32 = (int32_t *)(((int8_t *)MemBase) + reg_esp);

    switch (Blood_ProcID)
    {
    case BloodInt_Init:
        Blood_Init();
        break;
    case BloodInt_Return:
        Blood_Status = 2;
        break;

    /* blood_build.hpp */
    case BloodInt_SetAspect:
        Blood_SetAspect(reg_eax, reg_edx);
        break;
    case BloodInt_DoSetAspect:
        Blood_DoSetAspect();
        break;
    case BloodInt_SetView:
        Blood_SetView(reg_eax, reg_edx, reg_ebx, reg_ecx);
        break;
    case BloodInt_DrawRooms:
        /*Blood_ShowInvisibility = 1;*/
        Blood_DrawRooms(reg_eax, reg_edx, reg_ebx, reg_ecx, Stack32[1],
                        Stack32[2]);
        break;
    case BloodInt_ScanSector:
        Blood_ScanSector(reg_eax);
        break;
    case BloodInt_GetZsOfSlope:
        Blood_GetZsOfSlope(reg_eax, reg_edx, reg_ebx,
                           (int32_t *)&MemBase[reg_ecx],
                           (int32_t *)&MemBase[Stack32[1]]);
        break;
    case BloodInt_Inside:
        reg_eax = Blood_Inside(reg_eax, reg_edx, reg_ebx);
        break;
    case BloodInt_UpdateSector:
        Blood_UpdateSector(reg_eax, reg_edx, (short *)&MemBase[reg_ebx]);
        break;
    case BloodInt_DrawWalls:
        Blood_DrawWalls(reg_eax);
        break;
    case BloodInt_WallScan:
        Blood_WallScan(reg_eax, reg_edx,
                       (int16_t *)&MemBase[reg_ebx],
                       (int16_t *)&MemBase[reg_ecx],
                       (int32_t *)&MemBase[Stack32[1]],
                       (int32_t *)&MemBase[Stack32[2]]);
        break;
    case BloodInt_FlorScan:
        Blood_FlorScan(reg_eax, reg_edx, reg_ebx);
        break;
    case BloodInt_CeilScan:
        Blood_CeilScan(reg_eax, reg_edx, reg_ebx);
        break;
    case BloodInt_ParaScan:
        Blood_ParaScan(reg_eax, reg_edx, reg_ebx, reg_ecx,
                       Stack32[1]);
        break;
    case BloodInt_GrouScan:
        Blood_GrouScan(reg_eax, reg_edx, reg_ebx, reg_ecx);
        break;

    /* blood_db.hpp */
    case BloodInt_DB_InsertSpriteSect:
        Blood_DB_InsertSpriteSect(reg_eax, reg_edx);
        break;
    case BloodInt_DB_DeleteSpriteSect:
        Blood_DB_DeleteSpriteSect(reg_eax);
        break;
    case BloodInt_DB_InsertSpriteStat:
        Blood_DB_InsertSpriteStat(reg_eax, reg_edx);
        break;
    case BloodInt_DB_DeleteSpriteStat:
        Blood_DB_DeleteSpriteStat(reg_eax);
        break;
    case BloodInt_DB_InitSprites:
        Blood_DB_InitSprites();
        break;
    case BloodInt_DB_SpawnSprite:
        reg_eax = Blood_DB_SpawnSprite(reg_eax, reg_edx);
        break;
    case BloodInt_DB_DespawnSprite:
        Blood_DB_DespawnSprite(reg_eax);
        break;
    case BloodInt_DB_MoveSpriteSect:
        reg_eax = Blood_DB_MoveSpriteSect(reg_eax, reg_edx);
        break;
    case BloodInt_DB_MoveSpriteStat:
        reg_eax = Blood_DB_MoveSpriteStat(reg_eax, reg_edx);
        break;
    case BloodInt_DB_NewXSprite:
        reg_eax = Blood_DB_NewXSprite(reg_eax);
        break;
    case BloodInt_DB_DelXSprite:
        Blood_DB_DelXSprite(reg_eax);
        break;
    case BloodInt_DB_NewXWall:
        reg_eax = Blood_DB_NewXWall(reg_eax);
        break;
    case BloodInt_DB_DelXWall:
        Blood_DB_DelXWall(reg_eax);
        break;
    case BloodInt_DB_NewXSector:
        reg_eax = Blood_DB_NewXSector(reg_eax);
        break;
    case BloodInt_DB_DelXSector:
        Blood_DB_DelXSector(reg_eax);
        break;

    default:
        LOG_MSG("WARNING: Blood interrupt called with invalid eax value!\n");
        break;
    };
}

void Blood_Interrupt(void)
{
    Blood_ProcID = reg_eax;
    if (Blood_ProcID != BloodInt_Init)
        reg_eax = CPU_Pop32();

    Blood_Status = 1;
    Blood_OldCycles = CPU_Cycles;
    CPU_Cycles = 0;
}

void Blood_ExitError(int32_t LineNum, uint32_t FileNameString,
                     uint32_t ErrorMsgString)
{
    reg_eax = ErrorMsgString;
    reg_edx = FileNameString;
    reg_ebx = LineNum;
    Blood_Call(Blood_ProcAddress_ExitError);
}

void Blood_SetErrorInfo(int32_t LineNum, uint32_t FileNameString)
{
    Blood_ErrorLineNum = LineNum;
    Blood_ErrorFileName = FileNameString;
}

/* TODO: Variable number of args */
void Blood_ExitError1(uint32_t ErrorMsgString)
{
    CPU_Push32(ErrorMsgString);
    Blood_Call(Blood_ProcAddress_ExitError1);
}
