<html>
<head>

	<script language="JavaScript" src="wisRunSelectionScript.js"></script>
</head>
<body>
<script>
<?php

function fixoutput($str)
{
	for($a=32;$a<127;$a++)
		$good[] = $a;
	$len = strlen($str);
	for($b=0;$b < $len+1; $b++)
		if(in_array(ord($str[$b]), $good))
			$newstr .= $str[$b];
	return $newstr;
}

	//Connect Tilecomm Database
	$mysqlCon = mysql_connect("atlasdev1.cern.ch","lodi","cOAnAd26")
        	or die("cannot connect to database server atlasdev1 :(");
	mysql_select_db("tbanalysis", $mysqlCon);
	
	//Taking the modules
	$runNumber = "";
	$module="";

	if(!$HTTP_POST_VARS)
	        $HTTP_POST_VARS = &$HTTP_GET_VARS;
        reset ($HTTP_POST_VARS);
	while ( list($key, $val) = each($HTTP_POST_VARS))
	{
        	if ( $key=="runNumber" ) $runNumber = $val;  //getting the module name
        	if ( $key=="module" ) $module= $val;  //getting the module name
	}
	
	$modulePrefix = substr($module, 0 , 3);
	$moduleNumber = substr($module, 3 , 2);

	
	$selectComment = "SELECT comments FROM tcaRun, idatlas, testsID ";
	$selectComment .= "WHERE tcaRun.runNumber=$runNumber ";
	$selectComment .= "AND tcaRun.idAtlasId=idatlas.id AND idatlas.code='$modulePrefix' ";
	$selectComment .= "AND tcaRun.idTestId=testsID.id AND testsID.testID=$moduleNumber;";
	$resComment = mysql_query($selectComment,$mysqlCon) or die("Failure: " . mysql_error());
	$rowComment = mysql_fetch_row($resComment);
	$comment="";
	if($rowComment!=false && $rowComment[0]!="")
		$comment=addcslashes((rtrim($rowComment[0])), "\"@\0..\32");
	?>
	var div = parent.document.getElementById("commentBox");
	div.innerHTML= "";
	var top = parent.document.createElement("DIV");
	top.setAttribute("id", "top-bar");
	top.setAttribute("width", "100%");
	<?php
		echo"var text = parent.document.createTextNode(\"$module\" + \" \");";
	?>
	//var table= parent.document.createElement("table");
	//table.setAttribute("style", "table-layout: fixed;border: 0;border-spacing: 0px;empty-cells: show;margin: 0;");
	//var tr =  parent.document.createElement("tr");
	//var tdLeft=  parent.document.createElement("td");
	//tdLeft.setAttribute("width", "95%");
	//tdLeft.setAttribute("style", "color:#FFCCCC;  text-decoration: none;font-weight: bold;font-family: sans-serif;font-size: 0.8em;");
	 //tdLeft.appendChild(text);
	//tr.appendChild(tdLeft);
	//var tdRight = parent.document.createElement("td");
	//tdRight.setAttribute("width", "5%");
	var img = parent.document.createElement("IMG");
	img.src = "images/xicon2.gif";
	img.setAttribute("border", 0);
	var link = parent.document.createElement("A");
	link.setAttribute("href", "javascript:commentBoxFunction();");
	link.appendChild(img);
	//tdRight.appendChild(link);
	//tr.appendChild(tdRight);
	//table.appendChild(tr);
	//top.appendChild(table);
	top.appendChild(text);
	top.appendChild(link);
	div.appendChild(top);
	var header=  parent.document.createElement("DIV");
	header.setAttribute("id", "header");
	header.setAttribute("width", "100%");
	var a=  parent.document.createElement("A");
	a.href="http://tcws.web.cern.ch/tcws/WICOM/current/wicomViewStatusCommentTimeline.php?moduleNames=<?php echo$module?>";
	text=parent.document.createTextNode("Click here to see this module Timeline");
	a.target="_blank";
	a.appendChild(text);
	header.appendChild(a);
	div.appendChild(header);
	var main = parent.document.createElement("DIV");
	main.setAttribute("id", "main-container");
	var content =  parent.document.createElement("DIV");
	content.setAttribute("id", "contents");
	content.setAttribute("width", "100%");
	main.appendChild(content);

	var textarea= parent.document.createElement("textarea");
	textarea.setAttribute("rows", 4);
	textarea.setAttribute("cols", 30);
	//textarea.setAttribute("name", "comment");
	//textarea.setAttribute("id", "comment");
	<?php
		echo"text = parent.document.createTextNode(\"$comment\");";
	?>
	textarea.appendChild(text);
	//var form = parent.document.createElement("FORM");
	textarea.setAttribute("id", "comment");
	content.appendChild(textarea);
	br = parent.document.createElement("BR");
	content.appendChild(br);
	var link = parent.document.createElement("A");
	//var hidden1=parent.document.createElement("input");
	//hidden1.setAttribute("type", "hidden");
	//hidden1.setAttribute("value","<?php echo $runNumber?>");
	//hidden1.setAttribute("name", "runNumber");
	
	//var hidden2=parent.document.createElement("input");
	//hidden2.setAttribute("type", "hidden");
	//hidden2.setAttribute("value","<?php echo $module?>");
	//hidden2.setAttribute("name", "moduleName");
	

//	var hidden3=parent.document.createElement("input");
//	hidden3.setAttribute("type", "hidden");
//	hidden3.setAttribute("value", parent.document.URL);
//	hidden3.setAttribute("name", "url");
//			


/*	form.appendChild(hidden1);
	form.appendChild(hidden2);
	form.appendChild(hidden3);
*/
/*	br = parent.document.createElement("BR");
	form.appendChild(br);
	var button=parent.document.createElement("input");
	button.setAttribute("type", "submit");
	button.setAttribute("value",  "submit");
	form.appendChild(button);
	var button=parent.document.createElement("input");
	button.setAttribute("type", "reset");
	button.setAttribute("value",  "reset");
	form.appendChild(button);

	form.setAttribute("method", "post");
	form.setAttribute("action", "wisInsertDetailedComment.php");
	content.appendChild(form);
*/	

	div.appendChild(main);
	<?php
	mysql_free_result($resComment);
	mysql_close($mysqlCon);
?>

</script>
</body>
</html>
