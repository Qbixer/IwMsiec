#pragma once
#include <string>
void wybor(std::string &a, std::string &b, std::string &c, int typ, int i)
{
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
		a += std::to_string(i);
		b += std::to_string(i);
		c += std::to_string(i);
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
		a += std::to_string(i);
		b += std::to_string(i);
		c += std::to_string(i);
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
		a += std::to_string(i);
		b += std::to_string(i);
		c += std::to_string(i);
		a += "_dr.jpg";
		b += "_dr.jpg";
		c += "_dr_mask.jpg";
		break;
	}
	case 4:
	{
		a += "test/01_o.png";
		b += "test/01_e.png";
		c += "test/01_m.png";
		break;
	}
	}
}
