/*
 * main.cpp
 *
 *  Created on: Aug 11, 2008
 *      Author: yamokosk
 */
#include <iostream>
#include <string>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>

// include log4cxx header files.
#include <log4cxx/logger.h>
#include <log4cxx/basicconfigurator.h>
#include <log4cxx/propertyconfigurator.h>
#include <log4cxx/helpers/exception.h>

using namespace log4cxx;
using namespace log4cxx::helpers;

LoggerPtr logger(Logger::getLogger("TestSuite"));


#ifdef __APPLE__
#include <stdlib.h>

int atexit(void (*func)(void));

void persistent()
{
	std::cout << "Press any key to continue ...";
	std::string z;
	getline(std::cin,z);
}
#endif



int main( int argc, char **argv)
{
	// Set up a simple configuration that logs on the console.
	if (argc > 1)
	{
		// BasicConfigurator replaced with PropertyConfigurator.
		PropertyConfigurator::configure(argv[1]);
	}
	else
	{
		BasicConfigurator::configure();
	}

	LOG4CXX_INFO(logger, "Entering application.");

#ifdef __APPLE__
	/*
	 * author yamokosk
	 *
	 * Hack to get some memory leak detection on OS X. This call will prevent the program
	 * from terminating, thus allowing us to run a program like leaks to look for memory leaks.
	 */
	atexit(persistent);
#endif

	CppUnit::TextUi::TestRunner runner;
	CppUnit::TestFactoryRegistry &registry = CppUnit::TestFactoryRegistry::getRegistry();
	runner.addTest( registry.makeTest() );
	bool wasSucessful = runner.run( "", false );
	LOG4CXX_INFO(logger, "Exiting application.");

	return !wasSucessful;
}
