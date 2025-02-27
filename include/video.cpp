#include "video.hpp"
#include "SFML/Window/VideoMode.hpp"
#include "median.hpp"
#include <cstdlib>
#include <ostream>
#include <ranges>

double aspect(sf::VideoMode m) { return 1.0 * m.width / m.height; }

std::ostream& operator<<(std::ostream& os, sf::VideoMode m) {
    return os << "[" << m.width << ", " << m.height << "] " << m.bitsPerPixel
              << "bpp, AR = " << aspect(m) << "";
}

sf::VideoMode best_video_mode() {
    auto modes = sf::VideoMode::getFullscreenModes();
    if (modes.empty()) {
        throw std::logic_error("Error: no suitable video modes found");
    }

    // std::max_element is probably unnnecessary here, as sf::VideoMode::getFullscreenModes is
    // alredy sorted by bpp first so modes[0].bitsPerPixel; would probably suffice
    unsigned max_bits_per_pixel =
        std::ranges::max_element(modes, {}, &sf::VideoMode::bitsPerPixel)->bitsPerPixel;

    // remove all "lower bpp" modes
    modes.erase(std::remove_if(modes.begin(), modes.end(),
                               [max_bits_per_pixel](const sf::VideoMode& m) {
                                   return m.bitsPerPixel != max_bits_per_pixel;
                               }),
                modes.end());

    double med_aspect = median(modes, {}, [](const auto& m) { return aspect(m); });
    // std::cerr << "median aspect = " << med_aspect << "\n";

    // sf::VideoMode::getFullscreenModes returns "best" first, and std::remove, erase above
    // maintains order, so first = best
    auto best_mode_iter =
        std::find_if(modes.begin(), modes.end(), [med_aspect](const sf::VideoMode& mode) {
            double deviation = aspect(mode) / med_aspect;
            // eliminates massive outliers which are probably due to dual screen setups
            return deviation > 0.75 && deviation < 1.33;
        });

    if (best_mode_iter == modes.end()) {
        throw std::logic_error("Error: no suitable video modes found");
    }
    return *best_mode_iter;
}
