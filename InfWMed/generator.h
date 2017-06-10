#pragma once
#ifndef GENERATOR_H_
#define GENERATOR_H_
#include <allegro5/bitmap.h>
#include "enums.h"
#include <string>
#include <fstream>
#include "network_reworked.h"

class generator
{
public:
	generator(int offset, int set_size);
	generator(int offset, std::string folder);
	void open_graphics(char* filename, char *filename_expected, char *filename_mask);
	void initiate_matrix();
	void destroy_graphics();
	void delete_matrices();
	void open_text(std::string folder);
	

	void generate_pictures(std::string folder);
	void generate_data();	
	void close_file();
	

	network_reworked * net;
private:
	double get_value_from_pixel(int i, int j, type_of_matrix typ);
	bool take_random_window();
	bool take_new_window(network_reworked * net, ALLEGRO_BITMAP* display);
	void input_weights(std::string folder, int i);
	void prepare_picture();

	ALLEGRO_BITMAP *image = nullptr;
	ALLEGRO_BITMAP *image_expected = nullptr;
	ALLEGRO_BITMAP *image_mask = nullptr;

	int width;
	int height;

	double ** matrix;
	double ** matrix_expected;
	double ** matrix_mask;
	double ** window;

	int set_size, sum;
	int offset;

	std::ofstream tn,tp,pnn,nnp,stats;
	
	int ttp, ttn, tfp, tfn, mse, cur_x = 0, cur_y = 0;

	int itp, itn, innp, ipnn;
};



#endif