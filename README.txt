INTRODUCTION
------------

The Concurrent UNIX Processes and Shared Memory m
odule will test a number given as its command lin
e argument to check whether the number is a prime
number. The code will run as a child process to a
nother controller process. You will create two ex
ecutable files. The first executable file, oss, w
ill be in charge of launching a specific number o
f child processes at various times using a fork()
followed by an exec(). oss should then keep track
of how many children have finished executing and 
terminate itself only when all of its children ha
ve finished.

 * For a full description of the module, visit th
   e project page of CS4760: Operating Systems at:
   http://www.cs.umsl.edu/~sanjiv/classes/cs4760/

 * To submit bug reports and feature suggestions o
   r track changes visit:
   https://github.com/Keenandrea?tab=repositories


REQUIREMENTS
------------

This module requires the following:

 * Hoare Linux (http://www.umsl.edu/technology/tsc/)
 * UMSL ID (https://www.umsl.edu/degrees/index.html)


INSTALLATION
------------

 * Install as you would normally install a distrib
   uted C module. To compile and execute, you must
   have the build-essential packages properly inst
   alled on your system. 


COMPILATION
-----------

To compile, open a new CLI window, change the the
directory nesting your module. Type:

 * make


EXECUTION
---------
																								 
Find the executable named oss, located inside the
directory in which you compiled the module. After
you have found oss, invoke the program with:

 * ./oss [-n x] [-s x] [-b B] [-i I] [-o filename]
 
 * The options are to be interpreted as follows:
 
  * h Describe how the project should be run and then terminate.											
  * n x Indicate the maximum total number of child processes [x] 
        oss will ever create. Defaults [x] to 4.
  * s x Indicate the number of children [x] allowed to exist in 
        the system at the same time. Defaults [x] to 2.
  * b B Start of the sequence of numbers [B] to be tested for p
        rimality.												
  * i I Increment [I] between numbers that we test. 
  * o filename Output file [filename].


MAINTAINERS
-----------

Current maintainers:
 * Keenan Andrea - https://github.com/Keenandrea

This project has been sponsored by:
 * UMSL Mathematics and Computer Science 
   Specialized in bringing young minds to graduat
   ion with the skills needed. 