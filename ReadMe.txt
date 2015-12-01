This is an exercise to get familiar with WxWidgets and Boost.test (and Git, for that matter).

The point of this exercise is to create an aplication which offers GDI graphics context to a client and calls client functions which presumably will draw in this context.

The application is WxWidgets-based. It implements a certain GDI-style drawing interface as a wrapper around a device context.
A wrapper object is created around a specific DC and passed to the client, which thus gets the ability to draw to this DC.

The client is supposed to be implemented as a dll, to be user-selected and loaded at run time. 
Client provides a function to accept the graphics wrapper, which it stores and uses for its drawing needs.
Client does not know anything about WxWidgets, it just gets the set of functions to be used for drawing.


The set of available client functions to be called by application is implemented as the Boost unit test tree. The application is Boot.test-aware, so if the client contains any Boost unit tests, they will be available for execution from the application's menu.

Experimental clients are included.

THe solutions for VC++ Express 2015 are provided. 
They expect WxWidgets to be extracted to drive w: and libraries built in W:\lib\vc_lib.
Boost is supposed to be extracted to u: and libraries built in U:\stage\lib.

A bit more detail on getting this to work:
- wxWidgets:
Downloaded WxWidgets 3.0.2 from https://www.wxwidgets.org/downloads/ (Windows Installer (47 MB))
To build it, had to change 1 line in src\tiff\libtiff\tif_config.h:
#define snprintf _snprintf -> #define wx_snprintf _snprintf
After that, loading build\msw\wx.dsw (with migration) and building all configurations worked
Did subst w: C:\wxWidgets-3.0.2
- boost test:
Downloaded boost_1_59_0.zip from http://sourceforge.net/projects/boost/?source=typ_redirect
Did subst u: C:\boost_1_59_0
in cmd, went to u:\, then
bootstrap.bat
b2 address-model=32 architecture=x86 --with-test link=static
b2 address-model=32 architecture=x86 --with-test link=shared
Added U:\stage\lib to path
- built BoostTest and WxGenGraph without issues
Run WxGenGraph, and in its menu do 
Init Client (browse to the dll built for BoostTest), then 
Select Test (double-click to select), then 
Run Test(s)


