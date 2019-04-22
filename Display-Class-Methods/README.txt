EVO CLASSES

The NVIDIA "EVO" Display Engine was introduced in NV50, and has been
incrementally updated in GPUs since then.

EVO consists of several channels:

    "core": This channel is used to perform modesets, as well as things
            such as manage the LUT, and cursor properties (other than
            position).  It has GPU scope.
    "base": This channel is intended to be used for OpenGL SwapBuffers
            flipping.  There is one base channel per head.
    "overlay": This channel is intended to be used for flipping the
            overlay.  There is one overlay channel per head.
    "overlay immediate": This channel is intended to be used to position
            the overlay within the raster of the head.  There is one
            overlay immediate channel per head.  Originally, it was
            conceived that a display driver would position the overlay
            in response to a window move, using the "overlay immediate"
            channel, and a video driver would flip buffers using the
            overlay channel.
    "cursor": This channel is used to position the cursor.  There is
            one cursor channel per head.  The cursor format and buffer
            is specified through the core channel.  It was originally
            conceived that the management of cursor would be distributed
            between the core and cursor channels in the same way that
            overlay management is distributed between overlay and overlay
            immediate channels.  The cursor channel allows low-latency
            cursor position updates, asynchronously to the core channel.

There is a per-channel header file that defines the method interface to
each channel.

There are both an EVO "class name" and a software class number that are
used to describe the combination of channel versions used together.

The table below describes which EVO class name is used with which GPU,
and which channel header files are used with that EVO class.

__________________________________________________________________________

Class Name:                 DISP010X
Software Class Number:      5070
Cursor Channel:             cl507a.h
Overlay Immediate Channel:  cl507b.h
Base Channel:               cl507c.h
Core Channel:               cl507d.h
Overlay Channel:            cl507e.h
GPUs:                       nv50
__________________________________________________________________________

Class Name:                 DISP011X
Software Class Number:      8270
Cursor Channel:             cl827a.h
Overlay Immediate Channel:  cl827b.h
Base Channel:               cl827c.h
Core Channel:               cl827d.h
Overlay Channel:            cl827e.h
GPUs:                       g84, g86, g92
__________________________________________________________________________

Class Name:                 DISP012X
Software Class Number:      8370
Cursor Channel:             cl827a.h
Overlay Immediate Channel:  cl827b.h
Base Channel:               cl837c.h
Core Channel:               cl837d.h
Overlay Channel:            cl837e.h
GPUs:                       gt200
__________________________________________________________________________

Class Name:                 DISP014X
Software Class Number:      8870
Cursor Channel:             cl827a.h
Overlay Immediate Channel:  cl827b.h
Base Channel:               cl837c.h
Core Channel:               cl887d.h
Overlay Channel:            cl837e.h
GPUs:                       g94, g96, g98, mcp7x
__________________________________________________________________________

Class Name:                 DISP015X
Software Class Number:      8570
Cursor Channel:             cl857a.h
Overlay Immediate Channel:  cl857b.h
Base Channel:               cl857c.h
Core Channel:               cl857d.h
Overlay Channel:            cl857e.h
GPUs:                       gt215, gt216, gt218, mcp89
                            gf100, gf104, gf106, gf114, gf116, gf108
__________________________________________________________________________

Class Name:                 DISP020X
Software Class Number:      9070
Cursor Channel:             cl907a.h
Overlay Immediate Channel:  cl907b.h
Base Channel:               cl907c.h
Core Channel:               cl907d.h
Overlay Channel:            cl907e.h
GPUs:                       gf119
__________________________________________________________________________

Class Name:                 DISP021X
Software Class Number:      9170
Cursor Channel:             cl917a.h
Overlay Immediate Channel:  cl917b.h
Base Channel:               cl917c.h
Core Channel:               cl917d.h
Overlay Channel:            cl917e.h
GPUs:                       gk104, gk106, gk107
__________________________________________________________________________

Class Name:                 DISP022X
Software Class Number:      9270
Cursor Channel:             cl917a.h
Overlay Immediate Channel:  cl917b.h
Base Channel:               cl927c.h
Core Channel:               cl927d.h
Overlay Channel:            cl917e.h
GPUs:                       gk110, gk208
__________________________________________________________________________

Class Name:                 DISP024X
Software Class Number:      9470
Cursor Channel:             cl917a.h
Overlay Immediate Channel:  cl917b.h
Base Channel:               cl927c.h
Core Channel:               cl947d.h
Overlay Channel:            cl917e.h
GPUs:                       gm107
__________________________________________________________________________

Class Name:                 DISP025X
Software Class Number:      9570
Cursor Channel:             cl917a.h
Overlay Immediate Channel:  cl917b.h
Base Channel:               cl927c.h
Core Channel:               cl957d.h
Overlay Channel:            cl917e.h
GPUs:                       gm204
__________________________________________________________________________

