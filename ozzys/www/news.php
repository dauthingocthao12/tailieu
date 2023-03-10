<?PHP
//	入荷情報

	include("../cone.inc");
	include("./sub/setup.inc");
	include("./sub/news.inc");

	$news = news();

	echo($news);

?>
