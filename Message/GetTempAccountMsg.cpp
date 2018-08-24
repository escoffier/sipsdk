#include "GetTempAccountMsg.hpp"
#include "SipInterface.h"

bool CGetTempAccountMsg::Encode( std::string &message )
{
    std::ostringstream content;
    content<<"<?xml version=\"1.0\"?>\r\n";
    content<<"<Query>\r\n";
    content<<"<CmdType>TempAccount</CmdType>\r\n";
    content<<"<SN>"<< m_sn <<"</SN>\r\n";
    content<<"<DeviceID>"<< m_deviceid <<"</DeviceID>\r\n";
    content<<"</Query>\r\n";
    message = content.str();
    return true;
}

bool CGetTempAccountMsg::Decode( const std::vector< XMLNode* > &nodes, std::string &reason )
{
    size_t size = nodes.size();
    for( size_t i = 0; i < size; ++i )
    {
    }

    return true;
}

void CGetTempAccountMsg::Process( CSipInterface *pInterface )
{
    if( NULL != pInterface )
    {
        pInterface->OnGetTempAccount( *this );
    }
}

bool CGetTempAccountResponseMsg::Encode( std::string &message )
{
    std::ostringstream content;
    content<<"<?xml version=\"1.0\"?>\r\n";
    content<<"<Response>\r\n";
    content<<"<CmdType>TempAccount</CmdType>\r\n";
    content<<"<SN>"<< m_sn <<"</SN>\r\n";
    content<<"<DeviceID>"<< m_deviceid <<"</DeviceID>\r\n";

    if( IsOK() )
    {
        content<<"<Result>OK</Result>\r\n";
    }
    else
    {
        content<<"<Result>ERROR</Result>\r\n";
    }

    if( !m_account.empty() )
    {
        content << "<Account>" << m_account << "</Account>\r\n";
    }

    if( !m_password.empty() )
    {
        content << "<Password>" << m_password << "</Password>\r\n";
    }
    content<<"</Response>\r\n";
    
    message = content.str();
    return true;
}

bool CGetTempAccountResponseMsg::Decode( const std::vector< XMLNode* > &nodes, std::string &reason )
{
    // 必须参数校验
    XMLNode *pResult = NULL;
    XMLNode *pAccount = NULL;
    XMLNode *pPassword = NULL;
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

        if( CGBMessage::CompareNoCase( type, "Result" ) )
        {
            pResult = pNode;
        }
        else if( CGBMessage::CompareNoCase( type, "Account" ) )
        {
            pAccount = pNode;
        }
        else if( CGBMessage::CompareNoCase( type, "Password" ) )
        {
            pPassword = pNode;
        }
    }

    // 必选参数必须填
    if( pResult == NULL || pAccount == NULL || pPassword == NULL )
    {
        ERROR_LOG( "参数Result,Account,Password没有被设置" );
        reason = "参数Result,Account,Password没有被设置!";
        return false;
    }

    // 参数解析
    std::list< XMLNode* > items;
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
            continue;
        }

        const char *value = pChild->Value();
        if( value == NULL )
        {
            continue;
        }

        if( CGBMessage::CompareNoCase( type, "Result" ) )
        {
            if( CGBMessage::CompareNoCase( value, "OK" ) )
            {
                m_result = CGBMessage::RESULT_OK;
            }
            else if( CGBMessage::CompareNoCase( value, "Error" ) )
            {
                m_result = CGBMessage::RESULT_ERROR;
            }
            else
            {
                ERROR_LOG( "参数值\'" << value << "\'无效" );
                reason = "参数Result值无效!";
                return false;
            }
        }
        else if( CGBMessage::CompareNoCase( type, "Account" ) )
        {
            m_account = value;
        }
        else if( CGBMessage::CompareNoCase( type, "Password" ) )
        {
            m_password = value;
        }
    }

    return true;
}

void CGetTempAccountResponseMsg::Process( CSipInterface *pInterface )
{
    if( pInterface != NULL )
    {
        pInterface->OnGetTempAccountResponse( *this );
    }
}
