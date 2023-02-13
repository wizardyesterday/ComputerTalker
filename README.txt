Years ago, there was an article in the Circuit Cellar column of Byte
Magazine that discussed converting text to speech using sound rules.
This device was called the MicroVox.  Part 1 (the hardware) was described
in September 1982, and part 2 (the software) was described in October 1982.
I had decided to build a synthesizer and write software in Pascal/MT+
 (and some Z80 assembly language) on a CP/M machine.  I had actually built
the hardware and wrote the software probably in 1984.

This week, I decided to port the code to C++.  Rather than connect the
speech synthesizer box to the parallel port of one of my Linux systems,
I decided to make the synthesizer software-based.

There will be two C++ classes:
1. PhonemeMaker - Converts text to a phoneme stream.
2. SpeechSynthesizer - Plays the phoneme stream.

If I find PCM files for the phonemes, the speech synthesizer part will be
fairly easy.  As of right now, the PhonemMaker works, although there is a
segfault now and then.

To build this app type:
  sh buildit.sh.

The binary will be in the current directory, and you launch it by typing:
  ./talker > someAudioFile.raw,
or
  ./talker | aplay -f S16_LE -r 16000,
to listen live.  Add a bunch of extra spaces to your line of text to
deal with the pipeline of ALSA.

If you want input from a file, you type:
  ./talker < textFileName.

--------------------------------------------------------------------
01/21/2023
--------------------------------------------------------------------
We now have a functioning system.  The phoneme files really need
to be replaced though.  Their duration is excessive to the point
that speech sounds really slow.  With a stretch of the imagination,
you can hear some words.  I'll have to capture the utterings of my
speech synthesizer box.  The BeagleBone Black has enough I/O pins
so that I can operate the synthesizer box.  I'm not sure if I should
just capture the audio with a microphone or hardwire the audio
output to a sound card.  I'd hate to blow up the synthesizer box,
though.

--------------------------------------------------------------------
01/22/2023
--------------------------------------------------------------------
Today, I decided to create my own phonemes with the aid of
festital, using their text2wave script to create the example
words from the Votrax SC-01 data sheet.  I was then able to select
the speech samples of interest, using Audacity.  The sample rate of
these raw PCM files is 16000S/s.  The format of the PCM samples is,
16-bit signed little endian.  The speech is more intelligible than
that obtained from the original PCM files, but it still could use
some improvement.

--------------------------------------------------------------------
01/27/2023
--------------------------------------------------------------------
Today, I automated creation of the speech files associated with
each example word in the Votrax SC-01 datasheet.  These words
illustrate how a phoneme sounds when spoken.  The PCM files for
the example words are in the research/speechFiles directory.
Additionally, the festivalScripts directory has been moved
to the research directory.

--------------------------------------------------------------------
02/01/2023
--------------------------------------------------------------------
1. File names have been changed, and content has been fixed so that
phoneme is properly spelled.

2. The phoneme table is now implemented as a map.


--------------------------------------------------------------------
02/06/2023
--------------------------------------------------------------------
1. The phoneme.txt file has been removed.  It isn't really
necessary since this system already knows that Votrax SC-01
phnemes are being generated.

2. Rules are being approximately validated when reading in the
rules file.  Validation is nothing fancy, but enough checks are
being made to avoid buffer overflows.

