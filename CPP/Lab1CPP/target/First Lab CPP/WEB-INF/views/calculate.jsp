<%--
  Created by IntelliJ IDEA.
  User: Dan
  Date: 05.03.2020
  Time: 17:51
  To change this template use File | Settings | File Templates.
--%>
<%@ taglib prefix="spring" uri="http://www.springframework.org/tags" %>
<%@ taglib prefix="form" uri="http://www.springframework.org/tags/form" %>
<%@ page contentType="text/html;charset=UTF-8" language="java" %>
<html>
<head>
    <title>Calculate</title>
</head>
<body>
<div class="${Error != null ? 'has-error' : ''}">
    <p>${Error}</p>

</div>
<div class="${Error == null ? 'has-no-error' : ''}">
    <p>${Result}</p>
</div>
</body>
</html>
