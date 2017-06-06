#include "neuron_reworked.h"


void neuron_reworked::reset_weight()
{
	this->weight.clear();
	if (layer == 0)
	{
		this->weight.push_back(1);
		this->base_weight = 0;
	}
	else
	{
		for (int i = 0; i < input.size(); i++)
		{
			this->weight.push_back(static_cast <double> (rand()) / static_cast <double> (RAND_MAX) - 0.5f);
		}
		this->base_weight = static_cast <double> (rand()) / static_cast <double> (RAND_MAX) - 0.5f;
	}
}

void neuron_reworked::input_weight(std::vector<double> vec, double base)
{
	if (layer == 0)
		return;
	this->weight.clear();
	for (int i = 0; i < input.size(); i++)
	{
		this->weight.push_back(vec[i]);
	}
	this->base_weight = base;
}

void neuron_reworked::calculate_value()
{
	double temp = 0;
	if (layer != 0)
	{
		for (int i = 0; i < input.size(); i++)
		{
			temp += weight[i] * input[i]->output;
		}
		temp += base_weight*(-1);
		weight_sum = temp;
		output = tanh(temp);
	}
	else
	{
		output = input_value;
	}
}

void neuron_reworked::weight_changer(double u)
{
	if (layer != 0)
	{
		double derivative = (2 / (cosh(2 * weight_sum) + 1));
		for (int i = 0; i < input.size(); i++)
		{			
			weight[i] = weight[i] + u*mistake*derivative*input[i]->output;
		}
		base_weight = base_weight + u*mistake*derivative * (-1);
	}
}

neuron_reworked::neuron_reworked(std::vector<neuron_reworked*> input, int layer)
{
	this->input = input;
	this->layer = layer;
	reset_weight();
}

neuron_reworked::neuron_reworked()
{
	this->layer = 0;
	reset_weight();
}


