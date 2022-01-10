#ifndef EVENT_H
#define EVENT_H

class Event0
{
private:
	void (*_handlers[10])();
	int _handlerCount;

public:
	void Subscribe(void (*handler)())
	{
		_handlers[_handlerCount++] = handler;
	}
	void Raise()
	{
		for (int i = 0; i < _handlerCount; ++i)
		{
			_handlers[i]();
		}
	}
};

template <typename T1>
class Event1
{
private:
	void (*_handlers[10])(T1);
	int _handlerCount;

public:
	void Subscribe(void (*handler)(T1))
	{
		_handlers[_handlerCount++] = handler;
	}
	void Raise(T1 x1)
	{
		for (int i = 0; i < _handlerCount; ++i)
		{
			_handlers[i](x1);
		}
	}
};

template <typename T1, typename T2>
class Event2
{
private:
	void (*_handlers[10])(T1, T2);
	int _handlerCount;

public:
	void Subscribe(void (*handler)(T1, T2))
	{
		_handlers[_handlerCount++] = handler;
	}
	void Raise(T1 x1, T2 x2)
	{
		for (int i = 0; i < _handlerCount; ++i)
		{
			_handlers[i](x1, x2);
		}
	}
};

#endif // EVENT_H