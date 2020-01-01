//
// Created by nerya on 18/12/2019.
//

#include <sys/socket.h>
#include <string>
#include <iostream>
#include <unistd.h>
#include <netinet/in.h>
#include "OpenDataServerCommand.h"
#include "VariablesSingelton.h"

#define SIZE_OF_BUFFER 1024

extern mutex mtx;
static int client_socket;

void updateValueWithXmlFile(string bufferString, vector<string> vectorOfNameValueFromXml) {
    VariablesSingelton* variablesSingelton = variablesSingelton->getInstanceOfVariablesSingelton();
    string delimiterOfAllText = ",";
    size_t posOfAllText = 0;
    string tokenOfAllText;
    unsigned int i = 0;
    while ((posOfAllText = bufferString.find(delimiterOfAllText)) != string::npos) {
        tokenOfAllText = bufferString.substr(0, posOfAllText);
        bufferString.erase(0, posOfAllText + delimiterOfAllText.length());
        variablesSingelton->updateValueInMapSim(vectorOfNameValueFromXml[i], stof(tokenOfAllText));
        ++i;
    }
    variablesSingelton->updateValueInMapSim(vectorOfNameValueFromXml[i], stof(bufferString));
}

void readingFromServer() {
    vector<string> vectorOfNameValue = {"/instrumentation/airspeed-indicator/indicated-speed-kt",
                                        "/sim/time/warp",
                                        "/controls/switches/magnetos",
                                        "/instrumentation/heading-indicator/offset-deg",
                                        "/instrumentation/altimeter/indicated-altitude-ft",
                                        "/instrumentation/altimeter/pressure-alt-ft",
                                        "/instrumentation/attitude-indicator/indicated-pitch-deg",
                                        "/instrumentation/attitude-indicator/indicated-roll-deg",
                                        "/instrumentation/attitude-indicator/internal-pitch-deg",
                                        "/instrumentation/attitude-indicator/internal-roll-deg",
                                        "/instrumentation/encoder/indicated-altitude-ft",
                                        "/instrumentation/encoder/pressure-alt-ft",
                                        "/instrumentation/gps/indicated-altitude-ft",
                                        "/instrumentation/gps/indicated-ground-speed-kt",
                                        "/instrumentation/gps/indicated-vertical-speed",
                                        "/instrumentation/heading-indicator/indicated-heading-deg",
                                        "/instrumentation/magnetic-compass/indicated-heading-deg",
                                        "/instrumentation/slip-skid-ball/indicated-slip-skid",
                                        "/instrumentation/turn-indicator/indicated-turn-rate",
                                        "/instrumentation/vertical-speed-indicator/indicated-speed-fpm",
                                        "/controls/flight/aileron",
                                        "/controls/flight/elevator",
                                        "/controls/flight/rudder",
                                        "/controls/flight/flaps",
                                        "/controls/engines/engine/throttle",
                                        "/controls/engines/current-engine/throttle",
                                        "/controls/switches/master-avionics",
                                        "/controls/switches/starter",
                                        "/engines/active-engine/auto-start",
                                        "/controls/flight/speedbrake",
                                        "/sim/model/c172p/brake-parking",
                                        "/controls/engines/engine/primer",
                                        "/controls/engines/current-engine/mixture",
                                        "/controls/switches/master-bat",
                                        "/controls/switches/master-alt",
                                        "/engines/engine/rpm"};
    string dataStr;
    bool weAreConnectingSocket;
    while (true) {
        mtx.try_lock();
        weAreConnectingSocket = VariablesSingelton::getInstanceOfVariablesSingelton()->isConnectSocket();
        mtx.unlock();
        if (weAreConnectingSocket) {
            char buffer[SIZE_OF_BUFFER];
            int numBytesRead = read(client_socket, buffer, sizeof(buffer));
            if (numBytesRead > 0) {
                for (int i = 0; i < numBytesRead; i++) {
                    char c = buffer[i];
                    if (c == '\n') {
                        if (dataStr.length() > 0) {
                            updateValueWithXmlFile(dataStr, vectorOfNameValue);
                            dataStr.clear();
                        }
                    } else {
                        dataStr += c;
                    }
                }
            }
        } else {
            break;
        }
    }

}


void openSocketForDataServerCommand(string portOfServerString) {
    int socketfd = socket(AF_INET, SOCK_STREAM, 0);
    if (socketfd == -1) {
        //error
        std::cerr << "Could not create a socket" << std::endl;
        exit(1);
    }


    //bind socket to IP address
    // we first need to create the sockaddr obj.
    sockaddr_in address; //in means IP4
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY; //give me any IP allocated for my machine
    int portOfServer = (int) VariablesSingelton::getInstanceOfVariablesSingelton()->calculateStringInInterpret(portOfServerString);
    address.sin_port = htons(portOfServer);
    //we need to convert our number
    // to a number that the network understands.

    //the actual bind command
    if (bind(socketfd, (struct sockaddr *) &address, sizeof(address)) == -1) {
        std::cerr << "Could not bind the socket to an IP" << std::endl;
        exit(1);
    }

    //making socket listen to the port
    if (listen(socketfd, 5) == -1) { //can also set to SOMAXCON (max connections)
        std::cerr << "Error during listening command" << std::endl;
        exit(1);
    }/* else {
        std::cout << "Server is now listening ..." << std::endl;
    }*/

    // accepting a client
    client_socket = accept(socketfd, (struct sockaddr *) &address,
                           (socklen_t *) &address);

    if (client_socket == -1) {
        std::cerr << "Error accepting client" << std::endl;
        exit(1);
    }

    close(socketfd); //closing the listening socket
}


int OpenDataServerCommand::execute(vector<vector<std::__cxx11::string> > &detailsOfTheCommand, unsigned int index) {
    thread t1(openSocketForDataServerCommand, detailsOfTheCommand[index][1]);
    t1.join();
    VariablesSingelton::getInstanceOfVariablesSingelton()->connectMe();
    thread t2(readingFromServer);
    t2.detach();
    return ++index;
}


OpenDataServerCommand::OpenDataServerCommand() {}
