How to use:

Step 1:

The following API requires openCV. Follow the instructions provided on openCV will guide you through the installation on
windows.

http://docs.opencv.org/doc/tutorials/introduction/windows_install/windows_install.html#windows-installation

Step 2:

Once installed you can start a new Visual Studio Project. Create the necessary property files and use them to link the library.

http://docs.opencv.org/doc/tutorials/introduction/windows_visual_studio_Opencv/windows_visual_studio_Opencv.html#windows-visual-studio-how-to

Step 3:

Once the project is created you need to copy the following source and header files into your project folder & project explorer to
compile and run the demo application.

Note: Files marked with (*) are the required files if you want to use the API without the demo application.

Source
------------------------
AdaptiveMedianBGS.cpp (*)
DPAdaptiveMedianBGS.cpp (*)
FrameDifferenceBGS.cpp (*)
Image.cpp (*)

sqlite3.c (*)

main.cpp
proxy.cpp
runWrapper.cpp
carCounter.cpp (*)

Header
------------------------
AdaptiveMedianBGS.h (*)
DPAdaptiveMedianBGS.h (*)
FrameDifferenceBGS.h (*)
Image.h (*)

sqlite3.h (*)

main.h
proxy.h 
runWrapper.h
carCounter.h (*)

Bgs.h (*)
BgsParams.h (*)
IBGS.h (*)

Extras 
(Put in the project folder)
------------------------
DPAdaptiveMedianBGS.xml
FrameDifferenceBGS.xml