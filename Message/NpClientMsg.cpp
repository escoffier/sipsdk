#include "NpClientMsg.hpp"
#include "SipClient.hpp"

void CClientOfflineMsg::Process( CSipInterface *pInterface )
{
	CSipClient *pClient = dynamic_cast< CSipClient* >( pInterface );
	if( pClient != NULL )
	{
		pClient->OnOffline( *this );
	}
}
