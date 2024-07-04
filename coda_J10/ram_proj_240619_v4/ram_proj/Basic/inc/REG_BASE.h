#ifndef _REG_BASE_H_
#define _REG_BASE_H_
#include <stdint.h>
#include <stdbool.h>


//#define BIT0                 ((uint32_t)0x00000001)  /*BIT 0 selected    */
//#define BIT1                 ((uint32_t)0x00000002)  /*BIT 1 selected    */
//#define BIT2                 ((uint32_t)0x00000004)  /*BIT 2 selected    */
//#define BIT3                 ((uint32_t)0x00000008)  /*BIT 3 selected    */
//#define BIT4                 ((uint32_t)0x00000010)  /*BIT 4 selected    */
//#define BIT5                 ((uint32_t)0x00000020)  /*BIT 5 selected    */
//#define BIT6                 ((uint32_t)0x00000040)  /*BIT 6 selected    */
//#define BIT7                 ((uint32_t)0x00000080)  /*BIT 7 selected    */
//#define BIT8                 ((uint32_t)0x00000100)  /*BIT 8 selected    */
//#define BIT9                 ((uint32_t)0x00000200)  /*BIT 9 selected    */
//#define BIT10                ((uint32_t)0x00000400)  /*BIT 10 selected   */
//#define BIT11                ((uint32_t)0x00000800)  /*BIT 11 selected   */
//#define BIT12                ((uint32_t)0x00001000)  /*BIT 12 selected   */
//#define BIT13                ((uint32_t)0x00002000)  /*BIT 13 selected   */
//#define BIT14                ((uint32_t)0x00004000)  /*BIT 14 selected   */
//#define BIT15                ((uint32_t)0x00008000)  /*BIT 15 selected   */
//#define BIT16                ((uint32_t)0x00010000)  /*BIT 16 selected    */
//#define BIT17                ((uint32_t)0x00020000)  /*BIT 17 selected    */
//#define BIT18                ((uint32_t)0x00040000)  /*BIT 18 selected    */
//#define BIT19                ((uint32_t)0x00080000)  /*BIT 19 selected    */
//#define BIT20                ((uint32_t)0x00100000)  /*BIT 20 selected    */
//#define BIT21                ((uint32_t)0x00200000)  /*BIT 21 selected    */
//#define BIT22                ((uint32_t)0x00400000)  /*BIT 22 selected    */
//#define BIT23                ((uint32_t)0x00800000)  /*BIT 23 selected    */
//#define BIT24                ((uint32_t)0x01000000)  /*BIT 24 selected    */
//#define BIT25                ((uint32_t)0x02000000)  /*BIT 25 selected    */
//#define BIT26                ((uint32_t)0x04000000)  /*BIT 26 selected   */
//#define BIT27                ((uint32_t)0x08000000)  /*BIT 27 selected   */
//#define BIT28                ((uint32_t)0x10000000)  /*BIT 28 selected   */
//#define BIT29                ((uint32_t)0x20000000)  /*BIT 29 selected   */
//#define BIT30                ((uint32_t)0x40000000)  /*BIT 30 selected   */
//#define BIT31                ((uint32_t)0x80000000)  /*BIT 31 selected   */  //bit31


//#define BIT(x)          ((uint32_t)(1<<x))



#define REG_1BITS_MASK          ((uint32_t)0x1)
#define REG_2BITS_MASK          ((uint32_t)0x3)
#define REG_3BITS_MASK          ((uint32_t)0x7)
#define REG_4BITS_MASK          ((uint32_t)0xF)
#define REG_5BITS_MASK          ((uint32_t)0x1F)
#define REG_6BITS_MASK          ((uint32_t)0x3F)
#define REG_7BITS_MASK          ((uint32_t)0x7F)
#define REG_8BITS_MASK          ((uint32_t)0xFF)
#define REG_9BITS_MASK          ((uint32_t)0x1FF)
#define REG_10BITS_MASK     ((uint32_t)0x3FF)
#define REG_11BITS_MASK     ((uint32_t)0x7FF)
#define REG_12BITS_MASK     ((uint32_t)0xFFF)
#define REG_13BITS_MASK     ((uint32_t)0x1FFF)
#define REG_14BITS_MASK     ((uint32_t)0x3FFF)
#define REG_15BITS_MASK     ((uint32_t)0x7FFF)
#define REG_16BITS_MASK     ((uint32_t)0xFFFF)
#define REG_17BITS_MASK     ((uint32_t)0x1FFFF)
#define REG_18BITS_MASK     ((uint32_t)0x3FFFF)
#define REG_19BITS_MASK     ((uint32_t)0x7FFFF)
#define REG_20BITS_MASK     ((uint32_t)0xFFFFF)
#define REG_21BITS_MASK     ((uint32_t)0x1FFFFF)
#define REG_23BITS_MASK     ((uint32_t)0x3FFFFF)
#define REG_24BITS_MASK     ((uint32_t)0x7FFFFF)
#define REG_25BITS_MASK     ((uint32_t)0xFFFFFF)



#define BIT_MASK(HighBit,LowBit)     ( BIT((HighBit) + 1- (LowBit))-1<<(LowBit))


static inline uint32_t REG_GetStatus(uint32_t *RegValue, uint32_t  StateBit)
{
    uint32_t bitstatus = 0x0;
    if ((RegValue && StateBit) != 0)
    {
        bitstatus = 1;
    }

    return bitstatus;
}


//#define HAL_READ32(base, addr)            \
//        (*((volatile unsigned int *)(base + addr)))
//
//#define HAL_WRITE32(base, addr, value32)  \
//        ((*((volatile unsigned int *)(base + addr))) = ((unsigned int) (value32)))

#define IO_READ32(base, addr)            \
    (*((volatile unsigned int *)(base + addr)))

#define IO_WRITE32(base, addr, value32)  \
    ((*((volatile unsigned int *)(base + addr))) = ((unsigned int) (value32)))

//void Set_RTCAon_REG(uint16_t REG_ADD,uint32_t Bit_Value);
//void Set_Aon_REG_Bit(uint16_t REG_ADD,uint32_t Bit_Shift,uint8_t Bit_Value);
//void Set_Aon_REG_Bit(uint16_t REG_ADD,uint32_t Bit_Shift,uint8_t Bit_Value);

#endif
