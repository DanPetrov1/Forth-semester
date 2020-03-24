
<%--
  Created by IntelliJ IDEA.
  User: Dan
  Date: 17.03.2020
  Time: 08:35
  To change this template use File | Settings | File Templates.
--%>
<%@ page contentType="text/html;charset=UTF-8" language="java" %>


<body>
<!-- Page Content -->
<div class="container">

    <div class="row">
        <div class="col-lg-12">
            <h1 class="page-header">
                <small>Exception page</small>
            </h1>
            <ol class="breadcrumb">
                <li><a href="character">Character</a>
                </li>
                <li><a href="calculate">Calculate</a>
                </li>
            </ol>
        </div>
    </div>
    <!-- /.row -->

    <!-- Content Row -->
    <div class="row">

        <div class="col-lg-12">
            <h2>Exception occurred while processing the request</h2>
            <p>${pageContext.exception.message}</p>
        </div>

    </div>
    <!-- /.row -->

    <hr>

</div>
</body>