#pragma once
#include <vector>
#include <string>
#include "olcPixelGameEngine/olcPixelGameEngine.h"
namespace dwe {
	struct KeyInfo
	{
		std::string ascii = "";
		size_t ascii_code = 0;
		std::vector<olc::HWButton> buttons;
	};
}