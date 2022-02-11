#pragma once
#include "median.hpp"
#include <SFML/Window/VideoMode.hpp>
#include <iostream>

double aspect(sf::VideoMode m) { return 1.0 * m.width / m.height; }

std::ostream& operator<<(std::ostream& os, sf::VideoMode m) {
  return os << "[" << m.width << ", " << m.height << "] " << m.bitsPerPixel
            << "bpp, AR = " << aspect(m) << "";
}

// finds "best" mode, which does not have an outlier aspect ratio, thereby eliminating incorrectly
// reported dual screen resolutions
sf::VideoMode best_video_mode() {
  auto modes = sf::VideoMode::getFullscreenModes();
  if (modes.empty()) {
    std::cerr << "Error: no suitable video modes found\n";
    exit(EXIT_FAILURE); // NOLINT thread safety
  }

  // std::max_element is probably unnnecessary here, as sf::VideoMode::getFullscreenModes is alredy
  // sorted by bpp first so modes[0].bitsPerPixel; would probably suffice
  unsigned max_bits_per_pixel =
      std::max_element(modes.begin(), modes.end(),
                       [](const sf::VideoMode& a, const sf::VideoMode& b) {
                         return a.bitsPerPixel < b.bitsPerPixel;
                       })
          ->bitsPerPixel;

  // remove all "lower bpp" modes
  modes.erase(std::remove_if(modes.begin(), modes.end(),
                             [max_bits_per_pixel](const sf::VideoMode& m) {
                               return m.bitsPerPixel != max_bits_per_pixel;
                             }),
              modes.end());

  // for (const auto& m : modes) std::cout << m << "\n";
  double med_aspect = median(modes, {}, [](const auto& m) { return aspect(m); });
  std::cout << "median aspect using std::nth_element = " << med_aspect << "\n";

  // sf::VideoMode::getFullscreenModes returns "best" first, and std::remove, erase above maintains
  // order, so first = best
  auto best_mode_iter =
      std::find_if(modes.begin(), modes.end(), [med_aspect](const sf::VideoMode& mode) {
        double deviation = aspect(mode) / med_aspect;
        // eliminates massive outliers which are probably due to dual screen setups
        return deviation > 0.75 && deviation < 1.33;
      });

  if (best_mode_iter == modes.end()) {
    std::cerr << "Error: no suitable video modes found\n";
    exit(EXIT_FAILURE); // NOLINT thread safety
  }
  return *best_mode_iter;
}

