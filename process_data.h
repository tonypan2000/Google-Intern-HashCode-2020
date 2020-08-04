#pragma once
#include "data.h"
#include "output.h"
#include <fstream>


class Process {
public:
	void read_input(string filename, Data &input);
	void process_data(Data &input, Output &result);
	void save_results(string filename, Output &result);
};
