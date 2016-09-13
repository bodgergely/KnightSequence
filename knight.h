#ifndef _KNIGHT_H_
#define _KNIGHT_H_

#include <iostream>
#include <memory>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <cassert>
#include <chrono>
#include <thread>


class KnightSequenceGenerator
{
public:
	KnightSequenceGenerator(char start, int sequenceLen, uint maxVowelCount) : _startChar(start), _maxVowelCount(maxVowelCount), _seqLen(sequenceLen), _count(0)
	{
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
		return _countpriv();
	}


	void generateThreaded()
	{
		_thread = std::thread(&KnightSequenceGenerator::_generate, this, _startChar, 0);
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
		if(is_vowel(currChar))
			++_vowelCnt;

		if(depth+1 == _seqLen)
		{
			++_count;
			popBack(currChar);
			return;
		}

		const std::vector<char>& nbs = _neighborMap[currChar];

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


	inline void popBack(char ch)
	{
		if(is_vowel(ch))
			--_vowelCnt;
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
	std::unordered_map<char, std::vector<char> > _neighborMap;
	uint   _vowelCnt{0};
	const uint   _maxVowelCount;
	int _seqLen;
	unsigned long long	   _count;
	std::thread _thread;
};


class KnightSequenceGeneratorTester : public KnightSequenceGenerator
{
public:
	KnightSequenceGeneratorTester(char start, int sequenceLen, uint maxVowelCount) : KnightSequenceGenerator(start, sequenceLen, maxVowelCount) {}
	virtual ~KnightSequenceGeneratorTester() {}

	std::unordered_set<std::string> getSequences() {return _sequenceSet;}

protected:
	virtual unsigned long long _countpriv() {return _sequenceSet.size();}
	virtual void _generate(char currChar, int depth)
	{
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

		const std::vector<char>& nbs = _neighborMap[currChar];
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
	std::string _seq;
	std::unordered_set<std::string> _sequenceSet;
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
	const std::vector<char> _chars{'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', '1', '2', '3'};
	std::vector<KnightSequenceGeneratorPtr> _generators;

};


#endif
