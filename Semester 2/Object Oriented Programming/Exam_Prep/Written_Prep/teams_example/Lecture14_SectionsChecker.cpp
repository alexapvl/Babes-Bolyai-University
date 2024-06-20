#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

class Section;

class Checker {
public:
  virtual bool check(Section* s) = 0;
  virtual ~Checker() = default;
};

class Section {
protected:
  string title, content;
  Checker* checker;

public:
  Section(string t, string c, Checker* ch) : title{t}, content{c}, checker{ch} {}
  virtual void addSection(Section* s) = 0;
  virtual void generate() {
    if (checker == nullptr)
      return;
    if (checker->check(this))
      cout << title << "\n"
           << content;
  }

  string getTitle() { return title; }
  string getContent() { return content; }
  virtual ~Section() = default;
};

class Preface : public Section {
public:
  Preface(string t, string c, Checker* ch) : Section{t, c, ch} {}

  void addSection(Section*) override {}
};

class Chapter : public Section {
private:
  vector<Section*> sections;

public:
  Chapter(string t, string c, Checker* ch) : Section{t, c, ch} {}
  void addSection(Section* s) { sections.push_back(s); }

  void generate() {
    Section::generate();
    for (Section* s : sections)
      s->generate();
  }
};

class TitleChecker : public Checker {
  bool check(Section* s) {
    if (s == nullptr)
      return false;
    return s->getTitle().size() > 2;
  }
};

class ContentChecker : public Checker {
  bool check(Section* s) {
    if (s == nullptr)
      return false;
    stringstream stream{s->getContent()};
    string line;
    while (getline(stream, line, '.')) {
      if (!isupper(line[0]))
        return false;
    }

    return true;
  }
};

class DoubleChecker : public Checker {
private:
  Checker *ch1, *ch2;

public:
  DoubleChecker(Checker* c1, Checker* c2) : ch1{c1}, ch2{c2} {}

  bool check(Section* s) {
    if (ch1 != nullptr && ch2 != nullptr)
      return (ch1->check(s) && ch2->check(s));
    return false;
  }
};

int main() {
  {
    ContentChecker contentChecker{};
    unique_ptr<Section> preface = make_unique<Preface>("Preface", "Preface content\n", &contentChecker);

    TitleChecker titleChecker{};
    DoubleChecker titleAndContent{&contentChecker, &titleChecker};
    unique_ptr<int> x = make_unique<int>(5);
    unique_ptr<Section> chapter = make_unique<Chapter>("Chapter 1", "First.Chapter\n", &titleChecker);
    unique_ptr<Section> subchapter1 = make_unique<Chapter>("Subchapter 1.1", "First.Subchapter", &titleAndContent);
    unique_ptr<Section> subchapter2 = make_unique<Chapter>("Subchapter 1.2", "Second.subchapter", &titleAndContent);

    chapter->addSection(subchapter1.get());
    chapter->addSection(subchapter2.get());

    unique_ptr<Section> book = make_unique<Chapter>("Book", "MyBook", nullptr);
    book->addSection(preface.get());
    book->addSection(chapter.get());
    book->generate();
  }

  return 0;
}