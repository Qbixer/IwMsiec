#pragma once
#include <complex>
#include "neuron.h"
#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <cmath>

using namespace std;
class network
{
public:
	const static int max_set_count = 10;
	std::vector<std::vector<neuron*>> siec;
	double u;
	std::vector<double**> test_true_positive[max_set_count];
	std::vector<double**> test_true_negative[max_set_count];
	std::vector<double**> test_negative_near_positive[max_set_count];
	std::vector<double**> test_positive_near_negative[max_set_count];
	double ** window;
	int tp_set, tn_set, pnn_set, nnp_set;
	int tp_cur = 0, tn_cur = 0, pnn_cur = 0, nnp_cur = 0;
	ifstream tp;
	ifstream tn;
	ifstream nnp;
	ifstream pnn;
	int ttp, ttn, tfp, tfn;

	network(int offset, double u,int layers, int typ, int tp_prop,int tn_prop,int pnn_prop, int nnp_prop)
	{	
		this->u = u;
		initalize_matrix(offset);
		load_vectors(offset, typ, tp_prop, tn_prop, pnn_prop, nnp_prop);
		teach_network(offset, layers, typ);
	}

	network(int offset, int layers)
	{
		initalize_matrix(offset);
		create_new_web(offset, layers);
	}
	//Work as intended
	void load_vectors(int offset, int typ, int tp_prop, int tn_prop, int pnn_prop, int nnp_prop)
	{
		switch (typ)
		{
		case 1:
		{
			tp.open("healthy/tp.txt", fstream::in);
			tn.open("healthy/tn.txt", fstream::in);
			nnp.open("healthy/nnp.txt", fstream::in);
			pnn.open("healthy/pnn.txt", fstream::in);
			break;
		}
		case 2:
		{
			tp.open("glaucoma/tp.txt", fstream::in);
			tn.open("glaucoma/tn.txt", fstream::in);
			nnp.open("glaucoma/nnp.txt", fstream::in);
			pnn.open("glaucoma/pnn.txt", fstream::in);
			break;
		}
		case 3:
		{
			tp.open("diabetic_retinopathy/tp.txt", fstream::in);
			tn.open("diabetic_retinopathy/tn.txt", fstream::in);
			nnp.open("diabetic_retinopathy/nnp.txt", fstream::in);
			pnn.open("diabetic_retinopathy/pnn.txt", fstream::in);
			break;
		}
		case 4:
		{
			tp.open("test/tp.txt", fstream::in);
			tn.open("test/tn.txt", fstream::in);
			nnp.open("test/nnp.txt", fstream::in);
			pnn.open("test/pnn.txt", fstream::in);
			break;
		}
		}
		int zmienna = 0;
		string dummyLine;
		double ** window;
		double ** window2;
		double ** window3;
		double ** window4;
		while (!tp.eof() && !tn.eof() && !nnp.eof() && !pnn.eof())
		{
			window = new double*[offset * 2 + 1];
			window2 = new double*[offset * 2 + 1];
			window3 = new double*[offset * 2 + 1];
			window4 = new double*[offset * 2 + 1];
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
		int m1, m2, m3;
		m1 = max(tp_prop, tn_prop);
		m2 = max(pnn_prop, nnp_prop);
		m3 = max(m1, m2);
		tp_set = set_size / (m3 / tp_prop);
		tn_set = set_size / (m3 / tn_prop);
		pnn_set = set_size / (m3 / pnn_prop);
		nnp_set = set_size / (m3 / nnp_prop);
	}
	//Work as intended
	void initalize_matrix(int offset)
	{
		window = new double*[offset * 2 + 1];
		for (int i = 0; i < offset * 2 + 1; i++)
		{
			window[i] = new double[offset * 2 + 1];
		}
	}
	//Work as intended
	void create_new_web(int offset, int layers)
	{
		int neurons = (2*offset+1)*(2*offset+1);
		std::vector<neuron*> tmp = std::vector<neuron*>();
		if (siec.size() == 0)
		{
			for (int i = 0; i < layers - 1; i++)
			{
				tmp.clear();
				if (i == 0)
				{
					for (int j = 0; j < neurons; j++)
					{

						tmp.push_back(new neuron(0.0f, 0));
					}
				}
				else
				{
					int dana;
					cout << "Podaj liczbe neuronow warstwy " + to_string(i) + "\n";
					cin >> dana;
					for (int j = 0; j < dana; j++)
					{
						tmp.push_back(new neuron(siec[i - 1], i));
					}
				}
				siec.push_back(tmp);
			}
			tmp.clear();
			tmp.push_back(new neuron(siec[layers - 2], layers - 1));
			siec.push_back(tmp);
		}
		else
		{
			for (int i = 0; i < layers - 1; i++)
			{
				for (int j = 0; j < siec[i].size(); j++)
				{
					siec[i][j]->reset_weights(i);
				}
			}
			siec[layers - 1][0]->reset_weights(layers - 1);
		}
	}
	//Work as intended
	void insert_data(int offset, double** matrix)
	{
		int tmp = 0;
		for (int i = 0; i < offset * 2 + 1; i++)
		{
			for (int j = 0;j < offset * 2 + 1;j++)
			{
				siec[0][tmp]->input_value = matrix[i][j];
				tmp++;
			}

		}
	}

	//Sprawdzone teoretycznie - wszystko dobrze
	void error_propagation(double expected_value)
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
			for (int j = 0; j<siec[i + 1].size(); j++)
			{
				/*double tmp = 0;
				for (int k = 0; k<siec[i+1][j]->weight.size(); k++)
				{
					tmp += siec[i+1][j]->weight[k];
				}
				tmp += siec[i + 1][j]->base_weight;*/
				//wybor neuronu z danej warstwy i
				for (int k = 0; k<siec[i].size(); k++)
				{
					//Propagacja bledu po starych wagach
					siec[i][k]->mistake += (siec[i + 1][j]->mistake*siec[i + 1][j]->weight[k]);
				}
				//po wykorzystaniu neuronu z warsty wyzszej przeliczamy dla niego nowa wage
				siec[i + 1][j]->weight_changer(u);
			}
		}
		//na samym koncu przelicz nowe wagi dla warstwy pierwszej
		/*for (int i = 0; i<siec[0].size(); i++)
		{
			siec[0][i]->weight_changer(u);
		}*/
	}
	
	void teach_network(int offset, int layers, int typ)
	{
		for(int i = 0;i<max_set_count;i++)
		{
			create_new_web(offset, layers);
			for(int j = 0;j<max_set_count;j++)
			{
				if(i==j)
					continue;
				one_set_iteration(offset, j);
			}
			ttp = 0;
			ttn = 0;
			tfp = 0;
			tfn = 0;
			one_set_iteration(offset, i,1);		
			output_weights(offset, typ,i);
		}		

	}

	void one_set_iteration(int offset, int k_set, int typ = 0)
	{
		tp_cur = 0;
		tn_cur = 0;
		pnn_cur = 0;
		nnp_cur = 0;
		int which = -1;
		int suma = tp_set + tn_set + pnn_set + nnp_set;
		int expected = 0.0f;
		while (suma != (tp_cur + tn_cur + pnn_cur + nnp_cur))
		{
			
			int which = rand() % suma;
			//which++;
			//true_positive
			if (which < tp_set && tp_cur < tp_set)
			{
				expected = 1.0f;
				insert_data(offset, test_true_positive[k_set][tp_cur]);
				tp_cur++;
			}
			//true_negative
			else if (which < tp_set + tn_set && tn_cur < tn_set)
			{
				expected = -1.0f;
				insert_data(offset, test_true_negative[k_set][tn_cur]);
				tn_cur++;
			}
			//positive_near_negative
			else if (which < tp_set + tn_set + pnn_set && pnn_cur < pnn_set)
			{
				expected = 1.0f;
				insert_data(offset, test_positive_near_negative[k_set][pnn_cur]);
				pnn_cur++;
			}
			//negative_near_positive
			else if (which < tp_set + tn_set + pnn_set + nnp_set && nnp_cur < nnp_set)
			{
				expected = -1.0f;
				insert_data(offset, test_negative_near_positive[k_set][nnp_cur]);
				nnp_cur++;
			}
			else
				continue;

			//Przetwarzanie
			calculate();
			
			if (typ != 0)
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
				//Propagacja b³êdu	
				error_propagation(expected);
			cout << siec[siec.size() - 1][0]->output << "\t\t" << expected << "\n";
		}
	}

	void calculate()
	{
		for (int i = 0; i < siec.size(); i++)
		{
			for (int j = 0; j < siec[i].size(); j++)
			{
				siec[i][j]->calculate_value();
			}
		}
	}
	
	void output_weights(int offset, int typ, int k_set)
	{
		ofstream test;
		string a;
		string b;
		switch (typ)
		{
		case 1:
		{
			a += "healthy/";
			a += to_string(offset);
			a += "_";
			a += to_string(k_set);
			a += "_h.txt";
			break;
		}
		case 2:
		{
			a += "glaucoma/";
			a += to_string(offset);
			a += "_";
			a += to_string(k_set);
			a += "_g.txt";
			break;
		}
		case 3:
		{
			a += "diabetic_retinopathy/";
			a += to_string(offset);
			a += "_";
			a += to_string(k_set);
			a += "_dr.txt";
			break;
		}
		case 4:
		{
			a += "test/";
			a += to_string(offset);
			a += "_";
			a += to_string(k_set);
			a += "_t.txt";
			break;
		}
		}
		char* path = new char[a.size() + 1];
		strcpy(path, a.c_str());
		b = "wyniki/" + a;
		char* path2 = new char[b.size() + 1];
		strcpy(path2, b.c_str());
		test.open(path, fstream::app);
		for (int i = 0; i < siec.size(); i++)
		{
			for (int j = 0; j < siec[i].size(); j++)
			{
				if (i == 0)
					test << siec[i][j]->weight[0] << "\t" << siec[i][j]->base_weight << "\n";
				else
				{
					for (int k = 0;k < siec[i-1].size();k++)
						test << siec[i][j]->weight[k] << "\t";
					test << siec[i][j]->base_weight << "\n";
				}
			}
		}
		test.close();
		test.open(path2, fstream::app);
		test << "True positive: " << ttp << "\t" << "False positive: " << tfp << "\n";
		test << "False negative: " << tfn << "\t" << "True negative: " << ttn;
		test.close();
	}

	



};
