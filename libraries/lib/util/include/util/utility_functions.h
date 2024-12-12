#ifndef UTILITY_FUNCTIONS_H
#define UTILITY_FUNCTIONS_H

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <stdexcept>
#include <algorithm>
#include <array> 

// Function to execute a shell command and capture its output
std::string executeShellCommand(const std::string& command);

// Function to convert UTM coordinates to Latitude and Longitude
std::vector<double> convertUTMToLatLong(double utm_x, double utm_y, int utm_zone, const std::string& utm_zone_letter);
std::vector<double> convertUTMToLatLongPython(double utm_x, double utm_y, int utm_zone, const std::string& utm_zone_letter);


// Function to convert Latitude and Longitude UTM coordinates
std::vector<double> convertLatLongToUTM(double lat, double lon);
std::vector<double> convertLatLongToUTMPython(double lat, double lon);

#endif // UTILITY_FUNCTIONS_H

