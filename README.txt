Years ago, there was an article in the Circuit Cellar column of Byte
Magazine that discussed converting text to speech using sound rules.  This
may have been back in 1984. I had decided to build a synthesizer and write
softwware in Pascal/MT+ (and some Z80 assembly language) on a CP/M machine.

This week, I decided to port the code to C++.  Rather than connect the
speech synthesizer box to the parallel port of one of my Linux systems,
I decided to make the synthesizer software-based.

There will be two C++ classes:
1. PhonemMaker - Converts text to a phonem stream.
2. SpeechSynthesizer - Plays the phonem stream.

If I find PCM files for the phonems, the speech synthesizer part will be
fairly easy.  As of right now, the PhonemMaker works, although there is a
segfault now and then.

To build this app type:
  sh buildit.sh.

The binary will be in the current directory, and you launch it by typing:
  ./talker.

If you want input from a file, you type:
  ./talker < textFileName.




