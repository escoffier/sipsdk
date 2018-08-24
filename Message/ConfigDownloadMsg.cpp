#include "ConfigDownloadMsg.hpp"
#include "SipInterface.h"

bool CConfigDownloadMsg::Encode( std::string &message )
{
    std::ostringstream content;
    content<<"<?xml version=\"1.0\"?>\r\n";
    content<<"<Query>\r\n";
    content<<"<CmdType>ConfigDownload</CmdType>\r\n";
    content<<"<SN>"<< m_sn <<"</SN>\r\n";
    content<<"<DeviceID>"<< m_deviceid <<"</DeviceID>\r\n";
    content<<"<ConfigType>";
    std::vector<std::string>::iterator iter;
//     for (iter=m_vConfigType.begin(); iter!=m_vConfigType.end(); iter++)  
//     {
//         if (iter != m_vConfigType.begin())
//         {
//             content<<"/";
//         }
//         content<<*iter;
//     }  
    content<<"</ConfigType>\r\n";
    content<<"</Query>\r\n";

    message = content.str();
    return true;
}

bool CConfigDownloadMsg::Decode( const std::vector< XMLNode* > &nodes, std::string &reason )
{
    size_t size = nodes.size();
    for( size_t i = 0; i < size; ++i )
    {
        XMLNode *pNode = nodes[i];
        if( pNode == NULL )
        {
            DEBUG_LOG( "参数错误" );
            continue;
        }

        const char *type = pNode->Value();
        if( type == NULL )
        {
            DEBUG_LOG( "参数名字为空" );
            continue;
        }

        XMLNode *pChild = pNode->FirstChild();
        if( pChild == NULL )
        {
            DEBUG_LOG( "参数值没有设置" );
            continue;
        }

        const char *value = pChild->Value();
        if( value == NULL )
        {
            DEBUG_LOG( "参数值为空" );
            continue;
        }		

//         if( CGBMessage::CompareNoCase( type, "ConfigType" ) )
//         {
//             //m_startTime = value;
//             SplitWord(value, m_vConfigType, "/");
//             if (0 == m_vConfigType.size())
//             {
//                 DEBUG_LOG( "必选字段ConfigType值为空" );
//                 continue;
//             }
//         }
//         else
//         {
//             DEBUG_LOG( "国标未定义的参数:" << type );
//             continue;
//         }
    }

    return true;
}

void CConfigDownloadMsg::Process( CSipInterface *pInterface )
{
    if( pInterface != NULL )
    {
        pInterface->OnConfigDownload( *this );
    }
}

bool CBasicParamResponseMsg::Encode( std::string &message )
{
    std::ostringstream content;
    content<<"<?xml version=\"1.0\"?>\r\n";
    content<<"<Response>\r\n";
    content<<"<CmdType>ConfigDownload</CmdType>\r\n";
    content<<"<SN>"<<GetSN()<<"</SN>\r\n";
    content<<"<DeviceID>"<<GetDeviceID()<<"</DeviceID>\r\n";

//     if( IsOK() )
//     {
//         content<<"<Result>OK</Result>\r\n";
//     }
//     else
//     {
//         content<<"<Result>ERROR</Result>\r\n";
//     }
// 
//     content<<"<BasicParam>\r\n";
//     if ()
//     {
//     }
    content<<"</BasicParam>\r\n";

    content<<"</Response>\r\n\r\n";
    message = content.str();
    return true;
}

bool CBasicParamResponseMsg::Decode( const std::vector< XMLNode* > &nodes, std::string &reason )
{
    return true;
}

void CBasicParamResponseMsg::Process( CSipInterface *pInterface )
{
    if( pInterface != NULL )
    {
        pInterface->OnBasicParamResponse( *this );
    }
}

bool CVideoParamConfigResponseMsg::Encode( std::string &message )
{
    return true;
}

bool CVideoParamConfigResponseMsg::Decode( const std::vector< XMLNode* > &nodes, std::string &reason )
{
    return true;
}

void CVideoParamConfigResponseMsg::Process( CSipInterface *pInterface )
{
    if( pInterface != NULL )
    {
        pInterface->OnVideoParamConfigResponse( *this );
    }
}

bool CAudioParamConfigResponseMsg::Encode( std::string &message )
{
    return true;
}

bool CAudioParamConfigResponseMsg::Decode( const std::vector< XMLNode* > &nodes, std::string &reason )
{
    return true;
}

void CAudioParamConfigResponseMsg::Process( CSipInterface *pInterface )
{
    if( pInterface != NULL )
    {
        pInterface->OnAudioParamConfigResponse( *this );
    }
}

bool CSVACEncodeConfigResponseMsg::Encode( std::string &message )
{
    return true;
}

bool CSVACEncodeConfigResponseMsg::Decode( const std::vector< XMLNode* > &nodes, std::string &reason )
{
    return true;
}

void CSVACEncodeConfigResponseMsg::Process( CSipInterface *pInterface )
{
    if( pInterface != NULL )
    {
        pInterface->OnSVACEncodeConfigResponse( *this );
    }
}

bool CSVACDecodeConfigResponseMsg::Encode( std::string &message )
{
    return true;
}

bool CSVACDecodeConfigResponseMsg::Decode( const std::vector< XMLNode* > &nodes, std::string &reason )
{
    return true;
}

void CSVACDecodeConfigResponseMsg::Process( CSipInterface *pInterface )
{
    if( pInterface != NULL )
    {
        pInterface->OnSVACDecodeConfigResponse( *this );
    }
}

bool CVideoParamOptResponseMsg::Encode( std::string &message )
{
    return true;
}

bool CVideoParamOptResponseMsg::Decode( const std::vector< XMLNode* > &nodes, std::string &reason )
{
    return true;
}

void CVideoParamOptResponseMsg::Process( CSipInterface *pInterface )
{

}

bool CAudioParamOptResponseMsg::Encode( std::string &message )
{
    return true;
}

bool CAudioParamOptResponseMsg::Decode( const std::vector< XMLNode* > &nodes, std::string &reason )
{
    return true;
}

void CAudioParamOptResponseMsg::Process( CSipInterface *pInterface )
{

}
