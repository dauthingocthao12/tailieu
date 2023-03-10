<?PHP
//	ログアウト

//	session_cache_limiter('nocache');
	session_start();
//	session_register("idpass");

	if ($_GET['url']) {
		$burl = $_GET['url'];
	}

	unset($idpass);
	unset($_SESSION['idpass']);
	unset($_COOKIE['idpass']);
	setcookie("idpass");

	if (!$burl) { $burl = "/"; }
	$burl .= "out";
	header ("Location: $burl\n\n");

	exit;

?>
