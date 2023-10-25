/*
 * Main.cpp
 *
 *  Created on: 2011-9-26
 *      Author: Junhao Gan
 */

#include "headers.h"

/*
 * Prints the usage of the Raw Dataset Processor.
 */
void usage() {
	printf("Raw Dataset Processor coded by Junhao Gan, Sun Yat-sen University\n");
	printf("Option\n");
	printf("-opt{integer}\t 1 for Color; 2 for Mnist; 3 for Audio; 4 for any other datasets in formating \"id coordinates\" in text (i.e. LabelMe and RandInt).\n");
	printf("-n  {integer}\t the number of points\n");
	printf("-qn {integer}\t the number of queries\n");
	printf("-d  {integer}\t the dimensionality of points\n");
	printf("-ds {string}\t the file path of raw dataset\n");
	printf("-qs {string}\t the file path of query set\n");
	printf("-s  {double}\t the value of the largest coordinate value after scaled\n");
	printf("-td {integer}\t the dimensionality of data points that you want\n");
	printf("-of {string}\t the folder path of output files\n");
	printf("\n");

	printf("1. Process raw Color dataset.\n");
	printf("   Parameter list: -opt 1 -n -d -ds -of -qn [-s]\n");
	printf("\n");

	printf("2. Process raw Mnist dataset.\n");
	printf("   Parameter list: -opt 2 -n -d -ds -of -qn [-s] -td -qs\n");
	printf("\n");

	printf("3. Process raw Audio dataset.\n");
	printf("   Parameter list: -opt 3 -n -d -ds -of -qn [-s]\n");
	printf("\n");

	printf("4. Process any other datasets in formating \"id coordinates\"in text (i.e. LabelMe and RandInt).\n");
	printf("   Parameter list: -opt 4 -n -d -ds -of -qn [-s]\n");
	printf("\n");

	printf("5. Generate RandInt dataset.\n");
	printf("   Parameter list: -opt 5 -n -d -of -qn [-s]\n");
	printf("\n");
}

/*
 * Get the index of next unblank char from a string.
 */
int GetNextChar(char *str) {
	int rtn = 0;

	// Jump over all blanks
	while (str[rtn] == ' ') {
		rtn++;
	}

	return rtn;
}

/*
 * Get next word from a string.
 */
void GetNextWord(char *str, char *word) {
	// Jump over all blanks
	while (*str == ' ') {
		str++;
	}

	while (*str != ' ' && *str != '\0') {
		*word = *str;
		str++;
		word++;
	}

	*word = '\0';
}

int main(int nargs, char **args) {

	// Initialize the random seed.
	srand((unsigned) time(NULL));

	usage();

	// These two are global variables
	nPoints = -1; // the number of points
	pointsDimension = -1; // the dimensionality of points

	int opt = -1; // the operation number
	int qn = -1; // the number of queries
	int td = -1; // the target dimensionality of processed data
	double s = -1; // the value of the largest coordinate value after scaled
	char ds[100] = ""; // the file path of raw dataset
	char qs[100] = ""; // the file path of query set
	char gt[100] = ""; // the file path of ground truth
	char of[100] = ""; // the folder path of output files


	int cnt = 1;
	bool failed = false;
	char *arg;
	int i;
	char para[10];

	while (cnt < nargs && !failed) {
		arg = args[cnt++];
		if (cnt == nargs) {
			failed = true;
			break;
		}

		i = GetNextChar(arg);
		if (arg[i] != '-') {
			failed = true;
			break;
		}

		GetNextWord(arg + i + 1, para);
		printf("%s\t", para);

		arg = args[cnt++];
		if (strcmp(para, "opt") == 0) {
			opt = atoi(arg);
			if (opt <= 0 || opt > 5) {
				failed = true;
				break;
			}
			printf("%d\n", opt);
		} else if (strcmp(para, "n") == 0) {
			nPoints = atoi(arg);
			if (nPoints <= 0) {
				failed = true;
				break;
			}
			printf("%d\n", nPoints);
		} else if (strcmp(para, "d") == 0) {
			pointsDimension = atoi(arg);
			if (pointsDimension <= 0) {
				failed = true;
				break;
			}
			printf("%d\n", pointsDimension);
		} else if (strcmp(para, "qn") == 0) {
			qn = atoi(arg);
			if (qn <= 0) {
				failed = true;
				break;
			}
			printf("%d\n", qn);
		} else if (strcmp(para, "td") == 0) {
			td = atoi(arg);
			if (td <= 0) {
				failed = true;
				break;
			}
			printf("%d\n", td);
		} else if (strcmp(para, "s") == 0) {
			s = atof(arg);
			if (s <= 0) {
				failed = true;
				break;
			}
			printf("%lf\n", s);
		} else if (strcmp(para, "ds") == 0) {
			GetNextWord(arg, ds);
			printf("%s\n", ds);
		} else if (strcmp(para, "qs") == 0) {
			GetNextWord(arg, qs);
			printf("%s\n", qs);
		} else if (strcmp(para, "of") == 0) {
			GetNextWord(arg, of);
			printf("%s\n", of);
		} else {
			failed = true;
			printf("Unknown option -%s!\n\n", para);
		}

	}

	if (failed) {
		usage();
		return 0;
	}

	switch (opt) {
	case 1:
		// Color
		if (s == -1)
			s = 1000000;
		readDataSetFromFile(ds, nPoints, pointsDimension);
		processDataSet(of, qn, s);
		sprintf(qs, "%s.q", of);
		sprintf(gt, "%s.gt", of);
		sprintf(ds, "%s.ds", of);
		// Read the processed dataset.
		readDataSetFromFile(ds, nPoints - qn, pointsDimension);
		writeGroundTruth(qs, gt, qn);
		printf("Color Process Finished.\n");
		break;
	case 2:
		// Mnist
		if (s == -1)
			s = 255.0;
		readMnistBinaryDataSet(ds, qs, nPoints, 10000, pointsDimension);
		writeMnistDatasetAndQueryset(of, td, s);
		sprintf(qs, "%s.q", of);
		sprintf(gt, "%s.gt", of);
		sprintf(ds, "%s.ds", of);
		// Read the processed dataset.
		readDataSetFromFile(ds, nPoints, td);
		writeGroundTruth(qs, gt, qn);
		printf("Mnist Process Finished.\n");
		break;
	case 3:
		// Audio
		if (s == -1)
			s = 100000;
		readAudioBinaryDataSet(ds, nPoints, pointsDimension);
//		printf("%lf %lf %lf\n", maxCoordinate, minCoordinate, maxCoordinate - minCoordinate);
		processDataSet(of, qn, s);
		sprintf(qs, "%s.q", of);
		sprintf(gt, "%s.gt", of);
		sprintf(ds, "%s.ds", of);
		// Read the processed dataset.
		readDataSetFromFile(ds, nPoints - qn, pointsDimension);
		writeGroundTruth(qs, gt, qn);
		printf("Audio Process Finished.\n");
		break;
	case 4:
		// Any other dataset in formating "id coordinates" in text (i.e. LabelMe and RandInt).
		if (s == -1)
			s = 10000;
		readDataSetFromFile(ds, nPoints, pointsDimension);
		processDataSet(of, qn, s);
		sprintf(qs, "%s.q", of);
		sprintf(gt, "%s.gt", of);
		sprintf(ds, "%s.ds", of);
		// Read the processed dataset.
		readDataSetFromFile(ds, nPoints - qn, pointsDimension);
		writeGroundTruth(qs, gt, qn);
		printf("Dataset Process Finished.\n");
		break;
	case 5:
		if (s == -1)
			s = 10000;
		generateRandInt(of, nPoints, pointsDimension, qn, s);
		sprintf(ds, "%s.ds", of);
		readDataSetFromFile(ds, nPoints, pointsDimension);
		sprintf(qs, "%s.q", of);
		sprintf(gt, "%s.gt", of);
		writeGroundTruth(qs, gt, qn);
		printf("RandInt Process Finished.\n");
		break;
	default:
		printf("Operation Number Error! Please input valid operation number.\n");
		break;
	}

	return 0;
}
