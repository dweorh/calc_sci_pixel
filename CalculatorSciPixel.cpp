#define OLC_PGE_APPLICATION
#include "CalculatorSciPixel.h"
//#include "KeyInfo.h"

dwe::CalculatorSciPixel::CalculatorSciPixel(void)
{
	sAppName = "CalcSci by dweorh";
}

dwe::CalculatorSciPixel::~CalculatorSciPixel(void)
{
}

bool dwe::CalculatorSciPixel::OnUserCreate()
{
	calc_result = new CalculatorResult();
	calc = new Calculator(calc_result);
	graph_height = ScreenHeight() - 40;
	return true;
}

dwe::KeyInfo dwe::CalculatorSciPixel::getPressedKey() {
	KeyInfo kInfo;
	olc::HWButton button;
	bool shift = GetKey(olc::Key::SHIFT).bHeld;
	std::map<olc::Key, size_t> shiftedKeys;
	std::map<olc::Key, size_t> irregularKeys;
	irregularKeys[olc::Key::OEM_1] = 59;
	irregularKeys[olc::Key::OEM_2] = 47;
	irregularKeys[olc::Key::OEM_3] = 96;
	irregularKeys[olc::Key::OEM_4] = 91;
	irregularKeys[olc::Key::OEM_5] = 92;
	irregularKeys[olc::Key::OEM_6] = 93;
	irregularKeys[olc::Key::OEM_7] = 39;
	irregularKeys[olc::Key::BACK] = 8;
	irregularKeys[olc::Key::ENTER] = 10;
	irregularKeys[olc::Key::RETURN] = 13;
	irregularKeys[olc::Key::ESCAPE] = 27;
	irregularKeys[olc::Key::SPACE] = 32;
	irregularKeys[olc::Key::DEL] = 127;
	irregularKeys[olc::Key::MINUS] = 45;
	irregularKeys[olc::Key::EQUALS] = 61;
	irregularKeys[olc::Key::COMMA] = 44;
	irregularKeys[olc::Key::PERIOD] = 46;
	irregularKeys[olc::Key::NP_DIV] = 47;
	irregularKeys[olc::Key::NP_MUL] = 42;
	irregularKeys[olc::Key::NP_SUB] = 45;
	irregularKeys[olc::Key::NP_ADD] = 43;
	irregularKeys[olc::Key::NP_DECIMAL] = 44;
	//irregularKeys[olc::Key::OEM_8] = 1;

	shiftedKeys[olc::Key::K0] = 41;
	shiftedKeys[olc::Key::K1] = 33;
	shiftedKeys[olc::Key::K2] = 64;
	shiftedKeys[olc::Key::K3] = 35;
	shiftedKeys[olc::Key::K4] = 36;
	shiftedKeys[olc::Key::K5] = 37;
	shiftedKeys[olc::Key::K6] = 94;
	shiftedKeys[olc::Key::K7] = 38;
	shiftedKeys[olc::Key::K8] = 42;
	shiftedKeys[olc::Key::K9] = 40;
	shiftedKeys[olc::Key::OEM_1] = 58;
	shiftedKeys[olc::Key::OEM_2] = 63;
	shiftedKeys[olc::Key::OEM_3] = 126;
	shiftedKeys[olc::Key::OEM_4] = 123;
	shiftedKeys[olc::Key::OEM_5] = 124;
	shiftedKeys[olc::Key::OEM_6] = 125;
	shiftedKeys[olc::Key::OEM_7] = 34;
	shiftedKeys[olc::Key::MINUS] = 95;
	shiftedKeys[olc::Key::EQUALS] = 43;
	shiftedKeys[olc::Key::COMMA] = 60;
	shiftedKeys[olc::Key::PERIOD] = 62;
	//shiftedKeys[olc::Key::OEM_8] = 1;

	kInfo.ascii = "";
	kInfo.ascii_code = 0;
	for (size_t key = olc::Key::NONE; key < olc::Key::ENUM_END; key++) {
		button = GetKey(olc::Key(key));
		if (button.bPressed) {
			if (key >= olc::Key::A && key <= olc::Key::Z) {
				kInfo.ascii_code = 97 + (key - olc::Key::A);
				if (shift) {
					kInfo.ascii_code -= 32;
				}
			}
			else if (key >= olc::Key::K0 && key <= olc::Key::K9 && !shift) {
				kInfo.ascii_code = 48 + (key - olc::Key::K0);
			}
			else if (key >= olc::Key::NP0 && key <= olc::Key::NP9) {
				kInfo.ascii_code = 48 + (key - olc::Key::NP0);
			}
			else if (irregularKeys.count(olc::Key(key)) > 0) {
				kInfo.ascii_code = irregularKeys[olc::Key(key)];
			}

			if (shift && shiftedKeys.count(olc::Key(key)) > 0) {
				kInfo.ascii_code = shiftedKeys[olc::Key(key)];
			}
			if (kInfo.ascii_code > 0) {
				kInfo.ascii = (char)kInfo.ascii_code;
			}

			kInfo.buttons.push_back(button);
		}
	}
	return kInfo;
}

dwe::CalculatorResult* dwe::CalculatorSciPixel::calculate() {
	std::string equation = implode("", equation_vect);
	if (equation.length() > 0) {
		calc->setEquation(equation);
		if (calc->requireParameter()) {
			if (parameter_vect.size() > 0) {
				calc_param = new CalculatorParameter(implode("", parameter_vect));
				calc->setParameter(calc_param);
				parameter_needed = false;
				parameter_vect.clear();
			}
			else {
				parameter_needed = true;
				return nullptr;
			}
		}
		return calc->calculate();
	}
	return nullptr;
}

void dwe::CalculatorSciPixel::printResult(CalculatorResult* res, bool bottom_graph) {
	size_t y = ScreenHeight() / 2;
	if (bottom_graph) {
		y = graph_height - 30;
	}
	if (res->ok) {
		DrawString(10, y - 10, res->result, olc::WHITE, bottom_graph ? 1 : 2);
		DrawString(10, y + 20, res->onp, olc::WHITE, 1);
	}
	else {
		DrawString(10, y - 10, res->msg, olc::WHITE, 1);
	}
}

void dwe::CalculatorSciPixel::drawGraph(CalculatorResult& res)
{
	const size_t DRAW_RESOLUTION = ScreenWidth() * 2;
	size_t w = ScreenWidth() - 10; // add some margin
	size_t h = graph_height - 10;

	double minX = res.curParam,
		minY = res.d_result,
		maxX = res.curParam,
		maxY = res.d_result;
	std::vector<CalculatorResult> results;
	std::vector<CalculatorResult> points;
	CalculatorResult* result;

	results.push_back(res);
	// cache results to display them as points
	while (result = calc->nextCalc()) {
		results.push_back(*result);
	}
	// param for a drawing
	calc_param->curVal = calc_param->minVal;
	calc_param->resolution = DRAW_RESOLUTION;


	while (result = calc->nextCalc()) {
		points.push_back(*result);
		if (result->d_result < minY) {
			minY = result->d_result;
		}
		if (result->d_result > maxY) {
			maxY = result->d_result;
		}
		if (result->curParam < minX) {
			minX = result->curParam;
		}
		if (result->curParam > maxX) {
			maxX = result->curParam;
		}
	}
	//double step_x = 1.0, step_y = 1.0;
	double total_x_points = maxX - minX;
	double total_y_points = maxY - minY;
	double step_x = 1.0, step_y = 1.0;
	if (total_x_points > 0) {
		step_x = w / total_x_points;
	}
	if (total_y_points > 0) {
		step_y = h / total_y_points;
	}
	if (step_y < step_x) {
		step_x = step_y;
	}
	else {
		step_y = step_x;
	}

	olc::vi2d zero = { int(5.0 + std::abs(minX * step_x)), int(h + std::abs(minY * step_y)) };
	olc::vi2d start = { int(points[0].curParam * step_x), int(points[0].d_result * step_y) * -1 };
	olc::vi2d end;
	olc::vi2d text_offset = { 5, -5 };
	olc::vi2d text_point;
	for (CalculatorResult result : points) {
		end = { int(result.curParam * step_x), int(result.d_result * step_y) * -1 };
		DrawLine(zero + start, zero + end, olc::WHITE);
		start = end;
	}

	std::string label = "";
	char result_buffer[20];
	for (CalculatorResult result : results) {
		sprintf_s(result_buffer, "%.2f", result.d_result);
		label = "(" + std::to_string(result.curParam) + "," + result_buffer + ")";
		start = { int(result.curParam * step_x), int(result.d_result * step_y) * -1 };
		text_point = zero + start + text_offset;
		DrawString(text_point.x, text_point.y, label, olc::WHITE, 1);

	}
}

bool dwe::CalculatorSciPixel::OnUserUpdate(float fElapsedTime)
{
	std::vector<std::string> text;
	std::vector<std::string> parameter_text;
	CalculatorResult* res = nullptr;
	bool multi_result = false;

	text.push_back("equation:");
	KeyInfo keyPressed = getPressedKey();
	if (keyPressed.ascii_code == 10 || keyPressed.ascii_code == 13) {
		if (equation_vect.size() == 0) {
			return false;
		}
		if (parameter_needed) {
			multi_result = true;
		}
		res = calculate();
	}
	else if (keyPressed.ascii_code == 8) {
		if (parameter_needed && parameter_vect.size() > 0) {
			parameter_vect.pop_back();
		}
		else if (equation_vect.size() > 0) {
			equation_vect.pop_back();
		}
	}
	else if (keyPressed.ascii != "") {
		parameter_needed ? parameter_vect.push_back(keyPressed.ascii) : equation_vect.push_back(keyPressed.ascii);
	}
	text.push_back(implode("", equation_vect));
	if (equation_vect_size == 0 || equation_vect_size != equation_vect.size() || parameter_needed || res != nullptr) {
		equation_vect_size = equation_vect.size();
		// Draw Screen
		Clear(olc::BLACK);
		DrawString(10, graph_height + (parameter_needed ? 10 : 20), dwe::implode(" ", text), olc::WHITE, 1);
		if (parameter_needed) {
			parameter_text.push_back("x:");
			parameter_text.push_back(implode("", parameter_vect));
			DrawString(10, graph_height + 20, dwe::implode(" ", parameter_text), olc::WHITE, 1);
		}
		else if (res != nullptr) {
			if (multi_result) {
				drawGraph(*res);
				// printResult(res, true);
			}
			else {
				printResult(res, false);
			}
		}
	}

	return true;
}