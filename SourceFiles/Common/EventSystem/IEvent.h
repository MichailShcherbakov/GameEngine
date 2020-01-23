#ifndef _I_EVENT_H_
#define _I_EVENT_H_

#include <list>
#include <algorithm>

#include "IHandler.h"

#define events public
#define emit

template<typename... Args>
class IEvent
{
	using Handler = IHandler<Args...>;
	using HandlerIt = typename std::list<Handler*>::const_iterator;

public:
	void operator+=(Handler& handler)
	{
		if (Find(handler) == m_handlers.end())
			m_handlers.push_back(&handler);
	}

	void operator-=(Handler& handler)
	{
		auto it = Find(handler);
		if (it != m_handlers.end())
		{
			auto handler = *it;
			m_handlers.erase(it);
			delete handler;
		}
	}

private:
	inline HandlerIt Find(Handler& handler) const
	{
		return std::find_if(m_handlers.begin(), m_handlers.end(), [&handler](const Handler* _handler) { return (*_handler == handler); });
	}

protected:
	std::list<Handler*> m_handlers;
};

#endif // _I_EVENT_H_
