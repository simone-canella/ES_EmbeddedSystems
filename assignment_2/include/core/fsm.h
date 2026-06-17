/**
 * @file fsm.h
 * @brief Finite State Machine engine declaration header for the robot buggy platform.
 * @author Simone Canella, Samuele Isaia, Riccardo Parotelli
 * @version 1.0.0
 */

#ifndef FSM_H
#define FSM_H

#include "config.h"
#include "robot_state.h"

/**
 * @brief Enumeration of available master states in the robot's FSM.
 */
typedef enum {
    STATE_HALTED,               /**< Robot is completely stopped. */
    STATE_MOVING,               /**< Robot is executing motion commands. */
    STATE_OBSTACLE_AVOIDANCE,   /**< Robot is executing an autonomous avoidance maneuver. */
    MAX_STATES                  /**< Total number of states. */
} State_t;

/**
 * @brief Enumeration of internal system events capable of triggering a state transition.
 */
typedef enum {
    EVENT_RE8_PRESSED,          /**< Physical toggle button was pressed. */
    EVENT_OBSTACLE_DETECTED,    /**< IR sensor distance threshold was breached. */
    EVENT_MANEUVER_COMPLETE,    /**< Autonomous evasion sequence finished. */
    MAX_EVENTS                  /**< Total number of events. */
} Event_t;

// --- Public Function Interfaces ---

/**
 * @brief Initializes the FSM engine, placing it into the default halted state.
 * @param rover Pointer to the shared global robot state structure.
 */
void fsm_init(RobotState_t *rover);

/**
 * @brief Submits a system event to the FSM for evaluation and potential state transition.
 * @param rover Pointer to the shared global robot state structure.
 * @param event The triggered event code.
 * @param payload An optional pointer to contextual data related to the event.
 */
void dispatch_event(RobotState_t *rover, Event_t event, void *payload);

/**
 * @brief Retrieves the current operating state of the finite state machine.
 * @return The current State_t value.
 */
State_t get_current_state(void);

#endif // FSM_H