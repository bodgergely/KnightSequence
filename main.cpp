
#include "knight.h"

using namespace std;

int main(int argc, char** argv)
{
	int steps = 17;
	CompositeGenerator knight(steps, 2);
	auto start = chrono::system_clock::now();
	unsigned long long count = knight.count();
	auto end = chrono::system_clock::now();
	cout << count << endl;
	cout << "Took: " << chrono::duration_cast<chrono::seconds>(end-start).count() << endl;

	return 0;
}







