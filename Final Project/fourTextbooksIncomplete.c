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
    SEED,
    PLAYER1,
    PLAYER2,
    GAMEFINISHED
} GameState;

GameState currentGameState = PLAYER1;

//6x6
int PlayerShips[] = {
    1, 0, 0, 0, 0, 1,
    0, 1, 0, 0, 0, 1,
    0, 0, 1, 0, 0, 0,
    0, 0, 0, 1, 0, 0,
    1, 0, 0, 0, 1, 0,
    1, 0, 0, 1, 0, 1,
}; // end of buffer
int PlayerGuesses[] = {
    0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0,
}; // end of buffer
int PlayerShipsLeft = 10;

int AIShips[] = {
    0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0,
}; // end of buffer
int AIGuesses[] = {
    0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0,
}; // end of buffer
int AIShipsLeft = 10;
//row, collumn
int PlayerSelection[] = {0, 0};

int seed = 0;

//get the length of the buffer with some sizeof magic

int bufferLength(int buffer[]) {
    return sizeof (buffer) / sizeof (buffer[0]);
}

char Status[] = "Miss!";

int buttonIsActive = 0;

//shifts a value into the buffer array, dropping the left most element in the array

void numpadInput(int value) {
    //detect control values that arent 0-9
    switch (value) {
        case 0xA:
        case 0xB:
        case 0xC:
        case 0xD:
        case 0xE:
        case 0xF:
            PlayerSelection[0] = value;
            break;
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
            PlayerSelection[1] = value;
            break;
        case 9:
            seed = PlayerSelection[0] + PlayerSelection[1];
            //HANDLE GUESS
            break;
    }

    //update the player selection

    //compute the index to use
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
        numpadInput(row1);
        buttonIsActive = 1;
    } else if (PORTGbits.RG8 == 0) //0 means the button is pushed
    {
        numpadInput(row2);
        buttonIsActive = 1;
    } else if (PORTGbits.RG7 == 0) //0 means the button is pushed
    {
        numpadInput(row3);
        buttonIsActive = 1;
    } else if (PORTCbits.RC3 == 0) //0 means the button is pushed
    {
        numpadInput(row4);
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

void setWritePosition(x, y) {
    unsigned char bAddrOffset = (y == 0 ? 0 : 0x40) + x;
    LCD_SetWriteDdramPosition(bAddrOffset);
}

void drawBoards() {
    int x;
    int y;

    unsigned char player1leftchar[] = {
        0b00000,
        0b00000,
        0b00000,
        0b00000,
        0b00000,
        0b00000,
        0b00000,
        0b00000,
    };

    unsigned char player1rightchar[] = {
        0b00000,
        0b00000,
        0b00000,
        0b00000,
        0b00000,
        0b00000,
        0b00000,
        0b00000,
    };

    int index;

    for (x = 0; x < 5; x++) {
        for (y = 0; y < 6; y++) {
            index = (y * 6) + x;
            player1leftchar[y] = player1leftchar[y] << 1;
            player1leftchar[y] = player1leftchar[y] | PlayerShips[index];
        }
    }
    for (x = 0; x < 1; x++) {
        for (y = 0; y < 6; y++) {
            index = (y * 6) + x + 5;
            player1rightchar[y] = player1rightchar[y] << 1;
            player1rightchar[y] = player1rightchar[y] | PlayerShips[index];

            //shift to align left
            player1rightchar[y] = player1rightchar[y] << 4;
        }
    }

    unsigned char player2leftchar[] = {
        0b00000,
        0b00000,
        0b00000,
        0b00000,
        0b00000,
        0b00000,
        0b00000,
        0b00000,
    };

    unsigned char player2rightchar[] = {
        0b00000,
        0b00000,
        0b00000,
        0b00000,
        0b00000,
        0b00000,
        0b00000,
        0b00000,
    };

    for (x = 0; x < 5; x++) {
        for (y = 0; y < 6; y++) {
            index = (y * 6) + x;
            player2leftchar[y] = player2leftchar[y] << 1;
            player2leftchar[y] = player2leftchar[y] | PlayerGuesses[index];
        }
    }
    for (x = 0; x < 1; x++) {
        for (y = 0; y < 6; y++) {
            index = (y * 6) + x + 5;
            player2rightchar[y] = player2rightchar[y] << 1;
            player2rightchar[y] = player2rightchar[y] | PlayerGuesses[index];

            //shift to align left
            player2rightchar[y] = player2rightchar[y] << 4;
        }
    }

    //load all of the characters into cgram
    LCD_WriteBytesAtPosCgram(player1leftchar, 8, posCgramChar0);
    LCD_WriteBytesAtPosCgram(player1rightchar, 8, posCgramChar1);
    LCD_WriteBytesAtPosCgram(player2leftchar, 8, posCgramChar2);
    LCD_WriteBytesAtPosCgram(player2rightchar, 8, posCgramChar3);

    setWritePosition(0, 0);
    LCD_WriteDataByte(0b00000000);

    setWritePosition(1, 0);
    LCD_WriteDataByte(0b00000001);

    setWritePosition(14, 0);
    LCD_WriteDataByte(0b00000010);

    setWritePosition(15, 0);
    LCD_WriteDataByte(0b00000011);

    LCD_WriteStringAtPos("You", 1, 0);
    LCD_WriteStringAtPos("AI", 1, 14);

    LCD_WriteStringAtPos(Status, 1, 6);
}

/////////////////////////////// ISR ///////////////////////////  

void __ISR_SINGLE() T3ISR(void) {
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

int random(int min, int max) {
    return min + rand() / (RAND_MAX / (max - min + 1) + 1);
}

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
    //LCD_WriteStringAtPos("testing", 3, 0);

    //LCD_WriteStringAtPos(mess, 0, 0);

    strcpy(Status, "Seed?");

    int j;
    while (1) // Infinite loop
    {
        drawBoards();

        turnOnSeg(PlayerSelection[0]); // Send the content of first buffer location.
        DIG_AN3(0) // turn on
        for (j = 0; j < 4000; j++); // wait
        DIG_AN3(1) // turn off

        turnOnSeg(PlayerSelection[1]); // Send the content of first buffer location.
        DIG_AN2(0) // turn on
        for (j = 0; j < 4000; j++); // wait
        DIG_AN2(1) // turn off

        if (seed != 0) {
            drawBoards();
            srand(seed);

            //generate the AI board
            while (AIShipsLeft > 0) {
                //generate row col
                int randindex = random(0, 35);

                //check if we can place there
                if (AIShips[randindex] == 0) {
                    AIShips[randindex] = 1;
                    AIShipsLeft--;
                }
            }
            
            //reset to break out of this condition
            seed = 0;
            strcpy(Status, "Setup");
        }
    } // while
}// main