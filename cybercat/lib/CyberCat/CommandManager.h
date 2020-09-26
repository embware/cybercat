//
//  CommandManager.h
//  CyberCat
//
//  Created by Goran Stojanovic on 16/06/2020.
//  Copyright © 2020 embware. All rights reserved.
//

#ifndef CommandManager_h
#define CommandManager_h

#include "Coord.h"
#include "Command.h"

struct CommandManger
{
    Command height[10];
    Command forward[19];
    Command backward[19];
    Command walk_forward[30];
    Command walk_backward[30];
    Command test[50];

    
    void updateCommands(Degree sa, Degree ka, Len moveAngle = 8);
    void update_forward(Degree shoulder_init, Degree knee_init, Degree move);
    void update_backward(Degree shoulder_init, Degree knee_init, Degree move);
    void update_walk_forward(Degree shoulder_init, Degree knee_init, Degree move);
    void update_walk_backward(Degree shoulder_init, Degree knee_init, Degree move);
    
   

   
    void update_test(Degree shoulder_init, Degree knee_init, Degree move);

   
};


inline void CommandManger::updateCommands(Degree sa, Degree ka, Len moveAngle)
   {
       height[0] = {FLS,sa};
       height[1] = {BRS,sa};
       
       height[2] = {FRS,sa};
       height[3] = {BLS,sa};

       height[4] = {FLK,ka};
       height[5] = {BRK,ka};
       height[6] = {FRK,ka};
       height[7] = {BLK,ka};
     
      
       height[8] = {Command::SYN};
       height[9] = {Command::END};
      
       update_walk_forward(sa,ka,moveAngle);
       update_walk_backward(sa,ka,moveAngle);
     
       update_test(sa,ka,moveAngle);

           
   }
   
inline void CommandManger::update_test(Degree shoulder_init, Degree knee_init, Degree move)
{
    // TODO for testing 
}


inline void CommandManger::update_forward(Degree shoulder_init, Degree knee_init, Degree move)
{
    int index = 0;
    forward[index++] = {FLS, shoulder_init + move};
    forward[index++] = {FLK, knee_init + move};
    
    forward[index++] = {FRS, shoulder_init};
    forward[index++] = {FRK, knee_init};
    
    forward[index++] = {BLS, shoulder_init};
    forward[index++] = {BLK, knee_init};
    
    forward[index++] = {BRS, shoulder_init + move};
    forward[index++] = {BRK, knee_init + move};

    forward[index++] = {Command::SYN};
    
    forward[index++] = {FLS, shoulder_init};
    forward[index++] = {FLK, knee_init};
    
    forward[index++] = {FRS, shoulder_init + move};
    forward[index++] = {FRK, knee_init + move};
    
    forward[index++] = {BLS, shoulder_init + move};
    forward[index++] = {BLK, knee_init + move};
    
    forward[index++] = {BRS, shoulder_init};
    forward[index++] = {BRK, knee_init};

    forward[index++] = {Command::SYN};
    forward[index++] = {Command::END};
}


inline void CommandManger::update_backward(Degree shoulder_init, Degree knee_init, Degree move)
{
    int index = 0;
    backward[index++] = {FLS, shoulder_init - move};
    backward[index++] = {FLK, knee_init + move};
    
    backward[index++] = {FRS, shoulder_init};
    backward[index++] = {FRK, knee_init};
    
    backward[index++] = {BLS, shoulder_init};
    backward[index++] = {BLK, knee_init};
    
    backward[index++] = {BRS, shoulder_init - move};
    backward[index++] = {BRK, knee_init + move};

    backward[index++] = {Command::SYN};
    
    backward[index++] = {FLS, shoulder_init};
    backward[index++] = {FLK, knee_init};
    
    backward[index++] = {FRS, shoulder_init - move};
    backward[index++] = {FRK, knee_init + move};
    
    backward[index++] = {BLS, shoulder_init - move};
    backward[index++] = {BLK, knee_init + move};
    
    backward[index++] = {BRS, shoulder_init};
    backward[index++] = {BRK, knee_init};

    backward[index++] = {Command::SYN};
    backward[index++] = {Command::END};
}




inline void CommandManger::update_walk_forward(Degree shoulder_init, Degree knee_init, Degree moveShoulder)
{
    int index = 0;
    Degree moveKnee =  round((float) knee_init/shoulder_init * moveShoulder) / 2;
    Degree upAngle = moveShoulder;
   
    walk_forward[index++] = {FLS, shoulder_init + moveShoulder};
    walk_forward[index++] = {FLK, knee_init + moveKnee};
    
    walk_forward[index++] = {FRS, shoulder_init};
    walk_forward[index++] = {FRK, knee_init - upAngle};
    
    walk_forward[index++] = {BLS, shoulder_init};
    walk_forward[index++] = {BLK, knee_init - upAngle};
    
    walk_forward[index++] = {BRS, shoulder_init + moveShoulder};
    walk_forward[index++] = {BRK, knee_init + moveKnee};

    walk_forward[index++] = {Command::SYN};
    
    walk_forward[index++] = {FRK, knee_init};
    
    walk_forward[index++] = {BLK, knee_init};
    
    walk_forward[index++] = {Command::SYN};
    
    walk_forward[index++] = {FLS, shoulder_init};
    walk_forward[index++] = {FLK, knee_init - upAngle};
    
    walk_forward[index++] = {FRS, shoulder_init + moveShoulder};
    walk_forward[index++] = {FRK, knee_init + moveKnee};
    
    walk_forward[index++] = {BLS, shoulder_init + moveShoulder};
    walk_forward[index++] = {BLK, knee_init + moveKnee};
    
    walk_forward[index++] = {BRS, shoulder_init};
    walk_forward[index++] = {BRK, knee_init - upAngle};
    
    walk_forward[index++] = {Command::SYN};
   
    walk_forward[index++] = {FLK, knee_init};
    
    walk_forward[index++] = {BRK, knee_init };
    
    walk_forward[index++] = {Command::SYN};
    walk_forward[index++] = {Command::END};
}

inline void CommandManger::update_walk_backward(Degree shoulder_init, Degree knee_init, Degree moveShoulder)
{
    int index = 0;
   
    Degree moveKnee =  round((float) knee_init * moveShoulder /shoulder_init ) / 2;
    Degree upAngle = moveShoulder;
   
    walk_backward[index++] = {FLS, shoulder_init - moveShoulder};
    walk_backward[index++] = {FLK, knee_init + moveKnee};
    
    walk_backward[index++] = {FRS, shoulder_init};
    walk_backward[index++] = {FRK, knee_init - upAngle};
    
    walk_backward[index++] = {BLS, shoulder_init};
    walk_backward[index++] = {BLK, knee_init - upAngle};
    
    walk_backward[index++] = {BRS, shoulder_init - moveShoulder};
    walk_backward[index++] = {BRK, knee_init + moveKnee};

    walk_backward[index++] = {Command::SYN};
    
    walk_backward[index++] = {FRK, knee_init};
    
    walk_backward[index++] = {BLK, knee_init};
    
    walk_backward[index++] = {Command::SYN};
    
    walk_backward[index++] = {FLS, shoulder_init};
    walk_backward[index++] = {FLK, knee_init - upAngle};
    
    walk_backward[index++] = {FRS, shoulder_init - moveShoulder};
    walk_backward[index++] = {FRK, knee_init + moveKnee};
    
    walk_backward[index++] = {BLS, shoulder_init - moveShoulder};
    walk_backward[index++] = {BLK, knee_init + moveKnee};
    
    walk_backward[index++] = {BRS, shoulder_init};
    walk_backward[index++] = {BRK, knee_init - upAngle};
    
    walk_backward[index++] = {Command::SYN};

    walk_backward[index++] = {FLK, knee_init};
    
    walk_backward[index++] = {BRK, knee_init };
    
    walk_backward[index++] = {Command::SYN};
    walk_backward[index++] = {Command::END};
}

   
#endif /* CommandManager_h */
