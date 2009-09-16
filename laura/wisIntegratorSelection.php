 	<?php	
	/*$con = mysql_connect("atlasdev1.cern.ch","lodi","cOAnAd26")
                        or die("cannot connect to database server atlasdev1 :(");

        mysql_select_db("tbanalysis", $con);*/
        
     $con = ocilogon("ATLAS_TILECOM", "X#ep!zu75", "INTR")
			or die("cannot connect to database server INTR :(");

	

?>
<html>
	<head>
		<title>TileCal Commissioning Offline Shifts</title>
		<link href="./css/standard.css" type="text/css" rel="stylesheet" />
		<script language="JavaScript" src="wisRunSelectionScript.js"></script>
		<script>
		function chooseOrder( page)
		{
			if(document.getElementById("date_").selected)
				select= "date_";
			else
				select="moduleName";
			 var url= 'wisIntegratorSelection.php?page='+page+'&select='+select;
			window.location= url;
		}
               function openViewComments(filename){
                        var URL = "wisViewIntegratorSummaryComments.php?filename=" + filename;
                        window.open(URL, null , 'width=350,height=375,Scrollbars=YES');
        }
	
	function runIntegratorMacro(filename, rowNumber)
	{
		var URL = "wisRunIntegratorMacro.php?filename="+filename+"&rowNumber="+rowNumber;
		var tdId = "result" + rowNumber;
        	var td = document.getElementById(tdId);
        	td.setAttribute('align', 'left');
        	var b = document.createElement('B');
        	var image = document.createElement('IMG');
        	image.src = "images/hourglass_icon.gif";
        	image.width = 25;
        	image.height= 25;
        	b.appendChild(document.createTextNode('Running'));
        	b.appendChild(image);
        	var link = td.getElementsByTagName('A')[0];
        	if (link)
        	{
        	        td.insertBefore(b, link);
        	        var br = document.createElement('BR');
        	        td.insertBefore(br, link);
        	}
        	else
       	 	{
	                td.appendChild(b);
        	}
		callToServer(URL);
	//	window.location=URL;
	}
	</script>

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

  <h1 align="center">TileCal Commissioning Offline Shifts</h1>
        </div>
        <!-- /Header -->


        <!-- Main Container -->
        <div id="main-container">


                <!-- Contents -->

  <div id="contents">
    <p align ="left"><font size="1" face="arial"><a href="index.html">Initial page</a> </p><br />

	<?php

		//showing the total of pages
		$ROWS = 30;

		$page = $_GET["page"];
		if (!$page) {
		   $start = 0;
		   $page=1;
		}
		else {
		   $start = ($page - 1) * $ROWS;
		} 
		$query = "select * from wisIntegratorMacro ";
		$result = ociparse($con, $query);
		ociexecute($result, OCI_DEFAULT);
  	ocisetprefetch($result, 10000);
  	while(ocifetch($result));
		$result_num = ocirowcount($result);
		$pages_num = ceil($result_num / $ROWS);
		echo "<font size='2' face='Arial'>";
		if($start+$ROWS>$result_num)
			echo "Displaying ".($start+1)." to ".$result_num." from ".$result_num." results";
		else
			echo "Displaying ".($start+1)." to ".($start+$ROWS)." from ".$result_num." results";
		$select = $_GET["select"];
		echo" order by <select onChange=\"chooseOrder('$page');\">";
		if(!$select)
			$select ="date_";
		if($select=="date_"){
			echo"<option id='date_' selected='selected'>Test Date</option>";
			echo"<option id='moduleName' >Module Name</option>";
		}
		else{
			echo"<option id='moduleName' selected='selected'>Module Name</option>";
			echo"<option id='date_' >Test Date</option>";
		}
		echo"</select><br />";
		
		echo"Pages: ";
		if($page != 1)
			echo"&lt;&lt; <a class=\"blue\" href=\"wisIntegratorSelection.php?page=".($page-1)."&select=$select\" target=\"_self\">Prev</a>&nbsp;|\n" ;
		for($i=1; $i <= $pages_num; $i++)
		{	
			if($i< ($page-7))
				continue;
			if($i> ($page+7))
				break;
			if($page == $i)
				echo"<b>$i |</b> ";
			else
				echo"<a class=\"blue\" href=\"wisIntegratorSelection.php?page=$i&select=$select\" target=\"_self\"> $i</a>&nbsp;|\n" ;
		}
		if($page != $page_num)
			echo"<a href=\"wisIntegratorSelection.php?page=".($page+1)."&select=$select\" target=\"_self\"> Next</a> &gt;&gt;&nbsp;\n" ;
		echo"</font>";
	?>
	<table class="main" border="0" cellpadding="2" cellspacing="1" align="center" width="75%">

	<tr>
		<th width="3%" rowspan="2">&nbsp;</th>
		<th width="5%" rowspan="2">Module Name</th>
		<th width="45%" rowspan="2">Test Date</th>
		<th width="15%" rowspan="2">Check Integrator Output</th>
		<th width="32%" rowspan="2">Macro Results</th>
	</tr>
	<tr>
	</tr>
<?php
		$query= "Select filename, moduleName, date_, results, comments from wisIntegratorMacro ORDER BY ".$select." DESC";
		$result = ociparse($con, $query);
		ociexecute($result, OCI_DEFAULT);
		ocisetprefetch($result, $ROWS);
		//$result= mysql_query($query);
		if(!$result)
			echo"Invalid query: ".mysql_error();
		//mysql_data_seek($result, $start);
		$end= ($start+$ROWS);
		$i=$start;
		$j = 0;
		for ($j=0; $j < $i; $j++)
			ocifetch($result);
		while((ocifetchinto($result, $rrow, OCI_NUM)))
		{
			$i++;
			 if ($i % 2 == 0)
        		        $tcol = "#AAAAAA";
		        else $tcol = "#FFFFCC";//"#999999";
			echo"<tr bgcolor=\"$tcol\">";
			echo"<td>$i</td>\n";
			list( $filename, $moduleName, $date, $rresult, $comment)=$rrow;
			$tempArray = explode(".", $filename);
			array_pop($tempArray);
			$tempfilename = implode(".", $tempArray);
			if(!$comment || $comment=="")
					$lcol="gray";
			else
					$lcol="blue";
	
			echo"<td><a href=\"javascript:openViewComments('$filename')\" class=\"$lcol\">$moduleName</a></td>\n";
			echo"<td>$date</td>\n";
			echo"<td><button onClick=\"runIntegratorMacro('$tempfilename', $i);\">Check</button></td>\n";
			if(!$rresult)
				echo"<td id=\"result$i\">&nbsp;</td>\n";
			else
			{
				echo"<td id=\"result$i\"><a href=\"http://tileinfo.web.cern.ch/tileinfo/lps/WIS/dev/wisSeeMacroResult.php?filename=$tempfilename\" target=\"_blank\">Results</a></td>\n";
			}
			echo'</tr>';
			if($i == $end)
				break;
		}
?>

	</table>
	<?php
		echo "<font size='2' face='Arial'>";
		echo"Pages: ";
		if($page != 1)
			echo"&lt;&lt; <a class=\"blue\" href=\"wisIntegratorSelection.php?page=".($page-1)."&select=$select\" target=\"_self\">Prev</a>&nbsp;|\n" ;
		for($i=1; $i <= $pages_num; $i++)
		{	
			if($i< ($page-7))
				continue;
			if($i> ($page+7))
				break;
			if($page == $i)
				echo"<b>$i |</b> ";
			else
				echo"<a class=\"blue\" href=\"wisIntegratorSelection.php?page=$i&select=$select\" target=\"_self\"> $i</a>&nbsp;|\n" ;
		}
		if($page != $page_num)
			echo"<a href=\"wisIntegratorSelection.php?page=".($page+1)."&select=$select\" target=\"_self\"> Next</a> &gt;&gt;&nbsp;\n" ;
		echo"</font>";
		
		/* FREE RESULTS*/ 	
//mysql_free_result($resComment);
OCIFreeStatement($result);
/* close connection */
//mysql_close($mysqlCon);
ocilogoff($con);
	?>
                </div>
                <!-- /Contents -->

        </div>
        <!-- /Main -->


        <!-- Footer -->
        <div id="footer">
        &nbsp;Please <a href="mailto: alexfaria@lps.ufrj.br,amandama@lps.ufrj.br,Carmen.Maidantchik@cern.ch, Felipe.Grael@cern.ch,fferreira@lps.ufrj.br,Kaio.Galvao@cern.ch">send us</a> your comments and suggestions.
        </div>
        <!-- /Footer -->
	<script>
		 setTimeout("callToServer('wisFillIntegratorMacroTable.php');", 1*200);
	</script>
</body>
</html>

