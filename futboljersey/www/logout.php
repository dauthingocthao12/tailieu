<?PHP
//	ログアウト

	session_cache_limiter('nocache');
	session_start();

	if ($_GET['url']) {
		$burl = $_GET['url'];
	}

	$refere_ = $_SERVER[HTTP_REFERER];

	unset($idpass);
	setcookie("idpass","$idpass",time(),"/",".futboljersey.com");
	unset($idpass);
	unset($_SESSION['idpass']);
	unset($_SESSION['blurl']);
	unset($_COOKIE['idpass']);
	setcookie("idpass");
	unset($addr);
	unset($_SESSION['addr']);

	if (!$burl) { $burl = "/"; }
##	if (ereg("/goods/",$refere_)) {
	if (preg_match("/goods/",$refere_)) {
		$burl .= "out";
	}
	header ("Location: $burl\n\n");

	exit;

?>
