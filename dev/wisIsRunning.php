<html>

<head>
	<?php
		
		if (!$_POST)
			$_POST = &$_GET;
		reset($_POST);
		
		if (isset($_POST["pid"]))
			$pid = $_POST["pid"];

		if (isset($_POST["filename"]))
			$filename = $_POST["filename"];
	
		if (isset($_POST["pidFilename"]))
			$pidFilename = $_POST["pidFilename"];
		
		if (isset($_POST["tdId"]))
			$tdId = $_POST["tdId"];

		if (isset($_POST["moduleName"]))
			$moduleName = $_POST["moduleName"];

		if (isset($_POST["macroName"]))
			$macroName = $_POST["macroName"];

		if (isset($_POST["runNumber"]))
			$runNumber = $_POST["runNumber"];

		ob_implicit_flush();
		ob_end_flush();
		
		$isRunning = 0;
		passthru("./wisIsRunning $pidFilename > /dev/null", $isRunning);
		if ($isRunning)
		{
?>
	<script>
	setTimeout("document.getElementById('myForm').submit()", 10*1000);
	</script>
</head>

<body>
	<form name="myForm" id="myForm" action="wisIsRunning.php" method="post">
		<?php
		
		echo "<input type=\"hidden\" name=\"tdId\" value=\"$tdId\" />\n";
		echo "<input type=\"hidden\" name=\"filename\" value=\"$filename\" />\n";
		echo "<input type=\"hidden\" name=\"pidFilename\" value=\"$pidFilename\" />\n";
		echo "<input type=\"hidden\" name=\"pid\" value=\"$pid\" />\n";
		echo "<input type=\"hidden\" name=\"moduleName\" value=\"$moduleName\" />\n";
		echo "<input type=\"hidden\" name=\"macroName\" value=\"$macroName\" />\n";
		echo "<input type=\"hidden\" name=\"runNumber\" value=\"$runNumber\" />\n";

		?>
	</form>
</body>

</html>

<?php
		}
		else
		{
		//	passthru("./wisEraseLines $filename", $retCode);
			
			//Now, save XML file in database.
			//server: atlasdev1.cern.ch / user: lodi / password: cOAnAd26 / database: commAnalysis
			//table: wisMacroResult / columns: Run_Number, Macro_Name, Module_Name, Data (BLOB)
			$con = mysql_connect("atlasdev1.cern.ch", "lodi", "cOAnAd26")
				or die("cannot connect to database server :(");
			mysql_select_db("commAnalysis", $con);
			$select = "SELECT * FROM wisMacroResult WHERE Run_Number=$runNumber AND Macro_Name='$macroName' AND Module_Name='$moduleName'"; //LIMIT 0,10";
			$res = mysql_query($select)
				or die("<script>alert(\"query failed :(\n SELECT * FROM wisMacroResult WHERE Run_Number=$runNumber AND Macro_Name='$macroName' AND Module_Name='$moduleName'\");</script>");
			$nrows = mysql_num_rows($res);
			$nfields = mysql_num_fields($res);
			
			
			//$fp = fopen($filename, 'r');
			//$content = fread($fp, filesize($filename));
			//$content = addslashes($content);
			//fclose($fp);
			$content = file_get_contents($filename);
			$content = addslashes($content);
			if ($nrows > 0)
			{
				//echo "alert('Resutado já existe! Deve ser atualizado: UPDATE.');\n";
				//$select = "UPDATE wisMacroResult SET Data=\'XMLContent\' WHERE Run_Number=$runNumber AND Macro_Name=\'$macroName\' AND Module_Name=\'$moduleName\'";
				//echo "alert('$select');\n";
				$select = "UPDATE wisMacroResult SET Data='$content' WHERE Run_Number=$runNumber AND Macro_Name='$macroName' AND Module_Name='$moduleName'";
			}
			else
			{
	//			//echo "alert('Resutado não existe! Deve ser inserido: INSERT.');\n";
				//$select = "INSERT INTO wisMacroResult (Run_Number, Macro_Name, Module_Name, Data) VALUES($runNumber, \'$macroName\', \'$moduleName\', \'XMLContent\')";
				//echo "alert('$select');\n";
				$select = "INSERT INTO wisMacroResult (Run_Number, Macro_Name, Module_Name, Data) VALUES($runNumber, '$macroName', '$moduleName', '$content')";
			}
			echo " $content";		
			$res = mysql_query($select);
			if ($res == FALSE)
			{
	//			echo("alert('query failed :( Error: " . addslashes(mysql_error($con)) . "');\n");
				//echo "</script>";
				//echo "FALHOU!\n";
				//echo "/*\n$content\n*/";
				//die("");
			}

			mysql_close();

			echo "</script>";

?>
	<script>
	parent.processesRunning = parent.processesRunning - 1;
	//alert('Running now: ' + parent.processesRunning);

	// - pega a celula da tabela onde vai ficar o link para o resultado da macro
	var td = parent.document.getElementById('<?php echo "$tdId"; ?>');

	// - remove o texto "Running macro..."
	var b = td.getElementsByTagName('B')[0];
	if (b)
		td.removeChild(b);
	var br = td.getElementsByTagName('BR')[0];
	if (br)
		td.removeChild(br);

	// - poe um link para os resultados, que serah basicamente um link para o $filename, abrindo em outra janela.
	//parent.resultFile['<?php echo "$tdId"; ?>'] = '<?php echo "$filename"; ?>'; 
	var tdId = '<?php echo "$tdId"; ?>';
	var moduleName = '<?php echo "$moduleName"; ?>';
	parent.resultFile[tdId] = '<?php echo "wisSeeMacroResult.php?runNumber=$runNumber&macroName=$macroName"; if ($moduleName == "all") echo "&moduleName=$moduleName"; ?>';
	link = td.getElementsByTagName('A')[0]; // link para os resultados
	if (link)
	{
		link.style.display = 'block';
		//link.style.color = '#CC0000';
		
	}
	else
	{
		link = parent.document.createElement('A');
		link.setAttribute('href', '#');
		link.onclick = parent.showMacroResult;
		link.onmouseover = parent.showStatusBarResultMsg;
		link.onmouseout = parent.eraseStatusBarMsg;

		if (link.captureEvents) link.captureEvents(Event.CLICK); // for Netscape 4(?)
		var text = parent.document.createTextNode(parent.cell[tdId]['macroTitle'] + ' Results');
		link.appendChild(text);
		//link.style.color = '#CC0000';
		td.appendChild(link);
	}
	if (moduleName != 'all')
	{
		var select = parent.document.getElementById(tdId + 'Select');
		if (!select)
		{
			select = parent.document.createElement('SELECT');
			select.setAttribute('id', tdId + 'Select');
			//select.options.length = 0;
			td.appendChild(parent.document.createTextNode(' '));
			td.appendChild(select);
		}
		
		var i, found = -1;
		if (select.options.length)
		{
			for (i = 0; i < select.options.length; i++)
			{
				if (select.options[i].value == moduleName)
					found = i;
			}
		}
		//alert('length = ' + select.options.length + '; i = ' + i + '; moduleName = ' + moduleName);
		if (found >= 0)
		{
			select.selectedIndex = found;
			//alert ('value = ' + select.options[i].value);
		}
		else
		{
			var option = parent.document.createElement('OPTION');
			option.appendChild(document.createTextNode(moduleName));
			option.setAttribute('value', moduleName);
			select.appendChild(option);
			select.selectedIndex = select.options.length - 1;
		}
	}
	
	</script>
</head>

<body>
</body>

</html>
<?php  unlink($filename);
} ?>
