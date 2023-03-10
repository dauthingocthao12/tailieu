<?PHP
//	お勧め OSUSUME

	include("../cone.inc");
	include("./sub/setup.inc");
	include("./sub/osusume.inc");

	if ($_GET['list']) {
		$LIST = explode("/",$_GET['list']);
	}

	$osusume = osusume($LIST);

	echo($osusume);

?>
