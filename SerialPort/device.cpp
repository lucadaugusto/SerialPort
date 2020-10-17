/**
  * @file		device.c
  * @version	1.0
  * @date		Janeiro/2020
  * @author		Lucas Augusto - lucas.demetrius.la@gmail.com
  */

/**
  * @brief    Includes
  */
#include "device.h"


// Métodos públicos
//------------------------------------------------------------------------------------------------------//
Device::Device(boost::asio::io_service &ioService, const std::string &device)
    : m_io(ioService),
    m_strand(m_io),
    m_tmrScan(m_io,boost::posix_time::seconds(TIME_SCAN)),
    //m_tmrScan(m_io),
    //m_tmrTimeout(m_io),
    m_serialport(m_io,device)
{
    LOG(INFO) << "Criado o objeto Device";

    //Abre a porta serial
    m_serialport.Open();

    //Escalonamento da rotina de leitura do dispositivo
    m_tmrScan.async_wait(m_strand.wrap([&](const boost::system::error_code& e){ Device::checkScan(); }));

    //Inicialização do contador de falhas de leitura
    m_iCountFail = 0;

    //Inicialização do timer para colocar em modo de espera
    //m_tmrTimeout.expires_at(boost::posix_time::pos_infin);

    //Inicialização da função de callback para quando o timeout expirar
    //checkTimeout();
}
//------------------------------------------------------------------------------------------------------//



//------------------------------------------------------------------------------------------------------//
Device::~Device()
{
    //Fecha a porta serial
    m_serialport.Close();
}
//------------------------------------------------------------------------------------------------------//


// Métodos privados
//------------------------------------------------------------------------------------------------------//
void Device::sendSync()
{
    try
    {
        std::string sync;
        const char arraySync[] = {" Colocar aqui sequência de inicialização do dispositivo"};
        sync = arraySync;
        LOG(INFO) << "Enviado sequencia de Inicialização";
        m_serialport.Write(sync);
    }
    catch (std::exception& error)
    {
        LOG(ERROR) << "Erro ao enviar arquivo de sincronismo - Erro: " << error.what();
    }
}
//------------------------------------------------------------------------------------------------------//



//------------------------------------------------------------------------------------------------------//
void Device::sendCommand(Commandos commando)
{
    try
    {
        //TODO: retirar o Line feed do final do envio do comando desejado
        //Verifico qual o comando devo enviar para o Device abaixo alguns exemplos:
        std::string strComandos[] = {"LIGAR\n","DESLIGAR\n","AUTOTESTE\n","EVENTOS\n","FALTAS\n","DATA\n","HORA\n"};
        m_serialport.Write(strComandos[commando]);

        //Ligo o timer para a operação assincrona de timeout
        m_tmrTimeout.expires_from_now(boost::posix_time::seconds(TIME_OUT));
    }
    catch (std::exception& error)
    {
        LOG(ERROR) << "Erro ao enviar comando para o Device - Erro: " << error.what();
    }
}
//------------------------------------------------------------------------------------------------------//



//------------------------------------------------------------------------------------------------------//
void Device::waitResponse()
{
    m_strStatus = m_serialport.Read();

    if (m_strStatus == "fail_data")
    {
        m_iCountFail++;
        LOG(INFO) << m_strStatus;
    }
    else
    {
        m_iCountFail = 0;
        LOG(INFO) << m_strStatus;

        //Desligo o timer para a operação assincrona de timeout
        m_tmrTimeout.expires_from_now(boost::posix_time::seconds(TIME_OUT));
        decodeResponse();
    }
}
//------------------------------------------------------------------------------------------------------//



//------------------------------------------------------------------------------------------------------//
void Device::decodeResponse()
{
    //Declaração de variáveis
    std::string value;
    int count;

    //Defino no template o tipo para a classe tokenizer
    typedef boost::tokenizer<boost::char_separator<char> > tokenizer;

    //Crio objeto para definir os separados dos tokens
    boost::char_separator<char> sep(" \r\n");

    //Crio objeto para gerar os tokens
    tokenizer tokens(m_strStatus, sep);

    //Vetor com as posições dos simbolos
    int pos[50];
    for(int i = 0; i <= 50; ++i)
    {
        pos[i] = 0;
    }

    //Executo varredura para localizar posiçao dos simbolos no array de resposta
    count = 0;
    for (tokenizer::iterator tok_iter = tokens.begin();tok_iter != tokens.end(); ++tok_iter)
    {
        // Atribução da string atual do token
        value = *tok_iter;

        //Verifico as posições dos tokens de acordo com os simbolos
        if(value.compare("Valor_1:") == 0)
        {
            pos[Valor_1] = count;
        }
        else if(value.compare("Valor_2:") == 0)
        {
            pos[Valor_2] = count;
        }
        else if(value.compare("Valor_3:") == 0)
        {
            pos[Valor_3] = count + 2;
        }
        count++;
    }

    //Inserçao dos valores no map
    count = 0;
    for (tokenizer::iterator tok_iter = tokens.begin();tok_iter != tokens.end(); ++tok_iter)
    {
        // Atribução da string atual do token
        value = *tok_iter;

        if(count == (pos[Valor_1]+1))
        {
            m_MapValores.insert(std::make_pair(Valor_1,value));
            m_MapValores[ Valor_1] = value;
        }
        else if(count == (pos[Valor_2]+1))
        {
            m_MapValores.insert(std::make_pair(Valor_2,value));
            m_MapValores[Valor_2] = value;
        }
        else if(count == (pos[Valor_3]+1))
        {
            m_MapValores.insert(std::make_pair(Valor_3,value));
            m_MapValores[ Valor_3 ] = value;
        }

        count++;
    }
}
//------------------------------------------------------------------------------------------------------//



//------------------------------------------------------------------------------------------------------//
void Device::saveDeviceStatus()
{

}
//------------------------------------------------------------------------------------------------------//


//------------------------------------------------------------------------------------------------------//
void Device::checkTimeout()
{
    //Verifica se ocorreu o estouro do timeout
    if(m_tmrTimeout.expires_at() <= boost::asio::deadline_timer::traits_type::now())
    {
        //Desligo o timer para a próxima operação assincrona de timeout
        m_tmrTimeout.expires_at(boost::posix_time::pos_infin);
        std::cout << "Estouro do timeout" << std::endl;
    }

    m_tmrTimeout.async_wait(m_strand.wrap([&](const boost::system::error_code& e){ Device::checkTimeout(); }));
}
//------------------------------------------------------------------------------------------------------//



//------------------------------------------------------------------------------------------------------//
void Device::checkScan()
{
    m_tmrScan.expires_at(m_tmrScan.expires_at() + boost::posix_time::seconds(TIME_SCAN));
    m_tmrScan.async_wait(m_strand.wrap([&](const boost::system::error_code& e){ checkScan(); }));

    LOG(INFO) << "Realizado o Scan: " << m_iCountFail;
    sendSync();
    sendCommand(STATUS);
    waitResponse();
    saveDeviceStatus();
}
//------------------------------------------------------------------------------------------------------//
