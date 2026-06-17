/**
 * @file main.c
 * @author ric
 * @date 2026-04-05
 * @brief Main function for Assignment 1
 */

#include <xc.h>
#include <math.h>
#include <stdio.h>  // Required for sprintf

#include "timer.h" 
#include "uart.h"
#include "button.h"
#include "spi.h"
#include "BOSCH_BMX055.h"

#define RAD_TO_DEG (180.0f / 3.14159265358979323846f)

#define LD2_TRIS    TRISAbits.TRISA1
#define LD2_LAT     LATAbits.LATA1
#define BTN_T1_TRIS TRISEbits.TRISE9
#define BTN_T1_PORT PORTEbits.RE9

#define inputERR "$ERR,1*\n"

int deadlineMissed = 0;
int charReceived = 0;    

// --- hyperloop cycles counters --- //
int step_led = 0;          

int step_acc = 0;          
int step_acc_max = 20;      // Initial frequency: 5 Hz

int step_ang = 0;          

int waitme = 0;             // Accelerometer filter bandwidth settling time [10ms ticks]
int new_data_available = 0; 

// --- string formatting and interpretation --- //
char tx_buffer[64];         // for formatting sensor data 
char cmd[16];               // for interpreting commands

// --- sensor data initialization --- //
float roll = 0.0f, pitch = 0.0f;
float gx = 0.0f, gy = 0.0f, gz = 0.0f;

sensordata acc;

void algorithm(){
    tmr_wait_ms(TIMER2, 7); // Simulates 7ms execution time
}

int main(){
    ANSELA = ANSELB = ANSELC = ANSELD = ANSELE = ANSELG = 0x0000;

    LD2_TRIS = 0;   // Set LD2 as output

    // uart buffer sizes are explained in uart.h
    uart1_mikrobus2_setup();
    spi1_mikrobus1_setup();     

    // --- Peripheral specific setup ---
    BMX055_setup();
    BMX055_mag_wakeup();
    
    // Set Initial Bandwidth to 1000 Hz (Register 0x10, Value 15)
    ACC_CS = 0;
    spi1_write(0x10 & 0x7F); // MSB = 0 for write
    spi1_write(15); 
    ACC_CS = 1;

    tmr_setup_period(TIMER1, 10); // 10ms tick = exactly 100Hz loop

    while (1){
        // --- 1. Perform 7ms heavy computation --- //
        algorithm();

        // --- 2. Make LD2 blink at 1 Hz --- //
        if (step_led >= 50) {   // 50 ticks * 10ms = 500ms (500ms ON, 500ms OFF)
            LD2_LAT ^= 1;  
            step_led = 0;
        }

        // --- 3. read data (empty buffer) --- //
        int data = uart1_read_char();
        
        while(data != RX_BUFFER_EMPTY){
            charReceived++;
            
            // --- 3.1 cmd interpretation --- //
            if( (cmd[0] == '$') && (cmd[3] == ',')&& (cmd[6] == '*')){

                int val = 10*(cmd[4] - '0') + (cmd[5] - '0'); // xx or yy casted as decimal

                // --- 3.1.1 BW command ---//
                if ((cmd[1] == 'B') && (cmd[2] == 'W')){
                    if (val >= 8 && val <= 15) {

                        ACC_CS = 0;
                        spi1_write(0x10 & 0x7F); // 0x10 is PMU_BW register
                        spi1_write(val);         // Send Bandwidth bit pattern
                        ACC_CS = 1;
                        
                        // update periods based on datasheet tut (Update Time)
                        switch (val){
                            case 8:  new_data_available = 7; break; // 64ms tut -> ~7 ticks
                            case 9:  new_data_available = 4; break; // 32ms tut -> ~4 ticks
                            case 10: new_data_available = 2; break; // 16ms tut -> 2 ticks
                            case 11: new_data_available = 1; break; //  8ms tut -> 1 tick
                            default: new_data_available = 0; break; // >4ms tut -> no wait time since algorithm() takes 7ms
                        }

                        waitme = 0;

                        // --- [!] ATTENTION [!] ---
                        // task requirements are not coherent with datasheet documentation
                        // to be assignment compliant, update time must be ignored manually overwriting new_data_available
                        // comment the following line to make it datasheet compliant 
                        new_data_available = 0;

                    } else {
                        uart1_write(inputERR);
                    }
                }

                // --- 3.1.1 HZ command ---//
                else if ((cmd[1] == 'H') && (cmd[2] == 'Z')){
                    switch (val){
                        case 0:  step_acc_max = 0;   break; // 0 disables message
                        case 1:  step_acc_max = 100; break; // 100 ticks = 1Hz
                        case 2:  step_acc_max = 50;  break; // 50 ticks = 2Hz
                        case 5:  step_acc_max = 20;  break; // 20 ticks = 5Hz
                        case 10: step_acc_max = 10;  break; // 10 ticks = 10Hz
                        default: uart1_write(inputERR); break;
                    }
                }
            }

            // --- 3.3 cmd update --- // 
            cmd[0] = cmd[1];
            cmd[1] = cmd[2];
            cmd[2] = cmd[3];
            cmd[3] = cmd[4];
            cmd[4] = cmd[5];
            cmd[5] = cmd[6];
            cmd[6] = data;

            data = uart1_read_char();
        }
       
        if (waitme > new_data_available) {

            // --- 4 & 5. Acquire Data & Compute Angles @ 50 Hz (only in case of manual overwrite) --- //
            if (step_led % 2 == 0){ 
                acc = imuRead(ACC);
                
                // Sensitivity for 2g range is 1024 LSB/g.
                // Cast to float to get accurate gravitational forces and prevent NaN integer overflows
                gx = ((float)acc.x * 0.00098f);
                gy = ((float)acc.y * 0.00098f);
                gz = ((float)acc.z * 0.00098f);

                roll = atan2f(gy, gz) * RAD_TO_DEG;
                pitch = atan2f(-gx, sqrtf(gy * gy + gz * gz)) * RAD_TO_DEG;
                
                waitme = 0; 
            }            
        }

        // --- 6. Send ACC @ yy Hz --- //
        // Note: step_acc_max = 0 only when ACC output is disabled
        if ((step_acc_max > 0) && (step_acc >= step_acc_max)){ 
            sprintf(tx_buffer, "$ACC,%.2f,%.2f,%.2f*\t", gx, gy, gz);
            uart1_write(tx_buffer);
            step_acc = 0; 
        } 

        // --- 7. Send ANG @ 5 Hz --- //
        if (step_ang >= 20){ 

            // send roll and pitch to uart
            sprintf(tx_buffer, "$ANG,%.2f,%.2f*\t", roll, pitch);
            uart1_write(tx_buffer);
            
            // send number of deadline missed
            // technically, not required for the assignment, added for clarity
            sprintf(tx_buffer, "$MSD,%d*\n", deadlineMissed);
            uart1_write(tx_buffer);
            step_ang = 0; 
        }

        // Increment loop counters
        step_acc++;
        step_ang++;
        step_led++;
        waitme++;

        // Wait the remaining time to guarantee 10ms loop (100Hz)
        deadlineMissed += tmr_wait_period(TIMER1); 
    }   
    return 0;
}