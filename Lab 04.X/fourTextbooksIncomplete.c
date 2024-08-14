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
    1, 2, 3, 4, 5, 6, 7, 8
}; // end of buffer
// You may also use a 4-byte buffer.

/////////////////////////////// ISR ///////////////////////////  
// Single-vector Interrupt: There is only one interrupt source in this project: Timer3.
// The ISR checks the 8 request lines (slide switches), and then puts the numbers of all the 
// asserted lines in the buffer. The line (7) with the highest priority is checked first.

void __ISR_SINGLE() T3ISR(void) {
    ///////// Monitor the ISR by toggling LED0 ///////// 
    LATAINV = 1; // Flip LED0
    //////////////////////////////////////////////////// 

    IFS0bits.T3IF = 0; // Reset Interrupt Flag Status bit.

    int i = 0;
    for (i = 0; i < 8; i++)
        buffer[i] = 16; // blank the entire buffer.

    int studentNo = 7; // Student 7 (switch 7) has the highest priority.
    i = 0;
    if (PORTBbits.RB9 == 1) // Check the slide switch with the highest priority.
    {
        buffer[i] = studentNo;
        i++;
    }
    studentNo--;
    if (PORTBbits.RB10 == 1) // Check the slide switch with the highest priority.
    {
        buffer[i] = studentNo;
        i++;
    }
    studentNo--;
    if (PORTBbits.RB11 == 1) // Check the slide switch with the highest priority.
    {
        buffer[i] = studentNo;
        i++;
    }
    studentNo--;
    if (PORTDbits.RD14 == 1) // Check the slide switch with the highest priority.
    {
        buffer[i] = studentNo;
        i++;
    }
    studentNo--;
    if (PORTDbits.RD15 == 1) // Check the slide switch with the highest priority.
    {
        buffer[i] = studentNo;
        i++;
    }
    studentNo--;
    if (PORTFbits.RF4 == 1) // Check the slide switch with the highest priority.
    {
        buffer[i] = studentNo;
        i++;
    }
    studentNo--;
    if (PORTFbits.RF5 == 1) // Check the slide switch with the highest priority.
    {
        buffer[i] = studentNo;
        i++;
    }
    studentNo--;
    if (PORTFbits.RF3 == 1) // Check the slide switch with the highest priority.
    {
        buffer[i] = studentNo;
        i++;
    }
    //studentNo--;
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

    /*
       ///////////////////////// input pins ////////////////////////////
       No blanks to be filled!
       SW7 drives RB9
       SW6 drives RB10
       SW5 drives RB11
       SW4 drives RD14
       SW3 drives RD15
       SW2 drives RF4 
       SW1 drives RF5
       SW0 drives RF3
       /////////////////////////////////////////////////////////////////
     */

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

                // ****** Fill in the blanks to also enable the displays 2, 1, and 0 one at a time:

    } // while
}// main