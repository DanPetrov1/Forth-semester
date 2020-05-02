package com.epam.CPP.service;

public class Counter {
    private int number = 0;

    public Counter(int number) {
        this.number = number;
    }

    public synchronized void increment() {
        this.number++;
    }

    public synchronized void decrement() {
        this.number--;
    }

    public int getNumber() {
        return number;
    }

    public void setNumber(int number) {
        this.number = number;
    }
}
