
    #include <plib.h>

    #define AN0cfg()   (TRISBbits.TRISB12 = 0, ANSELBbits.ANSB12 = 0)   // RB12
    #define AN1cfg()   (TRISBbits.TRISB13 = 0, ANSELBbits.ANSB13 = 0)   // RB13
    #define AN2cfg()   TRISAbits.TRISA9 = 0    // RA9
    #define AN3cfg()   TRISAbits.TRISA10 = 0   // RA10

    #define SEG_CA(b)  {if(b) LATGCLR = 0x1000; else LATGSET = 0x1000;}  //RG12
    #define SEG_CB(b)  {if(b) LATACLR = 0x4000; else LATASET = 0x4000;}  //RA14
    #define SEG_CC(b)  {if(b) LATDCLR = 0x40;  else LATDSET = 0x40; }  //RD5
    #define SEG_CD(b)  {if(b) LATGCLR = 0x2000; else LATGSET = 0x2000;}  //RG13
    #define SEG_CE(b)  {if(b) LATGCLR = 0x8000; else LATGSET = 0x8000;}  //RG15
    #define SEG_CF(b)  {if(b) LATDCLR = 0x80 ; else LATDSET = 0x80; }  //RD6
    #define SEG_CG(b)  {if(b) LATDCLR = 0x2000; else LATDSET = 0x2000;}  //RD13
    #define SEG_DP(b)  {if(b) LATGCLR = 0x4000; else LATGSET = 0x4000;}  //RG14

    #define DIG_AN0(b) {if(b) LATBbits.LATB12 = 1; else LATBbits.LATB12 = 0;}  //RB12
    #define DIG_AN1(b) {if(b) LATBbits.LATB13 = 1; else LATBbits.LATB13 = 0;}  //RB13
    #define DIG_AN2(b) {if(b) LATAbits.LATA9 = 1;  else LATAbits.LATA9 = 0; }  //RA9
    #define DIG_AN3(b) {if(b) LATAbits.LATA10 = 1; else LATAbits.LATA10 = 0;}  //RA10

    #define SEG_LED0 {SEG_CA(1); SEG_CB(1);SEG_CC(1);SEG_CD(1);SEG_CE(1);SEG_CF(1);SEG_CG(0);SEG_DP(0);}
    #define SEG_LED1 {SEG_CA(0); SEG_CB(1);SEG_CC(1);SEG_CD(0);SEG_CE(0);SEG_CF(0);SEG_CG(0);SEG_DP(0);}
    #define SEG_LED2 {SEG_CA(1); SEG_CB(1);SEG_CC(0);SEG_CD(1);SEG_CE(1);SEG_CF(0);SEG_CG(1);SEG_DP(0);}
    #define SEG_LED3 {SEG_CA(1); SEG_CB(1);SEG_CC(1);SEG_CD(1);SEG_CE(0);SEG_CF(0);SEG_CG(1);SEG_DP(0);}
    #define SEG_LED4 {SEG_CA(0); SEG_CB(1);SEG_CC(1);SEG_CD(0);SEG_CE(0);SEG_CF(1);SEG_CG(1);SEG_DP(0);}
    #define SEG_LED5 {SEG_CA(1); SEG_CB(0);SEG_CC(1);SEG_CD(1);SEG_CE(0);SEG_CF(1);SEG_CG(1);SEG_DP(0);}
    #define SEG_LED6 {SEG_CA(1); SEG_CB(0);SEG_CC(1);SEG_CD(1);SEG_CE(1);SEG_CF(1);SEG_CG(1);SEG_DP(0);}
    #define SEG_LED7 {SEG_CA(1); SEG_CB(1);SEG_CC(1);SEG_CD(0);SEG_CE(0);SEG_CF(0);SEG_CG(0);SEG_DP(0);}
    #define SEG_LED8 {SEG_CA(1); SEG_CB(1);SEG_CC(1);SEG_CD(1);SEG_CE(1);SEG_CF(1);SEG_CG(1);SEG_DP(0);}
    #define SEG_LED9 {SEG_CA(1); SEG_CB(1);SEG_CC(1);SEG_CD(0);SEG_CE(0);SEG_CF(1);SEG_CG(1);SEG_DP(0);}
// ****** Continue the above lines to also display digits a to F:
    #define SEG_LEDA {SEG_CA(1); SEG_CB(1);SEG_CC(1);SEG_CD(0);SEG_CE(1);SEG_CF(1);SEG_CG(1);SEG_DP(0);}
    #define SEG_LEDB {SEG_CA(0); SEG_CB(0);SEG_CC(1);SEG_CD(1);SEG_CE(1);SEG_CF(1);SEG_CG(1);SEG_DP(0);}
    #define SEG_LEDC {SEG_CA(1); SEG_CB(0);SEG_CC(0);SEG_CD(1);SEG_CE(1);SEG_CF(1);SEG_CG(0);SEG_DP(0);}
    #define SEG_LEDD {SEG_CA(0); SEG_CB(1);SEG_CC(1);SEG_CD(1);SEG_CE(1);SEG_CF(0);SEG_CG(1);SEG_DP(0);}
    #define SEG_LEDE {SEG_CA(1); SEG_CB(0);SEG_CC(0);SEG_CD(1);SEG_CE(1);SEG_CF(1);SEG_CG(1);SEG_DP(0);}
    #define SEG_LEDF {SEG_CA(1); SEG_CB(0);SEG_CC(0);SEG_CD(0);SEG_CE(1);SEG_CF(1);SEG_CG(1);SEG_DP(0);}

    //#define SEG_NEG  {SEG_CA(0); SEG_CB(0);SEG_CC(0);SEG_CD(0);SEG_CE(0);SEG_CF(0);SEG_CG(1);SEG_DP(0);}
    #define SEG_OFF  {SEG_CA(0); SEG_CB(0);SEG_CC(0);SEG_CD(0);SEG_CE(0);SEG_CF(0);SEG_CG(0);SEG_DP(0);}
    #define DIGITS_OFF {DIG_AN0(1); DIG_AN1(1); DIG_AN2(1); DIG_AN3(1);}

