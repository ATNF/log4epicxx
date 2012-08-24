from askapdev.rbuild.builders import Epics as Builder
import askapdev.rbuild.utils as utils

builder = Builder(pkgname='.', epicsbase='epicsbase')

# Each support module this application depends must be added to builder
# The builder will automatically add an entry in the auto-generated
# RELEASE.<arch> file
builder.add_support('l4cpp')
builder.add_support('ioclog')

builder.build()
