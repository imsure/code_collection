#ifndef _VECTOR_SPACE_H_
#define _VECTOR_SPACE_H_

#include <iostream>
#include <string>
#include <vector>
#include <cassert>
#include <map>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <sstream>

using namespace std;

/* Structure holding information about
   documents or queries. */
typedef struct {
  map<string, int>* term2freq;
  vector<double> weights; // term weight
  string name;
  string type; // document or query
  int max_term_freq;
  double vec_length; //length of vector
}vector_space;

//#define DEBUG
#define T_DOC "DOC"
#define T_QUERY "QUERY"

static void build_term2freq(map<string, int>& t2c, string doc);
static string normalize_word(string raw_word);
static int compute_max_term_freq(map<string, int>& t2f);
static void build_term2docFreq(map<string, int>* t2f,
			       map<string, int>& t2df,
			       int doc_num);
void compute_term_weight(vector_space& vs,
			 map<string, int>& t2df,
			 int num_docs);
void compute_term_freq(vector<double>& tf,
		       vector_space& vs,
		       map<string, int>& t2df);
void compute_vector_length(vector_space& vs);
void process_query(vector_space& qvec, vector_space *docs,
		   map<string, int>& t2df,
		   int doc_num);
void compute_similarity_and_rank(vector_space& qvec,
				 vector_space *docs,
				 int doc_num,
				 map<int,pair<string,double> >& r2s);
void show_results(vector_space *vs, vector_space& query,
		  map<string, int>& t2df, int num,
		  map<int, pair<string, double> >& r2s);

const char *collection[] = {"D1.txt", "D2.txt", "D3.txt"};

#endif
