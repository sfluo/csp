
#include <vector>

typedef std::vector<std::string>	WordList;

class CwordSolver {
public:
	CwordSolver(std::string words[], int num, int c1[], int m, int c2[], int n);
	~CwordSolver();
	
	void local_search();
	void print_state();
	
private:
	int get_fscore();
	int get_fscore(int k);

	std::vector<int> m_var;
	std::vector<int> m_cst1; // unary constrait on each variable
	std::vector<std::vector<int> > m_cst2; // cross constrait for each variable (j, k, m)
	std::vector<std::vector<int> > m_doms; // domains (indices in m_words) 
	std::vector<std::string> m_words;

	int m_Nvar;
	int m_Ncst;
};

