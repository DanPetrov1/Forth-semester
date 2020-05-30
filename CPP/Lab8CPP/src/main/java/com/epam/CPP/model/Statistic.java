package com.epam.CPP.model;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

public class Statistic {
    private String min;
    private String max;
    private String popular;
    private List<String> stat = new ArrayList<>();

    public String getMin() {
        return min;
    }

    public void setMin(String min) {
        this.min = min;
    }

    public String getMax() {
        return max;
    }

    public void setMax(String max) {
        this.max = max;
    }

    public String getPopular() {
        return popular;
    }

    public void setPopular(String popular) {
        this.popular = popular;
    }

    public List<String> getStat() {
        return stat;
    }

    public void setStat(List<String> stat) {
        this.stat = stat;
    }

    public void update() {
        for (String string : stat) {
            if (min == null || string.compareTo(min) < 0){
                min = string;
            }
            if (max == null || string.compareTo(max) > 0){
                max = string;
            }
            if (popular == null || Collections.frequency(stat, string) > Collections.frequency(stat, popular)) {
                popular = string;
            }
        }
    }
}
