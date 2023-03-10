<?PHP
/*
 * 注意 (2017-01-17)
 *
 * ↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓
 * 同時に修正されるファイル：
 *  - loginmsg.php
 *  - loginmsg_resp.php
 * ↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑
 */

//	ログインメッセージ LOGIN

	include("../cone.inc");
	include("./sub/setup.inc");
	include("./sub/loginmsg.inc");

//	session_cache_limiter('nocache');
	session_start();
//	session_register("idpass","customer");

	$burl = $_SERVER['REQUEST_URI'];

	$loginmsg = loginmsg($burl);

	echo($loginmsg);

?>
