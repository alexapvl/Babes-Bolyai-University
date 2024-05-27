#pragma once
#include "../service/service.h"
#include <QApplication>
#include <QLabel>
#include <QLayout>
#include <QLineEdit>
#include <QListWidget>
#include <QMessageBox>
#include <QPushButton>
#include <QScreen>
#include <QStackedWidget>
#include <QTableWidget>
#include <QWidget>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <string>

using namespace std;

class GUI : public QWidget {
private:
  Service& service;

  // Main Layout
  QVBoxLayout* mainLayout;
  QTabWidget* mainTabWidget;

  // Admin Tab Elements
  QWidget* adminTab;
  QHBoxLayout* adminLayout;
  QVBoxLayout *adminLeftLayout, *adminRightLayout;
  QLabel *adminLeftLabel, *adminRightLabel;
  QGridLayout *adminFormLayout, *adminButtonsLayout;
  QListWidget *moviesListWidget, *titleEdit, *genreEdit, *yearEdit, *likesEdit, *trailerEdit;
  QPushButton *addButton, *deleteButton, *updateButton;
  QLabel *adminTitleLabel, *adminGenreLabel, *adminYearLabel, *adminLikesLabel, *adminTrailerLabel;
  QLineEdit *adminTitleEdit, *adminGenreEdit, *adminYearEdit, *adminLikesEdit, *adminTrailerEdit;

  // User Tab Elements
  QWidget* userTab;
  QHBoxLayout* userLayout;
  QVBoxLayout *userLeftLayout, *userRightLayout;
  QLabel *userLeftLabel, *userRightLabel;
  QGridLayout *userGenreFilterLayout, *userCurrentMovieLayout, *userButtonsLayout;
  QLabel* userGenreFilterLabel;
  QLineEdit* userGenreFilterEdit;
  QPushButton* userGenreFilterButton;
  QListWidget* userWatchListWidget;
  QPushButton *userOpenCSVFileButton, *userOpenHTMLFileButton;
  QGridLayout* userLeftButtonsGrid;
  QLabel *userCurrentMovieTitle, *userCurrentMovieGenre, *userCurrentMovieYear, *userCurrentMovieLikes;
  QLineEdit *userCurrentMovieTitleEdit, *userCurrentMovieGenreEdit, *userCurrentMovieYearEdit, *userCurrentMovieLikesEdit;
  QPushButton *addWatchListButton, *rateMovieWatchListButton, *openTrailerWatchListButton, *nextWatchListButton;
  int currentIndexInFilteredList;
  vector<int> filteredMovies;

  // Chart Tab Elements
  QWidget* chartTab;
  QChart* chart;
  QChartView* chartView;
  QVBoxLayout* chartLayout;
  QPieSeries* chartSeries;
  QList<QColor> colors = {Qt::red, Qt::green, Qt::blue, Qt::yellow, Qt::cyan, Qt::magenta, Qt::gray, Qt::darkRed, Qt::darkGreen, Qt::darkBlue, Qt::darkYellow, Qt::darkCyan, Qt::darkMagenta};

  // GUI Methods
  void buildGUI();
  void populateMovies();
  void populateWatchList();
  void openCurrentMovieTrailerFromLineEdit();
  void populateUserLineEdits();
  void populateChart();
  void changeTabHandler();

  // Admin Tab Handlers
  void adminItemSelectedHandler();
  void adminAddButtonHandler();
  void adminDeleteButtonHandler();
  void adminUpdateButtonHandler();

  // User Tab Handlers
  void userFilterButtonHandler();
  void userAddWatchListButtonHandler();
  void userRateMovieWatchListButtonHandler();
  void userOpenTrailerButtonHandler();
  void userNextMovieFromFilteredListHandler();
  void userOpenCSVFileButtonHandler();
  void userOpenHTMLFileButtonHandler();

public:
  GUI(Service& service);
};