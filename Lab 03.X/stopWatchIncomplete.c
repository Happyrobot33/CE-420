// stopWatch: Write the while loop

#pragma config POSCMOD=XT, FNOSC=PRIPLL 
#pragma config FPLLIDIV=DIV_2, FPLLMUL=MUL_20, FPLLODIV=DIV_1
#pragma config FPBDIV=DIV_2, FWDTEN=OFF, CP=OFF, BWP=OFF

#include <plib.h>

#define freqDivMax 1000
main()
{
    SYSTEMConfigPerformance(80000000);
    DDPCONbits.JTAGEN = 0;  // Required to use Pin RA0 as IO
    TRISA = 0;              // PORTA configured as output
    LATA = 0;               // reset timer
  
    TRISFbits.TRISF0 = 1;
    LATA = 0;
    TRISBbits.TRISB8 = 1;  
    ANSELB = 0;  // Set to digital
    ANSELF = 0;  // Set to digital
   
    unsigned char state = 0; //current state of the state machine
    unsigned int timer = 0; //time in seconds
    unsigned int freqDiv = freqDivMax; //used to convert from ms to seconds
    
    while (1)
    {
        //use the LEDs to show the current time
        PORTA = timer;
        
        // wait 1 ms
        unsigned int sleep = 7500; //brute forced sleep time value
        while(sleep > 0)
        {
            sleep--;
        }
        
        //switch on the current state of the statemachine
        switch(state)
        {
            case 0b00:
                if (PORTFbits.RF0 == 1) //check BTNC (Button Center)
                {
                    //Reset timer, init freqDiv
                    timer = 0;
                    freqDiv = freqDivMax;
                }
                
                if (PORTBbits.RB8 == 1) //check BTNR (Button Right)
                {
                    state = 0b01;
                }
                break;
            case 0b01:
                if (PORTBbits.RB8 == 0) //check BTNR (Button Right)
                {
                    state = 0b10;
                }
                break;
            case 0b10:
                if (PORTBbits.RB8 == 1) //check BTNR (Button Right)
                {
                    state = 0b11;
                }
                else
                {
                    //subtract from the freqdiv
                    freqDiv--;
                    
                    //if the freqDiv value is 0, reset the freqDiv value and increment the 1 second timer value
                    if (freqDiv == 0)
                    {
                        freqDiv = freqDivMax;
                        timer++;
                    }
                }
                break;
            case 0b11:
                if (PORTBbits.RB8 == 0) //check BTNR (Button Right)
                {
                    state = 0b00;
                }
                break;
        }// Switch
    }//While
}// main