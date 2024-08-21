// Microcomputer Systems lab04 Summer 2024
// Eight students and 4 textbooks
// Shows the numbers of the requesting students with the highest priorities
// among all the requesting students.
#include "7SegForLab04.h"          // Bit banging is done here
#include <plib.h>           // Peripherial library 

// configuration bit settings, Fcy=80MHz, Fpb=40MHz
#pragma config POSCMOD=XT, FNOSC=PRIPLL
#pragma config FPLLIDIV=DIV_2, FPLLMUL=MUL_20
#pragma config FPLLODIV=DIV_1, FPBDIV=DIV_2
#pragma config FWDTEN=OFF, CP=OFF, BWP=OFF

// function "turnOnSeg" normally receives a 4-bit value (n), converts it to 7-segment, 
// code word and sends it to 7-segment bus 

void turnOnSeg(int n) {
    switch (n) {
        case 0:
            SEG_LED0
            break;
        case 1:
            SEG_LED1
            break;
        case 2:
            SEG_LED2
            break;
        case 3:
            SEG_LED3
            break;
        case 4:
            SEG_LED4
            break;
        case 5:
            SEG_LED5
            break;
        case 6:
            SEG_LED6
            break;
        case 7:
            SEG_LED7
            break;
        case 8:
            SEG_LED8
            break;
        case 9:
            SEG_LED9
            break;
        case 10:
            SEG_LEDA
            break;
        case 11:
            SEG_LEDB
            break;
        case 12:
            SEG_LEDC
            break;
        case 13:
            SEG_LEDD
            break;
        case 14:
            SEG_LEDE
            break;
        case 15:
            SEG_LEDF
            break;
        default:
            SEG_OFF
    } // Switch           
} // turnOnSeg

char buffer[] = {
    16, 16, 16, 16
}; // end of buffer

int buttonIsActive = 0;

void shiftIn(int value)
{
    int i = 4;
    //shift all values over by one
    for(i = 4; i > 0; i--)
    {
        buffer[i] = buffer[i - 1];
    }
    
    //put in the new value
    buffer[0] = value;
}

/////////////////////////////// ISR ///////////////////////////  
// Single-vector Interrupt: There is only one interrupt source in this project: Timer3.
// The ISR checks the 8 request lines (slide switches), and then puts the numbers of all the 
// asserted lines in the buffer. The line (7) with the highest priority is checked first.

void __ISR_SINGLE() T3ISR(void) {
    ///////// Monitor the ISR by toggling LED0 ///////// 
    LATAINV = 1; // Flip LED0
    //////////////////////////////////////////////////// 

    IFS0bits.T3IF = 0; // Reset Interrupt Flag Status bit.
    
    int j = 0;
    
    //setup 4 collumn
    TRISCbits.TRISC2 = 0;
    PORTCbits.RC2 = 0;
    
    if (PORTGbits.RG9 == 0 && buttonIsActive == 0) //0 means the button is pushed
    {
        shiftIn(0xA);
        buttonIsActive = 1;
    }
    else if (PORTGbits.RG8 == 0 && buttonIsActive == 0) //0 means the button is pushed
    {
        shiftIn(0xB);
        buttonIsActive = 1;
    }
    else if (PORTGbits.RG7 == 0 && buttonIsActive == 0) //0 means the button is pushed
    {
        shiftIn(0xC);
        buttonIsActive = 1;
    }
    else if (PORTCbits.RC3 == 0 && buttonIsActive == 0) //0 means the button is pushed
    {
        shiftIn(0xD);
        buttonIsActive = 1;
    }
    
    //reset
    TRISCbits.TRISC2 = 1;
    
    //for (j = 0; j < 40; j++); // wait
    
    //setup 3 collumn
    TRISCbits.TRISC1 = 0;
    PORTCbits.RC1 = 0;
    
    if (PORTGbits.RG9 == 0 && buttonIsActive == 0) //0 means the button is pushed
    {
        shiftIn(0x3);
        buttonIsActive = 1;
    }
    else if (PORTGbits.RG8 == 0 && buttonIsActive == 0) //0 means the button is pushed
    {
        shiftIn(0x6);
        buttonIsActive = 1;
    }
    else if (PORTGbits.RG7 == 0 && buttonIsActive == 0) //0 means the button is pushed
    {
        shiftIn(0x9);
        buttonIsActive = 1;
    }
    else if (PORTCbits.RC3 == 0 && buttonIsActive == 0) //0 means the button is pushed
    {
        shiftIn(0xE);
        buttonIsActive = 1;
    }

    //reset
    TRISCbits.TRISC1 = 1;
    
    //for (j = 0; j < 40; j++); // wait
    
    //setup 2 collumn
    TRISCbits.TRISC4 = 0;
    PORTCbits.RC4 = 0;
    
    if (PORTGbits.RG9 == 0 && buttonIsActive == 0) //0 means the button is pushed
    {
        shiftIn(0x2);
        buttonIsActive = 1;
    }
    else if (PORTGbits.RG8 == 0 && buttonIsActive == 0) //0 means the button is pushed
    {
        shiftIn(0x5);
        buttonIsActive = 1;
    }
    else if (PORTGbits.RG7 == 0 && buttonIsActive == 0) //0 means the button is pushed
    {
        shiftIn(0x8);
        buttonIsActive = 1;
    }
    else if (PORTCbits.RC3 == 0 && buttonIsActive == 0) //0 means the button is pushed
    {
        shiftIn(0xF);
        buttonIsActive = 1;
    }
    
    
    //reset
    TRISCbits.TRISC4 = 1;
    
    //for (j = 0; j < 40; j++); // wait
    
    //setup 1 collumn
    TRISGbits.TRISG6 = 0;
    PORTGbits.RG6 = 0;
    
    if (PORTGbits.RG9 == 0 && buttonIsActive == 0) //0 means the button is pushed
    {
        shiftIn(0x1);
        buttonIsActive = 1;
    }
    else if (PORTGbits.RG8 == 0 && buttonIsActive == 0) //0 means the button is pushed
    {
        shiftIn(0x4);
        buttonIsActive = 1;
    }
    else if (PORTGbits.RG7 == 0 && buttonIsActive == 0) //0 means the button is pushed
    {
        shiftIn(0x7);
        buttonIsActive = 1;
    }
    else if (PORTCbits.RC3 == 0 && buttonIsActive == 0) //0 means the button is pushed
    {
        shiftIn(0x0);
        buttonIsActive = 1;
    }
    
    //reset
    TRISGbits.TRISG6 = 1;
    
    //for (j = 0; j < 40; j++); // wait
    
    char activeCols[] = {0,0,0,0};
    
    //check if all buttons are released
    //setup collumn 4
    TRISCbits.TRISC2 = 0;
    PORTCbits.RC2 = 0;
    
    if (PORTGbits.RG9 == 0 || PORTGbits.RG8 == 0 || PORTGbits.RG7 == 0 || PORTCbits.RC3 == 0)
    {
        activeCols[3] = 0b1;
    }
    
    //reset
    TRISCbits.TRISC2 = 1;
    
    //setup collumn 3
    TRISCbits.TRISC1 = 0;
    PORTCbits.RC1 = 0;
    
    if (PORTGbits.RG9 == 0 || PORTGbits.RG8 == 0 || PORTGbits.RG7 == 0 || PORTCbits.RC3 == 0)
    {
        activeCols[2] = 1;
    }
    
    //reset
    TRISCbits.TRISC1 = 1;
    
    //setup collumn 2
    TRISCbits.TRISC4 = 0;
    PORTCbits.RC4 = 0;
    
    if (PORTGbits.RG9 == 0 || PORTGbits.RG8 == 0 || PORTGbits.RG7 == 0 || PORTCbits.RC3 == 0)
    {
        activeCols[1] = 1;
    }
    
    //reset
    TRISCbits.TRISC4 = 1;
    
    //setup collumn 1
    TRISGbits.TRISG6 = 0;
    PORTGbits.RG6 = 0;
    
    if (PORTGbits.RG9 == 0 || PORTGbits.RG8 == 0 || PORTGbits.RG7 == 0 || PORTCbits.RC3 == 0)
    {
        activeCols[0] = 0b1;
    }
    
    //reset
    TRISGbits.TRISG6 = 1;
    
    if (activeCols[0] == 0 && activeCols[1] == 0 && activeCols[2] == 0 && activeCols[3] == 0)
    {
        buttonIsActive = 0;
    }
} // ISR ends here.

void main(void) {
    //////////// Monitor the ISR by toggling LED0 ///////////////
    TRISAbits.TRISA0 = 0; // Configure LS pin of Port A as output.
    LATA = 1; // Turn LED 0 on.
    ////////////////////////////////////////////////////////////
   
    SYSTEMConfigPerformance(80000000); // PBCLK=80MHz. If it is removed, PBCLK=40MHz
    DDPCONbits.JTAGEN = 0; // Disable the JTAG interface to use Pin RA0 as IO  

    ANSELA = 0; // PORTA all digital
    ANSELB = 0; // PORTB all digital
    ANSELF = 0; // PORTF all digital 
    ANSELG = 0;
    //ANSELC = 0;

    // The eight pins driving the 7-segment bus are configured as output:
    TRISGbits.TRISG12 = 0;
    TRISAbits.TRISA14 = 0;
    TRISDbits.TRISD6 = 0;
    TRISGbits.TRISG13 = 0;
    TRISGbits.TRISG15 = 0;
    TRISDbits.TRISD7 = 0;
    TRISDbits.TRISD13 = 0;
    TRISGbits.TRISG14 = 0;

    // The four pins driving the four common anodes are configured as output;
    TRISAbits.TRISA10 = 0; // Common anode 3 configured as output
    TRISAbits.TRISA9 = 0; // Common anode 2 configured as output
    TRISBbits.TRISB13 = 0; // Common anode 1 configured as output
    TRISBbits.TRISB12 = 0; // Common anode 0 configured as output

    //////////////////////// set up Timer 3 /////////////////////////
    ////////// Create the largest possible interrupt period. ////////
    // ****** Fill in the blanks:
    T3CON = 0; // Turn OFF Timer3 and more.
    // Clear Timer3 Counter
    TMR3 = 0;
    // Initialize PeriodRegister 3
    PR3 = 0xFFFF;

    // Set Timer3 Priority Level to 1 (unimportant)
    IPC3bits.T3IP = 1;
    IEC0bits.T3IE = 1; // Enable Timer3 Interrupt 

    INTEnableSystemSingleVectoredInt();
    T3CONbits.TCKPS0 = 1;
    T3CONbits.TCKPS1 = 1;
    T3CONbits.TCKPS2 = 1;
    T3CONbits.ON = 1; // Turn ON Timer3; Set pre-scaler to the largest value. 
    //////////////////// End of Timer 3 setup ///////////////////////

    // All displays off
    DIG_AN3(1)
    DIG_AN2(1)
    DIG_AN1(1)
    DIG_AN0(1)

    int j;
    while (1) // Infinite loop
    {
        turnOnSeg(buffer[0]); // Send the content of first buffer location.
        DIG_AN3(0) // turn on
        for (j = 0; j < 4000; j++); // wait
        DIG_AN3(1) // turn off
        turnOnSeg(buffer[1]); // Send the content of first buffer location.
        DIG_AN2(0) // turn on
        for (j = 0; j < 4000; j++); // wait
        DIG_AN2(1) // turn off
        turnOnSeg(buffer[2]); // Send the content of first buffer location.
        DIG_AN1(0) // turn on
        for (j = 0; j < 4000; j++); // wait
        DIG_AN1(1) // turn off
        turnOnSeg(buffer[3]); // Send the content of first buffer location.
        DIG_AN0(0) // turn on
        for (j = 0; j < 4000; j++); // wait
        DIG_AN0(1) // turn off
    } // while
}// main