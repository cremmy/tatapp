<?php
$dir=scandir('npc/');

$fp=fopen('out.xml', 'w');

foreach($dir as $file)
	{
	if(!stristr($file, '.xml'))
		continue;
		
	fprintf($fp, "\t<npc template=\"level/npc/%s\" visible=\"1\">\n", $file);
	fprintf($fp, "\t\t<orientation\n");
	fprintf($fp, "\t\t\tx=\"0\" y=\"0\" z=\"0\"\n");
	fprintf($fp, "\t\t\trx=\"1\" ry=\"0\" rz=\"0\"\n");
	fprintf($fp, "\t\t\tux=\"0\" uy=\"0\" uz=\"1\"\n");
	fprintf($fp, "\t\t\tscale=\"1\" />\n");
	fprintf($fp, "\t</npc>\n");
	}
?>