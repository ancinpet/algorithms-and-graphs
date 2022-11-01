#include <cstdlib>
#include <cstdio>
#include <cstddef>
#include <cstring>
#include <limits>
#include <iostream>

using namespace std;

struct CStreet {
	CStreet() : m_from(-1), m_to(-1), m_length(-1) {}

	int m_from;
	int m_to;
	int m_length;
};

struct CPlazaNeighbour {
	CPlazaNeighbour() : m_to(numeric_limits<int>::max()), m_length(numeric_limits<int>::max()), m_index(numeric_limits<int>::max()) {}

	int m_to;
	int m_length;
	int m_index;
};

struct CPlaza {
	CPlaza() : m_neighbours(nullptr), m_numOfNeighbours(0), visited(false), used(false) {}

	void add(int to, int length, int index) {
		m_neighbours[m_numOfNeighbours].m_to = to;
		m_neighbours[m_numOfNeighbours].m_length = length;
		m_neighbours[m_numOfNeighbours++].m_index = index;
	}

	CPlazaNeighbour * m_neighbours;
	int m_numOfNeighbours;
	bool visited;
	bool used;
};


struct CHeap {
	CHeap(int size) : m_data(new CPlazaNeighbour[size]), m_heapSize(0) {}

	void print() {
		cout << "size: " << m_heapSize << endl;
		for (int i = 0; i < m_heapSize; ++i) {
			cout << m_data[i].m_length << "-" << m_data[i].m_index << "-" << m_data[i].m_to << "  ";
		}
		cout << endl;
	}

	void swap(int a, int b) {
		int tmpTo = m_data[a].m_to;
		int tmpLen = m_data[a].m_length;
		int tmpInd = m_data[a].m_index;

		m_data[a].m_to = m_data[b].m_to;
		m_data[a].m_length = m_data[b].m_length;
		m_data[a].m_index = m_data[b].m_index;

		m_data[b].m_to = tmpTo;
		m_data[b].m_length = tmpLen;
		m_data[b].m_index = tmpInd;
	}

	void bubbleUp(int currentIndex) {
		if (currentIndex == 0) {
        	return;
		}

	    int parentIndex = (currentIndex - 1) / 2;

	    if (m_data[parentIndex].m_length > m_data[currentIndex].m_length) {
	    	swap(parentIndex, currentIndex);
	        bubbleUp(parentIndex);
	    }
	}

	void bubbleDown(int currentIndex) {
		int left = (2 * currentIndex) + 1;
		int right = (2 * currentIndex) + 2;

    	if (left >= m_heapSize) {
    		return;
    	}

    	int minIndex = currentIndex;

    	if (m_data[currentIndex].m_length > m_data[left].m_length) {
	        minIndex = left;
	    }
    
	    if ((right < m_heapSize) && (m_data[minIndex].m_length > m_data[right].m_length)) {
	        minIndex = right;
	    }

	    if (minIndex != currentIndex) {
	        swap(minIndex, currentIndex);
	        bubbleDown(minIndex);
	    }
	}

	void add(CPlazaNeighbour & toAdd) {
		m_data[m_heapSize].m_to = toAdd.m_to;
		m_data[m_heapSize].m_length = toAdd.m_length;
		m_data[m_heapSize].m_index = toAdd.m_index;
		bubbleUp(m_heapSize);
		++m_heapSize;
	}

	CPlazaNeighbour extractMin() {
		CPlazaNeighbour tmp;
		tmp.m_to = m_data[0].m_to;
		tmp.m_length = m_data[0].m_length;
		tmp.m_index = m_data[0].m_index;

		swap(0, --m_heapSize);

		bubbleDown(0);

		return tmp;
	}

	CPlazaNeighbour * m_data;
	int m_heapSize;
};

void jarnik(CPlaza * plazas, int numOfVerticles, int numOfEdges, int * res, int & resCount) {
	CHeap edgeHeap(numOfEdges * 2);

	plazas[0].visited = true;
	for (int i = 0; i < plazas[0].m_numOfNeighbours; ++i) {
		edgeHeap.add(plazas[0].m_neighbours[i]);
	}

	while (resCount < numOfVerticles - 1) {
		//cout << "pre extract "; edgeHeap.print();
		CPlazaNeighbour tmp = edgeHeap.extractMin();
		//cout << "post extract "; edgeHeap.print();
		if (!plazas[tmp.m_to].visited) {
			res[resCount++] = tmp.m_index;
		}
		plazas[tmp.m_to].visited = true;

		for (int i = 0; i < plazas[tmp.m_to].m_numOfNeighbours; ++i) {
			CPlazaNeighbour neighbour = plazas[tmp.m_to].m_neighbours[i];

			if (!plazas[neighbour.m_to].visited) {
				edgeHeap.add(neighbour);
			}
		}
	}
}

void minVertex(CPlaza * plazas, CPlaza & search) {
	search.visited = true;

	for (int i = 0; i < search.m_numOfNeighbours; ++i) {
		int neighbourVertex = search.m_neighbours[i].m_to;
		if (!plazas[neighbourVertex].visited) {
			minVertex(plazas, plazas[neighbourVertex]);

			if (!plazas[neighbourVertex].used) {
				search.used = true;
			}
		}
	}
}

int main(void) {
	int numPlazas = 0, numStreets = 0;

	if (scanf("%d %d", &numPlazas, &numStreets) != 2) {
		return 1;
	}

	int * streetsInPlaza = new int[numPlazas];
	CStreet * streetArray = new CStreet[numStreets];
	CPlaza * plazaArray = new CPlaza[numPlazas];

	memset(streetsInPlaza, 0, numPlazas*sizeof(int));

	int from, to, length;
	for (int i = 0; i < numStreets; ++i) {
		if (scanf("%d %d %d", &from, &to, &length) != 3) {
			return 1;
		}

		++streetsInPlaza[from];
		++streetsInPlaza[to];

		streetArray[i].m_from = from;
		streetArray[i].m_to = to;
		streetArray[i].m_length = length;
	}

	for (int i = 0; i < numPlazas; ++i) {
		plazaArray[i].m_neighbours = new CPlazaNeighbour[streetsInPlaza[i]];
	}

	for (int i = 0; i < numStreets; ++i) {
		plazaArray[streetArray[i].m_from].add(streetArray[i].m_to, streetArray[i].m_length, i);
		plazaArray[streetArray[i].m_to].add(streetArray[i].m_from, streetArray[i].m_length, i);
	}


	/*TESTS*/
	/*
	printf("HEAP PRINT\n");
	for (int i = 0; i < numPlazas; ++i) {
		printf("Plaza %d - ", i);
		for (int j = 0; j < plazaArray[i].m_numOfNeighbours; ++j) {
			printf("%d:%d ", plazaArray[i].m_neighbours[j].m_to, plazaArray[i].m_neighbours[j].m_length);
		}
		printf("\n");
	}
	*/

	int * res = new int[numPlazas - 1];
	int resCount = 0;
	jarnik(plazaArray, numPlazas, numStreets, res, resCount);

	long long int sumOfEdges = 0;
	for (int i = 0; i < resCount; ++i) {
		sumOfEdges += streetArray[res[i]].m_length;
	}


	CPlaza * plazaArray2 = new CPlaza[numPlazas];

	for (int i = 0; i < numPlazas; ++i) {
		plazaArray2[i].m_neighbours = new CPlazaNeighbour[streetsInPlaza[i]];
	}

	for (int i = 0; i < resCount; ++i) {
		plazaArray2[streetArray[res[i]].m_from].add(streetArray[res[i]].m_to, streetArray[res[i]].m_length, res[i]);
		plazaArray2[streetArray[res[i]].m_to].add(streetArray[res[i]].m_from, streetArray[res[i]].m_length, res[i]);
	}

	minVertex(plazaArray2, plazaArray2[0]);

	long long int numOfVers = 0;
	for (int i = 0; i < numPlazas; ++i) {
		if (plazaArray2[i].used) {
			++numOfVers;
		}
	}

	printf("%lld %lld\n", sumOfEdges, numOfVers);
	printf("%d", res[0]);
	for (int i = 1; i < resCount; ++i) {
		 printf(" %d", res[i]);
	}
	printf("\n");
	bool once = true;
	for (int i = 0; i < numPlazas; ++i) {
		if (plazaArray2[i].used) {
			if (once) {
				once = false;
				printf("%d", i);
			} else {
				printf(" %d", i);
			}
		}
	}
	printf("\n");

	return 0;
}