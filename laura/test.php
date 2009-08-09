<?php

//phpinfo();
ini_set('display_errors', 'On');
echo 'display_errors = ' . ini_get('display_errors') . "<br>\n";

//if (extension_loaded('xml'))
if (dl('domxml.so'))
	echo "OK!!!<br>\n";
else
	echo "NOT OK!!!<br>\n";


print_r(get_extension_funcs("gd"));

if (!$_POST)
	$_POST = &$_GET;
reset($_POST);

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

$res = mysql_query($select)
	or die("query failed :(");

$row = mysql_fetch_row($res);

$data = $row[0];

mysql_close();

//header("Content-Type: text/xml");
//header("Content-Transfer-Encoding: binary");
//header("Content-Length: ".strlen($data)); // not sure if it works

//echo $data;
/*
$dom = domxml_open_mem($data);
if (!$dom)
{
  echo "Error while parsing the document\n";
  exit;
}

echo "OK!";

$root = $dom->document_element();
*/
?>
