
#include <iostream>
#include <vector>

int main() {
	using namespace std;
	vector<int> v;
	for (int i = 0; i < 5; i++)
		v.push_back(i);

	auto it = v.begin(); // this is auto, clean my code
	while (it != v.end()) {
		cout << *it << endl;
		it++;
	}
	return 0;
}


