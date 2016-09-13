
#include "knight.h"

using namespace std;

int main(int argc, char** argv)
{
	int steps = 17;
	CompositeGenerator knight(steps, 2);
	unsigned long long count = knight.count();
	cout << count << endl;

	return 0;
}







