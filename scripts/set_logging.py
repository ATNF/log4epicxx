#!/usr/bin/env python

import os, sys
import epics
import re
import time

def show_loggers(names, levels):
    for x, name in enumerate(names):
        if len(name):
            print "%3d. %-40s : %s" % (x, name, levels[x])

def update_logging(regex, level):
    retest = re.compile('.*' + regex + '.*')
    for x, name in enumerate(names):
        if retest.match(name):
            levels[x] = LevelMap[level.lower()]

    pvLevels.put(levels, wait=True)

LevelMap = {'unset' : 0, 'off' : 1, 'fatal' : 2, 'error' : 3, 'warn' : 4, 'info' : 5, 'debug' : 6, 'trace' : 7}
ChangeLogger = None
ChangeLevel = None

Prefix = sys.argv[1]

if len(sys.argv) > 2:
    ChangeLogger = sys.argv[2]

if len(sys.argv) > 3:
    ChangeLevel = sys.argv[3]

retry = 0
while retry < 5:
    pvLevels = epics.PV(Prefix + "log:loggerLevels")
    levels = pvLevels.get(timeout=5)
    pvLevelNames = epics.PV(Prefix + "log:loggerLevelNames")
    levelNames = pvLevelNames.get(timeout=5)
    pvLoggerNames = epics.PV(Prefix + "log:loggerNames")
    names = pvLoggerNames.get(timeout=5)

    if len(names) == len(levels) and len(names) == len(levelNames):
        break

    retry += 1
    pvLevelNames.disconnect()
    pvLevels.disconnect()
    pvLoggerNames.disconnect()
    print "bad PV connection, trying again.."
    continue

if retry >= 10:
    raise RuntimeError("bad PV connection, giving up %d %d %d" % (len(names), len(levelNames), len(levels)))

#print len(levels), len(levelNames), len(names)

if None is ChangeLogger:
    cmd = ''
    while cmd != 'quit':
        os.system('clear')
        show_loggers(names, levelNames)
        cmd = raw_input('regex level\n>')
        (regex, level) = cmd.split(' ')
        update_logging(regex, level)
        levelNames = pvLevelNames.get(count=100, timeout=5)

    exit()

if None is ChangeLevel:
    ChangeLevel = 'unset'

update_logging(ChangeLogger, ChangeLevel)
levelNames = pvLevelNames.get(count=100, timeout=5)
show_loggers(names, levelNames)
