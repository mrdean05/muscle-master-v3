#include "Events.h"

namespace APP{
    Events::Event Events::getEventFromChar (char ch){
        switch (ch){
            case 'M': return Event::Menu;
            case '=': return Event::Enter;     // = : = or ENTER
            case '^': return Event::UpArrow;   
            case 'D': return Event::DownArrow;     // D: down
            case '>': return Event::RightArrow;
            case '<': return Event::LeftArrow; 
            //case 'P': return ;     // P: 00 or space
            case '0':     //
            case '1':      //
            case '2':       //
            case '3':      //
            case '4': 
            case '5': 
            case '6':  
            case '7': 
            case '8': 
            case '9': 
                return Event::AlphaNumeric;
            case 'C': return Event::Clear;     // C: CLEAR 
            case 'A': return Event::Allclear;     // A: ALL CLEAR - AC
            case 'I': return Event::Cashin;     // I: CASH IN
            case 'O': return Event::Cashout;     // O: CASH OUT
            //case '.': return ;     //
            //case '/': return ;     //
            //case '+': return ;     //
            //case '-': return ;     // 
            //case 'X': return ;     // X
            //case 'W': return ;     // W: M+
            //case 'S': return ;     // S: SHIFT
            //case 'Q': return ;     // Q: M-  
            //case 'E': return ;     // E: MC
            default:
                return Event::Null;
        }
    }
}