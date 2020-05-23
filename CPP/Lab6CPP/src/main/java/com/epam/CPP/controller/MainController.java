package com.epam.CPP.controller;

import com.epam.CPP.model.ArithmeticRequest;
import com.epam.CPP.model.Statistic;
import com.epam.CPP.service.Counter;
import com.epam.CPP.service.StreamService;
import org.apache.commons.lang.StringUtils;
import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.web.bind.annotation.ModelAttribute;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.RequestParam;

import java.util.HashMap;
import java.util.List;
import java.util.stream.Stream;

@Controller
public class MainController {
    Statistic statistic = new Statistic();
    List<String> list;
    StreamService streamService = new StreamService();
    Counter counter = new Counter(0);
    HashMap<Integer, Double> hashMap = new HashMap<Integer, Double>();

    @RequestMapping(value = "/calculate", method = RequestMethod.GET)
    public String calculate(Model model, @RequestParam("param1") String first, @RequestParam("param2") String second,
                            @RequestParam("param3") String act) {
        counter.increment();
        ArithmeticRequest request = new ArithmeticRequest(Double.parseDouble(first), Double.parseDouble(second), act);
        double result = 0;
        if (hashMap.containsKey(request.hashCode())) {
            result = hashMap.get(request.hashCode());
        } else {
            if (!act.equals("+") && !act.equals("-") && !act.equals("*") && !act.equals("/")) {  //%2b = '+'
                model.addAttribute("Error", "We can't calculate it!");
                counter.decrement();
                return "calculate";
            }
            switch (act) {
                case "+": {
                    result = request.getFirstNumber() + request.getSecondNumber();
                    break;
                }
                case "-": {
                    result = request.getFirstNumber() - request.getSecondNumber();
                    break;
                }
                case "*": {
                    result = request.getFirstNumber() * request.getSecondNumber();
                    break;
                }
                case "/": {
                    if (request.getSecondNumber() == 0) {
                        throw new ArithmeticException("/ by zero");
                    } else {
                        result = request.getFirstNumber() / request.getSecondNumber();
                    }
                    break;
                }
            }
            hashMap.put(request.hashCode(), result);
        }
        model.addAttribute("Result", result);
        counter.decrement();
        return "calculate";
    }

    @RequestMapping(value = "character", method = RequestMethod.POST)
    public String countCharacter(Model model, @ModelAttribute("Text") String text, @ModelAttribute("Letter") String letter) {
        if (letter.length() != 1) {
           model.addAttribute("Error", "Invalid number of letters!");
           return "character";
        }
        statistic.getStat().add(text);
        statistic.update();
        list.add(text + letter);
        Stream<Integer> stream = streamService.countLetters(list.stream());
        int occurrence = stream.findFirst().orElse(0);
        model.addAttribute("Result", occurrence);
        list.remove(text + letter);
        return "character";
    }

    @RequestMapping(value = "character", method = RequestMethod.GET)
    public String getNumberOfCharactersInString() {
        return "character";
    }
}
