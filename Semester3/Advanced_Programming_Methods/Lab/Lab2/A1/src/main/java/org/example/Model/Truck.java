package org.example.Model;

public class Truck implements IEntity {
    private int price;
    public Truck(int price) {
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
        if(!(other instanceof Truck truck)) { // pattern variable, makes code cleaner
            return false;
        }
        // Truck truck = (Truck) other; // down casting
        return truck.getPrice() == this.price;
    }

    @Override
    public String toString() {
        return "Truck: " + this.price + " RON";
    }

    @Override
    public String myToString() {
        return this.toString();
    }
}
