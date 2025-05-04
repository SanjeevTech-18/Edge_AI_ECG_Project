#ifndef __ECG_ANALYZER_H__
#define __ECG_ANALYZER_H__

/* Include Files */
#include <stddef.h>
#include <stdlib.h>
#include "string.h"
#include "stdint.h"
#include "stdbool.h"

#include "Arduino.h"



extern void ecg_analyzer(int rawECG);
extern void ECG_Filter_initialize(void);


extern int calculated_ecg_values[3];

#define filteredECGIdx 0
#define RRintervalIdx 1
#define PRintervalIdx 2
#endif
