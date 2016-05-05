DESCRIPTION
The downloadable code sample demonstrates the basics of acquiring raw camera streams from Intel® RealSense™ cameras (R200 and F200) in the MATLAB® workspace using the Intel® RealSense™ SDK for Windows* and MATLAB’s Image Acquisition Toolbox™ Adaptor Kit. This code sample creates possibilities for MATLAB developers to develop Intel® RealSense™ applications for Intel® platforms and has the following features:
 - Multi-stream synchronization. Color stream and depth stream can be acquired simultaneously.
 - Multi-camera support. Raw streams can be acquired from multiple cameras simultaneously.
 - User adjustable properties. This adaptor supports video input with different camera-specific properties.

NOTE
To set up the code sample:
 - The MATLAB installation path I used was C:\MATLAB and the SDK installation path was C:\Program Files (x86)\Intel\RSSDK. Note that the include directories and library directories will need to be changed if your SDK and MATLAB installation paths are different. 
 - The file location I use to put the entire code sample RealSenseImaq was C:\My_Adaptor\RealSenseImaq. 
 - You will also need to set an environment variable MATLAB in system variables that contains the name of your MATLAB installation folder.

To build the DLL from this code sample:
 - First run Microsoft Visual Studio* as administrator and open the RealSenseImaq solution. You must ensure that “x64” is specified under the platform setting in the project properties. 
 - To build this code sample, right-click the project name RealSenseImaq in the solution explorer, then select it as the startup project from the menu option and build it.
 - For users who are MATLAB developers and not interested in the source code, pre-build DLL can be found in the C:\My_Adaptor\RealSenseImaq\x64\Debug\folder. Note that the DLL directory will need to be changed if you put the code sample in a different location.

To register the DLL in the MATLAB:
 - You must inform the image acquisition toolbox software of DLL’s existence by registering it with the Imaqregister function. The DLL can be registered by using the following MATLAB code:
Imaqregister (‘<your_directory>\RealSenseImaq.dll’);
 - Start MATLAB and call the imaqhwinfo function. You should be able to see the RealSenseImaq adaptor included in the adaptors listed in the InstalledAdaptors field. 

To run the DLL in the MATLAB:
Three MATLAB scripts that I created have been put under the code sample directory C:\My_Adaptor\RealSenseImaq\matlab. 
To start to run the DLL in MATLAB, use the scripts as follows:
 - MATLAB script “test1” can be used to acquire raw F200 color streams in MATLAB. 
 - Raw color and depth streams from the Intel RealSense camera (F200) can be acquired simultaneously by using the MATLAB script “test2”.
 - You can also use this adaptor to adjust the camera-specific property and retrieve the current value of the property. For example, the MATLAB script “test3” in the code sample file can be used to retrieve the current value of color brightness and adjust its value.
 
Prepared by: Jing Huang

*Third Party trademarks are the property of their respective owners


