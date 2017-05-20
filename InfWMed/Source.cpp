#include <stdio.h>
#include <allegro5/allegro.h>
#include "allegro5/allegro_image.h"
#include "network.h"
#include <iostream>

using namespace std;
int main()
{
	int opcja;
	bool loop = true;
	while (loop)
	{
		cout << "\n\n1 - Generowanie danych z obrazkow\n2 - Utowrzenie sieci\n3 - Testowanie sieci zbudowanej\n9 - Wyjscie\n";
		cin >> opcja;
		switch (opcja)
		{
		case 1:
		{
			int offset;
			cout << "Podaj offset\n";
			cin >> offset;
			input(offset, "test.bmp", "test.bmp", "test.bmp");
			break;
		}
		case 4:
		{
			loop = false;
			break;
		}
		default:
		{
			cout << "Wybierz opcje";
			break;
		}
		}
	}
	network * siec = new network(5, 0.2, "test.bmp", "test.bmp", "test.bmp",4);
	siec->one_iteration_matrix(false);
	ALLEGRO_DISPLAY* display = NULL;
	display = al_create_display(siec->in->width,siec->in->height);
	al_set_target_bitmap(al_get_backbuffer(display));
	al_clear_to_color(al_map_rgb(0, 0, 0));
	siec->one_iteration_matrix(true);
	for (int i = 0; i < siec->in->width;i++)
	{
		for (int j = 0; j < siec->in->height; j++)
		{
			al_draw_pixel(j + 1, i + 1, al_map_rgb(siec->matrix_expected[j][i], siec->matrix_expected[j][i], siec->matrix_expected[j][i]));
		}
	}
	al_flip_display();
	system("pause");


}
