#!/usr/bin/env python
# Copyright (c) 2013 CSIRO
# Australia Telescope National Facility (ATNF)
# Commonwealth Scientific and Industrial Research Organisation (CSIRO)
# PO Box 76, Epping NSW 1710, Australia
# atnf-enquiries@csiro.au
#
# This file is part of the ASKAP software distribution.
#
# The ASKAP software distribution is free software: you can redistribute it
# and/or modify it under the terms of the GNU General Public License as
# published by the Free Software Foundation; either version 2 of the License
# or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA.
#
import os, sys
import epics
import re
import time

Levels = ['unset', 'off', 'fatal', 'error', 'warn', 'info', 'debug', 'trace']
LevelMap = { Levels[x] : x for x in xrange(0, len(Levels)) }
ChangeLogger = None
NewLevel = None

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

# get parameters
Prefix = sys.argv[1]
if len(sys.argv) > 2:
    ChangeLogger = sys.argv[2]

if len(sys.argv) > 3:
    NewLevel = sys.argv[3]

# read epics logging PVs
pvLevels = epics.PV(Prefix + ":loggerLevels")
pvLevelNames = epics.PV(Prefix + ":loggerLevelNames")
pvLoggerNames = epics.PV(Prefix + ":loggerNames")
levels = pvLevels.get(timeout=5, use_monitor=False)
levelNames = pvLevelNames.get(timeout=5, use_monitor=False)
names = pvLoggerNames.get(timeout=5, use_monitor=False)

if ChangeLogger is not None:
    # update logger level directly
    if None is NewLevel:
        NewLevel = 'unset'
    update_logging(ChangeLogger, NewLevel)
    levelNames = pvLevelNames.get(count=100, timeout=5)
    show_loggers(names, levelNames)

else:
    # display a menu
    cmd = ''
    while cmd != 'quit':
        os.system('clear')
        show_loggers(names, levelNames)
        cmd = raw_input('regex level (%s)\n>' % ','.join(Levels))
        try:
            (regex, level) = cmd.split(' ')
        except ValueError:
            continue
            
        update_logging(regex, level)
        levelNames = pvLevelNames.get(count=100, timeout=5)
