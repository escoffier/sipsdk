#include "CallInfoMsg.hpp"
#include "SipInterface.h"
#include <sstream>

//////////////////////////////////////////////////////////////////////////
bool CCallInfoMsg::Send( resip::DialogUsageManager &mDum, bool tcp )
{
	// session handle must be not null
	if( m_handle == NULL )
	{
		return false;
	}

	// create body
	std::string body;
	if( !Encode( body ) )
	{
		return false;
	}

	__DUM_TRY
	resip::Mime mime_type( "Application", "MANSRTSP" );
	resip::PlainContents content( resip::Data( body.c_str() ), mime_type );
	m_handle->info( content );
	__DUM_CATCH
	return true;
}

bool CCallInfoMsg::Encode( std::string &message )
{
	return false;
}

bool CCallInfoMsg::Decode( const char *body, std::string &reason )
{
	WARN_LOG( "CallInfoŒ¥ µœ÷£°" );
	return false;
}

void CCallInfoMsg::Process( CSipInterface *pInteface )
{
}

//////////////////////////////////////////////////////////////////////////
bool CVodCtrlMsg::Encode( std::string &message )
{
	std::ostringstream content;
	if( m_type == PLAY_TYPE_PLAY )
	{
		if( m_seq.empty() )
		{
			char buf[16] = { 0 };
			sprintf( buf, "%d", GetNextSN() );
			m_seq = buf;
		}

		content << "PLAY RTSP/1.0\r\n";
		content << "CSeq: " << m_seq << "\r\n";

		if( !m_speed.empty() )
		{
			content << "Scale: " << m_speed << "\r\n";
			m_pos.clear();
		}

		if( !m_pos.empty() )
		{
			content << "Range: npt=" << m_pos << "-\r\n";
		}
		
		message = content.str();
		return true;
	}
	else if( m_type == PLAY_TYPE_PAUSE )
	{
		if( m_seq.empty() )
		{
			char buf[16] = { 0 };
			sprintf( buf, "%d", GetNextSN() );
			m_seq = buf;
		}

		content << "PAUSE RTSP/1.0\r\n";
		content << "CSeq: " << m_seq << "\r\n";
		content << "PauseTime: now\r\n";
		message = content.str();
		return true;
	}
	else if( m_type == PLAY_TYPE_TEARDOWN )
	{
		return false;
	}
	else
	{
		return false;
	}
}

bool CVodCtrlMsg::Decode( const char *body, std::string &reason )
{
	if( strstr( body, "PAUSE" ) )
	{
		m_type = PLAY_TYPE_PAUSE;

		std::string str = body;
		int index1 = str.find( "PauseTime:" );
		int index2 = str.find( "\r\n", index1 );
		if( index1 > 0 && index2 > 0 )
		{
			m_pos = str.substr( index1 + strlen( "PauseTime:" ), index2 - index1 - strlen( "PauseTime:" ) );
			m_pos.erase( 0, m_pos.find_first_not_of( "\t " ) ); 
			m_pos.erase( m_pos.find_last_not_of( "\t " ) + 1 );
		}
	}
	else if( strstr( body, "TEARDOWN" ) )
	{
		m_type = PLAY_TYPE_TEARDOWN;
	}
	else
	{
		m_type = PLAY_TYPE_PLAY;
		std::string str = body;
		int index1 = str.find( "npt=" );
		int index2 = str.find( '-', index1 );
		int index3 = str.find( "Scale:" );
		int index4 = str.find( "\r\n", index3 );

		if( index3 > 0 && index4 > 0 )
		{
			m_speed = str.substr( index3 + strlen( "Scale:" ), index4- ( index3 + strlen( "Scale:" ) ) );
			m_speed.erase( 0, m_speed.find_first_not_of( "\t " ) ); 
			m_speed.erase( m_speed.find_last_not_of( "\t " ) + 1 );
		}

		if( index1 > 0 && index2 > 0 )
		{
			m_pos = str.substr( index1 + strlen( "npt=" ), index2 - index1 - strlen( "npt=" ) );
			m_pos.erase( 0, m_pos.find_first_not_of( "\t " ) ); 
			m_pos.erase( m_pos.find_last_not_of( "\t " ) + 1 );
		}
	}

    return true;
}

void CVodCtrlMsg::Process( CSipInterface *pInterface )
{
    if( pInterface != NULL )
    {
        pInterface->OnVodCtrl( *this );
    }
}

//////////////////////////////////////////////////////////////////////////
bool CCallInfoResponseMsg::Send( resip::DialogUsageManager &mDum, bool tcp )
{
	if( m_handle == NULL )
	{
		return false;
	}

	if( m_statusCode >= 200 && m_statusCode < 400 )
	{
		m_handle->acceptNITCommand( m_statusCode );
	}
	else if( m_statusCode >= 400 )
	{
		m_handle->rejectNITCommand( m_statusCode );
	}
	else
	{
		m_handle->rejectNITCommand( 500 );
	}

	return true;
}

bool CCallInfoResponseMsg::Encode( std::string &message )
{
	return true;
}

bool CCallInfoResponseMsg::Decode( const char *body, std::string &reason )
{
	return true;
}

void CCallInfoResponseMsg::Process( CSipInterface *pInterface )
{
}
