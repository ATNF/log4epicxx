///
/// @copyright (c) 2011 CSIRO
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
/// @author Craig Haskins<Craig.Haskins@csiro.au>

#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <string>
#include <vector>
#include <aSubRecord.h>
#include <epicsStdio.h>
#include <dbStaticLib.h> 
#include <dbAddr.h> 
#include <dbAccessDefs.h>
#include <registryFunction.h>
#include <epicsExport.h>
#include <errlog.h>
#include <epicsTypes.h>

#include <log4cxx/logmanager.h>
#include <adbe/Logging.h>

using namespace askap;
using namespace adbe;

CREATE_LOGGER(".AdeSubroutines");

int logging_get_loggers(aSubRecord *asub)
{
    LogContext context("rec.%s", asub->name);
    epicsOldString* loggers = (epicsOldString *)asub->vala;
    epicsOldString* levelNames = (epicsOldString *)asub->valb;
    epicsInt32* levelNums = (epicsInt32 *)asub->valc;

    log4cxx::LoggerList loggerList = log4cxx::LogManager::getCurrentLoggers();
    loggerList.push_back(log4cxx::LogManager::getRootLogger());
    LOG_INFO("getting loggers, %d loggers available", loggerList.size());
    int i = 0;
    for(log4cxx::LoggerList::iterator loggerIter = loggerList.begin(); loggerIter != loggerList.end(); ++loggerIter, ++i)
    {
        std::string loggerName;
        std::string levelName;
        log4cxx::LevelPtr level;

        (*loggerIter)->getName(loggerName);
        strncpy(loggers[i], loggerName.c_str(), sizeof(epicsOldString));

        level = (*loggerIter)->getLevel();
        if (level) {
            level->toString(levelName);
            levelNums[i] = level->toInt();
            if(levelNums[i] == log4cxx::Level::OFF_INT) {
               levelNums[i] = 60;
            }
            else {
               levelNums[i] /= 1000;
            }
        }
        else {
            levelName = "";
            levelNums[i] = 0;
        }
        strncpy(levelNames[i], levelName.c_str(), sizeof(epicsOldString));
        LOG_DEBUG("logger %d: %s, level %s (%d)", i, loggerName.c_str(), levelNames[i], levelNums[i]);
    }
    asub->neva = i;
    asub->nevb = i;
    asub->nevc = i;

    return 0;
}

int logging_set_log_levels(aSubRecord *asub)
{
    LogContext context("rec.%s", asub->name);
    epicsOldString* loggerNames = (epicsOldString *)asub->a;
    epicsOldString* levelNames = (epicsOldString *)asub->b;
    epicsInt32* levels = (epicsInt32 *)asub->b;

    LOG_INFO("setting log levels...");
    for (int i=0; i < asub->noa; ++i) {
        if (0 == strlen(loggerNames[i]) || 0 == levels[i]) {
            continue;
        }
        LOG_DEBUG("Trying to set %s to %d", loggerNames[i], levels[i]);
        log4cxx::LoggerPtr logger;
        if (0 == strcmp("root", loggerNames[i])) {
            logger = log4cxx::LogManager::getRootLogger();
        } else {
            logger = log4cxx::LogManager::getLogger(loggerNames[i]);
        }
        if (logger) {
            levels[i] *= 1000;
            if(levels[i] > log4cxx::Level::FATAL_INT) {
                levels[i] = log4cxx::Level::OFF_INT;
            }
            LOG_DEBUG("setting logger %s to level %d", loggerNames[i], levels[i]);
            logger->setLevel(log4cxx::Level::toLevel(levels[i]));
        }
    }
    return 0;
}

epicsRegisterFunction(logging_get_loggers);
epicsRegisterFunction(logging_set_log_levels);
