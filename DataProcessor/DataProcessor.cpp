/*
 * DataProcessor.cpp
 *
 *  Created on: 2011-9-26
 *      Author: Junhao Gan
 */

#include "headers.h"

char sBuffer[100];

//Generate a random integer in [rangeStart, rangeEnd]
int genRandomInt(int rangeStart, int rangeEnd) {
	ASSERT(rangeStart <= rangeEnd);
	int r;
	r = rangeStart + (int) ((rangeEnd - rangeStart + 1.0) * random() / (RAND_MAX + 1.0));
	ASSERT(r >= rangeStart && r <= rangeEnd);
	return r;
}

/**
 * Returns the Euclidean distance from point <p1> to <p2>.
 */
double distance(int dimension, PPointT p1, PPointT p2) {
	double result = 0;

	for (int i = 0; i < dimension; i++) {
		result += (p1->coordinates[i] - p2->coordinates[i]) * (p1->coordinates[i] - p2->coordinates[i]);
	}

	return sqrt(result);
}

/**
 * Read a Point from file.
 */
PPointT readPoint(FILE *fileHandle) {
	PPointT p;
	double sqrLength = 0;
	FAILIF(NULL == (p = (PPointT) malloc(sizeof(PointT))));
	FAILIF(NULL == (p->coordinates = (double*) malloc(pointsDimension * sizeof(double))));
	fscanf(fileHandle, "%d", &(p->index));

	for (int d = 0; d < pointsDimension; d++) {
		fscanf(fileHandle, "%lf", &(p->coordinates[d]));
		sqrLength += (p->coordinates[d]) * (p->coordinates[d]);

		// Record the maximum coordinate value.
		if (p->coordinates[d] > maxCoordinate)
			maxCoordinate = p->coordinates[d];
		// Record the minimum coordinate value.
		if (p->coordinates[d] < minCoordinate)
			minCoordinate = p->coordinates[d];
	}

	fscanf(fileHandle, "%[^\n]", sBuffer);

	if (sqrLength > maxSqrLength)
		maxSqrLength = sqrLength;
	else if (sqrLength < minSqrLength && sqrLength != 0)
		minSqrLength = sqrLength;
	return p;
}

/*
 * Reads in the data set points from <filename> in the array
 * <dataSetPoints>. Each point get a unique number in the field
 * <index> to be easily indentified.
 */
void readDataSetFromFile(char *filename, int n, int d) {
	// Initialize the global variables
	nPoints = n;
	pointsDimension = d;

	FILE *f = fopen(filename, "rt");
	FAILIF(f == NULL);
	FAILIF(NULL == (dataSetPoints = (PPointT*) malloc(nPoints * sizeof(PPointT))));

	for (int i = 0; i < nPoints; i++) {
		dataSetPoints[i] = readPoint(f);
	}

//	printf("%lf %lf %lf\n", maxCoordinate, minCoordinate, maxCoordinate - minCoordinate);
}

// Compare two points if they are the same.
bool same(PPointT a, PPointT b) {
	for (int i = 0; i < pointsDimension; i++) {
		if (a->coordinates[i] != b->coordinates[i])
			return false;
	}
	return true;
}

/**
 * Output the real top 100NN distances of each query point in query set to a file as a ground truth.
 */
void writeGroundTruth(char* queryFilePath, char* groundTruthFilePath, int nQueries) {
	PPointT* queryPointList;

	FAILIF(NULL == (queryPointList = (PPointT*) malloc(nQueries * sizeof(PPointT))));

	FILE *queryFile = fopen(queryFilePath, "rt");
	FAILIF(queryFile == NULL);

	FILE *groundTruthFile = fopen(groundTruthFilePath, "w");
	FAILIF(groundTruthFile == NULL);

	int maxKNN = 100;
	fprintf(groundTruthFile, "%d %d\n", nQueries, maxKNN);

	// Read all query points once.
	for (int i = 0; i < nQueries; i++) {

		queryPointList[i] = readPoint(queryFile);

		vector<double> groundTruthes;
		double dis;

		// Calculate the real distances.
		for (int j = 0; j < nPoints; j++) {
			dis = distance(pointsDimension, dataSetPoints[j], queryPointList[i]);
			groundTruthes.push_back(dis);
		}

		// Sort these real distances.
		partial_sort(groundTruthes.begin(), groundTruthes.begin() + 100, groundTruthes.end());

		fprintf(groundTruthFile, "%d", queryPointList[i]->index);
		for (int k = 0; k < maxKNN; k++) {
			fprintf(groundTruthFile, " %lf", groundTruthes[k]);
		}
		fprintf(groundTruthFile, "\n");
		//		printf("%d\n", i + 1);
	}
	fclose(queryFile);
	fclose(groundTruthFile);
}

/**
 * Process a dataset by randomly removing <num> points and using them to form a query set.
 * Please note that the dataset should have been read by Funtion
 * void readDataSetFromFile(char *filename, int n, int d).
 */
void processDataSet(char *filename, int num, double scale) {
	FILE* fout;
	FILE* fileTemp;
	FILE* fout_ds;
	char buffer[1000];
	int random, count, i, j, k;
	int* index;

	int pcount = 0;
	int qcount = 0;
	int id;

	sprintf(buffer, "%s.q", filename);
	fout = fopen(buffer, "w");

	sprintf(buffer, "%s.ds", filename);
	fout_ds = fopen(buffer, "w");

	// An array to store the random number.
	FAILIF(NULL == (index = (int*) malloc(num * sizeof(int))));

	count = 0;
	srand(time(0));

	// Generate <num> random integers within [0, <nPoints> - 1]
	while (count < num) {
		random = genRandomInt(0, nPoints - 1);
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

	if (maxCoordinate != minCoordinate)
		scale = scale / (maxCoordinate - minCoordinate);

	for (i = 0; i < nPoints; i++) {

		bool isQuery = false;
		for (j = 0; j < num; j++) {
			if (index[j] == i) {
				isQuery = true;
				break;
			}
		}

		if (isQuery) {
			fileTemp = fout;
			id = ++qcount;
		} else {
			fileTemp = fout_ds;
			id = ++pcount;
		}

		fprintf(fileTemp, "%d", id);
		for (k = 0; k < pointsDimension; k++) {
			//fprintf(fileTemp, " %d", (int) ((dataSetPoints[i]->coordinates[k])));
			fprintf(fileTemp, " %d", (int) ((dataSetPoints[i]->coordinates[k] - minCoordinate) * scale));
		}
		fprintf(fileTemp, "\n");
		fflush(fileTemp);
	}
	fileTemp = NULL;
	fclose(fout);
	fclose(fout_ds);
	free(dataSetPoints);
}

/*
 * Generate random integer dataset in the range of [0, <scale>].
 * <filename> the output file path
 * <n>  the dataset size
 * <d>  the dimensionality of data
 * <qn> the query set size
 */
void generateRandInt(char* filename, int n, int d, int qn, int scale) {
	FILE* fout;
	char buffer[1000];
	int random;

	sprintf(buffer, "%s.ds", filename);
	fout = fopen(buffer, "w");
	FAILIF(fout==NULL);

	for (int i = 1; i <= n; i++) {
		fprintf(fout, "%d", i);
		for (int j = 0; j < d; j++) {
			random = genRandomInt(0, scale);
			fprintf(fout, " %d", random);
		}
		fprintf(fout, "\n");
	}
	fclose(fout);

	sprintf(buffer, "%s.q", filename);
	fout = fopen(buffer, "w");
	FAILIF(fout==NULL);

	for (int i = 1; i <= qn; i++) {
		fprintf(fout, "%d", i);
		for (int j = 0; j < d; j++) {
			random = genRandomInt(0, scale);
			fprintf(fout, " %d", random);
		}
		fprintf(fout, "\n");
	}
	fclose(fout);
}

//// Compare two points.
//bool cmp_coordinate(PPointT a, PPointT b) {
//	for (int i = 0; i < pointsDimension; i++) {
//		if (a->coordinates[i] != b->coordinates[i])
//			return a->coordinates[i] < b->coordinates[i];
//	}
//	return true;
//}

///**
// * Remove the duplicate points in dataset.
// */
//void removeDuplicate() {
//
//
//	sort(dataSetPoints, dataSetPoints + nPoints, cmp_coordinate);
//
//	printf("Finish sorting.\n");
//
//	int i = 0, j = 0;
//	while (i < nPoints - 1) {
//		j = i++;
//		while (i < nPoints && same(dataSetPoints[j], dataSetPoints[i])) {
//			dataSetPoints[i]->index = -1;
//			i++;
//		}
//	}
//}
