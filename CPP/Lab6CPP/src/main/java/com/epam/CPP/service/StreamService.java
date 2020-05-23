package com.epam.CPP.service;

import org.apache.commons.lang.StringUtils;

import java.util.stream.Stream;

public class StreamService {
    public Stream<Integer> countLetters(Stream<String> data) {
        return data.parallel().map(this::countLetters);
    }

    public int countLetters(String string) {
        return (StringUtils.countMatches(string, String.valueOf(string.charAt(string.length() - 1))) - 1);
    }
}
