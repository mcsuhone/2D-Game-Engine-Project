#pragma once

#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING

#include <iostream>
#include <vector>
#include <experimental/filesystem>
#include <cstdio>
#include <thread>
#include "scene.h"
#include "server.h"

void ServerThread(Server* server) {
    server->Init();
    delete server;
}

void EventPollerThread(Scene* scene) {
    scene->EventPoller();
}

int main()
{
     
    std::string input;
    /*
    std::cout << "Host server?\n";
    std::cin >> input;
    */
    input = "n";
    
    Scene* scene = new Scene();

    //std::thread event_poller_thread(EventPollerThread);

    if (input.compare("y") == 0) {
        Server* server = new Server();
        std::thread server_thread(ServerThread, server);
        scene->MainLoop();
        delete scene;
        server->Close();
        server_thread.join();
    }
    else {
        scene->MainLoop();
        delete scene;
    }

    return 0;
}


