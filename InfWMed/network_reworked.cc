#include "network_reworked.h"
#include <algorithm>
#include <iostream>

void network_reworked::initalize_matrix()
{
	window = new double*[offset * 2 + 1];
	for (int i = 0; i < offset * 2 + 1; i++)
	{
		window[i] = new double[offset * 2 + 1];
	}
}

void network_reworked::delete_matrix()
{
	for (int i = 0; i < offset * 2 + 1; i++)
	{
		delete[] window[i];
	}
	delete[] window;
}

void network_reworked::open_text(std::string folder)
{
	folder.append("/");
	folder.append(std::to_string(offset));
	folder.append("_");

	std::string tmp = folder;
	tmp.append("tp.txt");
	char* file = new char[tmp.size() + 1];
	strcpy(file, tmp.c_str());
	tp.open(file, std::fstream::in);
	delete[] file;

	tmp = folder;
	tmp.append("tn.txt");
	file = new char[tmp.size() + 1];
	strcpy(file, tmp.c_str());
	tn.open(file, std::fstream::in);
	delete[] file;

	tmp = folder;
	tmp.append("pnn.txt");
	file = new char[tmp.size() + 1];
	strcpy(file, tmp.c_str());
	pnn.open(file, std::fstream::in);
	delete[] file;

	tmp = folder;
	tmp.append("nnp.txt");
	file = new char[tmp.size() + 1];
	strcpy(file, tmp.c_str());
	nnp.open(file, std::fstream::in);
	delete[] file;

	tmp = folder;
	tmp.append("test_set.txt");
	file = new char[tmp.size() + 1];
	strcpy(file, tmp.c_str());
	text_test_set.open(file, std::fstream::trunc);
	delete[] file;
}

void network_reworked::open_text_weights(int i, std::string folder)
{
	folder.append("/");
	folder.append(std::to_string(offset));
	folder.append("_");
	folder.append(std::to_string(i));
	folder.append(".txt");


	char* file = new char[folder.size() + 1];
	strcpy(file, folder.c_str());
	text_weights.open(file, std::fstream::trunc);
	delete[] file;
}

void network_reworked::load_vectors(double tp_prop, double tn_prop, double pnn_prop, double nnp_prop)
{
	int zmienna = 0;
	std::string dummyLine;
	
	while (!tp.eof() && !tn.eof() && !nnp.eof() && !pnn.eof())
	{
		double ** window = new double*[offset * 2 + 1];
		double ** window2 = new double*[offset * 2 + 1];
		double ** window3 = new double*[offset * 2 + 1];
		double ** window4 = new double*[offset * 2 + 1];
		for (int i = 0; i < offset * 2 + 1; i++)
		{
			window[i] = new double[offset * 2 + 1];
			window2[i] = new double[offset * 2 + 1];
			window3[i] = new double[offset * 2 + 1];
			window4[i] = new double[offset * 2 + 1];
			for (int j = 0; j < offset * 2 + 1; j++)
			{
				tp >> window[i][j];
				tn >> window2[i][j];
				nnp >> window3[i][j];
				pnn >> window4[i][j];
			}
		}
		test_true_positive[zmienna].push_back(window);
		test_true_negative[zmienna].push_back(window2);
		test_negative_near_positive[zmienna].push_back(window3);
		test_positive_near_negative[zmienna].push_back(window4);
		getline(tp, dummyLine);
		getline(tn, dummyLine);
		getline(nnp, dummyLine);
		getline(pnn, dummyLine);
		zmienna = (zmienna + 1) % max_set_count;
	}
	int set_size = test_true_positive[max_set_count - 1].size();
	double m1 = std::max(tp_prop, tn_prop);
	double m2 = std::max(pnn_prop, nnp_prop);
	double m3 = std::max(m1, m2);
	if (tp_prop > 0)
		tp_set = set_size / (m3 / tp_prop);
	else
		tp_set = 0;
	if (tn_prop > 0)
		tn_set = set_size / (m3 / tn_prop);
	else
		tn_set = 0;
	if (pnn_prop > 0)
		pnn_set = set_size / (m3 / pnn_prop);
	else
		pnn_set = 0;
	if (nnp_prop > 0)
		nnp_set = set_size / (m3 / nnp_prop);
	else
		nnp_set = 0;
}

void network_reworked::create_new_web(int layers)
{
	int neurons = (2 * offset + 1)*(2 * offset + 1);
	std::vector<neuron_reworked*> tmp = std::vector<neuron_reworked*>();
	if (siec.size() == 0)
	{
		for (int i = 0; i < layers - 1; i++)
		{
			tmp.clear();
			if (i == 0)
			{
				for (int j = 0; j < neurons; j++)
				{

					tmp.push_back(new neuron_reworked());
				}
			}
			else
			{
				int dana;
				std::cout << "Podaj liczbe neuronow warstwy " + std::to_string(i) + "\n";
				std::cin >> dana;
				for (int j = 0; j < dana; j++)
				{
					tmp.push_back(new neuron_reworked(siec[i - 1], i));
				}
			}
			siec.push_back(tmp);
		}
		tmp.clear();
		tmp.push_back(new neuron_reworked(siec[layers - 2], layers - 1));
		siec.push_back(tmp);
	}
	else
	{
		for (int i = 0; i < layers; i++)
		{
			for (int j = 0; j < siec[i].size(); j++)
			{
				siec[i][j]->reset_weight();
			}
		}
	}
}

void network_reworked::teach_network(int layers, std::string folder)
{
	for (int i = 0;i<max_set_count;i++)
	{
		create_new_web(layers);
		for (int j = 0;j<max_set_count;j++)
		{
			if (i == j)
				continue;
			one_set_iteration(j, learning_set);
		}
		ttp = 0;
		ttn = 0;
		tfp = 0;
		tfn = 0;
		one_set_iteration(i, test_set);
		output_weights(i, folder);
	}
	text_test_set.close();
}

void network_reworked::insert_data(double** matrix)
{
	int tmp = 0;
	for (int i = 0; i < offset * 2 + 1; i++)
	{
		for (int j = 0;j < offset * 2 + 1;j++)
		{
			siec[0][tmp]->set_input_value(matrix[i][j]);
			tmp++;
		}

	}
}

void network_reworked::calculate()
{
	for (int i = 0; i < siec.size(); i++)
	{
		for (int j = 0; j < siec[i].size(); j++)
		{
			siec[i][j]->calculate_value();
		}
	}
}

void network_reworked::error_propagation(double expected_value)
{

	siec[siec.size() - 1][0]->mistake = expected_value - siec[siec.size() - 1][0]->output;
	if (siec[siec.size() - 1][0]->mistake == 0)
		return;
	//wybor warstwy (oprocz ostatniej) od tylu
	for (int i = siec.size() - 2; i >= 0; i--)
	{
		//zerowanie bledu neuronow danej warstwy i
		for (int j = 0; j < siec[i].size(); j++)
			siec[i][j]->mistake = 0;
		//wybor neuronu w warstwy kolejnej (i+1)
		for (int j = 0; j < siec[i + 1].size(); j++)
		{
			//wybor neuronu z danej warstwy i
			for (int k = 0; k < siec[i].size(); k++)
			{
				//Propagacja bledu po starych wagach
				siec[i][k]->mistake += (siec[i + 1][j]->mistake*siec[i + 1][j]->weight[k]);
			}
			//po wykorzystaniu neuronu z warsty wyzszej przeliczamy dla niego nowa wage
			siec[i + 1][j]->weight_changer(u);
		}
	}
}

void network_reworked::output_weights(int k_set, std::string folder)
{	
	open_text_weights(k_set, folder);
	for (int i = 0; i < siec.size(); i++)
	{
		for (int j = 0; j < siec[i].size(); j++)
		{
			if (i == 0)
				text_weights << siec[i][j]->weight[0] << "\t" << siec[i][j]->base_weight << "\n";
			else
			{
				for (int k = 0;k < siec[i - 1].size();k++)
					text_weights << siec[i][j]->weight[k] << "\t";
				text_weights << siec[i][j]->base_weight << "\n";
			}
		}
	}
	text_weights.close();
	text_test_set << "Test set number: " << std::to_string(k_set) << "\n";
	text_test_set << "True positive: " << ttp << "\t" << "False positive: " << tfp << "\n";
	text_test_set << "False negative: " << tfn << "\t" << "True negative: " << ttn << "\n\n";
	
}

void network_reworked::one_set_iteration(int k_set, type_of_set type)
{
	int tp_cur = 0, tn_cur = 0,pnn_cur = 0,nnp_cur = 0;	
	int which = -1;
	int suma = tp_set + tn_set + pnn_set + nnp_set;
	int expected = 0.0f;
	while (suma != (tp_cur + tn_cur + pnn_cur + nnp_cur))
	{
		int which = rand() % suma;
		//true_positive
		if (which < tp_set && tp_cur < tp_set)
		{
			expected = 1.0f;
			insert_data(test_true_positive[k_set][tp_cur]);
			tp_cur++;
		}
		//true_negative
		else if (which < tp_set + tn_set && tn_cur < tn_set)
		{
			expected = -1.0f;
			insert_data(test_true_negative[k_set][tn_cur]);
			tn_cur++;
		}
		//positive_near_negative
		else if (which < tp_set + tn_set + pnn_set && pnn_cur < pnn_set)
		{
			expected = 1.0f;
			insert_data(test_positive_near_negative[k_set][pnn_cur]);
			pnn_cur++;
		}
		//negative_near_positive
		else if (which < tp_set + tn_set + pnn_set + nnp_set && nnp_cur < nnp_set)
		{
			expected = -1.0f;
			insert_data(test_negative_near_positive[k_set][nnp_cur]);
			nnp_cur++;
		}
		else
			continue;

		calculate();

		if (type == test_set)
		{
			if (expected > 0)
				if (siec[siec.size() - 1][0]->output > 0)
					ttp++;
				else
					tfn++;
			else
				if (siec[siec.size() - 1][0]->output < 0)
					ttn++;
				else
					tfp++;
		}
		else
			error_propagation(expected);
		//std::cout << siec[siec.size() - 1][0]->output << "\t\t" << expected << "\n";
	}
}

network_reworked::network_reworked(int offset, double u)
{
	this->offset = offset;
	this->u = u;
}
