package org.example.Model;

public class MyException extends Exception {
    private final String message; // final because its value is not meant to be changed
    public MyException(String message) {
        this.message = message;
    }

    @Override
    public String getMessage() {
        return message;
    }

}