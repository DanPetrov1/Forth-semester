package com.epam.CPP.service;

import com.epam.CPP.model.StringRequest;
import org.apache.commons.lang.StringUtils;

import java.util.stream.Stream;

public class StreamService {
    public Stream<Integer> countLetters(Stream<StringRequest> data) {
        return data.parallel().map(this::countLetters);
    }

    public int countLetters(StringRequest stringRequest) {
        return (StringUtils.countMatches(stringRequest.getString(), stringRequest.getLetter()));
    }
}
