#include <iostream>
using namespace std;

class node {
public:
	int v;
	node* sig[2];
	node(int as) {
		v = as;
		sig[0] = sig[1] = nullptr;
	}
};
class arbol {
public:
	node* root=nullptr;

};
int main{
	return;
}
