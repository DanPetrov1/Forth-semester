package com.epam.CPP.controller;

import org.apache.commons.lang.StringUtils;
import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.web.bind.annotation.ModelAttribute;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.RequestParam;

@Controller
public class MainController {

    @RequestMapping(value = "/calculate", method = RequestMethod.GET)
    public String calculate(Model model, @RequestParam("param1") String first, @RequestParam("param2") String second,
                            @RequestParam("param3") String act) {
        double firstNumber = Double.parseDouble(first);
        double secondNumber = Double.parseDouble(second);
        double result = 0;
        if(!act.equals("+") && !act.equals("-") && !act.equals("*") && !act.equals("/")) {  //%2b = '+'
            model.addAttribute("Error", "We can't calculate it!");
            return "calculate";
        }
        switch (act) {
            case "+":
            {
                result = firstNumber + secondNumber;
                break;
            }
            case "-":
            {
                result = firstNumber - secondNumber;
                break;
            }
            case "*":
            {
                result = firstNumber * secondNumber;
                break;
            }
            case "/":
            {
                if (secondNumber == 0) {
                    throw new ArithmeticException("/ by zero");
                } else {
                    result = firstNumber / secondNumber;
                }
                break;
            }
        }
        model.addAttribute("Result", result);
        return "calculate";
    }

    @RequestMapping(value = "character", method = RequestMethod.POST)
    public String countCharacter(Model model, @ModelAttribute("Text") String text, @ModelAttribute("Letter") String letter) {
        if (letter.length() != 1) {
           model.addAttribute("Error", "Invalid number of letters!");
           return "character";
        }
        int occurrence = StringUtils.countMatches(text, letter);
        model.addAttribute("Result", occurrence);
        return "character";
    }

    @RequestMapping(value = "character", method = RequestMethod.GET)
    public String getNumberOfCharactersInString() {
        return "character";
    }
}
