#include "vector_space_model.h"

int main(int argc, char *argv[])
{
  // a list of documents collection
  vector<string> docs(collection, collection+3);
  // array of maps mapping each term in document
  // to its frequency.
  map<string, int> term2freq[docs.size()];
  vector_space documents[docs.size()];
  map<string, int> term2docFreq;
  vector_space query;

  for (int i = 0; i < docs.size(); i++) {
    // build term-freq pair for computing tf(term frequency)
    build_term2freq(term2freq[i], docs[i]);
    documents[i].term2freq = &term2freq[i];
    documents[i].name = docs[i];
    documents[i].type = T_DOC;
    documents[i].max_term_freq = compute_max_term_freq(term2freq[i]);
  }

  // get whole term list for all documents and map each
  // term to the number of documents containing it.
  build_term2docFreq(term2freq, term2docFreq, docs.size());

  for (int i = 0; i < docs.size(); i++) {
    compute_term_weight(documents[i], term2docFreq, docs.size());
    compute_vector_length(documents[i]);
  }

  process_query(query, documents, term2docFreq, docs.size());
}

void compute_vector_length(vector_space& vs)
{
  double square_sum = 0.0;
  for (int i = 0; i < vs.weights.size(); ++i)
    square_sum += pow(vs.weights[i], 2);
  vs.vec_length = sqrt(square_sum);
#ifdef DEBUG
  cout << vs.vec_length << endl;
#endif
}

void process_query(vector_space& qvec, vector_space *docs,
		   map<string, int>& t2df,
		   int doc_num)
{
  string query;
  string term;
  vector<string> query_terms;
  cout << "\nEnter a query: ";
  getline(cin, query);
  stringstream qstream(query);
  while (qstream >> term) {
    term = normalize_word(term);
    query_terms.push_back(term);
  }
#ifdef DEBUG
  for (int i = 0; i < query_terms.size(); ++i)
    cout << query_terms[i] << endl;
#endif

  qvec.name = "QUERY";
  qvec.type = T_QUERY;
  qvec.term2freq = new map<string, int>;
  for (int i = 0; i < query_terms.size(); ++i)
    (*qvec.term2freq)[query_terms[i]] = 1;

  compute_term_weight(qvec, t2df, doc_num);
  compute_vector_length(qvec);

  map<int, pair<string, double> > rank2sim;
  compute_similarity_and_rank(qvec, docs, doc_num, rank2sim);
  show_results(docs, qvec, t2df,
	       doc_num, rank2sim);
}

void compute_similarity_and_rank(vector_space& qvec,
				 vector_space *docs,
				 int doc_num,
				 map<int, pair<string, double> >& r2s)
{
  map<double, string> similarity;
  for (int i = 0; i < doc_num; ++i) {
    double doc_product = 0.0;
    double sim;
    for (int j = 0; j < qvec.weights.size(); ++j) {
      doc_product += docs[i].weights[j] * qvec.weights[j];
    }
    sim = doc_product / (docs[i].vec_length * qvec.vec_length);
    similarity[sim] = docs[i].name;
  }

  int rank = 1;
  for (map<double, string>::reverse_iterator it = similarity.rbegin();
       it != similarity.rend(); ++it) {
    r2s[rank++] = make_pair(it->second, it->first);
  }

#ifdef DEBUG
  for(map<int, pair<string, double> >::iterator it = r2s.begin();
      it != r2s.end(); ++it) {
    cout << it->first << ':' << (it->second).first
	 << ':' << (it->second).second << endl;
  }
#endif
}

void show_results(vector_space *vs, vector_space& query,
		  map<string, int>& t2df, int num,
		  map<int, pair<string, double> >& r2s)
{
  cout << endl;
  cout << setfill('*') << setw(20) << "" << setfill(' ') << endl;
  cout << "TERM WEIGHTS:" << endl;
  cout << setfill('*') << setw(20) << "" << setfill(' ') << endl;
  cout << endl;
  cout << setw(7) << "TERM" << '|';
  for (int i = 0; i < num; ++i) {
    cout << setw(8) << vs[i].name << '|';
  }
  cout << setw(8) << query.name << '|';
  cout << endl;
  cout << setfill('-') << setw(44) << "" << setfill(' ');
  cout << endl;
  int count = 0;
  for (map<string, int>::iterator it = t2df.begin();
       it != t2df.end(); ++it) {
    cout << setw(7) << it->first << '|';
    for (int i = 0; i < num; ++i) {
      cout << setprecision(3) <<
	setw(8) << vs[i].weights[count] << '|';
    }
    cout << setprecision(3) <<
      setw(8) << query.weights[count++] << '|';
    cout << endl;
  }
  cout << setfill('-') << setw(44) << "" << setfill(' '); 
  cout << endl;
  cout << setw(7) << "LENGTH" << '|';
  for (int i = 0; i < num; ++i) {
    cout << setprecision(3) << setw(8)
	 << vs[i].vec_length << '|';
  }
  cout << setprecision(3) << setw(8)
       << query.vec_length << '|' << endl;

  cout << endl;
  cout << setfill('*') << setw(50) << "" << setfill(' ') << endl;
  cout << "SIMILARITY and RANK(Best Match to Worst Match):" << endl;
  cout << setfill('*') << setw(50) << "" << setfill(' ') << endl;
  cout << endl;
  
  cout << setw(4) << "RANK" << '|';
  cout << setw(8) << "DOC" << '|';
  cout << setw(12) << "SIMILARITY" << '|' << endl;
  cout << setfill('-') << setw(27) << "" << setfill(' ');
  cout << endl;
  for(map<int, pair<string, double> >::iterator it = r2s.begin();
      it != r2s.end(); ++it) {
    cout << setw(4) << it->first << '|';
    cout << setw(8) << (it->second).first << '|';
    cout << setprecision(3) << setw(12) << (it->second).second << '|' << endl;
  }

  cout << endl;
  cout << setfill('*') << setw(22) << "" << setfill(' ') << endl;
  cout << "Best match is " << (r2s.begin()->second).first << ':' << endl;
  cout << setfill('*') << setw(22) << "" << setfill(' ') << endl;
  
  ifstream best((r2s.begin()->second).first.c_str());
  string content;
  while (getline(best, content))
    cout << content;
  cout << endl;

  cout << endl;
  cout << setfill('*') << setw(22) << "" << setfill(' ') << endl;
  cout << "Worst match is " << (r2s.rbegin()->second).first << ':' << endl;  
  cout << setfill('*') << setw(22) << "" << setfill(' ') << endl;

  ifstream worst((r2s.rbegin()->second).first.c_str());
  while (getline(worst, content))
    cout << content;
  cout << endl;

}

void compute_term_freq(vector<double>& tf,
		       vector_space& vs,
		       map<string, int>& t2df)
{
  map<string, int>& t2f = *(vs.term2freq);
  for (map<string, int>::iterator it1 = t2df.begin();
       it1 != t2df.end(); ++it1) {
    map<string, int>::iterator it2 = t2f.find(it1->first);
    if (it2 != t2f.end()) { // found
      if (vs.type == T_DOC) { // document
	tf.push_back(static_cast<double> (it2->second) /
		     vs.max_term_freq);
      } else { // query, tf is always 1.
	tf.push_back(1);
      }
    } else {
      tf.push_back(0);
    }
  }
#ifdef DEBUG
  for (int i=0; i < tf.size(); ++i) {
    cout << tf[i] << ", ";
  }
  cout << endl;
#endif
}

void compute_term_weight(vector_space& vs,
			 map<string, int>& t2df, int num_docs)
{
  vector<double> tf;
  compute_term_freq(tf, vs, t2df);

  int count = 0;
  double weight;
  for (map<string, int>::iterator it = t2df.begin();
       it != t2df.end(); ++it) {
    weight = tf[count++] *
      log2(static_cast<double> (num_docs)/it->second);
#ifdef DEBUG
    cout << it->first << ':' << it->second << ':'
	 << weight<< endl;
#endif
    vs.weights.push_back(weight);
  }
#ifdef DEBUG
  for (int i = 0; i < vs.weights.size(); ++i)
    cout << vs.weights[i] << ", ";
  cout << endl;
#endif    
}

/* map each term in the 't2f' to the number of document
   contining it, store term-docFreq pairs in 't2df'.
   'doc_num' is the number of documents, that is, number
   of elements in array 't2f'.*/
static void build_term2docFreq(map<string, int>* t2f,
			       map<string, int>& t2df,
			       int doc_num)
{
  map<string, int>::iterator it2;
  
  for (int i = 0; i < doc_num; ++i) {
    map<string, int>& termFreq = t2f[i];
    map<string, int>::iterator it1 = termFreq.begin();
    for ( ; it1 != termFreq.end(); ++it1) {
      it2 = t2df.find(it1->first);
      if (it2 == t2df.end()) // not found
	t2df[it1->first] = 1;
      else
	it2->second++;
    }
  }
  
#ifdef DEBUG
  for (it2 = t2df.begin(); it2 != t2df.end(); ++it2) {
    cout << "{" << it2->first << ":" << it2->second << "},";
  }
  cout << endl;
#endif   
}

static int compute_max_term_freq(map<string, int>& t2f)
{
  int max = 1;
  map<string, int>::iterator it;
  for (it = t2f.begin(); it != t2f.end(); ++it) {
    if (it->second > max)
      max = it->second;
  }
  return max;
}

/* build terms and frequency pairs for document 'doc' */
static void build_term2freq(map<string, int>& t2f,
			    string doc)
{
  ifstream input(doc.c_str());
  assert(input || !!!"file open failed!");

  map<string, int>::iterator it;
  string raw_word;
  while (input >> raw_word) {
    string word = normalize_word(raw_word);
    it = t2f.find(word);
    if (it != t2f.end()) // element found
      it->second += 1;
    else {// not found
      t2f[word] = 1;
    }
  }

#ifdef DEBUG
  cout << doc << endl;
  for (it=t2f.begin(); it!=t2f.end(); it++) {
    cout << "{" << it->first << ":" << it->second << "},";
  }
  cout << endl;
#endif
}

/* Normalize a raw word into a pure word with lowercase. */
static string normalize_word(string raw_word)
{
  for (int i = 0; i < raw_word.length(); i++) {
    if (!isalpha(raw_word[i]))
      raw_word.erase(i, 1);
    else
      raw_word[i] = tolower(raw_word[i]);
  }
  return raw_word;
}
