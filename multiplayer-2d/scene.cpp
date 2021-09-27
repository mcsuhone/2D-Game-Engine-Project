#include "scene.h"

Scene::Scene(bool server_state) {
	window_ = new sf::RenderWindow(sf::VideoMode(1600, 900), "Cool game!");
	view_ = new sf::View();
	view_->setSize(1600 / 3, 900 / 3);
	view_->setCenter(800, 400);
	window_->setView(*view_);

    state_ = ProgramState::GAME;
    server_state_ = server_state;

    game_ = NULL;
    client_ = NULL;
}

Scene::~Scene() {
    delete window_;
    delete view_;
    delete game_;
    delete client_;
}

void Scene::MainLoop() {
    while (window_->isOpen())
    {
        if (state_ == ProgramState::GAME) {
            GameLoop();
        }
        else if (state_ == ProgramState::MENU) {
            MenuLoop();
        }
    }
}

void Scene::GameLoop() {
    // Determines if client was able to connect to server.
    bool networked = false;
    
    game_ = new Game(window_, view_);

    if (server_state_) {
        client_ = new Client();
        /* Try to connect to a server. */
        if (client_->Connect("192.168.0.102", 5555)) {
            networked = true;
            game_->SetNetworkID(client_->GetNetworkId());
        }
    }
    /* Load world after possible connection to server. */
    game_->LoadWorld();
    
    /* Make game run at 144Hz. */
    sf::Clock update_timer;
    sf::Clock render_timer;
    float update_time = 1.f / 144.f;
    float fps_limit = 1.f / 144.f;

    while (window_->isOpen()) {
        EventPoller();

        // What is going on with these times here??
        sf::Time u_dt = update_timer.getElapsedTime();
        if (u_dt.asSeconds() >= update_time) {
            game_->Update(u_dt.asMilliseconds());

            update_timer.restart();
        }
        if (render_timer.getElapsedTime() >= sf::seconds(fps_limit)) {
            game_->Render();

            render_timer.restart();
        }
        if (networked) {
            client_->SendGameData(game_);
            client_->ReceiveGameData(game_);
        }
    }
}

void Scene::MenuLoop() {
    std::cout << "No menu here lol.\n";
    return;
}


void Scene::EventPoller() {
    sf::Event event;
    while (window_->isOpen()) {
        if (state_ == ProgramState::GAME) {
            while (window_->pollEvent(event) && window_->hasFocus()) {
                if (event.type == sf::Event::Closed) {
                    window_->close();
                }
                else if (event.type == sf::Event::KeyPressed) {
                    game_->Action(event.key);
                }
            }
        }
        else if (state_ == ProgramState::MENU && window_->hasFocus()) {
            while (window_->pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    window_->close();
                }
            }
        } 
    }
}
