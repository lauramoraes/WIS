<html>

<head>

	<script>
<?php

	// if we're called by GET method, get POST vars from GET ones.
	if(!$HTTP_POST_VARS) $HTTP_POST_VARS = &$HTTP_GET_VARS;

	$runNumber = $HTTP_POST_VARS["run"];
	$moduleName = $HTTP_POST_VARS["module"];
	$comment = $HTTP_POST_VARS["comment"];
	
	//echo "alert('Run: $runNumber');";
	//echo "alert('Module: $moduleName');";
	//echo "alert('Comment: $comment');";

	mysql_connect("atlasdev1.cern.ch", "lodi", "tB2004ana")
		or die("alert('cannot connect to database server :(');");

	$modulePrefix = "LB" . $moduleName[0];
	$moduleNumber = $moduleName[1] . $moduleName[2];
	
	$select  = "UPDATE tcaRun, idatlas, testsID SET comments='$comment' ";
	$select .= "WHERE tcaRun.runNumber=$runNumber ";
	$select .= "AND tcaRun.idAtlasId=idatlas.id AND idatlas.code='$modulePrefix' ";
	$select .= "AND tcaRun.idTestId=testsID.id AND testsID.testID=$moduleNumber";

	//echo "alert('$select');";	
	
	$res = mysql_db_query("tbanalysis", $select)
		or die("alert('query failed :(');");

	echo "alert('Comments succesfully updated!');";

	mysql_close();

?>

	</script>

</head>

<body>

</body>

</html>
		