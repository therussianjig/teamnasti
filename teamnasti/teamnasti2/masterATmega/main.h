#include <avr/io.h>
#include <avr/interrupt.h>
#include <inttypes.h>

#include "i2cmaster.h"
#include "serial.h"
#include "motorLookup.h"

#include "GVARS.h"
#include "init.h"
#include "ISR.h"
#include "pulseFuncs.h"
