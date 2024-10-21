package org.example.Controller;

import org.example.Model.IEntity;
import org.example.Model.MyException;
import org.example.Repository.IRepo;


public class Controller {
    IRepo repo;
    public Controller(IRepo repo) {
       this.repo = repo;
    }

    public int addEntity(IEntity e) {
        try {
            return repo.addEntity(e);
        } catch (MyException ex) {
            System.out.println("Error: " + ex.getMessage());
            return -1;
        }
    }

    public IEntity removeEntity(int index) {
        try {
            return repo.removeEntity(index);
        } catch (MyException e) {
            System.out.println(e.getMessage());
        }
        return null;
    }

    public void printExpensiveEntities() {
        IEntity[] entities = repo.getAll();
        for(IEntity e : entities) {
            if(e.computePrice() > 1000) {
                System.out.println(e.myToString());
            }
        }
    }
}
