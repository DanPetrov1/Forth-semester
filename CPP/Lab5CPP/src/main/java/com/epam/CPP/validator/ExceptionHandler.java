package com.epam.CPP.validator;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.stereotype.Component;
import org.springframework.web.servlet.HandlerExceptionResolver;
import org.springframework.web.servlet.ModelAndView;

@Component
public class ExceptionHandler implements HandlerExceptionResolver {
    
    private static final Logger logger = LoggerFactory.getLogger(ExceptionHandler.class);

    @Override
    public ModelAndView resolveException(javax.servlet.http.HttpServletRequest request,
                                         javax.servlet.http.HttpServletResponse response, Object o, Exception e) {
        System.out.println("Spring MVC ExceptionHandler handling");
        logger.error("ErrorLog: ", e);
        return new ModelAndView("exception");
    }
}
