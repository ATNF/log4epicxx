#include <stdio.h>

#include <dbDefs.h>
#include <registryFunction.h>
#include <subRecord.h>
#include <aSubRecord.h>
#include <epicsExport.h>

#include <Logging.h>

CREATE_LOGGER(".asubs");

static long mySubInit(subRecord *precord)
{
    LOG_DEBUG("Record %s called mySubInit(%p)",
           precord->name, (void*) precord);
    return 0;
}

static long mySubProcess(subRecord *precord)
{
    LOG_DEBUG("Record %s called mySubProcess(%p)",
           precord->name, (void*) precord);
    return 0;
}

static long myAsubInit(aSubRecord *precord)
{
    LOG_DEBUG("Record %s called myAsubInit(%p)",
           precord->name, (void*) precord);
    return 0;
}

static long myAsubProcess(aSubRecord *precord)
{   
    double *inp = precord->a;
    LOG_DEBUG("Record %s called myAsubProcess(%p)",
           precord->name, (void*) precord);
    LOG_INFO("val = %.1f", *inp);
    return 0;
}

/* Register these symbols for use by IOC code: */

epicsRegisterFunction(mySubInit);
epicsRegisterFunction(mySubProcess);
epicsRegisterFunction(myAsubInit);
epicsRegisterFunction(myAsubProcess);
