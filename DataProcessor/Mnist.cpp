/*
 * Mnist.cpp
 *
 *  Created on: 2011-9-26
 *      Author: Junhao Gan
 */

#include "headers.h"

// Variance node
struct vNode {
	double var;
	int id;
}*node1;

// Compare funtion
bool cmp1(vNode a, vNode b) {
	return a.var > b.var;
}
// An array to store the top 50 dimension index with largest variance.
int* top50d;

// Read the Mnist binary file
void readMnistBinaryFile(char *filename, int n, int d, PPointT* dataset) {
	// Read the binary dataset.
	ifstream fin(filename, ios::binary);
	if (!fin) {
		printf("File open error!\n");
		return;
	}

	// Jump over the first 16 bytes.
	int header[4]; /* magic num, nPoints, width, height */
	fin.read((char*) header, sizeof(header));

	int id = 0;
	PPointT p;
	int temp = 0;

	while (!fin.eof()) {
		FAILIF(NULL == (p = (PPointT) malloc(sizeof(PointT))));
		FAILIF(NULL == (p->coordinates = (double*) malloc(d * sizeof(double))));
		for (int i = 0; i < d; i++) {
			fin.read((char*) (&temp), sizeof(char));
			p->coordinates[i] = temp;

			// Record the maximum coordinate value.
			if (p->coordinates[i] > maxCoordinate)
				maxCoordinate = p->coordinates[i];
			// Record the minimum coordinate value.
			if (p->coordinates[i] < minCoordinate)
				minCoordinate = p->coordinates[i];
		}
		p->index = id + 1;
		dataset[id] = p;
		id++;
	}
	fin.close();
}

//Read the Original Mnist Dataset and Queryset
void readMnistBinaryDataSet(char *trainfilename, char *queryfilename, int n, int n2, int d) {
	// Initialize the global variables
	nPoints = n;
	pointsDimension = d;

	FAILIF(NULL==(dataSetPoints = (PPointT*) malloc(n * sizeof(PPointT))));
	FAILIF(NULL==(querySetPoints = (PPointT*) malloc(n2 * sizeof(PPointT))));

	readMnistBinaryFile(trainfilename, n, d, dataSetPoints);
	readMnistBinaryFile(queryfilename, n2, d, querySetPoints);

	printf("%lf %lf %lf\n", maxCoordinate, minCoordinate, maxCoordinate - minCoordinate);
}

//Write the Mnist Dataset
void writeMnistDataSet(char *filename, int targetDimension, double scale) {

	FAILIF(NULL == (top50d = (int*) malloc(targetDimension * sizeof(int))));

	FILE* file;
	char buffer[1000];
	sprintf(buffer, "%s.ds", filename);
	file = fopen(buffer, "w");

	double avg;

	FAILIF(NULL == (node1 = (vNode*) malloc(pointsDimension * sizeof(vNode))));

	for (int i = 0; i < pointsDimension; i++) {
		node1[i].var = avg = 0.0;
		node1[i].id = i;
		for (int j = 0; j < nPoints; j++) {
			node1[i].var += dataSetPoints[j]->coordinates[i] * dataSetPoints[j]->coordinates[i];
			avg += dataSetPoints[j]->coordinates[i];
		}
		node1[i].var -= avg * avg / nPoints;
		node1[i].var /= nPoints;
	}

	// Sort <nodel> by decreasing variance.
	partial_sort(node1, node1 + targetDimension, node1 + pointsDimension, cmp1);

	//Record the dimension index with the top 50 largest <targetDimension> variance
	for (int i = 0; i < targetDimension; i++) {
		top50d[i] = node1[i].id;
	}

	// Sort the index array with increasing id.
	sort(top50d, top50d + targetDimension);

	int id = 1;

	if (maxCoordinate != minCoordinate)
		scale = scale / (maxCoordinate - minCoordinate);

	for (int i = 0; i < nPoints; i++) {
		if (dataSetPoints[i]->index != -1) {
			fprintf(file, "%d", id++);
			for (int j = 0; j < targetDimension; j++) {
				fprintf(file, " %d", (int) ((dataSetPoints[i]->coordinates[top50d[j]] - minCoordinate) * scale));
			}
			fprintf(file, "\n");
			fflush(file);
		}
	}
	fclose(file);
	delete (node1);

	//update the global pointsDimension
	pointsDimension = targetDimension;
	free(dataSetPoints);
}

// Write random 50 queries to file
void writeMnistQuerySet(char* filename, int targetDimension, int nQueries, double scale) {

	FILE* file;
	char buffer[1000];
	sprintf(buffer, "%s.q", filename);
	file = fopen(buffer, "w");

	int id = 1;

	int count = 0;
	int index[50];
	int random;
	int i;
	srand(time(0));

	// Generate 50 random integers.
	while (count < 50) {
		random = genRandomInt(0, nQueries - 1);
		for (i = 0; i < count; i++) {
			if (index[i] == random) {
				break;
			}
		}
		if (i == count) {
			index[count] = random;
			count++;
		}
	}
	sort(index, index + 50);

	count = 0;

	if (maxCoordinate != minCoordinate)
		scale = scale / (maxCoordinate - minCoordinate);

	for (int i = 0; i < nQueries; i++) {
		if (querySetPoints[i]->index == index[count]) {
			fprintf(file, "%d", id++);
			for (int j = 0; j < targetDimension; j++) {
				fprintf(file, " %d", (int) ((querySetPoints[i]->coordinates[top50d[j]] - minCoordinate) * scale));
			}
			fprintf(file, "\n");
			fflush(file);
			count++;
		}
		if (count == 50)
			break;
	}

	fclose(file);
	free(querySetPoints);
}

void writeMnistDatasetAndQueryset(char* filename, int targetDimension, double scale) {
	writeMnistDataSet(filename, targetDimension, scale);
	writeMnistQuerySet(filename, targetDimension, 10000, scale);
}
