<html>

<head>
	<title>TileCal Commissioning Offline Shifts</title>
	<link href="./css/standard.css" type="text/css" rel="stylesheet" />
	<script language="JavaScript" src="wisRunSelectionScript.js"></script>
	<!--<script language="JavaScript" src="createIFrame.js"></script>-->
	<script type="text/javascript">
		function arrangeValues()
		{	
			var run1=document.myForm.run.value;
			var run2=document.myForm.run2.value;
			
			var month=adjustDate(document.myForm.month.value);
			var day=adjustDate(document.myForm.rday.value);

			var month2=adjustDate(document.myForm.month2.value);
			var day2=adjustDate(document.myForm.rday2.value);
			
			var date2=document.myForm.year2.value+month2+day2;
			var date=document.myForm.year.value+month+day;
			document.myForm.exp_run.value = document.myForm.opr_run.value.concat(document.myForm.run.value);
			document.myForm.ex2_run.value = document.myForm.opr_run2.value.concat(document.myForm.run2.value);
			document.myForm.exp_date.value = document.myForm.opr_date.value.concat(date);
			document.myForm.ex2_date.value = document.myForm.opr_date2.value.concat(date2);
			return compareAll(date, date2, run1, run2);
		}
	</script>
<script type="text/javascript">
function today()
{
	var today=new Date();
	document.myForm.rday2.value=today.getDate();
	document.myForm.month2.value=today.getMonth()+1;
	document.myForm.year2.value=today.getFullYear();
}



function verifDate(date)
{
	var year="";
	var month="";
	var day="";
	year+=date.charAt(0);
	year+=date.charAt(1);
	year+=date.charAt(2);
	year+=date.charAt(3);
	
	month+=date.charAt(4);
	month+=date.charAt(5);
	
	day+=date.charAt(6);
	day+=date.charAt(7);
	
	var leap=0;
            switch (month)
            {
                case '01':
                case '03':
                case '05':
                case '07':
                case '08':
                case '10':
                case '12':
                    if  ((day <= 31) && (day >= 1))
                    {
                        return true;
                    }
                    break
                
                case '04':        
                case '06':
                case '09':
                case '11':
                    if  ((day <= 30) && (day >= 1))
                    {
                        return true;
                    }
                    break
                case '02':
                    
                    if ((year % 4 == 0) || (year % 100 == 0) || (year % 400 == 0))
                    {
                        leap = 1;
                    }
                    if ((leap == 1) && (day <= 29) && (day>=1))
                    {
                        return true;                
                    }
                    if ((leap != 1) && (day <= 28) && (day>=1))
                    {
                        return true;
                    }            
                    break                        
            }
	var date=day+"/"+month+"/"+year;
	alert(date+" is not a valid date.");
	return false;
}

function emptyFields()
{
	if ( 
	((document.myForm.run.value=="" || document.myForm.run2.value=="") || 
	(document.myForm.month.value=="" || document.myForm.rday.value=="") )||
	((document.myForm.month2.value=="" || document.myForm.rday2.value=="") || 
	(document.myForm.year2.value=="" || document.myForm.year.value=="")) )
	{
		alert("There are one or more empty fields.");
		return false;
	}
	return true;

}

function adjustDate(date)
{
	var ret_date="";
	
	if (date.length==1)
	{
	ret_date+="0"; 
	ret_date+=date;
	}
	else {ret_date=date; }
	return ret_date;
}
//Compare dates to make sure from is sooner than from
function compareDates(date1, date2){
//date format: YYYYMMDD	
	if(date2<=date1 && (document.myForm.opr_date.value==">=" || document.myForm.opr_date.value==">") && (document.myForm.opr_date2.value=="<=" || document.myForm.opr_date2.value=="<") )
	{
		alert("The search cannot proceed with the given dates. Correct the dates or the operators.\nHint: The upper date is working as the starting point, and the lower date as the finish.");
		return false;
	}
	if(date1<=date2 && (document.myForm.opr_date.value=="<=" || document.myForm.opr_date.value=="<") && (document.myForm.opr_date2.value==">=" || document.myForm.opr_date2.value==">") )
	{
		alert("The search cannot proceed with the given dates. Correct the dates or the operators.\nHint: The lower date is working as the starting point, and the upper date as the finish.");
		return false;
	}
	return true;
}

function compareRuns(run1, run2){
//date format: YYYYMMDD	
	if(run2<=run1 && (document.myForm.opr_run.value==">=" || document.myForm.opr_run.value==">") && (document.myForm.opr_run2.value=="<=" || document.myForm.opr_run2.value=="<") )
	{
		alert("The search cannot proceed with the given runs. Correct the runs or the operators.\nHint: The upper run is working as the starting point, and the lower run as the finish.");
		return false;
	}
	if(run1<=run2 && (document.myForm.opr_run.value=="<=" || document.myForm.opr_run.value=="<") && (document.myForm.opr_run2.value==">=" || document.myForm.opr_run2.value==">") )
	{
		alert("The search cannot proceed with the given runs. Correct the runs or the operators.\nHint: The lower run is working as the starting point, and the upper run as the finish.");
		return false;
	}
	return true;
}

function compareAll(date1, date2, run1, run2)
{
	if (emptyFields() && verifDate(date1) && verifDate(date2) && compareDates(date1, date2) && compareRuns(run1, run2))
	{
		return true;
	}
	return false;
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
		<h1>TileCal Commissioning Offline Shifts</h1>
	</div>
	<!-- /Header -->


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
		<h2>Commissioning Runs</h2>
		<font size="+1">Use this form to search for more runs.</font><br />


<font size=+1>Q</font>uery:
<i>set start and stop run number, dates, run type and setup, press <b>submit</b> button!</i>

<form name="myForm" action="runSelection_test.php" method="post" onSubmit="return arrangeValues();">

<input type="hidden" name="query" value="query" />
<input name="exp_run"  type="hidden" size="12" value="" />
<input name="exp_date" type="hidden" size="12" value="" />
<input name="ex2_run"  type="hidden" size="12" value="" />
<input name="ex2_date" type="hidden" size="12" value="" />

<table cellspacing="2">

<tr>
<td colspan="5"></td>
<td>year</td><td>month</td><td>day</td>
</tr>

<tr>
<td>run</td>
<td><select name="opr_run">
	<option value="=">=</option>
	<option value="&gt;">&gt;</option>
	<option value="&lt;">&lt;</option>
	<option value="&gt;=" selected>&gt;=</option>
	<option value="&lt;=">&lt;=</option>
	<!--<option>contains</option>-->
</select></td>
<td><input name="run" type="text" size="12" value="0000000"></td>

<td>date</td>
<td><select name="opr_date">
        <option value="=">=</option>
        <option value="&gt;">&gt;</option>
        <option value="&lt;">&lt;</option>
        <option value="&gt;=" selected>&gt;=</option>
	<option value="&lt;=">&lt;=</option>
        <!--<option>contains</option>-->
</select></td>
<td><input name="year" type="text" size="4" value="2000" maxlength="4"></td>
<td><input name="month" type="text" size="2" value="05" maxlength="2"></td>
<td><input name="rday" type="text" size="2" value="01" maxlength="2"></td>
<td>type</td><td>
<?php
  mysql_connect("pcata007","tilecal","")
  or die("cannot connect to the database server :(");

  $select = "select type from comminfo group by type order by run desc";
  $res = mysql_db_query("tile",$select)
  or die("query failed :(");

  $nrows = mysql_num_rows($res);	
 
  echo "<SELECT NAME=exp_type>\n";
  echo " <OPTION VALUE=\">=0\" SELECTED>All";
  for ($i = 0; $i < $nrows; $i++) {
    $field = mysql_fetch_row($res);
    echo "<OPTION>$field[0]\n";
  }
  echo "</SELECT>";
  mysql_close();
?>
</td>
</tr>
<tr>
<td>run</td>
<td><select name="opr_run2">
        <option value="=">=</option>
        <option value="&gt;">&gt;</option>
        <option value="&lt;">&lt;</option>
        <option value="&gt;=">&gt;=</option>
        <option value="&lt;=" selected>&lt;=</option>
        <!--<option>contains</option>-->
</select></td>
<td><input name="run2" type="text" size="12" value="9999999"></td>

<td>date</td>
<td><select name="opr_date2">
        <option value="=">=</option>
        <option value="&gt;">&gt;</option>
        <option value="&lt;">&lt;</option>
        <option value="&gt;=">&gt;=</option>
        <option value="&lt;=" selected>&lt;=</option>
        <!--<option>contains</option>-->
</select></td>
<td><input name="year2" type="text" size="4" value="" maxlength="4"></td>
<td><input name="month2" type="text" size="2" value="" maxlength="2"></td>
<td><input name="rday2" type="text" size="2" value="" maxlength="2"></td>
<script>today()</script>
<td>setup</td><td>
<?php
  mysql_connect("pcata007","tilecal","")
  or die("cannot connect to the database server :(");

  $select = "select setup from comminfo group by setup order by run desc";
  $res = mysql_db_query("tile",$select)
  or die("query failed :(");

  $nrows = mysql_num_rows($res);	
 
  echo "<SELECT NAME=exp_setup>\n";
  echo " <OPTION VALUE=\">=0\" SELECTED>All";
  for ($i = 0; $i < $nrows; $i++) {
    $field = mysql_fetch_row($res);
    echo "<OPTION>$field[0]\n";
  }
  echo "</SELECT>";

  mysql_close();
?>
</td>
</tr>
</table>
<p>
<br />
show <select name="rows"">
     <option>5</option>
     <option>10</option>
     <option selected>25</option>
     <option>50</option>
     </select> rows/page

and <input name="orderedby" type="hidden" value="date">
arrange in 
  <select name="orderdir">
      <option value="asc">ascending</option>
      <option value="desc" selected>descending</option>
  </select> order
<p><br>
<input type="submit">
<input type="reset">
</form>


		</div>
		<!-- /Contents -->

	</div>
	<!-- /Main -->


	<div id="footer">
	&nbsp;Please <a href="mailto:Kaio.Galvao@cern.ch,Carmen.Maidantchik@cern.ch">send us</a> your comments and suggestions.
	</div>
	<!-- /Footer -->

	<script language="JavaScript">
	<?php echo "$script\n"; ?>
	</script>

</body>

</html>
