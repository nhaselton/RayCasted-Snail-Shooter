#include "ImageImporter.h"
Texture* ReadBMP(const char* path)
{
	Texture* texture = malloc(sizeof(Texture));

	FILE* file = fopen(path,"r");
	int test[14];

	//Get Header
	unsigned char header[54];
	fread(header, 1,54, file);
	
	//W&H of Image
	texture->width =  *(int*)&header[18];
	texture->height = *(int*)&header[22];
	int size = 3 * texture->width * texture->height;
	//Read rest of data	
	unsigned char* fileData = (unsigned char*)malloc(size);
	fread(fileData, 1, size, file);

	//Flip from BGR To RGB (i hate you microsoft)
	//isnt actually needed, i could just change how things are shifted but im lazy
	for (int i = 0; i < size; i += 3)
	{
		unsigned char temp = fileData[i];
		fileData[i] = fileData[i + 2];
		fileData[i + 2] = temp;
	}

	//CREATING THE TEXTURE
	//Will have 8 padding
	texture->data = malloc(size/3 * sizeof(int));
	
	int t = 0;
	for (int i = 0; i < size; i)
	{
		int r = fileData[i]; i++;
		int g = fileData[i]; i++;
		int b = fileData[i]; i++;
		int test = b | g << 8 | r << 16;
		texture->data[t] = test;
		t++;
	}

	free(fileData);
	fclose(file);
	return texture;
}