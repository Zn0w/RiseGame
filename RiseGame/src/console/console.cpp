#include "console.h"

namespace console
{
	void print(std::string message)
	{
		console_log.setString(console_log.getString() + message + "\n");
	}
}