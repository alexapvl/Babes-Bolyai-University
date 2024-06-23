#pragma once
#include "../repository/repository.h"
#include "movie.h"

class Action {
protected:
public:
  Action(){};
  virtual ~Action(){};

  virtual void executeUndo() = 0;
  virtual void executeRedo() = 0;
};

class ActionAdd : public Action {
private:
  Movie addedMovie;
  Repository& repo;

public:
  ActionAdd(Repository& repo, const Movie& addedMovie) : repo{repo}, addedMovie{addedMovie} {};
  ~ActionAdd(){};
  void executeUndo() override {
    int index = repo.returnPosition(addedMovie);
    repo.removeMovieByPosition(index);
  }
  void executeRedo() override {
    repo.addMovie(addedMovie);
  }
};

class ActionRemove : public Action {
private:
  Movie removedMovie;
  Repository& repo;

public:
  ActionRemove(Repository& repo, const Movie& removedMovie) : repo{repo}, removedMovie{removedMovie} {};
  ~ActionRemove(){};
  void executeUndo() override {
    repo.addMovie(removedMovie);
  }
  void executeRedo() override {
    int index = repo.returnPosition(removedMovie);
    repo.removeMovieByPosition(index);
  }
};

class ActionUpdate : public Action {
private:
  Movie oldMovie;
  Movie newMovie;
  Repository& repo;

public:
  ActionUpdate(Repository& repo, const Movie& oldMovie, const Movie& newMovie) : repo{repo}, oldMovie{oldMovie}, newMovie{newMovie} {};
  ~ActionUpdate(){};
  void executeUndo() override {
    int index = repo.returnPosition(newMovie);
    repo.updateMovie(index, oldMovie);
  }
  void executeRedo() override {
    int index = repo.returnPosition(oldMovie);
    repo.updateMovie(index, newMovie);
  }
};
