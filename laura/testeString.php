<?php
function fixoutput($str){
   $good[] = 9;  #tab
   $good[] = 10; #nl
   $good[] = 13; #cr
   for($a=32;$a<127;$a++){
       $good[] = $a;
   }   
   $len = strlen($str);
   for($b=0;$b < $len+1; $b++){
       if(in_array(ord($str[$b]), $good)){
           $newstr .= $str[$b];
       }//fi
   }//rof
   return $newstr;
}

?>

<html>
<body>
<?php $teste="ola\n tudo bem\n";

$teste =   fixoutput("ola\n tudo bem\n");
echo $teste; 

$some_string = "test\ntext text\r\n";

echo convert_uuencode($some_string);
?>
</body>
</html>
