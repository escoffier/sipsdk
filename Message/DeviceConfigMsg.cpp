#include "DeviceConfigMsg.hpp"
#include "SipInterface.h"

bool CBasicParamMsg::Encode( std::string &message )
{
    return true;
}

bool CBasicParamMsg::Decode( const std::vector< XMLNode* > &nodes, std::string &reason )
{
    return true;
}

void CBasicParamMsg::Process( CSipInterface *pInterface )
{
    if( pInterface != NULL )
    {
        pInterface->OnBasicParam( *this );
    }
}

bool CVideoParamConfigMsg::Encode( std::string &message )
{
    return true;
}

bool CVideoParamConfigMsg::Decode( const std::vector< XMLNode* > &nodes, std::string &reason )
{
    return true;
}

void CVideoParamConfigMsg::Process( CSipInterface *pInterface )
{
    if( pInterface != NULL )
    {
        pInterface->OnVideoParamConfig( *this );
    }
}

bool CAudioParamConfigMsg::Encode( std::string &message )
{
    return true;
}

bool CAudioParamConfigMsg::Decode( const std::vector< XMLNode* > &nodes, std::string &reason )
{
    return true;
}

void CAudioParamConfigMsg::Process( CSipInterface *pInterface )
{
    if( pInterface != NULL )
    {
        pInterface->OnAudioParamConfig( *this );
    }
}

bool CSVACEncodeConfigMsg::Encode( std::string &message )
{
    return true;
}

bool CSVACEncodeConfigMsg::Decode( const std::vector< XMLNode* > &nodes, std::string &reason )
{
    return true;
}

void CSVACEncodeConfigMsg::Process( CSipInterface *pInterface )
{
    if( pInterface != NULL )
    {
        pInterface->OnSVACEncodeConfig( *this );
    }
}

bool CSVACDecodeConfigMsg::Encode( std::string &message )
{
    return true;
}

bool CSVACDecodeConfigMsg::Decode( const std::vector< XMLNode* > &nodes, std::string &reason )
{
    return true;
}

void CSVACDecodeConfigMsg::Process( CSipInterface *pInterface )
{
    if( pInterface != NULL )
    {
        pInterface->OnSVACDecodeConfig( *this );
    }
}

bool CDeviceConfigResponseMsg::Encode( std::string &message )
{
    return true;
}

bool CDeviceConfigResponseMsg::Decode( const std::vector< XMLNode* > &nodes, std::string &reason )
{
    return true;
}

void CDeviceConfigResponseMsg::Process( CSipInterface *pInterface )
{
    if( pInterface != NULL )
    {
        pInterface->OnDeviceConfigResponse( *this );
    }
}
