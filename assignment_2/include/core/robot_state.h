/**
 * @file robot_state.h
 * @brief Central global data context tracking matrix for the robot buggy platform.
 * @author Simone Canella, Samuele Isaia, Riccardo Parotelli
 * @version 1.0.0
 */

#ifndef ROBOT_STATE_H
#define ROBOT_STATE_H

#include "config.h"
#include "adc.h"
#include "BOSCH_BMX055.h"
#include "button.h"

/**
 * @brief Enumeration of sub-phases executed sequentially during obstacle avoidance.
 */
typedef enum {
    OBAV_PHASE_STOP        = 0, /**< Halt current movement and zero velocities. */
    OBAV_PHASE_ROTATE_CW   = 1, /**< Perform initial 90-degree right turn. */
    OBAV_PHASE_MOVE_FW     = 2, /**< Drive straight to bypass the obstacle. */
    OBAV_PHASE_ROTATE_CCW  = 3, /**< Perform final 90-degree left turn to resume heading. */
    OBAV_PHASE_EXIT        = 4  /**< Evaluate environment and exit maneuver. */
} ObAvPhase_t;

// --- SUBSYSTEM DATA STRUCTURES ---

/**
 * @brief Groups all raw and filtered sensor reading telemetry.
 */
typedef struct {
    IMUdata acc_raw;            /**< Raw XYZ vectors from the accelerometer. */
    IMUdata mag_raw;            /**< Raw XYZ vectors from the magnetometer. */
    IMUdata gyr_raw;            /**< Raw XYZ vectors from the gyroscope. */
    float acc_x;                /**< Filtered/Scaled Acceleration X (g). */
    float acc_y;                /**< Filtered/Scaled Acceleration Y (g). */
    float acc_z;                /**< Filtered/Scaled Acceleration Z (g). */
    float battery_voltage;      /**< Computed system battery level (Volts). */
    float ir_distance;          /**< Computed forward obstacle distance (cm). */
    adcState adc_status;        /**< Internal structure holding ADC hardware states. */
} SystemSensors_t;

/**
 * @brief Groups all attitude and spatial orientation data.
 */
typedef struct {
    float roll;                 /**< Computed Roll angle (degrees). */
    float pitch;                /**< Computed Pitch angle (degrees). */
    float yaw;                  /**< Computed absolute Yaw angle from magnetometer (degrees). */
    float relative_yaw;         /**< Integrated relative Yaw angle from gyroscope (degrees). */
} SystemAttitude_t;

/**
 * @brief Groups targeted velocity commands.
 */
typedef struct {
    int32_t v;                  /**< Target linear velocity percentage (-100 to 100). */
    int32_t w;                  /**< Target angular yaw rate percentage (-100 to 100). */
} Twist_t;

/**
 * @brief Tracks the logical execution progression of autonomous missions.
 */
typedef struct {
    ObAvPhase_t current_phase;  /**< Currently executing sub-phase of the avoidance maneuver. */
    ObAvPhase_t next_phase;     /**< Scheduled subsequent phase upon completion. */
    uint32_t phase_counter;     /**< Internal clock tick counter used for timing specific phases. */
    uint32_t obstacle_counter;  /**< Tally of consecutive obstacles encountered. */
} MissionControl_t;

/**
 * @brief Manages Input/Output peripherals and communication buffers.
 */
typedef struct {
    ButtonState btn_re8;        /**< Debouncing structure for Button T1. */
    ButtonState btn_re9;        /**< Debouncing structure for Button T2. */
    int char_received;          /**< Flag indicating active UART reception. */
    char tx_buffer[64];         /**< Dedicated transmission text formatting buffer. */
    char rx_cmd[16];            /**< Dedicated reception parsing buffer. */
} SystemIO_t;

// --- THE ROOT STATE STRUCT ---
/**
 * @brief The root aggregation object holding the complete logical state of the robot.
 */
typedef struct {
    SystemSensors_t  sensors;   /**< System sensing and telemetry block. */
    SystemAttitude_t attitude;  /**< Spatial orientation block. */
    Twist_t          cmd_vel;   /**< Locomotion target tracking block. */
    MissionControl_t mission;   /**< Autonomous logic state block. */
    SystemIO_t       io;        /**< Peripheral and interaction tracking block. */
} RobotState_t;

#endif  // ROBOT_STATE_H