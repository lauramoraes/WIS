<html>
<head>
  <title>
       SSH - WEB
  </title>
<STYLE type=text/css>
A:link { text-decoration: none}
A:visited { text-decoration: none}
</STYLE>
</head>

<body>
<center><font face=arial size=5 color=red> Administraç ssh via web </font></center>
<br>
<h2 align=right><font color=#0000f2 face=arial> tucs </font></h2>
<hr width=400 align=right>
<br><br>

<form method=post actiom=exec_comandos.php>
<input type=text name=comando size=40>
<input type=submit value=Executar>
</form>

</body>
</html>
<?
$comando = shell_exec("sudo $comando");

echo "<pre> $comando </pre>" ;
?>
