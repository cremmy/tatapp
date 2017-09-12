<?php
if(count($argv)<2)
{
	die();
}

$lines=file($argv[1]);

printf("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
printf("<sprite name=\"testator\">\n");
printf("<image name=\"default\">%s.png</image>\n", substr($argv[1], 0, -4));

$i=0;

foreach($lines as $line)
{
	$sprinfo=explode(" ", $line);
	
	if(count($sprinfo)<7)
	{
		continue;
	}
	
	printf("\t<animation name=\"%d\">\n", $i++);
	printf("\t\t<direction name=\"down\" fps=\"0\" loopto=\"0\">\n");
	printf("\t\t\t<frame x=\"%d\" y=\"%d\" w=\"%d\" h=\"%d\" offsetx=\"0\" offsety=\"0\" />\n",
		$sprinfo[1], $sprinfo[2], $sprinfo[3], $sprinfo[4]);
	printf("\t\t</direction>\n");
	printf("\t</animation>\n");
}

printf("</sprite>\n");
?>