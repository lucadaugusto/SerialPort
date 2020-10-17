/**
  * @file		serial.h
  * @version	1.0
  * @date		Janeiro/2020
  * @author		Lucas Augusto - lucas.demetrius.la@gmail.com
  */

#ifndef SERIAL_H
#define SERIAL_H

/**
  * @brief    Includes
  */
#include "defs.h"

/**
  *	Classe para fazer o gerenciamento de uma porta serial
  *	@class Serial serial.h "serial.h"
  */
class Serial
{
public:
    /**
      * @brief    Construtor da classe
      * @param    Endereço do escalonador do boost ASIO
      * @param    Endereço com a string da porta serial
      */
    Serial(boost::asio::io_service &ioService, const std::string &device);

    /**
       * @brief    Destrutor da classe
       */
    ~Serial();

    /**
      * @brief    Abre a porta serial
      */
    void Open(void);

    /**
      * @brief    Fecha a porta serial
      */
    void Close(void);

    /**
      * @brief    Leitura da porta serial
      * @return   string com os dados válidos recebidos pela porta serial
      */
    std::string Read(void);

    /**
      * @brief    Escrita da porta serial
      * @param    string com os dados a serem enviados a porta serial
      * @param    Tamanho do buffer a ser transmitido
      */
    //void Write(std::string msg, int length);
    void Write(std::string msg);

    /**
      * @brief    Configuração da porta serial
      * @param    inteiro com o baudrate
      * @param    inteiro com a paridade
      * @param    inteiro com o tamanho dos dados
      * @param    inteiro com o bit de parada
      */
    void Ioctl(unsigned int baudRate, unsigned int parity, unsigned int characterSize, unsigned int stopBit);

    /**
      * @brief    Verificação do status porta serial
      * @return   bool com o estado atual da porta true-open / false-close
      */
    bool IsActive(void);

private:
    /**
      * @brief    handle(manipulador) para finalizar a recepção de dados
      * @param    Erro do boost
      * @param    Quantidade de dados a serem transferidos
      */
    void HandlerReceiveComplete(const boost::system::error_code& error, size_t bytes_transferred);

    /**
      * @brief    Leitura da porta serial
      */
    void ReadPort(void);

    /**
      * @brief    handle(manipulador) para finalizar a escrita de dados
      * @param    Erro do boost
      */
    void HandlerWriteComplete(const boost::system::error_code &error);

    /**
      * @brief    Escrita na porta serial
      * @param    Quantidade de dados a serem escritos na porta serial
      */
    void WritePort(std::size_t length);

private:
    bool m_bActive;                              //!< Membro para indicar o estado da porta serial (ativado = true / desativado = false)
    bool m_bDataOk;                              //!< Membro para indicar se os dados são validos
    boost::asio::io_service &m_io;               //!< Endereco para o escalonador do boost ASIO
    boost::asio::serial_port m_SerialPort;       //!< Objeto da porta serial do boost ASIO
    unsigned int m_iBaudRate;                    //!< Parametro da porta serial - Baud Rate
    unsigned int m_iParity;                      //!< Parametro da porta serial - Paridade
    unsigned int m_iCharacterSize;               //!< Parametro da porta serial - Tamanho dos Dados
    unsigned int m_iStopBit;                     //!< Parametro da porta serial - Stop Bits
    std::string m_strDevice;                     //!< Parametro da porta serial - Nome da porta
    static const int m_iBufferSize = 32;         //!< Tamanho maximo de dados que podem ser lidos
    char m_vs8InputBuffer[m_iBufferSize];        //!< Buffer dados de entrada
    char m_vs8OutputBuffer[m_iBufferSize];       //!< Buffer dados de saida
    std::stringstream ss;                        //!< Variável para concatenar dados recebidos
    std::string m_sReceivedBuffer;               //!< Variável com dados concatenados prontos para decodificar
};

#endif // SERIAL_H
