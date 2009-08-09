<?
//Taking the modules
$moduleString = "";
$runNumber = "";
$runType="";
$createdModule="";
$nonCreatedModule="";
if(!$HTTP_POST_VARS)
	$HTTP_POST_VARS = &$HTTP_GET_VARS;
	reset ($HTTP_POST_VARS);
while ( list($key, $val) = each($HTTP_POST_VARS))
{
	if ( $key=="moduleString" ) $moduleString = $val;  //getting the module name
	if ( $key=="runNumber" ) $runNumber = $val;  //getting the module name
	if ( $key=="runType" ) $runType = $val;  //getting the module name
	if (  $key=="createdModuleString") $createdModule = $val;
	if (  $key=="nonCreatedModuleString") $nonCreatedModule = $val;
}

include_once("TileCalBarrel.php");




//Create an instance of the TileCalBarrel
$barrel = new TileCalBarrel();
$barrel2 = new TileCalBarrel();
$barrel3 = new TileCalBarrel();
$barrel4 = new TileCalBarrel();

//Set the barrel name. Initializes internal properties
//of the barrel (chirality,...)
$barrel->SetName("LBA");
$barrel2->SetName("LBC");
$barrel3->SetName("EBA");
$barrel4->SetName("EBC");

//Define the number of wedges you want for your barrel
$barrel->wedges = 3;
$barrel2->wedges = 3;
$barrel3->wedges = 3;
$barrel4->wedges = 3;


//Set if you want to see the Sector Labels
//$barrel->SetSectorLabels();

//Set the barrel Legend depending on chirality
//$barrel->SetLegend();

//Use your own WAY of setting the colors
$color = $barrel->lightYellow;
$color1 = $barrel->darkGreen;
$color2 = $barrel->darkRed;
$color3 = $barrel->gray;
$color4 = $barrel->blue;
$statusColor = array($barrel->white,$barrel->green,$barrel->orange,$barrel->red,$barrel->black);

if(isset($_GET["draw"]))
{
	//Connect Tilecomm Database
	$mysqlCon = mysql_connect("atlasdev1.cern.ch","lodi","cOAnAd26")
		or die("cannot connect to database server atlasdev1 :(");
	mysql_select_db("tbanalysis", $mysqlCon);

	//Creating commentsArray
	// All module commented array
	$selectComment="SELECT  tcaRun.idAtlasId, tcaRun.idTestId, idatlas.code, testsID.testID FROM tcaRun, idatlas, testsID WHERE tcaRun.runNumber=$runNumber AND (LTRIM(comments) IS NOT NULL AND LTRIM(comments) <> \"\") AND testsID.id=tcaRun.idTestId AND idatlas.id= tcaRun.idAtlasId ;";
	$resComment = mysql_query($selectComment,$mysqlCon) or die("Failure: " . mysql_error());

	$countComment=  mysql_num_rows($resComment);

	$commentsArray= array();

	for($indexComment=0; $indexComment<$countComment;$indexComment++)
	{
		$rowComment = mysql_fetch_row($resComment);
		$tempString=  $rowComment[2];
		if($rowComment[3]<10)
			$tempString .= 0;
		$tempString .= $rowComment[3];
		array_push($commentsArray, $tempString);
	}
	mysql_free_result($resComment);
	$selectComment="SELECT  tcaRun.idAtlasId, tcaRun.idTestId, idatlas.code, testsID.testID, tcaRun.statusCommentsId FROM tcaRun, idatlas, testsID WHERE tcaRun.runNumber=$runNumber AND testsID.id=tcaRun.idTestId AND idatlas.id= tcaRun.idAtlasId ;";
	//echo "<H1>$selectComment</H1>";
	$resComment = mysql_query($selectComment,$mysqlCon) or die("Failure: " . mysql_error());
	$countComment=  mysql_num_rows($resComment);
	$statusCommentsArray = array();
	for($indexComment=0; $indexComment<$countComment;$indexComment++)
        {
                $rowComment = mysql_fetch_row($resComment);
		$tempString=  $rowComment[2];
                if($rowComment[3]<10)
                        $tempString .= 0;
                $tempString .= $rowComment[3];
                $tempArray=  array($tempString => $rowComment[4]);
                //print_r($tempArray);echo"<BR>";
		$statusCommentsArray= array_merge($statusCommentsArray, $tempArray);
        }
	//print_r($statusCommentsArray);

 	mysql_close($mysqlCon);

	for($i=1;$i<=DEF_NUM_MODULES;$i++)
	{
		$moduleNameTemp=sprintf("LBA%02u", $i);
		if(array_search($moduleNameTemp,$commentsArray) !==false)
			$barrel->SetModuleColor($i,3,$color4);
		else
			$barrel->SetModuleColor($i,3,$color3);

		if(isset($statusCommentsArray[$moduleNameTemp]))
			$barrel->SetModuleColor($i,2,$statusColor[$statusCommentsArray[$moduleNameTemp]]);
		else
			$barrel->SetModuleColor($i,2,$statusColor[0]);
		if(strstr($createdModule, $moduleNameTemp))
                        $barrel->SetModuleColor($i,1,$color1);
                else
                {
                        if(strstr($nonCreatedModule, $moduleNameTemp))
                                $barrel->SetModuleColor($i,1,$color2);
                        else{
                                $barrel->SetModuleColor($i,1,$color);
                                $barrel->SetModuleColor($i,2,$color);
                                $barrel->SetModuleColor($i,3,$color);
			}
                }

		$moduleNameTemp=sprintf("LBC%02u", $i);
		if(array_search($moduleNameTemp,$commentsArray) !==false)
			$barrel2->SetModuleColor($i,3,$color4);
		else
			$barrel2->SetModuleColor($i,3,$color3);
		if(isset($statusCommentsArray[$moduleNameTemp]))
                        $barrel2->SetModuleColor($i,2,$statusColor[$statusCommentsArray[$moduleNameTemp]]);
                else
                        $barrel2->SetModuleColor($i,2,$statusColor[0]);
		if(strstr($createdModule, $moduleNameTemp))
                        $barrel2->SetModuleColor($i,1,$color1);
                else
                {
                        if(strstr($nonCreatedModule, $moduleNameTemp))
                                $barrel2->SetModuleColor($i,1,$color2);
                        else{
                                $barrel2->SetModuleColor($i,1,$color);
                                $barrel2->SetModuleColor($i,2,$color);
                                $barrel2->SetModuleColor($i,3,$color);
                        }
                }

				$moduleNameTemp=sprintf("EBA%02u", $i);
		if(array_search($moduleNameTemp,$commentsArray) !==false)
			$barrel3->SetModuleColor($i,3,$color4);
		else
			$barrel3->SetModuleColor($i,3,$color3);
		if(isset($statusCommentsArray[$moduleNameTemp]))
                        $barrel3->SetModuleColor($i,2,$statusColor[$statusCommentsArray[$moduleNameTemp]]);
                else
                        $barrel3->SetModuleColor($i,2,$statusColor[0]);
		if(strstr($createdModule, $moduleNameTemp))
                        $barrel3->SetModuleColor($i,1,$color1);
                else
                {
                        if(strstr($nonCreatedModule, $moduleNameTemp))
                                $barrel3->SetModuleColor($i,1,$color2);
                        else{
                                $barrel3->SetModuleColor($i,1,$color);
                                $barrel3->SetModuleColor($i,2,$color);
                                $barrel3->SetModuleColor($i,3,$color);
                        }
                }
				$moduleNameTemp=sprintf("EBC%02u", $i);
		if(array_search($moduleNameTemp,$commentsArray) !==false)
			$barrel4->SetModuleColor($i,3,$color4);
		else
			$barrel4->SetModuleColor($i,3,$color3);
		if(isset($statusCommentsArray[$moduleNameTemp]))
                        $barrel4->SetModuleColor($i,2,$statusColor[$statusCommentsArray[$moduleNameTemp]]);
                else
                        $barrel4->SetModuleColor($i,2,$statusColor[0]);
		if(strstr($createdModule, $moduleNameTemp))
                        $barrel4->SetModuleColor($i,1,$color1);
                else
                {
                        if(strstr($nonCreatedModule, $moduleNameTemp))
                                $barrel4->SetModuleColor($i,1,$color2);
                        else{
                                $barrel4->SetModuleColor($i,1,$color);
                                $barrel4->SetModuleColor($i,2,$color);
                                $barrel4->SetModuleColor($i,3,$color);
                        }
                }

	}
}
//Commit all the chages before drawing
$barrel->Commit();
$barrel2->Commit();
$barrel3->Commit();
$barrel4->Commit();


//GET TO DRAW
if($_GET["draw"]=="1"){
    $barrel->Draw();
    exit;
}
else if($_GET["draw"]=="2"){
    $barrel2->Draw();
    exit;
}
else if($_GET["draw"]=="3"){
    $barrel3->Draw();
    exit;
}
else if($_GET["draw"]=="4"){
    $barrel4->Draw();
    exit;
}

function writeMap($name,&$barrel, $runNumber, $moduleString){
	for ($i=0;$i< DEF_NUM_MODULES; $i++){
 	   $moduleNumber="";
 	   if($i<9){$moduleNumber = "0";}
 	   $moduleNumber .= $i+1;
	   for ($w=0;$w<$barrel->wedges;$w++){
    	        echo '<';
	        echo 'area ';
	        echo 'shape="poly" ';
	        echo 'coords="';
		echo $barrel->coordinates[$i][$w];
		echo '" ';
		$moduleName = $name.$moduleNumber;
		//switch($w)
		//{
		//	case 1:
			if(strpos($moduleString,$moduleName )!==false)
				echo "href=\"javascript:retrieveComment($runNumber, '$moduleName');\" ";
			else
				 echo "href=\"javascript: commentBoxFunction();\" ";
		//	break;
		//	case 0:
		//		echo "href=\"#\" ";
		//	break;
		//	default:
		//}
		echo 'onMouseOver="pop0(';
		echo "'<center><u>";
		echo $name.$moduleNumber;
		echo "</u></center>'";
		echo ')" ';
		echo 'onMouseOut="disparaitre0()" ';
		echo '/>'."\n";
	   }

	}
}

?>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<link href="./css/standard.css" type="text/css" rel="stylesheet" />
<style type="text/css"  rel="stylesheet">
#top-bar span.left {
float: left;
       text-align: left;
       font-weight: bold;
color: #fff;
width: 49%;
}
span.right {
float: right;
       text-align: right;
       font-weight: bold;
color: #fff;
width: 49%;
}
</style>
<title>
Web Interface for Commissioning Monitoring
</title>
<script language="JavaScript" src="wisRunSelectionScript.js"></script>
<script type="text/javascript">
	function linkTo(URL){
		window.parent.location = URL;
	}

	function retrieveComment(runNumber, module)
	{
		var URL ="wisRetrieveComment.php?runNumber="+ runNumber;
		URL+="&module="+module;
		callToServer(URL);

		//setTimeout('matchWidth("header", "contents");matchWidth("top-bar", "header");',200);

	}
	function commentBoxFunction()
	{
		document.getElementById("commentBox").innerHTML = '';
	}
	function getDate()
	{
		var URL="wicomShowNtupleDate.php?runNumber=<?php echo $runNumber;?>&runType=<?php echo $runType;?>";
	//	alert(URL);
		//window.location=URL;
		callToServer(URL)
	}
	function getFocus()
	{
		window.focus();
	}
</script>
	<style type="text/css">
	div.legenda {
		position: fixed;
		bottom: 0;
		left: 0;
		width: 400px; }
	div.comentBox {
		position: fixed;
		bottom: 0;
		right: 20px;
		width: 300px; }
	</style>
<!--[if lte IE 7]>
<style type="text/css">
html, body
	{
	height: 100%;
	overflow: auto;
	}
div.legenda {
	position: absolute;
}

div.comentBox {
	position: absolute;
}
</style>
<![endif]-->
</head>
<body onLoad="getDate();">
<CENTER>
<div id="bulle" style="position: absolute;"></div>
<script language="javascript">
decal_x = 25;
decal_y = -15;
document.onmousemove = suivre_souris0;
var contenu
function pop0(contenu)
{
document.getElementById("bulle").innerHTML = "<table border='1'bordercolor='#C0C0C0'style='background-color: #fff2d2;'cellpadding='6' cellspacing='0'><tr><td><font color='#000000'face='verdana'size='1'><b>"+contenu+"</b></font></td></tr></table>";
}
function suivre_souris0(e)
{
if (navigator.appName=="Microsoft Internet Explorer")
{
var x = event.x + document.body.scrollLeft;    var y = event.y + document.body.scrollTop;
}
else
{
var x =  e.pageX;var y =  e.pageY;
}
document.getElementById("bulle").style.left = x + decal_x; document.getElementById("bulle").style.top  = y + decal_y;
}
function disparaitre0()
{
document.getElementById("bulle").innerHTML = '';
}

</script>


<img usemap="#map1" src="wisNtuplesPerModule.php?name=LBA&draw=1" border="0">
<img usemap="#map2" src="wisNtuplesPerModule.php?name=LBC&draw=2" border="0">
<br>
<img usemap="#map3" src="wisNtuplesPerModule.php?name=EBA&draw=3" border="0" >
<img usemap="#map4" src="wisNtuplesPerModule.php?name=EBC&draw=4" border="0">

<map name="map1">
<?
writeMap("LBA",$barrel, $runNumber, $moduleString);
?>
</map>
<map name="map2">
<?
writeMap("LBC",$barrel2, $runNumber, $moduleString);
?>
<map name="map3">
<?
writeMap("EBA",$barrel3, $runNumber, $moduleString);
?>
<map name="map4">
<?
writeMap("EBC",$barrel4, $runNumber, $moduleString);
?>
</map>
</body>
</html>
