<?PHP
//	入荷情報

	include("../cone.inc");
	include("./sub/setup.inc");
	include("./sub/news_display.inc");

	$news_display = news_display();

	echo($news_display);

?>
