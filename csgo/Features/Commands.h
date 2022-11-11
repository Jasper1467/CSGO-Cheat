#pragma once

#include <string>
#include <vector>

namespace Features
{
	namespace Commands
	{
		void Handler(std::string szCommandAndArgs);

		// setname <args>
		// args: a1 (text of name to set)
		// Example: setname; "Elite"
		void SetName(std::string szCommand, std::vector<std::string> Args);
	}
}