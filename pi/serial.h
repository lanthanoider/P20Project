//
//  serial.h
//  serial
//
//  uses the boost C++ library from: http://www.boost.org, especially boost.asio & boost.system

#ifndef serial_serial_h
#define serial_serial_h

#include <boost/asio.hpp>


class SimpleSerial
{
public:
    /*
     * Constructor - set up serial device
     */
    SimpleSerial(std::string port, unsigned int baud_rate)
    : io(), serial(io,port)
    {
        serial.set_option(boost::asio::serial_port_base::baud_rate(baud_rate));
    }
    
    /**
    * Write a string to the serial device.
    */
    void writeString(char c)
    {
        boost::asio::write(serial,boost::asio::buffer(c,sizeof(c)));
    }
    
    /* Read a string from serial device */
    
    std::string readLine()
    {
        //Reading data char by char, code is optimized for simplicity, not speed
        using namespace boost;
        char c;
        std::string result;
        
        for(;;)
        {
            asio::read(serial,asio::buffer(&c,1));
            result += c;
            return result;


          /*  switch(c)
            {
                case '\r':
                    return result;
                case '\n':
                    return result;
                default:
                    result+=c;
            }*/
        }
    }
    
    
private:
    boost::asio::io_service io;
    boost::asio::serial_port serial;
};

#endif
