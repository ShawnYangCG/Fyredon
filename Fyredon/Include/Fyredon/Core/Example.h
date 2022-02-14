// TEMP
#pragma once

#include <Fyredon/Other/dllport.h>

namespace Fyredon {

class FD_API Example {
	public:
		
		explicit Example();
		void Print();

	private:
		
		const char* message = "It works";
	};
}