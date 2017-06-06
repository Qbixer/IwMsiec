#include <stdio.h>
#include <allegro5/allegro.h>
#include "network.h"
#include <iostream>
#include "input.h"
#include <allegro5/allegro_primitives.h>
#include "generator.h"
#include "bad_thing.h"
#include "network_reworked.h"

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
		cout << "\n\n1 - Generowanie danych z obrazkow\n2 - Utowrzenie sieci\n3 - Testowanie sieci zbudowanej (losowy obraz)\n4 - Testowanie sieci zbudowanej (ze statystykami)\n9 - Wyjscie\n";
		cin >> opcja;
		switch (opcja)
		{
		case 1:
		{
			int offset,set_size;
			string folder;
			cout << "Podaj offset\n";
			cin >> offset;
			cout << "Podaj ilosc probek z jednego zdjecia\n";
			cin >> set_size;
			generator *gen = new generator(offset, set_size);
			cout << "Do jakiego folderu zapisac wynik?\n";
			cin >> folder;
			gen->open_text(folder);
			int typ;
			cout << "1 - healthy\n2 - glaucoma\n3 - diabetic_retinopathy\n4 - test\n";
			cin >> typ;
			//Zmienic
			for (int i = 1;i <= 1;i++)
			{	
				string a,b,c;
				wybor(a, b, c, typ, i);
				char* image = new char[a.size() + 1];
				strcpy(image, a.c_str());
				char* image_good = new char[b.size() + 1];
				strcpy(image_good, b.c_str());
				char* image_mask = new char[c.size() + 1];
				strcpy(image_mask, c.c_str());
				cout << "Przetwarzanie:" << i << "\n";
				gen->open_graphics(image, image_good, image_mask);
				gen->initiate_matrix();
				gen->destroy_graphics();
				gen->generate_data();
				gen->delete_matrices();
				

				//in->do_stuff(image, image_good, image_mask,typ);
			}
			gen->close_file();
			break;
		}
		case 2:
		{
			int offset,layers,ptp,ptn,ppnn,pnnp;
			double u;
			cout << "Podaj offset\n";
			cin >> offset;
			cout << "Podaj wspolczynnik uczenia sie\n";
			cin >> u;
			cout << "Podaj ilosc warstw\n";
			cin >> layers;
			cout << "Podaj relatywne wielkosci zbiorow: tp, tn, pnn, nnp\n";
			cin >> ptp >> ptn >> ppnn >> pnnp;
			layers++;
			string folder;
			cout << "Podaj folder docelowy\n";
			cin >> folder;
			network_reworked *net = new network_reworked(offset, u);
			net->initalize_matrix();
			net->open_text(folder);
			net->load_vectors(ptp, ptn, ppnn, pnnp);
			net->teach_network(layers, folder);
			net->delete_matrix();
			break;
		}
		case 3:
		{
			int offset, typ, layers;
			string obraz;
			cout << "Podaj offset\n";
			cin >> offset;
			cout << "1 - healthy\n2 - glaucoma\n3 - diabetic_retinopathy\n4 - test\n";
			cin >> typ;
			cout << "Podaj ilosc warstw\n";
			cin >> layers;
			cout << "Podaj obraz do przetworzenia\n";
			cin >> obraz;
			char* path = new char[obraz.size() + 1];
			strcpy(path, obraz.c_str());
			cout << "Podaj obraz do zapisu\n";
			cin >> obraz;
			input *in = new input(offset);
			in->do_stuff2(path,obraz, typ, layers,0);
			break;
		}
		case 4:
		{
			int offset, typ, layers,i;
			string obraz;
			cout << "Podaj offset\n";
			cin >> offset;
			cout << "Podaj ilosc warstw\n";
			cin >> layers;
			cout << "1 - healthy\n2 - glaucoma\n3 - diabetic_retinopathy\n4 - test\n";
			cin >> typ;
			cout << "Podaj ktore zdjecie\n";
			cin >> i;			
			string a, b, c;
			wybor(a, b, c, typ, i);
			input *in = new input(offset);
			char* image = new char[a.size() + 1];
			strcpy(image, a.c_str());
			char* image_good = new char[b.size() + 1];
			strcpy(image_good, b.c_str());
			char* image_mask = new char[c.size() + 1];
			strcpy(image_mask, c.c_str());
			in->do_stuff3(image, image_good, image_mask, typ,layers);
			break;
		}
		case 5:
		{

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
