<html>

<head>
<?php

if (!$_POST)
	$_POST = &$_GET;
reset($_POST);

if (isset($_POST["rowNumber"]))
	$tdId = $_POST["rowNumber"];

if (isset($_POST["runNumber"]))
	$runNumber = $_POST["runNumber"];

$con = mysql_connect("atlasdev1.cern.ch", "lodi", "cOAnAd26")
	or die("cannot connect to database server :(");
mysql_select_db("commAnalysis", $con);
$select = "SELECT Macro_Name FROM wisMacroResult WHERE Run_Number=$runNumber GROUP BY Macro_Name";

$res = mysql_query($select)
	or die("query failed :(");

$nrows = mysql_num_rows($res);
$nfields = mysql_num_fields($res);

$moduleName = "all";

if ($nrows > 0)
{
/*	if ($row[0] == "all")
		$moduleName = $row[0];
	else
		$moduleName = "";*/
	echo "<script>\n";
	while ($row = mysql_fetch_row($res))
	{
		$macroName = $row[0];
		$tdId = "Row" . $rowNumber . $macroName;
?>

	var td = parent.document.getElementById('<?php echo "$tdId"; ?>');
	parent.resultFile['<?php echo "$tdId"; ?>'] = '<?php echo "wisSeeMacroResult.php?runNumber=$runNumber&macroName=$macroName&moduleName=$moduleName"; ?>';
	link = td.getElementsByTagName('A')[0]; // link para os resultados
	if (link)
	{
		link.style.display = 'block';
		link.style.color = '#CC0000';
	}
	else
	{
		link = parent.document.createElement('A');
		link.setAttribute('href', '#');
		link.onclick = parent.showMacroResult;
		link.onmouseover = parent.showStatusBarResultMsg;
		link.onmouseout = parent.eraseStatusBarMsg;

		if (link.captureEvents) link.captureEvents(Event.CLICK); // for Netscape 4(?)
		var text = parent.document.createTextNode('Macro results');
		link.appendChild(text);
		link.style.color = '#CC0000';
		td.appendChild(link);
	}

<?php

	echo "</script>\n";
	}

}

mysql_close();

?>
</head>

<body>

</body>

</html>
