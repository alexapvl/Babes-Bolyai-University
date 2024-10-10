package org.example;

import org.example.Model.*;
import org.example.Repository.Repository;

public class Main {
    public static void main(String[] args) {
        Car c1 = new Car(100);
        Car c2 = new Car(1001);
        Truck t1 = new Truck(2001);
        Truck t2 = new Truck(500);
        Motorbike m1 = new Motorbike(300);
        Motorbike m2 = new Motorbike(1500);
        Repository repo = new Repository(6);
        Controller controller = new Controller(repo);
        controller.addEntity(c1);
        controller.addEntity(c2);
        controller.addEntity(t1);
        controller.addEntity(t2);
        controller.addEntity(m1);
        controller.addEntity(m2);
        System.out.println("Print all vehicles with at least 1000 RON repair cost");
        controller.printExpensiveEntities();
        IEntity e = controller.removeEntity(2);
        System.out.println("Removed: " + e.myToString());
        System.out.println("Now print them again to check that the remove function worked as intended");
        controller.printExpensiveEntities();
    }
}