package com.epam.CPP.service;

import com.epam.CPP.model.StringRequest;
import com.epam.CPP.repository.StringRequestRepository;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import java.util.List;
import java.util.stream.Stream;

@Service
public class StringRequestService {

    final
    StringRequestRepository stringRequestRepository;

    List<StringRequest> list;
    StreamService streamService = new StreamService();

    public StringRequestService(StringRequestRepository stringRequestRepository) {
        this.stringRequestRepository = stringRequestRepository;
    }

    public void createRequest(String request, String letter) {
        StringRequest stringRequest = stringRequestRepository.findByString(request);
        if (stringRequest == null) {
            stringRequest = new StringRequest();
            stringRequest.setString(request);
            stringRequest.setLetter(letter);
        }
        stringRequestRepository.save(stringRequest);
    }

    public List<StringRequest> getAll() {
        return stringRequestRepository.findAll();
    }

    public int completeTask(String text, String letter) {
        StringRequest stringRequest = stringRequestRepository.findByString(text);
        if (stringRequest == null) return 0;
        if (!stringRequest.getLetter().equals(letter)) stringRequest.setLetter(letter);
        list.add(stringRequest);
        Stream<Integer> stream = streamService.countLetters(list.stream());
        return stream.findFirst().orElse(0);
    }
}
