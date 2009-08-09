
<?php
	$moduleName = "";
	$runNumber = "";
	$comment= "";
	$url="";
	if(!$HTTP_POST_VARS) 
		$HTTP_POST_VARS = &$HTTP_GET_VARS;
		reset ($HTTP_POST_VARS);
	while ( list($key, $val) = each($HTTP_POST_VARS))
	{
		if ( $key=="moduleName" ) $moduleName = $val;  //getting the module name
		if ( $key=="runNumber" ) $runNumber = $val;  //getting the module name
		if ( $key=="comment" ) $comment = $val;//getting newComment if exists
		if ( $key=="url" ) $url = $val;//getting newComment if exists
	}

	//connecting to mysql database
	$mysqlCon = mysql_connect("atlasdev1.cern.ch","lodi","cOAnAd26")
				or die("cannot connect to database server atlasdev1 :(");
	mysql_select_db("tbanalysis", $mysqlCon);


	$modulePrefix = substr($moduleName, 0, 3);
	$moduleNumber = substr($moduleName, -2, 2);
	$selectComment  = "UPDATE tcaRun, idatlas, testsID SET comments='$comment' ";
	$selectComment .= "WHERE tcaRun.runNumber=$runNumber ";
	$selectComment .= "AND tcaRun.idAtlasId=idatlas.id AND idatlas.code='$modulePrefix' ";
	$selectComment .= "AND tcaRun.idTestId=testsID.id AND testsID.testID=$moduleNumber;";
	mysql_query($selectComment, $mysqlCon) or die("FAILURE: " . mysql_error());


?>
<html>
<head>
</head>
<body>
Inserting comment...
<script>
	window.location ="<?php echo $url;?>";	
</script>
</body>
</html>
