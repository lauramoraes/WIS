
<?php
set_time_limit(0);
		ob_implicit_flush();
		ob_end_flush();


		$con = mysql_connect("pcata007","tilecal","")
			or die("cannot connect to database server pcata007 :(");

//Accessing comminfo

		$select = "SELECT run, DATE_FORMAT(date,'%d/%m/%Y %H:%i:%s') FROM comminfo WHERE (trigger IS NULL or strcmp(trigger,'BAD')!=0) AND date >= '2007-08-10'"; //LIMIT 0,10";

		mysql_select_db("tile", $con);
		$res = mysql_query($select, $con)
			or die("query failed :(");
		$nrows = mysql_num_rows($res);
		$nfields = mysql_num_fields($res);
		$end = $nrows-1;


  $curr = urlencode($select) . "&rows=" . $rows . "&start=";


?>

        <?php


for ($i = 0; $i <= $end; $i++)
{
	$row = mysql_fetch_array($res, MYSQL_BOTH);
	echo $row[0]." ";
}
mysql_close($con);



?>
