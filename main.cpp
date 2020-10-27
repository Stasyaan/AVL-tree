#include <functional>
#include <iostream>
#include <string>
#include <iomanip>
template <typename T>
class Set {
public:
	typedef std::function<signed char(T, T)> fn;
	Set(fn _cmp = defaultCMP) :cmp(_cmp) {
		tree = NULL;
	}
	~Set() {
		if (tree)delete tree;
	}
	void print() {
		_print(tree,0,"");
	}
	void insert(T key) {
		tree = _insert(tree, key);
	}

	void remove(T key) {
		tree = remove(tree, key);
	}

	T find(T val) {
		return _find(tree, val);
	}

private:
	signed char defaultCMP(T& left, T& right) {
		if (left > right)return 1;
		else if (left < right)return -1;
		else return 0;
	}
	fn cmp;
	class node {
	public:
		T key;
		unsigned char height;
		node* left;
		node* right;
		node(T k) { key = k; left = right = 0; height = 1; }
		~node(){
			if (left) delete left;
			if (right) delete right;
		}
	};

	unsigned char height(node* p)
	{
		return p ? p->height : 0;
	}

	int bfactor(node* p)
	{
		return height(p->right) - height(p->left);
	}

	void fixheight(node* p) {
		unsigned char hl = height(p->left);
		unsigned char hr = height(p->right);
		p->height = (hl > hr ? hl : hr) + 1;
	}

	node* rotateright(node* p) {// правый поворот вокруг p
		node* l = p->left;
		p->left = l->right;
		l->right = p;
		fixheight(p);
		fixheight(l);
		return l;
	}

	node* rotateleft(node* q) { // левый поворот вокруг q
		node* p = q->right;
		q->right = p->left;
		p->left = q;
		fixheight(q);
		fixheight(p);
		return p;
	}

	node* balance(node* p) { // балансировка узла p
		fixheight(p);
		if (bfactor(p) == 2)
		{
			if (bfactor(p->right) < 0)
				p->right = rotateright(p->right);
			return rotateleft(p);
		}
		if (bfactor(p) == -2)
		{
			if (bfactor(p->left) > 0)
				p->left = rotateleft(p->left);
			return rotateright(p);
		}
		return p; // балансировка не нужна
	}
	node* _insert(node* p, T k) { // вставка ключа k в дерево с корнем p
		if (!p) return new node(k);
		int result = cmp(k, p->key);
		if (result < 0)
			p->left = _insert(p->left, k);
		else
			p->right = _insert(p->right, k);
		return balance(p);
	}
	node* findmin(node* p) { // поиск узла с минимальным ключом в дереве p 
		return p->left ? findmin(p->left) : p;
	}

	node* removemin(node* p) { // удаление узла с минимальным ключом из дерева p
		if (p->left == 0)
			return p->right;
		p->left = removemin(p->left);
		return balance(p);
	}

	node* remove(node* p, T k) { // удаление ключа k из дерева p
		if (!p) return 0;
		int result = cmp(k, p->key);
		if (result < 0)p->left = remove(p->left, k);
		else if (result > 0)p->right = remove(p->right, k);
		else {//  k == p->key
			node* l = p->left;
			node* r = p->right;
			p->left = NULL;
			p->right = NULL;
			delete p;
			if (!r) return l;
			node* min = findmin(r);
			min->right = removemin(r);
			min->left = l;
			return balance(min);
		}
		return balance(p);
	}

	T _find(node* p, T val) {
		if (!p) { 
			T tmp;
			return tmp;
		};
		int result = cmp(val, p->key);
		if (result < 0)
			return _find(p->left, val);
		else if (result > 0)
			return _find(p->right, val);
		else
			return p->key;
	}
	node* tree;
	void _print(node* p,int from, std::string s) {
		if (p->right) {
			if (from == -1)_print(p->right, 1, s + (char)186 + "          ");
			else _print(p->right, 1, s + "           ");
			std::cout << '\n';
		}

		if (from > 0) std::cout << s << (char)201 << p->key;
		if (from < 0) std::cout << s << (char)200 <<  p->key;
		if (from == 0) std::cout<<p->key<<" ";

		if (p->right && p->left)std::cout<< (char)185 << '\n';
		if (p->right && !p->left)std::cout << (char)188;
		if (!p->right && p->left)std::cout << (char)187<<'\n';

		if (p->left) {
			if (from == 1)_print(p->left,-1,s + (char)186 + "          ");
			else _print(p->left, -1, s + "           ");
		}
	}
};


class wood {
public:
	wood(int _key = -1, int _value = -1) {
		key = _key;
		value = _value;
	}
	int key;
	int value;
	friend 	std::ostream& operator<< (std::ostream& out, const wood& obj);

};

std::ostream& operator<< (std::ostream& out, const wood& obj) {
	out << "(" << std::setw(3) << std::right << obj.key << ", " << std::setw(3) << std::right << obj.value << ")";
	return out;
}

int main() {
	Set <wood>obj([](wood left, wood right) {
		if (left.key > right.key)return 1;
		else if (left.key < right.key)return -1;
		else return 0;
		});

	wood tmp;
	for (int i = 0; i <31; i++) {
		tmp.key = rand()%100;
		tmp.value = i;
		obj.insert(tmp);
	}
	std::cout << "(KEY, VALUE)\n";
	obj.print();
	std::cout << std::endl;
	tmp.key = 41;
	obj.remove(tmp);
	obj.print();
	std::cout << std::endl;

	std::cout<<obj.find(tmp);
}
