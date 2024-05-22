#include "../include/GUI.h"
#include <string>

GUI::GUI(Service& serv, QWidget* parent)
    : QWidget(parent), service{serv} {
  ui.setupUi(this);
  this->populateList();
  connect(ui.addButton, &QPushButton::clicked, this, &GUI::addButtonHandler);
  connect(ui.allSongsListWidget, &QListWidget::itemClicked, this, &GUI::itemChangedHandler);
  connect(ui.deleteButton, &QPushButton::clicked, this, &GUI::deleteButtonHandler);
  connect(ui.filterEdit, &QLineEdit::textChanged, this, &GUI::filterLineEditHandler);
}

GUI::~GUI() {}

void GUI::populateList() {
  this->ui.allSongsListWidget->clear();

  for (Song s : this->service.getAllSongs()) {
    this->ui.allSongsListWidget->addItem(QString::fromStdString(
        s.getArtist() + " - " + s.getTitle()));
  }
}
void GUI::addButtonHandler() {
  QString artist = ui.artistEdit->text();
  QString title = ui.titleEdit->text();
  this->service.addSongToRepository(title.toStdString(), artist.toStdString(), 4, 3, "www.a.com");
  this->populateList();
}

void GUI::itemChangedHandler() {
  int row = this->ui.allSongsListWidget->currentRow();
  Song clickedSong = this->service.getAllSongs()[row];
  this->ui.titleEdit->setText(QString::fromStdString(clickedSong.getTitle()));
  this->ui.artistEdit->setText(QString::fromStdString(clickedSong.getArtist()));
  this->ui.linkEdit->setText(QString::fromStdString(clickedSong.getSource()));
}

void GUI::deleteButtonHandler() {
  int row = this->ui.allSongsListWidget->currentRow();
  Song clickedSong = this->service.getAllSongs()[row];
  this->service.removeSongFromRepository(clickedSong.getArtist(), clickedSong.getTitle());
  this->populateList();
}

void GUI::filterLineEditHandler() {
  this->ui.allSongsListWidget->clear();
  std::string contents = this->ui.filterEdit->text().toStdString();
  for (auto song : this->service.getAllSongs()) {
    if (song.getArtist().find(contents) != std::string::npos || song.getTitle().find(contents) != std::string::npos)
      this->ui.allSongsListWidget->addItem(QString::fromStdString(
          song.getArtist() + " - " + song.getTitle()));
  }
}