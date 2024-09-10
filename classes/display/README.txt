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

The "NVDisplay" Engine is new in Volta, and is a major improvement upon EVO.
The software interface consists of a set of channels as in EVO, but how those
channels can be used is more flexible.

    "core": This channel is similar to EVO's core channel, except it does not
            support programming a surface as a base layer.
    "window": This channel replaces EVO's base and overlay channels.  It is
            designed support a superset of the capabilities of the earlier
            channels, but in addition supports scaling, alpha blending, and
            blend order, among other improvements.  Windows are not inherently
            bound to a head and must be bound explicitly with core channel
            methods.
    "window immediate": Like EVO's overlay immediate, this can be used to
            position the corresponding window channel on the raster.  Unlike
            EVO's overlay immediate, this is a DMA channel to support
            enqueuing more than one update at a time.
    "cursor": This is similar to EVO's cursor channel.  There is one cursor
            per head.

There is a per-channel header file that defines the method interface to
each channel.

There are both a "class name" and a software class number that are
used to describe the combination of channel versions used together.

Also, many GPUs have a "DISP_SF_USER" class.  This software construct
exposes a mappable region of the Serial Frontend (SF) registers which
can be used to program infoframe data.

The table below describes which class name is used with which GPU,
and which channel header files are used with that class.

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
CRC notifier definitions:   cl907dcrcnotif.h
GPUs:                       gf119
__________________________________________________________________________

Class Name:                 DISP021X
Software Class Number:      9170
Cursor Channel:             cl917a.h
Overlay Immediate Channel:  cl917b.h
Base Channel:               cl917c.h
Core Channel:               cl917d.h
Overlay Channel:            cl917e.h
CRC notifier definitions:   cl917dcrcnotif.h
DISP_SF_USER:               cl9171.h
GPUs:                       gk104, gk106, gk107
__________________________________________________________________________

Class Name:                 DISP022X
Software Class Number:      9270
Cursor Channel:             cl917a.h
Overlay Immediate Channel:  cl917b.h
Base Channel:               cl927c.h
Core Channel:               cl927d.h
Overlay Channel:            cl917e.h
CRC notifier definitions:   cl927dcrcnotif.h
DISP_SF_USER:               cl9271.h
GPUs:                       gk110, gk208
__________________________________________________________________________

Class Name:                 DISP024X
Software Class Number:      9470
Cursor Channel:             cl917a.h
Overlay Immediate Channel:  cl917b.h
Base Channel:               cl927c.h
Core Channel:               cl947d.h
Overlay Channel:            cl917e.h
CRC notifier definitions:   cl927dcrcnotif.h
DISP_SF_USER:               cl9471.h
GPUs:                       gm107, gm108
__________________________________________________________________________

Class Name:                 DISP025X
Software Class Number:      9570
Cursor Channel:             cl917a.h
Overlay Immediate Channel:  cl917b.h
Base Channel:               cl927c.h
Core Channel:               cl957d.h
Overlay Channel:            cl917e.h
CRC notifier definitions:   cl927dcrcnotif.h
DISP_SF_USER:               cl9571.h
GPUs:                       gm200, gm204, gm206
__________________________________________________________________________

Class Name:                 DISP027X
Software Class Number:      9770
Cursor Channel:             cl917a.h
Overlay Immediate Channel:  cl917b.h
Base Channel:               cl927c.h
Core Channel:               cl977d.h
Overlay Channel:            cl917e.h
CRC notifier definitions:   cl927dcrcnotif.h
DISP_SF_USER:               cl9571.h
GPUs:                       gp100
__________________________________________________________________________

Class Name:                 DISP028X
Software Class Number:      9870
Cursor Channel:             cl917a.h
Overlay Immediate Channel:  cl917b.h
Base Channel:               cl927c.h
Core Channel:               cl987d.h
Overlay Channel:            cl917e.h
CRC notifier definitions:   cl927dcrcnotif.h
DISP_SF_USER:               cl9571.h
GPUs:                       gp102, gp104, gp106, gp107, gp108
__________________________________________________________________________

Class Name:                 NVD_20
Software Class Number:      C370
Cursor Channel:             clc37a.h
Window Immediate Channel:   clc37b.h
Core Channel:               clc37d.h
Window Channel:             clc37e.h
CRC notifier definitions:   clc37dcrcnotif.h
DISP_SF_USER:               clc371.h
GPUs:                       gv100
__________________________________________________________________________

Class Name:                 NVD_30
Software Class Number:      C570
Cursor Channel:             clc57a.h
Window Immediate Channel:   clc57b.h
Core Channel:               clc57d.h
Window Channel:             clc57e.h
CRC notifier definitions:   clc37dcrcnotif.h
DISP_SF_USER:               clc571.h
GPUs:                       tu102, tu104, tu106, tu116, tu117
__________________________________________________________________________

Class Name:                 NVD_40
Software Class Number:      C670
Cursor Channel:             clc67a.h
Window Immediate Channel:   clc67b.h
Core Channel:               clc67d.h
Window Channel:             clc67e.h
CRC notifier definitions:   clc37dcrcnotif.h
DISP_SF_USER:               clc671.h
GPUs:                       ga102,ga103,ga104,ga106,ga107
__________________________________________________________________________

Class Name:                 NVD_40
Software Class Number:      C770
Cursor Channel:             clc67a.h
Window Immediate Channel:   clc67b.h
Core Channel:               clc77d.h
Window Channel:             clc67e.h
CRC notifier definitions:   clc37dcrcnotif.h
DISP_SF_USER:               clc771.h
GPUs:                       ad102,ad103,ad104,ad106,ad107
__________________________________________________________________________

