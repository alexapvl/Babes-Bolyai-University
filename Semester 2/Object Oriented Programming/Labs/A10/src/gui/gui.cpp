#include "../../include/gui/gui.h"

using namespace std;

GUI::GUI(Service& service) : service(service) {
  buildGUI();
  filteredMovies = vector<int>{};
  changeTabHandler();

  // Admin Tab
  QObject::connect(movieTableView, &QTableView::clicked, this, &GUI::adminItemSelectedHandler);
  QObject::connect(addButton, &QPushButton::clicked, this, &GUI::adminAddButtonHandler);
  QObject::connect(deleteButton, &QPushButton::clicked, this, &GUI::adminDeleteButtonHandler);
  QObject::connect(updateButton, &QPushButton::clicked, this, &GUI::adminUpdateButtonHandler);
  QObject::connect(undoButton, &QPushButton::clicked, this, &GUI::adminUndoButtonHandler);
  QObject::connect(redoButton, &QPushButton::clicked, this, &GUI::adminRedoButtonHandler);
  QObject::connect(undoShortcut, &QShortcut::activated, this, &GUI::adminUndoButtonHandler);
  QObject::connect(redoShortcut, &QShortcut::activated, this, &GUI::adminRedoButtonHandler);

  // User Tab
  QObject::connect(userGenreFilterButton, &QPushButton::clicked, this, &GUI::userFilterButtonHandler);
  QObject::connect(addWatchListButton, &QPushButton::clicked, this, &GUI::userAddWatchListButtonHandler);
  QObject::connect(rateMovieWatchListButton, &QPushButton::clicked, this, &GUI::userRateMovieWatchListButtonHandler);
  QObject::connect(openTrailerWatchListButton, &QPushButton::clicked, this, &GUI::userOpenTrailerButtonHandler);
  QObject::connect(nextWatchListButton, &QPushButton::clicked, this, &GUI::userNextMovieFromFilteredListHandler);
  QObject::connect(userOpenCSVFileButton, &QPushButton::clicked, this, &GUI::userOpenCSVFileButtonHandler);
  QObject::connect(userOpenHTMLFileButton, &QPushButton::clicked, this, &GUI::userOpenHTMLFileButtonHandler);

  // For all tabs
  QObject::connect(mainTabWidget, &QTabWidget::currentChanged, this, &GUI::changeTabHandler);
}

void GUI::buildGUI() {
  mainLayout = new QVBoxLayout{this}; // Main layout for the GUI widget

  // Create the main tab widget
  mainTabWidget = new QTabWidget{};

  // Add the main tab widget to the main layout
  mainLayout->addWidget(mainTabWidget);

  // Set the font for the labels
  QFont font{"Courier New", 15, 5};

  // Set the size for the buttons
  const QSize btnSize = QSize(100, 25);

  // ########### Admin Tab ###########
  adminTab = new QWidget{};

  // Admin Tab Layout
  adminLayout = new QHBoxLayout{adminTab}; // Create the main layout for the admin tab
  adminLeftLayout = new QVBoxLayout{};     // Create an empty layout for the left side
  adminLeftLabel = new QLabel{"Movies List"};
  adminLeftLabel->setFont(font);
  adminLeftLayout->addWidget(adminLeftLabel);
  adminLeftLabel->setAlignment(Qt::AlignCenter | Qt::AlignTop);
  movieTableModel = new MovieTableModel(service, this);
  movieTableView = new QTableView();
  movieTableView->setModel(movieTableModel);
  movieTableView->resizeColumnsToContents();
  adminLeftLayout->addWidget(movieTableView);
  adminLayout->addLayout(adminLeftLayout); // Add the left layout to the admin tab layout

  adminRightLayout = new QVBoxLayout{}; // Create an empty layout for the right side
  adminRightLabel = new QLabel{"Movie Details"};
  adminRightLabel->setFont(font);
  adminRightLabel->setAlignment(Qt::AlignTop | Qt::AlignHCenter);
  adminRightLayout->addWidget(adminRightLabel);
  adminFormLayout = new QGridLayout{}; // Create an empty layout for the form
  adminFormLayout->setAlignment(Qt::AlignTop | Qt::AlignCenter);
  adminButtonsLayout = new QGridLayout{}; // Create an empty layout for the buttons
  adminButtonsLayout->setAlignment(Qt::AlignCenter);
  adminUndoRedoLayout = new QGridLayout{};
  adminUndoRedoLayout->setAlignment(Qt::AlignCenter);

  // Create the labels and input fields for the form
  adminTitleLabel = new QLabel{"Name:"};
  adminTitleEdit = new QLineEdit{};
  adminGenreLabel = new QLabel{"Genre:"};
  adminGenreEdit = new QLineEdit{};
  adminYearLabel = new QLabel{"Year:"};
  adminYearEdit = new QLineEdit{};
  adminLikesLabel = new QLabel{"Likes:"};
  adminLikesEdit = new QLineEdit{};
  adminTrailerLabel = new QLabel{"Trailer:"};
  adminTrailerEdit = new QLineEdit{};

  // Arrange the form widgets in the form layout
  adminFormLayout->addWidget(adminTitleLabel, 0, 0);
  adminFormLayout->addWidget(adminTitleEdit, 0, 1);
  adminFormLayout->addWidget(adminGenreLabel, 1, 0);
  adminFormLayout->addWidget(adminGenreEdit, 1, 1);
  adminFormLayout->addWidget(adminYearLabel, 2, 0);
  adminFormLayout->addWidget(adminYearEdit, 2, 1);
  adminFormLayout->addWidget(adminLikesLabel, 3, 0);
  adminFormLayout->addWidget(adminLikesEdit, 3, 1);
  adminFormLayout->addWidget(adminTrailerLabel, 4, 0);
  adminFormLayout->addWidget(adminTrailerEdit, 4, 1);

  // Add the buttons to the buttons layout
  addButton = new QPushButton{"Add"};
  deleteButton = new QPushButton{"Delete"};
  updateButton = new QPushButton{"Update"};
  adminButtonsLayout->addWidget(addButton, 0, 0);
  adminButtonsLayout->addWidget(deleteButton, 0, 1);
  adminButtonsLayout->addWidget(updateButton, 0, 2);
  addButton->setFixedSize(btnSize);
  deleteButton->setFixedSize(btnSize);
  updateButton->setFixedSize(btnSize);

  // Add the buttons to the undo redo layout
  undoShortcut = new QShortcut{QKeySequence{"Ctrl+Z"}, this};
  redoShortcut = new QShortcut{QKeySequence{"Ctrl+Shift+Z"}, this};
  undoButton = new QPushButton{"Undo"};
  redoButton = new QPushButton{"Redo"};
  adminUndoRedoLayout->addWidget(undoButton, 0, 0);
  adminUndoRedoLayout->addWidget(redoButton, 0, 1);
  undoButton->setFixedSize(btnSize);
  redoButton->setFixedSize(btnSize);

  // Add the form and buttons layouts to the right layout
  adminRightLayout->addLayout(adminFormLayout);
  adminRightLayout->addLayout(adminButtonsLayout);
  adminRightLayout->addLayout(adminUndoRedoLayout);

  // Add the right layout to the admin tab layout
  adminLayout->addLayout(adminRightLayout);

  // Add the admin tab to the main tab widget
  mainTabWidget->addTab(adminTab, "Admin");

  // ########### User Tab Layout ###########
  userTab = new QWidget{};

  // User Tab Layout
  // User Tab Left Layout
  userLayout = new QHBoxLayout{userTab};
  userLeftLayout = new QVBoxLayout{};
  userLeftLabel = new QLabel{"Watch List"};
  userLeftLabel->setFont(font);
  userLeftLayout->addWidget(userLeftLabel);
  userLeftLabel->setAlignment(Qt::AlignCenter | Qt::AlignTop);
  userWatchListWidget = new QListWidget{};
  userLeftLayout->addWidget(userWatchListWidget);
  userLeftButtonsGrid = new QGridLayout{};
  userOpenCSVFileButton = new QPushButton{"Open CSV"};
  userOpenHTMLFileButton = new QPushButton{"Open HTML"};
  userLeftButtonsGrid->addWidget(userOpenCSVFileButton, 0, 0);
  userOpenCSVFileButton->setFixedSize(btnSize);
  userLeftButtonsGrid->addWidget(userOpenHTMLFileButton, 0, 1);
  userOpenHTMLFileButton->setFixedSize(btnSize);
  userLeftLayout->addLayout(userLeftButtonsGrid);
  userLayout->addLayout(userLeftLayout);

  // User Tab Right Layout
  userRightLayout = new QVBoxLayout{};

  // Right Label
  userRightLabel = new QLabel{"Watch List Options"};
  userRightLabel->setFont(font);
  userRightLabel->setAlignment(Qt::AlignTop | Qt::AlignHCenter);
  userRightLayout->addWidget(userRightLabel);

  userGenreFilterLayout = new QGridLayout{};
  userGenreFilterLayout->setAlignment(Qt::AlignTop | Qt::AlignCenter);
  userGenreFilterLabel = new QLabel{"Genre filter"};
  userGenreFilterLabel->setAlignment(Qt::AlignCenter);
  userGenreFilterLabel->setFont(font);
  userGenreFilterLayout->addWidget(userGenreFilterLabel, 0, 0);
  userGenreFilterEdit = new QLineEdit{};
  userGenreFilterEdit->setAlignment(Qt::AlignCenter);
  userGenreFilterEdit->setFixedWidth(250);
  userGenreFilterLayout->addWidget(userGenreFilterEdit, 1, 0);
  userGenreFilterButton = new QPushButton{"Filter movies"};
  userGenreFilterButton->setFixedSize(btnSize);
  userGenreFilterLayout->addWidget(userGenreFilterButton, 2, 0, Qt::AlignCenter);
  userRightLayout->addLayout(userGenreFilterLayout);

  // Current Movie Layout
  userCurrentMovieLayout = new QGridLayout{};
  userCurrentMovieLayout->setAlignment(Qt::AlignTop | Qt::AlignCenter);
  userCurrentMovieTitle = new QLabel{"Title: "};
  userCurrentMovieGenre = new QLabel{"Genre: "};
  userCurrentMovieYear = new QLabel{"Year: "};
  userCurrentMovieLikes = new QLabel{"Likes: "};
  userCurrentMovieLayout->addWidget(userCurrentMovieTitle, 0, 0);
  userCurrentMovieLayout->addWidget(userCurrentMovieGenre, 1, 0);
  userCurrentMovieLayout->addWidget(userCurrentMovieYear, 2, 0);
  userCurrentMovieLayout->addWidget(userCurrentMovieLikes, 3, 0);
  userCurrentMovieTitleEdit = new QLineEdit{};
  userCurrentMovieTitleEdit->setReadOnly(true);
  userCurrentMovieGenreEdit = new QLineEdit{};
  userCurrentMovieGenreEdit->setReadOnly(true);
  userCurrentMovieYearEdit = new QLineEdit{};
  userCurrentMovieYearEdit->setReadOnly(true);
  userCurrentMovieLikesEdit = new QLineEdit{};
  userCurrentMovieLikesEdit->setReadOnly(true);
  userCurrentMovieLayout->addWidget(userCurrentMovieTitleEdit, 0, 1);
  userCurrentMovieLayout->addWidget(userCurrentMovieGenreEdit, 1, 1);
  userCurrentMovieLayout->addWidget(userCurrentMovieYearEdit, 2, 1);
  userCurrentMovieLayout->addWidget(userCurrentMovieLikesEdit, 3, 1);
  userRightLayout->addLayout(userCurrentMovieLayout);

  // User Buttons Layout
  userButtonsLayout = new QGridLayout{};
  userButtonsLayout->setAlignment(Qt::AlignCenter);
  addWatchListButton = new QPushButton{"Add"};
  addWatchListButton->setFixedSize(btnSize);
  rateMovieWatchListButton = new QPushButton{"Rate Movie"};
  rateMovieWatchListButton->setFixedSize(btnSize);
  openTrailerWatchListButton = new QPushButton{"Open Trailer"};
  openTrailerWatchListButton->setFixedSize(btnSize);
  nextWatchListButton = new QPushButton{"Next movie"};
  nextWatchListButton->setFixedSize(btnSize);
  userButtonsLayout->addWidget(addWatchListButton, 0, 0);
  userButtonsLayout->addWidget(rateMovieWatchListButton, 0, 1);
  userButtonsLayout->addWidget(openTrailerWatchListButton, 0, 2);
  userButtonsLayout->addWidget(nextWatchListButton, 0, 3);
  userRightLayout->addLayout(userButtonsLayout);

  // Add the user right layout to the user layout
  userLayout->addLayout(userRightLayout);

  // Add the user tab to the main tab widget
  mainTabWidget->addTab(userTab, "User");

  // Chart Tab
  chartTab = new QWidget{};
  chartLayout = new QVBoxLayout{chartTab};
  chartSeries = new QPieSeries{};
  populateChart();
  chart = new QChart{};
  chart->addSeries(chartSeries);
  chart->legend()->hide();
  chartView = new QChartView{chart};
  chartView->setRenderHint(QPainter::Antialiasing);
  chartLayout->addWidget(chartView);

  // Add the chart to the chart layout
  mainTabWidget->addTab(chartTab, "Chart");

  // Set the initial size of the GUI widget
  resize(1800, 500);
}

void GUI::populateMoviesTable() {
  movieTableModel->setMovies();
}

void GUI::populateWatchList() {
  userWatchListWidget->clear();
  for (auto index : service.getWatchList()) {
    QListWidgetItem* item = new QListWidgetItem{QString::fromStdString(service.getMovieByPosition(index).getTitle() + " | " + service.getMovieByPosition(index).getGenre() + " | " + std::to_string(service.getMovieByPosition(index).getYearOfRelease()) + " | " + std::to_string(service.getMovieByPosition(index).getNumberOfLikes()) + " likes")};
    QFont font{"Courier New", 15, 5};
    item->setFont(font);
    userWatchListWidget->addItem(item);
  }
}

void GUI::openCurrentMovieTrailerFromLineEdit() {
  string title = userCurrentMovieTitleEdit->text().toStdString();
  string genre = userCurrentMovieGenreEdit->text().toStdString();
  int year = userCurrentMovieYearEdit->text().toInt();
  Movie movie = Movie(title, genre, year, 0, "www.youtube.com/trailer-link");
  Movie movieFromRepo = service.getMovieByPosition(service.returnPosition(movieFromRepo));
  string trailer = movieFromRepo.getTrailer();
  string command = "open " + trailer;
  system(command.c_str());
}

void GUI::populateUserLineEdits() {
  Movie currentMovie = service.getMovies()[filteredMovies[currentIndexInFilteredList]];
  userCurrentMovieTitleEdit->setText(QString::fromStdString(currentMovie.getTitle()));
  userCurrentMovieGenreEdit->setText(QString::fromStdString(currentMovie.getGenre()));
  userCurrentMovieYearEdit->setText(QString::fromStdString(to_string(currentMovie.getYearOfRelease())));
  userCurrentMovieLikesEdit->setText(QString::fromStdString(to_string(currentMovie.getNumberOfLikes())));
}

void GUI::populateChart() {
  chartSeries->clear();
  std::vector<Movie> sortedMovies = service.getMoviesSortedByNumberOfLikes();
  for (auto movie : sortedMovies) {
    chartSeries->append(QString::fromStdString(movie.getTitle()), movie.getNumberOfLikes());
  }
  int colorIndex = 0;
  for (auto slice : chartSeries->slices()) {
    slice->setExploded();
    slice->setLabel(QString("%1: %2%").arg(slice->label()).arg(100 * slice->percentage(), 0, 'f', 1));
    slice->setLabelVisible();
    slice->setBrush(colors.at(colorIndex++ % colors.size()));
  }
}

void GUI::changeTabHandler() {
  if (mainTabWidget->currentIndex() == 0 || mainTabWidget->currentIndex() == 1) {
    resize(1800, 500);
  } else { // for chart tab
    populateChart();
    resize(1800, 1000);
  }
  QRect screenGeometry = QGuiApplication::primaryScreen()->geometry();
  int x = (screenGeometry.width() - width()) / 2;
  int y = (screenGeometry.height() - height()) / 2;
  move(x, y);
}

// Admin Tab Handlers

void GUI::adminItemSelectedHandler() {
  int row = movieTableView->currentIndex().row();
  Movie movie = service.getMovieByPosition(row);
  adminTitleEdit->setText(QString::fromStdString(movie.getTitle()));
  adminGenreEdit->setText(QString::fromStdString(movie.getGenre()));
  adminYearEdit->setText(QString::fromStdString(std::to_string(movie.getYearOfRelease())));
  adminLikesEdit->setText(QString::fromStdString(std::to_string(movie.getNumberOfLikes())));
  adminTrailerEdit->setText(QString::fromStdString(movie.getTrailer()));
}

void GUI::adminAddButtonHandler() {
  QString title = adminTitleEdit->text();
  QString genre = adminGenreEdit->text();
  QString year = adminYearEdit->text();
  QString likes = adminLikesEdit->text();
  QString trailer = adminTrailerEdit->text();
  Movie movie;
  try {
    Movie movie = Movie{title.toStdString(), genre.toStdString(), year.toInt(), likes.toInt(), trailer.toStdString()};
    if (!service.addMovie(movie)) {
      throw RepositoryException("Movie already exists");
    }
    this->populateMoviesTable();
  } catch (DomainException& e) {
    QMessageBox::critical(this, "Error", e.what());
  } catch (RepositoryException& e) {
    QMessageBox::critical(this, "Error", e.what());
  }
}

void GUI::adminDeleteButtonHandler() {
  int row = movieTableView->currentIndex().row();
  if (row == -1) {
    QMessageBox::critical(this, "Error", "No movie selected\nSelect a movie from the list to delete it");
    return;
  }
  service.removeMovieByPosition(row);
  populateMoviesTable();
}

void GUI::adminUpdateButtonHandler() {
  int row = movieTableView->currentIndex().row();
  if (row == -1) {
    QMessageBox::critical(this, "Error", "No movie selected\nSelect a movie from the list to update it");
    return;
  }
  QString title = adminTitleEdit->text();
  QString genre = adminGenreEdit->text();
  QString year = adminYearEdit->text();
  QString likes = adminLikesEdit->text();
  QString trailer = adminTrailerEdit->text();
  Movie movie{title.toStdString(), genre.toStdString(), year.toInt(), likes.toInt(), trailer.toStdString()};
  service.updateMovie(row, movie);
  populateMoviesTable();
}

void GUI::adminUndoButtonHandler() {
  try {
    service.undo();
    populateMoviesTable();
  } catch (RepositoryException& e) {
    QMessageBox::critical(this, "Error", e.what());
  }
}

void GUI::adminRedoButtonHandler() {
  try {
    service.redo();
    populateMoviesTable();
  } catch (RepositoryException& e) {
    QMessageBox::critical(this, "Error", e.what());
  }
}

// User Tab Handlers

void GUI::userFilterButtonHandler() {
  string genre = userGenreFilterEdit->text().toStdString();
  filteredMovies = service.getMoviesByGenre(genre);
  if (filteredMovies.size() == 0) {
    QMessageBox::critical(this, "Error", "No movies found with the given genre");
  } else {
    currentIndexInFilteredList = 0;
    populateUserLineEdits();
  }
}

void GUI::userAddWatchListButtonHandler() {
  if (userCurrentMovieTitleEdit->text().isEmpty()) {
    QMessageBox::critical(this, "Error", "No current movie available to be added in the list\nPlease first filter the movies based on a genre");
    return;
  }
  if (service.addMovieToWatchList(filteredMovies[currentIndexInFilteredList])) {
    populateWatchList();
  } else {
    QMessageBox::critical(this, "Error", "Movie already in watch list");
  }
  userNextMovieFromFilteredListHandler();
}

void GUI::userRateMovieWatchListButtonHandler() {
  if (service.getWatchListCount() == 0) {
    QMessageBox::critical(this, "Error", "There are no movies to be rated in the watch list\nAdd a movie to the watch list to rate it");
    return;
  }
  int row = userWatchListWidget->currentRow();
  if (row == -1) {
    QMessageBox::critical(this, "Error", "No movie selected\nSelect a movie from the watch list to rate it");
    return;
  }
  QMessageBox::StandardButton reply = QMessageBox::question(this, "Rate movie", "Did you like the movie?", QMessageBox::Yes | QMessageBox::No);
  if (reply == QMessageBox::Yes) {
    int indexOfMovieInRepository = service.getWatchList()[row];
    service.incrementLikes(indexOfMovieInRepository);
  }
  service.removeMovieByPositionWatchList(row);
  populateWatchList();
  populateMoviesTable();
}

void GUI::userOpenTrailerButtonHandler() {
  if (userCurrentMovieTitleEdit->text().isEmpty()) {
    QMessageBox::critical(this, "Error", "No movie selected to open the trailer\nPlease first filter the movies based on a genre");
    return;
  }
  QString title = userCurrentMovieTitleEdit->text();
  QString genre = userCurrentMovieGenreEdit->text();
  QString year = userCurrentMovieYearEdit->text();
  Movie movie = Movie(title.toStdString(), genre.toStdString(), year.toInt(), 0, "https://youtube.com/trailer-link");
  Movie movieFromRepo = service.getMovieByPosition(service.returnPosition(movie));
  string trailer = movieFromRepo.getTrailer();
  string command = "open " + trailer;
  system(command.c_str());
  userNextMovieFromFilteredListHandler();
}

void GUI::userNextMovieFromFilteredListHandler() {
  if (filteredMovies.size() == 0) {
    QMessageBox::critical(this, "Error", "Movies were not filtered\nPlease filter the movies based on a valid genre");
    return;
  }
  if (currentIndexInFilteredList == filteredMovies.size() - 1) {
    currentIndexInFilteredList = 0;
  } else {
    ++currentIndexInFilteredList;
  }
  populateUserLineEdits();
}

void GUI::userOpenCSVFileButtonHandler() {
  service.setCSV();
  service.saveWatchList();
  string command = "open " + service.getFileNameWatchList();
  system(command.c_str());
}

void GUI::userOpenHTMLFileButtonHandler() {
  service.setHTML();
  service.saveWatchList();
  string command = "open " + service.getFileNameWatchList();
  system(command.c_str());
}

MovieTableModel::MovieTableModel(Service& service, QObject* parent) : QAbstractTableModel{parent}, service(service) {}

MovieTableModel::~MovieTableModel() {}

int MovieTableModel::rowCount(const QModelIndex& parent) const {
  return service.getMovies().size();
}

int MovieTableModel::columnCount(const QModelIndex& parent) const {
  return 5;
}

QVariant MovieTableModel::data(const QModelIndex& index, int role) const {
  int row = index.row();
  int column = index.column();
  Movie movie = service.getMovieByPosition(row);
  if (role == Qt::DisplayRole) {
    switch (column) {
    case 0:
      return QString::fromStdString(movie.getTitle());
    case 1:
      return QString::fromStdString(movie.getGenre());
    case 2:
      return QString::fromStdString(std::to_string(movie.getYearOfRelease()));
    case 3:
      return QString::fromStdString(std::to_string(movie.getNumberOfLikes()));
    case 4:
      return QString::fromStdString(movie.getTrailer());
    default:
      break;
    }
  }
  return QVariant{};
}

QVariant MovieTableModel::headerData(int section, Qt::Orientation orientation, int role) const {
  if (role == Qt::DisplayRole) {
    if (orientation == Qt::Horizontal) {
      switch (section) {
      case 0:
        return QString{"Title"};
      case 1:
        return QString{"Genre"};
      case 2:
        return QString{"Year"};
      case 3:
        return QString{"Likes"};
      case 4:
        return QString{"Trailer"};
      default:
        break;
      }
    }
  }
  return QVariant{};
}

void MovieTableModel::setMovies() {
  beginResetModel();
  endResetModel();
}