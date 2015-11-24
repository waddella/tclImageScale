tclImageScale
=============

This `tcl` extension scales/resizes `tcl` images with a `C` routine
that is quite fast. The package follows the
[TEA](http://wiki.tcl.tk/327) recommendation, so that it should
compile easily under Windows, OS X, and Linux.

The image scaling code is taken from the tcl.tk wiki:
[Image scaling in C](http://wiki.tcl.tk/25685).


### Installation OSX and Linux


Open the terminal of your choice and download the repository

~~~
git clone https://github.com/waddella/tclImageScale.git
~~~

Change your working directory into the package folder

~~~
cd tclImageScale
~~~

and run

~~~
./configure
make
~~~

If you get an error then you probably have the wrong default `Tcl`
version (e.g. 8.4, look for` wish8.4`). The `ImageScale` extension
requires `Tcl` and `Tk` version 8.5 or higher. If the `./configure`
picks up a `Tcl` build with a lower version then one needs to specify
the path to the `Tcl` and `Tk` `tkConfig.sh` and `tclConfig.sh` files
(for version 8.5 or 8.6) explicitly. Use `locate tkConfig.sh` to find
the correct path. For example, in OS X the following configure call
might be necessary

~~~
./configure --with-tcl=/usr/local/lib/ --with-tk=/usr/local/lib/
make
~~~

You can install the package with

~~~
sudo make install
~~~

which copies the package to somewhere where `tclsh` will find the
package.

### Installation Windows

If you would like to compile the extension with VC++ see the
instructions in the `makefile.vc` file in the `win` directory. This is
currently untested.

### Load the Package

In the `tcl` console of your choice (I recommend `tkcon`) run

~~~{.tcl}
package require ImageScale
~~~

which will return the package version.


### Use

The `ImageScale` package provides the `image_scale` procedure in the
`imagescale` namespace which takes the following 5 arguments

* `srcimg` A photo object created with "image create photo"
* `newwidth` The new width of the image in pixels
* `newheight` The new height of the image in pixels
* `destimage` Name for the resulting image
* `alpha` Alpha to apply to the image (1 - opaque, 0-clear)

See the `image_scale.test` file in the `tests` folder.


### More on TEA

I used the `sampleextension` as a base to make this `TEA` package. See

~~~
git clone https://github.com/tcltk/sampleextension.git
git clone https://github.com/tcltk/tclconfig.git
~~~
