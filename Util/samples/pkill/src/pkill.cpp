//
// pkill.cpp
//
// $Id: //poco/1.3/Util/samples/pkill/src/pkill.cpp#1 $
//
// Process killer utility application.
//
// Copyright (c) 2010, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// Permission is hereby granted, free of charge, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, reproduce, display, distribute,
// execute, and transmit the Software, and to prepare derivative works of the
// Software, and to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:
// 
// The copyright notices in the Software and this entire statement, including
// the above license grant, this restriction and the following disclaimer,
// must be included in all copies of the Software, in whole or in part, and
// all derivative works of the Software, unless such copies or derivative
// works are solely in the form of machine-executable object code generated by
// a source language processor.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//


#include "Poco/Util/Application.h"
#include "Poco/Util/Option.h"
#include "Poco/Util/OptionSet.h"
#include "Poco/Util/HelpFormatter.h"
#include "Poco/Process.h"
#include "Poco/NumberParser.h"
#include <iostream>


using Poco::Util::Application;
using Poco::Util::Option;
using Poco::Util::OptionSet;
using Poco::Util::HelpFormatter;
using Poco::Util::OptionCallback;


class ProcessKillerApp: public Application
{
public:
	ProcessKillerApp(): 
		_helpRequested(false),
		_friendly(false)
	{
	}

protected:		
	void defineOptions(OptionSet& options)
	{
		Application::defineOptions(options);

		options.addOption(
			Option("help", "h", "Display help information on command line arguments.")
				.required(false)
				.repeatable(false)
				.callback(OptionCallback<ProcessKillerApp>(this, &ProcessKillerApp::handleHelp)));

		options.addOption(
			Option("friendly", "f", "Kindly ask application to shut down.")
				.required(false)
				.repeatable(false)
				.callback(OptionCallback<ProcessKillerApp>(this, &ProcessKillerApp::handleFriendly)));
	}
	
	void handleHelp(const std::string& name, const std::string& value)
	{
		_helpRequested = true;
		stopOptionsProcessing();
	}
	
	void handleFriendly(const std::string& name, const std::string& value)
	{
		_friendly = true;
	}
			
	void displayHelp()
	{
		HelpFormatter helpFormatter(options());
		helpFormatter.setCommand(commandName());
		helpFormatter.setUsage("[options] <pid> ...");
		helpFormatter.setHeader("A utility application to kill processes.");
		helpFormatter.setFooter("Note that the friendly option only works with applications using Poco::Util::ServerApplication::waitForTerminationRequest().");
		helpFormatter.format(std::cout);
	}
	
	int main(const std::vector<std::string>& args)
	{
		if (_helpRequested || args.empty())
		{
			displayHelp();
		}
		else
		{
			for (std::vector<std::string>::const_iterator it = args.begin(); it != args.end(); ++it)
			{
				Poco::Process::PID pid = Poco::NumberParser::parseUnsigned(*it);
				if (_friendly)
					Poco::Process::requestTermination(pid);
				else
					Poco::Process::kill(pid);
			}
		}
		return Application::EXIT_OK;
	}
		
private:
	bool _helpRequested;
	bool _friendly;
};


POCO_APP_MAIN(ProcessKillerApp)