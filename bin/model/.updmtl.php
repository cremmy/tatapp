<?php
$dir=scandir(".");

foreach($dir as $file)
{
	if(!stristr($file, ".mtl"))
		continue;
	
	$base=substr($file, 0, -4);
	
	$fp=fopen($file, "a");
	
	fprintf($fp, "map_Kd %s/diffuse.png\n", $base);
	fprintf($fp, "map_Bump %s/normal.png\n", $base);
	
	fclose($fp);
}
?>