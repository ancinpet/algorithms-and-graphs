#include <cstdlib>
#include <iostream>

//#define DBG

using namespace std;

#define STATE_NEW 0
#define STATE_PROCESSING 1
#define STATE_FINISHED 2

struct CPart {
	CPart() : startMakingTime(0), state(STATE_NEW), vecDependencies(NULL), vecDepSize(0) {}

	~CPart() {
		if (vecDependencies != NULL) {
			delete[] vecDependencies;
		}
	}

	unsigned long startMakingTime;
	unsigned char state;
	unsigned long * vecDependencies;
	unsigned int vecDepSize;
};

void dfs_komp(unsigned long * vecTimeToMakePart, CPart * vecParts, unsigned long index) {
	if (vecParts[index].state == STATE_PROCESSING) {
		printf("%s", "No solution.\n");
		delete[] vecTimeToMakePart;
		delete[] vecParts;
		exit(0);
	}

	if (vecParts[index].state == STATE_FINISHED) {
		return;
	}

	vecParts[index].state = STATE_PROCESSING;

	unsigned long tmpMax = 0;
	for (unsigned long i = 0; i < vecParts[index].vecDepSize; ++i) {
		dfs_komp(vecTimeToMakePart, vecParts, vecParts[index].vecDependencies[i]);
		
		if (vecParts[vecParts[index].vecDependencies[i]].startMakingTime > tmpMax) {
			tmpMax = vecParts[vecParts[index].vecDependencies[i]].startMakingTime;
		}
	}

	vecParts[index].startMakingTime += tmpMax;
	vecParts[index].startMakingTime += vecTimeToMakePart[index];
	vecParts[index].state = STATE_FINISHED;
}

int main(void) {
	unsigned long numOfParts = 0;

	/* Load amount of parts */
	cin >> numOfParts;

	unsigned long * vecTimeToMakePart = new unsigned long[numOfParts];
	CPart * vecParts = new CPart[numOfParts];

	/* Load how much time does it take to make a part */
	for (unsigned long i = 0; i < numOfParts; ++i) {
		cin >> vecTimeToMakePart[i];
	}

	/* Load dependencies */
	for (unsigned long i = 0; i < numOfParts; ++i) {

		unsigned long amountOfRequiedParts;
		cin >> amountOfRequiedParts;

		vecParts[i].vecDependencies = new unsigned long[amountOfRequiedParts];
		vecParts[i].vecDepSize = amountOfRequiedParts;


		for (unsigned long j = 0; j < amountOfRequiedParts; ++j) {
			unsigned long tmpPartDependency;
			cin >> tmpPartDependency;

			vecParts[i].vecDependencies[j] = tmpPartDependency;
		}

	}

	#ifdef DBG
	 cout << "size: " << numOfParts << endl;

	 for (unsigned long i = 0; i < numOfParts; ++i) {
	     cout << vecTimeToMakePart[i] << " ";
	 }
	 cout << endl;


	 for (unsigned long i = 0; i < numOfParts; ++i) {

	 	 cout << vecParts[i].vecDepSize;

		 for (unsigned long j = 0; j < vecParts[i].vecDepSize; ++j) {
			 cout << " " << vecParts[i].vecDependencies[j];
		 }
		 cout << endl;
	 }
	#endif

	for (unsigned long i = 0; i < numOfParts; ++i) {
		dfs_komp(vecTimeToMakePart, vecParts, i);
	}

	unsigned long max = 0;
	for (unsigned long i = 0; i < numOfParts; ++i) {
		if (vecParts[i].startMakingTime > max) {
			max = vecParts[i].startMakingTime;
		}
	}

	printf("%lu\n", max);

	for (unsigned long i = 0; i < numOfParts; ++i) {
		if (i == numOfParts - 1) {
			printf("%lu\n", vecParts[i].startMakingTime - vecTimeToMakePart[i]);
		} else {
			printf("%lu ", vecParts[i].startMakingTime - vecTimeToMakePart[i]);
		}
	}
}