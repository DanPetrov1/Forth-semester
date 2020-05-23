package com.epam.CPP.model;

import java.util.Objects;

public class ArithmeticRequest {
    private double firstNumber;
    private double secondNumber;
    private String act;

    public ArithmeticRequest(double firstNumber, double secondNumber, String act) {
        this.firstNumber = firstNumber;
        this.secondNumber = secondNumber;
        this.act = act;
    }

    public double getFirstNumber() {
        return firstNumber;
    }

    public void setFirstNumber(double firstNumber) {
        this.firstNumber = firstNumber;
    }

    public double getSecondNumber() {
        return secondNumber;
    }

    public void setSecondNumber(double secondNumber) {
        this.secondNumber = secondNumber;
    }

    public String getAct() {
        return act;
    }

    public void setAct(String act) {
        this.act = act;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        ArithmeticRequest that = (ArithmeticRequest) o;
        return firstNumber == that.firstNumber &&
                secondNumber == that.secondNumber &&
                Objects.equals(act, that.act);
    }

    @Override
    public int hashCode() {
        return Objects.hash(firstNumber, secondNumber, act);
    }
}
