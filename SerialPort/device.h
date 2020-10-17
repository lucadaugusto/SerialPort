/**
  * @file		device.h
  * @version	1.0
  * @date		Janeiro/2020
  * @author		Lucas Augusto - lucas.demetrius.la@gmail.com
  */

#ifndef DEVICE_H
#define DEVICE_H

/**
  * @brief    Includes
  */
#include "defs.h"
#include "serial.h"

/**
  *	Classe para fazer o gerenciamento do dispositivo
  *	@class Device device.h "device.h"
  */
class Device
{
public:
    /**
      * @brief    Construtor da classe
      * @param    Endereço do escalonador do boost ASIO
      * @param    Endereço com a string da porta serial
      */
    Device(boost::asio::io_service &ioService, const std::string &device);

    /**
      * @brief    Destrutor da classe
      */
    ~Device();

private:
    /**
      * @brief    Envia sinal de inicialização para o dispositivo
      * @note     Formato depende do dispositvo a ser conectada na porta serial
      */
    void sendInit(void);

    /**
      * @brief    Envia comando para o Dispositivo
      * @param    Tipo de comando a ser enviado
      */
    void sendCommand(Commandos commando);

    /**
      * @brief    Aguarda resposta do dispositivo
    void waitResponse(void);

    /**
      * @brief    Decodifica os dados recebidos pelo dispositivo
      */
    void decodeResponse(void);

    /**
      * @brief    Salva o status atual do dispositivo
      */
    void saveStatus(void);

    /**
      * @brief    Tratamento de timeout da espera de dados do dispositivo
      */
    void checkTimeout(void);

    /**
      * @brief    Tratamento de time do Scan do dispositivo
      */
    void checkScan(void);
private:
    unsigned int m_iCountFail;                                  //!< Contador de falhas de de leitura
    boost::asio::io_service &m_io;                              //!< Endereco para o escalonador do boost ASIO
    boost::asio::io_service::strand m_strand;                   //!< Objeto Strand para garantir o sincronismo dos timers
    boost::asio::deadline_timer m_tmrScan;                      //!< Objeto de timer do boost ASIO
    Serial m_serialport;                                        //!< Objeto para controle da porta serial
    std::string m_strStatus;                                    //!< String com os dados recebidos da porta serial
    std::map<Medidas,std::string>  m_MapValores;                //!< Map com os valores decodificados dos dados no Dispositivo
};

#endif // DEVICE_H
