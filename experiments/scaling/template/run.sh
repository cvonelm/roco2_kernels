#!/usr/bin/env bash
set -euo pipefail

# Load site site_defaults
if [[ ! -f $EXP_SRC_DIR/site_defaults/$(hostname).sh ]]
then
    echo "per-site configuration does not exist!"
    echo "please copy"
    echo "  $EXP_SRC_DIR/site_defaults/example.sh"
    echo "to"
    echo "  $EXP_SRC_DIR/site_defaults/$(hostname).sh"
    echo "And set them accordingly!"
    exit 1
fi

source $EXP_SRC_DIR/site_defaults/$(hostname).sh

export SCOREP_METRIC_PLUGINS="metricq_plugin"
export SCOREP_METRIC_METRICQ_PLUGIN=$METRICQ_POWER
export SCOREP_METRIC_METRICQ_PLUGIN_SERVER=$METRICQ_URL

$LO2S --no-process-recording -a -o out_trace &

MEASUREMENT_PID=$!

export OPENBLAS_NUM_THREADS=1
$KERNEL_RUNNER $KERNEL $(${PENGUINXX_CPU_SET} all) 20

kill -s INT $MEASUREMENT_PID

wait $MEASUREMENT_PID

$PYTHON $EXP_SRC_DIR/otf2-to-values.py $PWD/out_trace

rm -r out_trace
