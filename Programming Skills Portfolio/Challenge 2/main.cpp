#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>

struct Joke {
    std::string setup;
    std::string punchline;
};

std::vector<Joke> loadJokes(const std::string& filename) {
    std::vector<Joke> jokes;
    std::ifstream file(filename);
    std::string line;

    while (std::getline(file, line)) {
        size_t delimiterPos = line.find('?');
        if (delimiterPos != std::string::npos) {
            Joke joke;
            joke.setup = line.substr(0, delimiterPos + 1);
            joke.punchline = line.substr(delimiterPos + 1);
            jokes.push_back(joke);
        }
    }
    return jokes;
}

int main() {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    std::vector<Joke> jokes = loadJokes("randomJokes.txt");

    if (jokes.empty()) {
        std::cerr << "Error: No jokes found in randomJokes.txt\n";
        return 1;
    }

    std::string input;
    std::cout << "Type 'Alexa, tell me a joke' or 'quit' to exit.\n";

    while (true) {
        std::cout << "\n> ";
        std::getline(std::cin, input);

        if (input == "quit") {
            break;
        }

        if (input == "Alexa, tell me a joke") {
            int index = std::rand() % jokes.size();
            std::cout << jokes[index].setup << "\n";
            std::cout << "(Press Enter to see punchline...)";
            std::cin.get();
            std::cout << jokes[index].punchline << "\n";
        } else {
            std::cout << "Invalid prompt. Type 'Alexa, tell me a joke' or 'quit'.\n";
        }
    }

    return 0;
}