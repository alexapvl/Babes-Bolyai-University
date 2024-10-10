package org.example.Repository;

import org.example.Model.IEntity;
import org.example.Model.MyException;

public interface IRepo {
    public int addEntity(IEntity entity) throws MyException;
    public IEntity removeEntity(int index) throws MyException;
    public IEntity[] getAll();
}
