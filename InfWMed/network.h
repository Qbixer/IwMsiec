#pragma once
#include <complex>
#include "neuron.h"
#include <vector>
#include "input.h"

class network
{
public:
	std::vector<std::vector<neuron*>> siec;
	float u;
	int k = 0;
	float ** matrix_expected = nullptr;
	input *in = nullptr;

	network(float offset, float u, char *filename, char* filename_expected, int layers)
	{
		this->in = new input(offset, filename,filename_expected);
		this->u = u;
		int neurons = offset*offset;
		std::vector<neuron*> tmp = std::vector<neuron*>();
		for (int i = 0; i < layers - 1; i++)
		{
			tmp.clear();
			for (int j = 0; j < neurons; j++)
			{
				if (i == 0)
				{
					tmp.push_back(new neuron(0.0f, 0));
				}
				else
				{
					tmp.push_back(new neuron(siec[i - 1], i));
				}
				if (j == neurons - 1)
				{
					siec.push_back(tmp);
				}
			}
		}
		tmp.clear();
		tmp.push_back(new neuron(siec[layers - 2], layers - 1));
		siec.push_back(tmp);
	}

	void one_iteration_matrix(bool p)
	{
		int x = 0;
		int y = 0;
		if (p && matrix_expected == nullptr)
		{
			matrix_expected = new float*[in->width];
			for (int i = 0; i < in->width; i++)
			{
				matrix_expected[i] = new float[in->height];
				for (int k = 0; k < in->height; k++)
					matrix_expected[i][k] = 0.0;
			}
		}
		while (in->take_new_window())
		{
			int tmp = 0;

			//Pobranie nowego elemtu do przetwarzania
			for (int i = 0; i < in->offset; i++)
			{
				for (int j = 0; j < in->offset; j++)
				{
					siec[0][tmp]->set_input_value(in->window[i][j]);
					tmp++;
				}
			}
			//Przetwarzanie
			for (int i = 0; i < siec.size(); i++)
			{
				for (int j = 0; j < siec[i].size(); j++)
				{
					siec[i][j]->calculate_value();
				}
			}
			if (p)
			{
				matrix_expected[x][y] = (siec[siec.size() - 1][0]->output+1)/2;
				x++;
				//gdy x jest juz za duzy
				if (x >= in->width)
				{
					x = 0;
					y++;
				}
				//gdy y jest za duzy - blad
				if (y >= in->height)
				{
					x = 0;
					y = 0;
				}
				printf("%d\n",matrix_expected[x][y]);
			}
			else {
				//Propagacja b³êdu	
				error_propagation(in->expected);
				printf("%d\n", k);
				k++;
			}
		}

	}
	

	void error_propagation(float expected_value)
	{
		/*
		float result = siec[siec.size() - 1][0]->output;
		if (result < -1)
			result = -1;
		if (result > 1)
			result = 1;
		float diff = (in->expected * 2 - 1) - siec[siec.size() - 1][0]->output;
		*/
		siec[siec.size() - 1][0]->mistake = (expected_value*2-1) - siec[siec.size() - 1][0]->output;
		//wybor warstwy
		for (int i = siec.size() - 2; i >= 0; i--)
		{
			//zerowanie bledu
			for (int j = 0; j < siec[i].size(); j++)
				siec[i][j]->mistake = 0;
			//wybor neuronu w warstwy kolejnej
			for (int j = 0; j<siec[i + 1].size(); j++)
			{
				//wybor neuronu z danej warstwy i
				for (int k = 0; k<siec[i].size(); k++)
				{
					siec[i][k]->mistake += siec[i + 1][j]->mistake*siec[i + 1][j]->weight[k];
				}
				//po wykorzystaniu neuronu z warsty wyzszej przeliczamy dla niego nowa wage
				siec[i + 1][j]->weight_changer(u);
			}
			//na samym koncu przelicz nowe wagi dla warstwy pierwszej
			if (i == 0)
			{
				for (int j = 0; j<siec[i].size(); j++)
				{
					siec[i][j]->weight_changer(u);
				}
			}
		}
	}



};
