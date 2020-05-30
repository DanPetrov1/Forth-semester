package com.epam.CPP.repository;


import com.epam.CPP.model.StringRequest;
import org.springframework.data.domain.Example;
import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.stereotype.Repository;

import java.util.List;

@Repository
public interface StringRequestRepository extends JpaRepository<StringRequest, Long> {
    List<StringRequest> findAll();
    StringRequest findByString(String string);
}
