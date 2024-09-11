// Microcomputer Systems lab05 Summer 2024
#include "7SegForLab04.h"          // Bit banging is done here
#include <plib.h>           // Peripherial library 
#include "lcd.h"

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

typedef enum {
    INPUT,
    COUNTDOWN,
    FINISHED
} State;

State currentState = INPUT;

long timerStartTimeTicks;
long timerTicksRemaining = 1 * 5 * 1000; //6 minutes to seconds to ms

int buffer[] = {
    0, 0, 0, 0
}; // end of buffer

//get the length of the buffer with some sizeof magic

int bufferLength() {
    return sizeof (buffer) / sizeof (buffer[0]);
}

int buttonIsActive = 0;

//shifts a value into the buffer array, dropping the left most element in the array

void shiftIn(int value) {
    //detect control values that arent 0-9
    switch (value) {
        case 0xA:
        {
            //we need to convert the buffer over to a single integer value
            //interpret left 2 digits and minutes, right two as seconds
            int seconds = buffer[3];
            seconds += buffer[2] * 10;

            //if seconds is greater than 60, then reset
            if (seconds >= 60) {
                currentState = INPUT;
                buffer[3] = 0;
                buffer[2] = 0;
                buffer[1] = 0;
                buffer[0] = 0;
                return;
            }

            int minutes = buffer[1];
            minutes += buffer[0] * 10;
            currentState = COUNTDOWN;

            seconds += minutes * 60;
            timerTicksRemaining = seconds * 1000;
            timerStartTimeTicks = timerTicksRemaining;
            return;
        }
        case 0xB:
        {
            currentState = INPUT;
            return;
        }
        case 0xC:
        {
            currentState = INPUT;
            buffer[3] = 0;
            buffer[2] = 0;
            buffer[1] = 0;
            buffer[0] = 0;
            return;
        }
        case 0xD:
        case 0xE:
        case 0xF:
            return;
    }

    int i;
    //shift all values over by one
    for (i = 0; i < bufferLength() - 1; i++) //- 1 is to avoid the last element
    {
        buffer[i] = buffer[i + 1];
    }

    //put in the new value
    buffer[bufferLength() - 1] = value;
}

// Selects a col in the numpad

void selectCollumn(int col) {
    PORTCbits.RC2 = col != 4; // 4
    PORTCbits.RC1 = col != 3; // 3
    PORTCbits.RC4 = col != 2; // 2
    PORTGbits.RG6 = col != 1; // 1
}

//will shift in the appropriate row value if that button is pressed and we havent registered any other button presses

void rowLogic(int row1, int row2, int row3, int row4) {
    if (buttonIsActive == 1) {
        return;
    }

    if (PORTGbits.RG9 == 0) //0 means the button is pushed
    {
        shiftIn(row1);
        buttonIsActive = 1;
    } else if (PORTGbits.RG8 == 0) //0 means the button is pushed
    {
        shiftIn(row2);
        buttonIsActive = 1;
    } else if (PORTGbits.RG7 == 0) //0 means the button is pushed
    {
        shiftIn(row3);
        buttonIsActive = 1;
    } else if (PORTCbits.RC3 == 0) //0 means the button is pushed
    {
        shiftIn(row4);
        buttonIsActive = 1;
    }
}

//checks if all of the buttons on the number pad are released, and if so resets buttonIsActive

void checkAllButtonsReleased() {
    char activeCols[] = {0, 0, 0, 0};
    //setup collumn 4
    selectCollumn(4);

    if (PORTGbits.RG9 == 0 || PORTGbits.RG8 == 0 || PORTGbits.RG7 == 0 || PORTCbits.RC3 == 0) {
        activeCols[3] = 1;
    }

    //setup collumn 3
    selectCollumn(3);

    if (PORTGbits.RG9 == 0 || PORTGbits.RG8 == 0 || PORTGbits.RG7 == 0 || PORTCbits.RC3 == 0) {
        activeCols[2] = 1;
    }

    //setup collumn 2
    selectCollumn(2);

    if (PORTGbits.RG9 == 0 || PORTGbits.RG8 == 0 || PORTGbits.RG7 == 0 || PORTCbits.RC3 == 0) {
        activeCols[1] = 1;
    }

    //setup collumn 1
    selectCollumn(1);

    if (PORTGbits.RG9 == 0 || PORTGbits.RG8 == 0 || PORTGbits.RG7 == 0 || PORTCbits.RC3 == 0) {
        activeCols[0] = 1;
    }

    if (activeCols[0] == 0 && activeCols[1] == 0 && activeCols[2] == 0 && activeCols[3] == 0) {
        buttonIsActive = 0;
    }
}

/////////////////////////////// ISR ///////////////////////////  

void __ISR_SINGLE() T3ISR(void) {
    if (currentState == COUNTDOWN || currentState == FINISHED) {
        timerTicksRemaining -= 1;
        if (timerTicksRemaining < -1000) {
            timerTicksRemaining = 0;
        }
    }
    ///////// Monitor the ISR by toggling LED0 ///////// 
    //LATAINV = 1; // Flip LED0
    //////////////////////////////////////////////////// 

    IFS0bits.T3IF = 0; // Reset Interrupt Flag Status bit.

    //setup 4 collumn
    selectCollumn(4);
    rowLogic(0xA,
            0xB,
            0xC,
            0xD);

    //setup 3 collumn
    selectCollumn(3);
    rowLogic(0x3,
            0x6,
            0x9,
            0xE);

    //setup 2 collumn
    selectCollumn(2);
    rowLogic(0x2,
            0x5,
            0x8,
            0xF);

    //setup 1 collumn
    selectCollumn(1);
    rowLogic(0x1,
            0x4,
            0x7,
            0x0);

    //check if all buttons are released
    checkAllButtonsReleased();
} // ISR ends here.

void main(void) {
    LCD_Init();
    //////////// Monitor the ISR by toggling LED0 ///////////////
    TRISAbits.TRISA0 = 0; // Configure LS pin of Port A as output.
    TRISA = 0x00;
    //LATA = 1; // Turn LED 0 on.
    ////////////////////////////////////////////////////////////

    const int CPUSpeed = 80000000;
    const int blinkSpeed = 500;
    SYSTEMConfigPerformance(CPUSpeed); // PBCLK=80MHz. If it is removed, PBCLK=40MHz
    DDPCONbits.JTAGEN = 0; // Disable the JTAG interface to use Pin RA0 as IO  

    ANSELA = 0; // PORTA all digital
    ANSELB = 0; // PORTB all digital
    ANSELF = 0; // PORTF all digital 
    ANSELG = 0;
    //ANSELC = 0;

    //collumn outputs
    TRISCbits.TRISC2 = 0;
    TRISCbits.TRISC1 = 0;
    TRISCbits.TRISC4 = 0;
    TRISGbits.TRISG6 = 0;

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

    // Set Timer3 Priority Level to 1 (unimportant)
    IPC3bits.T3IP = 1;
    IEC0bits.T3IE = 1; // Enable Timer3 Interrupt 

    INTEnableSystemSingleVectoredInt();
    /*
     8 256
     7 128
     6 64
     5 32
     4 16
     3 8
     2 4
     1 2
     0 1
     */
    T3CONbits.TCKPS = 1;
    // Initialize PeriodRegister 3
    //PR3 = 0xFFFF;
    PR3 = 40535;
    T3CONbits.ON = 1; // Turn ON Timer3; Set pre-scaler to the largest value. 
    //////////////////// End of Timer 3 setup ///////////////////////

    AN0cfg()
    AN1cfg()
    AN2cfg()
    AN3cfg()

    // All displays off
    DIGITS_OFF

    PORTA = 0;
    
    
    //LCD_DisplayClear();
    LCD_WriteStringAtPos("testing", 3, 0);
    
    
    unsigned char testingbuff[] = {
        0b11111,
        255,
        255,
        255,
        255,
        255,
        255,
        255,
    };
    while(1)
    {
        testingbuff[0] = testingbuff[0] + 1;
        LCD_WriteBytesAtPosCgram(testingbuff, 8, posCgramChar0);
        unsigned char bAddrOffset = (0 == 0 ? 0: 0x40) + 0;
        LCD_SetWriteDdramPosition(bAddrOffset);
        LCD_WriteDataByte(posCgramChar0);
    }
    //LCD_WriteStringAtPos(mess, 0, 0);

    int j;
    while (1) // Infinite loop
    {
        switch (currentState) {
            case INPUT:
            {
                //reset LEDs. we do it like this to avoid affecting the anodes of the LEDs
                LATAbits.LATA0 = 1;
                LATAbits.LATA1 = 1;
                LATAbits.LATA2 = 1;
                LATAbits.LATA3 = 1;
                LATAbits.LATA4 = 1;
                LATAbits.LATA5 = 1;
                LATAbits.LATA6 = 1;
                LATAbits.LATA7 = 1;

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
                break;
            }
            case COUNTDOWN:
            {
                int seconds = (timerTicksRemaining + 1000) / 1000;
                int minutes = seconds / 60;
                seconds -= minutes * 60;

                if (timerTicksRemaining <= 0) {
                    currentState = FINISHED;
                }

                int digit;

                //0 to 1 float of the progress of the timer
                float progress = (float) timerTicksRemaining / (float) timerStartTimeTicks;
                //int temp = 0b11111111;
                //temp = temp << (int) (progress * 8);
                LATAbits.LATA0 = progress > 1 / 8.0;
                LATAbits.LATA1 = progress > 2 / 8.0;
                LATAbits.LATA2 = progress > 3 / 8.0;
                LATAbits.LATA3 = progress > 4 / 8.0;
                LATAbits.LATA4 = progress > 5 / 8.0;
                LATAbits.LATA5 = progress > 6 / 8.0;
                LATAbits.LATA6 = progress > 7 / 8.0;
                LATAbits.LATA7 = progress > 8 / 8.0;

                //minutes display
                digit = minutes % 10;
                minutes /= 10;
                turnOnSeg(digit);
                SEG_DP(timerTicksRemaining % blinkSpeed > (blinkSpeed / 2)); //control the dot that lines up with the middle of the 7 segment display
                DIG_AN2(0);
                for (j = 0; j < 4000; j++); // wait
                DIG_AN2(1);

                digit = minutes % 10;
                minutes /= 10;
                turnOnSeg(digit);
                DIG_AN3(0);
                for (j = 0; j < 4000; j++); // wait
                DIG_AN3(1);
                ///////////////

                //seconds display
                digit = seconds % 10;
                seconds /= 10;
                turnOnSeg(digit);
                DIG_AN0(0);
                for (j = 0; j < 4000; j++); // wait
                DIG_AN0(1);

                digit = seconds % 10;
                seconds /= 10;
                turnOnSeg(digit);
                DIG_AN1(0);
                for (j = 0; j < 4000; j++); // wait
                DIG_AN1(1);
                //////////////
                break;
            }
            case FINISHED:
            {
                turnOnSeg(0); // Send the content of first buffer location.
                DIG_AN3(timerTicksRemaining % blinkSpeed > -(blinkSpeed / 2)) // turn on
                for (j = 0; j < 4000; j++); // wait
                DIG_AN3(1) // turn off
                turnOnSeg(0); // Send the content of first buffer location.
                DIG_AN2(timerTicksRemaining % blinkSpeed > -(blinkSpeed / 2)) // turn on
                for (j = 0; j < 4000; j++); // wait
                DIG_AN2(1) // turn off
                turnOnSeg(0); // Send the content of first buffer location.
                DIG_AN1(timerTicksRemaining % blinkSpeed > -(blinkSpeed / 2)) // turn on
                for (j = 0; j < 4000; j++); // wait
                DIG_AN1(1) // turn off
                turnOnSeg(0); // Send the content of first buffer location.
                DIG_AN0(timerTicksRemaining % blinkSpeed > -(blinkSpeed / 2)) // turn on
                for (j = 0; j < 4000; j++); // wait
                DIG_AN0(1) // turn off
                break;
            }
        }
    } // while
}// main