<?PHP
//	売れ筋BEST10

	include("../cone.inc");
	include("./sub/setup.inc");
	include("./sub/uresuji.inc");

	$uresuji = uresuji();

	echo($uresuji);

	if ($db) { pg_close($db); }

?>
