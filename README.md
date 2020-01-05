# FlightGear

This program sends a collection of commands to the provided simulator called Flight Gear 
and these commands cause the aircraft to fly. The purpose of the program is to be as both client and server. 
As a client server we will send the current values to the simulator for the variables we have changed their values. 
At that time, we will act as a server that constantly updates the values of the variables we created.

**Getting Started**

First of all, to use the program you need to install the Flight Gear simulator from the following link 
and select the installation of the workspace you are using (Windows, Linux, etc.):
https://www.flightgear.org/download/main-program/

Then, in the additional settings you will need to write these lines:

--telnet=socket,in,10,127.0.0.1,5402,tcp
--generic=socket,out,10,127.0.0.1,5400,tcp,generic_small

In addition, you will need a text file that contains commands and an xml file that needs 
to be put in the protocol folder that is in the data folder.

After you do all these things, you can run the program in the terminal together with an argument 
which is a text file with the commands (I called it fly.txt). Then you can press the fly button in the simulator, 
and after waiting for two minutes the plane will start flying.

**Built With**

Clion - Ide by JetBtrains

**Versioning**

We used [Github](https://github.com/naberdam/ex3) for version control

**Authors**

Amitay Popovsky and Nerya Aberdam
