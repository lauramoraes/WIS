<?
include_once("TileCalBarrel.php");

//Create an instance of the TileCalBarrel
$barrel = new TileCalBarrel();
$barrel2 = new TileCalBarrel();

//Set the barrel name. Initializes internal properties 
//of the barrel (chirality,...)
$barrel->SetName("LBA");
$barrel2->SetName("LBC");

//Define the number of wedges you want for your barrel
$barrel->wedges = 1;
$barrel2->wedges = 1;


//Set if you want to see the Sector Labels
//$barrel->SetSectorLabels();

//Set the barrel Legend depending on chirality
//$barrel->SetLegend();

//Use your own WAY of setting the colors
$color = $barrel->lightBlue;
for($i=1;$i<=DEF_NUM_MODULES;$i++){
    $barrel->SetModuleColor($i,1,$color);
    $barrel2->SetModuleColor($i,1,$color);

    
}
//Commit all the chages before drawing
$barrel->Commit();
$barrel2->Commit();


//GET TO DRAW
if($_GET["draw"]=="1"){
    $barrel->Draw();
    exit;
}
else if($_GET["draw"]=="2"){
    $barrel2->Draw();
    exit;
}

function writeMap($name,&$barrel){
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
  	        echo 'href="javascript:linkTo(\'http://atlasmonitor.web.cern.ch/atlasmonitor/QCSheetListByModOrRun.jsp?__searchType=byMod';
		  echo '&__module=';
		  echo $name;
		  echo '&__moduleNumber=';
		  echo $moduleNumber;
		  echo '\')" ';
      	  echo 'onMouseOver="pop0(';
      	  echo "'<center>";
		  echo $name;
      	  echo $moduleNumber;
      	  echo "</center>'";
      	  echo ')" ';
        	  echo 'onMouseOut="disparaitre0()" ';
              echo '/>'."\n";
	   }
	    
	}
}
?>
<html>
<head>
<meta http-equiv="Content-Language" content="es">
</head>
<script type="text/javascript">
	function linkTo(URL){
		window.parent.location = URL;
	}
</script>
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
}</script>

<img usemap="#map1" src="drawBarrel.php?name=LBA&draw=1" border="0" />
<img usemap="#map2" src="drawBarrel.php?name=LBC&draw=2" border="0" />

<map name="map1">
<?
writeMap("LBA",$barrel);
?>
</map>
<map name="map2">
<?
writeMap("LBC",$barrel2);
?>
</map>
</CENTER>
</body>
</html>

