#include "helpers.h"
#include "data.h"

std::ofstream logs;

void clearscreen() {
    std::system("cls");
}

void rest(int ms) {
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

void initialize_logger() {
    logs.open("session_logs.txt", std::ios::app);
    logger("log file started.");
}

void logger(const std::string& msg) {
    auto current_time = std::chrono::system_clock::now();
    std::time_t current_time_proper = std::chrono::system_clock::to_time_t(current_time);
    std::tm* local_time = std::localtime(&current_time_proper);
    logs 
        << std::setw(2) << std::setfill('0') << local_time->tm_hour << ":" 
        << std::setw(2) << std::setfill('0') << local_time->tm_min << ":" 
        << std::setw(2) << std::setfill('0') << local_time->tm_sec << ":" 
        << " > " << msg << "\n";
    std::cout << "> " << msg << "\n";
}

void deinitialize_logger() {
    logs.close();
}