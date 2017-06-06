#pragma once
#ifndef NEURON_REWORKED_H_
#define NEURON_REWORKED_H_
#include <vector>

class neuron_reworked
{
public:
	neuron_reworked();
	neuron_reworked(std::vector<neuron_reworked*> input, int layer);
	void reset_weight();
	void input_weight(std::vector<double> vec, double base);
	void calculate_value();
	void weight_changer(double u);
	void set_input_value(double val) { this->input_value = val; }


	std::vector<double> weight;
	double base_weight;
	double mistake;
	double output;
private:
	int layer;

	std::vector<neuron_reworked*> input;	

	double input_value;

	double weight_sum;
};

#endif
