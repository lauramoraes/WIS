<?php
	function getName($ccid){
		$command = "phone ";
		$command .= "-ccid $ccid -FU ";
		$exit = exec($command);
		if(strpos($exit,"not found") === false){
			$family = str_word_count(substr($exit,strpos($exit,";")+1,24),1);
			$name = str_word_count(substr($exit,strpos($exit,";")+24,15),1);
		}
		$auth = array_merge($name,$family);
		$autName = $auth[0];
		for ($i=1;$i<count($auth);$i++){
			$autName.=" ".$auth[$i];
		}
		return $autName;
	}
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



  // init variables
  $select = "";
  $orderedby = "";
  $orderdir="1";
  $rows = 25;
  $start = 1;

  // if we're called by GET method, get POST vars from GET ones.
  if(!$HTTP_POST_VARS) $HTTP_POST_VARS = &$HTTP_GET_VARS;

  reset ($HTTP_POST_VARS);
  while ( list($key, $val) = each($HTTP_POST_VARS) ) {
//    echo "$key => $val<br>";
    if ( $key[0]=="d" ) $dsp[] = substr($key, 4);
    if ( $key[0]=="e" && strlen($val)>0 ) {
      $val = strtr($val,"*","%"); // convert all * to %
      // check if there are no control symbols
      if ( strcspn($val,"!=><%") == strlen($val) ) $val = "=" . "\'" . $val . "\'";
      // check for wild cards, add last wild card in case of first one
      if ( strcspn($val,"%") < strlen($val) ) {
	if ($val[0]=="%") $val = $val . "%";
	$val = " like \'" . $val . "\'";
      }
      // finally construct expression
      $exp[] = substr($key, 4) . $val;
    }
    if ( $key=="select" ) $select = $val;
    if ( $key=="orderedby" ) $orderedby = $val;
    if ( $key=="orderdir" ) $orderdir = $val;
    if ( $key=="rows" ) $rows = $val;
    if ( $key=="start" ) $start = $val;
  }

  // construct select from where statement
  if ( strlen($select)<6 ) {
    $select = "select ";
    if ( count($dsp)>0 ) $select .= implode(",", $dsp);
    else $select .= " run, type, date, events, setup, digifrags ";

    $select .= " from comminfo ";
    if (count($exp) > 0)
        $select .= "where " . implode(" and ", $exp) . " and (trigger IS NULL or strcmp(trigger,'BAD')!=0)";
    else
        $select .= "where (trigger IS NULL or strcmp(trigger,'BAD')!=0)";

    if ( strlen($orderedby)>0 ) $select .= " order by " . $orderedby;
    if ( strlen($orderdir)>0 && strlen($orderedby)>0 ) $select .= " " . $orderdir;
  }

  //$select .= " limit $start,$rows";

  $select = stripslashes($select); // remove escape slashes

?>

<html>

<head>
<title>TileCal Commissioning Offline Shifts</title>
	<link href="./css/standard.css" type="text/css" rel="stylesheet" />
	<style type="text/css">
		<!--
	-->
	</style>
	<script language="JavaScript" src="wisRunSelectionScript.js"></script>
	<!--<script language="JavaScript" src="createIFrame.js"></script>-->
	<script type="text/javascript">
		function handleResponse() {
		alert('this function is called from server.html');

	}

		function adjustWidths()
        {
                matchWidth('header', 'contents');
                matchWidth('top-bar', 'header');
                matchWidth('footer', 'header');
        }


	/*	function openListModules(moduleString, runNumber)
	{
			var URL = "wisListModules.php?moduleString=" + moduleString + "&runNumber=" + runNumber;
			window.open(URL, "modules" , 'width=350,height=375,Scrollbars=YES');
			if(window.focus)
				popup.focus();
	}*/
		function ntuplePerDrawer(moduleString, runNumber, runType)
	{
			var URL = "wisNtuplesPerModule.php?moduleString=" + moduleString +"&runType="+runType+ "&runNumber=" + runNumber;
			popup=window.open(URL, "barrels", "width=780,height=600,scrollbars=1,resizable=1");
			if(window.focus)
				popup.focus();
	}

		function closeOrOpenCol(colName)
	{
			var colsArray = document.getElementsByName(colName);
			var i;
			var val;
			var lnkVal;
			if (colsArray[0].style.display== "none"){
				val = "";
				lnkVal="none";
			}
			else{
				val = "none";
				lnkVal="";
			}
			for(i=0;i<colsArray.length;i++){
				colsArray[i].style.display= val;

			}
			var lnkId = "collnk" +  colName;
			document.getElementById(lnkId).style.display=lnkVal;
			adjustWidths();
	}
	</script>
</head>

<body>

	<!-- Top bar -->
	<div id="top-bar">
		[ <a href="http://cern.ch/TileCommWebSystem">Tile Commissioning Web System</a> ]&nbsp;.:.&nbsp;
		[ <a href="http://atlas.web.cern.ch/">ATLAS</a> ]&nbsp;.:.&nbsp;
		[ <a href="http://atlas.web.cern.ch/Atlas/SUB_DETECTORS/TILE/tilecal.html">Tile Calorimeter</a> ]
	</div>
	<!-- /Top bar -->

	<!-- Header -->
	<div id="header">

  <h1 align="center">TileCal Commissioning Offline Shifts</h1>
	<font face="arial"><a href="index.html">Initial page</a> | <a href="wisRunSearch.php">Search</a></font>
	</div>
	<!-- /Header -->


	<!-- Main Container -->
	<div id="main-container">


		<!-- Contents -->

  <div id="contents">
<form name="form" action="wisReconstruction.php" method="post">
      <?php
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


	/***********************Taki9ng Shifters********************/

		$selectShifters="select tcaT5CommentsAuthors.ccid as ccid, tcaRun.runNumber as runNumber  from tcaT5CommentsAuthors, tcaRun where tcaT5CommentsAuthors.runId = tcaRun.id order by runNumber DESC;";
		$resShifters = mysql_query($selectShifters, $con2)
			or die("query failed :(");
		$countRunShifters = mysql_num_rows($resShifters);
		$shiftersArray = array();
		for($indexRunShifters=0 ; $indexRunShifters<$countRunShifters; $indexRunShifters++)
		{
			$runShifters= mysql_fetch_assoc($resShifters);
			$shiftersRuns[]=$runShifters['runNumber'];
			$shiftersArray[]= $runShifters['ccid'];
		}


//Accessing comminfo
		mysql_select_db("commAnalysis", $con2);

		if (!isset($HTTP_POST_VARS["query"]))
		{
			$select = "SELECT run, type, DATE_FORMAT(date,'%d/%m/%Y %H:%i:%s'), events, setup, digifrags FROM comminfo WHERE (trigger IS NULL or strcmp(trigger,'BAD')!=0) ORDER BY date DESC"; //LIMIT 0,10";
			$rows = 30;
		}

		mysql_select_db("tile", $con);
		$res = mysql_query($select, $con)
			or die("query failed :(");
		$nrows = mysql_num_rows($res);
		$nfields = mysql_num_fields($res);
		$end = $start+$rows-1;
		if ( $nrows < $end ) $end = $nrows;


  $curr = urlencode($select) . "&rows=" . $rows . "&start=";


?><BR/>

	<A ID="collnkrunData" HREF="javascript:closeOrOpenCol('runData')">Show Run |</A>
	<A ID="collnkrunTypeData" HREF="javascript:closeOrOpenCol('runTypeData')"> Show Run Type |</A>
	<A ID="collnkdateData" HREF="javascript:closeOrOpenCol('dateData')"> Show Date |</A>
	<A ID="collnkeventsData" HREF="javascript:closeOrOpenCol('eventsData')"> Show #events |</A>
	<A ID="collnksetup" HREF="javascript:closeOrOpenCol('setup')"> Show setup |</A>
	<A ID="collnkntupleData" HREF="javascript:closeOrOpenCol('ntupleData')"> Show Modules |</A>
	<A ID="collnkoverviewData" HREF="javascript:closeOrOpenCol('overviewData')"> Show Overview |</A>
	<A ID="collnkhistoData" HREF="javascript:closeOrOpenCol('histoData')"> Show Histogram |</A>
	<A ID="collnkPlotData" HREF="javascript:closeOrOpenCol('PlotData')"> Show Tilecomm Link |</A>
	<A ID="collnkshifterData" HREF="javascript:closeOrOpenCol('shifterData')"> Show  Shifter |</A>
	<!--<A ID="collnkrunAMacroData" HREF="javascript:closeOrOpenCol('runAMacroData')">Show "Run a macro"</A>-->
	<script type="text/javascript">
		document.getElementById('collnkrunData').style.display = "none";
		document.getElementById('collnkrunTypeData').style.display = "none";
		document.getElementById('collnkdateData').style.display = "none";
		document.getElementById('collnkeventsData').style.display = "none";
		document.getElementById('collnksetup').style.display = "none";
		document.getElementById('collnkntupleData').style.display = "none";
		document.getElementById('collnkhistoData').style.display = "none";
		document.getElementById('collnkPlotData').style.display = "none";
		document.getElementById('collnkoverviewData').style.display = "none";
		document.getElementById('collnkshifterData').style.display = "none";
		//document.getElementById('collnkrunAMacroData').style.display = "none";
	</script>
	<?php
	function orderLink($string)
	{
		global $orderdir;
		$tmp_orderdir="DESC";
		$URL="http://tcws.web.cern.ch/tcws/WIS/dev2/wisRunSelection.php?select=SELECT+run%2C+type%2C+DATE_FORMAT%28date%2C%27%25d%2F%25m%2F%25Y+%25H%3A%25i%3A%25s%27%29%2C+events%2C+setup%2C+digifrags+FROM+comminfo+WHERE+%28trigger+IS+NULL+or+strcmp%28trigger%2C%27BAD%27%29%21%3D0%29+ORDER+BY+";
		$endURL="&rows=30&query=query";
		$sufix="&orderdir=";
		if($string=="Run")
		{
			$value="run";
		}
		else if($string=="Run Date")
		{
			$value="date";
		}
		if ($orderdir=="1") $tmp_orderdir="ASC";
		if ($orderdir=="ASC") $tmp_orderdir="DESC";
		else $tmp_orderdir="ASC";
		$URL.=$value;
		$URL.="+".$tmp_orderdir.$endURL.$sufix.$tmp_orderdir;
		echo "<A HREF=\"$URL\">$string</A>";

	}
	?>


      <table id="mainTable" class="main" border="0" cellpadding="2" cellspacing="1" align="center" width="100%">
        <tr>
          <!--<th rowspan="2"></th>-->
          <th noWrap name="runData" id="runData" rowspan="2">
          <?php
          orderLink("Run");
          ?>

          <A HREF="javascript:closeOrOpenCol('runData')"/> <IMG BORDER="0" SRC="images/xicon2.gif"/></A></th>
          <th noWrap name="runTypeData" id="runTypeData" rowspan="2">Run Type <A HREF="javascript:closeOrOpenCol('runTypeData')"> <IMG BORDER="0" SRC="images/xicon2.gif"/></a> </th>
          <th noWrap name="dateData" id="dateData" rowspan="2">
          <?php
          orderLink("Run Date");
          ?>
          <A HREF="javascript:closeOrOpenCol('dateData')"> <IMG BORDER="0" SRC="images/xicon2.gif"/></a> </th>
          <th noWrap name="eventsData" id="eventsData" rowspan="2">#events <A HREF="javascript:closeOrOpenCol('eventsData')"> <IMG BORDER="0" SRC="images/xicon2.gif"/></a> </th>
           <th noWrap name="setup" id="setup" rowspan="2">Setup <A HREF="javascript:closeOrOpenCol('setup')"> <IMG BORDER="0" SRC="images/xicon2.gif"/></a> </th>
          <!--<th>Modules</th>-->
          <th noWrap name="ntupleData" id="ntupleData" rowspan="1">Modules <A HREF="javascript:closeOrOpenCol('ntupleData')"> <IMG BORDER="0" SRC="images/xicon2.gif"></a> </th>
          <th noWrap name="overviewData" id="overviewData" rowspan="1" colspan="6"> Overview  <A HREF="javascript:closeOrOpenCol('overviewData')"> <IMG BORDER="0" SRC="images/xicon2.gif"></a> </th>
          <th noWrap name="histoData" id="histoData" rowspan="2">Histogram  <A HREF="javascript:closeOrOpenCol('histoData')"> <IMG BORDER="0" SRC="images/xicon2.gif"></a> </th>
          <th noWrap name="PlotData" id="PlotData" rowspan="2">Plots <A HREF="javascript:closeOrOpenCol('PlotData')"><IMG BORDER="0" SRC="images/xicon2.gif"></a> </th>
          <th noWrap name="shifterData" id="shifterData" rowspan="2">Shifter <A HREF="javascript:closeOrOpenCol('shifterData')"><IMG BORDER="0" SRC="images/xicon2.gif"></a> </th>
        </tr>
        <tr id="MacroTitles">
	<th name="ntupleData" id="ntupleData"><font size="1" face="Arial, Helvetica, sans-serif">View <font color="#FF0000"> Ntuples Status</font> and <br /> <font color="#FF0000">detailed comments</font></font></th>
	<th name="overviewData" id="overviewData" width="40 px"><font size="1" face="Arial, Helvetica, sans-serif" color="green">OK</font></th>
	<th name="overviewData" id="overviewData"  width="40 px"><font size="1" face="Arial, Helvetica, sans-serif" color="#FBB117">Some Problems</font></th>
	<th name="overviewData" id="overviewData" width="40 px"><font size="1" face="Arial, Helvetica, sans-serif" color="red">Bad</font></th>
	<th name="overviewData" id="overviewData" width="40 px"><font size="1" face="Arial, Helvetica, sans-serif" color="black">Not to be Analized</font></th>
	<th name="overviewData" id="overviewData" width="40 px"><font size="1" face="Arial, Helvetica, sans-serif">Not Analized</font>
	</th>
	<th name="overviewData" id="overviewData" width="40 px"><font size="1" face="Arial, Helvetica, sans-serif" color="black"># of Modules</font></th>
        </tr>
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
		else if ($aux[2] == "2") $module .= "EBC";
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
		if($count==0)
		{
			echo "<tr align=\"right\" bgcolor=\"#FFFFCC\" name=\"Row1\" id=\"Row1\"><h3>No Results </h3></tr>";
		}
		else{
$script = "";
//echo $res.":".$start;
mysql_data_seek($res, $start - 1); // seek to the starting row

//for ($i = 0; $i < $nrows; $i++)
for ($i = $start; $i <= $end; $i++)
{
	$row = mysql_fetch_array($res, MYSQL_BOTH);

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


	if ($i % 2 == 0)
		$tcol = "#AAAAAA";
	else $tcol = "#FFFFCC";//"#999999";

	echo "<tr align=\"right\" bgcolor=\"$tcol\" name=\"Row$i\" id=\"Row$i\">";
	echo "<td name=\"runData\" id=\"runData\" nowrap align=\"left\"><a href=\"http://tileinfo.web.cern.ch/tileinfo/commselect.php?select=select * from comminfo where run=".$row[0]."\" target=\"_blank\">$row[0]</a></td>";

	$modules = getModuleNames($row[$nfields - 1]);
	echo "<td name=\"runTypeData\" id=\"runTypeData\" nowrap align=\"left\">$row[1]</td>";
	echo "<td name=\"dateData\" id=\"dateData\" nowrap align=\"left\">$row[2]</a></td>";
	echo "<td name=\"eventsData\" id=\"eventsData\" nowrap align=\"left\">$row[3]</td>";
    echo "<td name=\"setup\" id=\"setup\" nowrap align=\"left\">$row[4]</td>";

	echo"<td align=\"center\" name=\"ntupleData\" id=\"ntupleData\"><a href=\"javascript:ntuplePerDrawer('$modules','$row[0]', '$runType')\"><img src=\"./images/modules.gif\" border=\"0\"><img src=\"./images/editIcon.png\" width=\"20\" height=\"20\" border=\"0\"></a></td>";

	$Nmodules= str_word_count($modules);
	echo"<td align=\"center\" name=\"overviewData\" id=\"overviewData\">";
	if($Nmodules!= 0)
	{
		$parcial= getArrCount($runOKArray, $row[0]);
		$perc= $parcial/$Nmodules;
		$perc = $perc*100;
		$perc= round($perc);
		$total = $perc;
		echo"<font color=\"green\">".$perc."%</font>";
	}
	echo"</td>";
	echo"<td align=\"center\" name=\"overviewData\" id=\"overviewData\">";
	if($Nmodules!= 0)
	{
		$parcial= getArrCount($runSPArray, $row[0]);
		$perc= $parcial/$Nmodules;
		$perc = $perc*100;
		$perc= round($perc);
		$total += $perc;
		echo"<font color=\"#FBB117\">".$perc."%</font>";
	}

	echo"</td>";
	echo"<td align=\"center\" name=\"overviewData\" id=\"overviewData\">";
	if($Nmodules!= 0)
	{
		$parcial= getArrCount($runBdArray, $row[0]);
		$perc= $parcial/$Nmodules;
		$perc = $perc*100;
		$perc= round($perc);
		$total += $perc;
		echo"<font color=\"red\">".$perc."%</font>";
	}

	echo"</td>";
	echo"<td align=\"center\" name=\"overviewData\" id=\"overviewData\">";
	if($Nmodules!= 0)
	{
		$parcial= getArrCount($runNAArray, $row[0]);
		$perc= $parcial/$Nmodules;
		$perc = $perc*100;
		$perc= round($perc);
		$total += $perc;
		echo"<font color=\"black\">".$perc."%</font>";
	}

	echo"</td>";
	echo"<td align=\"center\" name=\"overviewData\" id=\"overviewData\">";
	 if($Nmodules!= 0)
        {
		$total= 100 - $total;
		if ($total<0)
				$total=0;//fix round error
		echo"$total%";
	}

	echo"</td>";
	echo"<td align=\"center\" name=\"overviewData\" id=\"overviewData\">";
	$numberModules=substr_count($modules, " ")+1;
	echo "$numberModules";


	echo"</td>";
		echo"<td align=\"center\" name=\"histoData\" id=\"histoData\">";

	$command = "./wicomExistHisto  $row[0] $runType >/dev/null";
	passthru($command, $existHisto);
                if( $existHisto!=0)
		{
			 $command = "./wicomExistHisto  $row[0] $runType";
		        passthru($command, $existHisto);

		}
                else
                        echo"No histogram";

	echo"</td>";
	echo"<td align=\"center\" name=\"PlotData\" id=\"PlotData\">";
	if(array_search($row[0],$runPlotArray ) !== false)
	{
	$selectPlot="http://atlas-php.web.cern.ch/atlas-php/NOVA/TBAnalysis/TileCommAnalysis/version1.3/searchResult.php?runNumber=".$row[0];
	echo"<a href=\"$selectPlot\" target=\"_BLANK\"><img src=\"./images/pngicon.gif\" border=0></a>";
	}

	echo"</td>";
	echo"<td align=\"center\" name=\"shifterData\" id=\"shifterData\">";
	for($b=0; $b< count($shiftersRuns); $b++)
	{
		while($shiftersRuns[$b]==$row[0])
		{
			if(($shiftersRuns[($b-1)]!= $shiftersRuns[$b]))
			{
				$temp=getName($shiftersArray[$b]);
				echo $temp."&nbsp;";
			}
			$b++;
		}

	}

	echo"</td>";
	echo "</tr>\n";
}

mysql_close($con);
mysql_close($con2);

}?>
      </table>

		</form>

<?php

  // fast page access stil

  $curr = urlencode($select) . "&rows=" . $rows . "&start=";

  echo "Pages: ";
  $p = $start - $rows;
  $prev = $curr . $p;
  if ( $p > 0 ) echo " &lt;&lt; <A HREF=\"wisRunSelection.php?select=$prev&query=query\">Prev</A> |\n";

  for ($i=1, $n=1; $i<$nrows; $i+=$rows, $n++) {
    $next = $curr . $i;
    if ($i==$start) echo " <B>$n</B> |\n";
    else if ( $i>($start-$rows*10) && $i<($start+$rows*10))
      echo " <A HREF=\"wisRunSelection.php?select=$next&query=query\">$n</A> |\n";
  }

  $n = $end+1;
  $next = $curr . $n;
  if ( $n < $nrows ) echo " <A HREF=\"wisRunSelection.php?select=$next&query=query\">Next</A> &gt;&gt; \n";

?>

		</div>
		<!-- /Contents -->

	</div>
	<!-- /Main -->


	<!-- Footer -->
	<div id="footer">
	&nbsp;Please <a href="mailto: Tile.Commissioning@cern.ch">send us</a> your comments and suggestions.
	</div>
	<!-- /Footer -->


	<script language="JavaScript">
	function writeAllActions()
	{
		<?php echo "$script\n"; ?>
	}
	setTimeout("writeAllActions()", 1*300);

	/*function writeAllLines()
	{
		writeMacroTitles();

		var i;
		for (i = <?php echo "$start"; ?>; i <= <?php echo "$end"; ?>; i++)
			writeRow(i);

	}*/

//	setTimeout("callToServer('wisUpdateTileCommDB.php');", 1*200);
	setTimeout(" adjustWidths();", 1*400);

	</script>

</body>

</html>
