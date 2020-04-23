<%--
  Created by IntelliJ IDEA.
  User: Dan
  Date: 06.03.2020
  Time: 09:50
  To change this template use File | Settings | File Templates.
--%>
<%@ page contentType="text/html;charset=UTF-8" language="java" %>
<c:set var="contextPath" value="${pageContext.request.contextPath}"/>
<html>
<head>
    <title>Character</title>
</head>
<body>
<form method="POST" action="${contextPath}/character" class="ui-form">

        <div class="${Error != null ? 'has-error' : ''}">
                <input name="Text" type="text" class="form-row" placeholder="Text"
                       autofocus="true"/>
                <input name="Letter" type="text" class="form-row" placeholder="Letter"/>
                <span>${Error}</span>
                <button class="btn btn-lg btn-primary btn-block" type="submit">Count</button>
                <span>${Result}</span>
        </div>

</form>
</body>
</html>
