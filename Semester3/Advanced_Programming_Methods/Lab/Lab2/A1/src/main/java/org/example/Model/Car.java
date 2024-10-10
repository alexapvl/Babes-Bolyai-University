package org.example.Model;

public class Car implements IEntity {
    private int price;
    public Car(int price) {
        this.price = price;
    }
    public int getPrice() {
        return price;
    }

    public void setPrice(int price) {
        this.price = price;
    }

    @Override
    public int computePrice() {
        return price;
    }

    @Override
    public boolean equals(Object other) {
        if(!(other instanceof Car car)) {
            return false;
        }
        return car.getPrice() == this.price;
    }

    @Override
    public String toString() {
        return "Car: " + this.price + " RON";
    }

    @Override
    public String myToString() {
        return this.toString();
    }
}
