<?php

if (!$_POST)
	$_POST = &$_GET;
reset($_POST);

if (isset($_POST["filename"]))
{
	$filename = $_POST["filename"];
	$con = mysql_connect("atlasdev1.cern.ch", "lodi", "cOAnAd26")
			or die("cannot connect to database server :(");
	mysql_select_db("tbanalysis", $con);
	$select = "SELECT results FROM wisIntegratorMacro WHERE filename='$filename".".root';"; //LIMIT 0,10";	
}
else
{
	if (isset($_POST["runNumber"]))
		$runNumber = $_POST["runNumber"];

	if (isset($_POST["macroName"]))
		$macroName = $_POST["macroName"];

	if (isset($_POST["moduleName"]))
		$moduleName = $_POST["moduleName"];
	$con = mysql_connect("atlasdev1.cern.ch", "lodi", "cOAnAd26")
		or die("cannot connect to database server :(");

	mysql_select_db("commAnalysis", $con);
	
	$select = "SELECT Data FROM wisMacroResult WHERE Run_Number=$runNumber AND Macro_Name='$macroName' AND Module_Name='$moduleName'";
}
$res = mysql_query($select)
	or die("query failed :(");

$row = mysql_fetch_row($res);

$data = $row[0];

header("Content-Type: text/xml");
header("Content-Transfer-Encoding: binary");
header("Content-Length: ".strlen($data)); // not sure if it works

echo $data;

?>
