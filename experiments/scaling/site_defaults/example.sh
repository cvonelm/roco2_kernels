# Copy this file to site_defaults/${HOST}.sh
# and set the per-PC specific environment variables accordingly

# The name of the power metric to use
export METRICQ_POWER=
# The URL of the MetricQ instance
export METRICQ_URL=

# https://github.com/tud-zih-energy/lo2s
export LO2S=
# This repo
export KERNEL_RUNNER=
export KERNEL_LIST=
# https://github.com/cvonelm/penguinxx
export PENGUINXX_CPU_SET=
# https://github.com/cvonelm/exomat
export EXOMAT=

# This python needs access to the otf2 module
export PYTHON=

# There is a high chance you need to set ${LD_LIBRARY_PATH} so that
# scorep_plugin_metricq is found.
LD_LIBRARY_PATH="${LD_LIBRARY_PATH:-}"
export LD_LIBRARY_PATH=
