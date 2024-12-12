#define _USE_MATH_DEFINES
#include <cmath>
#include "util/utility_functions.h"
#include <stdexcept>
#include <cstdio>
#include <cstring>
#include <sstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <array>

#include <proj.h>
#define DEG_TO_RAD (M_PI / 180.0)




#define QUOTE(...) #__VA_ARGS__
const char *UTM_TO_LAT_LONG_PYTHON_CONVERTER_COMMAND_TEMPLATE = QUOTE(python3 -c "from utm import to_latlon; print(to_latlon(%.2f, %.2f, %d, '%s'))");
const char *LAT_LONG_TO_UTM_PYTHON_CONVERTER_COMMAND_TEMPLATE = QUOTE(python3 -c "from utm import from_latlon; print(from_latlon(%.6f, %.6f))");

std::string executeShellCommand(const std::string& command) {
    std::array<char, 128> buffer;
    std::string result;
    FILE* pipe = popen(command.c_str(), "r");
    if (!pipe) throw std::runtime_error("popen() failed!");
    try {
        while (fgets(buffer.data(), buffer.size(), pipe) != nullptr) {
            result += buffer.data();
        }
    } catch (...) {
        pclose(pipe);
        throw;
    }
    pclose(pipe);

    result.erase(0, result.find_first_not_of(" \n\r\t"));
    result.erase(result.find_last_not_of(" \n\r\t") + 1);

    return result;
}

int calculateUTMZone(double lon) {
    lon = fmod(lon + 180.0, 360.0);
    if (lon < 0) {
        lon += 360.0;
    }
    return static_cast<int>(floor(lon / 6.0)) + 1;
}

// Function to calculate UTM zone letter
char calculateUTMZoneLetter(double lat) {
    const char utm_zone_letters[] = "CDEFGHJKLMNPQRSTUVWXX";
    int zone_index = static_cast<int>((lat + 80.0) / 8.0);

    if (zone_index < 0) {
        zone_index = 0;
    } else if (zone_index >= sizeof(utm_zone_letters) - 1) {
        zone_index = sizeof(utm_zone_letters) - 2;
    }

    return utm_zone_letters[zone_index];
}

std::vector<double> convertLatLongToUTM(double lat, double lon) {
    std::vector<double> output(4, 0.0);  // [utm_x, utm_y, utm_zone, utm_letter]
    try {
        PJ_CONTEXT *C = proj_context_create();
        if (!C) {
            throw std::runtime_error("Failed to create PROJ context.");
        }

        int utm_zone = calculateUTMZone(lon);
        char utm_letter = calculateUTMZoneLetter(lat);

        std::string proj_string = "+proj=utm +zone=" + std::to_string(utm_zone) + " +datum=WGS84";
        if (lat >= 0) {
            proj_string += " +north";
        } else {
            proj_string += " +south";
        }

        PJ *P = proj_create(C, proj_string.c_str());
        if (!P) {
            proj_context_destroy(C);
            throw std::runtime_error("Failed to create PROJ projection.");
        }

        PJ *P_latlong = proj_create(C, "+proj=latlong +datum=WGS84");
        if (!P_latlong) {
            proj_destroy(P);
            proj_context_destroy(C);
            throw std::runtime_error("Failed to create PROJ latlong projection.");
        }

        PJ_COORD input;
        input.lp.lam = lon * DEG_TO_RAD;
        input.lp.phi = lat * DEG_TO_RAD;

        PJ_COORD output_coord = proj_trans(P_latlong, PJ_FWD, input);
        output_coord = proj_trans(P, PJ_FWD, output_coord);

        if (output_coord.xyzt.t == HUGE_VAL) {
            proj_destroy(P);
            proj_destroy(P_latlong);
            proj_context_destroy(C);
            throw std::runtime_error("Invalid coordinate");
        }

        output[0] = output_coord.enu.e;  // UTM X coordinate
        output[1] = output_coord.enu.n;  // UTM Y coordinate
        output[2] = static_cast<double>(utm_zone);  // UTM zone as double
        output[3] = static_cast<double>(utm_letter);  // UTM letter as double

        proj_destroy(P);
        proj_destroy(P_latlong);
        proj_context_destroy(C);

    } catch (const std::exception& e) {
        std::cerr << "ERROR: Exception caught while converting LatLong to UTM. "
                  << "Arguments: lat=" << lat << ", lon=" << lon
                  << ". Error: " << e.what() << std::endl;
    }

    return output;
}

std::vector<double> convertUTMToLatLong(double utm_x, double utm_y, int utm_zone, const std::string& utm_zone_letter) {
    std::vector<double> output(2, 0.0);

    PJ_CONTEXT *C = proj_context_create();
    if (!C) {
        throw std::runtime_error("Failed to create PROJ context.");
    }

    std::string proj_string = "+proj=utm +zone=" + std::to_string(utm_zone) + " +datum=WGS84";
    if (utm_zone_letter >= "N") {
        proj_string += " +north";
    } else {
        proj_string += " +south";
    }

    PJ *P = proj_create(C, proj_string.c_str());
    if (!P) {
        proj_context_destroy(C);
        throw std::runtime_error("Failed to create PROJ transformation.");
    }

    PJ_COORD input_coord = proj_coord(utm_x, utm_y, 0, 0);
    PJ_COORD output_coord = proj_trans(P, PJ_INV, input_coord);

    if (proj_errno(P) != 0) {
        proj_destroy(P);
        proj_context_destroy(C);
        throw std::runtime_error("Coordinate transformation failed.");
    }

    output[0] = output_coord.lp.phi * 180.0 / M_PI;   // Latitude in degrees
    output[1] = output_coord.lp.lam * 180.0 / M_PI;   // Longitude in degrees

    // Clean up PROJ objects
    proj_destroy(P);
    proj_context_destroy(C);

    return output;
}

std::vector<double> convertUTMToLatLongPython(double utm_x, double utm_y, int utm_zone, const std::string& utm_zone_letter) {
    std::vector<double> output(2, 0.0); 
    
    try {
        char command[200];
        std::sprintf(command, UTM_TO_LAT_LONG_PYTHON_CONVERTER_COMMAND_TEMPLATE, utm_x, utm_y, utm_zone, utm_zone_letter.c_str());
        std::string result = executeShellCommand(command);
        
        result.erase(std::remove(result.begin(), result.end(), '('), result.end());
        result.erase(std::remove(result.begin(), result.end(), ')'), result.end());
        result.erase(std::remove(result.begin(), result.end(), ','), result.end());

        std::istringstream iss(result);
        double lat, lon;
        iss >> lat >> lon;
        
        output[0] = lat;
        output[1] = lon;
    } catch (const std::exception& e) {
        std::cerr << "ERROR: Exception caught while converting UTM to LatLong. "
                  << "Arguments: utm_x=" << utm_x << ", utm_y=" << utm_y
                  << ", utm_zone=" << utm_zone << ", utm_zone_letter=" << utm_zone_letter
                  << ". Error: " << e.what() << std::endl;
    }
    
    return output;
}

std::vector<double> convertLatLongToUTMPython(double lat, double lon) {
    std::vector<double> output(4, 0.0);  // [utm_x, utm_y, utm_zone, utm_hemisphere]

    try {
        char command[200];
        std::sprintf(command, LAT_LONG_TO_UTM_PYTHON_CONVERTER_COMMAND_TEMPLATE, lat, lon);
        std::cout << "    Shell command: " << command << std::endl;
        std::string result = executeShellCommand(command);

        // Clean up the result string
        result.erase(std::remove(result.begin(), result.end(), '('), result.end());
        result.erase(std::remove(result.begin(), result.end(), ')'), result.end());
        result.erase(std::remove(result.begin(), result.end(), ','), result.end());

        std::istringstream iss(result);
        double utm_x, utm_y;
        int utm_zone;
        std::string utm_zone_letter;

        iss >> utm_x >> utm_y >> utm_zone >> utm_zone_letter;

        output[0] = utm_x;
        output[1] = utm_y;
        output[2] = static_cast<double>(utm_zone);

        if (utm_zone_letter.size() == 3 && std::isalpha(utm_zone_letter[1])) {
            output[3] = static_cast<double>(utm_zone_letter[1]);
        } else {
            throw std::runtime_error("Invalid utm zone letter identifier received.");
        }

    } catch (const std::exception& e) {
        std::cerr << "ERROR: Exception caught while converting LatLong to UTM. "
                  << "Arguments: lat=" << lat << ", lon=" << lon
                  << ". Error: " << e.what() << std::endl;
    }

    return output;
}


