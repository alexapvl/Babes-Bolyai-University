#include "repo.h"

class Service {
private:
  Repo repo;

public:
  Service(Repo repo) : repo(repo) {}
  ~Service() {}

  vector<Document> getSortedDocuments() {
    return repo.getSortedDocuments();
  }

  Document bestMatch(string text) {
    Document result;
    double best = 0.0;
    for (auto d : getSortedDocuments()) {
      if (d.getName().find(text) != string::npos) {
        double temp = double(text.size()) / double(d.getName().size());
        if (temp > best) {
          best = temp;
          result = d;
        }
      }
    }
    return result;
  }
};