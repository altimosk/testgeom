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

