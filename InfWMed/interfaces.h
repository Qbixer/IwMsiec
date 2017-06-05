#pragma once
#ifndef INTERFACES_H_
#define INTERFACES_H_
#include <allegro5/bitmap.h>
#include <fstream>

class file_manager_interface
{
public:
	virtual ~file_manager_interface() = default;
	virtual bool open_graphics(char *filename) = 0;
	virtual bool open_text(char *filename) = 0;
	virtual void destroy_graphics(ALLEGRO_BITMAP *bitmap) = 0;
	virtual void close_file(std::fstream *text) = 0;
};


#endif