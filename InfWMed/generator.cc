#include "generator.h"
#include <allegro5/bitmap_io.h>
#include <iostream>
#include <allegro5/bitmap_lock.h>
#include <allegro5/shader.h>
#include <allegro5/allegro.h>

generator::generator(int offset, int set_size)
{
	this->offset = offset;
	this->set_size = set_size;
	sum = 4 * set_size;
}

generator::generator(int offset, std::string folder)
{
	this->offset = offset;
	folder = "obrazy/" + folder;	
	folder.append("/");
	folder.append(std::to_string(offset));
	folder.append("_stats.txt");
	char* file = new char[folder.size() + 1];
	strcpy(file, folder.c_str());
	stats.open(file, std::fstream::trunc);
	delete[] file;	
}



void generator::draw_pixel()
{
	if (net->siec[net->siec.size() - 1][0]->output < 0)
	{
		if (matrix_expected[cur_x][cur_y] < 0.5)
			al_draw_pixel(cur_x, cur_y, al_map_rgb(0, 0, 0));
		else
			al_draw_pixel(cur_x, cur_y, al_map_rgb(0, 0, 0));
	}
	else
	{
		if (matrix_expected[cur_x][cur_y] > 0.5)
			al_draw_pixel(cur_x, cur_y, al_map_rgb(0, 255, 0));
		else
			al_draw_pixel(cur_x, cur_y, al_map_rgb(0, 255, 0));
	}
}

bool generator::take_new_window(network_reworked* net, ALLEGRO_BITMAP* display)
{
	//gdy x jest juz za duzy
	if (cur_x >= width)
	{
		cur_x = 0;
		cur_y++;
	}
	//gdy y jest za duzy - blad
	if (cur_y >= height)
	{
		cur_x = 0;
		cur_y = 0;
		return false;
	}
	if (matrix_mask[cur_x][cur_y] == 0)
	{
		cur_x++;
		return true;
	}

	//okno jest od -offset - 0 (srodek badany) - offset
	for (int i = -offset; i <= offset; i++)
	{
		for (int j = -offset; j <= offset; j++)
		{
			window[i + offset][j + offset] = matrix[cur_x + offset + i][cur_y + offset + j];
		}
	}
	net->insert_data(window);
	net->calculate();
	draw_pixel();

	if (matrix_expected[cur_x][cur_y] > 0.5)
		if (net->siec[net->siec.size() - 1][0]->output > 0)
			ttp++;
		else
			tfp++;
	else
		if (net->siec[net->siec.size() - 1][0]->output < 0)
			ttn++;
		else
			tfn++;
	if (net->siec[net->siec.size() - 1][0]->output > 0)
		mse += (1 - matrix_expected[cur_x][cur_y])*(1 - matrix_expected[cur_x][cur_y]);
	else
		mse += (0 - matrix_expected[cur_x][cur_y])*(0 - matrix_expected[cur_x][cur_y]);


	cur_x++;
	return true;
}

void generator::generate_pictures(std::string folder)
{
	for (int i = 0;i < network_reworked::max_set_count;i++)
	{
		std::cout << "Przetwarzanie:" << i << "\n";
		input_weights(folder, i);
		std::string pom = "obrazy/" + folder;
		pom.append("/");
		pom.append(std::to_string(offset));
		pom.append("_");
		pom.append(std::to_string(i));
		pom.append(".png");
		ALLEGRO_BITMAP* display = al_create_bitmap(width, height);
		al_set_target_bitmap(display);
		al_clear_to_color(al_map_rgb(0, 0, 0));
		al_draw_bitmap(image, 0, 0, NULL);
		ALLEGRO_BITMAP* mask = al_create_bitmap(width, height);
		al_set_target_bitmap(mask);
		al_clear_to_color(al_map_rgb(0, 0, 0));
		ttp = 0;
		ttn = 0;
		tfp = 0;
		tfn = 0;
		mse = 0;
		while (take_new_window(net, display));
		char* path = new char[pom.size() + 1];
		strcpy(path, pom.c_str());
		al_set_target_bitmap(display);
		al_draw_tinted_bitmap(mask, al_map_rgba_f(1, 1, 1, 0.1), 0, 0, 0);
		al_save_bitmap(path, display);
		stats << i << "\n";
		stats << "True positive: " << ttp << "\t" << "False positive: " << tfp << "\n";
		stats << "False negative: " << tfn << "\t" << "True negative: " << ttn << "\n";
		double acc = double(ttp + ttn) / double(ttp + ttn + tfp + tfn);
		double pre = double(ttp) / double(ttp + tfp);
		double sens = double(ttp) / double(ttp + tfn);
		stats << "ACC: " << acc << "\n";
		stats << "Precision: " << pre << "\n";
		stats << "Sensitivity: " << sens << "\n";
		stats << "MSE: " << sqrt(mse) << "\n";	
	}
	stats.close();
}

void generator::input_weights(std::string folder, int i)
{
	std::ifstream text_weights;
	folder.append("/");
	folder.append(std::to_string(offset));
	folder.append("_");
	folder.append(std::to_string(i));
	folder.append(".txt");

	char* path = new char[folder.size() + 1];
	strcpy(path, folder.c_str());
	text_weights.open(path, std::fstream::in);

	std::vector<double> tmp;
	double pom, base;
	for (int i = 0;i < net->siec.size();i++)
	{
		for (int j = 0;j < net->siec[i].size();j++)
		{
			tmp.clear();
			if (i == 0)
			{
				text_weights >> pom;				
				tmp.push_back(pom);
			}
			else
			{
				for (int k = 0;k < net->siec[i - 1].size();k++)
				{
					text_weights >> pom;
					tmp.push_back(pom);
				}
				
			}
			text_weights >> base;
			net->siec[i][j]->input_weight(tmp, base);
		}
	}
	text_weights.close();
}

void generator::open_graphics(char* filename, char* filename_expected, char* filename_mask)
{
	image = al_load_bitmap(filename);
	if (image == NULL)
		std::cout << "Nie moge zaladowac zdjecia glownego\n";
	else
	{
		width = al_get_bitmap_width(image);
		height = al_get_bitmap_height(image);
	}
	image_expected = al_load_bitmap(filename_expected);
	if (image_expected == NULL)
		std::cout << "Nie moge zaladowac zdjecia expected\n";
	image_mask = al_load_bitmap(filename_mask);
	if (image_mask == NULL)
		std::cout << "Nie moge zaladowac zdjecia mask\n";	
	return;
}

double generator::get_value_from_pixel(int i, int j, type_of_matrix typ)
{
	if (typ == matrix_type)
	{
		if (i - offset >= 0 && i - offset < width && j - offset >= 0 && j - offset < height)
			return ((al_get_pixel(image, i - offset, j - offset).r + al_get_pixel(image, i - offset, j - offset).g + al_get_pixel(image, i - offset, j - offset).b) / 3.0) * 2 - 1;
		return -1;
	}
	if(typ == expected_type)
	{
		return al_get_pixel(image_expected, i, j).b;
	}
	if(typ == mask_type)
	{
		return al_get_pixel(image_mask, i, j).b;
	}
}

void generator::prepare_picture()
{
	double min = 2, max = -2;
	/*for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			if (matrix_mask[i][j] > 0.5)
			{
				if (matrix[i+offset][j+ offset]  > max)
					max = matrix[i + offset][j + offset];
				if (matrix[i+ offset][j+ offset]  < min)
					min = matrix[i + offset][j + offset];
			}
		}
	}*/
	min = -0.5;
	max = 0;
	double gap = max - min;
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			if (matrix_mask[i][j] > 0.5)
			{
				matrix[i + offset][j + offset] = (matrix[i + offset][j + offset] - min) / gap * 2 - 1;
			}
		}
	}

}

void generator::initiate_matrix()
{
	matrix = new double*[width + 2 * offset];
	al_lock_bitmap(image, al_get_bitmap_format(image), ALLEGRO_LOCK_READONLY);
	for (int i = 0; i < width + 2 * offset; i++)
	{
		matrix[i] = new double[height + 2 * offset];
		for (int j = 0; j < height + 2 * offset;j++)
		{
			matrix[i][j] = get_value_from_pixel(i, j, matrix_type);
		}
	}

	matrix_expected = new double*[width];
	al_lock_bitmap(image_expected, al_get_bitmap_format(image_expected), ALLEGRO_LOCK_READONLY);
	for (int i = 0; i < width; i++)
	{
		matrix_expected[i] = new double[height];
		for (int j = 0; j < height; j++)
		{
			matrix_expected[i][j] = get_value_from_pixel(i, j, expected_type);
		}
	}

	matrix_mask = new double*[width];
	al_lock_bitmap(image_mask, al_get_bitmap_format(image_mask), ALLEGRO_LOCK_READONLY);
	for (int i = 0; i < width; i++)
	{
		matrix_mask[i] = new double[height];
		for (int j = 0; j < height; j++)
		{
			matrix_mask[i][j] = get_value_from_pixel(i, j, mask_type);
		}
	}

	window = new double*[offset * 2 + 1];
	for (int i = 0; i < offset * 2 + 1; i++)
	{
		window[i] = new double[offset * 2 + 1];
	}

	prepare_picture();
}

void generator::destroy_graphics()
{
	al_destroy_bitmap(image);
	al_destroy_bitmap(image_expected);
	al_destroy_bitmap(image_mask);
}

void generator::delete_matrices()
{
	for (int i = 0; i < width + 2 * offset; i++)
	{
		delete[] matrix[i];
	}
	delete[] matrix;

	for (int i = 0; i < width; i++)
	{
		delete[] matrix_expected[i];
	}
	delete[] matrix_expected;

	for (int i = 0; i < width; i++)
	{
		delete[] matrix_mask[i];
	}
	delete[] matrix_mask;

	for (int i = 0; i < offset * 2 + 1; i++)
	{
		delete[] window[i];
	}
	delete[] window;
}

void generator::open_text(std::string folder)
{
	folder.append("/");
	folder.append(std::to_string(offset));
	folder.append("_");

	std::string tmp = folder;
	tmp.append("tp.txt");
	char* file = new char[tmp.size() + 1];
	strcpy(file, tmp.c_str());
	tp.open(file, std::fstream::trunc);
	delete[] file;

	tmp = folder;
	tmp.append("tn.txt");
	file = new char[tmp.size() + 1];
	strcpy(file, tmp.c_str());
	tn.open(file, std::fstream::trunc);
	delete[] file;

	tmp = folder;
	tmp.append("pnn.txt");
	file = new char[tmp.size() + 1];
	strcpy(file, tmp.c_str());
	pnn.open(file, std::fstream::trunc);
	delete[] file;

	tmp = folder;
	tmp.append("nnp.txt");
	file = new char[tmp.size() + 1];
	strcpy(file, tmp.c_str());
	nnp.open(file, std::fstream::trunc);
	delete[] file;

}

void generator::close_file()
{
	tp.close();
	tn.close();
	nnp.close();
	pnn.close();
}

void generator::generate_data()
{
	itp = 0;
	itn = 0;
	innp = 0;
	ipnn = 0;
	while (take_random_window());	
}

bool generator::take_random_window()
{
	if (itp + itn + innp + ipnn == sum)
		return false;
	int x = rand() % width;
	int y = rand() % height;
	int near_offset = 1;
	if (matrix_mask[x][y] == 1)
	{
		if (offset == 0)
		{
			if (matrix_expected[x][y] > 0.5 && itp == set_size && ipnn < set_size)
			{
				ipnn++;
				pnn << matrix[x][y] << "\n\n";
			}
			if (matrix_expected[x][y] > 0.5 && itp < set_size)
			{
				itp++;
				tp << matrix[x][y] << "\n\n";
			}			
			if (matrix_expected[x][y] < 0.5 && itp == set_size && innp < set_size)
			{
				innp++;
				nnp << matrix[x][y] << "\n\n";
			}
			if (matrix_expected[x][y] < 0.5 && itn < set_size)
			{
				itn++;
				tn << matrix[x][y] << "\n\n";
			}			
			return true;
		}
		else
		{
			for (int i = -offset; i <= offset; i++)
			{
				for (int j = -offset; j <= offset; j++)
				{
					window[i + offset][j + offset] = matrix[x + offset + i][y + offset + j];
				}
			}


			if (matrix_expected[x][y] > 0.5 && ipnn + itp < set_size * 2)
			{
				for (int i = -near_offset; i <= near_offset; i++)
				{
					for (int j = -near_offset; j <= near_offset; j++)
					{
						if (y + j < 0 || x + i < 0 || x + i >= width || y + j >= height)
							continue;
						if (matrix_expected[x + i][y + j] < 0.5 && ipnn < set_size)
						{
							for (int k = -offset; k <= offset; k++)
							{
								for (int l = -offset; l <= offset; l++)
								{
									pnn << window[k + offset][l + offset] << "\t";
								}
								pnn << "\n";
							}
							pnn << "\n";
							ipnn++;
							return true;
						}
					}
				}
				if (itp < set_size)
				{
					for (int i = -offset; i <= offset; i++)
					{
						for (int j = -offset; j <= offset; j++)
						{
							tp << window[i + offset][j + offset] << "\t";
						}
						tp << "\n";
					}
					tp << "\n";
					itp++;
					return true;
				}
			}
			else if (matrix_expected[x][y] < 0.5 && innp + itn < set_size * 2)
			{

				for (int i = -near_offset; i <= near_offset; i++)
				{
					for (int j = -near_offset; j <= near_offset; j++)
					{
						if (y + j < 0 || x + i < 0 || x + i >= width || y + j >= height)
							continue;
						if (matrix_expected[x + i][y + j] > 0.5 && innp < set_size)
						{
							for (int k = -offset; k <= offset; k++)
							{
								for (int l = -offset; l <= offset; l++)
								{
									nnp << window[k + offset][l + offset] << "\t";
								}
								nnp << "\n";
							}
							nnp << "\n";
							innp++;
							return true;
						}
					}
				}
				if (itn < set_size)
				{
					for (int i = -offset; i <= offset; i++)
					{
						for (int j = -offset; j <= offset; j++)
						{
							tn << window[i + offset][j + offset] << "\t";
						}
						tn << "\n";
					}
					tn << "\n";
					itn++;
					return true;
				}

			}
		}
	}
	return true;
}