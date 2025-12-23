#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <string>

namespace constants
{
    // Constants
   	constexpr int gScreenWidth     { 640 };
   	constexpr int gScreenHeight    { 480 };
   	// PATH TO YOUR RESOURCE FOLDER 'resources', 'gResPath'
   	const std::string gResPath { "./resources/" };
	// PATH'S TO ALL YOUR EXTERNAL RESOURCES using 'gResPath'
	const std::string bg_str  { gResPath + "images/bg.jpg" };
    const std::string sample_str { gResPath + "sounds/sample.wav"};
}

#endif