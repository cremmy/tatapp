<?php
//die('nope');

$dir=scandir('..\\model');

foreach($dir as $file)
	{
	if(!stristr($file, '.obj'))
		continue;
	
	$bname=substr($file, 0, -4);
	$name=$bname.'.xml';
	
	$fp=fopen('npc/'.$name, 'w');
	
	fprintf($fp, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
	fprintf($fp, "<npc name=\"%s\" type=\"model\">\n", $bname);
	fprintf($fp, "	<!--<collider z=\"0.25\" w=\"0.5\" d=\"0.5\" h=\"0.5\" />-->\n");
	fprintf($fp, "	<model>model/%s</model>\n", $file);
	fprintf($fp, "</npc>\n");
	}
?>