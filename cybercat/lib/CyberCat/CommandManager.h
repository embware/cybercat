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
    Degree upAngle = 15;
    
    Command height[10];
    Command forward[64];
    Command backward[64];
    Command walk_forward[64];
    Command walk_backward[64];
    Command test[50];

    void updateCommands(Degree sa, Degree ka, Len moveAngle);
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
      
       update_forward(sa,ka,moveAngle);
       update_backward(sa,ka,moveAngle);
       
       update_walk_forward(sa,ka,moveAngle);
       update_walk_backward(sa,ka,moveAngle);
     
       update_test(sa,ka,moveAngle);

           
   }
   
inline void CommandManger::update_test(Degree shoulder_init, Degree knee_init, Degree move)
{
    // TODO for testing 
}

inline void CommandManger::update_forward(Degree shoulder_init, Degree knee_init, Degree moveShoulder)
{
    int index = 0;
    
    const Degree moveKnee =  round((double) knee_init * moveShoulder * 0.5 /shoulder_init  );
    
    
    forward[index++] = {FLS, shoulder_init - moveShoulder};
    forward[index++] = {FLK, knee_init + moveKnee - upAngle};
    
    forward[index++] = {FRS, shoulder_init + moveShoulder};
    forward[index++] = {FRK, knee_init + moveKnee};
    
    forward[index++] = {BLS, shoulder_init + moveShoulder};
    forward[index++] = {BLK, knee_init + moveKnee};
    
    forward[index++] = {BRS, shoulder_init - moveShoulder};
    forward[index++] = {BRK, knee_init + moveKnee - upAngle};

    forward[index++] = {Command::SYN};
    
    forward[index++] = {FLK, knee_init + moveKnee};
    forward[index++] = {BRK, knee_init + moveKnee};

    forward[index++] = {Command::SYN};
    
    forward[index++] = {FLS, shoulder_init + moveShoulder};
    forward[index++] = {FLK, knee_init + moveKnee};
    
    forward[index++] = {FRS, shoulder_init - moveShoulder};
    forward[index++] = {FRK, knee_init + moveKnee - upAngle};
    
    forward[index++] = {BLS, shoulder_init - moveShoulder};
    forward[index++] = {BLK, knee_init + moveKnee - upAngle};
    
    forward[index++] = {BRS, shoulder_init + moveShoulder};
    forward[index++] = {BRK, knee_init + moveKnee};

    forward[index++] = {Command::SYN};
    
    forward[index++] = {FRK, knee_init + moveKnee};
    forward[index++] = {BLK, knee_init + moveKnee};
    
    forward[index++] = {Command::END};
}

inline void CommandManger::update_backward(Degree shoulder_init, Degree knee_init, Degree moveShoulder)
{
    int i = 0;
    
    const Degree moveKnee =  round((double) knee_init * moveShoulder * 0.5 /shoulder_init);
    
    
    backward[i++] = {FLS, shoulder_init - moveShoulder};
    backward[i++] = {FLK, knee_init + moveKnee };
    
    backward[i++] = {FRS, shoulder_init + moveShoulder};
    backward[i++] = {FRK, knee_init + moveKnee - upAngle};
    
    backward[i++] = {BLS, shoulder_init + moveShoulder};
    backward[i++] = {BLK, knee_init + moveKnee - upAngle};
    
    backward[i++] = {BRS, shoulder_init - moveShoulder};
    backward[i++] = {BRK, knee_init + moveKnee };

    backward[i++] = {Command::SYN};
    
    backward[i++] = {FRK, knee_init + moveKnee};
    backward[i++] = {BLK, knee_init + moveKnee};

    backward[i++] = {Command::SYN};
    
    backward[i++] = {FLS, shoulder_init + moveShoulder};
    backward[i++] = {FLK, knee_init + moveKnee - upAngle};
    
    backward[i++] = {FRS, shoulder_init - moveShoulder};
    backward[i++] = {FRK, knee_init + moveKnee};
    
    backward[i++] = {BLS, shoulder_init - moveShoulder};
    backward[i++] = {BLK, knee_init + moveKnee};
    
    backward[i++] = {BRS, shoulder_init + moveShoulder};
    backward[i++] = {BRK, knee_init + moveKnee- upAngle};

    backward[i++] = {Command::SYN};
    
    backward[i++] = {FLK, knee_init + moveKnee};
    backward[i++] = {BRK, knee_init + moveKnee};
    
    backward[i++] = {Command::END};
}

inline void CommandManger::update_walk_forward(Degree shoulder_init, Degree knee_init, Degree moveShoulder)
{
    int index = 0;
    
    Degree moveKnee =  round((double) knee_init * moveShoulder * 0.5 /shoulder_init  );
       
      
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

    Degree moveKnee =  round((double) knee_init * moveShoulder * 0.5 /shoulder_init  );
    
   
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
