#ifndef _I_HANDLER_H_
#define _I_HANDLER_H_

template<class... Args>
class IHandler
{
public:
	using Type = IHandler<Args...>;

protected:
	IHandler() {}

public:
	bool operator==(const Type& other) const
	{
		return IsEquals(other);
	}

	bool operator!=(const Type& other) const
	{
		return !(*this == other);
	}

protected:
	virtual bool IsEquals(const Type& other) const = 0;

public:
	virtual void Call(Args... args) = 0;
};

#endif // _I_HANDLER_H_
