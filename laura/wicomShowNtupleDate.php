<html>
<head>
<link href="./css/standard.css" type="text/css" rel="stylesheet" /
</head>
<body>
<div id="content">
<?php

$runNumber = "";
$runType="";
if(!$HTTP_POST_VARS) $HTTP_POST_VARS = &$HTTP_GET_VARS;

  reset ($HTTP_POST_VARS);

while ( list($key, $val) = each($HTTP_POST_VARS))
{
        if ( $key=="runNumber" ) $runNumber = $val;  //getting the module name
        if ( $key=="runType" ) $runType = $val;  //getting the module name
}

if($runNumber!= "" && $runType!="")
{
	$command = "./wicomShowNtupleDate $runNumber $runType";
	passthru($command, $retcode);
}
?>
</div>
<div id="teste">
</div>
<script>
	var div1=document.getElementById("content")
	var div2= parent.document.getElementById("dateBox");
	div2.innerHTML=div1.innerHTML;
	
</script>
</body>
</html>
