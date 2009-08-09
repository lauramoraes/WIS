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
			$htmlfilename = $_POST["htmlfilename"];

		ob_implicit_flush();
		ob_end_flush();
	
		$isRunning = 0;
		passthru("./wisIsRunning $pid", $isRunning);

		if($isRunning)
		{
			echo "<body>";
			echo "<html>";
			echo "<script>";
        		echo "setTimeout(\"document.getElementById('myForm').submit()\", 10*1000)";
			echo "</script>";
			echo "</head>";
			echo "<body>";
			echo "<form name=\"myForm\" id=\"myForm\" action=\"wisIsIntpedRunning.php\" method=\"post\">";
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
			passthru("./wisEraseLines $htmlfilename", $retCode);
			echo "<html>";
			echo "<body>";
			$con = mysql_connect("atlasdev1.cern.ch", "lodi", "cOAnAd26")
					or die("cannot connect to database server :(");
			mysql_select_db("tbanalysis", $con);
			$select = "SELECT * FROM wisIntegratorMacro WHERE filename='$filename".".root';"; //LIMIT 0,10";
			$res = mysql_query($select)
					or die("query failed :(");
			$nrows = mysql_num_rows($res);
			$nfields = mysql_num_fields($res);
			$content = file_get_contents($htmlfilename);
			$content = addslashes($content);
			$select = "UPDATE wisIntegratorMacro SET results='$content' WHERE filename='$filename".".root';";
			$res = mysql_query($select);
			mysql_close();
			echo "<script>";
			echo "	var td = parent.document.getElementById('$tdId');";
			echo "var b = td.getElementsByTagName('B')[0];";
			echo "if (b)";
			echo "	td.removeChild(b);";
			echo "var br = td.getElementsByTagName('BR')[0];";
			echo "if (br)";
			echo "	td.removeChild(br);";
			?>
			parent.resultFile[<?php echo "'$tdId'"; ?>] = '<?php echo "wisSeeMacroResult.php?filename=$filename"; ?>';
			link = td.getElementsByTagName('A')[0]; // link para os resultados
			if (link)
				link.style.display = 'block';
			else
			{
				link = parent.document.createElement('A');
				link.setAttribute('href', '#');
				link.onclick = parent.showMacroResult;
				//link.onmouseover = parent.showStatusBarResultMsg;
				//link.onmouseout = parent.eraseStatusBarMsg;

				if (link.captureEvents) link.captureEvents(Event.CLICK); // for Netscape 4(?)
				var text = parent.document.createTextNode('Results');
				link.appendChild(text);
				td.appendChild(link);
			}
		<?php
			echo "</script>";
			echo "</body>";
			echo "</html>";
		}
	?>
