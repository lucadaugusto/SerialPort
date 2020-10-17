/**
  * @file		main.c
  * @version	1.0
  * @date		Janeiro/2020
  * @author		Lucas Augusto - lucas.demetrius.la@gmail.com
  */

/**
  * @brief    Includes
  */
#include "serial.h"

// Métodos públicos
//------------------------------------------------------------------------------------------------------//
Serial::Serial(boost::asio::io_service &ioService, const std::string &device)
    : m_bActive(true),
      m_io(ioService),
      m_SerialPort(m_io)
{
    LOG(INFO) << "Criado o objeto Serial";

    //Configuro a porta serial com os parametros default
    m_iBaudRate = 9600;
    m_iCharacterSize = 8;
    m_strDevice = device;
    m_bDataOk = false;
}
//------------------------------------------------------------------------------------------------------//



//------------------------------------------------------------------------------------------------------//
Serial::~Serial()
{

}
//------------------------------------------------------------------------------------------------------//



//------------------------------------------------------------------------------------------------------//
void Serial::Open(void)
{
    //Verifico se a porta serial esta aberta
    if (m_SerialPort.is_open())
    {
        std::cout << "Erro - Porta serial esta aberta" << std::endl;
        return;
    }

    try
    {
        //Abre a porta serial
        m_SerialPort.open(m_strDevice);
        m_bActive = true;

        //Configuro os parametros da serial
        m_SerialPort.set_option(boost::asio::serial_port_base::baud_rate(m_iBaudRate));                // configura o baudrate com a porta serial aberta
        m_SerialPort.set_option(boost::asio::serial_port_base::parity(NONE));                          // configura a paridade com a porta serial aberta
        m_SerialPort.set_option(boost::asio::serial_port_base::character_size(m_iCharacterSize));      // configura o tamanho dos dados com a porta serial aberta
        m_SerialPort.set_option(boost::asio::serial_port_base::stop_bits(ONE_BIT));                    // configura o stop bit com a porta serial aberta

        // Chama função para leitura assincrona da porta serial
        ReadPort();
    }
    catch (std::exception& error)
    {
        LOG(ERROR) << "Erro ao abrir a porta serial - Erro: " << error.what();
    }
}
//------------------------------------------------------------------------------------------------------//



//------------------------------------------------------------------------------------------------------//
void Serial::Close(void)
{
    try
    {
        // Fecha a porta serial
        m_SerialPort.close();
        m_bActive = false;
    }
    catch (std::exception& error)
    {
        LOG(ERROR) << "Erro ao fechar a porta serial - Erro: " << error.what();
    }
}
//------------------------------------------------------------------------------------------------------//



//------------------------------------------------------------------------------------------------------//
std::string Serial::Read(void)
{
    try
    {
       if(m_bDataOk == true)
       {
           m_bDataOk = false;
           return m_sReceivedBuffer;
       }
       else
       {
           ss.str("");
           m_sReceivedBuffer.clear();
           return "fail_data";
       }
   }
    catch (std::exception& error)
    {
        LOG(ERROR) << "Erro ao realizar a leitura da porta serial - Erro: " << error.what();
    }
    //return "";
}
//------------------------------------------------------------------------------------------------------//



//------------------------------------------------------------------------------------------------------//
//void Serial::Write(std::string msg, int length)
void Serial::Write(std::string msg)
{
    try
    {
        //Copia da string para o buffer de saida
        msg.copy(m_vs8OutputBuffer,msg.length(),0);

        //Escrevo na porta serial
        WritePort(msg.length());
    }
    catch (std::exception& error)
    {
        LOG(ERROR) << "Erro ao escrever na porta serial - Erro: " << error.what();
    }
}
//------------------------------------------------------------------------------------------------------//



//------------------------------------------------------------------------------------------------------//
void Serial::Ioctl(unsigned int baudRate, unsigned int parity, unsigned int characterSize, unsigned int stopBit)
{
    try
    {
        m_iBaudRate = baudRate;
        m_iParity = parity;
        m_iCharacterSize = characterSize;
        m_iStopBit = stopBit;
    }
    catch (std::exception& error)
    {
        LOG(ERROR) << "Erro ao configurar a porta serial - Erro: " << error.what();
    }
}
//------------------------------------------------------------------------------------------------------//



//------------------------------------------------------------------------------------------------------//
bool Serial::IsActive(void)
{
    return m_bActive;
}
//------------------------------------------------------------------------------------------------------//



// Métodos privados
//------------------------------------------------------------------------------------------------------//
void Serial::HandlerReceiveComplete(const boost::system::error_code& error, size_t bytes_transferred)
{
    //Tratamento de erros
    if (error)
    {
        LOG(ERROR) << "Erro ao receber dados da porta serial - Erro: " << error.message();
        ReadPort();
        return;
    }

    //coloca o conteúdo do buffer na string
    for(size_t i = 0; i < bytes_transferred; ++i)
    {
        ss << m_vs8InputBuffer[i];

        //Aqui encontro o terminador
        if((m_vs8InputBuffer[0] == 'L') || (m_vs8InputBuffer[0] == 'D'))
        {
            m_bDataOk = true;
            m_sReceivedBuffer = ss.str();
            ss.str("");
        }
    }

    // Chama função para leitura assincrona da porta serial
    ReadPort();
}
//------------------------------------------------------------------------------------------------------//



//------------------------------------------------------------------------------------------------------//
void Serial::ReadPort()
{
    //Leitura assincrona da porta serial
    m_SerialPort.async_read_some(boost::asio::buffer(m_vs8InputBuffer, m_iBufferSize),
        boost::bind(&Serial::HandlerReceiveComplete,
        this,
        boost::asio::placeholders::error,
        boost::asio::placeholders::bytes_transferred));
}
//------------------------------------------------------------------------------------------------------//



//------------------------------------------------------------------------------------------------------//
void Serial::HandlerWriteComplete(const boost::system::error_code &error)
{
    if (error)
    {
        LOG(ERROR) << "Erro ao enviar dados da porta serial - Erro: " << error.message();
        return;
    }
}
//------------------------------------------------------------------------------------------------------//



//------------------------------------------------------------------------------------------------------//
void Serial::WritePort(std::size_t length)
{
    //Escrita assincrona da porta serial
    boost::asio::async_write(m_SerialPort,
        boost::asio::buffer(m_vs8OutputBuffer, length),
        boost::bind(&Serial::HandlerWriteComplete,
        this,
        boost::asio::placeholders::error));
}
//------------------------------------------------------------------------------------------------------//
