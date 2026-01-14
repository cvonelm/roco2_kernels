# Scaling experiment

This is an [exomat](https://github.com/tud-zih-energy/exomat) experiment.

## Experiment Description

This experiment runs the kernels of roco2_kernels for 20 seconds,
recording the power usage during the run using [lo2s](https://github.com/tud-zih-energy/lo2s) and [MetricQ](https://github.com/metricq/metricq).

The recorded data can then be used to judge the behaviour over time
of roco2 and thus adjust the runtime of roco2_kernels in other
experiment to yield stable results.

## Experiment Preparation

### Dependencies

**exomat** https://github.com/tud-zih-energy/exomat For running the experiment
  Specifically at the time of writing the "make_table_multiline" branch

**lo2s** https://github.com/tud-zih-energy/lo2s
  For recording the measurement data

**scorep_plugin_metricq** https://github.com/score-p/scorep_plugin_metricq
  For recording MetricQ data into lo2s

**Python with the otf2 module**
  For extracting power data from the lo2s recorded data.

**penguinxx** https://github.com/cvonelm/penguinxx
  For generating the cpu set
  
### Site defaults

The experiment will try to load the `site_defaults/$(hostname).sh`
settings file for your system.

Copy the `site_defaults/example.sh` to `site_defaults/$(hostname).sh` and
give the paths to the binaries accordingly.

Take special note of the LD_LIBRARY_PATH. If you have installed scorep_plugin_metricq
outside of `/usr/lib`, you probably have to set this to the path to the metricq plugin.

If the experiment environment is out of date (Because new kernels were added), 
you can regenerate it with `./generate_envs.sh`

## Running the experiment

The experiment can be run as

```
# From roco2_kernels/experiments
$ exomat run scaling -r 5
```

This runs every kernel 5 times.

## Collecting experiment results

Running the experiment, whether succesfully or unsuccesfully,
creates a result folder named:

`scaling-YYYY-MM-DD--HH-mm-ss`

If any error is encountered during the experiment run, then inspecting
the `scaling-YYYY-MM-DD--HH-mm-ss/runs/{stderr, stdout}.log` for any abnormal output might be worthwile.

In the case the experiment finishes succesfully

```
$ cd scaling-YYYY-MM-DD--HH-mm-ss
$ exomat make-table 
```

creates a CSV file scaling-YYYY-MM-DD--HH-mm-ss.csv that can be processed further.

This CSV contains 4 columns:

- `power` (in W) 
    Power consumption as reported by the MetricQ metric.
- `timestamp` (in ns) 
    The timestamps are anchored at the first power measurement.
    So the first power reading gets the timestamp 0, the next is then given
    as the offset to the first timestamp, the next as the offset to
    the first timestamp etc.
- `REPETITION`
    Which repetition of the same experiment configuration this is.
- `KERNEL`
    the name of the roco2_kernels kernel executed in this experiment run.

### Visualization

There is a visualization script, `graphics.R`.

```
$  Rscript graphics.R path_to.csv
```

This plots the power reading as a scatterplot, time on the X-Axis, power on the Y-Axis and kernels put in different colors.
