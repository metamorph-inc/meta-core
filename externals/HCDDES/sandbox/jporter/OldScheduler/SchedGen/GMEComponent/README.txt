
If you cannot find a project file for your interpreter in this folder, please read the following:

The wizard uses MPC to create an appropriate project file for your development environment.
If the MPC failed to run for some reason, you will need to manually invoke it to generate a project file for the interpreter. It is highly advisable to use MPC to generate your project file, as it will take care of several necessary settings to make your interpreter compile correctly. 

MPC is usually located in the 3rdParty/mpc folder of your UDM installation.

From the "Component" folder of your UdmInterpreter, run this command:

...\Component>%UDM_3RDPARTY_PATH%\mpc\mpc.exe -features "mfc=1" -type vc71 -ti vc7dll Component.mpc

This will generate a .vcproj file which will compile correctly in VS2003.

Make sure that the UDM_3RDPARTY_PATH environment variable is set correctly, or enter the correct path in its place int he command line.

(Please refer the MPC documentation if you are using another development configuration)


