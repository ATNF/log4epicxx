/// @file Logging.cc
///
/// log4cxx init and control wrapper
///
/// @copyright (c) 2012 CSIRO
/// Australia Telescope National Facility (ATNF)
/// Commonwealth Scientific and Industrial Research Organisation (CSIRO)
/// PO Box 76, Epping NSW 1710, Australia
/// atnf-enquiries@csiro.au
///
/// This file is part of the ASKAP software distribution.
///
/// The ASKAP software distribution is free software: you can redistribute it
/// and/or modify it under the terms of the GNU General Public License as
/// published by the Free Software Foundation; either version 2 of the License,
/// or (at your option) any later version.
///
/// This program is distributed in the hope that it will be useful,
/// but WITHOUT ANY WARRANTY; without even the implied warranty of
/// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
/// GNU General Public License for more details.
///
/// You should have received a copy of the GNU General Public License
/// along with this program; if not, write to the Free Software
/// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
///
/// @author Craig Haskins <Craig.Haskins@csiro.au>
///
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <fstream>
#include <getopt.h>
#include <log4cxx/logger.h>
#include <log4cxx/level.h>
#include <log4cxx/basicconfigurator.h>
#include <log4cxx/propertyconfigurator.h>
#include <log4cxx/xml/domconfigurator.h>
#include <log4cxx/patternlayout.h>
#include <log4cxx/consoleappender.h>
#ifndef STANDALONE_BUILD
#include "askap_ioclog.h"
#endif
#include "Logging.h"

CREATE_LOGGER(".init");

using namespace log4cxx;
using namespace log4cxx::helpers;
using namespace log4cxx::xml;
using namespace askap;
using namespace ioclog;

static LevelPtr LogLevels[] =
{
  Level::getOff(),
  Level::getFatal(),
  Level::getError(),
  Level::getWarn(),
  Level::getInfo(),
  Level::getDebug(),
  Level::getTrace()
};

static bool logInitDone = false;

/// @class Scoped Log Context
LogContext::LogContext(const char *fmt, ...)
{
    va_list va;
    char buf[256];

    va_start(va, fmt);
    vsnprintf(buf, sizeof(buf), fmt, va);
    va_end(va);
    log4cxx::NDC::push(buf);
}

LogContext::~LogContext()
{
    log4cxx::NDC::pop();
}

int askap::ioclog::log_init(const char *cfgParam)
{
    int result = 0;
    std::string cfgFile;

    if (logInitDone) {
        return result;
    }

    if (NULL != cfgParam) {
        cfgFile = cfgParam;
    }
    if (0 == cfgFile.size()) {
        cfgFile = "ioc.log_cfg";
        if (getenv("IOC_LOG_CONFIG")) {
            cfgFile = getenv("IOC_LOG_CONFIG");
        }
    }

    try
    {
        if (cfgFile.substr(cfgFile.find_last_of(".") + 1) == "xml") {
	        DOMConfigurator::configure(cfgFile.c_str());
	        LOG_INFO("logging configured with %s", cfgFile.c_str());	  
	    }
        else if (std::ifstream(cfgFile.c_str())) {
            PropertyConfigurator::configure(cfgFile.c_str());
            LOG_INFO("logging configured with %s", cfgFile.c_str());
        }
        else {
            //PatternLayout* layout = new PatternLayout(">>>%l<<< \%-5p [\%t]: \%m\%n");
            PatternLayout* layout = new PatternLayout("\%-5p \%c:\%m\%n");
            ConsoleAppender* appender = new ConsoleAppender(layout);
            BasicConfigurator::configure(appender);
            set_log_level(LOG_LEVEL_INFO);
            LOG_INFO("config file %s not found, loading default config", cfgFile.c_str());
        }
        logInitDone = true;
    }
    catch(Exception&)
    {
        result = -1;
    }
    return result;
}

static IocLogger get_logger(const char *logger)
{
  IocLogger log = 0;
  if(logger) {
      log = Logger::getLogger(logger);
  }
  else {
    log = Logger::getRootLogger();
  }
  return(log);
}

static int isLevelEnabled(IocLogLevel level, const char *logger)
{
  return(get_logger(logger)->isEnabledFor(LogLevels[level]));
}

void askap::ioclog::log_msg(IocLogger logger, log4cxx::Level *level, const log4cxx::spi::LocationInfo &loc, const char *fmt, ...)
{
    va_list va;
    char buf[256];

    va_start(va, fmt);
    vsnprintf(buf, sizeof(buf), fmt, va);
    va_end(va);
    logger->forcedLog(level, buf, loc);
}

void askap::ioclog::log_msg(const char *logger, IocLogLevel level, const char* file,
        const char* func, const int line, const char* fmt, ...)
{
    va_list va;
    char buf[256];

    va_start(va, fmt);
    vsnprintf(buf, sizeof(buf), fmt, va);
    va_end(va);
    get_logger(logger)->forcedLog(LogLevels[level], buf, log4cxx::spi::LocationInfo(file, func, line));
}

void askap::ioclog::log_push_context(const char *fmt, ...)
{
    va_list va;
    char buf[256];

    va_start(va, fmt);
    vsnprintf(buf, sizeof(buf), fmt, va);
    va_end(va);
    log4cxx::NDC::push(buf);
}

void askap::ioclog::log_pop_context()
{
    log4cxx::NDC::pop();
}


void askap::ioclog::set_log_level(IocLogLevel logLevel)
{
    IocLogger rootLogger = Logger::getRootLogger();
    rootLogger->setLevel(LogLevels[logLevel]);
}

LevelPtr askap::ioclog::level_num_to_ptr(IocLogLevel levelNum)
{
    if (levelNum > LOG_LEVEL_COUNT) {
        levelNum = LOG_LEVEL_TRACE;
    }
    return LogLevels[levelNum];
}

int askap::ioclog::level_ptr_to_num(LevelPtr level)
{
    for(int i=0; i < LOG_LEVEL_COUNT; ++i) {
        if (level->equals(LogLevels[i])) {
            return i;
        }
    }
    return 0;
}

/// @brief C Interface to Log4cxx Logging
IOC_LOG LogFunctions =
{
  &log_init,
  &isLevelEnabled,
  &log_msg,
  &log_push_context,
  &log_pop_context
};

IOC_LOG *IocLog = &LogFunctions;
