ORDO
An visual story about the endless cycle of ORDER and CHAOS, for the Commodore Amiga

![ordo-screenshot](https://github.com/steffest/ordo/assets/763047/e39a486e-a467-41b1-9d84-1fd6d75bbfc9)

![ordo-screenshot-2](https://github.com/steffest/ordo/assets/763047/47120acd-61e6-4545-9083-9ec681b665e4)

Source code in C

It runs (or should run) on any Amiga. it's fairly system safe.

This is my first demo for the Amiga. 
I know the source is a bit crap - I don't know donkey balls about C and I kind of assumed I could get the hang out of it just diving in.
This production is the result of banging my head to walls and in the end (literaly HOURS before the compo deadline) settling for the only stuff that I could get working: 
bliiter baby!
So it's mostly a glorified slideshow.

Some starting princples
- let's (first) stick to C, no Assembly yet. Yes, I know real demo-coders use assembly, but I feel "learning C" is a skill that comes in handy for other platforms as well, and C code runs everywhere.
- I had this idea to create "system friendly" demos, that work on any machine - even AROS and Morphos.
When more horsepower is available the "effects" get fanciers, the audio will be of better quality etc ...
That's the idea anyway. We're not there yet, but it's a decent attempt.
- No Cross Compiling. I like the lightweihgt approach of directly compiling on the Amiga - far easier to test and it's lightning fast on those super accelerated devices we have these days. 

Stuff that I figured out
- opening windows and screens
- loading externtal files in a ssytem friendly and robust way
- freeing memory if possible
- loading bitmap data into memory and blitting (parts of them) on the screen
- setting colors/palettes
- Playing tracker modules from file or memory (using and external library)

And all that without crashing the host.

Things I havent figured out yet
- the copper ... can't wrap my heaad around it
- scrolling windows. Should be easy, right?
- any kind of "graphical performance" of the code
- keeping the audio and screen in sync ...

Things I haven't investigated yet
- dual playfield
- 3D stuff

How to build:
- compile with SasC on the Amiga
 
  All graphics were produces with my own editor: dpaint.JS
  Music was made using my own Tracker: bassoontracker



