<?PHP
//	PDA商品検索

	include("../../cone.inc");
	include("../sub/setup.inc");
	include("../sub/array.inc");

	include("./sub/base_html.inc");
	include("./sub/subroutine.inc");
	include("./sub/error.inc");

	//	商品データーテーブル名
	$TABLE = "goods2";
	//	一覧表示数
	$view = 10;

	$PHP_SELF = $_SERVER['PHP_SELF'];

	//	セッション設定
	session_start();
##	session_register("SEARCH","ERROR");

	if ($_POST['mode']) { $mode = $_POST['mode']; }
	elseif ($_GET['mode']) { $mode = $_GET['mode']; }
	$mode = stripslashes($mode);

	if ($_POST['mode']) {
		if ($mode == "検索" || $mode == "change") {	//	検索項目セッション登録
			seach_check();
		}
		elseif ($mode == "リセット") {	//	検索項目セッションリセット
			search_reset();
		}
		elseif ($mode == "詳細") {	//	商品詳細項目セット
			check_detaile();
		}
	}

	if ($mode == "detaile") {	//	商品詳細ページ
			$htm = detaile();
	}
	else {	//	初期ページ
		$htm = first();
	}

	$html  = head();
	$html .= $htm;
	$html .= foot();

	echo ($html);
	ob_start("mb_output_handler");

	if ($db) { pg_close($db); }

	exit;

?>
