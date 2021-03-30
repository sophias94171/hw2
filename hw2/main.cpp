#include "mbed.h"
#include "uLCD_4DGL.h"

// Initialize a pins to perform analog and digital output functions

// Adjust analog output pin name to your board spec.

AnalogOut  aout(PA_4);
AnalogIn ain(A0);
DigitalIn B0(D8);
DigitalIn B1(D9);
DigitalIn B2(D10);
uLCD_4DGL uLCD(D1, D0, D2);


float f[8] = {20, 56, 81, 106, 131, 156, 250, 500};
int f_cur = 3;
int f_idx = 3;
void display(){
    uLCD.locate(0, 0);
    for (int i=0;i<8;i++){
        uLCD.color(GREEN);
        if (i==f_cur)
            uLCD.color(BLUE);
        if (i==f_idx)
            uLCD.color(RED);
        uLCD.printf("%.1f   \n", f[i]);
    }
}

int main(void)

{
    B0.mode(PullNone);
    B1.mode(PullNone);
    B2.mode(PullNone);
    
    int index=0;
    float scale = 3.0/3.3;
    aout = 0;
    
    float T = 1000/f[f_idx];
    uLCD.locate(0, 0);
    uLCD.text_width(2); //4X size text
    uLCD.text_height(2);
    float adc_data=0;
    display();
    while (1) {
        // check button
        if(B0.read()){
            f_cur  = f_cur + 1;
            if(f_cur>7)
                f_cur = 7;
            display();
            ThisThread::sleep_for(10ms);
        }
        if(B1.read()){
            f_cur = f_cur - 1;
            if(f_cur<0)
                f_cur = 0;
            display();
            ThisThread::sleep_for(10ms);
        }
        if(B2.read()){
            f_idx = f_cur;
            // freq = f[f_idx];
            aout = 0;
            // printf("Current Frequence : %.1f \n\r", freq);
            display();
            ThisThread::sleep_for(10ms);
        }
        // T = 1/ freq
        T = 1000/f[f_idx];
        if(index < 0.2*T){
            aout = float(index)/0.2/T*scale;
        }
        else{
            aout = scale*(1- float(index-0.2*T)/0.8/T);
        }
        if(index < T)
            index = index +1;
        else
            index = 0;
        // set aout 
        adc_data = ain;
        printf("%f\n", adc_data);
        ThisThread::sleep_for(1ms);
   }

}
