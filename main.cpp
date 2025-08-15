#include <SFML/Graphics.hpp>
#include <iostream>
#include <sstream>
#include "scanner.h"

int main() {

    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML Port Scanner");
    sf::Font font;

    if (!font.loadFromFile("arial.ttf")) {
        std::cerr << "Could not load font" << std::endl;
        return 1;
    }

    sf::Text label("Enter IP and Port Range: ", font, 20);
    label.setPosition(50, 50);

    sf::Text ipLabel("IP Address: ", font, 20);
    ipLabel.setPosition(50, 100);
    sf::Text portLabel("Port Range (start-end):", font, 20);
    portLabel.setPosition(50, 150);

    sf::Text inputText("", font, 20);
    inputText.setPosition(250, 100);
    inputText.setFillColor(sf::Color::Black);

    sf::Text portText("", font, 20);
    portText.setPosition(250, 150);
    portText.setFillColor(sf::Color::Black);

    sf::RectangleShape inputBox(sf::Vector2f(300, 30));
    inputBox.setPosition(240, 100);
    inputBox.setFillColor(sf::Color::Green);

    sf::RectangleShape portBox(sf::Vector2f(300, 30));
    portBox.setPosition(240, 150);
    portBox.setFillColor(sf::Color::Red);


    sf::RectangleShape button(sf::Vector2f(100, 50));
    button.setPosition(250, 200);
    button.setFillColor(sf::Color::Blue);

    sf::Text buttonText("Scan", font, 20);
    buttonText.setPosition(270, 215);
    buttonText.setFillColor(sf::Color::White);

    std::string ipAddress;
    std::string portRange;
    bool typingIP = false, typingPort = false;

    while (window.isOpen()) {


        sf::Event event;
        while(window.pollEvent(event)) {

            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (event.type == sf::Event::MouseButtonPressed) {

                if (button.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {

                    // Extract the start and end port from portRange string

                    std::istringstream iss(portRange);
                    int startPort, endPort;
                    char dash; // To consume the dash character
                    if (!(iss >> startPort >> dash >> endPort) || dash != '-') {
                        std::cerr << "Invalid port range format. Use start-end format." << std::endl;
                        continue;
                    }

                    // Start scanning
                    std::cout << "Scanning IP: " << ipAddress << " Port Range: " << portRange << std::endl;
                    scan(ipAddress, startPort, endPort);

                }
            }

            if (event.type == sf::Event::TextEntered) {

                if (typingIP) {

                    if (event.text.unicode == '\b' && !ipAddress.empty()) {
                        ipAddress.pop_back();
                    } else if (event.text.unicode < 128) {

                        ipAddress += static_cast<char>(event.text.unicode);

                    }
                    inputText.setString(ipAddress);
                }

                if (typingPort) {

                    if (event.text.unicode == '\b' && !portRange.empty()) {
                        portRange.pop_back();
                    } else if (event.text.unicode < 128) {

                        portRange += static_cast<char>(event.text.unicode);
                    }
                    portText.setString(portRange);
                }
            }


            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Tab) {
                    typingIP = !typingIP;
                    typingPort = !typingIP;
                }
            }
        }

        window.clear(sf::Color::White);
        window.draw(label);
        window.draw(ipLabel);
        window.draw(portLabel);
        window.draw(inputBox);
        window.draw(portBox);
        window.draw(inputText);
        window.draw(portText);
        window.draw(button);
        window.draw(buttonText);
        window.display();
    }


    return 0;
}