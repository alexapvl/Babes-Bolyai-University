#include <string>
#include <vector>

using namespace std;

class Document {
private:
  string name, keywords, content;

public:
  Document(string name = "", string keywords = "", string content = "") : name(name), keywords(keywords), content(content) {}
  ~Document() {}

  string getName() { return name; }
  string getKeyworkds() { return keywords; }
  string getContent() { return content; }

  string toStr() {
    return name + ": " + keywords;
  }
};