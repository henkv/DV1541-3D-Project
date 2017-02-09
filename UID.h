#pragma once

class UID
{
public:
	typedef size_t uid_t;
	static uid_t Get()
	{
		return UID_BASE++;
	}

private:
	static uid_t UID_BASE;
};

UID::uid_t UID::UID_BASE = 0;