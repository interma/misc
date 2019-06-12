#!/bin/bash
set -x

gptext-stop
zkManager stop
gpstop -a
exit 0
