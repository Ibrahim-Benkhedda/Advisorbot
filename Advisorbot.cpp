#include "Advisorbot.h"

Advisorbot::Advisorbot() {
    std::cout << ">>> Welcome to Advisorbot." << std::endl;
    std::cout << ">>> Use 'help' to list all available commands or 'help [command]' about the specific command." << std::endl;
    while (true) {
        init();      
    }
    
}

void Advisorbot::init() {
    
    std::vector<std::string> userCommands = parseUserCommands();

    /** Print the user input command */
    std::string command;
    for (const auto& command : userCommands) {
        std::cout << "commands: " << command << std::endl;
    }

    matchCommands(userCommands);
}

std::vector<std::string> Advisorbot::parseUserCommands() {
    std::string userInput; // stores the user input 
    std::vector<std::string> commands; // stores the tokenised commands from the userInput

    std::getline(std::cin, userInput);
    // istringstream is better for handling user Input then the our tokenise function.
    // we use it to extract tokens from the string userInput using the getline function
    std::istringstream userInputStream(userInput);

    std::string cmd;
    // get the tokens from the userInputStream until there are no more to extract
    while (std::getline(userInputStream, cmd, ' '))
    {
        // checks if the command is not a space, if not, append the command to the commands vector
        if (!cmd.empty()) commands.push_back(cmd);
    }

    // returns the tokenised commands of the user input
    return commands;
}

void Advisorbot::printHelp() {
    std::cout << "The available commands are help, help [command]:" << std::endl;
    std::cout << "help min" << std::endl;
    std::cout << "help max" << std::endl;
    std::cout << "help avg" << std::endl;
    std::cout << "help prod" << std::endl;
    std::cout << "help predict" << std::endl;
    std::cout << "help time" << std::endl;
}
void Advisorbot::getMin() {
    std::cout << "find minimum bid or ask for product in current time step" << std::endl;
}
void Advisorbot::getMax() {
    std::cout << "find maximum bid or ask for product in current time step" << std::endl;
}
void Advisorbot::getProduct() {
    std::cout << " list available products " << std::endl;
}
void Advisorbot::getAverage() {
    std::cout << "compute average ask or bid for the sent product over the sent number of time steps." << std::endl;
}
void Advisorbot::predictMarket() {
    std::cout << "predict max or min ask or bid for the sent product for the next time step" << std::endl;
}
void Advisorbot::getTime() {
    std::cout << "state current time in dataset, i.e. which timeframe are we looking at" << std::endl;
}

void Advisorbot::matchCommands(const std::vector<std::string>& userCommands) {
    // inspired from c++ - How to store methods as function pointers in a map container? - Stack Overflow
    // https://stackoverflow.com/questions/53336880/how-to-store-methods-as-function-pointers-in-a-map-container
    // 
    // map that contains the possible commands for advisorbot program
    // AdvisorbotFunctionPointer is pointer  which is used to call the member functions in Advisorbot.
    std::map<std::string, AdvisorbotFunctionPointer> commandsMap;

    // populate map
    commandsMap["help"] = &Advisorbot::printHelp;
    commandsMap["min"] = &Advisorbot::getMin;
    commandsMap["max"] = &Advisorbot::getMax;
    commandsMap["avg"] = &Advisorbot::getAverage;
    commandsMap["prod"] = &Advisorbot::getProduct;
    commandsMap["predict"] = &Advisorbot::predictMarket;
    commandsMap["time"] = &Advisorbot::getTime;

    if (userCommands[0] == "help") {
        if (userCommands.size() == 1) {
            // If there's no second command, call the "help" function
            if (commandsMap.count("help")) {
                (this->*commandsMap["help"])();
            }
            else {
                std::cout << "invalid input..." << std::endl;
            }
        }
        else if (userCommands.size() == 2 && userCommands[1] != "help" && commandsMap.count(userCommands[1])) {
            // If there's a second command that is not "help" and it's in the map, call the corresponding function
            (this->*commandsMap[userCommands[1]])();
        }
        else {
            std::cout << "invalid input..." << std::endl;
        }
    }
    else {
        // If the first command is not "help" and it's in the map, call the corresponding function
        if (commandsMap.count(userCommands[0])) {
            (this->*commandsMap[userCommands[0]])();
        }
        else {
            std::cout << "invalid input..." << std::endl;
        }
    }
}





    // check if the token is a number
    // bool isNumber = true;
    // int number = 0;

    // try {
        // number = std::stoi(cmd);
        // return number;
    // }
    // catch (const std::invalid_argument& e) {
    //    isNumber = false;
    // }