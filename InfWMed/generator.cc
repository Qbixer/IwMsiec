#include "generator.h"
#include <allegro5/bitmap_io.h>
#include <iostream>
#include <allegro5/bitmap_lock.h>

generator::generator(int offset, int set_size)
{
	this->offset = offset;
	this->set_size = set_size;
	sum = 4 * set_size;
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
		return al_get_pixel(image_expected, i, j).b * 2 - 1;
	}
	if(typ == mask_type)
	{
		return al_get_pixel(image_mask, i, j).b;
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
	if (matrix_mask[x][y] == 1)
	{
		for (int i = -offset; i <= offset; i++)
		{
			for (int j = -offset; j <= offset; j++)
			{
				window[i + offset][j + offset] = matrix[x + offset + i][y + offset + j];
			}
		}


		if (matrix_expected[x][y] > 0.5f && ipnn + itp<set_size * 2)
		{
			for (int i = -1; i <= 1; i++)
			{
				for (int j = -1; j <= 1; j++)
				{
					if (y + j < 0 || x + i < 0 || x + i >= width || y + j >= height)
						continue;
					if (matrix_expected[x + i][y + j] < 0.5f && ipnn<set_size)
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
			if (itp<set_size)
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
		else if (matrix_expected[x][y] < 0.5f && innp + itn < set_size * 2)
		{

			for (int i = -1; i <= 1; i++)
			{
				for (int j = -1; j <= 1; j++)
				{
					if (y + j < 0 || x + i < 0 || x + i >= width || y + j >= height)
						continue;
					if (matrix_expected[x + i][y + j] > 0.5f && innp<set_size)
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
			if (itn<set_size)
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