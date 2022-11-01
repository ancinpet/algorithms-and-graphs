#include <cstdlib>
#include <cstdio>
#include <cstddef>

int max(int a, int b) {
	return a > b ? a : b;
}

struct CVertex {
	CVertex(int i_value) : value(i_value), subsize(1), height(1), left(nullptr), right(nullptr) {}

	void print();
	CVertex * ceil(int findValue, int & index);
	CVertex * floor(int findValue, int & index);
	CVertex * find_index(int index);

	int value;
	int subsize;
	int height;
	CVertex * left;
	CVertex * right;
};

struct CStatisticsAVLTree {
	CStatisticsAVLTree() : root(nullptr), size(0) {}
	~CStatisticsAVLTree();

	void insert(int toInsert);
	CVertex * insert_r(CVertex * vertex, int toInsert);
	void del(int toDelete);
	CVertex * delete_r(CVertex * vertex, int toDelete);
	void print();
	CVertex * ceil(int findValue, int & index);
	CVertex * floor(int findValue, int & index);
	CVertex * find_index(int index);
	CVertex * right_rotate(CVertex * vertex);
	CVertex * left_rotate(CVertex * vertex);
	CVertex * get_min(CVertex * vertex);
	int get_bfactor(CVertex * vertex);
	void free_vertex(CVertex * vertex);

	CVertex * root;
	int size;
};

int main(void) {
	int input = 0;
	int firstParam = 0;
	int secondParam = 0;

	CStatisticsAVLTree tree;

	while (scanf("%d", &input)) {

		if (input == 1 && scanf("%d", &firstParam)) {
			tree.insert(firstParam);
		} else if (input == 2 && scanf("%d", &firstParam)) {
			tree.del(firstParam);
		} else if (input == 3 && scanf("%d %d", &firstParam, &secondParam)) {
			int low_index = -1;
			int high_index = -2;
			CVertex * low = tree.ceil(firstParam, low_index);
			CVertex * high = tree.floor(secondParam, high_index);

			if (low == nullptr || high == nullptr || high_index < low_index) {
				printf("notfound\n");
			} else {
				if (low_index == high_index) {
					printf("%d\n", low->value);
				} else {
					int range = high_index - low_index + 1;
					int median_index = low_index + (range / 2) - 1 + range % 2;

					CVertex * result = tree.find_index(median_index);
					if (result == nullptr) {
						printf("notfound\n");
					} else {
						printf("%d\n", result->value);
					}
				}
			}
		} else if (input == 4) {
			break;
		}
	}

	return 0;
}

void CStatisticsAVLTree::insert(int toInsert) {
	root = insert_r(root, toInsert);
	++size;
}

CVertex * CStatisticsAVLTree::insert_r(CVertex * vertex, int toInsert) {
	if (vertex == nullptr) {
		return new CVertex(toInsert);
	}

	if (toInsert < vertex->value) {
		vertex->left = insert_r(vertex->left, toInsert);
	} else {
		vertex->right = insert_r(vertex->right, toInsert);
	}

	vertex->subsize = (vertex->left == nullptr ? 0 : vertex->left->subsize) + (vertex->right == nullptr ? 0 : vertex->right->subsize) + 1;
	vertex->height = max((vertex->left == nullptr ? 0 : vertex->left->height), (vertex->right == nullptr ? 0 : vertex->right->height)) + 1;

	int balance = get_bfactor(vertex);
 
    //R-rot
    if (balance > 1 && toInsert < vertex->left->value) {
        return right_rotate(vertex);
    }
 
    //L-rot
    if (balance < -1 && toInsert > vertex->right->value) {
        return left_rotate(vertex);
    }
 
    //LR-rot
    if (balance > 1 && toInsert > vertex->left->value) {
    	vertex->left = left_rotate(vertex->left);
    	return right_rotate(vertex);
    }
 
    //RL-rot
    if (balance < -1 && toInsert < vertex->right->value) {
        vertex->right = right_rotate(vertex->right);
        return left_rotate(vertex);
    }

    return vertex;
}

void CStatisticsAVLTree::del(int toDelete) {
	root = delete_r(root, toDelete);
	--size;
}

CVertex * CStatisticsAVLTree::delete_r(CVertex * vertex, int toDelete) {
	if (vertex == nullptr) {
		return vertex;
	}

	if (toDelete < vertex->value) {
		vertex->left = delete_r(vertex->left, toDelete);
	} else if (toDelete > vertex->value) {
		vertex->right = delete_r(vertex->right, toDelete);
	} else {
        if (vertex->left == nullptr || vertex->right == nullptr) {
        	CVertex * tmp = nullptr;
        	if (vertex->left == nullptr) {
        		tmp = vertex->right;
        	} else {
        		tmp = vertex->left;
        	}

        	if (tmp == nullptr) {
        		tmp = vertex;
        		vertex = nullptr;
        		delete tmp;
        	} else {
        		vertex = tmp;
        	}
        } else {
        	//successor - most left in right subtree
        	CVertex * tmp = get_min(vertex->right);
        	//move successor value to this vertex
        	vertex->value = tmp->value;
        	//delete the successor node in the right subtree
        	vertex->right = delete_r(vertex->right, tmp->value);
        }
	}

	if (vertex == nullptr) {
		return vertex;
	}

	vertex->subsize = (vertex->left == nullptr ? 0 : vertex->left->subsize) + (vertex->right == nullptr ? 0 : vertex->right->subsize) + 1;
	vertex->height = max((vertex->left == nullptr ? 0 : vertex->left->height), (vertex->right == nullptr ? 0 : vertex->right->height)) + 1;

	int balance = get_bfactor(vertex);

    //R-rot
    if (balance > 1 && get_bfactor(vertex->left) >= 0) {
        return right_rotate(vertex);
    }

    //L-rot
    if (balance < -1 && get_bfactor(vertex->right) <= 0) {
    	return left_rotate(vertex);
    }
 
    //LR-rot
    if (balance > 1 && get_bfactor(vertex->left) < 0) {
    	vertex->left = left_rotate(vertex->left);
    	return right_rotate(vertex);
    }
 
    //RL-rot
    if (balance < -1 && get_bfactor(vertex->right) > 0) {
        vertex->right = right_rotate(vertex->right);
        return left_rotate(vertex);
    }

    return vertex;
}

void CStatisticsAVLTree::print() {
	if (root == nullptr) {
		printf("Tree is empty.");
	} else {
		root->print();
	}
	printf("\n");
}

CVertex * CStatisticsAVLTree::ceil(int findValue, int & index) {
	if (root != nullptr) {
		return root->ceil(findValue, index);
	} else {
		return nullptr;
	}
}

CVertex * CStatisticsAVLTree::floor(int findValue, int & index) {
	if (root != nullptr) {
		return root->floor(findValue, index);
	} else {
		return nullptr;
	}
}

CVertex * CStatisticsAVLTree::find_index(int index) {
	if (root != nullptr) {
		return root->find_index(index);
	} else {
		return nullptr;
	}
}

CVertex * CStatisticsAVLTree::right_rotate(CVertex * vertex) {
	CVertex * x = vertex->left;
	CVertex * rot = x->right;

	x->right = vertex;
	vertex->left = rot;

	vertex->subsize = (vertex->left == nullptr ? 0 : vertex->left->subsize) + (vertex->right == nullptr ? 0 : vertex->right->subsize) + 1;
	x->subsize = (x->left == nullptr ? 0 : x->left->subsize) + (x->right == nullptr ? 0 : x->right->subsize) + 1;
	vertex->height = max((vertex->left == nullptr ? 0 : vertex->left->height), (vertex->right == nullptr ? 0 : vertex->right->height)) + 1;
	x->height = max((x->left == nullptr ? 0 : x->left->height), (x->right == nullptr ? 0 : x->right->height)) + 1;

	return x;
}

CVertex * CStatisticsAVLTree::left_rotate(CVertex * vertex) {
	CVertex * y = vertex->right;
	CVertex * rot = y->left;

	y->left = vertex;
	vertex->right = rot;

	vertex->subsize = (vertex->left == nullptr ? 0 : vertex->left->subsize) + (vertex->right == nullptr ? 0 : vertex->right->subsize) + 1;
	y->subsize = (y->left == nullptr ? 0 : y->left->subsize) + (y->right == nullptr ? 0 : y->right->subsize) + 1;
	vertex->height = max((vertex->left == nullptr ? 0 : vertex->left->height), (vertex->right == nullptr ? 0 : vertex->right->height)) + 1;
	y->height = max((y->left == nullptr ? 0 : y->left->height), (y->right == nullptr ? 0 : y->right->height)) + 1;

	return y;
}

CVertex * CStatisticsAVLTree::get_min(CVertex * vertex) {
	if (vertex->left == nullptr) {
		return vertex;
	}

	return get_min(vertex->left);
}

int CStatisticsAVLTree::get_bfactor(CVertex * vertex) {
	if (vertex == nullptr) {
		return 0;
	}

	return (vertex->left == nullptr ? 0 : vertex->left->height) - (vertex->right == nullptr ? 0 : vertex->right->height);
}

CStatisticsAVLTree::~CStatisticsAVLTree() {
	free_vertex(root);
}

void CStatisticsAVLTree::free_vertex(CVertex * vertex) {
	if (vertex == nullptr) {
		return;
	}

	if (vertex->left != nullptr || vertex->right != nullptr) {
		free_vertex(vertex->left);
		free_vertex(vertex->right);
	}

	delete vertex;
}


void CVertex::print() {
	if (left != nullptr) {
		left->print();
	}
	printf("[%d : %d] ", value, height);
	if (right != nullptr) {
		right->print();
	}
}

CVertex * CVertex::ceil(int findValue, int & index) {
	if (findValue == value) {
		index = (left == nullptr ? 0 : left->subsize) + 1;
		return this;
	}

	if (value < findValue) {
		if (right != nullptr) {
			CVertex * ret = right->ceil(findValue, index);
			index += (left == nullptr ? 0 : left->subsize) + 1;
			return ret;
		} else {
			return nullptr;
		}
	}

	if (left == nullptr) {
		index = (left == nullptr ? 0 : left->subsize) + 1;
		return this;
	}

	CVertex * ret = left->ceil(findValue, index);

	if (ret == nullptr) {
		index = (left == nullptr ? 0 : left->subsize) + 1;
		return this;
	}

	if (ret->value >= findValue) {
		return ret;
	} else {
		index = (left == nullptr ? 0 : left->subsize) + 1;
		return this;
	}
}

CVertex * CVertex::floor(int findValue, int & index) {
	if (findValue == value) {
		index = (left == nullptr ? 0 : left->subsize) + 1;
		return this;
	}

	if (value > findValue) {
		if (left != nullptr) {
			return left->floor(findValue, index);
		} else {
			return nullptr;
		}
	}

	if (right == nullptr) {
		index = (left == nullptr ? 0 : left->subsize) + 1;
		return this;
	}

	CVertex * ret = right->floor(findValue, index);
	index += (left == nullptr ? 0 : left->subsize) + 1;
	if (ret == nullptr) {
		index = (left == nullptr ? 0 : left->subsize) + 1;
		return this;
	}

	if (ret->value <= findValue) {
		return ret;
	} else {
		index = (left == nullptr ? 0 : left->subsize) + 1;
		return this;
	}
}

CVertex * CVertex::find_index(int index) {
    int val = (left == nullptr ? 0 : left->subsize) + 1;

    if (index == val) {
    	return this;
    } else if (index < val) {
    	if (left == nullptr) {
    		return nullptr;
    	} else {
    		return left->find_index(index);
    	}
    } else {
    	if (right == nullptr) {
    		return nullptr;
    	} else {
    		return right->find_index(index - val);
    	}
    }
}