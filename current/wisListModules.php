<?php

//################################
function getComment($runNumber,$moduleName,&$linkDB)
{
	$modulePrefix = substr($moduleName, 0, 3);
        $moduleNumber = substr($moduleName, -2, 2);
        $selectComment = "SELECT comments FROM tcaRun, idatlas, testsID ";
        $selectComment .= "WHERE tcaRun.runNumber=$runNumber ";
        $selectComment .= "AND tcaRun.idAtlasId=idatlas.id AND idatlas.code='$modulePrefix' ";
        $selectComment .= "AND tcaRun.idTestId=testsID.id AND testsID.testID=$moduleNumber;";
        $resComment = mysql_query($selectComment,$linkDB) or die("Failure: " . mysql_error());
        $rowComment = mysql_fetch_row($resComment);
        if($rowComment!==false && $rowComment[0]!=="" && $rowComment[0]!==null)
        {
                return $rowComment[0];
		//return $selectComment;
        }
        return false;	
}

$moduleString = "";
$runNumber = "";
if(!$HTTP_POST_VARS) 
	$HTTP_POST_VARS = &$HTTP_GET_VARS;
reset ($HTTP_POST_VARS);
while ( list($key, $val) = each($HTTP_POST_VARS))
{
	if ( $key=="moduleString" ) $moduleString = $val;  //getting the module name
	if ( $key=="runNumber" ) $runNumber = $val;  //getting the module name
	//if ( $key=="newComment" ) $newComment = $val;//getting newComment if exists
}

//connecting to mysql database
$mysqlCon = mysql_connect("atlasdev1.cern.ch","lodi","cOAnAd26")
			or die("cannot connect to database server atlasdev1 :(");
mysql_select_db("tbanalysis", $mysqlCon);

?>
<html>
<head>
	<title><?php echo "Modules of run $runNumber";?></title>
	<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1">
	<link href="./css/standard.css" type="text/css" rel="stylesheet" />
	<script>
		function resetComment(commentId)
		{
			document.getElementById(commentId).innerHTML="";
			document.getElementById(commentId).value="";
		}
	</script>
</head>
<script>

	function wisComment(module,sComment,rowColor)
	{
		var tbl = document.getElementById("mainTable"); 
		var txtIndex = document.getElementById(module).rowIndex + 1;
		var txtId = 'form' + module;
		if(document.getElementById(txtId))
		{
			tbl.deleteRow(txtIndex);
		}
		else
		{
			var newRow = tbl.insertRow(txtIndex);
			newRow.bgColor= rowColor;
			newRow.name= txtId;
			var newCell = newRow.insertCell(-1);
			newCell.innerHTML = '<form id="form' + module + '" action="wisInsertSummaryComment.php" target="RSIFrame"><textarea id="' + module + 'Comment" cols="33" rows="8" name="comment" >' + sComment + '</textarea><br/><input type="submit" value="Submit"/> <input type="button" onClick="resetComment(\''+ module + 'Comment\')" value="Reset"/><input name="module" type="hidden" value="' + module + '"/> <input name="runNumber" type="hidden" value="<?php echo "$runNumber";?>"/></form>';
		}
	}
	
</script>
<body>
	<table class="main" id="mainTable" width="100%" align="center">
	<tr><th bgcolor="#FFFFCC" ><?php echo "Modules of run $runNumber";?></th></tr>
<?php
	$i=0;
	$rcolor = "";
	$module = strtok($moduleString, " ");
	while ($module !== false)
	{
		if ($i %2 == 0)
		{
			$rcolor = "#FFFFCC";
		}
		else
		{
			$rcolor = "#AAAAAA";
		}
		//row[0] is the runNumber
		if($comment = getComment($runNumber,$module,$mysqlCon))
		{
			echo "<tr bgcolor=\"$rcolor\" id=\"$module\"><td align=\"left\"><a id=\"link$id\" class=\"gray\" href=\"javascript:wisComment('$module','$comment','$rcolor');\">$module</a></td></tr> ";
		}
		else
		{
			echo "<tr bgcolor=\"$rcolor\" id=\"$module\"><td align=\"left\"><a id=\"link$id\" class=\"blue\" href=\"javascript:wisComment('$module','$comment','$rcolor');\">$module</a></td></tr>";
		}
		$module = strtok(" ");
		$i++;
	}
?>
	
<IFRAME NAME="RSIFrame" ID="RSIFrame" STYLE="border:0px;width:0px;height:0px;"></IFRAME>
</body>
