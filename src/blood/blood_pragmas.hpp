
// "Build Engine & Tools" Copyright (c) 1993-1997 Ken Silverman
// Ken Silverman's official web site: "http://www.advsys.net/ken"
// See the included license file "BUILDLIC.TXT" for license info.

#ifndef __BLOOD_PRAGMAS_HPP__
#define __BLOOD_PRAGMAS_HPP__

#define Blood_DivScaleMacro(ShiftBits) \
    static inline int32_t Blood_DivScale##ShiftBits(int32_t eax, int32_t ebx) \
{ \
    return (int32_t)((((int64_t)eax) << ShiftBits) / ((int64_t)ebx)); \
}
Blood_DivScaleMacro( 1) Blood_DivScaleMacro( 2) Blood_DivScaleMacro( 3) Blood_DivScaleMacro( 4)
Blood_DivScaleMacro( 5) Blood_DivScaleMacro( 6) Blood_DivScaleMacro( 7) Blood_DivScaleMacro( 8)
Blood_DivScaleMacro( 9) Blood_DivScaleMacro(10) Blood_DivScaleMacro(11) Blood_DivScaleMacro(12)
Blood_DivScaleMacro(13) Blood_DivScaleMacro(14) Blood_DivScaleMacro(15) Blood_DivScaleMacro(16)
Blood_DivScaleMacro(17) Blood_DivScaleMacro(18) Blood_DivScaleMacro(19) Blood_DivScaleMacro(20)
Blood_DivScaleMacro(21) Blood_DivScaleMacro(22) Blood_DivScaleMacro(23) Blood_DivScaleMacro(24)
Blood_DivScaleMacro(25) Blood_DivScaleMacro(26) Blood_DivScaleMacro(27) Blood_DivScaleMacro(28)
Blood_DivScaleMacro(29) Blood_DivScaleMacro(30) Blood_DivScaleMacro(31) Blood_DivScaleMacro(32)
#undef Blood_DivScaleMacro

static inline int32_t Blood_DivScale(int32_t eax, int32_t ebx, int32_t ecx)
{
    return (int32_t)((((int64_t)eax) << ((uint8_t)ecx)) / ((uint64_t)ebx));
}

#define Blood_MulScaleMacro(ShiftBits) \
static inline int32_t Blood_MulScale##ShiftBits(int32_t eax, int32_t edx) \
{ \
    return (int32_t)((((int64_t)eax) * ((int64_t)edx)) >> ShiftBits); \
} \
static inline int32_t Blood_DMulScale##ShiftBits(int32_t eax, int32_t edx, \
                                                 int32_t esi, int32_t edi) \
{ \
    return (int32_t)(((((int64_t)eax) * ((int64_t)edx)) + \
                      (((int64_t)esi) * ((int64_t)edi))) >> ShiftBits); \
} \
static inline int32_t Blood_TMulScale##ShiftBits(int32_t eax, int32_t edx, int32_t ebx, \
                                                 int32_t ecx, int32_t esi, int32_t edi) \
{ \
    return (int32_t)(((((int64_t)eax) * ((int64_t)edx)) + \
                    (((int64_t)ebx) * ((int64_t)ecx)) + \
                    (((int64_t)esi) * ((int64_t)edi))) >> ShiftBits); \
}
Blood_MulScaleMacro( 1) Blood_MulScaleMacro( 2) Blood_MulScaleMacro( 3) Blood_MulScaleMacro( 4)
Blood_MulScaleMacro( 5) Blood_MulScaleMacro( 6) Blood_MulScaleMacro( 7) Blood_MulScaleMacro( 8)
Blood_MulScaleMacro( 9) Blood_MulScaleMacro(10) Blood_MulScaleMacro(11) Blood_MulScaleMacro(12)
Blood_MulScaleMacro(13) Blood_MulScaleMacro(14) Blood_MulScaleMacro(15) Blood_MulScaleMacro(16)
Blood_MulScaleMacro(17) Blood_MulScaleMacro(18) Blood_MulScaleMacro(19) Blood_MulScaleMacro(20)
Blood_MulScaleMacro(21) Blood_MulScaleMacro(22) Blood_MulScaleMacro(23) Blood_MulScaleMacro(24)
Blood_MulScaleMacro(25) Blood_MulScaleMacro(26) Blood_MulScaleMacro(27) Blood_MulScaleMacro(28)
Blood_MulScaleMacro(29) Blood_MulScaleMacro(30) Blood_MulScaleMacro(31) Blood_MulScaleMacro(32)
#undef Blood_MulScaleMacro

static inline int32_t Blood_Scale(int32_t eax, int32_t edx, int32_t ecx)
{
    return (int32_t)((((int64_t)eax) * ((int64_t)edx)) / ((int64_t)ecx));
}

void Blood_QInterpolateDown16(intptr_t BufPtr, int32_t Count, int32_t Value, int32_t Add);
void Blood_QInterpolateDown16Short(intptr_t BufPtr, int32_t Count, int32_t Value, int32_t Add);
void Blood_ClearBuffer(void *BufPtr, int32_t Count, int32_t Value);
void Blood_CopyBuffer(void *SrcPtr, void *DstPtr, int32_t Count);
void Blood_SwapBuffer4(void *BufPtr1, void *BufPtr2, int32_t Count);
void Blood_ClearBufferByte(void *BufPtr, int32_t Count, int32_t Value);
void Blood_CopyBufferByte(void *SrcPtr, void *DstPtr, int32_t Count);
void Blood_CopyBufferReverse(void *SrcPtr, void *DstPtr, int32_t Count);

#endif /* __BLOOD_PRAGMAS_HPP__ */
