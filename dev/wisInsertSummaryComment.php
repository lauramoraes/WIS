<?php
$module = "";
$runNumber = "";
$comment= "";
if(!$HTTP_POST_VARS) 
	$HTTP_POST_VARS = &$HTTP_GET_VARS;
reset ($HTTP_POST_VARS);
while ( list($key, $val) = each($HTTP_POST_VARS))
{
	if ( $key=="module" ) $module = $val;  //getting the module name
	if ( $key=="runNumber" ) $runNumber = $val;  //getting the module name
	if ( $key=="comment" ) $comment = $val;//getting comment if exists
}


 //connecting to mysql database
 $mysqlCon = mysql_connect("atlasdev1.cern.ch","lodi","cOAnAd26")
 			or die("cannot connect to database server atlasdev1 :(");
 mysql_select_db("tbanalysis", $mysqlCon);
 
 $modulePrefix = substr($module, 0, 3);
         $moduleNumber = substr($module, -2, 2);
 		$selectComment  = "UPDATE tcaRun, idatlas, testsID SET comments='$comment' ";
        	$selectComment .= "WHERE tcaRun.runNumber=$runNumber ";
        	$selectComment .= "AND tcaRun.idAtlasId=idatlas.id AND idatlas.code='$modulePrefix' ";
         $selectComment .= "AND tcaRun.idTestId=testsID.id AND testsID.testID=$moduleNumber;";
 		mysql_query($selectComment, $mysqlCon) or die("FAILURE: " . mysql_error());
 		//echo "\tSuccessful Operation\n";
?>
<body>
<script>
	window.parent.location=window.parent.location;
</script>
</body>