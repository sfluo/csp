
#include <iostream>
#include <sstream>
#include <vector>
#include "cword.h"

void
test()
{
	std::string words[] = {"book", "lane", "buys", "year", "hold", "rock",
		"symbol", "ginger", "syntax", "search",
		"ant", "big", "bus", "car", "has", "beast"};
	int num_of_words = 15;

	int c1[] = {1,4,2,6,3,4,4,3,5,3,6,4};
	int c2[] = {1,3,3,1,1,5,1,1,2,3,3,3,2,4,5,1,2,5,1,3,4,6,3,1};
	int m = 10;
	int n = 20;

	// convert human-index (start from 1) to computer-index (start from 0)
	for (int i = 0; i < sizeof(c1)/sizeof(int); i+=2)
	{
		c1[i] -= 1;
	}
	for (int i = 0; i < sizeof(c2)/sizeof(int); i++)
	{
		c2[i] -= 1;
	}

	CwordSolver *csp = new CwordSolver(words, 15, c1, m, c2, n);
	csp->local_search();
}

int main(int argc, char **argv)
{
	//test();

	std::vector<std::string> words;
	std::string input, word;
	std::cout << "----------------------------------------------------------------\n";
	std::cout << "Welcome to Crossword Puzzle Solver. - Shoufu Luo, Mar. 1, 2015. \n";
	std::cout << "----------------------------------------------------------------\n";
	std::cout << "Input all candidate words (separated by <space> or ,): " << std::endl;
	std::getline(std::cin, input);
	int w = 0;

	char delim[] = ", ";
	char *token = strtok(const_cast<char *>(input.c_str()), delim);
	while(token != NULL)
	{
		words.push_back(std::string(token));	
		token = strtok(NULL, delim);
		w++;
	}

	std::vector<int> lens;
	std::cout << "----------------------------------------------------------------\n";
	std::cout << "Index each slot with a number (starting form 1)\n" 
		"and input index with their length in pair (with 0 0 to stop).\n"
		"e.g. 1 4 (Enter)" " -> word 1 requires 4 letters" << std::endl;
	int i, j, k = 0;
	do {
		std::cin >> i >> j;
		if (i != 0) {
			lens.push_back(i-1);
			lens.push_back(j);
			k++;
		}
	} while (i != 0);

	std::vector<int> csts;
	std::cout << "----------------------------------------------------------------\n";
	std::cout << "Input each cross in the form i, j, k, m, which means\n"
		"for example, if slot 1 (across) and slot 3 (down) has its 4th letter\n"
		"across 3-down's 2nd letter, then input 1 3 4 2 (Enter). (Use 0 0 0 0 to finish).\n";
	int r, s, t, p, m = 0;
	do {
		std::cin >> r >> s >> t >> p;
		if (r != 0)
		{
			csts.push_back(r-1);
			csts.push_back(s-1);
			csts.push_back(t-1);
			csts.push_back(p-1);
			m++;
		}
	} while (r != 0);

	std::cout << "----------------------------------------------------------------\n";
	//std::cout << "w=" << w << ", k=" << k << ", m=" << m << std::endl;
	//std::cout << words[0] << " " << words[1] << std::endl;
	CwordSolver *csp = new CwordSolver(&words[0], w, &lens[0], k * 2, &csts[0], m * 4);
	csp->local_search();
	return 0;
}
