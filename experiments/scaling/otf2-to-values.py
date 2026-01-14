#!/usr/bin/env python3

# ./otf2-to-values.py [TRACE_DIR]
#
# From the trace dir given as [TRACEDIR].
#
# And for the metric given by env["METRICQ_POWER"]
#
# Write out the timestamps and values of the metric to
# out_timestamp and out_power, respectively

import otf2
import csv
import os
import sys

offset = 0

ts_file = open("out_timestamp", "w")
power_file = open("out_power", "w")

power_sensor = os.environ["METRICQ_POWER"]

with otf2.reader.open(sys.argv[1] +  "/traces.otf2") as trace:
    for location, event in trace.events:
        if location.name == power_sensor:
            if offset == 0:
                offset = event.time
            ts = event.time - offset
            ts_file.write(str(ts) +"\n")
            power_file.write(str(event.values[0]) + "\n")
