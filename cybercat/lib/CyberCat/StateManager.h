
#ifndef StateManager_h
#define StateManager_h

// StateType 
typedef uint8 StateType;

#define STATE_OFFLINE         0
#define STATE_CALIBRATION     1
#define STATE_CONNECTED       2
#define STATE_STANDBY         3
#define STATE_FORWARD         4
#define STATE_BACKWARD        5 
#define STATE_UP              6
#define STATE_DOWN            7 

struct StateManager
{
    StateType state = STATE_OFFLINE;

    bool check(StateType check_state)
    {
        return state == check_state;
    }

    void set(StateType update_state)
    {
        LOG("Set %s ",mapState(update_state));
        state = update_state;
    }

    
    char* mapState(StateType state)
    {
        switch (state)
        {
        case STATE_OFFLINE:
            return (char*) "STATE_OFFLINE";
        case STATE_CONNECTED:
            return (char*) "STATE_CONNECTED";
        case STATE_STANDBY:
            return (char*) "STATE_STANDBY";
        case STATE_CALIBRATION:
            return (char*) "STATE_CALIBRATION";
        
        case STATE_FORWARD:
            return (char*) "STATE_FORWARD";
        case STATE_BACKWARD:
            return (char*) "STATE_BACKWARD";
        
        case STATE_UP:
            return (char*) "STATE_UP";
        case STATE_DOWN:
            return (char*) "STATE_DOWN";
        
        default:
            return (char*) "ERROR: STATE UNKOWN ?";
        }
    }
};


#endif //StateManager_h