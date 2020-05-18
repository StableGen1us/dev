#include <fstream>
#include <iostream>
using namespace std;

const int width = 1920, height = 1200;
static int r=0,g=0,b=0;
char sep;

int main(){
	ofstream img ("./pic.ppm");
	img << "P3" << endl;
	img << width << " " << height << endl;
	img << "255" << endl;

// read the bitmap file
	ifstream imgIn("./bmfile.txt");
	for(int y=0; y<height; y++){
		for(int x=0; x<width;  x++){
			imgIn>>r;
			imgIn>>sep;
			imgIn>>g;
			imgIn>>sep;
			imgIn>>b;
			imgIn>>sep;
			img<<r<<" "<<g<<" "<<b<<endl;
		}

	}
	return 0;



}
