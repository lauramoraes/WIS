<?

//Add this line to run in the Winservices central servers. Dinamically loads the GD library to PHP.
//dl("php_gd2.dll");

//Global colors
define ("COLOR_BLUE",      "#0000FF");
define ("COLOR_ORANGE",    "#FF8000");
define ("COLOR_LIGHTBLUE", "#0000FF");
define ("COLOR_BLACK",     "#000000");
define ("COLOR_WHITE",     "#FFFFFF");
define ("COLOR_GREEN",     "#00FF00");
define ("COLOR_YELLOW",    "#FFFF00");
define ("COLOR_RED",       "#FF0000");
define ("COLOR_DARKRED",   "#990000");
define ("COLOR_DARKGREEN", "#006600");

//Global size
define ("IMAGE_HEIGHT",    350);
define ("IMAGE_WIDTH",     350);

//Global font color
define ("DEFAULT_FONT",    5);

//Globals
define ("DEF_NUM_SECTORS", 8);
define ("DEF_NUM_WEDGES",  1);
define ("DEF_NUM_MODULES", 64);
define ("DEF_DEG_OFFSET",  90);
define ("DEF_TXT_OFFSETX",-12);
define ("DEF_TXT_OFFSETY",-8);

function Deg2X($r,$g){
	return $r*sin(deg2rad($g));
}

function Deg2Y($r,$g){
	return -$r*cos(deg2rad($g));
}

function hex2rgb($str,$component){
	$offset = 0;
	$color = 0;
	
	if(substr($str,0,1)=="#"){
		$offset = 1;
	}
	if(strtoupper($component)=="R"){
		$color = 0;
	}else if(strtoupper($component)=="G"){
		$color = 2;
	}else if(strtoupper($component)=="B"){
		$color = 4;
	}
	
	$ret = "0x".substr($str,$offset+$color,2);
	
	//echo "offset:".$offset." color:".$color." string:".$ret." ==>".intval($ret,16)."<br>";
	
	return intval($ret,16);
}

function imagelinethick($image, $x1, $y1, $x2, $y2, $color, $thick = 1) 
{
   /* this way it works well only for orthogonal lines
   imagesetthickness($image, $thick);
   return imageline($image, $x1, $y1, $x2, $y2, $color);
   */
   if ($thick == 1) {
       return imageline($image, $x1, $y1, $x2, $y2, $color);
   }
   $t = $thick / 2 - 0.5;
   if ($x1 == $x2 || $y1 == $y2) {
       return imagefilledrectangle($image, round(min($x1, $x2) - $t), round(min($y1, $y2) - $t), round(max($x1, $x2) + $t), round(max($y1, $y2) + $t), $color);
   }
   $k = ($y2 - $y1) / ($x2 - $x1); //y = kx + q
   $a = $t / sqrt(1 + pow($k, 2));
   $points = array(
       round($x1 - (1+$k)*$a), round($y1 + (1-$k)*$a),
       round($x1 - (1-$k)*$a), round($y1 - (1+$k)*$a),
       round($x2 + (1+$k)*$a), round($y2 - (1-$k)*$a),
       round($x2 + (1-$k)*$a), round($y2 + (1+$k)*$a),
   );    
   imagefilledpolygon($image, $points, 4, $color);
   return imagepolygon($image, $points, 4, $color);
}

function imagestringcentered ($img,$font,$x,$y,$text,$color) {
 
	$cy = $y - imagefontheight($font)/2;
	$cx = $x - strlen($text) * imagefontwidth($font)/2;
	imagestring($img,$font,$cx,$cy,$text,$color);
}


class TileCalBarrel{

	//Class variables
	//Image resource
	var $image;
	//Colors
	var $blue;
	var $lightBlue;
	var $white;
	var $orange;
	var $green;
	var $black;
	var $yellow;
	var $red;
	var $lightYellow;
	var $darkGreen;
	var $darkRed;
	//Barrel Name
	var $name;
	//Number of Wedges per module
	var $wedges;
	//Border
	
	//Center
	var $centerX;
	var $centerY;
	
	//Radius
	var $innerRadius;
	var $outerRadius;
	
	//Chirality (1 = ClockWise (LBC,EBC), -1 = AntiClockWise (LBA,EBA))
	var $chirality;
	
	//font
	var $font;
	
	//Modules
	var $module;
	
	//Coordinates
	var $coordinates;
	
	//Constructor function for PHP>5.0
	function __construct(){
		
		//Define the image
		$this->image = imagecreate(IMAGE_WIDTH,IMAGE_HEIGHT); 
		
		//Define the colors: The first one will be the image background
		$this->white     = imagecolorallocate($this->image, hex2rgb(COLOR_WHITE,  "R"),hex2rgb(COLOR_WHITE  ,"G"),hex2rgb(COLOR_WHITE  ,"B"));
		$this->blue      = imagecolorallocate($this->image, hex2rgb(COLOR_BLUE,   "R"),hex2rgb(COLOR_BLUE   ,"G"),hex2rgb(COLOR_BLUE   ,"B")); 
		$this->orange    = imagecolorallocate($this->image, hex2rgb(COLOR_ORANGE, "R"),hex2rgb(COLOR_ORANGE ,"G"),hex2rgb(COLOR_ORANGE ,"B")); 
		$this->black     = imagecolorallocate($this->image, hex2rgb(COLOR_BLACK,  "R"),hex2rgb(COLOR_BLACK,  "G"),hex2rgb(COLOR_BLACK , "B"));
		$this->darkGreen     = imagecolorallocate($this->image, hex2rgb(COLOR_DARKGREEN,  "R"),hex2rgb(COLOR_DARKGREEN,  "G"),hex2rgb(COLOR_DARKGREEN , "B")); 
		$this->darkRed     = imagecolorallocate($this->image, hex2rgb(COLOR_DARKRED,  "R"),hex2rgb(COLOR_DARKRED,  "G"),hex2rgb(COLOR_DARKRED , "B"));
		$this->green     = imagecolorallocate($this->image, 0, 255, 0); 
		$this->yellow    = imagecolorallocate($this->image, 255, 255, 0); 
		$this->red       = imagecolorallocate($this->image, 255, 0, 0);
		$this->lightBlue = imagecolorallocate($this->image, 0x66, 0xCC, 0xFF);
		$this->lightYellow=imagecolorallocate($this->image, 0xFF, 0xFF, 0xCC);
		$this->gray=imagecolorallocate($this->image, 0xCC, 0xCC, 0xCC);
		//Default font
		$this->font = DEFAULT_FONT;
		$this->modulenumberfont = 2;

		//Define the center
		$this->centerX = IMAGE_WIDTH/2;
		$this->centerY = IMAGE_HEIGHT/2; 

		//Wedges
		$this->wedges = DEF_NUM_WEDGES;
		
		
		//Internal circle
		$this->innerRadius = (IMAGE_WIDTH/2-20)/2;
		//Outer circle
		$this->outerRadius = (IMAGE_WIDTH-60)/2;
		
		//ImageEllipse ($this->image, $this->centerX, $this->centerY, $this->outerRadius*2, $this->outerRadius*2, $this->black);
		
		ImageFilledEllipse ($this->image, $this->centerX, $this->centerY, $this->innerRadius*2, $this->innerRadius*2, $this->white);
		
		
		
	}
	
	//Constructor function for PHP<5.0
	//Should call the __constructor function
	function TileCalBarrel(){
	
		$this->__construct();
	
	}

	function SetLegend(){
	
		if($this->chirality == -1){
			$x = 20;
		}else{
			$x = $this->centerX*2-20;
		} 
		imagestringcentered($this->image, $this->modulenumberfont, $x, 2*$this->centerY-10, $this->name."00", $this->black);
	
	}

	function SetName($name){
	
		$this->name = $name;

		imagestringcentered($this->image, 
					$this->font, 
					$this->centerX, 
					$this->centerY,
					$this->name, 
					$this->black);
	
		//SetChirality
		$uname = strtoupper($name);
		if($uname == "LBA" || $uname == "EBA"){ 
			$this->chirality = -1; //AntiClockWise
		}else if($uname == "LBC" || $uname == "EBC"){ 
			$this->chirality = 1;  //ClockWise
		}
	}
	
	function SetModuleColor($module,$wedge,$color){
	
		$this->module[$module-1][$wedge-1] = $color;
	
	}
	
	function Commit(){
	
	for($i=0;$i < DEF_NUM_MODULES; $i++){
		
			//Module 1 is in (r,phi)=(r,-1*Chirality*DEF_DEG_OFFSET) 	
			
			$module    = $i+1;
			
			for ($w=0;$w<$this->wedges;$w++){
			
				$len = ($this->outerRadius - $this->innerRadius)/$this->wedges;
				$r_start = $this->innerRadius + $len * $w;
				$r_end   = $this->innerRadius + $len * ($w + 1);
				
			
				$phi_start = ($this->chirality*(360/DEF_NUM_MODULES)* $i   ) + (-1*$this->chirality*DEF_DEG_OFFSET);
				$phi_end   = ($this->chirality*(360/DEF_NUM_MODULES)*($i+1)) + (-1*$this->chirality*DEF_DEG_OFFSET);
			
				$p[0] =	round($this->centerX+Deg2X($r_start,$phi_start));
				$p[1] =	round($this->centerY+Deg2Y($r_start,$phi_start));
				$p[2] = round($this->centerX+Deg2X($r_end,  $phi_start)); 
				$p[3] = round($this->centerY+Deg2Y($r_end,  $phi_start)); 
				$p[4] = round($this->centerX+Deg2X($r_end,  $phi_end)); 
				$p[5] = round($this->centerY+Deg2Y($r_end,  $phi_end)); 
				$p[6] =	round($this->centerX+Deg2X($r_start,$phi_end));
				$p[7] =	round($this->centerY+Deg2Y($r_start,$phi_end));
			
				$this->coordinates[$i][$w] = join(",",$p);
			
				//echo "coordinates: ".join(",",$p)."\n";
			
				ImagePolygon       ($this->image, $p, 4, $this->black);
			
				$r_middle = $r_start + ($r_end-$r_start)/2;
				$phi_middle = $phi_start + ($phi_end-$phi_start)/2;
			
				$px = $this->centerX+Deg2X($r_middle,$phi_middle);
				$py = $this->centerY+Deg2Y($r_middle,$phi_middle);
				
				if(isset($this->module[$i][$w])){
					ImageFillToBorder  ($this->image, $px, $py, $this->black, $this->module[$i][$w]);
				}
			}

			//Outside label
			
			$r_outer = $r_end + 15;

			imagestringcentered	( $this->image, 
								$this->modulenumberfont, 
								$this->centerX+Deg2X($r_outer,$phi_middle), 
								$this->centerY+Deg2Y($r_outer,$phi_middle), 
								$module , 
								$this->black);
			
		}
	
	}
	
	function Draw($file=null){
		
		if($file){
			imagejpeg($this->image, $file);
		}else{
			header ("Content-type: image/jpg"); 
			//Creation de l'image
			imagepng($this->image);
		} 
	}
	
	//Draw Sector Labels
	function SetSectorLabels(){
	
		for($i=0;$i < DEF_NUM_SECTORS; $i++){
			//Get position of sector 2*i+1
			//Sector 1 is in (r,phi)=(r,-1*Chirality*DEF_DEG_OFFSET) 
			
			$r_start   = $this->innerRadius - 40; 
			$r_middle  = $this->innerRadius - 20;
			$r_end     = $this->innerRadius;
			
			$sector = 2*$i+1;
			$phi = ($this->chirality*(360/8)*$i)+(-1*$this->chirality*DEF_DEG_OFFSET);
			
			imagestringcentered	( $this->image, 
								$this->font, 
								$this->centerX+Deg2X($r_middle,$phi), 
								$this->centerY+Deg2Y($r_middle,$phi), 
								$sector , 
								$this->orange);
			
			$phi_start = $phi - $this->chirality*(360/8)/2;
			
			
			//Only need to paint the start or the end
			imagelinethick( $this->image, 
							$this->centerX+Deg2X($r_start,$phi_start), 
							$this->centerX+Deg2Y($r_start,$phi_start),
							$this->centerX+Deg2X($r_end,  $phi_start),
							$this->centerX+Deg2Y($r_end,  $phi_start), 
							$this->black, 3);
		}
	}
}



?>
