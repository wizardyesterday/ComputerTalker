#! /bin/sh
# /_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
# This script converts a raw PCM file to a wav file.
# The PCM data has the format:
#
# 1. 16 bits
# 2. 2's complement (signed)
# 3. Little endian
# 4. Sample rate: 16000 S/s
#
# Note that my version of sox (an old version has
# different command line arguments when compared
# to newer versions of sox (why do they do this?!!!).
# /_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/

sox -t raw -s -w -r 16000 $1 $2

exit 
