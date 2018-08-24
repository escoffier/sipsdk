#include "NpGatewayMsg.hpp"
#include "SipGateway.hpp"

void COfflineMsg::Process( CSipInterface *pInterface )
{
	CSipGateway *pGateway = dynamic_cast< CSipGateway* >( pInterface );
	if( pGateway != NULL )
	{
		pGateway->OnOffline( *this );
	}
}

void CPlayOverMsg::Process( CSipInterface *pInterface )
{
	CSipGateway *pGateway = dynamic_cast< CSipGateway* >( pInterface );
	if( pGateway != NULL )
	{
		pGateway->OnPlayOver( *this );
	}
}

void CPlayCloseMsg::Process( CSipInterface *pInterface )
{
	CSipGateway *pGateway = dynamic_cast< CSipGateway* >( pInterface );
	if( pGateway != NULL )
	{
		pGateway->OnPlayClose( *this );
	}
}

void CMediaReconnectMsg::Process( CSipInterface *pInterface )
{
	CSipGateway *pGateway = dynamic_cast< CSipGateway* >( pInterface );
	if( pGateway != NULL )
	{
		pGateway->OnMediaConnect( *this );
	}

}
