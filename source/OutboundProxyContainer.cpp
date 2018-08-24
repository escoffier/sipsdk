#include <sstream>
#include "OutboundProxyContainer.h"

using namespace resip;
using namespace std;

Data OutboundProxyContainer::m_password;

Data OutboundProxyContainer::getPassword()
{
	return m_password;
}

OutboundProxyContainer::OutboundProxy::OutboundProxy(const char* id, const char* ip, int port, const char* password, const char* realm)
:m_id(id),m_password(password),m_realm(realm)
{
    ostringstream  os;
    os << "sip:" << id << "@" << ip << ":" << port;
    m_uri = Uri(Data(os.str().c_str()));
}

bool OutboundProxyContainer::isMatchedServerCode(const resip::Data& userId, resip::Uri& mathedUri)
{
    for (std::list<OutboundProxy>::iterator it = m_OutboundProxyList.begin(); it != m_OutboundProxyList.end(); ++it)
    {
        if (it->m_id == userId)
        {
            mathedUri = it->m_uri;
            return true;
        }
    }
    return false;
}

OutboundProxyContainer::Iterator  OutboundProxyContainer::begin()const
{
    return m_OutboundProxyList.begin();
}

OutboundProxyContainer::Iterator  OutboundProxyContainer::end()const
{
    return m_OutboundProxyList.end();
}

void        OutboundProxyContainer::addOutboundProxyInfo(const char* id, const char* ip, int port, const char* password, const char* realm)
{
    //目前所有的上级的password应该是一样的
    OutboundProxy proxy(id, ip, port, password, realm);
    m_password = password;
    m_OutboundProxyList.push_back(proxy);
}

bool OutboundProxyContainer::isForMe(const Data& userId)
{
    for (std::list<OutboundProxy>::iterator it = m_OutboundProxyList.begin(); it != m_OutboundProxyList.end(); ++it)
    {
        if (it->m_id == userId)
        {
            return true;
        }
    }
    return false;
}

