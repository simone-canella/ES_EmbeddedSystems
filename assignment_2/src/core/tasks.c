/**
 * @file tasks.c
 * @brief Subsystem task implementations and peripheral hardware drivers for the buggy.
 * @author Simone Canella, Samuele Isaia, Riccardo Parotelli
 * @version 1.0.0
 */

#include "config.h"
#include "robot_state.h"
#include "fsm.h"
#include "timer.h"
#include "uart.h"
#include "button.h"
#include "spi.h"
#include "BOSCH_BMX055.h"
#include "pwm.h"
#include "adc.h"
#include "scheduler.h"

// --- DEFINE --- //
#define MAX_YAW     80
#define MAX_SPEED   60

#define DISTANCE_THRESHOLD 35.0f
#define ROTATION_THRESHOLD 80.0f

#define inputERR "$ERR,1*\n"
#define PCREF_CMD_MAX_LEN 24

// --- SHARED VARIABLES --- //
/** @brief Flag indicating if a character has been received via UART. */
int charReceived = 0;    

/** @brief Buffer for incoming PCREF UART commands. */
static char cmd_buf[PCREF_CMD_MAX_LEN];
/** @brief Current length of the command stored in the buffer. */
static int cmd_len = 0;

// --- HELPER FUNCTIONS --- //

/**
 * @brief Evaluates if the car is stationary using accelerometer data.
 * @param rover Pointer to the robot state structure.
 * @return 1 if stopped, 0 otherwise.
 */
int car_stopped(RobotState_t *rover){
    if ((rover->sensors.acc_x < 0.1f)  && (rover->sensors.acc_x > -0.1f) && 
        (rover->sensors.acc_y < 0.1f)  && (rover->sensors.acc_y > -0.1f) && 
        (rover->sensors.acc_z < 1.05f) && (rover->sensors.acc_z > 0.95f)){
        return 1;
    } else return 0;
}

/**
 * @brief Initializes all necessary hardware peripherals and sets up initial robot state.
 * @param rover Pointer to the robot state structure to configure.
 */
void hw_init(RobotState_t *rover){
    ANSELA = ANSELB = ANSELC = ANSELD = ANSELE = ANSELG = 0x0000;

    uart1_mikrobus2_setup();
    spi1_mikrobus1_setup();  
    
    pwm_init();   
    adc_init(11);
    adc_init(15);

    BMX055_setup();
    BMX055_mag_wakeup();

    button_init(&rover->io.btn_re8);
    button_init(&rover->io.btn_re9);

    BTN_T1_TRIS = 1;    
    BTN_T2_TRIS = 1;    
    
    LED1_TRIS = 0;      
    LIL_TRIS = 0;
    LSL_TRIS = 0; 
    RSL_TRIS = 0;

    rover->mission.current_phase = OBAV_PHASE_STOP;
    rover->mission.next_phase = OBAV_PHASE_ROTATE_CW;

    rover->cmd_vel.v = 0;
    rover->cmd_vel.w = 0;

    tmr_setup_period(TIMER1, 2);
}

// --- TASKS --- //

/**
 * @brief Task routine to read IR sensors and battery voltage via ADC.
 * @param arg Pointer to the robot state structure.
 */
void read_IR(void *arg){         
    RobotState_t *rover = (RobotState_t *)arg;

    adc_converter(&rover->sensors.adc_status);
    rover->sensors.battery_voltage = rover->sensors.adc_status.battery_voltage;
    rover->sensors.ir_distance     = rover->sensors.adc_status.ir_distance - 7; 

    if ((rover->sensors.ir_distance < DISTANCE_THRESHOLD) && (get_current_state() == STATE_MOVING)){
        dispatch_event(rover, EVENT_OBSTACLE_DETECTED, NULL);
    }
}

/**
 * @brief Task routine to handle movement and obstacle avoidance state logic.
 * @param arg Pointer to the robot state structure.
 */
void move(void *arg){          
    RobotState_t *rover = (RobotState_t *)arg;
    State_t current_state = get_current_state();

    if (current_state ==  STATE_OBSTACLE_AVOIDANCE){
        switch (rover->mission.current_phase){
            case OBAV_PHASE_STOP:
                execute_motion(0, 0);
                if (car_stopped(rover)){
                    switch (rover->mission.next_phase){
                        case OBAV_PHASE_ROTATE_CW:
                            rover->attitude.relative_yaw = 0.0f;    
                            rover->mission.current_phase = OBAV_PHASE_ROTATE_CW;
                            rover->mission.next_phase = OBAV_PHASE_STOP;
                            rover->mission.phase_counter = 0;
                            break;

                        case OBAV_PHASE_MOVE_FW:
                            rover->mission.current_phase = OBAV_PHASE_MOVE_FW;
                            rover->mission.phase_counter = 0;
                            break; 

                        case OBAV_PHASE_ROTATE_CCW:
                            rover->attitude.relative_yaw = 0.0f;    
                            rover->mission.current_phase = OBAV_PHASE_ROTATE_CCW;
                            break;

                        default:
                            break;
                    }
                }
                break;

            case OBAV_PHASE_ROTATE_CW:
                if (rover->attitude.relative_yaw > -ROTATION_THRESHOLD) {
                    execute_motion(0, -MAX_YAW);
                } else {
                    rover->mission.current_phase = OBAV_PHASE_STOP;
                    rover->mission.next_phase = OBAV_PHASE_MOVE_FW;
                    rover->mission.phase_counter = 0;
                }
                break;

            case OBAV_PHASE_MOVE_FW:
                if (rover->mission.phase_counter < 1000) {
                    if (rover->sensors.ir_distance <= DISTANCE_THRESHOLD){
                        dispatch_event(rover, EVENT_OBSTACLE_DETECTED, NULL);
                        rover->mission.current_phase = OBAV_PHASE_STOP;
                        rover->mission.next_phase = OBAV_PHASE_ROTATE_CW;
                        rover->mission.phase_counter = 0; 
                        return;
                    }

                    execute_motion(MAX_SPEED, 0);
                    rover->mission.phase_counter++;
                }
                else {
                    rover->mission.current_phase = OBAV_PHASE_STOP;
                    rover->mission.next_phase = OBAV_PHASE_ROTATE_CCW;
                }
                break;

            case OBAV_PHASE_ROTATE_CCW:
                if (rover->attitude.relative_yaw < ROTATION_THRESHOLD) {
                    execute_motion(0, MAX_YAW);
                } else {
                    execute_motion(0, 0);
                    rover->mission.current_phase = OBAV_PHASE_EXIT;
                }
                break;

            case OBAV_PHASE_EXIT:
                dispatch_event(rover, EVENT_MANEUVER_COMPLETE, &rover->sensors.ir_distance);

                rover->mission.current_phase = OBAV_PHASE_STOP;
                rover->mission.next_phase = OBAV_PHASE_ROTATE_CW;
                rover->mission.phase_counter = 0;
                break;
            
            default:
                break;
        }
    }
    else if (current_state == STATE_MOVING){
        execute_motion(rover->cmd_vel.v, rover->cmd_vel.w);
    }
    else{
        execute_motion(0, 0);
        rover->mission.current_phase = OBAV_PHASE_STOP;
        rover->mission.next_phase = OBAV_PHASE_ROTATE_CW;
        rover->mission.phase_counter = 0;
    }
}

/**
 * @brief Task routine to poll button states and dispatch related events or UART info.
 * @param arg Pointer to the robot state structure.
 */
void button_polling(void *arg){  
    RobotState_t *rover = (RobotState_t *)arg;

    if (button_has_just_been_pressed(&rover->io.btn_re8, BTN_T1_PORT)){
        dispatch_event(rover, EVENT_RE8_PRESSED, NULL);
    }

    if (button_has_just_been_pressed(&rover->io.btn_re9, BTN_T2_PORT)){
        int tx_count = uart1_get_tx_count();
        int rx_count = uart1_get_rx_count();
        
        sprintf(rover->io.tx_buffer, "\n$MBUF,%d,%d*\n", tx_count, rx_count);
        uart1_write(rover->io.tx_buffer);
    }
}

/**
 * @brief Task routine to read and process raw accelerometer and magnetometer data.
 * @param arg Pointer to the robot state structure.
 */
void read_acc_mag(void *arg) {  
    RobotState_t *rover = (RobotState_t *)arg;

    rover->sensors.acc_raw = imuRead(ACC);
    float ax = ((float) rover->sensors.acc_raw.x * 0.00098f);
    float ay = ((float) rover->sensors.acc_raw.y * 0.00098f);
    float az = ((float) rover->sensors.acc_raw.z * 0.00098f);

    float roll = atan2f(ay, az) ;
    float pitch = atan2f(-ax, sqrtf(ay * ay + az * az));

    rover->sensors.mag_raw = imuRead(MAG);
    float mx = (float) rover->sensors.mag_raw.x - MAG_OFFSET_X;
    float my = (float) rover->sensors.mag_raw.y - MAG_OFFSET_Y; 
    float mz = (float) rover->sensors.mag_raw.z - MAG_OFFSET_Z; 

    float Xh = mx * cosf(pitch) + my * sinf(roll) * sinf(pitch) + mz * cosf(roll) * sinf(pitch);
    float Yh = my * cosf(roll) - mz * sinf(roll);
    float yaw = atan2f(-Yh, Xh);
    
    rover->sensors.acc_x = ax;
    rover->sensors.acc_y = ay;
    rover->sensors.acc_z = az;
    rover->attitude.roll = roll   * RAD_TO_DEG;
    rover->attitude.pitch = pitch * RAD_TO_DEG;
    rover->attitude.yaw = yaw     * RAD_TO_DEG;
}

/**
 * @brief Task routine to read gyroscope data and integrate relative yaw.
 * @param arg Pointer to the robot state structure.
 */
void read_gyro(void *arg) {
    RobotState_t *rover = (RobotState_t *)arg;

    rover->sensors.gyr_raw = imuRead(GYR);
    float gyr_z_dps = (float)rover->sensors.gyr_raw.z / 16.4f;
    rover->attitude.relative_yaw += (gyr_z_dps * 0.01f);
}

/**
 * @brief Task routine to log general system and sensor states over UART via Bluetooth.
 * @param arg Pointer to the robot state structure.
 */
void UART_log_bt(void *arg){        
    RobotState_t *rover = (RobotState_t *)arg;

    sprintf(rover->io.tx_buffer, "ST:%d | PH:%d->%d | ir:%.3f bat:%.3f | yaw:%.1f | v:%ld | w:%ld\n", 
        get_current_state(), 
        rover->mission.current_phase, 
        rover->mission.next_phase, 
        (double) rover->sensors.ir_distance, 
        (double) rover->sensors.battery_voltage,
        (double) rover->attitude.relative_yaw,
        (long) rover->cmd_vel.v,
        (long) rover->cmd_vel.w
    );
    uart1_write(rover->io.tx_buffer);  
}

/**
 * @brief Task routine to transmit distance and orientation angles via UART.
 * @param arg Pointer to the robot state structure.
 */
void UART_log(void *arg){        
    RobotState_t *rover = (RobotState_t *)arg;

    sprintf(rover->io.tx_buffer, "$MDIST,%d\n", (int)rover->sensors.ir_distance);
    uart1_write(rover->io.tx_buffer);

    sprintf(rover->io.tx_buffer, "$MANGLE,%.2f|%.2f|%.2f\n", 
            (double)rover->attitude.roll, 
            (double)rover->attitude.pitch, 
            (double)rover->attitude.yaw);
    uart1_write(rover->io.tx_buffer);
}

/**
 * @brief Task routine to receive and parse twist commands ($PCREF) over UART.
 * @param arg Pointer to the robot state structure.
 */
void UART_get_twist(void *arg){
    RobotState_t *rover = (RobotState_t *)arg;
    int data = uart1_read_char();

    while(data != RX_BUFFER_EMPTY){
        char c = (char)data;

        if(c == '$'){
            cmd_len = 0;
            cmd_buf[cmd_len++] = c;
        } else if(cmd_len > 0){
            if(cmd_len < PCREF_CMD_MAX_LEN - 1){
                cmd_buf[cmd_len++] = c;

                if(c == '*'){
                    cmd_buf[cmd_len] = '\0';

                    if(cmd_len > 8 && strncmp(cmd_buf, "$PCREF,", 7) == 0){
                        char *payload = &cmd_buf[7];
                        char *comma_ptr = strchr(payload, ',');

                        if(comma_ptr != NULL){
                            *comma_ptr = '\0';
                            int speed = atoi(payload);
                            int yawrate = atoi(comma_ptr + 1);

                            if(speed >= -100 && speed <= 100 && yawrate >= -100 && yawrate <= 100){
                                rover->cmd_vel.v = speed;
                                rover->cmd_vel.w = yawrate;
                            } else {
                                uart1_write(inputERR);
                            }
                        }
                    }

                    cmd_len = 0;
                }
            } else {
                cmd_len = 0;
            }
        }

        data = uart1_read_char();
    }
}

/**
 * @brief Task routine to toggle the A0 status LED.
 * @param arg Unused argument.
 */
void blink_A0(void *arg){        
    LED1_LAT ^= 1;
}

/**
 * @brief Task routine to log the current battery voltage over UART.
 * @param arg Pointer to the robot state structure.
 */
void UART_bat_log(void *arg){    
    RobotState_t *rover = (RobotState_t *)arg;

    sprintf(rover->io.tx_buffer, "$MBAT:%.2f\n", (double)rover->sensors.battery_voltage);
    uart1_write(rover->io.tx_buffer);
}

/**
 * @brief Task routine to manage blinker lights depending on the robot's state.
 * @param arg Unused argument.
 */
void blink_lights(void *arg){    
    State_t current_state = get_current_state();

    switch(current_state){
        case STATE_MOVING:
            LSL_LAT = 0;
            RSL_LAT = 0;
            LIL_LAT = 1;
            break;

        case STATE_OBSTACLE_AVOIDANCE:
            LSL_LAT = 0;
            RSL_LAT ^= 1;
            LIL_LAT = 1;
            break;

        default: 
            LSL_LAT ^= 1;
            RSL_LAT = LSL_LAT;
            LIL_LAT = 0;
            break;
    }
}

/**
 * @brief Initializes and registers all scheduler tasks.
 * @param param Pointer to the robot state structure to be passed into tasks.
 */
void tasks_init(void *param) {
    register_task( 0,   1, read_IR,         param); // task0
    register_task( 0,   1, move,            param); // task1
    register_task(-1,   5, button_polling,  param); // task2
    register_task(-2,   5, read_acc_mag,    param); // task3
    register_task(-3,  50, UART_log_bt,     param); // task4
    register_task(-4,  50, UART_get_twist,  param); // task5
    register_task(-5, 500, blink_A0,        param); // task6
    register_task(-6, 500, UART_bat_log,    param); // task7
    register_task(-7, 500, blink_lights,    param); // task8
    register_task(-8,   5, read_gyro,       param); 
}