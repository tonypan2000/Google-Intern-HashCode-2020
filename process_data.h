#pragma once
#include "data.h"
#include "output.h"
#include <fstream>


class Process {
public:
	void read_input(istream& fin, Data &input);
	void process_data(Data &input, Output &result);
	void print_results(Output &result);
};
