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

struct Command
{
    static const uint8 SYN = 0xFE;
    static const uint8 END = 0xFF;
    
    int  id; // Servo id
    int  value; // Servo angle
};

struct CommandQueue
{

    const static int MAX_SLOTS = 64;
    Command* pBuffer[MAX_SLOTS];
    
    uint8 head;
    uint8 tail;
    
    inline void push(Command* pCommands)
    {
        LOCK;
        tail++;
        if (tail == MAX_SLOTS)
        {
            tail = 0;
        }
        if (tail != head)
        {
            pBuffer[tail] = pCommands;
        }
        UNLOCK;
    }
    
    inline Command* pop()
    {
        LOCK;
        Command* pCommands = nullptr;
        
        if (head != tail)
        {
            head++;
            if (head == MAX_SLOTS)
            {
                head = 0;
            }
            pCommands = pBuffer[head];
            
        }
        UNLOCK;
        return pCommands;
    }
    
    inline bool empty()
    {
        LOCK;
        bool queue_empty = head == tail;
        UNLOCK;
        return queue_empty;
    }
   
    
} commandQueue;


#endif /* Command_h */
