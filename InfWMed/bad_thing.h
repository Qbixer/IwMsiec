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
		a += "_g.png";
		b += "_g.png";
		c += "_g_mask.png";
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
		a += "_dr.png";
		b += "_dr.png";
		c += "_dr_mask.png";
		break;
	}
	case 4:
	{
		a += "test/";
		b += "test/";
		c += "test/";
		if (i < 10)
		{
			a += "0";
			b += "0";
			c += "0";
		}
		a += std::to_string(i);
		b += std::to_string(i);
		c += std::to_string(i);
		a += "_o.png";
		b += "_e.png";
		c += "_m.png";
		break;
	}
	}
}
