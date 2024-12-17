#ifndef _C_DEF_H_
#define _C_DEF_H_


#undef BOOL
#undef TRUE
#undef FALSE
#undef U32
#undef UINT32
#undef ULONG
#undef LONG
#undef WORD
#undef DWORD


#define U64    unsigned long long
#define u64    unsigned long long
#define UINT64 unsigned long long
#define I64    long long
#define i64    long long
#define U32    unsigned long
#define u32    unsigned long
#define UINT32 unsigned long
#define INT32  long
#define I32    int
#define i32    int
#define ULONG  unsigned long
#define LONG   long
#define U16    unsigned short
#define u16    unsigned short
#define UINT16 unsigned short
#define I16    short int
#define i16    short int
#define U8     unsigned char
#define u8     unsigned char
#define UINT8  unsigned char
#define UCHAR  unsigned char
#define I8     signed char
#define i8     signed char
#define CHAR   signed char

#define s16     short int
//#define wchar_t unsigned short


#define WORD    unsigned short
#define DWORD   unsigned long
#define BYTE    unsigned char
#define BOOL    unsigned char

#define ULCD	unsigned long

#define TRUE	1
#define FALSE	0
#define true	1
#define false	0


typedef void (*FUNCTION) (void);
typedef void (*FUNCTION_PARM1) (U32 PARM1);
typedef void (*FUNCTION_PARM2) (U32 PARM1, U32 PARM2);
typedef void (*FUNCTION_PARM3) (U32 PARM1, U32 PARM2, U32 PARM3);


#undef NULL
#define NULL    ((void *)0)


#define BIT0_MASK       (1<<0)
#define BIT1_MASK		(1<<1)
#define BIT2_MASK		(1<<2)
#define BIT3_MASK		(1<<3)
#define BIT4_MASK       (1<<4)
#define BIT5_MASK       (1<<5)
#define BIT6_MASK       (1<<6)
#define BIT7_MASK       (1<<7)
#define BIT8_MASK       (1<<8)
#define BIT9_MASK		(1<<9)
#define BIT10_MASK		(1<<10)
#define BIT11_MASK		(1<<11)
#define BIT12_MASK      (1<<12)
#define BIT13_MASK      (1<<13)
#define BIT14_MASK      (1<<14)
#define BIT15_MASK      (1<<15)

#define BIT16_MASK      (1<<16)
#define BIT17_MASK      (1<<17)
#define BIT18_MASK      (1<<18)
#define BIT19_MASK      (1<<19)
#define BIT20_MASK      (1<<20)
#define BIT21_MASK      (1<<21)
#define BIT22_MASK      (1<<22)
#define BIT23_MASK      (1<<23)
#define BIT24_MASK      (1<<24)
#define BIT25_MASK      (1<<25)
#define BIT26_MASK      (1<<26)
#define BIT27_MASK      (1<<27)
#define BIT28_MASK      (1<<28)
#define BIT29_MASK      (1<<29)
#define BIT30_MASK      (1<<30)
#define BIT31_MASK      (1<<31)


#endif //_C_DEF_H_
