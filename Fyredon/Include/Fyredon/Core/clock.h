// Copyright Fyredon. All Rights Reserved.

/*=============================================================================
	Clock.h: [..] : ?
=============================================================================*/
#pragma once

namespace Fyredon {

class Clock
{
public:
	Clock();
	~Clock();
	void run();
	void stop();

private:
	bool isRuning;
	int hours;
	int min;
	int sec;
	void show();
	void tick();
};

}
