#!/bin/sh

#_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
# This script creates a wav file from the phoneme files, EH3,
# EH2, etc.  The contents of a phoneme file is a textual word.
# The actual conversion from text to phonemes is performed by the
# text2wave script.
# Note: This script must reside in a subdirectory of the
# festival/bin directory.
#_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/

../text2wave < $1 > $1.wav

exit 0
