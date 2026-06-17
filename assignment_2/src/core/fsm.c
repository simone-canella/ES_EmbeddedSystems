/**
 * @file fsm.c
 * @brief Finite State Machine engine implementation for the robot buggy platform.
 * @author Simone Canella, Samuele Isaia, Riccardo Parotelli
 * @version 1.0.0
 */

#include "fsm.h"

/**
 * @brief Function pointer type for FSM event handlers.
 */
typedef State_t (*EventHandler)(RobotState_t *rover, void *payload);

/** @brief The current state of the FSM. */
static State_t current_state;

// --- Private Transition Handler Prototypes ---
/**
 * @brief Handler to stop the robot.
 * @param rover Pointer to the robot state structure.
 * @param payload Optional event payload.
 * @return The new state (STATE_HALTED).
 */
State_t handle_halt(RobotState_t *rover, void *payload);       

/**
 * @brief Handler to move the robot.
 * @param rover Pointer to the robot state structure.
 * @param payload Optional event payload.
 * @return The new state (STATE_MOVING).
 */
State_t handle_move(RobotState_t *rover, void *payload);       

/**
 * @brief Handler for obstacle detection, initiating avoidance maneuvers.
 * @param rover Pointer to the robot state structure.
 * @param payload Optional event payload.
 * @return The new state (STATE_OBSTACLE_AVOIDANCE).
 */
State_t handle_obstacle(RobotState_t *rover, void *payload);  

/**
 * @brief Evaluates an obstacle to determine if avoidance should continue or complete.
 * @param rover Pointer to the robot state structure.
 * @param payload Pointer to the IR distance reading.
 * @return The new state based on evaluation.
 */
State_t evaluate_obstacle(RobotState_t *rover, void *payload); 

/**
 * @brief State transition table mapping current state and event to an event handler.
 */
const EventHandler state_table[MAX_STATES][MAX_EVENTS] = {
    /* EVENT_RE8_PRESSED,  EVENT_OBSTACLE_DETECTED,    EVENT_MANEUVER_COMPLETE */
    /* STATE_HALTED             */ {handle_move,        NULL,                       NULL},
    /* STATE_MOVING             */ {handle_halt,        handle_obstacle,            NULL},
    /* STATE_OBSTACLE_AVOIDANCE */ {handle_halt,        handle_halt,                evaluate_obstacle}
};

/**
 * @brief Retrieves the current state of the FSM.
 * @return The current state.
 */
State_t get_current_state(){
    return current_state;
}

/**
 * @brief Initializes the FSM and resets mission variables.
 * @param rover Pointer to the robot state structure.
 */
void fsm_init(RobotState_t *rover) {
    current_state = STATE_HALTED;
    rover->mission.obstacle_counter = 0;
}

/**
 * @brief Dispatches an event to the FSM to trigger potential state transitions.
 * @param rover Pointer to the robot state structure.
 * @param event The event being dispatched.
 * @param payload Optional payload associated with the event.
 */
void dispatch_event(RobotState_t *rover, Event_t event, void *payload) {
    EventHandler handler = state_table[current_state][event];
    
    if (handler != NULL) {
        current_state = handler(rover, payload);
    } 
}

State_t handle_halt(RobotState_t *rover, void *payload){ 
    return STATE_HALTED; 
}

State_t handle_move(RobotState_t *rover, void *payload){ 
    return STATE_MOVING; 
}

State_t handle_obstacle(RobotState_t *rover, void *payload) { 
    rover->mission.obstacle_counter = 0;
    rover->mission.current_phase = OBAV_PHASE_STOP;
    rover->mission.next_phase = OBAV_PHASE_ROTATE_CW;
    rover->mission.phase_counter = 0; 
    return STATE_OBSTACLE_AVOIDANCE;
}

State_t evaluate_obstacle(RobotState_t *rover, void *payload) {
    if (payload == NULL) {
        return STATE_OBSTACLE_AVOIDANCE; 
    }

    float ir_reading = *(float*)payload; 
    
    if(ir_reading > 20.0f) {
        return STATE_MOVING;
    } 

    rover->mission.obstacle_counter++;
    
    if (rover->mission.obstacle_counter >= 3) {
        return STATE_HALTED;
    }

    return STATE_OBSTACLE_AVOIDANCE;
}