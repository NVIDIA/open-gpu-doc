3D CLASSES

The clxx97.h classes are used to program the 3D engine of the GPU.
The 3D engine class typically has the same name as the GPU architecture
(Ampere, Turing, etc).

The corresponding clxx97tex.h header file defines the Texture Header
and Texture Sampler layout.

The table below describes which class name is used with which GPU,
and which channel header files are used with that class.

Note that in some cases a GPU will support multiple classes (e.g., gf119
supports FERMI_A, FERMI_B, FERMI_C).  In general, it is recommended to
use the newest class a GPU supports.

__________________________________________________________________________

Class Name:                 FERMI_A
3D Class Header:            cl9097.h
3D Class Texture Header:    cl9097tex.h
GPUs:                       gf100, gf104, gf106, gf114, gf116, gf108, gf119
__________________________________________________________________________

Class Name:                 FERMI_B
3D Class Header:            cl9197.h
3D Class Texture Header:    cl9097tex.h (yes, this reuses 9097's tex.h)
GPUs:                       gf108, gf119
__________________________________________________________________________

Class Name:                 FERMI_C
3D Class Header:            cl9297.h
3D Class Texture Header:    cl9297tex.h
GPUs:                       gf119
__________________________________________________________________________

Class Name:                 KEPLER_A
3D Class Header:            cla097.h
3D Class Texture Header:    cla097tex.h
GPUs:                       gk104, gk106, gk107
__________________________________________________________________________

Class Name:                 KEPLER_B
3D Class Header:            cla197.h
3D Class Texture Header:    cla197tex.h
GPUs:                       gk110, gk208
__________________________________________________________________________

Class Name:                 KEPLER_C
3D Class Header:            cla297.h
3D Class Texture Header:    cla297tex.h
GPUs:                       t124, t132
__________________________________________________________________________

Class Name:                 MAXWELL_A
3D Class Header:            clb097.h
3D Class Texture Header:    clb097tex.h
GPUs:                       gm107, gm108
__________________________________________________________________________

Class Name:                 MAXWELL_B
3D Class Header:            clb197.h
3D Class Texture Header:    clb197tex.h
GPUs:                       gm200, gm204, gm206, t210
__________________________________________________________________________

Class Name:                 PASCAL_A
3D Class Header:            clc097.h
3D Class Texture Header:    clc097tex.h
GPUs:                       gp100, gp102, gp104, gp106, gp107, gp108, t186
__________________________________________________________________________

Class Name:                 PASCAL_B
3D Class Header:            clc197.h
3D Class Texture Header:    clc197tex.h
GPUs:                       gp102, gp104, gp106, gp107, gp108
__________________________________________________________________________

Class Name:                 VOLTA_A
3D Class Header:            clc397.h
3D Class Texture Header:    clc397tex.h
GPUs:                       gv100, t194
__________________________________________________________________________

Class Name:                 TURING_A
3D Class Header:            clc597.h
3D Class Texture Header:    clc597tex.h
GPUs:                       tu102, tu104, tu106, tu116, tu117
__________________________________________________________________________

Class Name:                 AMPERE_A
3D Class Header:            clc697.h
3D Class Texture Header:    clc697tex.h
GPUs:                       ga100
__________________________________________________________________________

Class Name:                 AMPERE_B
3D Class Header:            clc797.h
3D Class Texture Header:    clc797tex.h
GPUs:                       ga102, ga103, ga104, ga106, ga107, t234, t239
__________________________________________________________________________

Class Name:                 ADA_A
3D Class Header:            clc997.h
3D Class Texture Header:    clc997tex.h
GPUs:                       ad102, ad103, ad104
__________________________________________________________________________

Class Name:                 HOPPER_A
3D Class Header:            clcb97.h
3D Class Texture Header:    clcb97tex.h
GPUs:                       gh100
__________________________________________________________________________

Class Name:                 BLACKWELL_A
3D Class Header:            clcd97.h
3D Class Texture Header:    clcd97tex.h
GPUs:                       gb100, gb102, gb10b, gb110, gb112
__________________________________________________________________________

Class Name:                 BLACKWELL_B
3D Class Header:            clce97.h
3D Class Texture Header:    clce97tex.h
GPUs:                       gb202, gb203, gb205, gb206, gb207, gb20b

