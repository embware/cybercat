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
    
    void update_height(Degree sa, Degree ka, Len moveAngle = 15);
    void update_forward(Degree shoulder_init, Degree knee_init, Degree move);
    void update_backward(Degree shoulder_init, Degree knee_init, Degree move);

#ifdef CATSIM
    Command test[50];
    void update_test(Degree shoulder_init, Degree knee_init, Degree move);
#endif // CATSIM
   
};


inline void CommandManger::update_height(Degree sa, Degree ka, Len moveAngle)
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
       
#ifdef CATSIM
           update_test(sa,ka,moveAngle);
#endif
           
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
    backward[index++] = {FLK, knee_init - move};
    
    backward[index++] = {FRS, shoulder_init};
    backward[index++] = {FRK, knee_init};
    
    backward[index++] = {BLS, shoulder_init};
    backward[index++] = {BLK, knee_init};
    
    backward[index++] = {BRS, shoulder_init - move};
    backward[index++] = {BRK, knee_init - move};

    backward[index++] = {Command::SYN};
    
    backward[index++] = {FLS, shoulder_init};
    backward[index++] = {FLK, knee_init};
    
    backward[index++] = {FRS, shoulder_init - move};
    backward[index++] = {FRK, knee_init - move};
    
    backward[index++] = {BLS, shoulder_init - move};
    backward[index++] = {BLK, knee_init - move};
    
    backward[index++] = {BRS, shoulder_init};
    backward[index++] = {BRK, knee_init};

    backward[index++] = {Command::SYN};
    backward[index++] = {Command::END};
}

#ifdef CATSIM

inline void CommandManger::update_test(Degree shoulder_init, Degree knee_init, Degree move)
{
    int index = 0;
    test[index++] = {FLS, shoulder_init + move};
    test[index++] = {FLK, knee_init + move};
    
    test[index++] = {FRS, shoulder_init};
    test[index++] = {FRK, knee_init};
    
    test[index++] = {BLS, shoulder_init};
    test[index++] = {BLK, knee_init};
    
    test[index++] = {BRS, shoulder_init + move};
    test[index++] = {BRK, knee_init + move};

    test[index++] = {Command::SYN};
    
    test[index++] = {FLS, shoulder_init};
    test[index++] = {FLK, knee_init};
    
    test[index++] = {FRS, shoulder_init + move};
    test[index++] = {FRK, knee_init + move};
    
    test[index++] = {BLS, shoulder_init + move};
    test[index++] = {BLK, knee_init + move};
    
    test[index++] = {BRS, shoulder_init};
    test[index++] = {BRK, knee_init};

    test[index++] = {Command::SYN};
    test[index++] = {Command::END};
}
#endif // CAT_SIM
   
#endif /* CommandManager_h */
