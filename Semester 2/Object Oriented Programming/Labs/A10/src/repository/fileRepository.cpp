#include "../../include/repository/fileRepository.h"

FileRepository::FileRepository(const std::string& fileName, const std::string& fileNameWatchList, const int& repoType) : fileName(fileName), fileNameWatchList(fileNameWatchList), repoType(repoType) {}

FileRepository::~FileRepository() {
  saveMovies();
  clearWatchListFile(); // the file for the user does not persist, it is only used when running the app
  if (repoType == repoType::HTML)
    setCSV();
  else if (repoType == repoType::CSV)
    setHTML();
  clearWatchListFile();
}

// Movies Repository

void FileRepository::loadMoviesIntoRepository() {
  std::ifstream fileInput(fileName);
  if (!fileInput.is_open())
    throw FileException("Failed to open movie repository file");

  Movie readMovieFromTextFile;
  while (true) {
    try {
      fileInput >> readMovieFromTextFile;
      addMovie(readMovieFromTextFile);
    } catch (DomainException& e) {
      break;
    }
  }
}

void FileRepository::saveMovies() {
  std::ofstream fileOutput(fileName);

  if (!fileOutput.is_open())
    throw FileException("The file could not be opened");

  for (auto movie : getMovies())
    fileOutput << movie << "\n";
  fileOutput.close();
}

// Watch List Repository

void FileRepository::saveWatchList() {
  std::ofstream fileOutput(fileNameWatchList);
  if (!fileOutput.is_open())
    throw FileException("Failed to open watch list file");

  if (repoType == repoType::CSV) {
    fileOutput << "Title,Genre,Year of Release,Number of Likes,Trailer\n";
    for (auto index : getWatchList())
      // CSV Format
      fileOutput
          << getMovies()[index].getTitle() << "," << getMovies()[index].getGenre() << "," << std::to_string(getMovies()[index].getYearOfRelease()) << "," << std::to_string(getMovies()[index].getNumberOfLikes()) << "," << getMovies()[index].getTrailer() << "\n";
  } else if (repoType == repoType::HTML) {
    // HTML Format
    fileOutput << "<!DOCTYPE html>\n"
               << "<html>\n"
               << "<head>\n"
               << "<title>Watch List</title>\n"
               << "</head>\n"
               << "<body>\n"
               << "<table border=\"1\">\n"
               << "<tr>\n"
               << "<td>Title</td>\n"
               << "<td>Genre</td>\n"
               << "<td>Year of Release</td>\n"
               << "<td>Number of Likes</td>\n"
               << "<td>Trailer</td>\n"
               << "</tr>\n";
    for (auto index : getWatchList()) {
      fileOutput << "<tr>\n"
                 << "<td>" << getMovies()[index].getTitle() << "</td>\n"
                 << "<td>" << getMovies()[index].getGenre() << "</td>\n"
                 << "<td>" << std::to_string(getMovies()[index].getYearOfRelease()) << "</td>\n"
                 << "<td>" << std::to_string(getMovies()[index].getNumberOfLikes()) << "</td>\n"
                 << "<td><a href=\"" << getMovies()[index].getTrailer() << "\">Link</a></td>\n"
                 << "</tr>\n";
    }
    fileOutput << "</table>\n"
               << "</body>\n"
               << "</html>\n";
  } else
    throw FileException("Invalid watch list repo type");
  fileOutput.close();
}

void FileRepository::clearWatchListFile() {
  std::ifstream fileInput(fileNameWatchList, std::ofstream::out | std::ofstream::trunc);
  if (!fileInput.is_open())
    throw FileException("Failed to open watch list file");
  fileInput.close();
}

void FileRepository::setCSV() {
  fileNameWatchList = "../watchlist.csv";
  repoType = repoType::CSV;
}

void FileRepository::setHTML() {
  fileNameWatchList = "../watchlist.html";
  repoType = repoType::HTML;
}

std::string FileRepository::getFileNameWatchList() {
  return fileNameWatchList;
}
