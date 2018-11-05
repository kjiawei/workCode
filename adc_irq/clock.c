#define APLL_LOCK	( *((volatile unsigned long *)0xE0100000) )		
#define MPLL_LOCK	( *((volatile unsigned long *)0xE0100008) )

#define APLL_CON0	( *((volatile unsigned long *)0xE0100100) )
#define APLL_CON1	( *((volatile unsigned long *)0xE0100104) )
#define MPLL_CON 	( *((volatile unsigned long *)0xE0100108) )

#define CLK_SRC0	( *((volatile unsigned long *)0xE0100200) )
#define CLK_SRC1	( *((volatile unsigned long *)0xE0100204) )
#define CLK_SRC2	( *((volatile unsigned long *)0xE0100208) )
#define CLK_SRC3	( *((volatile unsigned long *)0xE010020c) )
#define CLK_SRC4	( *((volatile unsigned long *)0xE0100210) )
#define CLK_SRC5	( *((volatile unsigned long *)0xE0100214) )
#define CLK_SRC6	( *((volatile unsigned long *)0xE0100218) )
#define CLK_SRC_MASK0	( *((volatile unsigned long *)0xE0100280) )
#define CLK_SRC_MASK1 	( *((volatile unsigned long *)0xE0100284) )

#define CLK_DIV0	( *((volatile unsigned long *)0xE0100300) )
#define CLK_DIV1	( *((volatile unsigned long *)0xE0100304) )
#define CLK_DIV2	( *((volatile unsigned long *)0xE0100308) )
#define CLK_DIV3	( *((volatile unsigned long *)0xE010030c) )
#define CLK_DIV4	( *((volatile unsigned long *)0xE0100310) )
#define CLK_DIV5	( *((volatile unsigned long *)0xE0100314) )
#define CLK_DIV6	( *((volatile unsigned long *)0xE0100318) )
#define CLK_DIV7	( *((volatile unsigned long *)0xE010031c) )

#define CLK_DIV0_MASK	0x7fffffff

#define APLL_MDIV       0x7d
#define APLL_PDIV       0x3
#define APLL_SDIV      	0x1
#define MPLL_MDIV	0x29b
#define MPLL_PDIV	0xc
#define MPLL_SDIV	0x1

#define set_pll(mdiv, pdiv, sdiv)	(1<<31 | mdiv<<16 | pdiv<<8 | sdiv)
#define APLL_VAL	set_pll(APLL_MDIV,APLL_PDIV,APLL_SDIV)
#define MPLL_VAL	set_pll(MPLL_MDIV,MPLL_PDIV,MPLL_SDIV)
	
	//PCLK_PSYS = HCLK_PSYS/2  ;PCLK_PSYS=66MHz
	//HCLK_PSYS = MOUT_PSYS/5  ;HCLK_PSYS=133MHz
	//PCLK_DSYS = HCLK_DSYS/2  ;PCLK_DSYS=83MHz
	//HCLK_DSYS = MOUT_DSYS/4  ;HCLK_DSYS=166MHz
	//PCLK_MSYS = HCLK_MSYS/2  ;PCLK_MSYS=100MHz
	//HCLK_MSYS = ARMCLK/5     ;HCLK_MSYS=200MHz
	//SCLKA2M = SCLKAPLL/4
	//ARMCLK = MOUT_MSYS       ;ARMCLK   =1000MHz

void clock_init()
{
	CLK_SRC0 = 0x0;//Select clock source 0 (Main)
	APLL_LOCK = 0x0000FFFF; //PLL locking period for APLL.         			
	MPLL_LOCK = 0x0000FFFF; //locking period for MPLL.					
    
	//Set clock divider ratio 0 (System Clocks)
	CLK_DIV0 = ((1<<28)|(4<<24)|(1<<20)|(3<<16)|(1<<12)|(4<<8)|(4<<4)|0);
    
	//Control PLL output frequency for APLL.	
	//FOUT= MDIV * FIN / (PDIV*2^(SDIV-1)) = 0x7d*24/(0x3*2^(1-1))=1000 MHz
	APLL_CON0 = APLL_VAL;  //MOUT_MSYS = 1000MHz

	//Control PLL output frequency for MPLL
	//FOUT = MDIV*FIN/(PDIV*2^SDIV)=0x29b*24/(0xc*2^1)= 667 MHz
	MPLL_CON  = MPLL_VAL;//MOUT_DSYS = 667MHz, MOUT_PSYS =667MHz			
    
	CLK_SRC0 = 0x10001111;
}


