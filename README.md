# ORDO
A visual story about the endless cycle of ORDER and CHAOS, for the Commodore Amiga

![ordo-screenshot](https://github.com/steffest/ordo/assets/763047/e39a486e-a467-41b1-9d84-1fd6d75bbfc9)

![ordo-screenshot-2](https://github.com/steffest/ordo/assets/763047/47120acd-61e6-4545-9083-9ec681b665e4)

Source code in C

It runs (or should run) on any Amiga. it's fairly system safe.
Video version is [here](https://www.youtube.com/watch?v=6vdG_Bejn5g)

This is my first demo for the Amiga. 
I know the source is a bit crap - I don't know donkey balls about C and I kind of assumed I could get the hang out of it just diving in.
This production is the result of banging my head to walls and in the end (literaly HOURS before the compo deadline) settling for the only stuff that I could get working: 
blitter, baby!
So it's mostly a glorified slideshow.

## Some starting principles
- Let's stick to C, no assembly yet. Yes, I know real demo-coders use assembly, but I feel "learning C" is a skill that comes in handy for other platforms as well, and C code runs everywhere.
- I had this idea to create "system friendly" demos, that work on any machine - even AROS and Morphos.
When more horsepower is available the "effects" get fanciers, the audio will be of better quality etc ...
That's the idea anyway. We're not there yet, but it's a decent attempt.
- No Cross Compiling. I like the lightweight approach of directly compiling on the Amiga - far easier to test and it's lightning fast on those super accelerated devices we have these days. 

## Stuff that I figured out
- Opening windows and screens
- Loading external files in a sytem friendly and robust way
- Freeing memory if possible
- Loading bitmap data into memory and blitting (parts of them) on the screen
- Setting colors/palettes
- Playing tracker modules from file or memory (using and external library)

And all that without crashing the host.

## Things I havent figured out yet
- The copper ... can't wrap my head around it
- Scrolling windows. Should be easy, right?
- Any kind of "graphical performance" of the code
- Keeping the audio and screen in sync ...
- how to integrate a decompressor: data files are uncompressed currently, hence the huge size and long loading time.

## Things I haven't investigated yet
- dual playfield
- 3D stuff

## How to build:
- compile with SasC on the Amiga
 
  All graphics were produced with my own editor: [Dpaint.js](https://www.stef.be/dpaint/).
  The .json files in the assets folder can be opened with it.
  It has an bitplane exporter that generates the .planes files in the data folder.
  
  Music was made using my own tracker: [Bassoontracker](https://www.stef.be/bassoontracker/), it's a standard Amiga .mod file.
  Samples were sampled from "Vespers" by Bola.



