tclImageScale
=============

This package is still work in progress. Currently executing the
`image_scale` procedure causes a *Segmentation fault (core dump)*
error.

This extension should allow one to scale tcl images with C (fast) and
packed with [TEA](http://wiki.tcl.tk/327) so that it compiles easily
under Windows, OS X, and Linux.

The image scaling code is taken from tcl.tk wiki:
[Image scaling in C](http://wiki.tcl.tk/25685). The code works, see
the `original`. The compile instructions are in the `imgscale.c` file.


I used the `sampleextension` as a base to make a `TEA` package.

~~~
git clone https://github.com/tcltk/sampleextension.git
git clone https://github.com/tcltk/tclconfig.git
~~~

The compilation instructions for the `Imagescale` package are

~~~
./configure
make
~~~

And to test whether the `C` extension is working run

~~~
make test
~~~

If you modify `configure.in` you need to recreate the `configure` file
with

~~~
autoconf
~~~

When modifying the `Makefile.in` the `./configure` command creates a
new `Makefile`.
