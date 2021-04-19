#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include "KeyInfo.h"
#include "calc_sci/Calculator.h"

#include "olcPixelGameEngine/olcPixelGameEngine.h"

namespace dwe
{
	class CalculatorSciPixel : public olc::PixelGameEngine
	{
	public:
		CalculatorSciPixel(void);
		~CalculatorSciPixel(void);

	protected:

		std::vector<std::string> equation_vect;
		size_t equation_vect_size = 0;
		size_t graph_height = 0;
		bool parameter_needed = false;
		std::vector<std::string> parameter_vect;
		Calculator* calc = nullptr;
		CalculatorResult* calc_result = nullptr;
		CalculatorParameter* calc_param = nullptr;
		bool OnUserCreate() override;
		bool OnUserUpdate(float) override;
		KeyInfo getPressedKey();
		CalculatorResult* calculate();
		void printResult(CalculatorResult*, bool);
		void drawGraph(CalculatorResult&);
	};
}
