#include "Logger.h"

int main() {

    try {
        Logger logger("log.txt");
        logger.log(LogLevel::INFO,"starting application...");

        int user_id = 42;
        std::string action = "user";
        double duration = 3.5;
        std::string wolrd = "World";
        logger.log(LogLevel::INFO,"User {} performed {} in {} seconds", user_id, action, duration);
        logger.log(LogLevel::DEBUG,"Hello {}", wolrd);
        logger.log(LogLevel::WARNING,"This is a message whitout  any placeholders");
        logger.log(LogLevel::ERROR,"Multiple placeholders : {},{},",1,2,3);
    }catch (std::exception &e) {
        std::cerr << "Error is"<<e.what() << std::endl;
    }

    return 0;
}
