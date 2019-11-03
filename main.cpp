#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <time.h>
#include <iostream>
#include <fstream>
using namespace std;

#define NUMBER_OF_FLOATS_IN_DISK_BLOCK 10
#define SIZE_OF_DISK_BLOCK sizeof(float)*NUMBER_OF_FLOATS_IN_DISK_BLOCK


float* generate_data(int no_records, int range) {
	float* records = (float*) malloc(sizeof(float) * no_records);
	if (records == NULL)
		return NULL;
	srand((unsigned int)time(NULL));
	for (int i = 0; i < no_records; i++) {
		if (rand() % 2)
			records[i] = (float)(rand() % range); //generating an integer
		else
			records[i] = (float)(rand()) / (RAND_MAX/range); // generating a floating point number
	}
	return records;
}

void print_records(float* records, int no_records) {
	printf("RECORDS:\n");
	for (int i = 0; i < no_records; i++)
		printf("%f\n", records[i]);
	printf("\n");
}

void store_data(float records[], int no_records) {
	ofstream myfile;
	myfile.open("data.txt");
	myfile.write((char*)&records[0], sizeof(records[0]) * no_records);
	myfile.close();
}

/*float* load_data(int no_bytes) {
	float* records = (float*) malloc(no_bytes);
	FILE* myfile;
	fopen_s(&myfile, "data.txt", "rb");
	fread(records, 1, no_bytes, myfile);
	fclose(myfile);
	return records;
}*/

void load_buffer(float** buffer, const char* filename, int data_position) {
	ifstream myfile;
	myfile.open(filename);
	myfile.seekg(data_position);
	myfile.read(((char*)*buffer), SIZE_OF_DISK_BLOCK);
	myfile.close();
}



int main(int argc, char* argv[]) {
	bool generate_flag = false; //generate data flag
	bool show_flag = false; //show records after each phase flag
	unsigned int no_records;
	unsigned int range;
	float* records;
	float* buffer = (float*) malloc(SIZE_OF_DISK_BLOCK);
	for (int i = 1; i <= argc; i++) {
		if (strcmp(argv[i], "-g") == 0)
		{
			 //GENERATE RANDOMLY
			generate_flag = true;
			i++;
			if (i < argc) {
				try {
					no_records = stoi(argv[i++]);
					range = stoi(argv[i++]);
				}
				catch(const char* msg) {
					fprintf(stderr, "Invalid input for generation, ERROR: %s\n", msg);
					generate_flag = false;
				}
			}
				
		}
		else if (strcmp(argv[i], "-w") == 0)
		{
			//SHOW RECORDS AFTER EACH PHASE
			show_flag = true;
		}
		else
		{
			fprintf(stderr, "Incorrect argument(s)\n");
		}
	}
	if (generate_flag) {
		records = generate_data(no_records, range);
		print_records(records, no_records);
		store_data(records, no_records);
		free(records);
		load_buffer(&buffer, "data.txt", 0);
		print_records(buffer, NUMBER_OF_FLOATS_IN_DISK_BLOCK);
		load_buffer(&buffer, "data.txt", SIZE_OF_DISK_BLOCK);
		print_records(buffer, NUMBER_OF_FLOATS_IN_DISK_BLOCK);
	}

	return 0;
}