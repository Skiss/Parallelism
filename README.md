Parallelism
===========

Video processing using Intel TBB (Threading Building Block).

### The project:
***

This is a school project whose objective is perform some treatments on videos, such as:

* Blur a video
* Swap content of a video with another
* Edge detection
* And more...

However, such treatments can take a very long time to perform, especially if the video is big.

This is why we want to use the power of multithreading.

### TBB:
***

TBB (Threading Building Block) is a library developped by Intel that allows to take advantage of the power
of multi-core processor performance without being an expert in multithreading.
It offers you a simple, clean, C++ oriented way to greatly improve the performances of your programs.

### How to use:
***

First you need a compiler that supports C++11 (g++ 4.6 at least, or clang 3.1).

To compile the project:

* cmake .
* make

To use it:

./prpa video1 [video2] options outputVideo

* video1 is a mandatory argument, it must a .avi formated video
* If your process requires a second video (for instance you want to swap content), then video2 is this extra video
* outputVideo is the name of the video that will be created by the program

For the options, if your process only requires 1 video:
* --blur
* --sharpen
* --edge
* --light
* --dark

Else, if you have a second video
* -swap : will swap content between the first and the second video
* -blend : will blend the 2 videos

Of course, you can have several options queued.
