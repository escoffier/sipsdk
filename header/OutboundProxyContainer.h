#ifndef _OUTBOUNDPROXYCONTAINER_H_
#define _OUTBOUNDPROXYCONTAINER_H_

#include <list>
#include "rutil/Data.hxx"
#include "resip/stack/Uri.hxx"

class OutboundProxyContainer
{
    struct OutboundProxy
    {
        OutboundProxy(const char* id, const char* ip, int port, const char* password, const char* realm);
        resip::Data m_id;
        resip::Data m_password;
        resip::Data m_realm;
        resip::Uri  m_uri;
    }; 

public:
    typedef std::list<OutboundProxy>::const_iterator Iterator;
    bool isMatchedServerCode(const resip::Data& userId, resip::Uri& mathedUri);
    void addOutboundProxyInfo(const char* id, const char* ip, int port, const char* password, const char* realm);
    bool isForMe(const resip::Data& userId);
    OutboundProxyContainer::Iterator begin()const;
    OutboundProxyContainer::Iterator end()const;
	static resip::Data getPassword();

private:
    std::list<OutboundProxy> m_OutboundProxyList;
    static resip::Data m_password;
};

#endif
