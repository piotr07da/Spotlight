#ifndef EVENT_H
#define EVENT_H

const int Event_MaxHandlerCount = 5;

class Event0
{
private:
	void (*_handlers[Event_MaxHandlerCount])();
	int _handlerCount;

public:
	Event0()
	{
		_handlerCount = 0;
	}

	void Subscribe(void (*handler)())
	{
		if (_handlerCount < Event_MaxHandlerCount)
		{
			_handlers[_handlerCount++] = handler;
		}
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
	void (*_handlers[Event_MaxHandlerCount])(T1);
	int _handlerCount;

public:
	Event1()
	{
		_handlerCount = 0;
	}

	void Subscribe(void (*handler)(T1))
	{
		if (_handlerCount < Event_MaxHandlerCount)
		{
			_handlers[_handlerCount++] = handler;
		}
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
	void (*_handlers[Event_MaxHandlerCount])(T1, T2);
	int _handlerCount;

public:
	Event2()
	{
		_handlerCount = 0;
	}

	void Subscribe(void (*handler)(T1, T2))
	{
		if (_handlerCount < Event_MaxHandlerCount)
		{
			_handlers[_handlerCount++] = handler;
		}
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