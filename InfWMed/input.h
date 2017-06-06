#pragma once
#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <fstream>
#include <string>
#include "network.h"
using namespace std;

//WAZNE stosujemy reprezentacje od 0 do 1
class input
{
public:
	double ** matrix;
	double ** matrix_expected;
	double ** matrix_mask;
	int offset;

	ALLEGRO_BITMAP *image = NULL;
	ALLEGRO_BITMAP *image_expected = NULL;
	ALLEGRO_BITMAP *image_mask = NULL;
	int innp = 0;
	int ipnn = 0;
	int itn = 0;
	int itp = 0;
	int max = 200;
	int sum = max * 4;
	ofstream nnp;	
	ofstream pnn;	
	ofstream tn;	
	ofstream tp;

	int ttp;
	int ttn;
	int tfp;
	int tfn;
	int mse;
	double ** window;
	int width;
	int height;
	int cur_x = 0;
	int cur_y = 0;
	double expected = 0;

	input(int offset)
	{
		this->offset = offset;
	}
	//Work as intended
	void load_image(char *filename, char *filename_expected, char *filename_mask)
	{
		image = al_load_bitmap(filename);
		width = al_get_bitmap_width(image);
		height = al_get_bitmap_height(image);
		image_expected = al_load_bitmap(filename_expected);
		image_mask = al_load_bitmap(filename_mask);		
	}
	//Work as intended
	void open_files(int typ)
	{
		switch (typ)
		{
		case 1:
		{
			nnp.open("healthy/nnp.txt", fstream::app);
			pnn.open("healthy/pnn.txt", fstream::app);
			tn.open("healthy/tn.txt", fstream::app);
			tp.open("healthy/tp.txt", fstream::app);
			break;
		}
		case 2:
		{
			nnp.open("glaucoma/nnp.txt", fstream::app);
			pnn.open("glaucoma/pnn.txt", fstream::app);
			tn.open("glaucoma/tn.txt", fstream::app);
			tp.open("glaucoma/tp.txt", fstream::app);
			break;
		}
		case 3:
		{
			nnp.open("diabetic_retinopathy/nnp.txt", fstream::app);
			pnn.open("diabetic_retinopathy/pnn.txt", fstream::app);
			tn.open("diabetic_retinopathy/tn.txt", fstream::app);
			tp.open("diabetic_retinopathy/tp.txt", fstream::app);
			break;
		}
		case 4:
			nnp.open("test/nnp.txt", fstream::app);
			pnn.open("test/pnn.txt", fstream::app);
			tn.open("test/tn.txt", fstream::app);
			tp.open("test/tp.txt", fstream::app);
		}
	}
	//Work as intended
	bool take_random_window()
	{
		if (itp + itn + innp + ipnn == sum)
			return false;
		int x = rand() % width;
		int y = rand() % height;
		if (matrix_mask[x][y] == 1)
		{
			for (int i = -offset; i <= offset; i++)
			{
				for (int j = -offset; j <= offset; j++)
				{
					window[i + offset][j + offset] = matrix[x + offset + i][y + offset + j];
				}
			}


			if (matrix_expected[x][y] > 0.5f && ipnn + itp<max * 2)
			{
				for (int i = -1; i <= 1; i++)
				{
					for (int j = -1; j <= 1; j++)
					{
						if (y + j < 0 || x + i < 0 || x + i >= width || y + j >= height)
							continue;
						if (matrix_expected[x + i][y + j] < 0.5f && ipnn<max)
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
				if (itp<max)
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
			else if (matrix_expected[x][y] < 0.5f && innp + itn < max * 2)
			{

				for (int i = -1; i <= 1; i++)
				{
					for (int j = -1; j <= 1; j++)
					{
						if (y + j < 0 || x + i < 0 || x + i >= width || y + j >= height)
							continue;
						if (matrix_expected[x + i][y + j] > 0.5f && innp<max)
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
				if (itn<max)
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
		return true;
	}
	//Work as intended
	void unload_image()
	{
		al_destroy_bitmap(image);
		al_destroy_bitmap(image_expected);
		al_destroy_bitmap(image_mask);
	}
	//Work as intended
	void draw_bitmaps_test()
	{
		ALLEGRO_BITMAP* bit1 = NULL;
		ALLEGRO_BITMAP* bit2 = NULL;
		ALLEGRO_BITMAP* bit3 = NULL;
		bit1 = al_create_bitmap(width, height);
		bit2 = al_create_bitmap(width, height);
		bit3 = al_create_bitmap(width, height);
		al_set_target_bitmap(bit1);
		for (int i = 0;i<width;i++)
		{
			for (int j = 0;j<height;j++)
				al_draw_pixel(i, j, al_map_rgb(matrix[i + offset][j + offset] * 255, matrix[i + offset][j + offset] * 255, matrix[i + offset][j + offset] * 255));
		}
		al_save_bitmap("test/bit1.jpg", bit1);
		al_set_target_bitmap(bit2);
		for (int i = 0;i<width;i++)
		{
			for (int j = 0;j<height;j++)
				al_draw_pixel(i, j, al_map_rgb(matrix_expected[i][j] * 255, matrix_expected[i][j] * 255, matrix_expected[i][j] * 255));
		}
		al_save_bitmap("test/bit2.jpg", bit2);
		al_set_target_bitmap(bit3);
		for (int i = 0;i<width;i++)
		{
			for (int j = 0;j<height;j++)
				al_draw_pixel(i, j, al_map_rgb(matrix_mask[i][j] * 255, matrix_mask[i][j] * 255, matrix_mask[i][j] * 255));
		}
		al_save_bitmap("test/bit3.jpg", bit3);
	}
	//Work as intended
	void initiate_matrix(int typ = 0)
	{
		matrix = new double*[width + 2 * offset];
		al_lock_bitmap(image, al_get_bitmap_format(image), ALLEGRO_LOCK_READONLY);
		for (int i = 0; i < width + 2 * offset; i++)
		{

			matrix[i] = new double[height + 2 * offset];
			for (int j = 0; j < height + 2 * offset;j++)
			{
				//odpowiednio przesuniety na srodek
				if (i - offset >= 0 && i - offset < width && j - offset >= 0 && j - offset < height)
					//matrix[i][j] = (al_get_pixel(image, i - offset, j - offset).b + al_get_pixel(image, i - offset, j - offset).r + al_get_pixel(image, i - offset, j - offset).g) / 3.0f;
					//matrix[i][j] = (al_get_pixel(image, i - offset, j - offset).r + al_get_pixel(image, i - offset, j - offset).g + al_get_pixel(image, i - offset, j - offset).b) / 3.0;
					matrix[i][j] = ((al_get_pixel(image, i - offset, j - offset).r + al_get_pixel(image, i - offset, j - offset).g + al_get_pixel(image, i - offset, j - offset).b) / 3.0)*2-1;
				else
					matrix[i][j] = 0;
			}
		}
		if (typ == 0) {
			matrix_expected = new double*[width];
			al_lock_bitmap(image_expected, al_get_bitmap_format(image_expected), ALLEGRO_LOCK_READONLY);
			for (int i = 0; i < width; i++)
			{
				matrix_expected[i] = new double[height];
				for (int j = 0; j < height; j++)
				{
					//matrix_expected[i][j] = al_get_pixel(image_expected, i, j).b;
					matrix_expected[i][j] = al_get_pixel(image_expected, i, j).b*2-1;
				}
			}
			matrix_mask = new double*[width];
			al_lock_bitmap(image_mask, al_get_bitmap_format(image_mask), ALLEGRO_LOCK_READONLY);
			for (int i = 0; i < width; i++)
			{
				matrix_mask[i] = new double[height];
				for (int j = 0; j < height; j++)
				{
					matrix_mask[i][j] = al_get_pixel(image_mask, i, j).b;
				}
			}
		}
			window = new double*[offset * 2 + 1];
			for (int i = 0; i < offset * 2 + 1; i++)
			{
				window[i] = new double[offset * 2 + 1];
			}
		
	}

	void do_stuff3(char *filename, char *filename_expected, char *filename_mask, int typ, int layers)
	{
		load_image(filename, filename_expected, filename_mask);
		initiate_matrix();
		ifstream test;
		ofstream test2;
		for (int i = 0;i < network::max_set_count;i++)
		{
			string b = "";
			string a = "";
			string c = "";
			switch (typ)
			{
			case 1:
			{
				a += "healthy/";
				a += to_string(offset);
				a += "_";
				c = a;
				c += "stats.txt";
				a += to_string(i);
				b = a;
				a += "_h.txt";
				b += "_h.png";
				break;
			}
			case 2:
			{
				a += "glaucoma/";
				a += to_string(offset);
				a += "_";
				c = a;
				c += "stats.txt";
				a += to_string(i);
				b = a;
				a += "_g.txt";
				b += "_g.png";
				break;
			}
			case 3:
			{
				a += "diabetic_retinopathy/";
				a += to_string(offset);
				a += "_";
				c = a;
				c += "stats.txt";
				a += to_string(i);
				b = a;
				a += "_dr.txt";
				b += "_dr.png";
				break;
			}
			case 4:
			{
				a += "test/";
				a += to_string(offset);
				a += "_";
				c = a;
				c += "stats.txt";
				a += to_string(i);
				b = a;
				a += ".txt";
				b += "_t.png";
				break;
			}
			}
			char* path = new char[a.size() + 1];
			strcpy(path, a.c_str());
			test.open(path, fstream::in);

			network *net = new network(offset, layers);
			for (int i = 0;i < net->siec.size();i++)
			{
				for (int j = 0;j < net->siec[i].size();j++)
				{
					if (i == 0)
					{
						test >> net->siec[i][j]->weight[0];
						test >> net->siec[i][j]->base_weight;
					}
					else
					{
						for (int k = 0;k < net->siec[i-1].size();k++)
							test >> net->siec[i][j]->weight[k];
						test >> net->siec[i][j]->base_weight;
					}
				}
			}
			test.close();
			ALLEGRO_BITMAP* display = NULL;
			display = al_create_bitmap(width, height);
			al_set_target_bitmap(display);
			al_clear_to_color(al_map_rgb(0, 0, 0));
			ttp = 0;
			ttn = 0;
			tfp = 0;
			tfn = 0;
			mse = 0;
			while (take_new_window(net, display,1));
			b = "obrazy/" + b;
			char* path2 = new char[b.size() + 1];
			strcpy(path2, b.c_str());
			al_save_bitmap(path2, display);
			c = "obrazy/" + c;
			char* path3 = new char[c.size() + 1];
			strcpy(path3, c.c_str());
			test2.open(path3, fstream::app);
			test2 << i << "\n";
			test2 << "True positive: " << ttp << "\t" << "False positive: " << tfp << "\n";
			test2 << "False negative: " << tfn << "\t" << "True negative: " << ttn << "\n";
			test2 << "MSE: " << sqrt(mse) << "\n";
			test2.close();
		}
	}

	void do_stuff2(char *filename, string result, int typ, int layers, int k_set)
	{
		image = al_load_bitmap(filename);
		width = al_get_bitmap_width(image);
		height = al_get_bitmap_height(image);
		initiate_matrix(1);
		ifstream test;
		
		for (int i = 0;i < network::max_set_count;i++)
		{
			string a;
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
			test.open(path, fstream::in);

			network *net = new network(offset, layers);
			for (int i = 0;i < net->siec.size();i++)
			{
				for (int j = 0;j < net->siec[i].size();j++)
				{
					if (i == 0)
					{
						test >> net->siec[i][j]->weight[0];
						test >> net->siec[i][j]->base_weight;
					}
					else
					{
						for (int k = 0;k < net->siec[i].size();k++)
							test >> net->siec[i][j]->weight[k];
						test >> net->siec[i][j]->base_weight;
					}
				}
			}
			test.close();
			ALLEGRO_BITMAP* display = NULL;
			display = al_create_bitmap(width, height);
			al_set_target_bitmap(display);
			al_clear_to_color(al_map_rgb(0, 0, 0));
			while (take_new_window(net, display));

			string b = "obrazy/" + to_string(i) + "_" + result;
			char* path2 = new char[b.size() + 1];
			strcpy(path2, b.c_str());
			al_save_bitmap(path2, display);
		}
	}	
	
	void do_stuff(char *filename, char *filename_expected, char *filename_mask, int typ)
	{
		itp = 0;
		itn = 0;
		innp = 0;
		ipnn = 0;
		load_image(filename, filename_expected, filename_mask);
		initiate_matrix();
		open_files(typ);
		//draw_bitmaps_test();
		while (take_random_window());
		nnp.close();
		pnn.close();
		tn.close();
		tp.close();
		unload_image();
	}


	


	bool take_new_window(network * net, ALLEGRO_BITMAP* display, int q = 0)
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
		net->insert_data(offset, window);
		net->calculate();
		if (net->siec[net->siec.size() - 1][0]->output < 0)
			al_draw_pixel(cur_x, cur_y, al_map_rgb(0, 0, 0));
		else
			al_draw_pixel(cur_x, cur_y, al_map_rgb(255, 255, 255));
		if(q != 0)
		{
			if (matrix_expected[cur_x][cur_y] >= 0)
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
				mse += (-1 - matrix_expected[cur_x][cur_y])*(-1 - matrix_expected[cur_x][cur_y]);
		}

		cur_x++;
		return true;

	}

	
};