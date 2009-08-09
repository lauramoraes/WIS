<html>

<head>

<?php

function checkCRCandBCID($runNumber, $runType)
{
	echo "alert('Macro: checkCRCandBCID()');\n";
	echo "alert('RunNumber: $runNumber; RunType: $runType');\n";
	passthru("./wisCheckCRCandBCID $runNumber $runType", $retCode);
}

function plots($runNumber)
{
	echo "alert('Macro: plots()');\n";
	echo "alert('RunNumber: $runNumber');\n";
	passthru("./wisPlots $runNumber $runType", $retCode);
}

function getField($name, $str)
{
	$aux = strstr($str, $name);
	//echo "alert('aux: $aux');";
	$aux2 = strtok($aux, ";");
	//echo "alert('aux2: $aux2');";
	strtok($aux2, ":");
	return strtok(":");
}

if (!$_POST)
	$_POST = &$_GET;
	reset($_POST);

	if (isset($_POST["name"]))
	$name = $_POST["name"];

	if (isset($_POST["parameters"]))
	$parameters = $_POST["parameters"];

	if (isset($_POST["tdId"]))
	$tdId = $_POST["tdId"];

	ob_implicit_flush();
	ob_end_flush();

	//echo "alert('name: $name');";
	//echo "alert('parameters: $parameters');";
	$runNumber = getField("runNumber", $parameters);
	$runType = getField("runType", $parameters);
	$maxEvents = getField("maxEvents", $parameters);
	$module = getField("module", $parameters);
	//echo "alert('./wisCheckCalibrationData $filename $pidFilename $runNumber $runType $module > /dev/null');";
	//echo "alert('maxEvents: $maxEvents');";
	/*
	   $running = getNumberOfProcsRunning();
	   if ($running >= 3)
	   {
	   echo "<script>\nalert('There are now $running macros running in the server. Wait');\n</script></head><body></body></html>";
	   exit;
	   }
	 */
	?>
	<script>
	setTimeout("document.getElementById('myForm').submit()", 10*1000);
	</script>

	</head>

	<body>

	<form name="myForm" id="myForm" action="wisIsRunning.php" method="post">
	<?php

	echo "<input type=\"hidden\" name=\"tdId\" value=\"$tdId\" />\n";

	$time = time();
	$filename = "tmp/".$time.".html";
	passthru("./wisCreateFile \"$filename\" ", $retCode);
while ($retCode != 0)
{
	$time++;
	$filename = "tmp/".$time.".html";
	passthru("./wisCreateFile \"$filename\" ", $retCode);
}
$pidFilename = "tmp/".$time.".tmp";
passthru("./wisCreateFile \"$pidFilename\"", $retCode);

echo "<input type=\"hidden\" name=\"filename\" value=\"$filename\" />\n";
echo "<input type=\"hidden\" name=\"pidFilename\" value=\"$pidFilename\" />\n";


switch ($name)
{
	case "CRCandBCID":
		$command ="./connect.sh ./wisExecuteInDEVWISFolder wisCallMacro ./macro/TTileCommRun/checkCRC_DMU_BCID ".$runNumber." ".$runType." ".$filename." ".$pidFilename. " ";
	echo "<pre>";
	passthru($command , $retCode);
	echo"</pre>";
	break;
	case "deadChannels":
		$command ="./connect.sh ./wisExecuteInDEVWISFolder wisCallMacro ./macro/TTileCommRun/checkDeadChannels ".$runNumber." ".$runType." ".$filename." ".$pidFilename. " ";
	echo "<pre>";
	passthru($command , $retCode);
	echo"</pre>";
	//	passthru("./wisDeadChannels $filename $pidFilename $runNumber $runType > /dev/null", $retCode);
	break;
	case "PulseHeight":
		$command ="./connect.sh ./wisExecuteInDEVWISFolder wisCallMacro ./macro/TTileCommRun/checkPulseHeight ".$runNumber." ".$runType." ".$filename." ".$pidFilename. " ";
	echo "<pre>";
	passthru($command , $retCode);
	echo"</pre>";

	//passthru("./wisPulseHeight $filename $pidFilename $runNumber $runType > /dev/null", $retCode);
	//echo "<script>prompt(\"\", \"./wisPulseHeight $filename $pidFilename $runNumber $runType $retCode> /dev/null\");</script>";
	break;
	case "baseline":
		$command ="./connect.sh ./wisExecuteInDEVWISFolder wisCallMacro ./macro/TTileCommRun/checkBaseline ".$runNumber." ".$runType." ".$filename." ".$pidFilename. " ";
	echo "<pre>";
	passthru($command , $retCode);
	echo"</pre>";

	//passthru("./wisBaseline $filename $pidFilename $runNumber $runType > /dev/null", $retCode);
	break;
	case "cnoise":
		$command ="./connect.sh ./wisExecuteInDEVWISFolder wisCallMacro ./macro/TTileCommRun/checkCnoise ".$runNumber." ".$runType." ". "$module $maxEvents ".$filename." ".$pidFilename; 
	echo "<pre>";
	passthru($command , $retCode);
	echo"</pre>";
	//passthru("./wisCnoise $filename $pidFilename $runNumber $runType $module $maxEvents > /dev/null", $retCode);
	break;
	case "checkCalibrationData":
		$command ="./connect.sh ./wisExecuteInDEVWISFolder wisCallMacro ./macro/TTileCommRun/checkCalibrationData ".$runNumber." ".$runType." ". "$module ".$filename." ".$pidFilename;
	echo "<pre>";
	passthru($command , $retCode);
	echo"</pre>";

	//passthru("./wisCheckCalibrationData $filename $pidFilename $runNumber $runType $module > /dev/null", $retCode);
	break;

}

//$command = "./wisCnoise $filename $pidFilename $runNumber $runType $module $maxEvents > /dev/null";

$pid = file_get_contents($pidFilename);
echo "<input type=\"hidden\" name=\"pid\" value=\"$pid\" />\n";

if (strlen($module) > 0 && strlen($module) < 4)
	$module = "LB" . $module;
	else
if (strlen($module) == 0)
	$module = "all";
	echo "<input type=\"hidden\" name=\"moduleName\" value=\"$module\" />\n";
	echo "<input type=\"hidden\" name=\"macroName\" value=\"$name\" />\n";
	echo "<input type=\"hidden\" name=\"runNumber\" value=\"$runNumber\" />\n";

	//		passthru("./wisDeleteFile $pidFilename > /dev/null", $retCode);

	?>
	</form>

	</body>

	</html>

