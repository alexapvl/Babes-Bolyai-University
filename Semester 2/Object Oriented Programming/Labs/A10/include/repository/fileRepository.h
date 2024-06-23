#pragma once
#include "../exceptions/repoExceptions.h"
#include "repository.h"

namespace repoType {
const int CSV = 1;
const int HTML = 2;
} // namespace repoType

class FileRepository : public Repository {
private:
  std::string fileName;
  std::string fileNameWatchList;
  int repoType;

public:
  FileRepository(const std::string& fileName, const std::string& fileNameWatchList, const int& repoType);
  virtual ~FileRepository();
  void loadMoviesIntoRepository() override;
  void saveMovies() override;
  void saveWatchList() override;
  void clearWatchListFile() override;
  void setCSV() override;
  void setHTML() override;
  std::string getFileNameWatchList() override;
};
