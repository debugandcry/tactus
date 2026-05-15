#pragma once
#include "data.h"

extern std::ofstream logs;


void clearscreen();

void rest(int ms);

void initialize_logger();

void logger(const std::string& msg);

void deinitialize_logger();