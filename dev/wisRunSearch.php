<html>

<head>
	<title>TileCal Commissioning Offline Shifts</title>
	<link href="./css/standard.css" type="text/css" rel="stylesheet" />
	<script language="JavaScript" src="wisRunSelectionScript.js"></script>
	<!--<script language="JavaScript" src="createIFrame.js"></script>-->
	<script type="text/javascript">
		function arrangeValues()
		{
			document.myForm.exp_run.value = document.myForm.opr_run.value.concat(document.myForm.run.value);
			document.myForm.ex2_run.value = document.myForm.opr_run2.value.concat(document.myForm.run2.value);
			document.myForm.exp_date.value = document.myForm.opr_date.value.concat(document.myForm.year.value, document.myForm.month.value, document.myForm.rday.value);
			document.myForm.ex2_date.value = document.myForm.opr_date2.value.concat(document.myForm.year2.value, document.myForm.month2.value, document.myForm.rday2.value);

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

<form name="myForm" action="wisRunSelection.php" method="post" onSubmit="arrangeValues();">

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
	<option value="&gt;" selected>&gt;</option>
	<option value="&lt;">&lt;</option>
	<option value="&gt;=">&gt;=</option>
	<option value="&lt;=">&lt;=</option>
	<!--<option>contains</option>-->
</select></td>
<td><input name="run" type="text" size="12" value="0060000"></td>

<td>date</td>
<td><select name="opr_date">
        <option value="=">=</option>
        <option value="&gt;" selected>&gt;</option>
        <option value="&lt;">&lt;</option>
        <option value="&gt;=">&gt;=</option>
	<option value="&lt;=">&lt;=</option>
        <!--<option>contains</option>-->
</select></td>
<td><input name="year" type="text" size="4" value="2000"></td>
<td><input name="month" type="text" size="2" value="05"></td>
<td><input name="rday" type="text" size="2" value="01"></td>
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
        <option value="&lt;" selected>&lt;</option>
        <option value="&gt;=">&gt;=</option>
        <option value="&lt;=">&lt;=</option>
        <!--<option>contains</option>-->
</select></td>
<td><input name="run2" type="text" size="12" value="9999999"></td>

<td>date</td>
<td><select name="opr_date2">
        <option value="=">=</option>
        <option value="&gt;">&gt;</option>
        <option value="&lt;" selected>&lt;</option>
        <option value="&gt;=">&gt;=</option>
        <option value="&lt;=">&lt;=</option>
        <!--<option>contains</option>-->
</select></td>
<td><input name="year2" type="text" size="4" value="9999"></td>
<td><input name="month2" type="text" size="2" value="12"></td>
<td><input name="rday2" type="text" size="2" value="31"></td>

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
