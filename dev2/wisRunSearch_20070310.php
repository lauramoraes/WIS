<html>

<head>
	<title>TileCal Commissioning Offline Shifts</title>
	<link href="./css/standard.css" type="text/css" rel="stylesheet" />
	<script language="JavaScript" src="wisRunSelectionScript.js"></script>
	<!--<script language="JavaScript" src="createIFrame.js"></script>-->
	<script type="text/javascript">
		function arrangeValues()
		{
			var date=getDateFromFormat(document.myForm.date.value);
			var date2=getDateFromFormat(document.myForm.date2.value);
			document.myForm.exp_run.value = document.myForm.opr_run.value.concat(document.myForm.run.value);
			document.myForm.ex2_run.value = document.myForm.opr_run2.value.concat(document.myForm.run2.value);
			document.myForm.exp_date.value = document.myForm.opr_date.value.concat(date);
			document.myForm.ex2_date.value = document.myForm.opr_date2.value.concat(date2);
			fillDate(date, date2);
			alert(document.myForm.exp_date.value);
			alert(document.myForm.ex2_date.value);
			return compareDates(date, date2);

		}
	</script>
<script type="text/javascript">
//Responsible for changing the date format into wisRunsearch friendly
function getDateFromFormat(date){
var day="";
var month="";
var year="";
var token1="";
var token2="";
var token3="";
token1=+date.charAt(1);
token2=+date.charAt(3);
token3=+date.charAt(4);
if (isNaN(token1))
{
	day+="0";
	day+=date.charAt(0);
	
	if (isNaN(token2))
	{
		month+="0";
		month+=date.charAt(2);
		
		year+=date.charAt(4);
		year+=date.charAt(5);
		year+=date.charAt(6);
		year+=date.charAt(7);
	}
	else
	{
		month+=date.charAt(2);
		month+=date.charAt(3);	
		
		year+=date.charAt(5);
		year+=date.charAt(6);
		year+=date.charAt(7);
		year+=date.charAt(8);
	}
}
else
{
	day+=date.charAt(0);
	day+=date.charAt(1);

	if (isNaN(token3))
	{
		month+="0";
		month+=date.charAt(3);
		
		year+=date.charAt(5);
		year+=date.charAt(6);
		year+=date.charAt(7);
		year+=date.charAt(8);
	}
	else
	{
		month+=date.charAt(3);
		month+=date.charAt(4);	
		
		year+=date.charAt(6);
		year+=date.charAt(7);
		year+=date.charAt(8);
		year+=date.charAt(9);
	}
}

ret_date=year+month+day;
return ret_date;
}
//Compare dates to make sure from is sooner than from
function compareDates(fromDate, toDate){
//date format: YYYYMMDD
	if(toDate<fromDate)
	{
		alert("The from date must be earlier than to date.");
		return false;
	}
	else 
	{
		return true;
	}
}

function fillDate(date, date2)
{
	var day="";
	var month="";
	var year="";
	var day2="";
	var month2="";
	var year2="";
	
	day+=date.charAt(6);
	day+=date.charAt(7);
	
	month+=date.charAt(4);
	month+=date.charAt(5);
	
	year+=date.charAt(0);
	year+=date.charAt(1);
	year+=date.charAt(2);
	year+=date.charAt(3);
	
	document.myForm.rday.value = document.myForm.rday.value.concat(day);
	document.myForm.month.value = document.myForm.month.value.concat(month);
	document.myForm.year.value = document.myForm.year.value.concat(year);
	
	day2+=date2.charAt(6);
	day2+=date2.charAt(7);
	
	month2+=date2.charAt(4);
	month2+=date2.charAt(5);
	
	year2+=date2.charAt(0);
	year2+=date2.charAt(1);
	year2+=date2.charAt(2);
	year2+=date2.charAt(3);
	
	document.myForm.rday2.value = document.myForm.rday2.value.concat(day2);
	document.myForm.month2.value = document.myForm.month2.value.concat(month2);
	document.myForm.year2.value = document.myForm.year2.value.concat(year2);
	
	return true;
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

<form name="myForm" action="wisRunSelection.php" method="post" onSubmit="return arrangeValues();">

<input type="hidden" name="query" value="query" />
<input name="exp_run"  type="hidden" size="12" value="" />
<input name="exp_date" type="hidden" size="12" value="" />
<input name="ex2_run"  type="hidden" size="12" value="" />
<input name="ex2_date" type="hidden" size="12" value="" />
<input name="year" type="hidden" size="4" value="">
<input name="month" type="hidden" size="2" value="">
<input name="rday" type="hidden" size="2" value="">
<input name="year2" type="hidden" size="4" value="">
<input name="month2" type="hidden" size="2" value="">
<input name="rday2" type="hidden" size="2" value="">

<table cellspacing="2">

<tr>
<td colspan="5"></td>
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
<td><input name="run" type="text" size="12" value="0060000"></td>

<td>date</td>
<td><select name="opr_date">
        <option value="=">=</option>
        <option value="&gt;">&gt;</option>
        <option value="&lt;">&lt;</option>
        <option value="&gt;="  selected>&gt;=</option>
	<option value="&lt;=">&lt;=</option>
        <!--<option>contains</option>-->
</select></td>
<td><input name="date" type="text" size="13" value="31/12/2000"></td>
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
        <option value="&lt;="  selected>&lt;=</option>
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
<td><input name="date2" type="text" size="13" value="31/12/9999"></td>

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

and <input name="orderedby" type="hidden" value="run">
arrange in 
  <select name="orderdir">
      <option value="asc">ascending</option>
      <option value="desc">descending</option>
  </select> order
<p><br />
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
