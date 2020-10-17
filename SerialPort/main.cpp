/**
  * @file		main.c
  * @version	1.0
  * @date		Janeiro/2020
  * @author		Lucas Augusto - lucas.demetrius.la@gmail.com
  */

#include "defs.h"
#include "serial.h"
#include "device.h"
#include "easylogging++.h"



INITIALIZE_EASYLOGGINGPP

int main()
{
    //Configuração de clase de log por arquivo
    el::Configurations conf("config.conf");
    el::Loggers::reconfigureLogger("default",conf);
    el::Loggers::reconfigureAllLoggers(conf);

    boost::asio::io_service io;
    Device myDevice(io,COM3);

    boost::thread t1([&]{ io.run(); });
    t1.join();

    return 0;
}
