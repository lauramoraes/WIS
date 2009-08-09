<?php

/******************************************
 *
 * wisReconstruction.php
 *
 ******************************************/

?>

<html>

<head>
	<title>Web Interface Analysis</title>
	<link href="./css/standard.css" type="text/css" rel="stylesheet" />
</head>

<body>

	<!-- Top bar -->
	<div id="top-bar">
		[ <a href="http://tileinfo.web.cern.ch/tileinfo/lps/WIS/hp/">Project Home Page</a> ]&nbsp;.:.&nbsp;
		[ <a href="http://www.cern.ch/">CERN</a> ]&nbsp;.:.&nbsp;
		[ <a href="http://atlas.web.cern.ch/">ATLAS</a> ]&nbsp;.:.&nbsp;
		[ <a href="http://atlas.web.cern.ch/Atlas/SUB_DETECTORS/TILE/tilecal.html">Tile Calorimeter</a> ]&nbsp;.:.&nbsp;
		[ <a href="http://www.ufrj.br">Universidade Federal do Rio de Janeiro</a> ]&nbsp;
	</div>
	<!-- /Top bar -->

	<!-- Header -->
	<div id="header">
		<h1>Web Interface Analysis</h1>
	</div>
	<!-- /Header -->


	<!-- Main Container -->
	<div id="main-container">

		<!-- Menu -->
		<div id="menu">
			<div id="menu-item"><font size="+1">Offline Shift</font></div>
		</div>
		<!-- /Menu -->

		<!-- Contents -->
		<div id="contents">

			<h2>Reconstruction</h2>
			<table><tr><td>
<?php
include "wisConsts.php";

if (!$_POST)
	$_POST = &$_GET;
reset($_POST);

if (isset($_POST["run"]))
	$run = $_POST["run"];

ob_implicit_flush();
ob_end_flush();

$index = count($run);
for ($i = 0; $i < $index; $i++)
{
	$time = time();
	$filename = $time . ".tmp";
	passthru("./wisCreateFile $filename > /dev/null", $retCode);
	while ($retCode != 0)
	{
		$time++;
		$filename = $time . ".tmp";
		passthru("./wisCreateFile $filename > /dev/null", $retCode);
	}

	$runNumber = strtok($run[$i], " ");
	$runType = strtok(" ");
	$maxEvents = 40000;
	$version = 0;
	$jobOptions = "jobOptions_TileMobiComm.py";
	$castor = "yes";
	$castorDirectory = "/castor/cern.ch/user/t/tilebeam/commissioning"; 

	echo "<b>Reconstructing run $runNumber of type $runType.";
	//echo " Results to file $filename."
	echo " This will take <b>several</b> minutes...</b><br />                                                                                                                                                                                                  \n";
	
	ob_flush();
	flush();

	$command = "./wisReconstruction $runNumber $runType $maxEvents $version \"".WIS_OUTPUT_DIR."\" \"$jobOptions\" $castor \"$castorDirectory\" $filename";
	//echo "Command: \"$command\"<br /><br />\n";
	passthru($command);
	
	passthru("./wisDeleteFile $filename > /dev/null", $retCode);

	echo "<br /><br />\n";
}


/*
if (isset($_POST["runNumber"]))
	$runNumber = $_POST["runNumber"];

if (isset($_POST["runType"]))
        $runType = $_POST["runType"];

if (isset($_POST["version"]))
        $version = $_POST["version"];

if (isset($_POST["maxEvents"]))
        $maxEvents = $_POST["maxEvents"];

if (isset($_POST["filePrefix"]))
        $filePrefix = $_POST["filePrefix"];

if (isset($_POST["tileMap"]))
        $tileMap = $_POST["tileMap"];

if (isset($_POST["jobOptions"]))
        $jobOptions = $_POST["jobOptions"];

if (isset($_POST["castor"]))
        $castor = $_POST["castor"];

if (isset($_POST["castorDirectory"]))
        $castorDirectory = $_POST["castorDirectory"];

echo "RunNumber='$runNumber'<br />\n";
echo "RunType='$runType'<br />\n";
echo "Version='$version'<br />\n";
echo "MaxEvents='$maxEvents'<br />\n";
echo "FilePrefix='$filePrefix'<br />\n";
echo "TileMap='$tileMap'<br />\n";
echo "JobOptions='$jobOptions'<br />\n";
echo "castor='$castor'<br />\n";
echo "CastorDirectory='$castorDirectory'<br />\n";


$time = time();
$filename = $time . ".tmp";
passthru("./wisCreateFile $filename", $retCode);
while ($retCode != 0)
{
	$time++;
	$filename = $time . ".tmp";
	passthru("./wisCreateFile $filename", $retCode);
}

if (!isset($castor))
	$castor = "no";

ob_implicit_flush();

echo "<b>Reconstructing run $runNumber of type $runType.";
//echo " Results to file $filename."
echo " This will take several minutes...</b><br />\n";

$command = "./wisReconstruction $runNumber $runType $maxEvents $version ".WIS_OUTPUT_DIR." $jobOptions $castor \"$castorDirectory\" $filename";
//echo "Command: \"$command\"<br /><br />\n";
passthru($command);
*/

?>

			</td></tr></table>
		<a href="http://tileinfo.web.cern.ch/tileinfo/lps/WIS/dev/wisRunSelection.php">Back</a>
		<a onClick="window.close(); return false;" href="">Close</a>
		</div>
		<!-- /Contents -->

	</div>
	<!-- /Main -->


	<!-- Footer -->
	<div id="footer">
	&nbsp;Please send your comments and suggestions to our <a href="mailto:Kaio.Galvao@cern.ch,Carmen.Maidantchik@cern.ch">team</a>
	</div>
	<!-- /Footer -->

</body>

</html>

