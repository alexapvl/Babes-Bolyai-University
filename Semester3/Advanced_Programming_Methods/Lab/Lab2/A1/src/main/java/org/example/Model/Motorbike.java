package org.example.Model;

public class Motorbike implements IEntity{
    private int price;
    public Motorbike(int price) {
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
        if(!(other instanceof Motorbike)) {
            return false;
        }
        Motorbike bike = (Motorbike) other;
        return bike.getPrice() == this.price;
    }

    @Override
    public String toString() {
        return "Motorbike: " + this.price + " RON";
    }

    @Override
    public String myToString() {
        return this.toString();
    }
}
