package org.example.Repository;

import org.example.Model.IEntity;
import org.example.Model.MyException;

public class Repository implements IRepo {
    private IEntity[] entities;
    private int currentPos;

    public Repository(int capacity) {
        entities = new IEntity[capacity];
        currentPos = 0;
    }

    public int addEntity(IEntity newEntity) throws MyException {
        if(currentPos >= entities.length) {
            throw new MyException("List is full!");
        }

        entities[currentPos] = newEntity;
        return currentPos++; // make use of the post-increment operator
    }

    public IEntity removeEntity(int index) throws MyException {
        if (index >= currentPos || index < 0) {
            throw new MyException("Index out of bounds!");
        }

        IEntity removedEntity = entities[index];

        // Shift elements to the left
        for (int i = index; i < currentPos - 1; i++) {
            entities[i] = entities[i + 1];
        }

        // Set the last element to null as it is now duplicated
        entities[--currentPos] = null;

        return removedEntity;
    }


    public IEntity[] getAll() {
        IEntity[] result = new IEntity[currentPos];

for (int i = 0; i < currentPos; i++) {
            result[i] = entities[i];
        }

        return result;
    }

}
