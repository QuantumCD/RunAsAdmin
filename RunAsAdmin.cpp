/* The MIT License (MIT)

Copyright (c) 2013 QuantumCD

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

*/

#include "stdafx.h"
#include <windows.h>
#include <string>
#include <vector>
#include <fstream>

// This little bit simply gets rid of the console window
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")

int main(int argc, char* argv[])
{
	std::vector<std::wstring> parameters;
	std::wstring paramString;
	std::wstring applicationExecutable;

	// We start at argv[1] as that is the second parameter passed, etc.
	// The first is always the program itself (possibly a path instead).
	for (int i = 1; i < argc; i++)
	{
		// Convert to wide string
		std::string standardString(argv[i]);
		std::wstring wideString;

		// This is a handy way you can convert strings to wide strings ;)
		wideString.assign(standardString.begin(), standardString.end());

		parameters.push_back(wideString);
	}

	// C++11 ;)
	for (auto &str : parameters)
		paramString += L" " + str;

	std::ifstream configFileStream;
	configFileStream.open("RunAsAdmin.cfg", std::ifstream::in);

	// Make sure the file is open for reading. 
	if (configFileStream.is_open())
	{
		std::string applicationNameFromConfig;
		std::getline(configFileStream, applicationNameFromConfig);

		std::wstring wApplicationNameFromConfig;
		wApplicationNameFromConfig.assign(applicationNameFromConfig.begin(), applicationNameFromConfig.end());

		std::ifstream checkIfExists(wApplicationNameFromConfig);

		// Make sure the config executable actually exists. 
		if (checkIfExists)
		{
			applicationExecutable = wApplicationNameFromConfig;
		}
		else
		{
			MessageBox(NULL, L"Unable to locate suitable executable.\nCheck your configuration file.\n(Have you created a configuration file?)",
				L"Run As Admin - Error", MB_OK | MB_ICONERROR);
			return -1;
		}

		std::string commandLineParametersFromConfig;
		std::getline(configFileStream, commandLineParametersFromConfig);

		// This is just a fallback. Use the provided parameters before anything else, i.e. the ones passed to this application (argv)
		if (!commandLineParametersFromConfig.empty() && paramString.empty())
		{
			paramString.assign(commandLineParametersFromConfig.begin(), commandLineParametersFromConfig.end());
		}

		configFileStream.close();
	}
	else if (applicationExecutable.empty()) // This triggers if a) the file can't be opened and b) there is no default file name provided in the source.
	{
		MessageBox(NULL, L"Unable to open configuration file.\n(Have you created a configuration file?)", L"Run As Admin - Error", MB_OK | MB_ICONERROR);
		return -2;
	}

	// This launches the application with the UAC prompt, and administrator rights are requested. 
	ShellExecute(NULL, _T("RUNAS"), (LPCWSTR)applicationExecutable.c_str(), (LPCWSTR)paramString.c_str(), NULL, SW_SHOWNORMAL);

	return 0;
}

