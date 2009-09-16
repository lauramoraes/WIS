<html>
<body>
<?php	

	/*$con = mysql_connect("atlasdev1.cern.ch","lodi","cOAnAd26")
                        or die("cannot connect to database server atlasdev1 :(");

        mysql_select_db("tbanalysis", $con);*/
  $con = ocilogon("ATLAS_TILECOM", "X#ep!zu75", "INTR")
			or die("cannot connect to database server INTR :(");
	
	$path = "/work/commissioning/team4/integrator/offline/";
	$dir = opendir($path);
	while ($filename = readdir($dir))
	{	
		$explodedString = explode("_", $filename);
		//selecting only the default filenames
		if((count($explodedString)!= 5))
			continue;
		//taking data from the filename and this data is atributed to variables
		list(, $prefix,$moduleName,$rawdate,$rawhour) = $explodedString;
		if($prefix != 'peds')
			continue;
		//taking a time formatting
		list($hour,,)= explode(".", $rawhour);
		//formating YYYY-MM-DD HH:MM:SS
		$completeDate = $rawdate[0].$rawdate[1].$rawdate[2].$rawdate[3];
		$completeDate = $completeDate."-".$rawdate[4].$rawdate[5];
		$completeDate = $completeDate."-".$rawdate[6].$rawdate[7]." ";
		$completeDate = $completeDate.$hour[0].$hour[1].":";
		$completeDate = $completeDate.$hour[2].$hour[3].":00";
		$query=" SELECT * from wisIntegratorMacro where filename=\"".$filename."\";";
		//$result=mysql_query($query) or die('Invalid query: ' . mysql_error());
		$result = ociparse($con, $query);
		ociexecute($result, OCI_DEFAULT) or die("FAILURE: " . ocierror($result));
		ocisetprefetch($result, 10000);
		while(ocifetch($result));
		//if(mysql_num_rows($result)==0)
		if(ocirowcount($result)==0)
		{
			$query=" INSERT INTO wisIntegratorMacro (filename, date, moduleName) VALUES ('". $filename."', '".$completeDate."', '".$moduleName."');";
			//mysql_query($query) or die('Invalid query: ' . mysql_error());
			$result = ociparse($con, $query);
			ociexecute($result, OCI_DEFAULT) or die("FAILURE: " . ocierror($result));
		}
	}

	closedir($dir);
	
	OCIFreeStatement($result);
	ocilogoff($con);
	
?>
</body>
<html>
