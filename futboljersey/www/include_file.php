<?PHP
/*

	ネイバーズスポーツ	インクルードファイルシステム

	include_file.php
		inc = head	ヘッダ
		inc = navi	左メニュー
		inc = foot	フッター
		inc = ossm	お勧め
*/

	//	サブルーチンフォルダー
	define("SUB_DIR", "./sub");

	//	新ルーチンフォルダー
	define("INCLUDE_DIR", "./include/");

	//	テンプレートフォルダー
	define("TEMPLATE_DIR", "./template/");

	include_once("../cone.inc");
	include_once(SUB_DIR."/setup.inc");
	include_once(SUB_DIR."/array.inc");

	include_once(INCLUDE_DIR."common.php");


	session_start();

	$inc = "";
	if ($_POST['inc']) {
		$inc = $_POST['inc'];
	} elseif ($_GET['inc']) {
		$inc = $_GET['inc'];
	}
	define("inc", $inc);

	$html = "";
	if (inc == "head") {
		include_once(INCLUDE_DIR."head.php");
		$html = read_head();
	} elseif (inc == "navi") {
		include_once(INCLUDE_DIR."navi.php");
		$html = read_navi();
	} elseif (inc == "foot") {
		include_once(INCLUDE_DIR."foot.php");
		$html = read_foot();
	} elseif (inc == "ossm") {
		include_once(INCLUDE_DIR."ossm.php");
		$html = read_ossm();
	} elseif (inc == "logincheck") {
		include_once(INCLUDE_DIR."logincheck.php");
		$html = login_check();
	}

	echo $html;

	exit;
?>