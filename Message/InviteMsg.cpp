#include "InviteMsg.hpp"
#include "SipInterface.h"
#include <sstream>

#ifndef NPLogError
#define NPLogError
#endif

//////////////////////////////////////////////////////////////////////////
bool CInviteMsg::Send( resip::DialogUsageManager &mDum, bool tcp )
{
	// create sdp
	std::string body;
	if( !Encode( body ) )
	{
		ERROR_LOG( "消息编码错误，发送失败!" );
		return false;
	}

	__DUM_TRY
	resip::NameAddr uasAor;
	uasAor.uri().user() = m_to.c_str();
	uasAor.uri().host() = m_to.substr( 0, 10 ).c_str();

	resip::HeaderFieldValue hfv( body.c_str(), body.length() );
	resip::Mime mime_type( "APPLICATION", "SDP" );
	resip::PlainContents sdp( hfv, mime_type );
	resip::SharedPtr<resip::SipMessage> reqInvite = mDum.makeInviteSession( uasAor, &sdp );

	// 传输模式设置顺序是ForceTarget,route,RequestLine
	// 消息没有设置ForceTarget和route头域，所以可以根据请求行设置传输模式
	// request
	resip::Uri from;
	from.user() = m_from.c_str();
	from.host() = m_from.substr( 0, 10 ).c_str();
	reqInvite->header( resip::h_From ).uri() = from;

	// 请求行
	resip::Uri rl;
	rl.user() = m_request.c_str();
	rl.host() = m_ip.c_str();
	rl.port() = m_port;
	rl.param( resip::p_transport ) = "UDP";
	reqInvite->header( resip::h_RequestLine ).uri() = rl;

	// subject
	if( !m_subject.empty() )
	{
		reqInvite->header( resip::h_Subject ) = resip::StringCategory( resip::Data( m_subject.c_str() ) );
	}

	// store callid
	if( reqInvite->exists( resip::h_CallID ) )
	{
		m_callid = reqInvite->header( resip::h_CallID ).value().c_str();
	}

	mDum.sendCommand( reqInvite );
	__DUM_CATCH
	return true;
}

bool CInviteMsg::Encode( std::string &message )
{
	DEBUG_LOG( "消息编码错误，发送失败!" );
	return false;
}

bool CInviteMsg::Decode( const char *body, std::string &reason )
{
	DEBUG_LOG( "InviteMessage解码未实现!" );
	return false;
}

void CInviteMsg::Process( CSipInterface *pInterface )
{
}

//////////////////////////////////////////////////////////////////////////
bool CVodDownloadMsg::Encode( std::string &message )
{
	// subject
	if( m_subject.empty() )
	{
		m_subject.clear();
        m_subject += m_sender.id;
        m_subject += ":";
        m_subject += m_sender.seq;
        m_subject += ",";
        m_subject += m_recver.id;
        m_subject += ":";
        m_subject += m_recver.seq;
	}

	// sdp
	std::stringstream context;
	context<<"v=0\r\n";
	context<<"o="<< m_recvid <<" 0 0 IN IP4 "<< m_mediaip <<"\r\n";
	context<<"s=Download\r\n";
	context<<"u=" << m_uri.id << ":" << m_uri.param << "\r\n";
	context<<"c=IN IP4 "<< m_connect <<"\r\n";
	context<<"t="<< m_starttime << " " << m_endtime << "\r\n";

	context<<"m=video "<< m_mediaport <<" RTP/AVP";
	std::map< std::string, std::string >::const_iterator it;
	for (it = m_rtpmap.begin(); it != m_rtpmap.end(); it++)
	{
		std::string key = it->first;
		std::string text = it->second;
		if ( !text.empty() )
		{
			context<<" "<<key;
		}
	}
	context<<"\r\n";

	context<<"a=recvonly\r\n";
	for (it = m_rtpmap.begin(); it != m_rtpmap.end(); it++)
	{
		std::string key = it->first;
		std::string text = it->second;
		if ( !text.empty() )
		{
			context<<"a=rtpmap:"<<key<<" "<<text<<"\r\n";
		}
	}

    if( !m_downloadspeed.empty() )
    {
        context<<"a=downloadspeed:"<<m_downloadspeed<<"\r\n";
    }

	if( !m_ssrc.empty() )
	{
		context<<"y="<< m_ssrc << "\r\n";
	}

	if( !m_audioproterty.empty() )
	{
		context<<"f="<< m_audioproterty << "\r\n";
	}

	message = context.str();
	return true;
}

bool CVodDownloadMsg::Decode( const char *body, std::string &reason )
{
	// message without sdp
	if( body == NULL )
	{
#ifdef _DEBUG
		WARN_LOG( "录像下载请求消息体不带SDP!" );
#endif
		return true;
	}

	const char* pContent = body;
	const char* anchor = 0;

	// 解析SDP字段
	while( pContent != '\0' )
	{
		char ch = *pContent;
		if( ch == 'v' )
		{
			if( *pContent++ != 'v' || *pContent++ != '=' )
			{
				return false;
			}

			while( *pContent != '\0' && *pContent != '\r' && *pContent != '\n' ) pContent++;
			if( *pContent == '\0' )
			{
				return false;
			}
		}
		else if( ch == 'o' )
		{
			if( *pContent++ != 'o' || *pContent++ != '=' )
			{
				return false;
			}

			// id
			anchor = pContent;
			while( *pContent != ' ' ) pContent++;
			if( *pContent == '\0' )
			{
				return false;
			}
			else
			{
				m_recvid.assign( anchor, pContent );
				while( *pContent == ' ' ) pContent++;
				if( *pContent == '\0' )
				{
					return false;
				}
			}

			// session
			anchor = pContent;
			while( *pContent != ' ' ) pContent++;
			if( *pContent == '\0' )
			{
				return false;
			}
			else
			{
				m_sessionid.assign( anchor, pContent );
				while( *pContent == ' ' ) pContent++;
				if( *pContent == '\0' )
				{
					return false;
				}
			}

			// version
			anchor = pContent;
			while( *pContent != ' ' ) pContent++;
			if( *pContent == '\0' )
			{
				return false;
			}
			else
			{
				m_version.assign( anchor, pContent );
				while( *pContent == ' ' ) pContent++;
				if( *pContent == '\0' )
				{
					return false;
				}
			}

			// in
			anchor = pContent;
			while( *pContent != ' ' ) pContent++;
			if( *pContent == '\0' )
			{
				return false;
			}
			else
			{
				while( *pContent == ' ' ) pContent++;
				if( *pContent == '\0' )
				{
					return false;
				}
			}

			// ip version
			anchor = pContent;
			while( *pContent != ' ' ) pContent++;
			if( *pContent == '\0' )
			{
				return false;
			}
			else
			{
				while( *pContent == ' ' ) pContent++;
				if( *pContent == '\0' )
				{
					return false;
				}
			}

			// media ip
			anchor = pContent;
			while( *pContent != '\r' && *pContent != '\n' ) pContent++;
			if( *pContent == '\0' )
			{
				return false;
			}
			else
			{
				m_mediaip.assign( anchor, pContent );
			}
		}
		else if( ch == 's' )
		{
			if( *pContent++ != 's' || *pContent++ != '=' )
			{
				return false;
			}

			while( *pContent != '\r' && *pContent != '\n' ) pContent++;
			if( *pContent == '\0' )
			{
				return false;
			}
		}
		else if( ch == 'u' )
		{
			if( *pContent++ != 'u' || *pContent++ != '=' )
			{
				return false;
			}

			// uri id
			anchor = pContent;
			while( *pContent != ':' ) pContent++;
			if( *pContent == '\0' )
			{
				return false;
			}
			else
			{
				m_uri.id.assign( anchor, pContent );
				pContent++;
				if( *pContent == '\0' )
				{
					return false;
				}
			}

			// uri param
			anchor = pContent;
			while( *pContent != '\r' && *pContent != '\n' ) pContent++;
			if( *pContent == '\0' )
			{
				return false;
			}
			else
			{
				m_uri.param.assign( anchor, pContent );
			}
		}
		else if( ch == 'c' )
		{
			if( *pContent++ != 'c' || *pContent++ != '=' )
			{
				return false;
			}

			// in
			while( *pContent != ' ' ) pContent++;
			if( pContent == '\0' )
			{
				return false;
			}
			else
			{
				while( *pContent == ' ' ) pContent++;
				if( *pContent == '\0' )
				{
					return false;
				}
			}

			// ip version
			while( *pContent != ' ' ) pContent++;
			if( pContent == '\0' )
			{
				return false;
			}
			else
			{
				while( *pContent == ' ' ) pContent++;
				if( *pContent == '\0' )
				{
					return false;
				}
			}

			// connect ip
			anchor = pContent;
			while( *pContent != '\r' && *pContent != '\n' ) pContent++;
			if( *pContent == '\0' )
			{
				return false;
			}
			else
			{
				m_connect.assign( anchor, pContent );
			}
		}
		else if( ch == 't' )
		{
			if( *pContent++ != 't' || *pContent++ != '=' )
			{
				return false;
			}

			// start time
			anchor = pContent;
			while( *pContent != ' ' ) pContent++;
			if( *pContent == '\0' )
			{
				return false;
			}
			else
			{
				m_starttime.assign( anchor, pContent );
				while( *pContent == ' ' ) pContent++;
				if( *pContent == '\0' )
				{
					return false;
				}
			}

			// end time
			anchor = pContent;
			while( *pContent != '\r' && *pContent != '\n' ) pContent++;
			if( *pContent == '\0' )
			{
				return false;
			}
			else
			{
				m_endtime.assign( anchor, pContent );
			}
		}
		else if( ch == 'm' )
		{
			if( *pContent++ != 'm' || *pContent++ != '=' )
			{
				return false;
			}

			// media type
			anchor = pContent;
			while( *pContent != ' ' ) pContent++;
			if( *pContent == '\0' )
			{
				return false;
			}
			else
			{
				m_mediatype.assign(anchor, pContent);
				while( *pContent == ' ' ) pContent++;
				if( *pContent == '\0' )
				{
					return false;
				}
			}

			// port
			anchor = pContent;
			while( *pContent != ' ') pContent++;
			if( *pContent == '\0' )
			{
				return false;
			}
			else
			{
				m_mediaport.assign( anchor, pContent );
				while( *pContent == ' ' ) pContent++;
				if( *pContent == '\0' )
				{
					return false;
				}
			}

			// transport
			anchor = pContent;
			while( *pContent != ' ' ) pContent++;
			if( *pContent == '\0' )
			{
				return false;
			}
			else
			{
				m_transport.assign( anchor, pContent );
				while( *pContent == ' ' ) pContent++;
				if( *pContent == '\0' )
				{
					return false;
				}
			}

			// rtpmap
			while( *pContent != '\r' && *pContent != '\n' )
			{
				anchor = pContent;
				while( *pContent != ' ' && *pContent != '\r' && *pContent != '\n' ) pContent++;
				if( *pContent == '\0' )
				{
					return false;
				}
				else
				{
					std::string rtpmap( anchor, pContent );
					m_rtpmap.insert( std::make_pair( rtpmap, "" ) );
					while( *pContent == ' ' ) pContent++;
					if( *pContent == '\0' )
					{
						return false;
					}
				}
			}
		}
		else if( ch == 'b' )
        {
            if( *pContent++ != 'b' || *pContent++ != '=' )
            {
                return false;
            }

            while( *pContent != '\0' && *pContent != '\r' && *pContent != '\n' ) pContent++;
            if( *pContent == '\0' )
            {
                return false;
            }
		}
		else if( ch == 'a' )
		{
			if( *pContent++ != 'a' || *pContent++ != '=' )
			{
				return false;
			}

			std::string type;
			anchor = pContent;
			while( *pContent != ':' && *pContent != '\r' && *pContent != '\n' ) pContent++;
			if( *pContent == '\0' )
			{
				return false;
			}
			else
			{
				type.assign( anchor, pContent );
				while( *pContent == ' ' || *pContent == ':' ) pContent++;
				if( *pContent == '\0' )
				{
					return false;
				}
			}

			if( CGBMessage::CompareNoCase( type, "rtpmap" ) )
			{
				anchor = pContent;
				while( *pContent != ' ' ) pContent++;
				if( *pContent == '\0' )
				{
					return false;
				}

				std::string rtpmap( anchor, pContent );
				while( *pContent == ' ' ) pContent++;
				if( *pContent == '\0' )
				{
					return false;
				}

				std::map< std::string, std::string >::iterator it = m_rtpmap.find( rtpmap );
				if( it == m_rtpmap.end() )
				{
					return false;
				}

				anchor = pContent;
				while( *pContent != '\0' && *pContent != '\r' && *pContent != '\n' )
				{
					pContent++;
				}

				if( anchor != pContent )
				{
					it->second.assign( anchor, pContent );
				}
			}
			else if( CGBMessage::CompareNoCase( type, "downloadspeed" ) )
			{
				anchor = pContent;
				while( *pContent != '\0' && *pContent != '\r' && *pContent != '\n' )
				{
					pContent++;
				}

				if( anchor != pContent )
				{
					m_downloadspeed.assign( anchor, pContent );
				}
			}
			else
			{
				anchor = pContent;
				while( *pContent != '\0' && *pContent != '\r' && *pContent != '\n' )
				{
					pContent++;
				}

				if( *pContent != '\0' )
				{
					// send or recv
				}
			}
		}
		else if( ch == 'y' )
		{
			if( *pContent++ != 'y' || *pContent++ != '=' )
			{
				return false;
			}

			anchor = pContent;
			while( *pContent != '\0' && *pContent != '\r' && *pContent != '\n' )
			{
				pContent++;
			}

			if( anchor != pContent )
			{
				m_ssrc.assign( anchor, pContent );
			}
		}
		else if( ch == 'f' )
		{
			if( *pContent++ != 'f' || *pContent++ != '=' )
			{
				return false;
			}

			anchor = pContent;
			while( *pContent != '\0' && *pContent != '\r' && *pContent != '\n' )
			{
				pContent++;
			}

			if( anchor != pContent )
			{
				m_audioproterty.assign( anchor, pContent );
			}
		}
		else
		{
            while( *pContent != '\0' && *pContent != '\r' && *pContent != '\n' ) pContent++;
            if( *pContent == '\0' )
            {
                return false;
            }
		}

		// next line
		while( *pContent == '\r' || *pContent == '\n' ) pContent++;
		if( *pContent == '\0' )
		{
			break;
		}
	}

	return true;
}

void CVodDownloadMsg::Process( CSipInterface *pInterface )
{
	if( pInterface != NULL )
	{
		pInterface->OnDownloadVodStream( *this );
	}
}

//////////////////////////////////////////////////////////////////////////
bool CVodStreamMsg::Encode( std::string &message )
{
	// subject
	if( m_subject.empty() )
	{
        m_subject.clear();
        m_subject += m_sender.id;
        m_subject += ":";
        m_subject += m_sender.seq;
        m_subject += ",";
        m_subject += m_recver.id;
        m_subject += ":";
        m_subject += m_recver.seq;
	}

	// sdp
	std::stringstream context;
	context<<"v=0\r\n";
	context<<"o="<< m_recvid <<" 0 0 IN IP4 "<< m_mediaip <<"\r\n";
	context<<"s=Playback\r\n";
	context<<"u=" << m_uri.id << ":" << m_uri.param << "\r\n";
	context<<"c=IN IP4 "<< m_connect <<"\r\n";
	context<<"t="<< m_starttime << " " << m_endtime << "\r\n";

	context<<"m=video "<< m_mediaport <<" RTP/AVP";
	std::map< std::string, std::string >::const_iterator it;
	for (it = m_rtpmap.begin(); it != m_rtpmap.end(); it++)
	{
		std::string key = it->first;
		std::string text = it->second;
		if ( !text.empty() )
		{
			context<<" "<<key;
		}
	}
	context<<"\r\n";

	context<<"a=recvonly\r\n";
	for (it = m_rtpmap.begin(); it != m_rtpmap.end(); it++)
	{
		std::string key = it->first;
		std::string text = it->second;
		if ( !text.empty() )
		{
			context<<"a=rtpmap:"<<key<<" "<<text<<"\r\n";
		}
	}


	if( !m_ssrc.empty() )
	{
		context<<"y="<< m_ssrc << "\r\n";
	}

	if( !m_audioproterty.empty() )
	{
		context<<"f=" << m_audioproterty << "\r\n";
	}

	message = context.str();
	return true;
}

bool CVodStreamMsg::Decode( const char *body, std::string &reason )
{
	// message without sdp
	if( body == NULL )
	{
#ifdef _DEBUG
		WARN_LOG( "录像回放请求消息体不带SDP!" );
#endif
		return true;
	}

	const char* pContent = body;
	const char* anchor = 0;

	// 解析SDP字段
	while( pContent != '\0' )
	{
		char ch = *pContent;
		if( ch == 'v' )
		{
			if( *pContent++ != 'v' || *pContent++ != '=' )
			{
				DEBUG_LOG( ch << "字段有误！" );
				return false;
			}

			while( *pContent != '\0' && *pContent != '\r' && *pContent != '\n' ) pContent++;
			if( *pContent == '\0' )
			{
				DEBUG_LOG( ch << "字段有误！" );
				return false;
			}
		}
		else if( ch == 'o' )
		{
			if( *pContent++ != 'o' || *pContent++ != '=' )
			{
				DEBUG_LOG( ch << "字段有误！" );
				return false;
			}

			// id
			anchor = pContent;
			while( *pContent != ' ' ) pContent++;
			if( *pContent == '\0' )
			{
				DEBUG_LOG( ch << "字段有误！" );
				return false;
			}
			else
			{
				m_recvid.assign( anchor, pContent );
				while( *pContent == ' ' ) pContent++;
				if( *pContent == '\0' )
				{
					DEBUG_LOG( ch << "字段有误！" );
					return false;
				}
			}

			// session
			anchor = pContent;
			while( *pContent != ' ' ) pContent++;
			if( *pContent == '\0' )
			{
				DEBUG_LOG( ch << "字段有误！" );
				return false;
			}
			else
			{
				m_sessionid.assign( anchor, pContent );
				while( *pContent == ' ' ) pContent++;
				if( *pContent == '\0' )
				{
					return false;
				}
			}

			// version
			anchor = pContent;
			while( *pContent != ' ' ) pContent++;
			if( *pContent == '\0' )
			{
				DEBUG_LOG( ch << "字段有误！" );
				return false;
			}
			else
			{
				m_version.assign( anchor, pContent );
				while( *pContent == ' ' ) pContent++;
				if( *pContent == '\0' )
				{
					DEBUG_LOG( ch << "字段有误！" );
					return false;
				}
			}

			// in
			anchor = pContent;
			while( *pContent != ' ' ) pContent++;
			if( *pContent == '\0' )
			{
				DEBUG_LOG( ch << "字段有误！" );
				return false;
			}
			else
			{
				while( *pContent == ' ' ) pContent++;
				if( *pContent == '\0' )
				{
					DEBUG_LOG( ch << "字段有误！" );
					return false;
				}
			}

			// ip version
			anchor = pContent;
			while( *pContent != ' ' ) pContent++;
			if( *pContent == '\0' )
			{
				DEBUG_LOG( ch << "字段有误！" );
				return false;
			}
			else
			{
				while( *pContent == ' ' ) pContent++;
				if( *pContent == '\0' )
				{
					DEBUG_LOG( ch << "字段有误！" );
					return false;
				}
			}

			// media ip
			anchor = pContent;
			while( *pContent != '\r' && *pContent != '\n' ) pContent++;
			if( *pContent == '\0' )
			{
				DEBUG_LOG( ch << "字段有误！" );
				return false;
			}
			else
			{
				m_mediaip.assign( anchor, pContent );
			}
		}
		else if( ch == 's' )
		{
			if( *pContent++ != 's' || *pContent++ != '=' )
			{
				DEBUG_LOG( ch << "字段有误！" );
				return false;
			}

			while( *pContent != '\r' && *pContent != '\n' ) pContent++;
			if( *pContent == '\0' )
			{
				DEBUG_LOG( ch << "字段有误！" );
				return false;
			}
		}
		else if( ch == 'u' )
		{
			if( *pContent++ != 'u' || *pContent++ != '=' )
			{
				DEBUG_LOG( ch << "字段有误！" );
				return false;
			}

			// uri id
			anchor = pContent;
			while( *pContent != ':' ) pContent++;
			if( *pContent == '\0' )
			{
				DEBUG_LOG( ch << "字段有误！" );
				return false;
			}
			else
			{
				m_uri.id.assign( anchor, pContent );
				pContent++;
				if( *pContent == '\0' )
				{
					DEBUG_LOG( ch << "字段有误！" );
					return false;
				}
			}

			// uri param
			anchor = pContent;
			while( *pContent != '\r' && *pContent != '\n' ) pContent++;
			if( *pContent == '\0' )
			{
				DEBUG_LOG( ch << "字段有误！" );
				return false;
			}
			else
			{
				m_uri.param.assign( anchor, pContent );
			}
		}
		else if( ch == 'c' )
		{
			if( *pContent++ != 'c' || *pContent++ != '=' )
			{
				DEBUG_LOG( ch << "字段有误！" );
				return false;
			}

			// in
			while( *pContent != ' ' ) pContent++;
			if( pContent == '\0' )
			{
				DEBUG_LOG( ch << "字段有误！" );
				return false;
			}
			else
			{
				while( *pContent == ' ' ) pContent++;
				if( *pContent == '\0' )
				{
					DEBUG_LOG( ch << "字段有误！" );
					return false;
				}
			}

			// ip version
			while( *pContent != ' ' ) pContent++;
			if( pContent == '\0' )
			{
				DEBUG_LOG( ch << "字段有误！" );
				return false;
			}
			else
			{
				while( *pContent == ' ' ) pContent++;
				if( *pContent == '\0' )
				{
					DEBUG_LOG( ch << "字段有误！" );
					return false;
				}
			}

			// connect ip
			anchor = pContent;
			while( *pContent != '\r' && *pContent != '\n' ) pContent++;
			if( *pContent == '\0' )
			{
				DEBUG_LOG( ch << "字段有误！" );
				return false;
			}
			else
			{
				m_connect.assign( anchor, pContent );
			}
		}
		else if( ch == 't' )
		{
			if( *pContent++ != 't' || *pContent++ != '=' )
			{
				DEBUG_LOG( ch << "字段有误！" );
				return false;
			}

			// start time
			anchor = pContent;
			while( *pContent != ' ' ) pContent++;
			if( *pContent == '\0' )
			{
				DEBUG_LOG( ch << "字段有误！" );
				return false;
			}
			else
			{
				m_starttime.assign( anchor, pContent );
				while( *pContent == ' ' ) pContent++;
				if( *pContent == '\0' )
				{
					DEBUG_LOG( ch << "字段有误！" );
					return false;
				}
			}

			// end time
			anchor = pContent;
			while( *pContent != '\r' && *pContent != '\n' ) pContent++;
			if( *pContent == '\0' )
			{
				DEBUG_LOG( ch << "字段有误！" );
				return false;
			}
			else
			{
				m_endtime.assign( anchor, pContent );
			}
		}
		else if( ch == 'm' )
		{
			if( *pContent++ != 'm' || *pContent++ != '=' )
			{
				DEBUG_LOG( ch << "字段有误！" );
				return false;
			}

			// media type
			anchor = pContent;
			while( *pContent != ' ' ) pContent++;
			if( *pContent == '\0' )
			{
				DEBUG_LOG( ch << "字段有误！" );
				return false;
			}
			else
			{
				m_mediatype.assign(anchor, pContent);
				while( *pContent == ' ' ) pContent++;
				if( *pContent == '\0' )
				{
					DEBUG_LOG( ch << "字段有误！" );
					return false;
				}
			}

			// port
			anchor = pContent;
			while( *pContent != ' ') pContent++;
			if( *pContent == '\0' )
			{
				DEBUG_LOG( ch << "字段有误！" );
				return false;
			}
			else
			{
				m_mediaport.assign( anchor, pContent );
				while( *pContent == ' ' ) pContent++;
				if( *pContent == '\0' )
				{
					DEBUG_LOG( ch << "字段有误！" );
					return false;
				}
			}

			// transport
			anchor = pContent;
			while( *pContent != ' ' ) pContent++;
			if( *pContent == '\0' )
			{
				DEBUG_LOG( ch << "字段有误！" );
				return false;
			}
			else
			{
				m_transport.assign( anchor, pContent );
				while( *pContent == ' ' ) pContent++;
				if( *pContent == '\0' )
				{
					DEBUG_LOG( ch << "字段有误！" );
					return false;
				}
			}

			// rtpmap
			while( *pContent != '\r' && *pContent != '\n' )
			{
				anchor = pContent;
				while( *pContent != ' ' && *pContent != '\r' && *pContent != '\n' ) pContent++;
				if( *pContent == '\0' )
				{
					DEBUG_LOG( ch << "字段有误！" );
					return false;
				}
				else
				{
					std::string rtpmap( anchor, pContent );
					m_rtpmap.insert( std::make_pair( rtpmap, "" ) );
					while( *pContent == ' ' ) pContent++;
					if( *pContent == '\0' )
					{
						DEBUG_LOG( ch << "字段有误！" );
						return false;
					}
				}
			}
		}
		else if( ch == 'b' )
        {
            if( *pContent++ != 'b' || *pContent++ != '=' )
            {
				DEBUG_LOG( ch << "字段有误！" );
                return false;
            }

            while( *pContent != '\0' && *pContent != '\r' && *pContent != '\n' ) pContent++;
            if( *pContent == '\0' )
            {
				DEBUG_LOG( ch << "字段有误！" );
                return false;
            }
		}
		else if( ch == 'a' )
		{
			if( *pContent++ != 'a' || *pContent++ != '=' )
			{
				DEBUG_LOG( ch << "字段有误！" );
				return false;
			}

			std::string type;
			anchor = pContent;
			while( *pContent != ':' && *pContent != '\r' && *pContent != '\n' ) pContent++;
			if( *pContent == '\0' )
			{
				DEBUG_LOG( ch << "字段有误！" );
				return false;
			}
			else
			{
				type.assign( anchor, pContent );
				while( *pContent == ' ' || *pContent == ':' ) pContent++;
				if( *pContent == '\0' )
				{
					DEBUG_LOG( ch << "字段有误！" );
					return false;
				}
			}

			if( CGBMessage::CompareNoCase( type, "rtpmap" ) )
			{
				anchor = pContent;
				while( *pContent != ' ' ) pContent++;
				if( *pContent == '\0' )
				{
					DEBUG_LOG( ch << "字段有误！" );
					return false;
				}

				std::string rtpmap( anchor, pContent );
				while( *pContent == ' ' ) pContent++;
				if( *pContent == '\0' )
				{
					DEBUG_LOG( ch << "字段有误！" );
					return false;
				}

				std::map< std::string, std::string >::iterator it = m_rtpmap.find( rtpmap );
				if( it == m_rtpmap.end() )
				{
					DEBUG_LOG( ch << "字段有误！" );
					return false;
				}

				anchor = pContent;
				while( *pContent != '\0' && *pContent != '\r' && *pContent != '\n' )
				{
					pContent++;
				}

				if( anchor != pContent )
				{
					it->second.assign( anchor, pContent );
				}
			}
			else
			{
				anchor = pContent;
				while( *pContent != '\0' && *pContent != '\r' && *pContent != '\n' )
				{
					pContent++;
				}

				if( *pContent != '\0' )
				{
					// send or recv
				}
			}
		}
		else if( ch == 'y' )
		{
			if( *pContent++ != 'y' || *pContent++ != '=' )
			{
				DEBUG_LOG( ch << "字段有误！" );
				return false;
			}

			anchor = pContent;
			while( *pContent != '\0' && *pContent != '\r' && *pContent != '\n' )
			{
				pContent++;
			}

			if( anchor != pContent )
			{
				m_ssrc.assign( anchor, pContent );
			}
		}
		else if( ch == 'f' )
		{
			if( *pContent++ != 'f' || *pContent++ != '=' )
			{
				DEBUG_LOG( ch << "字段有误！" );
				return false;
			}

			anchor = pContent;
			while( *pContent != '\0' && *pContent != '\r' && *pContent != '\n' )
			{
				pContent++;
			}

			if( anchor != pContent )
			{
				m_audioproterty.assign( anchor, pContent );
			}
		}
		else
		{
            while( *pContent != '\0' && *pContent != '\r' && *pContent != '\n' ) pContent++;
            if( *pContent == '\0' )
            {
				DEBUG_LOG( ch << "字段有误！" );
                return false;
            }
		}

		// next line
		while( *pContent == '\r' || *pContent == '\n' ) pContent++;
		if( *pContent == '\0' )
		{
			break;
		}
	}

	return true;
}

void CVodStreamMsg::Process( CSipInterface *pInterface )
{
    if( pInterface != NULL )
    {
        pInterface->OnOpenVodStream( *this );
    }
}

//////////////////////////////////////////////////////////////////////////
bool CRealStreamMsg::Encode( std::string &message )
{
	// subject
	if( m_subject.empty() )
	{
        m_subject.clear();
        m_subject += m_sender.id;
        m_subject += ":";
        m_subject += m_sender.seq;
        m_subject += ",";
        m_subject += m_recver.id;
        m_subject += ":";
        m_subject += m_recver.seq;
	}

	// sdp
	std::stringstream context;
	context<<"v=0\r\n";
	context<<"o="<< m_recvid <<" 0 0 IN IP4 "<< m_mediaip <<"\r\n";
	context<<"s=Play\r\n";
	context<<"c=IN IP4 "<< m_connect <<"\r\n";
	context<<"t=0 0\r\n";

	context<<"m=video "<< m_mediaport <<" RTP/AVP";
	std::map< std::string, std::string >::const_iterator it;
	for (it = m_rtpmap.begin(); it != m_rtpmap.end(); it++)
	{
		std::string key = it->first;
		std::string text = it->second;
		if ( !text.empty() )
		{
			context<<" "<<key;
		}
	}
	context<<"\r\n";

	context<<"a=recvonly\r\n";
	for (it = m_rtpmap.begin(); it != m_rtpmap.end(); it++)
	{
		std::string key = it->first;
		std::string text = it->second;
		if ( !text.empty() )
		{
			context<<"a=rtpmap:"<<key<<" "<<text<<"\r\n";
		}
	}

	if( !m_ssrc.empty() )
	{
		context<<"y="<< m_ssrc << "\r\n";
	}

	if( !m_audioproterty.empty() )
	{
		context<<"f=" << m_audioproterty << "\r\n";
	}

	message = context.str();
	return true;
}

bool CRealStreamMsg::Decode( const char *body, std::string &reason )
{
	// message without sdp
	if( body == NULL )
	{
#ifdef _DEBUG
		WARN_LOG( "实时流请求消息体不带SDP!" );
#endif
		return true;
	}

	const char* pContent = body;
	const char* anchor = 0;

	// 解析SDP字段
	while( pContent != '\0' )
	{
		char ch = *pContent;
		if( ch == 'v' )
		{
			if( *pContent++ != 'v' || *pContent++ != '=' )
			{
				return false;
			}

			while( *pContent != '\0' && *pContent != '\r' && *pContent != '\n' ) pContent++;
			if( *pContent == '\0' )
			{
				return false;
			}
		}
		else if( ch == 'o' )
		{
			if( *pContent++ != 'o' || *pContent++ != '=' )
			{
				return false;
			}

			// id
			anchor = pContent;
			while( *pContent != ' ' ) pContent++;
			if( *pContent == '\0' )
			{
				return false;
			}
			else
			{
				m_recvid.assign( anchor, pContent );
				while( *pContent == ' ' ) pContent++;
				if( *pContent == '\0' )
				{
					return false;
				}
			}

			// session
			anchor = pContent;
			while( *pContent != ' ' ) pContent++;
			if( *pContent == '\0' )
			{
				return false;
			}
			else
			{
				m_sessionid.assign( anchor, pContent );
				while( *pContent == ' ' ) pContent++;
				if( *pContent == '\0' )
				{
					return false;
				}
			}

			// version
			anchor = pContent;
			while( *pContent != ' ' ) pContent++;
			if( *pContent == '\0' )
			{
				return false;
			}
			else
			{
				m_version.assign( anchor, pContent );
				while( *pContent == ' ' ) pContent++;
				if( *pContent == '\0' )
				{
					return false;
				}
			}

			// in
			anchor = pContent;
			while( *pContent != ' ' ) pContent++;
			if( *pContent == '\0' )
			{
				return false;
			}
			else
			{
				while( *pContent == ' ' ) pContent++;
				if( *pContent == '\0' )
				{
					return false;
				}
			}

			// ip version
			anchor = pContent;
			while( *pContent != ' ' ) pContent++;
			if( *pContent == '\0' )
			{
				return false;
			}
			else
			{
				while( *pContent == ' ' ) pContent++;
				if( *pContent == '\0' )
				{
					return false;
				}
			}

			// media ip
			anchor = pContent;
			while( *pContent != '\r' && *pContent != '\n' ) pContent++;
			if( *pContent == '\0' )
			{
				return false;
			}
			else
			{
				m_mediaip.assign( anchor, pContent );
			}
		}
		else if( ch == 's' )
		{
			if( *pContent++ != 's' || *pContent++ != '=' )
			{
				return false;
			}

			while( *pContent != '\r' && *pContent != '\n' ) pContent++;
			if( *pContent == '\0' )
			{
				return false;
			}
		}
		else if( ch == 'c' )
		{
			if( *pContent++ != 'c' || *pContent++ != '=' )
			{
				return false;
			}

			// in
			while( *pContent != ' ' ) pContent++;
			if( pContent == '\0' )
			{
				return false;
			}
			else
			{
				while( *pContent == ' ' ) pContent++;
				if( *pContent == '\0' )
				{
					return false;
				}
			}

			// ip version
			while( *pContent != ' ' ) pContent++;
			if( pContent == '\0' )
			{
				return false;
			}
			else
			{
				while( *pContent == ' ' ) pContent++;
				if( *pContent == '\0' )
				{
					return false;
				}
			}

			// connect ip
			anchor = pContent;
			while( *pContent != '\r' && *pContent != '\n' ) pContent++;
			if( *pContent == '\0' )
			{
				return false;
			}
			else
			{
				m_connect.assign( anchor, pContent );
			}
		}
		else if( ch == 't' )
		{
			if( *pContent++ != 't' || *pContent++ != '=' )
			{
				return false;
			}

			// start time
			anchor = pContent;
			while( *pContent != ' ' ) pContent++;
			if( *pContent == '\0' )
			{
				return false;
			}
			else
			{
				m_starttime.assign( anchor, pContent );
				while( *pContent == ' ' ) pContent++;
				if( *pContent == '\0' )
				{
					return false;
				}
			}

			// end time
			anchor = pContent;
			while( *pContent != '\r' && *pContent != '\n' ) pContent++;
			if( *pContent == '\0' )
			{
				return false;
			}
			else
			{
				m_endtime.assign( anchor, pContent );
			}
		}
		else if( ch == 'm' )
		{
			if( *pContent++ != 'm' || *pContent++ != '=' )
			{
				return false;
			}

			// media type
			anchor = pContent;
			while( *pContent != ' ' ) pContent++;
			if( *pContent == '\0' )
			{
				return false;
			}
			else
			{
				m_mediatype.assign(anchor, pContent);
				while( *pContent == ' ' ) pContent++;
				if( *pContent == '\0' )
				{
					return false;
				}
			}

			// port
			anchor = pContent;
			while( *pContent != ' ') pContent++;
			if( *pContent == '\0' )
			{
				return false;
			}
			else
			{
				m_mediaport.assign( anchor, pContent );
				while( *pContent == ' ' ) pContent++;
				if( *pContent == '\0' )
				{
					return false;
				}
			}

			// transport
			anchor = pContent;
			while( *pContent != ' ' ) pContent++;
			if( *pContent == '\0' )
			{
				return false;
			}
			else
			{
				m_transport.assign( anchor, pContent );
				while( *pContent == ' ' ) pContent++;
				if( *pContent == '\0' )
				{
					return false;
				}
			}

			// rtpmap
			while( *pContent != '\r' && *pContent != '\n' )
			{
				anchor = pContent;
				while( *pContent != ' ' && *pContent != '\r' && *pContent != '\n' ) pContent++;
				if( *pContent == '\0' )
				{
					return false;
				}
				else
				{
					std::string rtpmap( anchor, pContent );
					m_rtpmap.insert( std::make_pair( rtpmap, "" ) );
					while( *pContent == ' ' ) pContent++;
					if( *pContent == '\0' )
					{
						return false;
					}
				}
			}
        }
        else if( ch == 'b' )
        {
            if( *pContent++ != 'b' || *pContent++ != '=' )
            {
                return false;
            }

            while( *pContent != '\0' && *pContent != '\r' && *pContent != '\n' ) pContent++;
            if( *pContent == '\0' )
            {
                return false;
            }
        }
		else if( ch == 'a' )
		{
			if( *pContent++ != 'a' || *pContent++ != '=' )
			{
				return false;
			}

			std::string type;
			anchor = pContent;
			while( *pContent != ':' && *pContent != '\r' && *pContent != '\n' ) pContent++;
			if( *pContent == '\0' )
			{
				return false;
			}
			else
			{
				type.assign( anchor, pContent );
				while( *pContent == ' ' || *pContent == ':' ) pContent++;
				if( *pContent == '\0' )
				{
					return false;
				}
			}

			if( CGBMessage::CompareNoCase( type, "rtpmap" ) )
			{
				anchor = pContent;
				while( *pContent != ' ' ) pContent++;
				if( *pContent == '\0' )
				{
					return false;
				}

				std::string rtpmap( anchor, pContent );
				while( *pContent == ' ' ) pContent++;
				if( *pContent == '\0' )
				{
					return false;
				}

				std::map< std::string, std::string >::iterator it = m_rtpmap.find( rtpmap );
				if( it == m_rtpmap.end() )
				{
					return false;
				}

				anchor = pContent;
				while( *pContent != '\0' && *pContent != '\r' && *pContent != '\n' )
				{
					pContent++;
				}

				if( anchor != pContent )
				{
					it->second.assign( anchor, pContent );
				}
			}
			else
			{
				anchor = pContent;
				while( *pContent != '\0' && *pContent != '\r' && *pContent != '\n' )
				{
					pContent++;
				}

				if( *pContent != '\0' )
				{
					// send or recv
				}
			}
		}
		else if( ch == 'y' )
		{
			if( *pContent++ != 'y' || *pContent++ != '=' )
			{
				return false;
			}

			anchor = pContent;
			while( *pContent != '\0' && *pContent != '\r' && *pContent != '\n' )
			{
				pContent++;
			}

			if( anchor != pContent )
			{
				m_ssrc.assign( anchor, pContent );
			}
		}
		else if( ch == 'f' )
		{
			if( *pContent++ != 'f' || *pContent++ != '=' )
			{
				return false;
			}

			anchor = pContent;
			while( *pContent != '\0' && *pContent != '\r' && *pContent != '\n' )
			{
				pContent++;
			}

			if( anchor != pContent )
			{
				m_audioproterty.assign( anchor, pContent );
			}
		}
		else
        {
            while( *pContent != '\0' && *pContent != '\r' && *pContent != '\n' ) pContent++;
            if( *pContent == '\0' )
            {
                return false;
            }
		}

		// next line
		while( *pContent == '\r' || *pContent == '\n' ) pContent++;
		if( *pContent == '\0' )
		{
			break;
		}
	}

	return true;
}

void CRealStreamMsg::Process( CSipInterface *pInterface )
{
	if( pInterface != NULL )
	{
		pInterface->OnOpenRealStream( *this );
	}
}
// #define NP_GB_SDK
//////////////////////////////////////////////////////////////////////////
bool CRealStreamToDecorderMsg::Encode( std::string &message )
{
	// subject
	if( m_subject.empty() )
	{
        m_subject.clear();
        m_subject += m_sender.id;
        m_subject += ":";
        m_subject += m_sender.seq;
        m_subject += ",";
        m_subject += m_recver.id;
        m_subject += ":";
        m_subject += m_recver.seq;
	}

	// sdp
    // 发给解码器的Invite消息不携带sdp
#ifdef NP_GB_SDK
	std::stringstream context;
	context<<"v=0\r\n";
	context<<"o="<< m_recvid <<" 0 0 IN IP4 "<< m_mediaip <<"\r\n";
	context<<"s=Play\r\n";
	context<<"c=IN IP4 "<< m_connect <<"\r\n";
	context<<"t=0 0\r\n";

	context<<"m=video "<< m_mediaport <<" RTP/AVP";
	std::map< std::string, std::string >::const_iterator it;
	for (it = m_rtpmap.begin(); it != m_rtpmap.end(); it++)
	{
		std::string key = it->first;
		std::string text = it->second;
		if ( !text.empty() )
		{
			context<<" "<<key;
		}
	}
	context<<"\r\n";

	context<<"a=sendonly\r\n";
	for (it = m_rtpmap.begin(); it != m_rtpmap.end(); it++)
	{
		std::string key = it->first;
		std::string text = it->second;
		if ( !text.empty() )
		{
			context<<"a=rtpmap:"<<key<<" "<<text<<"\r\n";
		}
	}

	if( !m_ssrc.empty() )
	{
		context<<"y="<< m_ssrc << "\r\n";
	}

	if( !m_audioproterty.empty() )
	{
		context<<"f=" << m_audioproterty << "\r\n";
	}

	message = context.str();
#endif

    return true;
}

bool CRealStreamToDecorderMsg::Decode( const char *body, std::string &reason )
{
    // message without sdp
    if( body == NULL )
    {
#ifdef _DEBUG
        WARN_LOG( "实时流请求消息体不带SDP!" );
#endif
        return true;
    }

    const char* pContent = body;
    const char* anchor = 0;

    // 解析SDP字段
    while( pContent != '\0' )
    {
        char ch = *pContent;
        if( ch == 'v' )
        {
            if( *pContent++ != 'v' || *pContent++ != '=' )
            {
                return false;
            }

            while( *pContent != '\0' && *pContent != '\r' && *pContent != '\n' ) pContent++;
            if( *pContent == '\0' )
            {
                return false;
            }
        }
        else if( ch == 'o' )
        {
            if( *pContent++ != 'o' || *pContent++ != '=' )
            {
                return false;
            }

            // id
            anchor = pContent;
            while( *pContent != ' ' ) pContent++;
            if( *pContent == '\0' )
            {
                return false;
            }
            else
            {
                m_recvid.assign( anchor, pContent );
                while( *pContent == ' ' ) pContent++;
                if( *pContent == '\0' )
                {
                    return false;
                }
            }

            // session
            anchor = pContent;
            while( *pContent != ' ' ) pContent++;
            if( *pContent == '\0' )
            {
                return false;
            }
            else
            {
                m_sessionid.assign( anchor, pContent );
                while( *pContent == ' ' ) pContent++;
                if( *pContent == '\0' )
                {
                    return false;
                }
            }

            // version
            anchor = pContent;
            while( *pContent != ' ' ) pContent++;
            if( *pContent == '\0' )
            {
                return false;
            }
            else
            {
                m_version.assign( anchor, pContent );
                while( *pContent == ' ' ) pContent++;
                if( *pContent == '\0' )
                {
                    return false;
                }
            }

            // in
            anchor = pContent;
            while( *pContent != ' ' ) pContent++;
            if( *pContent == '\0' )
            {
                return false;
            }
            else
            {
                while( *pContent == ' ' ) pContent++;
                if( *pContent == '\0' )
                {
                    return false;
                }
            }

            // ip version
            anchor = pContent;
            while( *pContent != ' ' ) pContent++;
            if( *pContent == '\0' )
            {
                return false;
            }
            else
            {
                while( *pContent == ' ' ) pContent++;
                if( *pContent == '\0' )
                {
                    return false;
                }
            }

            // media ip
            anchor = pContent;
            while( *pContent != '\r' && *pContent != '\n' ) pContent++;
            if( *pContent == '\0' )
            {
                return false;
            }
            else
            {
                m_mediaip.assign( anchor, pContent );
            }
        }
        else if( ch == 's' )
        {
            if( *pContent++ != 's' || *pContent++ != '=' )
            {
                return false;
            }

            while( *pContent != '\r' && *pContent != '\n' ) pContent++;
            if( *pContent == '\0' )
            {
                return false;
            }
        }
        else if( ch == 'c' )
        {
            if( *pContent++ != 'c' || *pContent++ != '=' )
            {
                return false;
            }

            // in
            while( *pContent != ' ' ) pContent++;
            if( pContent == '\0' )
            {
                return false;
            }
            else
            {
                while( *pContent == ' ' ) pContent++;
                if( *pContent == '\0' )
                {
                    return false;
                }
            }

            // ip version
            while( *pContent != ' ' ) pContent++;
            if( pContent == '\0' )
            {
                return false;
            }
            else
            {
                while( *pContent == ' ' ) pContent++;
                if( *pContent == '\0' )
                {
                    return false;
                }
            }

            // connect ip
            anchor = pContent;
            while( *pContent != '\r' && *pContent != '\n' ) pContent++;
            if( *pContent == '\0' )
            {
                return false;
            }
            else
            {
                m_connect.assign( anchor, pContent );
            }
        }
        else if( ch == 't' )
        {
            if( *pContent++ != 't' || *pContent++ != '=' )
            {
                return false;
            }

            // start time
            anchor = pContent;
            while( *pContent != ' ' ) pContent++;
            if( *pContent == '\0' )
            {
                return false;
            }
            else
            {
                m_starttime.assign( anchor, pContent );
                while( *pContent == ' ' ) pContent++;
                if( *pContent == '\0' )
                {
                    return false;
                }
            }

            // end time
            anchor = pContent;
            while( *pContent != '\r' && *pContent != '\n' ) pContent++;
            if( *pContent == '\0' )
            {
                return false;
            }
            else
            {
                m_endtime.assign( anchor, pContent );
            }
        }
        else if( ch == 'm' )
        {
            if( *pContent++ != 'm' || *pContent++ != '=' )
            {
                return false;
            }

            // media type
            anchor = pContent;
            while( *pContent != ' ' ) pContent++;
            if( *pContent == '\0' )
            {
                return false;
            }
            else
            {
                m_mediatype.assign(anchor, pContent);
                while( *pContent == ' ' ) pContent++;
                if( *pContent == '\0' )
                {
                    return false;
                }
            }

            // port
            anchor = pContent;
            while( *pContent != ' ') pContent++;
            if( *pContent == '\0' )
            {
                return false;
            }
            else
            {
                m_mediaport.assign( anchor, pContent );
                while( *pContent == ' ' ) pContent++;
                if( *pContent == '\0' )
                {
                    return false;
                }
            }

            // transport
            anchor = pContent;
            while( *pContent != ' ' ) pContent++;
            if( *pContent == '\0' )
            {
                return false;
            }
            else
            {
                m_transport.assign( anchor, pContent );
                while( *pContent == ' ' ) pContent++;
                if( *pContent == '\0' )
                {
                    return false;
                }
            }

            // rtpmap
            while( *pContent != '\r' && *pContent != '\n' )
            {
                anchor = pContent;
                while( *pContent != ' ' && *pContent != '\r' && *pContent != '\n' ) pContent++;
                if( *pContent == '\0' )
                {
                    return false;
                }
                else
                {
                    std::string rtpmap( anchor, pContent );
                    m_rtpmap.insert( std::make_pair( rtpmap, "" ) );
                    while( *pContent == ' ' ) pContent++;
                    if( *pContent == '\0' )
                    {
                        return false;
                    }
                }
            }
        }
        else if( ch == 'b' )
        {
            if( *pContent++ != 'b' || *pContent++ != '=' )
            {
                return false;
            }

            while( *pContent != '\0' && *pContent != '\r' && *pContent != '\n' ) pContent++;
            if( *pContent == '\0' )
            {
                return false;
            }
        }
        else if( ch == 'a' )
        {
            if( *pContent++ != 'a' || *pContent++ != '=' )
            {
                return false;
            }

            std::string type;
            anchor = pContent;
            while( *pContent != ':' && *pContent != '\r' && *pContent != '\n' ) pContent++;
            if( *pContent == '\0' )
            {
                return false;
            }
            else
            {
                type.assign( anchor, pContent );
                while( *pContent == ' ' || *pContent == ':' ) pContent++;
                if( *pContent == '\0' )
                {
                    return false;
                }
            }

            if( CGBMessage::CompareNoCase( type, "rtpmap" ) )
            {
                anchor = pContent;
                while( *pContent != ' ' ) pContent++;
                if( *pContent == '\0' )
                {
                    return false;
                }

                std::string rtpmap( anchor, pContent );
                while( *pContent == ' ' ) pContent++;
                if( *pContent == '\0' )
                {
                    return false;
                }

                std::map< std::string, std::string >::iterator it = m_rtpmap.find( rtpmap );
                if( it == m_rtpmap.end() )
                {
                    return false;
                }

                anchor = pContent;
                while( *pContent != '\0' && *pContent != '\r' && *pContent != '\n' )
                {
                    pContent++;
                }

                if( anchor != pContent )
                {
                    it->second.assign( anchor, pContent );
                }
            }
            else
            {
                anchor = pContent;
                while( *pContent != '\0' && *pContent != '\r' && *pContent != '\n' )
                {
                    pContent++;
                }

                if( *pContent != '\0' )
                {
                    // send or recv
                }
            }
        }
        else if( ch == 'y' )
        {
            if( *pContent++ != 'y' || *pContent++ != '=' )
            {
                return false;
            }

            anchor = pContent;
            while( *pContent != '\0' && *pContent != '\r' && *pContent != '\n' )
            {
                pContent++;
            }

            if( anchor != pContent )
            {
                m_ssrc.assign( anchor, pContent );
            }
        }
        else if( ch == 'f' )
        {
            if( *pContent++ != 'f' || *pContent++ != '=' )
            {
                return false;
            }

            anchor = pContent;
            while( *pContent != '\0' && *pContent != '\r' && *pContent != '\n' )
            {
                pContent++;
            }

            if( anchor != pContent )
            {
                m_audioproterty.assign( anchor, pContent );
            }
        }
        else
        {
            while( *pContent != '\0' && *pContent != '\r' && *pContent != '\n' ) pContent++;
            if( *pContent == '\0' )
            {
                return false;
            }
        }

        // next line
        while( *pContent == '\r' || *pContent == '\n' ) pContent++;
        if( *pContent == '\0' )
        {
            break;
        }
    }

    return true;
}

void CRealStreamToDecorderMsg::Process( CSipInterface *pInterface )
{
    if( pInterface != NULL )
    {
        pInterface->OnOpenRealStreamToDecorder( *this );
    }
}

//////////////////////////////////////////////////////////////////////////
bool CBroadcastStreamMsg::Encode( std::string &message )
{
	// subject
	if( m_subject.empty() )
	{
		m_subject.clear();
		m_subject += m_sender.id;
		m_subject += ":";
		m_subject += m_sender.seq;
		m_subject += ",";
		m_subject += m_recver.id;
		m_subject += ":";
		m_subject += m_recver.seq;
	}

	// sdp
	std::stringstream context;
	context<<"v=0\r\n";
	context<<"o="<< m_recvid <<" 0 0 IN IP4 "<< m_mediaip <<"\r\n";
	context<<"s=Play\r\n";
	context<<"c=IN IP4 "<< m_connect <<"\r\n";
	context<<"t=0 0\r\n";

	context<<"m=audio "<< m_mediaport <<" RTP/AVP";
	std::map< std::string, std::string >::const_iterator it;
	for (it = m_rtpmap.begin(); it != m_rtpmap.end(); it++)
	{
		std::string key = it->first;
		std::string text = it->second;
		if ( !text.empty() )
		{
			context<<" "<<key;
		}
	}
	context<<"\r\n";

	context<<"a=recvonly\r\n";
	for (it = m_rtpmap.begin(); it != m_rtpmap.end(); it++)
	{
		std::string key = it->first;
		std::string text = it->second;
		if ( !text.empty() )
		{
			context<<"a=rtpmap:"<<key<<" "<<text<<"\r\n";
		}
	}

	if( !m_ssrc.empty() )
	{
		context<<"y="<< m_ssrc << "\r\n";
	}

	if( !m_audioproterty.empty() )
	{
		context<<"f=" << m_audioproterty << "\r\n";
	}

	message = context.str();
	return true;
}

bool CBroadcastStreamMsg::Decode( const char *body, std::string &reason )
{
	// message without sdp
	if( body == NULL )
	{
#ifdef _DEBUG
		WARN_LOG( "语音广播请求消息体不带SDP!" );
#endif
		return true;
	}

	const char* pContent = body;
	const char* anchor = 0;

	// 解析SDP字段
	while( pContent != '\0' )
	{
		char ch = *pContent;
		if( ch == 'v' )
		{
			if( *pContent++ != 'v' || *pContent++ != '=' )
			{
				return false;
			}

			while( *pContent != '\0' && *pContent != '\r' && *pContent != '\n' ) pContent++;
			if( *pContent == '\0' )
			{
				return false;
			}
		}
		else if( ch == 'o' )
		{
			if( *pContent++ != 'o' || *pContent++ != '=' )
			{
				return false;
			}

			// id
			anchor = pContent;
			while( *pContent != ' ' ) pContent++;
			if( *pContent == '\0' )
			{
				return false;
			}
			else
			{
				m_recvid.assign( anchor, pContent );
				while( *pContent == ' ' ) pContent++;
				if( *pContent == '\0' )
				{
					return false;
				}
			}

			// session
			anchor = pContent;
			while( *pContent != ' ' ) pContent++;
			if( *pContent == '\0' )
			{
				return false;
			}
			else
			{
				m_sessionid.assign( anchor, pContent );
				while( *pContent == ' ' ) pContent++;
				if( *pContent == '\0' )
				{
					return false;
				}
			}

			// version
			anchor = pContent;
			while( *pContent != ' ' ) pContent++;
			if( *pContent == '\0' )
			{
				return false;
			}
			else
			{
				m_version.assign( anchor, pContent );
				while( *pContent == ' ' ) pContent++;
				if( *pContent == '\0' )
				{
					return false;
				}
			}

			// in
			anchor = pContent;
			while( *pContent != ' ' ) pContent++;
			if( *pContent == '\0' )
			{
				return false;
			}
			else
			{
				while( *pContent == ' ' ) pContent++;
				if( *pContent == '\0' )
				{
					return false;
				}
			}

			// ip version
			anchor = pContent;
			while( *pContent != ' ' ) pContent++;
			if( *pContent == '\0' )
			{
				return false;
			}
			else
			{
				while( *pContent == ' ' ) pContent++;
				if( *pContent == '\0' )
				{
					return false;
				}
			}

			// media ip
			anchor = pContent;
			while( *pContent != '\r' && *pContent != '\n' ) pContent++;
			if( *pContent == '\0' )
			{
				return false;
			}
			else
			{
				m_mediaip.assign( anchor, pContent );
			}
		}
		else if( ch == 's' )
		{
			if( *pContent++ != 's' || *pContent++ != '=' )
			{
				return false;
			}

			while( *pContent != '\r' && *pContent != '\n' ) pContent++;
			if( *pContent == '\0' )
			{
				return false;
			}
		}
		else if( ch == 'u' )
		{
			if( *pContent++ != 'u' || *pContent++ != '=' )
			{
				return false;
			}

			// uri id
			anchor = pContent;
			while( *pContent != ':' ) pContent++;
			if( *pContent == '\0' )
			{
				return false;
			}
			else
			{
				m_uri.id.assign( anchor, pContent );
				pContent++;
				if( *pContent == '\0' )
				{
					return false;
				}
			}

			// uri param
			anchor = pContent;
			while( *pContent != '\r' && *pContent != '\n' ) pContent++;
			if( *pContent == '\0' )
			{
				return false;
			}
			else
			{
				m_uri.param.assign( anchor, pContent );
			}
		}
		else if( ch == 'c' )
		{
			if( *pContent++ != 'c' || *pContent++ != '=' )
			{
				return false;
			}

			// in
			while( *pContent != ' ' ) pContent++;
			if( pContent == '\0' )
			{
				return false;
			}
			else
			{
				while( *pContent == ' ' ) pContent++;
				if( *pContent == '\0' )
				{
					return false;
				}
			}

			// ip version
			while( *pContent != ' ' ) pContent++;
			if( pContent == '\0' )
			{
				return false;
			}
			else
			{
				while( *pContent == ' ' ) pContent++;
				if( *pContent == '\0' )
				{
					return false;
				}
			}

			// connect ip
			anchor = pContent;
			while( *pContent != '\r' && *pContent != '\n' ) pContent++;
			if( *pContent == '\0' )
			{
				return false;
			}
			else
			{
				m_connect.assign( anchor, pContent );
			}
		}
		else if( ch == 't' )
		{
			if( *pContent++ != 't' || *pContent++ != '=' )
			{
				return false;
			}

			// start time
			anchor = pContent;
			while( *pContent != ' ' ) pContent++;
			if( *pContent == '\0' )
			{
				return false;
			}
			else
			{
				m_starttime.assign( anchor, pContent );
				while( *pContent == ' ' ) pContent++;
				if( *pContent == '\0' )
				{
					return false;
				}
			}

			// end time
			anchor = pContent;
			while( *pContent != '\r' && *pContent != '\n' ) pContent++;
			if( *pContent == '\0' )
			{
				return false;
			}
			else
			{
				m_endtime.assign( anchor, pContent );
			}
		}
		else if( ch == 'm' )
		{
			if( *pContent++ != 'm' || *pContent++ != '=' )
			{
				return false;
			}

			// media type
			anchor = pContent;
			while( *pContent != ' ' ) pContent++;
			if( *pContent == '\0' )
			{
				return false;
			}
			else
			{
				m_mediatype.assign(anchor, pContent);
				while( *pContent == ' ' ) pContent++;
				if( *pContent == '\0' )
				{
					return false;
				}
			}

			// port
			anchor = pContent;
			while( *pContent != ' ') pContent++;
			if( *pContent == '\0' )
			{
				return false;
			}
			else
			{
				m_mediaport.assign( anchor, pContent );
				while( *pContent == ' ' ) pContent++;
				if( *pContent == '\0' )
				{
					return false;
				}
			}

			// transport
			anchor = pContent;
			while( *pContent != ' ' ) pContent++;
			if( *pContent == '\0' )
			{
				return false;
			}
			else
			{
				m_transport.assign( anchor, pContent );
				while( *pContent == ' ' ) pContent++;
				if( *pContent == '\0' )
				{
					return false;
				}
			}

			// rtpmap
			while( *pContent != '\r' && *pContent != '\n' )
			{
				anchor = pContent;
				while( *pContent != ' ' && *pContent != '\r' && *pContent != '\n' ) pContent++;
				if( *pContent == '\0' )
				{
					return false;
				}
				else
				{
					std::string rtpmap( anchor, pContent );
					m_rtpmap.insert( std::make_pair( rtpmap, "" ) );
					while( *pContent == ' ' ) pContent++;
					if( *pContent == '\0' )
					{
						return false;
					}
				}
			}
		}
		else if( ch == 'b' )
        {
            if( *pContent++ != 'b' || *pContent++ != '=' )
            {
                return false;
            }

            while( *pContent != '\0' && *pContent != '\r' && *pContent != '\n' ) pContent++;
            if( *pContent == '\0' )
            {
                return false;
            }
		}
		else if( ch == 'a' )
		{
			if( *pContent++ != 'a' || *pContent++ != '=' )
			{
				return false;
			}

			std::string type;
			anchor = pContent;
			while( *pContent != ':' && *pContent != '\r' && *pContent != '\n' ) pContent++;
			if( *pContent == '\0' )
			{
				return false;
			}
			else
			{
				type.assign( anchor, pContent );
				while( *pContent == ' ' || *pContent == ':' ) pContent++;
				if( *pContent == '\0' )
				{
					return false;
				}
			}

			if( CGBMessage::CompareNoCase( type, "rtpmap" ) )
			{
				anchor = pContent;
				while( *pContent != ' ' ) pContent++;
				if( *pContent == '\0' )
				{
					return false;
				}

				std::string rtpmap( anchor, pContent );
				while( *pContent == ' ' ) pContent++;
				if( *pContent == '\0' )
				{
					return false;
				}

				std::map< std::string, std::string >::iterator it = m_rtpmap.find( rtpmap );
				if( it == m_rtpmap.end() )
				{
					return false;
				}

				anchor = pContent;
				while( *pContent != '\0' && *pContent != '\r' && *pContent != '\n' )
				{
					pContent++;
				}

				if( anchor != pContent )
				{
					it->second.assign( anchor, pContent );
				}
			}
			else
			{
				anchor = pContent;
				while( *pContent != '\0' && *pContent != '\r' && *pContent != '\n' )
				{
					pContent++;
				}

				if( *pContent != '\0' )
				{
					// send or recv
				}
			}
		}
		else if( ch == 'y' )
		{
			if( *pContent++ != 'y' || *pContent++ != '=' )
			{
				return false;
			}

			anchor = pContent;
			while( *pContent != '\0' && *pContent != '\r' && *pContent != '\n' )
			{
				pContent++;
			}

			if( anchor != pContent )
			{
				m_ssrc.assign( anchor, pContent );
			}
		}
		else if( ch == 'f' )
		{
			if( *pContent++ != 'f' || *pContent++ != '=' )
			{
				return false;
			}

			anchor = pContent;
			while( *pContent != '\0' && *pContent != '\r' && *pContent != '\n' )
			{
				pContent++;
			}

			if( anchor != pContent )
			{
				m_audioproterty.assign( anchor, pContent );
			}
		}
		else
		{
            while( *pContent != '\0' && *pContent != '\r' && *pContent != '\n' ) pContent++;
            if( *pContent == '\0' )
            {
                return false;
            }
		}

		// next line
		while( *pContent == '\r' || *pContent == '\n' ) pContent++;
		if( *pContent == '\0' )
		{
			break;
		}
	}

	return true;
}

void CBroadcastStreamMsg::Process( CSipInterface *pInterface )
{
	if( pInterface != NULL )
	{
		pInterface->OnOpenBroadcastStream( *this );
	}
}

//////////////////////////////////////////////////////////////////////////
bool CInviteResponseMsg::Send( resip::DialogUsageManager &mDum, bool tcp )
{
	// 回应关键的请求消息句柄不能为空
	resip::ServerInviteSession *handle = dynamic_cast< resip::ServerInviteSession* >( m_handle );
	if( handle == NULL )
	{
		ERROR_LOG( "请求消息handle为NULL, 发送失败!" );
		return false;
	}

	// 发送消息
	__DUM_TRY
	if( m_statusCode == 200 )
	{
		// 生成消息体内容
		std::string body;
		if( !Encode( body ) )
		{
			ERROR_LOG( "消息编码错误，发送失败!" );
			return false;
		}
		resip::HeaderFieldValue hfv( body.c_str(), (unsigned int)body.size() );
		resip::Mime mime_type( "Application", "sdp" );
		resip::PlainContents sdp( hfv, mime_type );
		handle->provideAnswer( sdp );
		handle->acceptCommand();
	}
	else
	{
		handle->rejectCommand( m_statusCode );
	}

	return true;
	__DUM_CATCH
}

//////////////////////////////////////////////////////////////////////////
bool CRealStreamResponseMsg::Encode( std::string &message )
{
	std::ostringstream sdp;
	sdp << "v=0\r\n";
	sdp << "o="<< m_recvid << " 0 0 IN IP4 "<< m_mediaip << "\r\n";
	sdp << "s=Play\r\n";
	sdp << "c=IN IP4 " << m_connect << "\r\n";
	sdp << "t=0 0\r\n";
	sdp <<"m=video "<< m_mediaport <<" RTP/AVP";
	std::map< std::string, std::string >::const_iterator it;
	for (it = m_rtpmap.begin(); it != m_rtpmap.end(); it++)
	{
		std::string key = it->first;
		std::string text = it->second;
		if ( !text.empty() )
		{
			sdp <<" "<<key;
		}
	}
	sdp <<"\r\n";
	sdp << "a=sendonly\r\n";
	for (it = m_rtpmap.begin(); it != m_rtpmap.end(); it++)
	{
		std::string key = it->first;
		std::string text = it->second;
		if ( !text.empty() )
		{
			sdp <<"a=rtpmap:"<<key<<" "<<text<<"\r\n";
		}
	}
	sdp << "y=" << m_ssrc << "\r\n";

	message = sdp.str();
	return true;
}

bool CRealStreamResponseMsg::Decode( const char *body, std::string &reason )
{
	// message without sdp
	if( body == NULL )
	{
#ifdef _DEBUG
		WARN_LOG( "实时流回应消息体不带SDP!" );
#endif
        //reason = "实时流回应消息体不带SDP!";
		return true;
	}

	const char* pContent = body;
	const char* anchor = 0;

	// 解析SDP字段
	while( pContent != '\0' )
	{
		char ch = *pContent;
		if( ch == 'v' )
		{
			if( *pContent++ != 'v' || *pContent++ != '=' )
            {
                reason = "没找到v字段!";
				return false;
			}

			while( *pContent != '\0' && *pContent != '\r' && *pContent != '\n' ) pContent++;
			if( *pContent == '\0' )
            {
                reason = "字段v结尾出现结束符!";
				return false;
			}
		}
		else if( ch == 'o' )
		{
			if( *pContent++ != 'o' || *pContent++ != '=' )
            {
                reason = "没找到o字段!";
				return false;
			}

			// id
			anchor = pContent;
			while( *pContent != ' ' ) pContent++;
			if( *pContent == '\0' )
            {
                reason = "o字段id前出现结束符!";
				return false;
			}
			else
			{
				m_recvid.assign( anchor, pContent );
				while( *pContent == ' ' ) pContent++;
				if( *pContent == '\0' )
                {
                    reason = "o字段id后出现结束符!";
					return false;
				}
			}

			// session
			anchor = pContent;
			while( *pContent != ' ' ) pContent++;
			if( *pContent == '\0' )
            {
                reason = "o字段session前出现结束符!";
				return false;
			}
			else
			{
				m_sessionid.assign( anchor, pContent );
				while( *pContent == ' ' ) pContent++;
				if( *pContent == '\0' )
                {
                    reason = "o字段session后出现结束符!";
					return false;
				}
			}

			// version
			anchor = pContent;
			while( *pContent != ' ' ) pContent++;
			if( *pContent == '\0' )
            {
                reason = "o字段version前出现结束符!";
				return false;
			}
			else
			{
				m_version.assign( anchor, pContent );
				while( *pContent == ' ' ) pContent++;
				if( *pContent == '\0' )
                {
                    reason = "o字段version后出现结束符!";
					return false;
				}
			}

			// in
			anchor = pContent;
			while( *pContent != ' ' ) pContent++;
			if( *pContent == '\0' )
            {
                reason = "o字段in前出现结束符!";
				return false;
			}
			else
			{
				while( *pContent == ' ' ) pContent++;
				if( *pContent == '\0' )
                {
                    reason = "o字段in后出现结束符!";
					return false;
				}
			}

			// ip version
			anchor = pContent;
			while( *pContent != ' ' ) pContent++;
			if( *pContent == '\0' )
            {
                reason = "o字段ip version前出现结束符!";
				return false;
			}
			else
			{
				while( *pContent == ' ' ) pContent++;
				if( *pContent == '\0' )
                {
                    reason = "o字段ip version后出现结束符!";
					return false;
				}
			}

			// media ip
			anchor = pContent;
			while( *pContent != '\r' && *pContent != '\n' ) pContent++;
			if( *pContent == '\0' )
            {
                reason = "o字段ip前出现结束符!";
				return false;
			}
			else
			{
				m_mediaip.assign( anchor, pContent );
			}
		}
		else if( ch == 's' )
		{
			if( *pContent++ != 's' || *pContent++ != '=' )
            {
                reason = "没找到s字段!";
				return false;
			}

            anchor = pContent;
            while( *pContent != '\r' && *pContent != '\n' ) pContent++;
            if( anchor != pContent )
            {
                m_sessionname.assign( anchor, pContent );
            }

            if( *pContent == '\0' )
            {
                reason = "字段s结尾出现结束符!";
                return false;
            }
		}
		else if( ch == 'c' )
		{
			if( *pContent++ != 'c' || *pContent++ != '=' )
            {
                reason = "没找到c字段!";
				return false;
			}

			// in
			while( *pContent != ' ' ) pContent++;
			if( pContent == '\0' )
            {
                reason = "c字段in前出现结束符!";
				return false;
			}
			else
			{
				while( *pContent == ' ' ) pContent++;
				if( *pContent == '\0' )
                {
                    reason = "c字段in后出现结束符!";
					return false;
				}
			}

			// ip version
			while( *pContent != ' ' ) pContent++;
			if( pContent == '\0' )
            {
                reason = "c字段ip version前出现结束符!";
				return false;
			}
			else
			{
				while( *pContent == ' ' ) pContent++;
				if( *pContent == '\0' )
                {
                    reason = "c字段ip version后出现结束符!";
					return false;
				}
			}

			// connect ip
			anchor = pContent;
			while( *pContent != '\r' && *pContent != '\n' ) pContent++;
			if( *pContent == '\0' )
            {
                reason = "c字段connect ip前出现结束符!";
				return false;
			}
			else
			{
				m_connect.assign( anchor, pContent );
			}
		}
		else if( ch == 't' )
		{
			if( *pContent++ != 't' || *pContent++ != '=' )
            {
                reason = "没找到t字段!";
				return false;
			}

			// start time
			anchor = pContent;
			while( *pContent != ' ' ) pContent++;
			if( *pContent == '\0' )
            {
                reason = "t字段start time前出现结束符!";
				return false;
			}
			else
			{
				m_starttime.assign( anchor, pContent );
				while( *pContent == ' ' ) pContent++;
				if( *pContent == '\0' )
                {
                    reason = "t字段start time后出现结束符!";
					return false;
				}
			}

			// end time
			anchor = pContent;
			while( *pContent != '\r' && *pContent != '\n' ) pContent++;
			if( *pContent == '\0' )
            {
                reason = "t字段end time前出现结束符!";
				return false;
			}
			else
			{
				m_endtime.assign( anchor, pContent );
			}
		}
		else if( ch == 'm' )
		{
			if( *pContent++ != 'm' || *pContent++ != '=' )
            {
                reason = "没找到m字段!";
				return false;
			}

			// media type
			anchor = pContent;
			while( *pContent != ' ' ) pContent++;
			if( *pContent == '\0' )
            {
                reason = "m字段media type前出现结束符!";
				return false;
			}
			else
			{
				m_mediatype.assign(anchor, pContent);
				while( *pContent == ' ' ) pContent++;
				if( *pContent == '\0' )
                {
                    reason = "m字段media type后出现结束符!";
					return false;
				}
			}

			// port
			anchor = pContent;
			while( *pContent != ' ') pContent++;
			if( *pContent == '\0' )
            {
                reason = "m字段port前出现结束符!";
				return false;
			}
			else
			{
				m_mediaport.assign( anchor, pContent );
				while( *pContent == ' ' ) pContent++;
				if( *pContent == '\0' )
                {
                    reason = "m字段port后出现结束符!";
					return false;
				}
			}

			// transport
			anchor = pContent;
			while( *pContent != ' ' ) pContent++;
			if( *pContent == '\0' )
            {
                reason = "m字段transport前出现结束符!";
				return false;
			}
			else
			{
				m_transport.assign( anchor, pContent );
				while( *pContent == ' ' ) pContent++;
				if( *pContent == '\0' )
                {
                    reason = "m字段transport后出现结束符!";
					return false;
				}
			}

			// rtpmap
			while( *pContent != '\r' && *pContent != '\n' )
			{
				anchor = pContent;
				while( *pContent != ' ' && *pContent != '\r' && *pContent != '\n' ) pContent++;
				if( *pContent == '\0' )
                {
                    reason = "m字段rtpmap前出现结束符!";
					return false;
				}
				else
				{
					std::string rtpmap( anchor, pContent );
					m_rtpmap.insert( std::make_pair( rtpmap, "" ) );
					while( *pContent == ' ' ) pContent++;
					if( *pContent == '\0' )
                    {
                        reason = "m字段rtpmap后出现结束符!";
						return false;
					}
				}
			}
		}
		else if( ch == 'b' )
        {
            if( *pContent++ != 'b' || *pContent++ != '=' )
            {
                reason = "没找到b字段!";
                return false;
            }

            while( *pContent != '\0' && *pContent != '\r' && *pContent != '\n' ) pContent++;
            if( *pContent == '\0' )
            {
                reason = "字段b结尾出现结束符!";
                return false;
            }
		}
		else if( ch == 'a' )
		{
			if( *pContent++ != 'a' || *pContent++ != '=' )
            {
                reason = "没找到a字段!";
				return false;
			}

			std::string type;
			anchor = pContent;
			while( *pContent != ':' && *pContent != '\r' && *pContent != '\n' ) pContent++;
			if( *pContent == '\0' )
            {
                reason = "a字段rtpmap前出现结束符!";
				return false;
			}
			else
			{
				type.assign( anchor, pContent );
				while( *pContent == ' ' || *pContent == ':' ) pContent++;
				if( *pContent == '\0' )
                {
                    reason = "a字段rtpmap后出现结束符!";
					return false;
				}
			}

			if( CGBMessage::CompareNoCase( type, "rtpmap" ) )
			{
				anchor = pContent;
				while( *pContent != ' ' ) pContent++;
				if( *pContent == '\0' )
                {
                    reason = "a字段payload type前出现结束符!";
					return false;
				}

				std::string rtpmap( anchor, pContent );
				while( *pContent == ' ' ) pContent++;
				if( *pContent == '\0' )
                {
                    reason = "a字段payload type后出现结束符!";
					return false;
				}

				std::map< std::string, std::string >::iterator it = m_rtpmap.find( rtpmap );
				if( it == m_rtpmap.end() )
                {
                    reason = "m字段不存在该payload type!";
					return false;
				}

				anchor = pContent;
				while( *pContent != '\0' && *pContent != '\r' && *pContent != '\n' )
				{
					pContent++;
				}

				if( anchor != pContent )
				{
					it->second.assign( anchor, pContent );
				}
			}
			else
			{
				anchor = pContent;
				while( *pContent != '\0' && *pContent != '\r' && *pContent != '\n' )
				{
					pContent++;
				}

				if( *pContent != '\0' )
				{
					// send or recv
				}
			}
		}
		else if( ch == 'y' )
		{
			if( *pContent++ != 'y' || *pContent++ != '=' )
            {
                reason = "没找到y字段!";
				return false;
			}

			anchor = pContent;
			while( *pContent != '\0' && *pContent != '\r' && *pContent != '\n' )
			{
				pContent++;
			}

			if( anchor != pContent )
			{
				m_ssrc.assign( anchor, pContent );
			}
		}
		else if( ch == 'f' )
		{
			if( *pContent++ != 'f' || *pContent++ != '=' )
            {
                reason = "没找到f字段!";
				return false;
			}

			anchor = pContent;
			while( *pContent != '\0' && *pContent != '\r' && *pContent != '\n' )
			{
				pContent++;
			}

			if( anchor != pContent )
			{
				m_audioproterty.assign( anchor, pContent );
			}
		}
		else
		{
            while( *pContent != '\0' && *pContent != '\r' && *pContent != '\n' ) pContent++;
            if( *pContent == '\0' )
            {
                reason = "不识别的字段并且出现结束符!";
                return false;
            }
		}

		// next line
		while( *pContent == '\r' || *pContent == '\n' ) pContent++;
		if( *pContent == '\0' )
		{
			break;
		}
	}

	return true;
}

void CRealStreamResponseMsg::Process( CSipInterface *pInterface )
{
	if( pInterface != NULL )
	{
		pInterface->OnInviteResponse( *this );
	}
}

//////////////////////////////////////////////////////////////////////////
bool CBroadcastStreamResponseMsg::Encode( std::string &message )
{
	std::ostringstream sdp;
	sdp << "v=0\r\n";
	sdp << "o="<< m_recvid << " 0 0 IN IP4 "<< m_mediaip << "\r\n";
	sdp << "s=Play\r\n";
	sdp << "c=IN IP4 " << m_connect << "\r\n";
	sdp << "t=0 0\r\n";
	sdp <<"m=audio "<< m_mediaport <<" RTP/AVP";
	std::map< std::string, std::string >::const_iterator it;
	for (it = m_rtpmap.begin(); it != m_rtpmap.end(); it++)
	{
		std::string key = it->first;
		std::string text = it->second;
		if ( !text.empty() )
		{
			sdp <<" "<<key;
		}
	}
	sdp <<"\r\n";
	sdp << "a=sendonly\r\n";
	for (it = m_rtpmap.begin(); it != m_rtpmap.end(); it++)
	{
		std::string key = it->first;
		std::string text = it->second;
		if ( !text.empty() )
		{
			sdp <<"a=rtpmap:"<<key<<" "<<text<<"\r\n";
		}
	}
	sdp << "y=" << m_ssrc << "\r\n";

	message = sdp.str();
	return true;
}

bool CBroadcastStreamResponseMsg::Decode( const char *body, std::string &reason )
{
	// message without sdp
	if( body == NULL )
	{
#ifdef _DEBUG
		WARN_LOG( "语音广播回应消息体不带SDP!" );
#endif
        reason = "语音广播回应消息体不带SDP!";
		return true;
	}

	const char* pContent = body;
	const char* anchor = 0;

	// 解析SDP字段
	while( pContent != '\0' )
	{
		char ch = *pContent;
		if( ch == 'v' )
		{
			if( *pContent++ != 'v' || *pContent++ != '=' )
            {
                reason = "没找到v字段!";
				return false;
			}

			while( *pContent != '\0' && *pContent != '\r' && *pContent != '\n' ) pContent++;
			if( *pContent == '\0' )
            {
                reason = "字段v结尾出现结束符!";
				return false;
			}
		}
		else if( ch == 'o' )
		{
			if( *pContent++ != 'o' || *pContent++ != '=' )
            {
                reason = "没找到o字段!";
				return false;
			}

			// id
			anchor = pContent;
			while( *pContent != ' ' ) pContent++;
			if( *pContent == '\0' )
            {
                reason = "o字段id前出现结束符!";
				return false;
			}
			else
			{
				m_recvid.assign( anchor, pContent );
				while( *pContent == ' ' ) pContent++;
				if( *pContent == '\0' )
                {
                    reason = "o字段id后出现结束符!";
					return false;
				}
			}

			// session
			anchor = pContent;
			while( *pContent != ' ' ) pContent++;
			if( *pContent == '\0' )
            {
                reason = "o字段session前出现结束符!";
				return false;
			}
			else
			{
				m_sessionid.assign( anchor, pContent );
				while( *pContent == ' ' ) pContent++;
				if( *pContent == '\0' )
                {
                    reason = "o字段session后出现结束符!";
					return false;
				}
			}

			// version
			anchor = pContent;
			while( *pContent != ' ' ) pContent++;
			if( *pContent == '\0' )
            {
                reason = "o字段version前出现结束符!";
				return false;
			}
			else
			{
				m_version.assign( anchor, pContent );
				while( *pContent == ' ' ) pContent++;
				if( *pContent == '\0' )
                {
                    reason = "o字段version后出现结束符!";
					return false;
				}
			}

			// in
			anchor = pContent;
			while( *pContent != ' ' ) pContent++;
			if( *pContent == '\0' )
            {
                reason = "o字段in前出现结束符!";
				return false;
			}
			else
			{
				while( *pContent == ' ' ) pContent++;
				if( *pContent == '\0' )
                {
                    reason = "o字段in后出现结束符!";
					return false;
				}
			}

			// ip version
			anchor = pContent;
			while( *pContent != ' ' ) pContent++;
			if( *pContent == '\0' )
            {
                reason = "o字段ip version前出现结束符!";
				return false;
			}
			else
			{
				while( *pContent == ' ' ) pContent++;
				if( *pContent == '\0' )
                {
                    reason = "o字段ip version后出现结束符!";
					return false;
				}
			}

			// media ip
			anchor = pContent;
			while( *pContent != '\r' && *pContent != '\n' ) pContent++;
			if( *pContent == '\0' )
            {
                reason = "o字段ip前出现结束符!";
				return false;
			}
			else
			{
				m_mediaip.assign( anchor, pContent );
			}
		}
		else if( ch == 's' )
        {
            if( *pContent++ != 's' || *pContent++ != '=' )
            {
                reason = "没找到s字段!";
                return false;
            }

            anchor = pContent;
            while( *pContent != '\r' && *pContent != '\n' ) pContent++;
            if( anchor != pContent )
            {
                m_sessionname.assign( anchor, pContent );
            }

            if( *pContent == '\0' )
            {
                reason = "字段s结尾出现结束符!";
                return false;
            }
		}
		else if( ch == 'c' )
		{
			if( *pContent++ != 'c' || *pContent++ != '=' )
            {
                reason = "没找到c字段!";
				return false;
			}

			// in
			while( *pContent != ' ' ) pContent++;
			if( pContent == '\0' )
            {
                reason = "c字段in前出现结束符!";
				return false;
			}
			else
			{
				while( *pContent == ' ' ) pContent++;
				if( *pContent == '\0' )
                {
                    reason = "c字段in后出现结束符!";
					return false;
				}
			}

			// ip version
			while( *pContent != ' ' ) pContent++;
			if( pContent == '\0' )
            {
                reason = "c字段ip version前出现结束符!";
				return false;
			}
			else
			{
				while( *pContent == ' ' ) pContent++;
				if( *pContent == '\0' )
                {
                    reason = "c字段ip version后出现结束符!";
					return false;
				}
			}

			// connect ip
			anchor = pContent;
			while( *pContent != '\r' && *pContent != '\n' ) pContent++;
			if( *pContent == '\0' )
            {
                reason = "c字段connect ip前出现结束符!";
				return false;
			}
			else
			{
				m_connect.assign( anchor, pContent );
			}
		}
		else if( ch == 't' )
		{
			if( *pContent++ != 't' || *pContent++ != '=' )
            {
                reason = "没找到t字段!";
				return false;
			}

			// start time
			anchor = pContent;
			while( *pContent != ' ' ) pContent++;
			if( *pContent == '\0' )
            {
                reason = "t字段start time前出现结束符!";
				return false;
			}
			else
			{
				m_starttime.assign( anchor, pContent );
				while( *pContent == ' ' ) pContent++;
				if( *pContent == '\0' )
                {
                    reason = "t字段start time后出现结束符!";
					return false;
				}
			}

			// end time
			anchor = pContent;
			while( *pContent != '\r' && *pContent != '\n' ) pContent++;
			if( *pContent == '\0' )
            {
                reason = "t字段end time前出现结束符!";
				return false;
			}
			else
			{
				m_endtime.assign( anchor, pContent );
			}
		}
		else if( ch == 'm' )
		{
			if( *pContent++ != 'm' || *pContent++ != '=' )
            {
                reason = "没找到m字段!";
				return false;
			}

			// media type
			anchor = pContent;
			while( *pContent != ' ' ) pContent++;
			if( *pContent == '\0' )
            {
                reason = "m字段media type前出现结束符!";
				return false;
			}
			else
			{
				m_mediatype.assign(anchor, pContent);
				while( *pContent == ' ' ) pContent++;
				if( *pContent == '\0' )
                {
                    reason = "m字段media type后出现结束符!";
					return false;
				}
			}

			// port
			anchor = pContent;
			while( *pContent != ' ') pContent++;
			if( *pContent == '\0' )
            {
                reason = "m字段port前出现结束符!";
				return false;
			}
			else
			{
				m_mediaport.assign( anchor, pContent );
				while( *pContent == ' ' ) pContent++;
				if( *pContent == '\0' )
                {
                    reason = "m字段port后出现结束符!";
					return false;
				}
			}

			// transport
			anchor = pContent;
			while( *pContent != ' ' ) pContent++;
			if( *pContent == '\0' )
            {
                reason = "m字段transport前出现结束符!";
				return false;
			}
			else
			{
				m_transport.assign( anchor, pContent );
				while( *pContent == ' ' ) pContent++;
				if( *pContent == '\0' )
                {
                    reason = "m字段transport后出现结束符!";
					return false;
				}
			}

			// rtpmap
			while( *pContent != '\r' && *pContent != '\n' )
			{
				anchor = pContent;
				while( *pContent != ' ' && *pContent != '\r' && *pContent != '\n' ) pContent++;
				if( *pContent == '\0' )
                {
                    reason = "m字段rtpmap前出现结束符!";
					return false;
				}
				else
				{
					std::string rtpmap( anchor, pContent );
					m_rtpmap.insert( std::make_pair( rtpmap, "" ) );
					while( *pContent == ' ' ) pContent++;
					if( *pContent == '\0' )
                    {
                        reason = "m字段rtpmap后出现结束符!";
						return false;
					}
				}
			}
		}
		else if( ch == 'b' )
        {
            if( *pContent++ != 'b' || *pContent++ != '=' )
            {
                reason = "没找到b字段!";
                return false;
            }

            while( *pContent != '\0' && *pContent != '\r' && *pContent != '\n' ) pContent++;
            if( *pContent == '\0' )
            {
                reason = "字段b结尾出现结束符!";
                return false;
            }
		}
		else if( ch == 'a' )
		{
			if( *pContent++ != 'a' || *pContent++ != '=' )
            {
                reason = "没找到a字段!";
				return false;
			}

			std::string type;
			anchor = pContent;
			while( *pContent != ':' && *pContent != '\r' && *pContent != '\n' ) pContent++;
			if( *pContent == '\0' )
            {
                reason = "a字段rtpmap前出现结束符!";
				return false;
			}
			else
			{
				type.assign( anchor, pContent );
				while( *pContent == ' ' || *pContent == ':' ) pContent++;
				if( *pContent == '\0' )
                {
                    reason = "a字段rtpmap后出现结束符!";
					return false;
				}
			}

			if( CGBMessage::CompareNoCase( type, "rtpmap" ) )
			{
				anchor = pContent;
				while( *pContent != ' ' ) pContent++;
				if( *pContent == '\0' )
                {
                    reason = "a字段payload type前出现结束符!";
					return false;
				}

				std::string rtpmap( anchor, pContent );
				while( *pContent == ' ' ) pContent++;
				if( *pContent == '\0' )
                {
                    reason = "a字段payload type后出现结束符!";
					return false;
				}

				std::map< std::string, std::string >::iterator it = m_rtpmap.find( rtpmap );
				if( it == m_rtpmap.end() )
                {
                    reason = "m字段不存在该payload type!";
					return false;
				}

				anchor = pContent;
				while( *pContent != '\0' && *pContent != '\r' && *pContent != '\n' )
				{
					pContent++;
				}

				if( anchor != pContent )
				{
					it->second.assign( anchor, pContent );
				}
			}
			else
			{
				anchor = pContent;
				while( *pContent != '\0' && *pContent != '\r' && *pContent != '\n' )
				{
					pContent++;
				}

				if( *pContent != '\0' )
				{
					// send or recv
				}
			}
		}
		else if( ch == 'y' )
		{
			if( *pContent++ != 'y' || *pContent++ != '=' )
            {
                reason = "没找到y字段!";
				return false;
			}

			anchor = pContent;
			while( *pContent != '\0' && *pContent != '\r' && *pContent != '\n' )
			{
				pContent++;
			}

			if( anchor != pContent )
			{
				m_ssrc.assign( anchor, pContent );
			}
		}
		else if( ch == 'f' )
		{
			if( *pContent++ != 'f' || *pContent++ != '=' )
            {
                reason = "没找到f字段!";
				return false;
			}

			anchor = pContent;
			while( *pContent != '\0' && *pContent != '\r' && *pContent != '\n' )
			{
				pContent++;
			}

			if( anchor != pContent )
			{
				m_audioproterty.assign( anchor, pContent );
			}
		}
		else
		{
            while( *pContent != '\0' && *pContent != '\r' && *pContent != '\n' ) pContent++;
            if( *pContent == '\0' )
            {
                reason = "不识别的字段并且出现结束符!";
                return false;
            }
		}

		// next line
		while( *pContent == '\r' || *pContent == '\n' ) pContent++;
		if( *pContent == '\0' )
		{
			break;
		}
	}

	return true;
}

void CBroadcastStreamResponseMsg::Process( CSipInterface *pInterface )
{
	if( pInterface != NULL )
	{
		pInterface->OnInviteResponse( *this );
	}
}



//////////////////////////////////////////////////////////////////////////
bool CVodStreamResponseMsg::Encode( std::string &message )
{
	std::ostringstream sdp;
	sdp << "v=0\r\n";
	sdp << "o="<< m_recvid << " 0 0 IN IP4 "<< m_mediaip << "\r\n";
	sdp << "s=Playback\r\n";
	sdp << "c=IN IP4 " << m_connect << "\r\n";
	sdp << "t=0 0\r\n";
	sdp <<"m=video "<< m_mediaport <<" RTP/AVP";
	std::map< std::string, std::string >::const_iterator it;
	for (it = m_rtpmap.begin(); it != m_rtpmap.end(); it++)
	{
		std::string key = it->first;
		std::string text = it->second;
		if ( !text.empty() )
		{
			sdp <<" "<<key;
		}
	}
	sdp <<"\r\n";
	sdp << "a=sendonly\r\n";
	for (it = m_rtpmap.begin(); it != m_rtpmap.end(); it++)
	{
		std::string key = it->first;
		std::string text = it->second;
		if ( !text.empty() )
		{
			sdp <<"a=rtpmap:"<<key<<" "<<text<<"\r\n";
		}
	}
	sdp << "y=" << m_ssrc << "\r\n";
	message = sdp.str();
	return true;
}

bool CVodStreamResponseMsg::Decode( const char *body, std::string &reason )
{
	// message without sdp
	if( body == NULL )
	{
#ifdef _DEBUG
		WARN_LOG( "录像回放回应消息体不带SDP!" );
#endif
        //reason = "录像回放回应消息体不带SDP!";
		return true;
	}

	const char* pContent = body;
	const char* anchor = 0;

	// 解析SDP字段
	while( pContent != '\0' )
	{
		char ch = *pContent;
		if( ch == 'v' )
		{
			if( *pContent++ != 'v' || *pContent++ != '=' )
            {
                reason = "没找到v字段!";
				return false;
			}

			while( *pContent != '\0' && *pContent != '\r' && *pContent != '\n' ) pContent++;
			if( *pContent == '\0' )
            {
                reason = "字段v结尾出现结束符!";
				return false;
			}
		}
		else if( ch == 'o' )
		{
			if( *pContent++ != 'o' || *pContent++ != '=' )
            {
                reason = "没找到o字段!";
				return false;
			}

			// id
			anchor = pContent;
			while( *pContent != ' ' ) pContent++;
			if( *pContent == '\0' )
            {
                reason = "o字段id前出现结束符!";
				return false;
			}
			else
			{
				m_recvid.assign( anchor, pContent );
				while( *pContent == ' ' ) pContent++;
				if( *pContent == '\0' )
                {
                    reason = "o字段id后出现结束符!";
					return false;
				}
			}

			// session
			anchor = pContent;
			while( *pContent != ' ' ) pContent++;
			if( *pContent == '\0' )
            {
                reason = "o字段session前出现结束符!";
				return false;
			}
			else
			{
				m_sessionid.assign( anchor, pContent );
				while( *pContent == ' ' ) pContent++;
				if( *pContent == '\0' )
                {
                    reason = "o字段session后出现结束符!";
					return false;
				}
			}

			// version
			anchor = pContent;
			while( *pContent != ' ' ) pContent++;
			if( *pContent == '\0' )
            {
                reason = "o字段version前出现结束符!";
				return false;
			}
			else
			{
				m_version.assign( anchor, pContent );
				while( *pContent == ' ' ) pContent++;
				if( *pContent == '\0' )
                {
                    reason = "o字段version后出现结束符!";
					return false;
				}
			}

			// in
			anchor = pContent;
			while( *pContent != ' ' ) pContent++;
			if( *pContent == '\0' )
            {
                reason = "o字段in前出现结束符!";
				return false;
			}
			else
			{
				while( *pContent == ' ' ) pContent++;
				if( *pContent == '\0' )
                {
                    reason = "o字段in后出现结束符!";
					return false;
				}
			}

			// ip version
			anchor = pContent;
			while( *pContent != ' ' ) pContent++;
			if( *pContent == '\0' )
            {
                reason = "o字段ip version前出现结束符!";
				return false;
			}
			else
			{
				while( *pContent == ' ' ) pContent++;
				if( *pContent == '\0' )
                {
                    reason = "o字段ip version后出现结束符!";
					return false;
				}
			}

			// media ip
			anchor = pContent;
			while( *pContent != '\r' && *pContent != '\n' ) pContent++;
			if( *pContent == '\0' )
            {
                reason = "o字段ip前出现结束符!";
				return false;
			}
			else
			{
				m_mediaip.assign( anchor, pContent );
			}
		}
		else if( ch == 's' )
        {
            if( *pContent++ != 's' || *pContent++ != '=' )
            {
                reason = "没找到s字段!";
                return false;
            }

            anchor = pContent;
            while( *pContent != '\r' && *pContent != '\n' ) pContent++;
            if( anchor != pContent )
            {
                m_sessionname.assign( anchor, pContent );
            }

            if( *pContent == '\0' )
            {
                reason = "字段s结尾出现结束符!";
                return false;
            }
		}
		else if( ch == 'c' )
		{
			if( *pContent++ != 'c' || *pContent++ != '=' )
            {
                reason = "没找到c字段!";
				return false;
			}

			// in
			while( *pContent != ' ' ) pContent++;
			if( pContent == '\0' )
            {
                reason = "c字段in前出现结束符!";
				return false;
			}
			else
			{
				while( *pContent == ' ' ) pContent++;
				if( *pContent == '\0' )
                {
                    reason = "c字段in后出现结束符!";
					return false;
				}
			}

			// ip version
			while( *pContent != ' ' ) pContent++;
			if( pContent == '\0' )
            {
                reason = "c字段ip version前出现结束符!";
				return false;
			}
			else
			{
				while( *pContent == ' ' ) pContent++;
				if( *pContent == '\0' )
                {
                    reason = "c字段ip version后出现结束符!";
					return false;
				}
			}

			// connect ip
			anchor = pContent;
			while( *pContent != '\r' && *pContent != '\n' ) pContent++;
			if( *pContent == '\0' )
            {
                reason = "c字段connect ip前出现结束符!";
				return false;
			}
			else
			{
				m_connect.assign( anchor, pContent );
			}
		}
		else if( ch == 't' )
		{
			if( *pContent++ != 't' || *pContent++ != '=' )
            {
                reason = "没找到t字段!";
				return false;
			}

			// start time
			anchor = pContent;
			while( *pContent != ' ' ) pContent++;
			if( *pContent == '\0' )
            {
                reason = "t字段start time前出现结束符!";
				return false;
			}
			else
			{
				m_starttime.assign( anchor, pContent );
				while( *pContent == ' ' ) pContent++;
				if( *pContent == '\0' )
                {
                    reason = "t字段start time后出现结束符!";
					return false;
				}
			}

			// end time
			anchor = pContent;
			while( *pContent != '\r' && *pContent != '\n' ) pContent++;
			if( *pContent == '\0' )
            {
                reason = "t字段end time前出现结束符!";
				return false;
			}
			else
			{
				m_endtime.assign( anchor, pContent );
			}
		}
		else if( ch == 'm' )
		{
			if( *pContent++ != 'm' || *pContent++ != '=' )
            {
                reason = "没找到m字段!";
				return false;
			}

			// media type
			anchor = pContent;
			while( *pContent != ' ' ) pContent++;
			if( *pContent == '\0' )
            {
                reason = "m字段media type前出现结束符!";
				return false;
			}
			else
			{
				m_mediatype.assign(anchor, pContent);
				while( *pContent == ' ' ) pContent++;
				if( *pContent == '\0' )
                {
                    reason = "m字段media type后出现结束符!";
					return false;
				}
			}

			// port
			anchor = pContent;
			while( *pContent != ' ') pContent++;
			if( *pContent == '\0' )
            {
                reason = "m字段port前出现结束符!";
				return false;
			}
			else
			{
				m_mediaport.assign( anchor, pContent );
				while( *pContent == ' ' ) pContent++;
				if( *pContent == '\0' )
                {
                    reason = "m字段port后出现结束符!";
					return false;
				}
			}

			// transport
			anchor = pContent;
			while( *pContent != ' ' ) pContent++;
			if( *pContent == '\0' )
            {
                reason = "m字段transport前出现结束符!";
				return false;
			}
			else
			{
				m_transport.assign( anchor, pContent );
				while( *pContent == ' ' ) pContent++;
				if( *pContent == '\0' )
                {
                    reason = "m字段transport后出现结束符!";
					return false;
				}
			}

			// rtpmap
			while( *pContent != '\r' && *pContent != '\n' )
			{
				anchor = pContent;
				while( *pContent != ' ' && *pContent != '\r' && *pContent != '\n' ) pContent++;
				if( *pContent == '\0' )
                {
                    reason = "m字段rtpmap前出现结束符!";
					return false;
				}
				else
				{
					std::string rtpmap( anchor, pContent );
					m_rtpmap.insert( std::make_pair( rtpmap, "" ) );
					while( *pContent == ' ' ) pContent++;
					if( *pContent == '\0' )
                    {
                        reason = "m字段rtpmap后出现结束符!";
						return false;
					}
				}
			}
		}
		else if( ch == 'b' )
        {
            if( *pContent++ != 'b' || *pContent++ != '=' )
            {
                reason = "没找到b字段!";
                return false;
            }

            while( *pContent != '\0' && *pContent != '\r' && *pContent != '\n' ) pContent++;
            if( *pContent == '\0' )
            {
                reason = "字段b结尾出现结束符!";
                return false;
            }
		}
		else if( ch == 'a' )
		{
			if( *pContent++ != 'a' || *pContent++ != '=' )
            {
                reason = "没找到a字段!";
				return false;
			}

			std::string type;
			anchor = pContent;
			while( *pContent != ':' && *pContent != '\r' && *pContent != '\n' ) pContent++;
			if( *pContent == '\0' )
            {
                reason = "a字段rtpmap前出现结束符!";
				return false;
			}
			else
			{
				type.assign( anchor, pContent );
				while( *pContent == ' ' || *pContent == ':' ) pContent++;
				if( *pContent == '\0' )
                {
                    reason = "a字段rtpmap后出现结束符!";
					return false;
				}
			}

			if( CGBMessage::CompareNoCase( type, "rtpmap" ) )
			{
				anchor = pContent;
				while( *pContent != ' ' ) pContent++;
				if( *pContent == '\0' )
                {
                    reason = "a字段payload type前出现结束符!";
					return false;
				}

				std::string rtpmap( anchor, pContent );
				while( *pContent == ' ' ) pContent++;
				if( *pContent == '\0' )
                {
                    reason = "a字段payload type后出现结束符!";
					return false;
				}

				std::map< std::string, std::string >::iterator it = m_rtpmap.find( rtpmap );
				if( it == m_rtpmap.end() )
                {
                    reason = "m字段不存在该payload type!";
					return false;
				}

				anchor = pContent;
				while( *pContent != '\0' && *pContent != '\r' && *pContent != '\n' )
				{
					pContent++;
				}

				if( anchor != pContent )
				{
					it->second.assign( anchor, pContent );
				}
			}
			else
			{
				anchor = pContent;
				while( *pContent != '\0' && *pContent != '\r' && *pContent != '\n' )
				{
					pContent++;
				}

				if( *pContent != '\0' )
				{
					// send or recv
				}
			}
		}
		else if( ch == 'y' )
		{
			if( *pContent++ != 'y' || *pContent++ != '=' )
            {
                reason = "没找到y字段!";
				return false;
			}

			anchor = pContent;
			while( *pContent != '\0' && *pContent != '\r' && *pContent != '\n' )
			{
				pContent++;
			}

			if( anchor != pContent )
			{
				m_ssrc.assign( anchor, pContent );
			}
		}
		else if( ch == 'f' )
		{
			if( *pContent++ != 'f' || *pContent++ != '=' )
            {
                reason = "没找到f字段!";
				return false;
			}

			anchor = pContent;
			while( *pContent != '\0' && *pContent != '\r' && *pContent != '\n' )
			{
				pContent++;
			}

			if( anchor != pContent )
			{
				m_audioproterty.assign( anchor, pContent );
			}
		}
		else
		{
            while( *pContent != '\0' && *pContent != '\r' && *pContent != '\n' ) pContent++;
            if( *pContent == '\0' )
            {
                reason = "不识别的字段并且出现结束符!";
                return false;
            }
		}

		// next line
		while( *pContent == '\r' || *pContent == '\n' ) pContent++;
		if( *pContent == '\0' )
		{
			break;
		}
	}

	return true;
}

void CVodStreamResponseMsg::Process( CSipInterface *pInterface )
{
	if( pInterface != NULL )
	{
		pInterface->OnInviteResponse( *this );
	}
}

//////////////////////////////////////////////////////////////////////////
bool CVodDownloadResponseMsg::Encode( std::string &message )
{
	std::ostringstream sdp;
	sdp << "v=0\r\n";
	sdp << "o="<< m_recvid << " 0 0 IN IP4 "<< m_mediaip << "\r\n";
	sdp << "s=Download\r\n";
	sdp << "c=IN IP4 " << m_connect << "\r\n";
	sdp << "t=0 0\r\n";
	sdp <<"m=video "<< m_mediaport <<" RTP/AVP";
	std::map< std::string, std::string >::const_iterator it;
	for (it = m_rtpmap.begin(); it != m_rtpmap.end(); it++)
	{
		std::string key = it->first;
		std::string text = it->second;
		if ( !text.empty() )
		{
			sdp <<" "<<key;
		}
	}
	sdp <<"\r\n";
	sdp << "a=sendonly\r\n";
	for (it = m_rtpmap.begin(); it != m_rtpmap.end(); it++)
	{
		std::string key = it->first;
		std::string text = it->second;
		if ( !text.empty() )
		{
			sdp <<"a=rtpmap:"<<key<<" "<<text<<"\r\n";
		}
	}
	sdp << "a=filesize:" << m_filesize << "\r\n";

	sdp << "y=" << m_ssrc << "\r\n";

	message = sdp.str();
	return true;
}

bool CVodDownloadResponseMsg::Decode( const char *body, std::string &reason )
{
	// message without sdp
	if( body == NULL )
	{
#ifdef _DEBUG
		WARN_LOG( "录像下载回应消息体不带SDP!" );
#endif
        reason = "录像下载回应消息体不带SDP!";
		return true;
	}

	const char* pContent = body;
	const char* anchor = 0;

	// 解析SDP字段
	while( pContent != '\0' )
	{
		char ch = *pContent;
		if( ch == 'v' )
		{
			if( *pContent++ != 'v' || *pContent++ != '=' )
            {
                reason = "没找到v字段!";
				return false;
			}

			while( *pContent != '\0' && *pContent != '\r' && *pContent != '\n' ) pContent++;
			if( *pContent == '\0' )
            {
                reason = "字段v结尾出现结束符!";
				return false;
			}
		}
		else if( ch == 'o' )
		{
			if( *pContent++ != 'o' || *pContent++ != '=' )
            {
                reason = "没找到o字段!";
				return false;
			}

			// id
			anchor = pContent;
			while( *pContent != ' ' ) pContent++;
			if( *pContent == '\0' )
            {
                reason = "o字段id前出现结束符!";
				return false;
			}
			else
			{
				m_recvid.assign( anchor, pContent );
				while( *pContent == ' ' ) pContent++;
				if( *pContent == '\0' )
                {
                    reason = "o字段id后出现结束符!";
					return false;
				}
			}

			// session
			anchor = pContent;
			while( *pContent != ' ' ) pContent++;
			if( *pContent == '\0' )
            {
                reason = "o字段session前出现结束符!";
				return false;
			}
			else
			{
				m_sessionid.assign( anchor, pContent );
				while( *pContent == ' ' ) pContent++;
				if( *pContent == '\0' )
                {
                    reason = "o字段session后出现结束符!";
					return false;
				}
			}

			// version
			anchor = pContent;
			while( *pContent != ' ' ) pContent++;
			if( *pContent == '\0' )
            {
                reason = "o字段version前出现结束符!";
				return false;
			}
			else
			{
				m_version.assign( anchor, pContent );
				while( *pContent == ' ' ) pContent++;
				if( *pContent == '\0' )
                {
                    reason = "o字段version后出现结束符!";
					return false;
				}
			}

			// in
			anchor = pContent;
			while( *pContent != ' ' ) pContent++;
			if( *pContent == '\0' )
            {
                reason = "o字段in前出现结束符!";
				return false;
			}
			else
			{
				while( *pContent == ' ' ) pContent++;
				if( *pContent == '\0' )
                {
                    reason = "o字段in后出现结束符!";
					return false;
				}
			}

			// ip version
			anchor = pContent;
			while( *pContent != ' ' ) pContent++;
			if( *pContent == '\0' )
            {
                reason = "o字段ip version前出现结束符!";
				return false;
			}
			else
			{
				while( *pContent == ' ' ) pContent++;
				if( *pContent == '\0' )
                {
                    reason = "o字段ip version后出现结束符!";
					return false;
				}
			}

			// media ip
			anchor = pContent;
			while( *pContent != '\r' && *pContent != '\n' ) pContent++;
			if( *pContent == '\0' )
            {
                reason = "o字段ip前出现结束符!";
				return false;
			}
			else
			{
				m_mediaip.assign( anchor, pContent );
			}
		}
		else if( ch == 's' )
        {
            if( *pContent++ != 's' || *pContent++ != '=' )
            {
                reason = "没找到s字段!";
                return false;
            }

            anchor = pContent;
            while( *pContent != '\r' && *pContent != '\n' ) pContent++;
            if( anchor != pContent )
            {
                m_sessionname.assign( anchor, pContent );
            }

            if( *pContent == '\0' )
            {
                reason = "字段s结尾出现结束符!";
                return false;
            }
		}
		else if( ch == 'c' )
		{
			if( *pContent++ != 'c' || *pContent++ != '=' )
            {
                reason = "没找到c字段!";
				return false;
			}

			// in
			while( *pContent != ' ' ) pContent++;
			if( pContent == '\0' )
            {
                reason = "c字段in前出现结束符!";
				return false;
			}
			else
			{
				while( *pContent == ' ' ) pContent++;
				if( *pContent == '\0' )
                {
                    reason = "c字段in后出现结束符!";
					return false;
				}
			}

			// ip version
			while( *pContent != ' ' ) pContent++;
			if( pContent == '\0' )
            {
                reason = "c字段ip version前出现结束符!";
				return false;
			}
			else
			{
				while( *pContent == ' ' ) pContent++;
				if( *pContent == '\0' )
                {
                    reason = "c字段ip version后出现结束符!";
					return false;
				}
			}

			// connect ip
			anchor = pContent;
			while( *pContent != '\r' && *pContent != '\n' ) pContent++;
			if( *pContent == '\0' )
            {
                reason = "c字段connect ip前出现结束符!";
				return false;
			}
			else
			{
				m_connect.assign( anchor, pContent );
			}
		}
		else if( ch == 't' )
		{
			if( *pContent++ != 't' || *pContent++ != '=' )
            {
                reason = "没找到t字段!";
				return false;
			}

			// start time
			anchor = pContent;
			while( *pContent != ' ' ) pContent++;
			if( *pContent == '\0' )
            {
                reason = "t字段start time前出现结束符!";
				return false;
			}
			else
			{
				m_starttime.assign( anchor, pContent );
				while( *pContent == ' ' ) pContent++;
				if( *pContent == '\0' )
                {
                    reason = "t字段start time后出现结束符!";
					return false;
				}
			}

			// end time
			anchor = pContent;
			while( *pContent != '\r' && *pContent != '\n' ) pContent++;
			if( *pContent == '\0' )
            {
                reason = "t字段end time前出现结束符!";
				return false;
			}
			else
			{
				m_endtime.assign( anchor, pContent );
			}
		}
		else if( ch == 'm' )
		{
			if( *pContent++ != 'm' || *pContent++ != '=' )
            {
                reason = "没找到m字段!";
				return false;
			}

			// media type
			anchor = pContent;
			while( *pContent != ' ' ) pContent++;
			if( *pContent == '\0' )
            {
                reason = "m字段media type前出现结束符!";
				return false;
			}
			else
			{
				m_mediatype.assign(anchor, pContent);
				while( *pContent == ' ' ) pContent++;
				if( *pContent == '\0' )
                {
                    reason = "m字段media type后出现结束符!";
					return false;
				}
			}

			// port
			anchor = pContent;
			while( *pContent != ' ') pContent++;
			if( *pContent == '\0' )
            {
                reason = "m字段port前出现结束符!";
				return false;
			}
			else
			{
				m_mediaport.assign( anchor, pContent );
				while( *pContent == ' ' ) pContent++;
				if( *pContent == '\0' )
                {
                    reason = "m字段port后出现结束符!";
					return false;
				}
			}

			// transport
			anchor = pContent;
			while( *pContent != ' ' ) pContent++;
			if( *pContent == '\0' )
            {
                reason = "m字段transport前出现结束符!";
				return false;
			}
			else
			{
				m_transport.assign( anchor, pContent );
				while( *pContent == ' ' ) pContent++;
				if( *pContent == '\0' )
                {
                    reason = "m字段transport后出现结束符!";
					return false;
				}
			}

			// rtpmap
			while( *pContent != '\r' && *pContent != '\n' )
			{
				anchor = pContent;
				while( *pContent != ' ' && *pContent != '\r' && *pContent != '\n' ) pContent++;
				if( *pContent == '\0' )
                {
                    reason = "m字段rtpmap前出现结束符!";
					return false;
				}
				else
				{
					std::string rtpmap( anchor, pContent );
					m_rtpmap.insert( std::make_pair( rtpmap, "" ) );
					while( *pContent == ' ' ) pContent++;
					if( *pContent == '\0' )
                    {
                        reason = "m字段rtpmap后出现结束符!";
						return false;
					}
				}
			}
		}
		else if( ch == 'b' )
        {
            if( *pContent++ != 'b' || *pContent++ != '=' )
            {
                reason = "没找到b字段!";
                return false;
            }

            while( *pContent != '\0' && *pContent != '\r' && *pContent != '\n' ) pContent++;
            if( *pContent == '\0' )
            {
                reason = "字段b结尾出现结束符!";
                return false;
            }
		}
		else if( ch == 'a' )
		{
			if( *pContent++ != 'a' || *pContent++ != '=' )
            {
                reason = "没找到a字段!";
				return false;
			}

			std::string type;
			anchor = pContent;
			while( *pContent != ':' && *pContent != '\r' && *pContent != '\n' ) pContent++;
			if( *pContent == '\0' )
            {
                reason = "a字段rtpmap前出现结束符!";
				return false;
			}
			else
			{
				type.assign( anchor, pContent );
				while( *pContent == ' ' || *pContent == ':' ) pContent++;
				if( *pContent == '\0' )
                {
                    reason = "a字段rtpmap后出现结束符!";
					return false;
				}
			}

			if( CGBMessage::CompareNoCase( type, "rtpmap" ) )
			{
				anchor = pContent;
				while( *pContent != ' ' ) pContent++;
				if( *pContent == '\0' )
                {
                    reason = "a字段payload type前出现结束符!";
					return false;
				}

				std::string rtpmap( anchor, pContent );
				while( *pContent == ' ' ) pContent++;
				if( *pContent == '\0' )
                {
                    reason = "a字段payload type后出现结束符!";
					return false;
				}

				std::map< std::string, std::string >::iterator it = m_rtpmap.find( rtpmap );
				if( it == m_rtpmap.end() )
                {
                    reason = "m字段不存在该payload type!";
					return false;
				}

				anchor = pContent;
				while( *pContent != '\0' && *pContent != '\r' && *pContent != '\n' )
				{
					pContent++;
				}

				if( anchor != pContent )
				{
					it->second.assign( anchor, pContent );
				}
            }
            else if( CGBMessage::CompareNoCase( type, "filesize" ) )
            {
                anchor = pContent;
                while( *pContent != '\r' && *pContent != '\n' ) pContent++;
                if( *pContent == '\0' )
                {
                    return false;
                }
                else
                {
                    m_filesize.assign( anchor, pContent );
                }
            }
			else
			{
				anchor = pContent;
				while( *pContent != '\0' && *pContent != '\r' && *pContent != '\n' )
				{
					pContent++;
				}

				if( *pContent != '\0' )
				{
					// send or recv
				}
			}
		}
		else if( ch == 'y' )
		{
			if( *pContent++ != 'y' || *pContent++ != '=' )
            {
                reason = "没找到y字段!";
				return false;
			}

			anchor = pContent;
			while( *pContent != '\0' && *pContent != '\r' && *pContent != '\n' )
			{
				pContent++;
			}

			if( anchor != pContent )
			{
				m_ssrc.assign( anchor, pContent );
			}
		}
		else if( ch == 'f' )
		{
			if( *pContent++ != 'f' || *pContent++ != '=' )
            {
                reason = "没找到f字段!";
				return false;
			}

			anchor = pContent;
			while( *pContent != '\0' && *pContent != '\r' && *pContent != '\n' )
			{
				pContent++;
			}

			if( anchor != pContent )
			{
				m_audioproterty.assign( anchor, pContent );
			}
		}
		else
		{
            while( *pContent != '\0' && *pContent != '\r' && *pContent != '\n' ) pContent++;
            if( *pContent == '\0' )
            {
                reason = "不识别的字段并且出现结束符!";
                return false;
            }
		}

		// next line
		while( *pContent == '\r' || *pContent == '\n' ) pContent++;
		if( *pContent == '\0' )
		{
			break;
		}
	}

	return true;
}

void CVodDownloadResponseMsg::Process( CSipInterface *pInterface )
{
	if( pInterface != NULL )
	{
		pInterface->OnInviteResponse( *this );
	}
}

//////////////////////////////////////////////////////////////////////////
bool CUnknowStreamResponseMsg::Encode( std::string &message )
{
    std::ostringstream sdp;
    sdp << "v=0\r\n";
    sdp << "o="<< m_recvid << " 0 0 IN IP4 "<< m_mediaip << "\r\n";
    sdp << "s="<< m_sessionname <<"\r\n";
    sdp << "c=IN IP4 " << m_connect << "\r\n";
    sdp << "t=0 0\r\n";
    sdp <<"m="<< m_mediatype << " " << m_mediaport <<" RTP/AVP";
    std::map< std::string, std::string >::const_iterator it;
    for (it = m_rtpmap.begin(); it != m_rtpmap.end(); it++)
    {
        std::string key = it->first;
        std::string text = it->second;
        if ( !text.empty() )
        {
            sdp <<" "<<key;
        }
    }
    sdp <<"\r\n";
    sdp << "a=sendonly\r\n";
    for (it = m_rtpmap.begin(); it != m_rtpmap.end(); it++)
    {
        std::string key = it->first;
        std::string text = it->second;
        if ( !text.empty() )
        {
            sdp <<"a=rtpmap:"<<key<<" "<<text<<"\r\n";
        }
    }
    if( !m_filesize.empty() )
    {
        sdp << "a=filesize:" << m_filesize << "\r\n";
    }

    message = sdp.str();
    return true;
}

bool CUnknowStreamResponseMsg::Decode( const char *body, std::string &reason )
{
    // message without sdp
    if( body == NULL )
    {
#ifdef _DEBUG
        WARN_LOG( "录像下载回应消息体不带SDP!" );
#endif
        reason = "实时流回应消息体不带SDP!";
        return true;
    }

    const char* pContent = body;
    const char* anchor = 0;

    // 解析SDP字段
    while( pContent != '\0' )
    {
        char ch = *pContent;
        if( ch == 'v' )
        {
            if( *pContent++ != 'v' || *pContent++ != '=' )
            {
                reason = "没找到v字段!";
                return false;
            }

            while( *pContent != '\0' && *pContent != '\r' && *pContent != '\n' ) pContent++;
            if( *pContent == '\0' )
            {
                reason = "字段v结尾出现结束符!";
                return false;
            }
        }
        else if( ch == 'o' )
        {
            if( *pContent++ != 'o' || *pContent++ != '=' )
            {
                reason = "没找到o字段!";
                return false;
            }

            // id
            anchor = pContent;
            while( *pContent != ' ' ) pContent++;
            if( *pContent == '\0' )
            {
                reason = "o字段id前出现结束符!";
                return false;
            }
            else
            {
                m_recvid.assign( anchor, pContent );
                while( *pContent == ' ' ) pContent++;
                if( *pContent == '\0' )
                {
                    reason = "o字段id后出现结束符!";
                    return false;
                }
            }

            // session
            anchor = pContent;
            while( *pContent != ' ' ) pContent++;
            if( *pContent == '\0' )
            {
                reason = "o字段session前出现结束符!";
                return false;
            }
            else
            {
                m_sessionid.assign( anchor, pContent );
                while( *pContent == ' ' ) pContent++;
                if( *pContent == '\0' )
                {
                    reason = "o字段session后出现结束符!";
                    return false;
                }
            }

            // version
            anchor = pContent;
            while( *pContent != ' ' ) pContent++;
            if( *pContent == '\0' )
            {
                reason = "o字段version前出现结束符!";
                return false;
            }
            else
            {
                m_version.assign( anchor, pContent );
                while( *pContent == ' ' ) pContent++;
                if( *pContent == '\0' )
                {
                    reason = "o字段version后出现结束符!";
                    return false;
                }
            }

            // in
            anchor = pContent;
            while( *pContent != ' ' ) pContent++;
            if( *pContent == '\0' )
            {
                reason = "o字段in前出现结束符!";
                return false;
            }
            else
            {
                while( *pContent == ' ' ) pContent++;
                if( *pContent == '\0' )
                {
                    reason = "o字段in后出现结束符!";
                    return false;
                }
            }

            // ip version
            anchor = pContent;
            while( *pContent != ' ' ) pContent++;
            if( *pContent == '\0' )
            {
                reason = "o字段ip version前出现结束符!";
                return false;
            }
            else
            {
                while( *pContent == ' ' ) pContent++;
                if( *pContent == '\0' )
                {
                    reason = "o字段ip version后出现结束符!";
                    return false;
                }
            }

            // media ip
            anchor = pContent;
            while( *pContent != '\r' && *pContent != '\n' ) pContent++;
            if( *pContent == '\0' )
            {
                reason = "o字段ip前出现结束符!";
                return false;
            }
            else
            {
                m_mediaip.assign( anchor, pContent );
            }
        }
        else if( ch == 's' )
        {
            if( *pContent++ != 's' || *pContent++ != '=' )
            {
                reason = "没找到s字段!";
                return false;
            }

            anchor = pContent;
            while( *pContent != '\r' && *pContent != '\n' ) pContent++;
            if( anchor != pContent )
            {
                m_sessionname.assign( anchor, pContent );
            }

            if( *pContent == '\0' )
            {
                reason = "字段s结尾出现结束符!";
                return false;
            }
        }
        else if( ch == 'c' )
        {
            if( *pContent++ != 'c' || *pContent++ != '=' )
            {
                reason = "没找到c字段!";
                return false;
            }

            // in
            while( *pContent != ' ' ) pContent++;
            if( pContent == '\0' )
            {
                reason = "c字段in前出现结束符!";
                return false;
            }
            else
            {
                while( *pContent == ' ' ) pContent++;
                if( *pContent == '\0' )
                {
                    reason = "c字段in后出现结束符!";
                    return false;
                }
            }

            // ip version
            while( *pContent != ' ' ) pContent++;
            if( pContent == '\0' )
            {
                reason = "c字段ip version前出现结束符!";
                return false;
            }
            else
            {
                while( *pContent == ' ' ) pContent++;
                if( *pContent == '\0' )
                {
                    reason = "c字段ip version后出现结束符!";
                    return false;
                }
            }

            // connect ip
            anchor = pContent;
            while( *pContent != '\r' && *pContent != '\n' ) pContent++;
            if( *pContent == '\0' )
            {
                reason = "c字段connect ip前出现结束符!";
                return false;
            }
            else
            {
                m_connect.assign( anchor, pContent );
            }
        }
        else if( ch == 't' )
        {
            if( *pContent++ != 't' || *pContent++ != '=' )
            {
                reason = "没找到t字段!";
                return false;
            }

            // start time
            anchor = pContent;
            while( *pContent != ' ' ) pContent++;
            if( *pContent == '\0' )
            {
                reason = "t字段start time前出现结束符!";
                return false;
            }
            else
            {
                m_starttime.assign( anchor, pContent );
                while( *pContent == ' ' ) pContent++;
                if( *pContent == '\0' )
                {
                    reason = "t字段start time后出现结束符!";
                    return false;
                }
            }

            // end time
            anchor = pContent;
            while( *pContent != '\r' && *pContent != '\n' ) pContent++;
            if( *pContent == '\0' )
            {
                reason = "t字段end time前出现结束符!";
                return false;
            }
            else
            {
                m_endtime.assign( anchor, pContent );
            }
        }
        else if( ch == 'm' )
        {
            if( *pContent++ != 'm' || *pContent++ != '=' )
            {
                reason = "没找到m字段!";
                return false;
            }

            // media type
            anchor = pContent;
            while( *pContent != ' ' ) pContent++;
            if( *pContent == '\0' )
            {
                reason = "m字段media type前出现结束符!";
                return false;
            }
            else
            {
                m_mediatype.assign(anchor, pContent);
                while( *pContent == ' ' ) pContent++;
                if( *pContent == '\0' )
                {
                    reason = "m字段media type后出现结束符!";
                    return false;
                }
            }

            // port
            anchor = pContent;
            while( *pContent != ' ') pContent++;
            if( *pContent == '\0' )
            {
                reason = "m字段port前出现结束符!";
                return false;
            }
            else
            {
                m_mediaport.assign( anchor, pContent );
                while( *pContent == ' ' ) pContent++;
                if( *pContent == '\0' )
                {
                    reason = "m字段port后出现结束符!";
                    return false;
                }
            }

            // transport
            anchor = pContent;
            while( *pContent != ' ' ) pContent++;
            if( *pContent == '\0' )
            {
                reason = "m字段transport前出现结束符!";
                return false;
            }
            else
            {
                m_transport.assign( anchor, pContent );
                while( *pContent == ' ' ) pContent++;
                if( *pContent == '\0' )
                {
                    reason = "m字段transport后出现结束符!";
                    return false;
                }
            }

            // rtpmap
            while( *pContent != '\r' && *pContent != '\n' )
            {
                anchor = pContent;
                while( *pContent != ' ' && *pContent != '\r' && *pContent != '\n' ) pContent++;
                if( *pContent == '\0' )
                {
                    reason = "m字段rtpmap前出现结束符!";
                    return false;
                }
                else
                {
                    std::string rtpmap( anchor, pContent );
                    m_rtpmap.insert( std::make_pair( rtpmap, "" ) );
                    while( *pContent == ' ' ) pContent++;
                    if( *pContent == '\0' )
                    {
                        reason = "m字段rtpmap后出现结束符!";
                        return false;
                    }
                }
            }
        }
        else if( ch == 'b' )
        {
            if( *pContent++ != 'b' || *pContent++ != '=' )
            {
                reason = "没找到b字段!";
                return false;
            }

            while( *pContent != '\0' && *pContent != '\r' && *pContent != '\n' ) pContent++;
            if( *pContent == '\0' )
            {
                reason = "字段b结尾出现结束符!";
                return false;
            }
        }
        else if( ch == 'a' )
        {
            if( *pContent++ != 'a' || *pContent++ != '=' )
            {
                reason = "没找到a字段!";
                return false;
            }

            std::string type;
            anchor = pContent;
            while( *pContent != ':' && *pContent != '\r' && *pContent != '\n' ) pContent++;
            if( *pContent == '\0' )
            {
                reason = "a字段rtpmap前出现结束符!";
                return false;
            }
            else
            {
                type.assign( anchor, pContent );
                while( *pContent == ' ' || *pContent == ':' ) pContent++;
                if( *pContent == '\0' )
                {
                    reason = "a字段rtpmap后出现结束符!";
                    return false;
                }
            }

            if( CGBMessage::CompareNoCase( type, "rtpmap" ) )
            {
                anchor = pContent;
                while( *pContent != ' ' ) pContent++;
                if( *pContent == '\0' )
                {
                    reason = "a字段payload type前出现结束符!";
                    return false;
                }

                std::string rtpmap( anchor, pContent );
                while( *pContent == ' ' ) pContent++;
                if( *pContent == '\0' )
                {
                    reason = "a字段payload type后出现结束符!";
                    return false;
                }

                std::map< std::string, std::string >::iterator it = m_rtpmap.find( rtpmap );
                if( it == m_rtpmap.end() )
                {
                    reason = "m字段不存在该payload type!";
                    return false;
                }

                anchor = pContent;
                while( *pContent != '\0' && *pContent != '\r' && *pContent != '\n' )
                {
                    pContent++;
                }

                if( anchor != pContent )
                {
                    it->second.assign( anchor, pContent );
                }
            }
            else if( CGBMessage::CompareNoCase( type, "filesize" ) )
            {
                anchor = pContent;
                while( *pContent != '\r' && *pContent != '\n' ) pContent++;
                if( *pContent == '\0' )
                {
                    reason = "a字段filesize前出现结束符!";
                    return false;
                }
                else
                {
                    m_filesize.assign( anchor, pContent );
                }
            }
            else
            {
                anchor = pContent;
                while( *pContent != '\0' && *pContent != '\r' && *pContent != '\n' )
                {
                    pContent++;
                }

                if( *pContent != '\0' )
                {
                    // send or recv
                }
            }
        }
        else if( ch == 'y' )
        {
            if( *pContent++ != 'y' || *pContent++ != '=' )
            {
                reason = "没找到y字段!";
                return false;
            }

            anchor = pContent;
            while( *pContent != '\0' && *pContent != '\r' && *pContent != '\n' )
            {
                pContent++;
            }

            if( anchor != pContent )
            {
                m_ssrc.assign( anchor, pContent );
            }
        }
        else if( ch == 'f' )
        {
            if( *pContent++ != 'f' || *pContent++ != '=' )
            {
                reason = "没找到f字段!";
                return false;
            }

            anchor = pContent;
            while( *pContent != '\0' && *pContent != '\r' && *pContent != '\n' )
            {
                pContent++;
            }

            if( anchor != pContent )
            {
                m_audioproterty.assign( anchor, pContent );
            }
        }
        else
        {
            while( *pContent != '\0' && *pContent != '\r' && *pContent != '\n' ) pContent++;
            if( *pContent == '\0' )
            {
                reason = "不识别的字段并且出现结束符!";
                return false;
            }
        }

        // next line
        while( *pContent == '\r' || *pContent == '\n' ) pContent++;
        if( *pContent == '\0' )
        {
            break;
        }
    }

    return true;
}

void CUnknowStreamResponseMsg::Process( CSipInterface *pInterface )
{
    if( pInterface != NULL )
    {
        pInterface->OnInviteResponse( *this );
    }
}

//////////////////////////////////////////////////////////////////////////
bool CAckMsg::Send( resip::DialogUsageManager &mDum, bool tcp )
{
    if( m_handle == NULL )
    {
        ERROR_LOG( "m_handle为空，发送失败!" );
        return false;
    }
    else
    {
        // create sdp
        std::string body;
        if( !Encode( body ) )
        {
            ERROR_LOG( "消息编码错误，发送失败!" );
            return false;
        }

        resip::HeaderFieldValue hfv( body.c_str(), body.length() );
        resip::Mime mime_type("Application","sdp");
        resip::PlainContents sdp(hfv, mime_type);
		m_handle->provideAnswer(sdp);
        //m_handle->sendAckWithSDP( sdp );
        return true;
    }
}

bool CAckMsg::Encode( std::string &message )
{
    // sdp
    std::stringstream context;
    context<<"v=0\r\n";
    context<<"o="<< m_recvid <<" 0 0 IN IP4 "<< m_mediaip <<"\r\n";
    context<<"s=Play\r\n";
    context<<"c=IN IP4 "<< m_connect <<"\r\n";
    context<<"t=0 0\r\n";

    context<<"m=video "<< m_mediaport <<" RTP/AVP";
    std::map< std::string, std::string >::const_iterator it;
    for (it = m_rtpmap.begin(); it != m_rtpmap.end(); it++)
    {
        std::string key = it->first;
        std::string text = it->second;
        if ( !text.empty() )
        {
            context<<" "<<key;
        }
    }
    context<<"\r\n";

    context<<"a=sendonly\r\n";
    for (it = m_rtpmap.begin(); it != m_rtpmap.end(); it++)
    {
        std::string key = it->first;
        std::string text = it->second;
        if ( !text.empty() )
        {
            context<<"a=rtpmap:"<<key<<" "<<text<<"\r\n";
        }
    }

	if( !m_ssrc.empty() )
	{
	   context<<"y="<< m_ssrc << "\r\n";
	}

	if( !m_audioproterty.empty() )
	{
	   context<<"f=" << m_audioproterty << "\r\n";
	}

    message = context.str();
	return true;
}

bool CAckMsg::Decode( const char *body, std::string &reason )
{
    // message without sdp
    if( body == NULL )
    {
        WARN_LOG( "ACK消息不带SDP!" );
        return true;
    }

    const char* pContent = body;
    const char* anchor = 0;

    // 解析SDP字段
    while( pContent != '\0' )
    {
        char ch = *pContent;
        if( ch == 'v' )
        {
            if( *pContent++ != 'v' || *pContent++ != '=' )
            {
                return false;
            }

            while( *pContent != '\0' && *pContent != '\r' && *pContent != '\n' ) pContent++;
            if( *pContent == '\0' )
            {
                return false;
            }
        }
        else if( ch == 'o' )
        {
            if( *pContent++ != 'o' || *pContent++ != '=' )
            {
                return false;
            }

            // id
            anchor = pContent;
            while( *pContent != ' ' ) pContent++;
            if( *pContent == '\0' )
            {
                return false;
            }
            else
            {
                m_recvid.assign( anchor, pContent );
                while( *pContent == ' ' ) pContent++;
                if( *pContent == '\0' )
                {
                    return false;
                }
            }

            // session
            anchor = pContent;
            while( *pContent != ' ' ) pContent++;
            if( *pContent == '\0' )
            {
                return false;
            }
            else
            {
                m_sessionid.assign( anchor, pContent );
                while( *pContent == ' ' ) pContent++;
                if( *pContent == '\0' )
                {
                    return false;
                }
            }

            // version
            anchor = pContent;
            while( *pContent != ' ' ) pContent++;
            if( *pContent == '\0' )
            {
                return false;
            }
            else
            {
                m_version.assign( anchor, pContent );
                while( *pContent == ' ' ) pContent++;
                if( *pContent == '\0' )
                {
                    return false;
                }
            }

            // in
            anchor = pContent;
            while( *pContent != ' ' ) pContent++;
            if( *pContent == '\0' )
            {
                return false;
            }
            else
            {
                while( *pContent == ' ' ) pContent++;
                if( *pContent == '\0' )
                {
                    return false;
                }
            }

            // ip version
            anchor = pContent;
            while( *pContent != ' ' ) pContent++;
            if( *pContent == '\0' )
            {
                return false;
            }
            else
            {
                while( *pContent == ' ' ) pContent++;
                if( *pContent == '\0' )
                {
                    return false;
                }
            }

            // media ip
            anchor = pContent;
            while( *pContent != '\r' && *pContent != '\n' ) pContent++;
            if( *pContent == '\0' )
            {
                return false;
            }
            else
            {
                m_mediaip.assign( anchor, pContent );
            }
        }
        else if( ch == 's' )
        {
            if( *pContent++ != 's' || *pContent++ != '=' )
            {
                return false;
            }

            while( *pContent != '\r' && *pContent != '\n' ) pContent++;
            if( *pContent == '\0' )
            {
                return false;
            }
        }
        else if( ch == 'c' )
        {
            if( *pContent++ != 'c' || *pContent++ != '=' )
            {
                return false;
            }

            // in
            while( *pContent != ' ' ) pContent++;
            if( pContent == '\0' )
            {
                return false;
            }
            else
            {
                while( *pContent == ' ' ) pContent++;
                if( *pContent == '\0' )
                {
                    return false;
                }
            }

            // ip version
            while( *pContent != ' ' ) pContent++;
            if( pContent == '\0' )
            {
                return false;
            }
            else
            {
                while( *pContent == ' ' ) pContent++;
                if( *pContent == '\0' )
                {
                    return false;
                }
            }

            // connect ip
            anchor = pContent;
            while( *pContent != '\r' && *pContent != '\n' ) pContent++;
            if( *pContent == '\0' )
            {
                return false;
            }
            else
            {
                m_connect.assign( anchor, pContent );
            }
        }
        else if( ch == 't' )
        {
            if( *pContent++ != 't' || *pContent++ != '=' )
            {
                return false;
            }

            // start time
            anchor = pContent;
            while( *pContent != ' ' ) pContent++;
            if( *pContent == '\0' )
            {
                return false;
            }
            else
            {
                m_starttime.assign( anchor, pContent );
                while( *pContent == ' ' ) pContent++;
                if( *pContent == '\0' )
                {
                    return false;
                }
            }

            // end time
            anchor = pContent;
            while( *pContent != '\r' && *pContent != '\n' ) pContent++;
            if( *pContent == '\0' )
            {
                return false;
            }
            else
            {
                m_endtime.assign( anchor, pContent );
            }
        }
        else if( ch == 'm' )
        {
            if( *pContent++ != 'm' || *pContent++ != '=' )
            {
                return false;
            }

            // media type
            anchor = pContent;
            while( *pContent != ' ' ) pContent++;
            if( *pContent == '\0' )
            {
                return false;
            }
            else
            {
                m_mediatype.assign(anchor, pContent);
                while( *pContent == ' ' ) pContent++;
                if( *pContent == '\0' )
                {
                    return false;
                }
            }

            // port
            anchor = pContent;
            while( *pContent != ' ') pContent++;
            if( *pContent == '\0' )
            {
                return false;
            }
            else
            {
                m_mediaport.assign( anchor, pContent );
                while( *pContent == ' ' ) pContent++;
                if( *pContent == '\0' )
                {
                    return false;
                }
            }

            // transport
            anchor = pContent;
            while( *pContent != ' ' ) pContent++;
            if( *pContent == '\0' )
            {
                return false;
            }
            else
            {
                m_transport.assign( anchor, pContent );
                while( *pContent == ' ' ) pContent++;
                if( *pContent == '\0' )
                {
                    return false;
                }
            }

            // rtpmap
            while( *pContent != '\r' && *pContent != '\n' )
            {
                anchor = pContent;
                while( *pContent != ' ' && *pContent != '\r' && *pContent != '\n' ) pContent++;
                if( *pContent == '\0' )
                {
                    return false;
                }
                else
                {
                    std::string rtpmap( anchor, pContent );
                    m_rtpmap.insert( std::make_pair( rtpmap, "" ) );
                    while( *pContent == ' ' ) pContent++;
                    if( *pContent == '\0' )
                    {
                        return false;
                    }
                }
            }
        }
        else if( ch == 'b' )
        {
            if( *pContent++ != 'b' || *pContent++ != '=' )
            {
                return false;
            }

            while( *pContent != '\0' && *pContent != '\r' && *pContent != '\n' ) pContent++;
            if( *pContent == '\0' )
            {
                return false;
            }
        }
        else if( ch == 'a' )
        {
            if( *pContent++ != 'a' || *pContent++ != '=' )
            {
                return false;
            }

            std::string type;
            anchor = pContent;
            while( *pContent != ':' && *pContent != '\r' && *pContent != '\n' ) pContent++;
            if( *pContent == '\0' )
            {
                return false;
            }
            else
            {
                type.assign( anchor, pContent );
                while( *pContent == ' ' || *pContent == ':' ) pContent++;
                if( *pContent == '\0' )
                {
                    return false;
                }
            }

            if( CGBMessage::CompareNoCase( type, "rtpmap" ) )
            {
                anchor = pContent;
                while( *pContent != ' ' ) pContent++;
                if( *pContent == '\0' )
                {
                    return false;
                }

                std::string rtpmap( anchor, pContent );
                while( *pContent == ' ' ) pContent++;
                if( *pContent == '\0' )
                {
                    return false;
                }

                std::map< std::string, std::string >::iterator it = m_rtpmap.find( rtpmap );
                if( it == m_rtpmap.end() )
                {
                    return false;
                }

                anchor = pContent;
                while( *pContent != '\0' && *pContent != '\r' && *pContent != '\n' )
                {
                    pContent++;
                }

                if( anchor != pContent )
                {
                    it->second.assign( anchor, pContent );
                }
            }
            else
            {
                anchor = pContent;
                while( *pContent != '\0' && *pContent != '\r' && *pContent != '\n' )
                {
                    pContent++;
                }

                if( *pContent != '\0' )
                {
                    // send or recv
                }
            }
        }
        else if( ch == 'y' )
        {
            if( *pContent++ != 'y' || *pContent++ != '=' )
            {
                return false;
            }

            anchor = pContent;
            while( *pContent != '\0' && *pContent != '\r' && *pContent != '\n' )
            {
                pContent++;
            }

            if( anchor != pContent )
            {
                m_ssrc.assign( anchor, pContent );
            }
        }
        else if( ch == 'f' )
        {
            if( *pContent++ != 'f' || *pContent++ != '=' )
            {
                return false;
            }

            anchor = pContent;
            while( *pContent != '\0' && *pContent != '\r' && *pContent != '\n' )
            {
                pContent++;
            }

            if( anchor != pContent )
            {
                m_audioproterty.assign( anchor, pContent );
            }
        }
        else
        {
            while( *pContent != '\0' && *pContent != '\r' && *pContent != '\n' ) pContent++;
            if( *pContent == '\0' )
            {
                return false;
            }
        }

        // next line
        while( *pContent == '\r' || *pContent == '\n' ) pContent++;
        if( *pContent == '\0' )
        {
            break;
        }
    }

	return true;
}

void CAckMsg::Process( CSipInterface *pInterface )
{
	if( pInterface != NULL )
	{
		pInterface->OnAck( *this );
	}
}

//////////////////////////////////////////////////////////////////////////
bool CByeMsg::Send( resip::DialogUsageManager &mDum, bool tcp )
{
	if( m_handle == NULL )
	{
		ERROR_LOG( "m_handle为空，发送失败!" );
		return false;
	}
	else
	{
        m_handle->end(resip::InviteSession::UserHangup);
		return true;
	}
}

bool CByeMsg::Encode( std::string &message )
{
	return true;
}

bool CByeMsg::Decode( const char *body, std::string &reason )
{
	if( body != NULL )
	{
		WARN_LOG(" Bye消息体不应该带消息体数据！ ");
		return false;
	}

	return true;
}

void CByeMsg::Process( CSipInterface *pInterface )
{
	if( pInterface != NULL )
	{
		pInterface->OnBye( *this );
	}
}
