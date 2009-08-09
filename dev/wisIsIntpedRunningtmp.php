	<?php
		if (!$_POST)
			$_POST = &$_GET;
		reset($_POST);

		if (isset($_POST["pid"]))
			$pid = $_POST["pid"];
		if (isset($_POST["tdId"]))
			$tdId = $_POST["tdId"];
		if (isset($_POST["filename"]))
			$filename = $_POST["filename"];

		if (isset($_POST["htmlfilename"]))
			$filename = $_POST["htmlfilename"];
		ob_implicit_flush();
		ob_end_flush();
	
		$isRunning = 0;
		passthru("./wisIsRunning $pid", $isRunning);

		if($isRunning)
		{
			echo "<html>";
			echo "<body>";
			echo "<script>";
        		echo "setTimeout(\"document.getElementById('myForm').submit()\", 10*1000)";
			echo "</script>";
			echo "<form name=\"myForm\" id=\"myForm\" action=\"wisIsRunning.php\" method=\"post\">";
			echo "<input type=\"hidden\" name=\"tdId\" value=\"$tdId\" />\n";
			echo "<input type=\"hidden\" name=\"filename\" value=\"$filename\" />\n";
			echo "<input type=\"hidden\" name=\"pid\" value=\"$pid\" />\n";
			echo "<input type=\"hidden\" name=\"htmlfilename\" value=\"$htmlfilename\" />\n";
			echo "</form>";
			echo "</body>";
			echo "</html>";
		}
		else
		{
			echo "<html>";
			echo "<body>";
				
			echo "<script>";
			echo "alert('CABOOOOOOOOOOOOOOOOOOOOOO!');"
			/*$con = mysql_connect("atlasdev1.cern.ch", "lodi", "cOAnAd26")
					or die("cannot connect to database server :(");
			mysql_select_db("tbanalysis", $con);
			$select = "SELECT * FROM wisIntegratorMacro WHERE filename='$filename'"; //LIMIT 0,10";
			$res = mysql_query($select)
					or die("query failed :(");
			$nrows = mysql_num_rows($res);
			$nfields = mysql_num_fields($res);
			$content = file_get_contents($filename);
			$content = addslashes($content);
			echo"alert(\"UPDATE wisMacroResult SET Data='$content' WHERE filename='$filename'\");";
			$select = "UPDATE wisMacroResult SET Data='$content' WHERE filename='$filename'";
			$res = mysql_query($select);
			mysql_close();
			
			echo "	var td = parent.document.getElementById('$tdId');";
			echo $tdId;
			echo "td.innerHTML=\"\";";*/
			echo "</script>";
			echo "</body>";
			echo "</html>";
		}
	?>
