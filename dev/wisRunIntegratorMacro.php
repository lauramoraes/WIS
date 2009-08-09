<html>

<head>

	<script>
	<?php
		if (!$_POST)
			$_POST = &$_GET;
		reset($_POST);
		
		if (isset($_POST["filename"]))
			$filename = $_POST["filename"];
		
		if (isset($_POST["rowNumber"]))
		{
			$rowNumber = $_POST["rowNumber"];
			$tdId= "result".$rowNumber;
		}
		
		ob_implicit_flush();
		ob_end_flush();
	?>
	//setTimeout("document.getElementById('myForm').submit()", 10*1000);
	</script>

</head>

<body>

	<form name="myForm" id="myForm" action="wisIsIntpedRunning.php" method="post">


	<?php
		
		echo "<input type=\"hidden\" name=\"tdId\" value=\"$tdId\" />\n";

                $time = time();
                $htmlfilename = "tmp/".$time.".html";
                passthru("./wisCreateFile \"$htmlfilename\" > /dev/null", $retCode);
                echo "<script>alert('pasthru 0 - ".$retCode."');</script>";
		/*while ($retCode != 0)
                {
                        $time++;
                        $htmlfilename = "tmp/".$time.".html";
                        passthru("./wisCreateFile \"$htmlfilename\" > /dev/null", $retCode);
                }*/

                echo "<input type=\"hidden\" name=\"htmlfilename\" value=\"$htmlfilename\" />\n";
                echo "<input type=\"hidden\" name=\"filename\" value=\"$filename\" />\n";

                $time = time();
                $pidFilename = "tmp/".$time.".tmp";
                passthru("./wisCreateFile \"$pidFilename\" > /dev/null", $retCode);
		echo"<script>";
		echo"alert('pasthru 1 - ".$retCode."');";
		echo"</script>";
                /*while ($retCode != 0)
                {
                        $time++;
                        $pidFilename = "tmp/".$time.".tmp";
                        passthru("./wisCreateFile \"$pidFilename\" > /dev/null", $retCode);
                }*/
		$command = "./connect.sh /afs/cern.ch/user/t/tilecom/public/www/DCS/dev/wisIntped $filename $pidFilename $htmlfilename";
	echo"<script>prompt(' ' ,'$command')</script>";
	//	passthru($command, $retCode);
		echo"<script>";
		echo"alert('pasthru 2 - ".$retCode."');";
		echo"</script>";
		$pid = file_get_contents($pidFilename);
                echo "<input type=\"hidden\" name=\"pid\" value=\"$pid\" />\n";
		passthru("./wisDeleteFile $pidFilename > /dev/null", $retCode);
		echo"<script>";
		echo"alert('pasthru 2 - ".$retCode."');";
		echo"</script>";
	?>
</form>
</body>

</html>

