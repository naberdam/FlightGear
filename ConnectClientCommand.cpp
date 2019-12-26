//
// Created by nerya on 19/12/2019.
//

#include "ConnectClientCommand.h"
#include "VariablesSingelton.h"
#include <sys/socket.h>
#include <string>
#include <iostream>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

static int client_socket;
extern mutex mtx;


void connectSocketClient(vector<vector<string> > detailsOfTheCommand, unsigned int index) {
    //create socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        //error
        /*std::cerr << "Could not create a socket" << std::endl;*/
        throw "Could not create a socket";
    }

    //We need to create a sockaddr obj to hold address of server
    sockaddr_in address; //in means IP4
    address.sin_family = AF_INET;//IP4
    string ipClient = detailsOfTheCommand[index][1];
    address.sin_addr.s_addr = inet_addr(ipClient.c_str());  //the localhost address
    int portOfClient = (int) VariablesSingelton::getInstanceOfVariablesSingelton()->calculateStringInInterpret(detailsOfTheCommand[index][2]);
    address.sin_port = htons(portOfClient);
    //we need to convert our number (both port & localhost)
    // to a number that the network understands.

    // Requesting a connection with the server on local host with port 5402
    int is_connect = connect(client_socket, (struct sockaddr *) &address, sizeof(address));
    if (is_connect == -1) {
        /*std::cerr << "Could not connect to host server" << std::endl;*/
        throw "Could not connect to host server";
    } else {
        std::cout << "Client is now connected to server" << std::endl;
    }
}

void sendMsgToClient() {
    const char *msg;
    string msgString;
    VariablesSingelton *variablesSingelton = VariablesSingelton::getInstanceOfVariablesSingelton();
    bool weAreConnectingSocket;
    while (true) {
        mtx.try_lock();
        weAreConnectingSocket = VariablesSingelton::getInstanceOfVariablesSingelton()->isConnectSocket();
        mtx.unlock();
        if (weAreConnectingSocket) {
            if (variablesSingelton->queueWithMsg()) {
                msgString = variablesSingelton->getMsgFromQueue();
                msg = msgString.c_str();
                int is_sent = send(client_socket, msg, strlen(msg), 0);
                if (is_sent == -1) {
                    std::cout<<"Error sending message"<<std::endl;
                } else {
                    std::cout<<"Hello message sent to server" <<std::endl;
                    std::cout<< msgString << std::endl;
                }
            } else {
                this_thread::sleep_for(chrono::milliseconds(10));
            }
        } else {
            break;
        }
    }
    close(client_socket);
}

int ConnectClientCommand::execute(vector<vector<string> > &detailsOfTheCommand, unsigned int index) {
    thread t1(connectSocketClient, detailsOfTheCommand, index);
    t1.join();
    VariablesSingelton::getInstanceOfVariablesSingelton()->connectMe();
    thread t2(sendMsgToClient);
    t2.detach();
    return ++index;

}