#include "domain.h"
#include <fstream>
#include <sstream>

using namespace std;

class Repo {
private:
  vector<Document> data;

public:
  Repo() {
    loadFromFile();
  }
  ~Repo() {}

  void loadFromFile() {
    ifstream file("../documents.txt");
    string line;
    while (getline(file, line)) {
      stringstream ss(line);
      string name, keys, content;
      getline(ss, name, '|');
      getline(ss, keys, '|');
      getline(ss, content);
      data.push_back(Document(name, keys, content));
    }
  }

  vector<Document> getSortedDocuments() {
    vector<Document> result = data;
    sort(result.begin(), result.end(), [](Document d1, Document d2) { return d1.getName() < d2.getName(); });
    return result;
  }
};