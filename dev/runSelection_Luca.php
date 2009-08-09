<?php
set_time_limit(0);

	function getArrCount($arr, $value)
	{
		$a=0;
		for($j=0; $j<count($arr);$j++)
		{
			if($arr[$j]==$value)
				$a++;
		}
		return $a;
	}



$myFile = "summary.csv";

$fh = fopen($myFile, 'w') or die("can't open file");
fwrite($fh, "#run, run type, run date, OK, Some Problems, Bad, Not to be Analised, Not Analised, # of Modules\n");
$stringData="";
  // init variables
  $start = 1;
?>
<html>

<head>
<title>TileCal Commissioning Offline Shifts</title>
</head>

<body>
     <pre>
     <?php
     		echo"Writing Data...";
		ob_implicit_flush();
		ob_end_flush();


		$con = mysql_connect("pcata007","tilecal","")
			or die("cannot connect to database server pcata007 :(");

		$con2 = mysql_connect("atlasdev1.cern.ch","lodi","cOAnAd26")
			or die("cannot connect to database server atlasdev1 :(");
// Selecting which runs have plots

		mysql_select_db("tbanalysis", $con2);

	/************* Taking Runs with plots*****************/
		$selectPlot="SELECT tcaRun.id as runId , tcaRun.runNumber as runNumber , tcaRunType.id , tcaRunType.name as runType , idatlas.id , idatlas.code as code , tcaRun.ModuleNumber as idAtlas FROM tcaRun ,  tcaRunType , idatlas WHERE idatlas.id = tcaRun.idAtlasId AND tcaRunType.id = tcaRun.runTypeId AND tcaRun.hasT5Plot='1'";
		$resPlots = mysql_query($selectPlot, $con2)
			or die("query failed :(");
		$countRunPlot = mysql_num_rows($resPlots);
		$runPlotArray = array();
		for($indexRunPlots=0 ; $indexRunPlots<$countRunPlot; $indexRunPlots++)
		{
			$runPlots= mysql_fetch_assoc($resPlots);
			array_push($runPlotArray, $runPlots['runNumber']);
		}
	/************* Taking Runs with OK Status*****************/
		$selectStatus="SELECT tcaRun.id as runId , tcaRun.runNumber as runNumber , tcaRunType.id , tcaRunType.name as runType , idatlas.id , idatlas.code as code , tcaRun.ModuleNumber as idAtlas FROM tcaRun ,  tcaRunType , idatlas WHERE idatlas.id = tcaRun.idAtlasId AND tcaRunType.id = tcaRun.runTypeId AND tcaRun.statusCommentsId='1'";
		$resStatus = mysql_query($selectStatus, $con2)
			or die("query failed :(");
		$countRunStatus = mysql_num_rows($resStatus);
		$runOKArray = array();
		for($indexRunStatus=0 ; $indexRunStatus<$countRunStatus; $indexRunStatus++)
		{
			$runStatus= mysql_fetch_assoc($resStatus);
			$runOKArray[]= $runStatus['runNumber'];
		}
	/************* Taking Runs with SP Status*****************/
		$selectStatus="SELECT tcaRun.id as runId , tcaRun.runNumber as runNumber , tcaRunType.id , tcaRunType.name as runType , idatlas.id , idatlas.code as code , tcaRun.ModuleNumber as idAtlas FROM tcaRun ,  tcaRunType , idatlas WHERE idatlas.id = tcaRun.idAtlasId AND tcaRunType.id = tcaRun.runTypeId AND tcaRun.statusCommentsId='2'";
		$resStatus = mysql_query($selectStatus, $con2)
			or die("query failed :(");
		$countRunStatus = mysql_num_rows($resStatus);
		$runSPArray = array();
		for($indexRunStatus=0 ; $indexRunStatus<$countRunStatus; $indexRunStatus++)
		{
			$runStatus= mysql_fetch_assoc($resStatus);
			$runSPArray[]= $runStatus['runNumber'];
		}
	/************* Taking Runs with Bad Status*****************/
		$selectStatus="SELECT tcaRun.id as runId , tcaRun.runNumber as runNumber , tcaRunType.id , tcaRunType.name as runType , idatlas.id , idatlas.code as code , tcaRun.ModuleNumber as idAtlas FROM tcaRun ,  tcaRunType , idatlas WHERE idatlas.id = tcaRun.idAtlasId AND tcaRunType.id = tcaRun.runTypeId AND tcaRun.statusCommentsId='3'";
		$resStatus = mysql_query($selectStatus, $con2)
			or die("query failed :(");
		$countRunStatus = mysql_num_rows($resStatus);
		$runBdArray = array();
		for($indexRunStatus=0 ; $indexRunStatus<$countRunStatus; $indexRunStatus++)
		{
			$runStatus= mysql_fetch_assoc($resStatus);
			$runBdArray[]=$runStatus['runNumber'];
		}
	/************* Taking Runs with NA Status*****************/
		$selectStatus="SELECT tcaRun.id as runId , tcaRun.runNumber as runNumber , tcaRunType.id , tcaRunType.name as runType , idatlas.id , idatlas.code as code , tcaRun.ModuleNumber as idAtlas FROM tcaRun ,  tcaRunType , idatlas WHERE idatlas.id = tcaRun.idAtlasId AND tcaRunType.id = tcaRun.runTypeId AND tcaRun.statusCommentsId='4'";
		$resStatus = mysql_query($selectStatus, $con2)
			or die("query failed :(");
		$countRunStatus = mysql_num_rows($resStatus);
		$runNAArray = array();
		for($indexRunStatus=0 ; $indexRunStatus<$countRunStatus; $indexRunStatus++)
		{
			$runStatus= mysql_fetch_assoc($resStatus);
			$runNAArray[]= $runStatus['runNumber'];
		}




//Accessing comminfo
		mysql_select_db("commAnalysis", $con2);

		$select = "SELECT run, type, DATE_FORMAT(date,'%d/%m/%Y %H:%i:%s'), events, setup, digifrags FROM comminfo WHERE (trigger IS NULL or strcmp(trigger,'BAD')!=0) ORDER BY date DESC"; //LIMIT 0,10";

		mysql_select_db("tile", $con);
		$res = mysql_query($select, $con)
			or die("query failed :(");
		$nrows = mysql_num_rows($res);
		$nfields = mysql_num_fields($res);
		$end = $nrows-1;


  $curr = urlencode($select) . "&rows=" . $rows . "&start=";


?>

        <?php


/************************************************/
function getModuleNames($digifrags)
{
	$modules = "";
	$aux = "";
	$aux = strtok($digifrags, " ");
	while ($aux !== false)
	{
		$module = "";
		if ($aux[2] == "1") $module .= "LBA";
		else if ($aux[2] == "2") $module .= "LBC";
		else if ($aux[2] == "3") $module .= "EBA";
		else if ($aux[2] == "4") $module .= "EBC";
		$hexa = $aux[3] . $aux[4];
		$dec = hexdec($hexa) +1;
		if ($dec<10)
			$module .= 0;
		$module .= ($dec);

		//echo "$aux[0]$aux[1]$aux[2]$aux[3]$aux[4]:$hexa:$dec:$module:";
		if (strlen($modules) > 0)
			$modules .= " ";
		$modules .= "$module";

		$aux = strtok(" ");
	}

	//echo "$digifrags<br />\n";

	return $modules;
}
/************************************************/
$count=mysql_num_rows($res);
mysql_data_seek($res, $start - 1); // seek to the starting row

for ($i = $start; $i <= $end; $i++)
{
	$row = mysql_fetch_array($res, MYSQL_BOTH);
	$stringData="";

	switch ($row["type"])
	{
		case "Pedestal":
			$runType = "Ped";
			break;
		case "LED":
			$runType = "Led";
			break;
		case "CISmono":
			$runType = "MonoCis";
			break;
		case "MonoCIS":
			$runType = "MonoCis";
			break;
		case "CISscan":
			$runType = "CIS";
			break;
		default:
			$runType = $row["type"];
			break;
	}

	$row["type"] = $runType;


	echo$row[0]."\n";
	$stringData.=$row[0].",";

	$modules = getModuleNames($row[$nfields - 1]);
	$stringData.="$row[1],";
	$stringData.="$row[2],";


	$modulesArray=explode(" ",$modules);
	$modulesArray=array_unique($modulesArray);
	$Nmodules=str_word_count($modules);
	$numModules=$Nmodules;
	if($Nmodules!= 0)
	{
		$parcial= getArrCount($runOKArray, $row[0]);
		$perc= $parcial/$Nmodules;
		$perc = $perc*100;
		$perc_array[0]= round($perc);
		
		$parcial= getArrCount($runSPArray, $row[0]);
		$perc= $parcial/$Nmodules;
		$perc = $perc*100;
		$perc_array[1]= round($perc);
		
		$parcial= getArrCount($runBdArray, $row[0]);
		$perc= $parcial/$Nmodules;
		$perc = $perc*100;
		$perc_array[2]= round($perc);
		
		$parcial= getArrCount($runNAArray, $row[0]);
		$perc= $parcial/$Nmodules;
		$perc = $perc*100;
		$perc_array[3]= round($perc);
	
		$total=0;
		for($w=0; $w<4; $w++)
		{
			$total+=$perc_array[$w];
		}
		
		$total= 100 - $total;
		if ($total<0)
		{
			$total=0;
			$max=max($perc_array);
			for($w=0; $w<4; $w++)
			{
				if($perc_array[$w]==$max) {$perc_array[$w]--;break;}
			}
		}
	}
	if($Nmodules!= 0)
		$stringData.=$perc_array[0]."%,";
	if($Nmodules!= 0)
		$stringData.=$perc_array[1]."%,";
	if($Nmodules!= 0)
		$stringData.=$perc_array[2]."%,";
	if($Nmodules!= 0)
		$stringData.="$perc_array[3]%,";
	if($Nmodules!= 0)
		$stringData.="$total%,";
	if($modules=="") $numModules=0;
	$stringData.= "$numModules";
	$stringData.="\n";
	fwrite($fh, $stringData);
}
fclose($fh);
mysql_close($con);
mysql_close($con2);



?>
<script>
window.location="summary.csv";
</script>
</pre>
</body>

</html>
