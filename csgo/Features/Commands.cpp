#include "Commands.h"
#include <vector>
#include "../Utilities/Utilities.h"
#include "../Utilities/Interfaces.h"
#include "../CompilerDefinitions.h"

// Example command without args: ping
// Example command with args ping; "8"
void Features::Commands::Handler(std::string szCommandAndArgs)
{
	std::vector<char> ArgsChars = {};
	std::vector<std::string> Args = {};

	bool bHasArgs = false;
	bool bSearchArgs = false;
	bool bStartArg = false;
	int nStartArgOffset = 0;

	std::string szCommand = szCommandAndArgs;

	for (int i = 0; i < szCommandAndArgs.length(); i++)
	{
		char c = szCommandAndArgs.at(i);
		if (c == '"')
		{
			bHasArgs = true;
			bSearchArgs = true;
			bStartArg = true;
			nStartArgOffset = i;
		}

		// n chars after start of arg
		if (bSearchArgs && c != '"')
			ArgsChars.push_back(c);

		// The second time we come across " is at the end of the arg
		if (c == '"' && bSearchArgs)
		{
			// Set the chars from the current argument into a const char*
			Args.push_back(Utilities::CharsToString(ArgsChars));
			bSearchArgs = false;
		}
	}

	if (!bHasArgs)
		szCommand = szCommandAndArgs;
	else
		szCommand = szCommandAndArgs.erase(nStartArgOffset, szCommand.back());

#ifdef PRINT_COMMAND_RECIEVED
	printf("[Features::Commands::Handler] cmd %s, ", szCommand.c_str());

	for (int i = 0; i < Args.size(); i++)
		printf("arg%i, %s", i, Args.at(i).c_str());
	
	printf("\n");
#endif // PRINT_COMMAND_RECIEVED

	SetName(szCommand, Args);
}

void Features::Commands::SetName(std::string szCommand, std::vector<std::string> Args)
{
	if (szCommand.compare("setname"))
	{
		// Is this what's crashing?

		/*static auto pNameConVar = Interfaces::m_pCVar->FindVar("name");
		pNameConVar->fnChangeCallbacks.Size() = 0;

		// Fix so we can change names how many times we want
		static bool bOnce = (pNameConVar->SetValue("\n���"), true);

		pNameConVar->SetValue(Args.at(0).c_str());*/
	}
}