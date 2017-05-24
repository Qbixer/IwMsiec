#pragma once
#include <complex>
#include <vector>
#include <iostream>

using namespace std;
class neuron
{
public:
	std::vector<neuron*> input;
	//tylko gdy layer = 0
	float input_value;
	vector<float> weight;
	float base_weight;
	float k = 1;
	float x0 = 0;
	float output = 0;
	int input_size;
	float mistake = 0;
	float weight_sum;
	//0 - wejscie, inne = i
	int layer;

	neuron(float input_value, int layer)
	{
		this->input_value = input_value;
		this->layer = layer;
		reset_weights(layer);
	}

	neuron(std::vector<neuron*> input, int layer)
	{

		this->input = input;
		this->input_size = input.size();
		this->layer = layer;
		reset_weights(layer);
	}

	//Work as intended
	void reset_weights(int layer)
	{
		this->weight.clear();
		if(layer == 0)
		{
			//this->weight[0] = static_cast <float> (rand()) / static_cast <float> (RAND_MAX) / 2 + 0.5;
			this->weight.push_back(static_cast <float> (rand()) / static_cast <float> (RAND_MAX)-0.5f);
			//this->weight.push_back(0.5f);
		}
		else
		{
			for (int i = 0; i < input.size(); i++)
			{
				//this->weight[i] = static_cast <float> (rand()) / static_cast <float> (RAND_MAX) / 2 + 0.5;
				this->weight.push_back(static_cast <float> (rand()) / static_cast <float> (RAND_MAX)-0.5f);
				//this->weight.push_back(0.5f);
			}
		}
		//this->base_weight = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		this->base_weight = static_cast <float> (rand()) / static_cast <float> (RAND_MAX) - 0.5f;
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
		temp += base_weight*(-1);
		weight_sum = temp;
		output = tanh(temp);		
	}

	
	// od -1 do 1
	//Niby OK
	float logistic_function(float input)
	{
		float val = (2.0 / (1 + std::exp(-k*(input - x0)))) - 1;
		return val;
	}
	//Niby OK
	/*void weight_changer(float u)
	{
		if (input.size() != 0)
		{
			for (int i = 0; i < input_size; i++)
			{
				//Niby OK
				weight[i] = weight[i] + u*mistake*((2 * k*exp(-k*(weight_sum - x0))) / ((exp(-k*(weight_sum - x0)) + 1)*(exp(-k*(weight_sum - x0)) + 1)))*input[i]->output;
			}
		}
		else
			weight[0] = weight[0] + u*mistake*((2 * k*exp(-k*(weight_sum - x0))) / ((exp(-k*(weight_sum - x0)) + 1)*(exp(-k*(weight_sum - x0)) + 1)))*input_value;
		base_weight = base_weight + u*mistake*((2 * k*exp(-k*(weight_sum - x0))) / ((exp(-k*(weight_sum - x0)) + 1)*(exp(-k*(weight_sum - x0)) + 1))) *1;
	}*/
	void weight_changer(float u)
	{
		if (input.size() != 0)
		{
			for (int i = 0; i < input_size; i++)
			{
				//Niby OK
				float k = (2 / (cosh(2 * weight_sum) + 1));
				weight[i] = weight[i] + u*mistake*(2/(cosh(2*weight_sum)+1))*input[i]->output;
			}
		}
		else
			weight[0] = weight[0] + u*mistake*(2 / (cosh(2 * weight_sum) + 1))*input_value;
		base_weight = base_weight + u*mistake*(2 / (cosh(2 * weight_sum) + 1)) * (-1);
	}

	~neuron();
};
