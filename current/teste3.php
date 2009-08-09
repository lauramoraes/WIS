<html>
<head>
</head>
<body>
<?php
/*
if (DIRECTORY_SEPARATOR=='/')
  $absolute_path = dirname(__FILE__).'/';
else
  $absolute_path = str_replace('\\\\', '/', dirname(__FILE__)).'/';

echo  $absolute_path;
		echo"<br>";
                $time = time();
                $filename = "/afs/cern.ch/user/t/tilecom/public/www/WIS/dev/tmp/".$time.".html";
		
	//$command = "/afs/cern.ch/user/t/tilecom/public/www/WIS/dev/wisCreateFile \"$filename\" > /dev/null";
        $command = "whoami > ./tmp/teste";
	echo $command;        
		echo"<br>";
	$teste=passthru($command , $retCode);
	echo $teste;	
	echo $retCode;
		echo"<br>";
	$command = "/afs/cern.ch/user/t/tilecom/public/www/WIS/dev/wisCreateFile \"$filename\"";
		echo"<br>";
	echo $command;        
		echo"<br>";
	passthru($command , $retCode);
		echo $retCode;
		echo"<br>";
		echo $filename;
	system($command , $retCode);

	echo "<br>More One test:<br><pre>";
	$command = "ldd /afs/cern.ch/user/t/tilecom/public/www/WIS/dev/wisCreateFile";
	$teste = passthru($command, $retcode);
	echo "</pre>Return value: ";
	echo $retcode;

        echo "<br>And more test:<br><pre>";
        $command = "ldd /afs/cern.ch/user/t/tilecom/public/www/WIS/dev/wisExistFile";
        $teste = passthru($command, $retcode);
        echo "</pre>Return value: ";
        echo $retcode;
	
	echo "<br>++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++<br>";
        echo "<br>And more more test:<br><pre>";
	putenv("LD_LIBRARY_PATH=/afs/cern.ch/sw/lcg/external/root/5.06.00/slc3_gcc323/root/lib");
        $command = "ldd /afs/cern.ch/user/t/tilecom/public/www/WIS/dev/macro/TTileCommRun/checkCRC_DMU_BCID 2>&1";
        echo $command;
	$teste = passthru($command, $retcode);
	echo "</pre>Return value: ";
        echo $retcode;
	echo "<br>++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++<br>";
	echo "<br>++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++<br>";
	echo "<br>And more more test:<br><pre>";
	$command="ldd";
	$argument= "/afs/cern.ch/user/t/tilecom/public/www/WIS/dev/macro/TTileCommRun/checkCRC_DMU_BCID";
	$teste = exec($command, $argument, $retcode);
	echo $teste;
	 echo "</pre>Return value: ";
	echo $retcode;

	echo "<br>++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++<br>";
        echo "<br>And more more test:<br><pre>";
	echo exec('whoami')."<br>";
        $command = "ls /afs/cern.ch/sw/lcg/external/root/5.06.00/slc3_gcc323/root/lib/";
        echo $command;
	$teste = passthru($command, $retcode);
	echo "</pre>Return value: ";
        echo $retcode;*/
  
      echo "<br>And more more test:<br><pre>";


        $command = "./connect.sh ls";
	echo exec('whoami')."<br>";
	echo exec('pwd')."<br>";
        echo $command."<br>";
	$teste = passthru($command, $retcode);
	echo "</pre>Return value: ";
        echo $retcode;
      echo "<br>And more more test:<br><pre>";


        $command = "./connect.sh ./wisExecuteInWISFolder wisCreateFile teste3000";
	echo exec('whoami')."<br>";
	echo exec('pwd')."<br>";
        echo $command."<br>";
	$teste = passthru($command, $retcode);
	echo "</pre>Return value: ";
        echo $retcode;
        
      echo "<br>And more more test:<br><pre>";


        $command = "./connect.sh ./wisExecuteInWISFolder testeLogin";
        echo $command."<br>";
	$teste = passthru($command, $retcode);
	echo "</pre>Return value: ";
        echo $retcode;
	echo "<br>And more more test:<br><pre>";
        $command = "./connect.sh ./wisExecuteInWISFolder wisCheckCRCandBCID tmp/1156960003.html tmp/1156960003.tmp 61220 Phys";
	echo exec('whoami')."<br>";
	echo exec('pwd')."<br>";
        echo $command."<br>";
	$teste = passthru($command, $retcode);
	echo "</pre>Return value: ";
        echo $retcode;
	
	echo "<br>And more more test:<br><pre>";
        $command = " ./connect.sh ./wisExecuteInWISFolder wisCheckCRCandBCID tmp/1156964670.html tmp/1156964670.tmp 61263 Phys ";
	echo exec('whoami')."<br>";
	echo exec('pwd')."<br>";
        echo $command."<br>";
	$teste = passthru($command, $retcode);
	echo "</pre>Return value: ";
        echo $retcode;
	echo "<br>And more more test:<br><pre>";
        $command = " ./connect.sh ./wisExecuteInWISFolder felipe wisCheckCRCandBCID tmp/1156969708.html tmp/1156969708.tmp 61261 MonoCis";
	echo exec('whoami')."<br>";
	echo exec('pwd')."<br>";
        echo $command."<br>";
	$teste = passthru($command, $retcode);
	echo "</pre>Return value: ";
        echo $retcode;
?>
</body>
</html>

