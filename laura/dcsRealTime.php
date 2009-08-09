<?
//Taking the modules
if(!$HTTP_POST_VARS)
	$HTTP_POST_VARS = &$HTTP_GET_VARS;
	reset ($HTTP_POST_VARS);
while ( list($key, $val) = each($HTTP_POST_VARS))
{
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

//Use your own WAY of setting the colors
$color1 = $barrel->gray;
$color2 = $barrel->green;
$color3 = $barrel->red;


//Set if you want to see the Sector Labels
//$barrel->SetSectorLabels();

//Set the barrel Legend depending on chirality
//$barrel->SetLegend();

if(isset($_GET["draw"]))
{

	for($i=1;$i<=DEF_NUM_MODULES;$i++)
	{
		//200 Volts control
		$barrel->SetModuleColor($i,1,$color1);
		$barrel2->SetModuleColor($i,1,$color1);
		$barrel3->SetModuleColor($i,1,$color1);
		$barrel4->SetModuleColor($i,1,$color1);
		//Mb e DIG bricks(LVPS)
		$barrel->SetModuleColor($i,2,$color1);
		$barrel2->SetModuleColor($i,2,$color1);
		$barrel3->SetModuleColor($i,2,$color1);
		$barrel4->SetModuleColor($i,2,$color1);
		//HV bricks (LVPS)
		$barrel->SetModuleColor($i,3,$color1);
		$barrel2->SetModuleColor($i,3,$color1);
		$barrel3->SetModuleColor($i,3,$color1);
		$barrel4->SetModuleColor($i,3,$color1);

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

function writeMap($name,&$barre){
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
		echo "href=\"#\" ";
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
<title>
DCS Web System
</title>
<script language="JavaScript" src="wisRunSelectionScript.js"></script>
</head>
<body>
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


<img usemap="#map1" src="wisNtuplesPerModule.phpt" border="0">
<img usemap="#map2" src="wisNtuplesPerModule.php" border="0">
<br>
<img usemap="#map3" src="wisNtuplesPerModule.php" border="0" >
<img usemap="#map4" src="wisNtuplesPerModule.php" border="0">
<BR>
</CENTER>


<map name="map1">
<?
writeMap("LBA",$barrel);
?>
</map>
<map name="map2">
<?
writeMap("LBC",$barrel2);
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
