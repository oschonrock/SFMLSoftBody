#pragma once

#include <SFML/Window/VideoMode.hpp>
#include <iostream>
#include <ostream>
#include <stdexcept>

double aspect(sf::VideoMode m);

std::ostream& operator<<(std::ostream& os, sf::VideoMode m);

// finds "best" mode, which does not have an outlier aspect ratio, thereby eliminating incorrectly
// reported dual screen resolutions
sf::VideoMode best_video_mode();
