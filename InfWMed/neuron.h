#pragma once
#include <complex>
#include <vector>

class neuron
{
public:
	std::vector<neuron*> input;
	//tylko gdy layer = 0
	float input_value;
	float * weight;
	float base_weight = -1;
	float k = 1;
	float x0 = 0;
	float output = 0;
	float input_size;
	float mistake = 0;
	float weight_sum;
	//0 - wejscie, inne = i
	int layer;

	neuron(float input_value, int layer)
	{
		this->input_value = input_value;
		this->layer = layer;
		this->weight = new float[1];
		reset_weights(layer);
	}

	neuron(std::vector<neuron*> input, int layer)
	{

		this->input = input;
		this->input_size = input.size();
		this->layer = layer;
		this->weight = new float[input.size()];
		reset_weights(layer);
	}

	void reset_weights(int layer)
	{
		if(layer == 0)
		{
			this->weight[0] = static_cast <float> (rand()) / static_cast <float> (RAND_MAX) / 2 + 0.5;
		}
		else
		{
			for (int i = 0; i < input.size(); i++)
			{
				this->weight[i] = static_cast <float> (rand()) / static_cast <float> (RAND_MAX) / 2 + 0.5;
			}
		}
	}

	void calculate_value()
	{
		float temp = 0;
		if (layer != 0)
		{
			for (int i = 0; i < input_size; i++)
			{
				temp += weight[i] * input[i]->output;
			}
		}
		else
		{
			temp += weight[0] * input_value;			
		}
		temp += base_weight;
		weight_sum = temp;
		output = logistic_function(temp);		
	}

	// od -1 do 1
	float logistic_function(float input)
	{
		float val = (2.0 / (1 + std::exp(-k*(input - x0)))) - 1;
		return val;
	}

	void weight_changer(float u)
	{
		if (input.size() != 0)
		{
			for (int i = 0; i < input_size; i++)
			{
				weight[i] = weight[i] + u*mistake*((2 * k*exp(-k*(weight_sum - x0))) / (pow((exp(-k*(weight_sum - x0)) + 1), 2)))*input[i]->output;
			}
		}
		else
			weight[0] = weight[0] + u*mistake*((2 * k*exp(-k*(weight_sum - x0))) / (pow((exp(-k*(weight_sum - x0)) + 1), 2)))*input_value;
		base_weight = base_weight + u*mistake*((2 * k*exp(-k*(weight_sum - x0))) / (pow((exp(-k*(weight_sum - x0)) + 1), 2)))*1;
	}

	void set_input_value(float input)
	{
		input_value = input;
	}


	~neuron();
};
