#include "service.h"
#include <QLineEdit>
#include <QListWidget>
#include <QListWidgetItem>
#include <QMessageBox>
#include <QObject>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

class GUI : public QWidget {
private:
  Service serv;
  QVBoxLayout* mainLayout;
  QListWidget* listWidget;
  QLineEdit* docSearch;
  QPushButton* searchButton;

public:
  GUI(Service serv) : serv(serv) {
    buildGUI();
    populateList();

    QObject::connect(docSearch, &QLineEdit::textChanged, this, &GUI::searchHandler);
    QObject::connect(searchButton, &QPushButton::clicked, this, &GUI::showBestMatchingHandler);
  }
  ~GUI() {}

  void buildGUI() {
    mainLayout = new QVBoxLayout{this};

    listWidget = new QListWidget();
    mainLayout->addWidget(listWidget);

    docSearch = new QLineEdit();
    mainLayout->addWidget(docSearch);

    searchButton = new QPushButton();
    searchButton->setText("Show best matching");
    mainLayout->addWidget(searchButton);
  }

  void populateList(string text = "") {
    listWidget->clear();
    for (auto d : serv.getSortedDocuments()) {
      QListWidgetItem* item = new QListWidgetItem();
      item->setText(QString::fromStdString(d.toStr()));
      if (d.toStr().find(text) != string::npos || text == "")
        listWidget->addItem(item);
    }
  }

  void searchHandler() {
    string text = docSearch->text().toStdString();
    populateList(text);
  }

  void showBestMatchingHandler() {
    string text = docSearch->text().toStdString();
    Document d = serv.bestMatch(text);
    string s = d.toStr() + "\nContents: " + d.getContent();
    QMessageBox::about(this, "Best match", QString::fromStdString(s));
  }
};