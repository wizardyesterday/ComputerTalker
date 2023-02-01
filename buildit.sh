#!/bin/sh

# Build the text to speech system.
g++ -I include -o talker src/talker.cc src/PhonemeMaker.cc src/SpeechSynthesizer.cc

# Build the speech synthesizer test system.
g++ -I include -o testSynthesizer src/testSynthesizer.cc src/SpeechSynthesizer.cc

