<?php

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
    if ( count($exp)>0 ) $select .= "where " . implode(" and ",$exp);

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

	<script language="JavaScript" src="wisRunSelectionScript.js"></script>
	<!--<script language="JavaScript" src="createIFrame.js"></script>-->
	<script type="text/javascript">
		function handleResponse() {
		alert('this function is called from server.html');
		
	}
		function closeWarning(){
			document.getElementById("castorWarningDiv").style.visibility = "hidden";
	} 
		function openViewComments(moduleName, runNumber){
			var URL = "wisViewSummaryComments.php?moduleName=" + moduleName + "&runNumber=" + runNumber;
			window.open(URL, 'Insert a Summary Comment for this Module', 'width=350,height=375,Scrollbars=YES');
	}
	</script>

</head>

<body>

	
	<!-- Main Container -->
	<div id="main-container">

		<!-- Menu --><!--
		<div id="menu">
			<div id="menu-item" class="current">Reconstruction</div>
			<div id="menu-item">Analysis</div>
		</div>
		--><!-- /Menu -->

		<!-- Contents -->
		
  <div id="contents"> 
    <!--<a href="javascript:parent.script.newFrame('macros.xml');">test1</a>
		<a onclick="return callToServer();" href="blank.html">test2</a>-->
    <p align ="left"> 
      <?php
		ob_implicit_flush();
		ob_end_flush();
		
		//$search_ELOG_link = "http://pcata007.cern.ch/elog/Commissioning/?mode=XML&reverse=1&npp=20&subtext=#RUN_NUMBER";
		
		$con = mysql_connect("localhost","tilecal","")
			or die("cannot connect to database server pcata007 :(");
		
		$con2 = mysql_connect("atlasdev1.cern.ch","lodi","cOAnAd26")
			or die("cannot connect to database server atlasdev1 :(");
			
		mysql_select_db("commAnalysis", $con2);
		
		if (!isset($HTTP_POST_VARS["query"]))
		{
			$select = "SELECT run, type, date, events, digifrags FROM comminfo WHERE run > 60000 ORDER BY date DESC"; //LIMIT 0,10";
			$rows = 30;
		}
		//echo $select."<br />";
		
		mysql_select_db("tile", $con);
		$res = mysql_query($select, $con)
			or die("query failed :(");
		
		$nrows = mysql_num_rows($res);
		$nfields = mysql_num_fields($res);
		$end = $start+$rows-1;
		if ( $nrows < $end ) $end = $nrows;
		?>
    </p>
    <form name="form" action="wisReconstruction.php" method="post">

<?php

  // fast page access stil

  $curr = urlencode($select) . "&rows=" . $rows . "&start=";

  $p = $start - $rows;
  $prev = $curr . $p;

  for ($i=1, $n=1; $i<$nrows; $i+=$rows, $n++) {
    $next = $curr . $i;
  }

  $n = $end+1;
  $next = $curr . $n;

?>

		
      <table class="main" border="0" cellpadding="2" cellspacing="1" align="center" width="100%">
        <tr> 
          <th width="5%" rowspan="2">Date</th>
          <th width="9%" rowspan="2">#events</th>
          <th width="24%">Modules</th>
          <th width="16%" rowspan="2">ntuple</th>
          <!--<th rowspan="2">pool file</th>-->
          <th width="13%" rowspan="2" align="center">Run a macro</th>
          <th width="17%" colspan="6">Macros Results</th>
        </tr>
        <tr id="MacroTitles">
          <th><font size="1" face="Arial, Helvetica, sans-serif">click on the 
            module name for inserting a <font color="#FF0000"><br />
            summary comment</font></font></th>
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
		if ($aux[2] == "1") $module .= "A";
		else if ($aux[2] == "2") $module .= "C";
		$hexa = $aux[3] . $aux[4];
		$dec = hexdec($hexa);
		$module .= ($dec + 1);
		
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
	$row = mysql_fetch_row($res);
	
	switch ($row[1])
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
			$runType = $row[1];
			break;
	}

	$row[1] = $runType;

	$command = "./wisExistFile ntuple $row[0] $runType > /dev/null";
	passthru($command, $existNtuple);
	$command = "./wisExistFile pool $row[0] $runType > /dev/null";
	passthru($command, $existPool);

	if ($i % 2 == 0)
		$tcol = "#AAAAAA";
	else $tcol = "#FFFFCC";//"#999999";
	for ($j = 1; $j < $nfields - 1; $j++)
		echo "<td nowrap align=\"left\">$row[$j]</td>"; 

	//echo "<td align=\"center\"><input type=\"checkbox\" name=\"run\" value=\"$row[0] $runType\" /></td>";


	//echo "<td nowrap align=\"right\">$runType</td>";
	//echo $row[$nfields - 1];
	$modules = getModuleNames($row[$nfields - 1]);
	echo "<td nowrap align=\"left\">";
	$aux = strtok($modules, " ");
	while ($aux !== false)
	{//row[0] is the runNumber
		echo "<a href=\"javascript:openViewComments('$aux', '$row[0]');\">$aux</a> ";
		$aux = strtok(" ");
	}

	echo "</td>";

	if ($existNtuple == 1) // ntuple in local directory
		echo "<td align=\"center\"><font color=\"green\"><b>Local</b></font></td>";
	else
	if ($existNtuple == 2) // ntuple in CASTOR directory
		echo "<td align=\"center\"><font color=\"red\"><b>CASTOR</b></font></td>";
	else // ntuple do not exist
		echo "<td></td>";
	/*
	if ($existPool == 1) // pool file in local directory
		echo "<td align=\"center\"><font color=\"green\"><b>Local</b></font></td>";
	else
	if ($existPool == 2) // pool file in CASTOR directory
		echo "<td align=\"center\"><font color=\"red\"><b>CASTOR</b></font></td>";
	else // ntuple do not exist
		echo "<td></td>";
	*/
	echo "<td nowrap id=\"Row".$i."RunMacro\" name=\"Row".$i."RunMacro\"><select name=\"Row".$i."Select\" id=\"Row".$i."Select\"></select><input type=\"button\" value=\"Run\" onClick=\"runMacro($i);\" id=\"Row".$i."Submit\" /></td>";
	//echo "<td id=\"action$i\"></td>";
	//echo "<input type=\"hidden\" id=\"action$i"."runNumber\" name=\"action$i"."runNumber\" value=\"$row[0]\">";
	//echo "<input type=\"hidden\" id=\"action$i"."runType\" name=\"action$i"."runType\" value=\"$runType\">";
	
	$select2 = "SELECT Macro_Name, Module_Name FROM wisMacroResult WHERE Run_Number=$row[0] GROUP BY Macro_Name";
	$res2 = mysql_query($select2, $con2);

	echo "\n<script language=\"JavaScript\">\n";
	if ($existNtuple == 1 || $existNtuple == 2)
		$existAux = 1;
	else $existAux = 0;
	//echo "run[$i] = new Array(); run[$i]['runNumber'] = '$row[0]'; run[$i]['runType'] = '$runType';";
	echo "row[$i] = new Array(); row[$i]['runNumber'] = '$row[0]'; row[$i]['runType'] = '$runType'; row[$i]['modules'] = '$modules'; row[$i]['existNtuple'] = $existAux;\n";
	
	while (($row2 = mysql_fetch_row($res2)))
	{
		if ($row2[1] == "all")
		{	
			echo "row[$i]['$row2[0]'] = 'all';\n";
		}
		else
		{
			$select3 = "SELECT Module_Name FROM wisMacroResult WHERE Run_Number=$row[0] AND Macro_Name='$row2[0]' ORDER BY Module_Name";
			$res3 = mysql_query($select3, $con2);
			//$aux = mysql_error();
			//echo "alert('$aux');";
			$aux = "";
			while (($row3 = mysql_fetch_row($res3)))
			{
				if (strlen($aux) > 0)
					$aux .= " ";
				$aux .= "$row3[0]";
			}
			echo "row[$i]['$row2[0]'] = '$aux';\n";
		}
	}

	//echo "writeActions('$row[0]', '$runType', 'action$i');";
	echo "</script>\n";
	
	//$script .= "writeActions('$row[0]', '$runType', 'action$i');\n";
	
	echo "</tr>\n";
}

mysql_close($con);
mysql_close($con2);

?>
      </table>

		</form>
		
<?php

  // fast page access stil

  $curr = urlencode($select) . "&rows=" . $rows . "&start=";
  $p = $start - $rows;
  $prev = $curr . $p;

  for ($i=1, $n=1; $i<$nrows; $i+=$rows, $n++) {
    $next = $curr . $i;
  }

  $n = $end+1;
  $next = $curr . $n;

?>

		</div>
		<!-- /Contents -->

	</div>
	<!-- /Main -->


</body>

</html>
