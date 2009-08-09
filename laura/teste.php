<html>
<head>
</head>
<body>
<pre>
<?php
$time = time();
$filename = "tmp/".$time.".tmp";
passthru("./wisCreateFile \"$filename\" ", $retCode);
while ($retCode != 0)
{
        $time++;
        $filename = "tmp/".$time.".tmp";
        passthru("./wisCreateFile \"$filename\" ", $retCode);
}

$command = "./wisReadCastorDir 61782  > $filename";
passthru($command, $retCode);

$rootArray= file($filename);
print_r($rootArray);

$time2 = time()- $time;
echo"Time to fill the array ".$time2." seconds";
?> 
</pre>
</body>
</html>
