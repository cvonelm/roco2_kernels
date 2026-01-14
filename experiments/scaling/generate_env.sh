#!/usr/bin/env bash

set -euo pipefail

EXP_SRC_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd ) 
 
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

$KERNEL_LIST | xargs $EXOMAT env --add KERNEL
