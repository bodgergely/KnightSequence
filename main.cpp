

#include <iostream>
#include <memory>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <cassert>
#include <chrono>
#include <thread>

using namespace std;

/*
template<class T>
class Node
{
public:
	using NodePtr = std::shared_ptr<T>;
	Node(const T& val) : _val(val) {}
private:
	T _val;
	std::vector<NodePtr>  _neighbors;
};

using NodePtr = typename Node<class T>::NodePtr;

template<class T>
class Graph
{
public:
	void addNode(const NodePtr& val);
private:

};


template<typename T>
class DepthFirstExplorer
{
public:
	DepthFirstExplorer(const Graph& graph)
private:
	Graph<T> _graph;
};
*/


template<class T>
class AdjancencyMatrix
{
public:
	AdjancencyMatrix(T size, const pair<T, T>& interval) : _size(size), _interval(interval)
	{
		for(int i=0;i<size;i++)
			_table.push_back(vector<bool>(size, false));
	}

	void addConnection(T row, T col, bool bidirectional=true)
	{
		if(row>=_size || col >= _size)
			throw std::runtime_error("Row or col bigger");

		_table[row][col] = true;
		if(bidirectional)
			_table[col][row] = true;
	}

	vector<T> neighbors(T val) const
	{
		vector<T> nb;
		if(val < _interval.first || val > _interval.second)
			throw std::runtime_error("Val is outside the interval!");
		const vector<bool>& row = _table[val];
		for(int i=_interval.first;i<=_interval.second;i++)
		{
			if(row[i] == true)
				nb.push_back(i);
		}
		return std::move(nb);
	}

private:
	T _size;
	vector<vector<bool>> _table;
	pair<T, T> 	 _interval;
};




class KnightSequenceGenerator
{
public:
	KnightSequenceGenerator(char start, int sequenceLen, uint maxVowelCount) : _startChar(start), _maxVowelCount(maxVowelCount), _seqLen(sequenceLen), _count(0)
	{
		/*
		_adjMatrix.addConnection('A', 'L');
		_adjMatrix.addConnection('A', 'H');
		_adjMatrix.addConnection('B', 'K');
		_adjMatrix.addConnection('B', 'M');
		_adjMatrix.addConnection('B', 'I');
		_adjMatrix.addConnection('C', 'F');
		_adjMatrix.addConnection('C', 'L');enerate();
		_adjMatrix.addConnection('C', 'J');
		_adjMatrix.addConnection('D', 'M');
		_adjMatrix.addConnection('D', 'O');
		_adjMatrix.addConnection('D', 'G');
		_adjMatrix.addConnection('E', 'H');
		_adjMatrix.addConnection('E', 'N');

		_adjMatrix.addConnection('F', '1');
		_adjMatrix.addConnection('F', 'M');
		_adjMatrix.addConnection('G', '2');
		_adjMatrix.addConnection('G', 'N');
		_adjMatrix.addConnection('H', 'K');
		_adjMatrix.addConnection('H', '1');
		_adjMatrix.addConnection('H', '3');
		_adjMatrix.addConnection('H', 'O');
		_adjMatrix.addConnection('I', 'L');
		_adjMatrix.addConnection('I', '2');
		_adjMatrix.addConnection('J', 'M');
		_adjMatrix.addConnection('J', '3');

		_adjMatrix.addConnection('K', '2');
		_adjMatrix.addConnection('L', '3');
		_adjMatrix.addConnection('N', '1');
		_adjMatrix.addConnection('O', '2');
		*/

		_neighborMap['A'] = {'L', 'H'};
		_neighborMap['B'] = {'K', 'M', 'I'};
		_neighborMap['C'] = {'F', 'L', 'J', 'N'};
		_neighborMap['D'] = {'M', 'O', 'G'};
		_neighborMap['E'] = {'H', 'N'};
		_neighborMap['F'] = {'1', 'M', 'C'};
		_neighborMap['G'] = {'2', 'N', 'D'};
		_neighborMap['H'] = {'A','E','K', '1', '3', 'O'};
		_neighborMap['I'] = {'L', '2', 'B'};
		_neighborMap['J'] = {'M', '3', 'C'};
		_neighborMap['K'] = {'2', 'B', 'H'};
		_neighborMap['L'] = {'A','C', 'I', '3'};
		_neighborMap['M'] = {'F','B', 'D', 'J'};
		_neighborMap['N'] = {'1', 'G', 'C', 'E'};
		_neighborMap['O'] = {'2', 'H', 'D'};
		_neighborMap['1'] = {'F', 'H', 'N'};
		_neighborMap['2'] = {'K', 'O', 'G', 'I'};
		_neighborMap['3'] = {'L', 'H', 'J'};



	}
	virtual ~KnightSequenceGenerator()
	{

	}

	unsigned long long generate()
	{
		_generate(_startChar, 0);
		return _count;
	}


	void generateThreaded()
	{
		_thread = thread(&KnightSequenceGenerator::_generate, this, _startChar, 0);
	}
	virtual unsigned long long count()
	{
		if(_thread.joinable())
			_thread.join();

		return _countpriv();
	}

protected:

	virtual unsigned long long _countpriv() {return _count;}

	virtual void _generate(char currChar, int depth)
	{
		//cout << depth << endl;

		if(is_vowel(currChar))
			++_vowelCnt;
		//_seq.push_back(currChar);
		if(depth+1 == _seqLen)
		{
			//assert(_seq.size() == depth+1);
			++_count;
			popBack(currChar);
			return;
		}

		const vector<char>& nbs = _neighborMap[currChar];
		//vector<char> nbs = _adjMatrix.neighbors(currChar);
		//cout << _seq << " Will try: ";
		//for(char c : nbs)
		//	cout << c << " ";
		//cout << endl;
		for(const char& c : nbs)
		{
			if(is_vowel(c) && _vowelCnt==_maxVowelCount)
				; // noop
			else
			{
				_generate(c, depth+1);
			}

		}
		popBack(currChar);

	}


	void popBack(char ch)
	{
		if(is_vowel(ch))
			--_vowelCnt;
		//_seq.pop_back();
	}

	inline bool is_vowel(char c) const
	{
		if(c == 'A' || c == 'E' || c == 'I' || c == 'O')
			return true;
		else
			return false;
	}

protected:

	char _startChar;
	unordered_map<char, vector<char> > _neighborMap;
	//string _seq;
	uint   _vowelCnt{0};
	const uint   _maxVowelCount;
	int _seqLen;
	//AdjancencyMatrix<char> _adjMatrix;
	unsigned long long	   _count;
	thread _thread;
	//unordered_set<string> _sequenceSet;
};


class KnightSequenceGeneratorTester : public KnightSequenceGenerator
{
public:
	KnightSequenceGeneratorTester(char start, int sequenceLen, uint maxVowelCount) : KnightSequenceGenerator(start, sequenceLen, maxVowelCount) {}
	virtual ~KnightSequenceGeneratorTester() {}

	unordered_set<string> getSequences() {return _sequenceSet;}

protected:
	virtual unsigned long long _countpriv() {return _sequenceSet.size();}
	virtual void _generate(char currChar, int depth)
	{
		//cout << depth << endl;
		if(depth==_seqLen)
		{
			assert(_seq.size() == depth);
			if(_sequenceSet.find(_seq) == _sequenceSet.end())
				_sequenceSet.insert(_seq);
			return;
		}

		if(is_vowel(currChar))
			++_vowelCnt;
		_seq.push_back(currChar);

		const vector<char>& nbs = _neighborMap[currChar];
		for(char c : nbs)
		{
			if(is_vowel(c) && _vowelCnt==_maxVowelCount)
				; // noop
			else
				_generate(c, depth+1);

		}
		popBack();

	}


	void popBack()
	{
		if(is_vowel(_seq.back()))
			--_vowelCnt;
		_seq.pop_back();
	}

	bool is_vowel(char c)
	{
		if(c == 'A' || c == 'E' || c == 'I' || c == 'O')
			return true;
		else
			return false;
	}

private:
	string _seq;
	unordered_set<string> _sequenceSet;
};

using KnightSequenceGeneratorPtr = std::unique_ptr<KnightSequenceGenerator>;

class CompositeGenerator
{
public:
	CompositeGenerator(int sequenceLen, uint maxVowelCount, bool tester = false)
	{
		for(char c : _chars)
		{
			if(!tester)
				_generators.push_back(KnightSequenceGeneratorPtr(new KnightSequenceGenerator(c, sequenceLen, maxVowelCount)));
			else
				_generators.push_back(KnightSequenceGeneratorPtr(new KnightSequenceGeneratorTester(c, sequenceLen, maxVowelCount)));
		}
	}

	unsigned long long count()
	{
		for(auto& g : _generators)
			g->generateThreaded();

		unsigned long long totalCount = 0;
		for(auto& g : _generators)
			totalCount += g->count();

		return totalCount;
	}

private:
	const vector<char> _chars{'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', '1', '2', '3'};
	vector<KnightSequenceGeneratorPtr> _generators;

};


int main(int argc, char** argv)
{
	int steps = 17;
	CompositeGenerator knightPerf(steps, 2);
	CompositeGenerator knightTest(steps, 2, true);
	auto start = chrono::system_clock::now();
	unsigned long long countPerf = knightPerf.count();
	cout << "Count Perf: " << countPerf << endl;
	cout << "Took Perf (secs): " << chrono::duration_cast<chrono::seconds>(chrono::system_clock::now()-start).count() << endl;
	start = chrono::system_clock::now();
	unsigned long long countTest = knightTest.count();
	cout << "Count Test: " << countPerf << endl;
	cout << "Took Test (secs): " << chrono::duration_cast<chrono::seconds>(chrono::system_clock::now()-start).count() << endl;
	//for(const string& s : result)
	//{
	//	cout << s << "\n";
	//}
}







