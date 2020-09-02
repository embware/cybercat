
#ifndef StateManager_h
#define StateManager_h

// StateType 
typedef unsigned int StateType;

#define STATE_OFFLINE         0
#define STATE_CALIBRATION     1
#define STATE_CONNECTED       2

struct StateManager
{
    StateType state = STATE_OFFLINE;

    bool check(StateType check_state)
    {
        return state & check_state;
    }

    void set(StateType update_state)
    {
        LOG("Set %s ",mapState(update_state));
        state |= update_state;
    }

    void clear(StateType update_state)
    {
        LOG("Clear %s ",mapState(update_state));
        state &= ~update_state;
    }

    char* mapState(StateType state)
    {
        switch (state)
        {
        case STATE_OFFLINE:
            return (char*) "STATE_OFFLINE";
        case STATE_CALIBRATION:
            return (char*) "STATE_CALIBRATION";
        case STATE_CONNECTED:
            return (char*) "STATE_CONNECTED";
        
        default:
            return (char*) "STATE Unknown?!";
        }
    }
};


#endif //StateManager_h