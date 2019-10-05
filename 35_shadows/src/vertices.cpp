#include <iostream>
#include <vector>

std::vector<float> populate_vertices(unsigned int height, unsigned int width)
{
	std::vector<float> v;
	for(unsigned int h=0; h<height; h++) 
		for(unsigned int w=0; w<width; w++) {
			v.push_back(w); v.push_back(h); v.push_back(0.0f); v.push_back(0.0f); v.push_back(0.0f); v.push_back(1.0f); v.push_back(w); v.push_back(h);
			v.push_back(w+1); v.push_back(h); v.push_back(0.0f); v.push_back(0.0f); v.push_back(0.0f); v.push_back(1.0f); v.push_back(w+1); v.push_back(h);
			v.push_back(w); v.push_back(h+1); v.push_back(0.0f); v.push_back(0.0f); v.push_back(0.0f); v.push_back(1.0f); v.push_back(w); v.push_back(h+1);

			v.push_back(w+1); v.push_back(h); v.push_back(0.0f); v.push_back(0.0f); v.push_back(0.0f); v.push_back(1.0f); v.push_back(w+1); v.push_back(h);
			v.push_back(w+1); v.push_back(h+1); v.push_back(0.0f); v.push_back(0.0f); v.push_back(0.0f); v.push_back(1.0f); v.push_back(w+1); v.push_back(h);
			v.push_back(w); v.push_back(h+1); v.push_back(0.0f); v.push_back(0.0f); v.push_back(0.0f); v.push_back(1.0f); v.push_back(w); v.push_back(h+1);
		}
	return v;
}

int main()
{
	unsigned int height = 2;
	unsigned int width = 2;
	
	std::vector<float> v = populate_vertices(height, width);
	float *vertices = &v[0];
	int no_of_points = 6 * height * width * 8;

	for(unsigned int i=0; i<no_of_points; i++)
	{
		if(i%8==0)
			std::cout << std::endl;
		std::cout << vertices[i] << ", ";
	}


	return 0;
}
