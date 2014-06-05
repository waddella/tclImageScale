# On Linux compile with
# gcc -shared -o imgscale.so -I/usr/include/tcl imgscale.c -DUSE_TCL_STUBS -DUSE_TK_STUBS -ltclstub8.6 -ltkstub8.6 -fPIC

# On OSX
# gcc -shared -o imgscale.dylib -framework tcl  imgscale.c -DUSE_TCL_STUBS -DUSE_TK_STUBS -ltclstub8.6 -ltkstub8.6
# tclsh8.6 test.tcl

load [file join imgscale[info sharedlibextension]]

set imgFrom [image create photo -file Tux.png]
set imgTo [image create photo]

image_scale $imgFrom 60 40 $imgTo
canvas .c
pack .c -fill both -expand TRUE

.c create image 200 200 -image $imgFrom
.c create image 600 200 -image $imgTo




