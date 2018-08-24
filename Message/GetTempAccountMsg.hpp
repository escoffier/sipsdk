#ifndef MSG_GET_TEMPACCOUNT_HPP_
#define MSG_GET_TEMPACCOUNT_HPP_
#include "MessageMsg.hpp"
#include <list>

//////////////////////////////////////////////////////////////////////////
class CGetTempAccountMsg : public CMessageMsgT< CMessageMsg::MsgCmdQuery >
{
public:
	CGetTempAccountMsg()
	{
	}

public:
	virtual bool Encode( std::string &message );
	virtual bool Decode( const std::vector< XMLNode* > &nodes, std::string &reason );
	virtual void Process( CSipInterface *pInterface );

public:

private:
};

//////////////////////////////////////////////////////////////////////////
class CGetTempAccountResponseMsg : public CMessageMsgT< CMessageMsg::MsgCmdResponse >
{
public:
	CGetTempAccountResponseMsg()
	{
	}

public:
    inline bool IsOK() const
    {
        return m_result == CGBMessage::RESULT_OK;
    }

    inline bool HasError() const
    {
        return m_result == CGBMessage::RESULT_ERROR;
    }

    inline CGBMessage::EGBResultType GetResult() const
    {
        return m_result;
    }

    inline void SetResult( CGBMessage::EGBResultType result )
    {
        m_result = result;
    }

    inline void SetOK()
    {
        m_result = CGBMessage::RESULT_OK;
    }

    inline void SetError()
    {
        m_result = CGBMessage::RESULT_ERROR;
    }

    inline const std::string& GetAccount() const
    {
        return m_account;
    }

    inline void SetAccount( const std::string& account )
    {
        m_account = account;
    }

    inline const std::string& GetPassword() const
    {
        return m_password;
    }

    inline void SetPassword( const std::string& password )
    {
        m_password = password;
    }

public:
	virtual bool Encode( std::string &message );
	virtual bool Decode( const std::vector< XMLNode* > &nodes, std::string &reason );
	virtual void Process( CSipInterface *pInterface );

private:
    EGBResultType             m_result;
    std::string               m_account;
    std::string               m_password;

};
//////////////////////////////////////////////////////////////////////////
#endif // MSG_GET_TEMPACCOUNT_HPP_
