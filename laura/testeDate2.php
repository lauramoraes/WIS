<pre>
<?php

	$date = date('U' ,  1164032977);
	echo $date;

	echo date(strtotime("@1164032977"));
	echo "<br>";
	echo date("l dS of F Y h:i:s A", 1164032977);
	echo "<br>";
	echo date("l dS of F Y h:i:s A", strtotime("@1164032977"));
	echo "<br>";
?>

<?php
$today = getdate(1164032977);
print_r($today);
	echo "<br>";
$today = getdate(strtotime("@1164032977"));
print_r($today);
?> 
	</pre>
