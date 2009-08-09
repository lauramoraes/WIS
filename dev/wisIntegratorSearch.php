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
		<h2>Integrator Outputs</h2>
		<font size="+1">Use this form to search for more integrator tests.</font><br />


<font size=+1>Q</font>uery:
<i>set start and stop dates and module name, then press <b>submit</b> button!</i>
<br><br/>
<form name="search" method="post" action="wisIntegratorSelection.php">
<table class="main">
	<tr>
		<td colspan="4" bgcolor="#cccccc"><center><b>Date<i>(yyyy/mm/dd)</i></b></center></td>
		<td bgcolor="#cccccc">&nbsp;</td>
		<td bgcolor="#cccccc"><center><b>ModuleName</b></center></td>
	</tr>
	<tr>
		<td>Between</td>
		<td><input type="text" name="ayear" size ="4" MAXLENGTH="4">/<input type="text" name="amouth" size="2" MAXLENGTH="2">/
		<input type="text" name="aday" size="2" MAXLENGTH="2"></td>
		<td>and</td>
		<td><input type="text" name="byear"  size ="4" MAXLENGTH="4">/<input type="text" name="bmouth" size="2" MAXLENGTH="2">/
		<input type="text" name="bday" size="2" MAXLENGTH="2"></td>
		<td><select name="operator"><option selected>or</option><option>and</option></select></td>
	</tr>
</table>
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
