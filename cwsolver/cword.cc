
#include <time.h>
#include <list>
#include <vector>
#include <iostream>
#include <exception>
#include <cstdlib>
#include <random>
#include "cword.h"

CwordSolver::CwordSolver(std::string words[], int num,
	int c1[], int m, int c2[], int n)
{
	if ( m <= 0 || n <= 0 || m % 2 != 0 || n % 4 != 0)
	{
		std::cout << "Invalid length: " << num << " " << m << " " << n << std::endl;
		exit(0);
	}
		
	m_Nvar = m/2;
	m_Ncst = n/4;

	m_var.resize(m_Nvar, int());
	m_cst1.resize(m_Nvar, int());
	m_cst2.resize(m_Nvar, std::vector<int>());

	int max_idx = -1;
	int max_len = -1;
	for (int i = 0; i < m; i+=2)
	{
		printf("Add constraints (%d, %d)\n", c1[i]+1, c1[i+1]);
		// the index of variable: c1[i]
		// the length of this variable (word): c1[i+1]
		int idx = c1[i];
		int len = c1[i+1];

		if (idx >= 0 && idx < m_Nvar && len > 0) 
		{
			m_cst1[idx] = len;

			if (idx > max_idx)
				max_idx = idx; 
			if (len > max_len)
				max_len = len;
		} else {
			std::cout << "invalid input:" << idx << "-" << len << std::endl;
			exit(0);
		}
	}
	if (max_idx + 1 != m_Nvar) //sanity check
	{
		std::cout << "Incorrect indexing varibles.\n" << std::endl;
		exit(0);
	}

	// check constraints: (r, s, t, p)
	// r-th word (t-th letter) cross s-th word (p-th letter)
	for (int i = 0; i < n; i+=4)
	{
		// (r, s, t, p)
		int r = c2[i];
		int s = c2[i+1];
		int t = c2[i+2];
		int p = c2[i+3];

		printf("Add constraints (%d, %d, %d, %d)\n", r+1, s+1, t+1, p+1);
		if (r >= 0 && r < m_Nvar && s >= 0 && s < m_Nvar
			&& t >= 0 && t < m_cst1[r] && p >= 0 && p < m_cst1[r]) 
		{
			//(r, s, t, p) -> (s, t, p)
			m_cst2[r].push_back(s);
			m_cst2[r].push_back(t);
			m_cst2[r].push_back(p);

			//(r, s, t, p) -> (r, p, t)
			m_cst2[s].push_back(r);
			m_cst2[s].push_back(p);
			m_cst2[s].push_back(t);

		} else {
			printf("Invalid constraints (%d, %d, %d, %d)\n", r, s, t, p);
			exit(0);
		}
	}

	// a vector of word lists: each list has the same length of words
	std::vector<std::vector<int> > w_groups(max_len + 1);

	// group words by length
	for (int i = 0, k = 0; i < num; i++)
	{
		int len = words[i].length();
		if (len > max_len)
			continue;
		m_words.push_back(words[i]);
		w_groups[len].push_back(k++);
	}
	// domains for each variable
	for (int i = 0; i < m_Nvar; i++)
	{
		int sz = w_groups[m_cst1[i]].size();
		m_doms.push_back(w_groups[m_cst1[i]]);
		if (sz == 0)
		{
			std::cout << "This puzzle won't have a solution. variable " 
				<< i+1 << " requires a word of length " << m_cst1[i]
				<< ", which does not exist in the candidate list. " << std::endl;
			exit(0);
		}
	}
	
	std::cout << m_Nvar << " Variables, " << m_Ncst << " Constraints." << std::endl;
}

CwordSolver::~CwordSolver()
{
}

int
CwordSolver::get_fscore(int idx)
{
	int fscore = 0;
	int i = idx;
	
	std::vector<int>::iterator mit;
	for (mit = m_cst2[i].begin(); mit != m_cst2[i].end(); )
	{
		int s = *mit++;
		int t = *mit++;
		int p = *mit++;

		//printf("\n%d, %d, %d,%d, %s, %s, ", i, s, t, p,
		//	m_words[m_var[i]].c_str(), m_words[m_var[s]].c_str());
		if (m_words[m_var[i]].at(t) == m_words[m_var[s]].at(p))
		{
			fscore++;	
		}
	}
	return fscore;
}
int
CwordSolver::get_fscore()
{
	int fscore = 0;
	
	//print_state();
	for (int i = 0; i < m_Nvar; i++)
	{
		fscore += get_fscore(i);
	}
	return fscore/2;
}

void
CwordSolver::local_search()
{
	int repeat = 0;
	int fails = 0;
	int seed = time(NULL);

	do 
	{
		srand(seed);

		for (int i = 0; i < m_Nvar; i++)
		{
			//std::cout << " size=" << m_doms[i].size() << std::endl;
			int v = rand() % m_doms[i].size();
			//std::cout << v << std::endl;
			m_var[i] = m_doms[i].at(v);
		}
		seed = rand();
		std::cout << "[Guess]: ";
		print_state();

		int steps = 0;

		int fscore = get_fscore();
		if (fscore == m_Ncst)
		{
			std::cout << "Solution found (steps: " << steps << ")." << std::endl;
			print_state();
			return;
		}
		
		do
		{
			srand(time(NULL));

			bool local_optima = true;
			int start = rand() % m_Nvar;

			for (int i = 0; i < m_Nvar; i++)
			{
				int k = (i + start) % m_Nvar;

				std::vector<int>::iterator mit;
				for (mit = m_doms[k].begin(); mit != m_doms[k].end(); mit++)
				{
					if (m_var[k] == *mit)
						continue;

					int prev_score = get_fscore(k);	

					int w = m_var[k];
					m_var[k] = *mit;

					int new_score = get_fscore(k);

					if (new_score <= prev_score)
					{
						//std::cout << "No Better! (" << m_words[w] 
						//	<< " -> " << m_words[m_var[k]] << ")" << std::endl;
						m_var[k] = w; //restore
					} else {
						std::cout << "Better >> ";
						print_state();
						steps++;	
						fscore += (new_score - prev_score);
						if (fscore == m_Ncst)
						{
							std::cout << "Solution found (steps: " << steps << ")." << std::endl;
							print_state();
							return;
						}
						local_optima = false;
						break;
					}
				}
			} // for-var

		} while (!local_optima)

		fails++;
		std::cout << "Search Failed. Trial No.=" << fails << std::endl;
		std::cout << "----------------------------------------------------------------\n";
	} while (repeat++ < 1000);
}

void
CwordSolver::print_state()
{
	std::cout << "State:"; 
	for (int i = 0; i < m_Nvar; i++)
	{
		std::cout << " " << i+1 << ":" << m_words[m_var[i]] << " ";
	}
	std::cout << std::endl;
}

