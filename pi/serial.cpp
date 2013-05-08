//
//  main.cpp
//  serial
//
//  uses the boost C++ library from: http://www.boost.org
//  To compile, add -lboost_system as argument to g++

#include <iostream>
#include "serial.h"
#include <string>

int main(int argc, const char * argv[])
{

    try {
        SimpleSerial serial("/dev/ttyAMA0",9600);
      
        char x; 
        
        while(1) {
       
            std::cout<< ">> ";
            std::cin >> x;  
            
            
           
            serial.writeString(x);
                
            
           
             
            std::cout<<"Received: "<<serial.readLine()<<std::endl;
            


        }
        
        
    } catch(boost::system::system_error& e)
    {
        std::cout<<"Error: "<<e.what()<<std::endl;
        return 1;
    }
}

