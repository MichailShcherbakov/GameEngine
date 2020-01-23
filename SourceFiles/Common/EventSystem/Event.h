#ifndef _EVENT_H_
#define _EVENT_H_

#include <EventSystem/IHandler.h>
#include <EventSystem/IEvent.h>

template<typename... Args>
class Event : public IEvent<Args...>
{
public:
	Event() : IEvent<Args...>() {}
	~Event()
	{
		for (auto handler : this->m_handlers)
			delete handler;

		this->m_handlers.clear();
	}

public:
	void operator()(Args... args)
	{
		for (auto handler : this->m_handlers)
			handler->Call(args...);
	}
};

template<class Class, class... Args>
class EventHandler : public IHandler<Args...>
{
	using Method = void(Class::*)(Args...);
	using Type = EventHandler<Class, Args...>;

public:
	EventHandler(Class& object, Method method) :
		IHandler<Args...>(),
		m_object(object),
		m_method(method)
	{
		if (&m_object == nullptr)
			throw std::runtime_error("The object pointer is nullptr");
	}

protected:
	// IHandler<Args...>
	virtual bool IsEquals(const IHandler<Args...>& other) const override final
	{
		const Type* _other = dynamic_cast<const Type*>(&other);
		return (_other != nullptr && &m_object == &_other->m_object && m_method == _other->m_method);
	}
	// ~IHandler<Args...>

public:
	// IHandler<Args...>
	virtual void Call(Args... args) override final
	{
		(m_object.*m_method)(args...);
	}
	// ~IHandler<Args...>

private:
	Class& m_object;
	Method m_method;
};

template<class Class, class... Args>
IHandler<Args...>& CreateEventHadnler(Class& object, void(Class::* method)(Args...))
{
	return *new EventHandler<Class, Args...>(object, method);
}

#define MAKE_EVENT(name, ...) \
protected: \
	Event<__VA_ARGS__> m_##name; \
events: \
	IEvent<__VA_ARGS__>& name = m_##name; \


#define LISTENER(class, method) CreateEventHadnler(class, &method);

#endif // _EVENT_H_
