

#include <iostream>
#include <memory>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <cassert>

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
	KnightSequenceGenerator(int sequenceLen, uint maxVowelCount) : _maxVowelCount(maxVowelCount), _seqLen(sequenceLen), _adjMatrix(127, make_pair('1', 'O'))
	{
		_adjMatrix.addConnection('A', 'L');
		_adjMatrix.addConnection('A', 'H');
		_adjMatrix.addConnection('B', 'K');
		_adjMatrix.addConnection('B', 'M');
		_adjMatrix.addConnection('B', 'I');
		_adjMatrix.addConnection('C', 'F');
		_adjMatrix.addConnection('C', 'L');
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


	}
	~KnightSequenceGenerator() {}

	unordered_set<string> generate()
	{
		for(char c : _chars)
		{
			cout << "Starting from char: " << c << endl;
			assert(_seq.size()==0);

			_generate(c, 0);
		}

		return _sequenceSet;
	}

private:
	void _generate(char currChar, int depth)
	{
		cout << depth << endl;
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

		vector<char> nbs = _adjMatrix.neighbors(currChar);
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
	const vector<char> _chars{'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', '1', '2', '3'};
	string _seq;
	uint   _vowelCnt{0};
	const uint   _maxVowelCount;
	int _seqLen;
	AdjancencyMatrix<char> _adjMatrix;
	unordered_set<string> _sequenceSet;
};


int main(int argc, char** argv)
{
	KnightSequenceGenerator knight(8, 2);
	unordered_set<string> result = knight.generate();
	for(const string& s : result)
	{
		cout << s << "\n";
	}
}







