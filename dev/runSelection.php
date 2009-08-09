
<?php
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
  $orderdir = "asc";
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
    else $select .= " run, type, date, events, digifrags ";

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
			$select = "SELECT run, type, DATE_FORMAT(date,'%d/%m/%Y %H:%i:%s'), events, digifrags FROM comminfo WHERE run > 60000 and (trigger IS NULL or strcmp(trigger,'BAD')!=0) and type ='Ped' ORDER BY date DESC"; //LIMIT 0,10";
		}
		
		mysql_select_db("tile", $con);
		$res = mysql_query($select, $con)
			or die("query failed :(");
		
		$nrows = mysql_num_rows($res);
		$nfields = mysql_num_fields($res);
		$end = $nrows;
		

?>

<pre>
<?php




echo"runNumber|runType|rundate|events|OK|Some Problems|Bad|Ignored|Not Analysed|Histo creation time|module|comment\n\n\n";		

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

	echo "$row[0]";
	
	$modules = getModuleNames($row[$nfields - 1]);
	echo "|$row[1]";
	echo "|$row[2]";
	echo "|$row[3]";

	//echo"<td align=\"center\" name=\"ntupleData\" id=\"ntupleData\"><a href=\"javascript:ntuplePerDrawer('$modules','$row[0]', '$runType')\"><img src=\"./images/modules.gif\" border=\"0\"><img src=\"./images/editIcon.png\" width=\"20\" height=\"20\" border=\"0\"></a></td>";
	
	$Nmodules= str_word_count($modules);
	if($Nmodules!= 0)
	{
		$parcial= getArrCount($runOKArray, $row[0]);
		$perc= $parcial/$Nmodules;
		$perc = $perc*100;
		$perc= round($perc);
		$total = $perc;
		echo"|$perc%";
	}
	if($Nmodules!= 0)
	{
		$parcial= getArrCount($runSPArray, $row[0]);
		$perc= $parcial/$Nmodules;
		$perc = $perc*100;
		$perc= round($perc);
		$total += $perc;
		echo"|$perc%";
	}
	else echo"|";

	if($Nmodules!= 0)
	{
		$parcial= getArrCount($runBdArray, $row[0]);
		$perc= $parcial/$Nmodules;
		$perc = $perc*100;
		$perc= round($perc);
		$total += $perc;
		echo"|$perc%";
	}
	else echo"|";

	if($Nmodules!= 0)
	{
		$parcial= getArrCount($runNAArray, $row[0]);
		$perc= $parcial/$Nmodules;
		$perc = $perc*100;
		$perc= round($perc);
		$total += $perc;
		echo"|$perc%";
	}
	else echo"|";

	 if($Nmodules!= 0)
        {
		$total= 100 - $total;
		if ($total<0)
				$total=0;//fix round error
		echo"|$total%";
	}
	else echo"|";
		
	echo"|";
	$command = "./wicomExistHisto  $row[0] $runType >/dev/null";
	passthru($command, $existHisto);
                if( $existHisto!=0)
		{
			 $command = "./wicomExistHisto  $row[0] $runType";
		        passthru($command, $existHisto);

		}
                else
                        echo"&nbsp;";

		$selectComment="SELECT tcaRun.comments as comments, idatlas.code as code,  tcaRun.ModuleNumber as ModuleNumber FROM tcaRun ,idatlas  WHERE  tcaRun.runNumber = '$row[0]' AND idatlas.id= tcaRun.idAtlasId AND comments IS NOT NULL";
		$resComment = mysql_query($selectComment, $con2);
		if(!$resComment)die('Invalid query: ' . mysql_error());		
		$numComments = mysql_num_rows($resComment);
		 $comments= array();
		 $modulesPreffix= array();
		 $modulesNumbers=array();
		for($indexComments = 0; $indexComments< $numComments; $indexComments++)
		{
			$rowComment = mysql_fetch_array($resComment, MYSQL_BOTH);
			$comments[$indexComments]=  $rowComment['comments'];
			$modulesPreffix[$indexComments] =  $rowComment['code'];
			$modulesNumbers[$indexComments] = $rowComment['ModuleNumber'];
		}
	$moduleTemp = strtok($modules, " ");
        while($moduleTemp!=false)
        {
		echo"|$moduleTemp ";
		for($indexComments = 0; $indexComments< $numComments; $indexComments++)
		{
			if ($modulesNumbers[$indexComments]<10)
				$temp=$modulesPreffix[$indexComments]."0".$modulesNumbers[$indexComments];
			else
				$temp=$modulesPreffix[$indexComments].$modulesNumbers[$indexComments];
			if($temp == $moduleTemp)
			{
				$comment=addcslashes((rtrim($comments[$indexComments])), "\"@\0..\32");
				echo"$comment";
			}
		}
                $moduleTemp = strtok(" ");
        }
	
		echo"\n";

}
echo"</pre>";
mysql_close($con);
mysql_close($con2);

?>
		</pre>

