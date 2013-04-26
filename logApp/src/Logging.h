/// @file Logging.h
///
/// Wrapper for log4cxx
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
#ifndef __IOC_LOGGING_H__
#define __IOC_LOGGING_H__

#ifdef __cplusplus
namespace askap {
namespace ioclog {
#endif

#ifdef ASKAP_PACKAGE_NAME
#define PACKAGE_NAME ASKAP_PACKAGE_NAME
#ifndef LOGGER_PREFIX
#define LOGGER_PREFIX "askap."
#endif
#else
#ifndef LOGGER_PREFIX
#define LOGGER_PREFIX "epics."
#endif
#endif

#ifndef PACKAGE_NAME
#define PACKAGE_NAME "unknown"
#endif

typedef enum {
    LOG_LEVEL_OFF,
    LOG_LEVEL_FATAL,
    LOG_LEVEL_ERROR,
    LOG_LEVEL_WARN,
    LOG_LEVEL_INFO,
    LOG_LEVEL_DEBUG,
    LOG_LEVEL_TRACE,
    LOG_LEVEL_COUNT
} IocLogLevel;

typedef struct _IOC_LOG
{
  int (*configure)(const char *logConfig);

  int (*isLevelEnabled)(IocLogLevel level, const char*logger);

  void (*forcedLog)(const char *logger, IocLogLevel level, const char* file,
          const char *func, const int line, const char* fmt, ...);

  void (*pushContext)(const char* fmt, ...);

  void (*popContext)();
} IOC_LOG;

#ifdef __cplusplus
}
}
#endif

#ifdef __cplusplus
#include <log4cxx/logger.h>
#include <log4cxx/ndc.h>

namespace askap {
namespace ioclog {


typedef log4cxx::LoggerPtr IocLogger;

inline std::string loggerName(const std::string& inname)
{
const std::string name(LOGGER_PREFIX);
if (inname.length() > 0) {
  if (inname[0] == '.') {
    return (name + std::string(PACKAGE_NAME) + inname);
  } else {
    return (name + inname);
  }
}
return (name + std::string(PACKAGE_NAME));
}

#define CREATE_IOC_LOGGER(logName) log4cxx::Logger::getLogger(askap::ioclog::loggerName(logName))

#define CREATE_NAMED_LOGGER(namedLogger, logName) \
    static askap::ioclog::IocLogger namedLogger = CREATE_IOC_LOGGER(logName)

#define CREATE_LOGGER(logName) CREATE_NAMED_LOGGER(LOGGER, logName)

#define LOG_PUSH_CONTEXT(fmt, ...) log_push_context(fmt, ##__VA_ARGS__)
#define LOG_POP_CONTEXT() log_pop_context()

#define LOG_TRACE(fmt, ...) LOG_TRACE2(LOGGER, fmt, ##__VA_ARGS__)
#define LOG_DEBUG(fmt, ...) LOG_DEBUG2(LOGGER, fmt, ##__VA_ARGS__)
#define LOG_INFO(fmt, ...) LOG_INFO2(LOGGER, fmt, ##__VA_ARGS__)
#define LOG_WARN(fmt, ...) LOG_WARN2(LOGGER, fmt, ##__VA_ARGS__)
#define LOG_ERROR(fmt, ...) LOG_ERROR2(LOGGER, fmt, ##__VA_ARGS__)
#define LOG_FATAL(fmt, ...) LOG_FATAL2(LOGGER, fmt, ##__VA_ARGS__)

#define LOG_MSG(level, logger, fmt, ...)   if (LOG4CXX_UNLIKELY(logger->is##level##Enabled())) {\
                                            askap::ioclog::log_msg(logger, log4cxx::Level::get##level(),\
                                            LOG4CXX_LOCATION, fmt, ##__VA_ARGS__); \
                                }

#define LOG_TRACE2(logger, fmt, ...)  LOG_MSG(Trace, logger, fmt, ##__VA_ARGS__)
#define LOG_DEBUG2(logger, fmt, ...)  LOG_MSG(Debug, logger, fmt, ##__VA_ARGS__)
#define LOG_INFO2(logger, fmt, ...)  LOG_MSG(Info, logger, fmt, ##__VA_ARGS__)
#define LOG_WARN2(logger, fmt, ...)  LOG_MSG(Warn, logger, fmt, ##__VA_ARGS__)
#define LOG_ERROR2(logger, fmt, ...)  LOG_MSG(Error, logger, fmt, ##__VA_ARGS__)
#define LOG_FATAL2(logger, fmt, ...)  LOG_MSG(Fatal, logger, fmt, ##__VA_ARGS__)


///@ brief Scoped Log Context
///
/// Creating an instance of the LogContext class will
/// push the supplied string (printf formatted) onto the
/// current thread's logging context. When the instance goes
/// out of scope, the context will be popped automatically
///
class LogContext
{
public:
    LogContext(const char *fmt, ...);
    ~LogContext();
};

/// @brief initialise logging
///
/// initialise logging
/// 
/// @param cfgFile full path to config file.  If omitted or NULL
///                then environment variable IOC_LOG_CONFIG is
///                used or finally ioc.log_cfg from the current directory
int log_init(const char* cfgFile=NULL);

/// @brief set log level of root logger
///
/// @param level level to set root logger to
///
void set_log_level(IocLogLevel level);

log4cxx::LevelPtr level_num_to_ptr(IocLogLevel level);
int level_ptr_to_num(log4cxx::LevelPtr level);

/// @brief log a message using log4cxx location info
///
/// @param logger
/// @param level
/// @param loc
/// @param fmt
///
void log_msg(askap::ioclog::IocLogger logger, log4cxx::Level *level,\
        const log4cxx::spi::LocationInfo &loc, const char *fmt, ...);

/// @brief log a message using C-style file, function name & line number
///
/// @param logger
/// @param level
/// @param file
/// @param line
/// @param fmt
///
void log_msg(const char *logger, IocLogLevel level, const char* file,
        const char* func, const int line, const char* fmt, ...);

/// @brief push a logging context onto the current threads stack
///
/// @param fmt
///
void log_push_context(const char *fmt, ...);

/// @brief pop a logging context off the current thread
///
void log_pop_context();
}
}
#else //cplusplus

#define CREATE_LOGGER(name)     static char *LOGGER = LOGGER_PREFIX PACKAGE_NAME name
#define CREATE_NAMED_LOGGER(logger, name)     static char *logger = LOGGER_PREFIX PACKAGE_NAME name

extern IOC_LOG *IocLog;

#define LOG_PUSH_CONTEXT(fmt, ...) IocLog->pushContext(fmt, ##__VA_ARGS__)
#define LOG_POP_CONTEXT() IocLog->popContext()

#define LOG_MSG(level, logger, fmt, ...)   if(IocLog->isLevelEnabled(LOG_LEVEL_##level, logger)) \
                                            IocLog->forcedLog(logger, LOG_LEVEL_##level,  \
                                            __FILE__, __FUNCTION__,  __LINE__, fmt, ##__VA_ARGS__)

#define LOG_TRACE(fmt, ...) LOG_TRACE2(LOGGER, fmt, ##__VA_ARGS__)
#define LOG_DEBUG(fmt, ...) LOG_DEBUG2(LOGGER, fmt, ##__VA_ARGS__)
#define LOG_INFO(fmt, ...) LOG_INFO2(LOGGER, fmt, ##__VA_ARGS__)
#define LOG_WARN(fmt, ...) LOG_WARN2(LOGGER, fmt, ##__VA_ARGS__)
#define LOG_ERROR(fmt, ...) LOG_ERROR2(LOGGER, fmt, ##__VA_ARGS__)
#define LOG_FATAL(fmt, ...) LOG_FATAL2(LOGGER, fmt, ##__VA_ARGS__)

#define LOG_TRACE2(logger, fmt, ...) LOG_MSG(TRACE, logger, fmt, ##__VA_ARGS__)
#define LOG_DEBUG2(logger, fmt, ...) LOG_MSG(DEBUG, logger, fmt, ##__VA_ARGS__)
#define LOG_INFO2(logger, fmt, ...) LOG_MSG(INFO, logger, fmt, ##__VA_ARGS__)
#define LOG_WARN2(logger, fmt, ...) LOG_MSG(WARN, logger, fmt, ##__VA_ARGS__)
#define LOG_ERROR2(logger, fmt, ...) LOG_MSG(ERROR, logger, fmt, ##__VA_ARGS__)
#define LOG_FATAL2(logger, fmt, ...) LOG_MSG(FATAL, logger, fmt, ##__VA_ARGS__)

#endif
#endif
