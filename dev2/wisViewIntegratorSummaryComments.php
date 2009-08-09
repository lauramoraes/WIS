<?php
$moduleName = "";
$runNumber = "";
$newComment ="";
$comment= "";
if(!$HTTP_POST_VARS) 
	$HTTP_POST_VARS = &$HTTP_GET_VARS;
	reset ($HTTP_POST_VARS);
while ( list($key, $val) = each($HTTP_POST_VARS))
{
	if ( $key=="filename" )
	{
		$filename = $val;  //getting the module name
	}	
	if ( $key=="newComment" ) $newComment = $val;//getting newComment if exists
}
//connecting to mysql database
$mysqlCon = mysql_connect("atlasdev1.cern.ch","lodi","cOAnAd26")
or die("cannot connect to database server atlasdev1 :(");
mysql_select_db("tbanalysis", $mysqlCon);
?>

<html>
<head>
<title><?php echo"Insert a Summary Comment for LB$moduleName Module";?></title>
<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1">
<link href="./css/standard.css" type="text/css" rel="stylesheet" />
<script>
	function ResetButton()
	{
		document.getElementById("newComment").value="";
	}
</script>
</head>

<body>

<!-- Top bar -->
<div id="top-bar">
<?php
if($_POST["submit"])
{
	$selectComment  = "UPDATE wisIntegratorMacro SET comments='$newComment' ";
	$selectComment .= "WHERE filename='$filename'; ";
	mysql_query($selectComment, $mysqlCon) or die("FAILURE: " . mysql_error());
	echo "\tSuccessful Operation\n";
}
else
echo "<br />";
?>
</div>
<!-- /Top bar -->

<!-- Header -->
<div id="header">
<h3 align="center">TileCal Commissioning Offline Shifts</h3>
</div>
<!-- /Header -->


<!-- Main Container -->
<div id="main-container" align="center">
<div id="contents" align="center">
<table class="main" width="100%" align="center">
<tr>
<th width="10%">&nbsp;</th>
<th width="90%">Summary Comments</th>

</tr>	
<?php
$i = 0;
$selectComment = "SELECT comments, moduleName, date FROM wisIntegratorMacro ";
$selectComment .= "WHERE filename='$filename'; ";
$resComment = mysql_query($selectComment,$mysqlCon) or die("Failure: " . mysql_error());
$rowComment = mysql_fetch_row($resComment);
list($comment, $moduleName, $date)=$rowComment;
if($rowComment!==false && $comment!=="")
{
	echo "\n\t\t<tr>\n";
	echo "\t\t\t<td>&nbsp;</td>\n";	
	echo "\t\t\t<td>$comment</td>\n";
	echo "\t\t</tr>";
}
else
{
	echo "\n\t\t<tr>\n";
	echo "\t\t\t<td>&nbsp;</td>\n";
	echo "\t\t\t<td>There is no comments for this module</td>\n";
	echo "\t\t</tr>";
}


echo "\t</table>\n";

if($comment=="")
echo"\t<h5>Insert your comment about the <b>$moduleName</b> performance in the <b><i>$date</i></b> test:</h5>\n";
else
echo"\t<h5>Edit this comment about the $moduleName performance in the <b><i>$date</i></b> test:</h5>\n";
?>
<form action="wisViewIntegratorSummaryComments.php" method="post" name="submitComments">
<input name="filename" type="hidden" value="<?php echo"$filename";?>">
<?php 
echo "\t\t<textarea name=\"newComment\" id=\"newComment\"cols=\"33\" rows=\"8\">$comment</textarea>";
?>
<br />

<input name="submit" type="submit" value="Submit">
<input name="reset" type="button"  onClick="ResetButton();" value="Reset">
</form>
</div>
</div>
<?php
/* FREE RESULTS*/ 	
mysql_free_result($resComment);
/* close connection */
mysql_close($mysqlCon);?>
<!-- Footer -->
<div id="footer">
&nbsp;Please <a href="mailto: alexfaria@lps.ufrj.br,amandama@lps.ufrj.br,Carmen.Maidantchik@cern.ch, Felipe.Grael@cern.ch,fferreira@lps.ufrj.br,Kaio.Galvao@cern.ch">send us</a> your comments and suggestions.
</div>
<!-- /Footer -->
</body>

