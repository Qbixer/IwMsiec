#pragma once
#ifndef NETWORK_REWORKED_H_
#define NETWORK_REWORKED_H_
#include <string>
#include <fstream>
#include <vector>
#include "neuron_reworked.h"
#include "enums.h"

class network_reworked
{
public:
	network_reworked(int offset, double u);
	network_reworked(int offset);
	void initalize_matrix();
	void delete_matrix();
	void open_text(std::string folder);
	void load_vectors(double tp_prop, double tn_prop, double pnn_prop, double nnp_prop);
	void teach_network(int layers, std::string folder);
	void create_new_web(int layers);
	void calculate();
	void insert_data(double** matrix);

	
	std::vector<std::vector<neuron_reworked*>> siec;
	const static int max_set_count = 10;
private:
	
	void one_set_iteration(int k_set, type_of_set type);
	
	
	void error_propagation(double expected_value);
	void open_text_weights(int i, std::string folder);
	void output_weights(int k_set, std::string folder);

	

	double u;
	
	int offset;
	double ** window;

	std::ifstream tp,tn,pnn,nnp;;
	std::ofstream text_test_set,text_weights;
	double tp_set, tn_set, pnn_set, nnp_set;
	int ttp, ttn, tfp, tfn;

	std::vector<double**> test_true_positive[max_set_count], test_true_negative[max_set_count], test_negative_near_positive[max_set_count], test_positive_near_negative[max_set_count];

};



#endif
