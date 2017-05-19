#pragma once
#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
//WAZNE stosujemy reprezentacje od 0 do 1
class input
{
public:
	float ** matrix;
	float ** matrix_expected;
	ALLEGRO_BITMAP *image = NULL;
	ALLEGRO_BITMAP *image_expected = NULL;
	int offset = 8;
	float ** window;
	int width;
	int height;
	int cur_x = 0;
	int cur_y = 0;
	float expected = 0;

	input(int offset,char *filename, char *filename_expected)
	{
		this->offset = offset;
		load_image(filename, filename_expected);
	}

	void load_image(char *filename, char *filename_expected)
	{
		al_init();
		al_init_image_addon();
		al_init_primitives_addon();
		image = al_load_bitmap(filename);
		width = al_get_bitmap_width(image);
		height = al_get_bitmap_height(image);
		al_lock_bitmap(image, al_get_bitmap_format(image), ALLEGRO_LOCK_READONLY);
		image_expected = al_load_bitmap(filename_expected);
		al_lock_bitmap(image_expected, al_get_bitmap_format(image_expected), ALLEGRO_LOCK_READONLY);
		initiate_matrix();
	}
	
	void initiate_matrix()
	{
		matrix = new float*[width + 2 * offset];
		al_lock_bitmap(image, al_get_bitmap_format(image), ALLEGRO_LOCK_READONLY);
		for (int i = 0; i < width + 2 * offset; i++)
		{
			matrix[i] = new float[height + 2 * offset];
			for(int j = 0; j < height + 2 * offset;j++)
			{
				//odpowiednio przesuniety na srodek
				if (i - offset >= 0 && i - offset < width && j - offset >= 0 && j - offset < height)
					//tutaj chyba dzielic na 255 trzeba, chyba ze zwraca w 0-1
					matrix[i][j] = al_get_pixel(image, i - offset + 1, j - offset + 1).b/255;
				else
					matrix[i][j] = 0.F;
			}
		}
		matrix_expected = new float*[width];
		al_lock_bitmap(image_expected, al_get_bitmap_format(image_expected), ALLEGRO_LOCK_READONLY);
		for (int i = 0; i < width; i++)
		{
			matrix_expected[i] = new float[height];
			for (int j = 0; j < height; j++)
			{
				matrix[i][j] = al_get_pixel(image, i + 1, j + 1).b / 255;
			}
		}
		window = new float*[offset * 2 + 1];
		for (int i = 0; i < offset * 2 + 1; i++)
		{
			window[i] = new float[offset * 2 + 1];
		}
	}

	bool take_new_window()
	{
		//gdy x jest juz za duzy
		if (cur_x >= width)
		{
			cur_x = 0;
			cur_y++;
		}
		//gdy y jest za duzy - blad
		if(cur_y >= height)
		{
			cur_x = 0;
			cur_y = 0;
			return false;
		}
		//okno jest od -offset - 0 (srodek badany) - offset
		for (int i = -offset; i <= offset; i++)
		{
			for (int j = -offset; j <= offset; j++)
			{
				window[i + offset][j + offset] = matrix[cur_x + offset + i][cur_y + offset + j];
			}		
		}
		expected = window[offset][offset];
		cur_x++;
		return true;
	}


	
};