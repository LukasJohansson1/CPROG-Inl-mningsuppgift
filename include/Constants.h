#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <string>

namespace constants
{
    // Constants
   	constexpr int gScreenWidth     { 1280 };
   	constexpr int gScreenHeight    { 720 };
   	// PATH TO YOUR RESOURCE FOLDER 'resources', 'gResPath'
   	const std::string gResPath { "./resources/" };
	// PATH'S TO ALL YOUR EXTERNAL RESOURCES using 'gResPath'
	const std::string bg_str  { gResPath + "images/scary-background.png" };
    const std::string sample_str { gResPath + "sounds/sample.wav"};
	const std::string player_charart {gResPath + "images/Walk.png"};
}

#endif