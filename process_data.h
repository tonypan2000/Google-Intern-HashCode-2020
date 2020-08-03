#pragma once
#include "data.h"
#include "output.h"


class Process {
public:
	void read_input();
	void process_data();
	void save_result();

private:
	Data input;
	Output result;
};
