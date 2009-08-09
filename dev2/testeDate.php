<pre>
<?php

	$date = date("l dS of F Y h:i:s A" ,  1164032977);
	echo $date;

	echo "<br>";
	echo date("l dS of F Y h:i:s A", 1164032977);
	echo "<br>";
?>

<?php
$today = getdate(1164032977);
print_r($today);
?> 
	</pre>
