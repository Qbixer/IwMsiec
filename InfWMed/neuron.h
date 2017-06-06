#pragma once
#include <complex>
#include <vector>

using namespace std;
class neuron
{
public:
	std::vector<neuron*> input;
	//tylko gdy layer = 0
	double input_value;
	vector<double> weight;
	double base_weight;
	double output = 0;
	int input_size;
	double mistake = 0;
	double weight_sum;
	int layer;

	neuron(double input_value, int layer)
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
			//this->weight[0] = static_cast <double> (rand()) / static_cast <double> (RAND_MAX) / 2 + 0.5;
			this->weight.push_back(1);
			this->base_weight = 0;
			//this->weight.push_back(0.5f);
		}
		else
		{
			for (int i = 0; i < input.size(); i++)
			{
				//this->weight[i] = static_cast <double> (rand()) / static_cast <double> (RAND_MAX) / 2 + 0.5;
				this->weight.push_back(static_cast <double> (rand()) / static_cast <double> (RAND_MAX)-0.5f);
				//this->weight.push_back(0.5f);
			}
			//this->base_weight = static_cast <double> (rand()) / static_cast <double> (RAND_MAX);
			this->base_weight = static_cast <double> (rand()) / static_cast <double> (RAND_MAX) - 0.5f;
			//this->base_weight = 0.5;
		}
		
	}

	void calculate_value()
	{
		
		double temp = 0;
		if (layer != 0)
		{
			for (int i = 0; i < input_size; i++)
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

	//Niby OK
	/*void weight_changer(double u)
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
	void weight_changer(double u)
	{
		if (layer != 0) 
		{
			double j = (cosh(2 * weight_sum) + 1);
			double k = (2 / j);
			if (input.size() != 0)
			{
				for (int i = 0; i < input_size; i++)
				{
					//Niby OK				
					weight[i] = weight[i] + u*mistake*(2 / (cosh(2 * weight_sum) + 1))*input[i]->output;
				}
			}
			else
				weight[0] = weight[0] + u*mistake*(2 / (cosh(2 * weight_sum) + 1))*input_value;
			base_weight = base_weight + u*mistake*(2 / (cosh(2 * weight_sum) + 1)) * (-1);
		}
	}

	~neuron();
};
