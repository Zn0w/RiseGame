#include "console.h"

namespace console
{
	sf::Text console_log;
	
	void print(std::string message)
	{
		console_log.setString(console_log.getString() + message);
	}
}