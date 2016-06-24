
#include "blood.hpp"

void Blood_QInterpolateDown16(intptr_t BufPtr, int32_t Count,
                              int32_t Value, int32_t Add)
{
    int32_t *IntPtr = (int32_t *)BufPtr;
    for (int32_t Index = 0; Index < Count; Index++)
    {
        IntPtr[Index] = (Value >> 16);
        Value += Add;
    }
}

void Blood_QInterpolateDown16Short(intptr_t BufPtr, int32_t Count,
                                   int32_t Value, int32_t Add)
{
    int16_t *IntPtr = (int16_t *)BufPtr;
    for (int32_t Index = 0; Index < Count; Index++)
    {
        IntPtr[Index] = (int16_t)(Value >> 16);
        Value += Add;
    }
}

void Blood_ClearBuffer(void *BufPtr, int32_t Count, int32_t Value)
{
    int32_t *IntPtr = (int32_t*)BufPtr;
    while ((Count--) > 0)
        *(IntPtr++) = Value;
}

void Blood_CopyBuffer(void *SrcPtr, void *DstPtr, int32_t Count)
{
    int32_t *IntSrcPtr = (int32_t*)SrcPtr;
    int32_t *IntDstPtr = (int32_t*)DstPtr;
    while ((Count--) > 0)
        *(IntDstPtr++) = *(IntSrcPtr++);
}

void Blood_SwapBuffer4(void *BufPtr1, void *BufPtr2, int32_t Count)
{
    int32_t *IntBufPtr1 = (int32_t*)BufPtr1;
    int32_t *IntBufPtr2 = (int32_t*)BufPtr2;
    while ((Count--) > 0)
    {
        int32_t X = *IntBufPtr2;
        int32_t Y = *IntBufPtr1;
        *(IntBufPtr2++) = Y;
        *(IntBufPtr1++) = X;
    }
}

void Blood_ClearBufferByte(void *BufPtr, int32_t Count, int32_t Value)
{
    int32_t Mask[4] = { 0xffl, 0xff00l, 0xff0000l, 0xff000000l };
    int32_t ShiftCount[4] = { 0, 8, 16, 24 };

    char *CharPtr = (char*)BufPtr;
    int32_t ArraysIndex = 0;

    while ((Count--) > 0)
    {
        *(CharPtr++) = (uint8_t)((Value & Mask[ArraysIndex]) >>
                                 ShiftCount[ArraysIndex]);
        ArraysIndex = (ArraysIndex + 1) & 3;
    }
}

void Blood_CopyBufferByte(void *SrcPtr, void *DstPtr, int32_t Count)
{
    char *CharSrcPtr = (char*)SrcPtr;
    char *CharDstPtr = (char*)DstPtr;
    while ((Count--) > 0)
        *(CharDstPtr++) = *(CharSrcPtr++);
}

void Blood_CopyBufferReverse(void *SrcPtr, void *DstPtr, int32_t Count)
{
    char *CharSrcPtr = (char*)SrcPtr;
    char *CharDstPtr = (char*)DstPtr;
    while ((Count--) > 0)
        *(CharDstPtr++) = *(CharSrcPtr--);
}
