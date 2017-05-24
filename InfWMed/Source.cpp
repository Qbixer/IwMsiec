#include <stdio.h>
#include <allegro5/allegro.h>
#include "network.h"
#include <iostream>
#include "input.h"

using namespace std;
int main()
{
	al_init();
	al_init_image_addon();
	al_init_primitives_addon();
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
			input *in = new input(offset);
			int typ;
			cout << "1 - healthy\n2 - glaucoma\n3 - diabetic_retinopathy\n4 - test\n";
			cin >> typ;
			//Zmienic
			for (int i = 1;i <= 15;i++)
			{	
				string a,b,c;
				switch (typ)
				{
				case 1:
				{
					a += "healthy/";
					b += "healthy/a/";
					c += "healthy/a/";
					if (i < 10)
					{
						a += "0";
						b += "0";
						c += "0";
					}
					a += to_string(i);
					b += to_string(i);
					c += to_string(i);
					a += "_h.jpg";
					b += "_h.jpg";
					c += "_h_mask.jpg";
					break;
				}
				case 2:
				{
					a += "glaucoma/";
					b += "glaucoma/a/";
					c += "glaucoma/a/";
					if (i < 10)
					{
						a += "0";
						b += "0";
						c += "0";
					}
					a += to_string(i);
					b += to_string(i);
					c += to_string(i);
					a += "_g.jpg";
					b += "_g.jpg";
					c += "_g_mask.jpg";
					break;
				}
				case 3:
				{
					a += "diabetic_retinopathy/";
					b += "diabetic_retinopathy/a/";
					c += "diabetic_retinopathy/a/";
					if (i < 10)
					{
						a += "0";
						b += "0";
						c += "0";
					}
					a += to_string(i);
					b += to_string(i);
					c += to_string(i);
					a += "_dr.jpg";
					b += "_dr.jpg";
					c += "_dr_mask.jpg";
					break;
				}
				case 4:
					{
					a += "test/01_o.jpg";
					b += "test/01_e.jpg";
					c += "test/01_m.jpg";
					break;
					}
				}		
				char* image = new char[a.size() + 1];
				strcpy(image, a.c_str());
				char* image_good = new char[b.size() + 1];
				strcpy(image_good, b.c_str());
				char* image_mask = new char[c.size() + 1];
				strcpy(image_mask, c.c_str());
				cout << "Przetwarzanie:" << i << "\n";
				in->do_stuff(image, image_good, image_mask,typ);
			}
			break;
		}
		case 2:
		{
			int offset,layers,ptp,ptn,ppnn,pnnp,typ;
			float u;
			cout << "Podaj offset\n";
			cin >> offset;
			cout << "Podaj wspolczynnik uczenia sie\n";
			cin >> u;
			cout << "Podaj ilosc warstw\n";
			cin >> layers;
			cout << "Podaj relatywne wielkosci zbiorow: tp, tn, pnn, nnp\n";
			cin >> ptp >> ptn >> ppnn >> pnnp;
			cout << "1 - healthy\n2 - glaucoma\n3 - diabetic_retinopathy\n4 - test\n";
			cin >> typ;
			network(offset, u,layers,typ,ptp,ptn,ppnn,pnnp);
			break;
		}
		case 3:
			{
			input *in = new input(5);
			in->do_stuff2("healthy/01_h.jpg", "healthy/a/01_h.jpg", "healthy/a/01_h_mask.jpg", 2, 3, 0);
			break;
			}
		case 9:
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
	/*network * siec = new network(5, 0.2, "test.bmp", "test.bmp", "test.bmp",4);
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
	*/

}
