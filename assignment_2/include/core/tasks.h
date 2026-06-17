/**
 * @file tasks.h
 * @brief Subsystem task abstractions and hardware setup declarations for the robot buggy.
 * @author Simone Canella, Samuele Isaia, Riccardo Parotelli
 * @version 1.0.0
 *
 * @details This header file declares the initialization interfaces responsible for 
 * configuring low-level microcontroller peripherals and registering periodic execution 
 * tasks with the cooperative system scheduler.
 */

#ifndef TASKS_H
#define TASKS_H

#include "config.h"
#include "robot_state.h"

// --- Public Initialization Interfaces ---

/**
 * @brief Registers all system runtime operational tasks into the scheduler matrix.
 * @param param Generic initialization parameter container representing the robot state.
 * @return void
 *
 * @details Connects individual software tasks (such as sensor reading, state machine dispatching, 
 * motor actuation, and telemetry logging) with the periodic cyclic scheduler. In order to 
 * guarantee maximum execution time budgets for every single clock tick, tasks are registered 
 * with pre-calculated initial offsets so that their worst-case workloads do not overlap.
 */
void tasks_init(void *param);

/**
 * @brief Configures low-level microcontroller hardware registers and peripherals.
 * @param rover Pointer to the robot state structure to configure.
 * @return void
 *
 * @details Sets up all required physical hardware components on the dsPIC core. This handles 
 * the initialization of data direction registers (TRIS), latch configurations (LAT), Peripheral 
 * Pin Select (PPS) pin remapping for UART/SPI communication links, analog-to-digital converters (ADC) 
 * for infrared and battery sensing, and Output Compare modules for the 10 kHz motor PWM waves.
 */
void hw_init(RobotState_t *rover);

#endif // TASKS_H