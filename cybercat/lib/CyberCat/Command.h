//
//  Command.h
//  CyberCat
//
//  Created by Goran Stojanovic on 12/02/2020.
//  Copyright © 2020 embware. All rights reserved.
//

#ifndef Command_h
#define Command_h

#include "Types.h"

// Servo motor mapping

const uint8 FLS = 0; // Shoulder Front Left
const uint8 FRS = 1; // Shoulder Front Right
const uint8 BLS = 2; // Shoulder Back Left
const uint8 BRS = 3; // Shoulder Back Right
const uint8 FLK = 4; // Knee Front Left
const uint8 FRK = 5; // Knee Front Right
const uint8 BLK = 6; // Knee Back Left
const uint8 BRK = 7; // Knee Back Left

struct Command
{
    static const uint8 SYN = 0xFE;
    static const uint8 END = 0xFF;
    
    int  id; // Servo id
    int  value; // Servo angle
};

#endif /* Command_h */
