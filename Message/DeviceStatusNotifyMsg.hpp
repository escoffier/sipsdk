#ifndef MSG_QUERY_DEVICESTATUS_HPP_
#define MSG_QUERY_DEVICESTATUS_HPP_
#include "MessageMsg.hpp"

//////////////////////////////////////////////////////////////////////////
class CQueryDeviceStatusMsg : public CGBMessageMsgT< CGBMessageMsg::MsgCmdQuery >
{
public:
	CQueryDeviceStatusMsg() : CGBMessageMsgT< CGBMessageMsg::MsgCmdQuery >( CGBMessage::CmdQueryDeviceStatus )
	{
	}

public:
	virtual bool Encode( std::ostream &stream );
	virtual bool Decode( TiXmlNode *pNode );
	virtual void Process( CSipInterface *pInteface );
};

//////////////////////////////////////////////////////////////////////////
class CQueryDeviceStatusReponseMsg : public CGBMessageMsgT< CGBMessageMsg::MsgCmdReponse >
{
public:
	CQueryDeviceStatusReponseMsg() : CGBMessageMsgT< CGBMessageMsg::MsgCmdReponse >( CGBMessage::CmdQueryDeviceStatusReponse )
	{
	}

public:
	virtual bool Encode( std::ostream &stream );
	virtual bool Decode( TiXmlNode *pNode );
	virtual void Process( CSipInterface *pInteface );
};

#endif // MSG_QUERY_DEVICESTATUS_HPP_