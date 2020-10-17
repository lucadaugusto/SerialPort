/**
  * @file		defs.h
  * @version	1.0
  * @date		Janeiro/2020
  * @author		Lucas Augusto - lucas.demetrius.la@gmail.com
  */
#ifndef DEFS_H
#define DEFS_H


/**
  * @brief  Arquivo com todos os includes globais e definiçoes
  * @note   A finalidade é tornar o código mais organizado e facilitar debugger
  *         e manutenção do código
  */

/**
  * @brief		Includes
  */
#include <iostream>
#include <string>
#include <map>
#include <boost/tokenizer.hpp>
#include <boost/asio.hpp>
#include <boost/asio/serial_port.hpp>
#include <boost/system/error_code.hpp>
#include <boost/system/system_error.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include "easylogging++.h"

/**
  * @brief		Defines
  */
#define   NONE       boost::asio::serial_port_base::parity::type::none
#define   EVEN       boost::asio::serial_port_base::parity::type::even
#define   ODD        boost::asio::serial_port_base::parity::type::odd
#define   ONE_BIT    boost::asio::serial_port_base::stop_bits::one
#define   TWO_BIT    boost::asio::serial_port_base::stop_bits::two
#define   COM1       "/dev/ttyS0"
#define   COM3       "/dev/ttyUSB0"
#define   COM4       "/dev/ttyUSB1"
#define   TIME_SCAN  1
#define   TIME_OUT   5


/**
  * @brief		Enums
  */
enum Commandos
{
    LIGAR = 0,
    DESLIGAR,
    AUTOTESTE,
    EVENTOS,
    FALTAS,
    DATA,
    HORA,
};

enum Medidas
{
    Valor_1 = 0,
    Valor_2,
    Valor_3,
};

#endif // DEFS_H
