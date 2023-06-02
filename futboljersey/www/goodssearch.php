<?PHP
/*

	ネイバーズスポーツ	商品検索表示システム

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
	include_once(INCLUDE_DIR."logincheck.php");
	include_once(INCLUDE_DIR."pankuzu_list.php");
	include_once(INCLUDE_DIR."goodssearch.php");
	include_once(INCLUDE_DIR."head.php");
	include_once(INCLUDE_DIR."navi.php");
	include_once(INCLUDE_DIR."aff_url.php");
	include_once(INCLUDE_DIR."ossm.php");
	include_once(INCLUDE_DIR."foot.php");


	session_start();

	//	ログインチェック
	login_check();

	$PHP_SELF = $_SERVER['PHP_SELF'];

	//	ログアウトチェック
	if (preg_match("/out$/", $PHP_SELF)) {
		$PHP_SELF = preg_replace("/\/out$/" ,"", $PHP_SELF);
		$PHP_SELF = preg_replace("/out$/" ,"", $PHP_SELF);
		unset($_SESSION['idpass']);
		unset($_COOKIE['idpass']);
		setcookie("idpass");
	}

	$main_contents = "";
	$title = "商品検索";

	//	パンくずリスト
	if ($CHECK) {
		list($title, $link) = read_pankuzu_list($CHECK);
	}

	//	ヘッダ
	$head = read_head();

	//	ナビ
	$navi = read_navi();

	//	アフィリエイトURL表示
	$aff_url = aff_url($CHECK);

	//	お勧め商品
	$ossm = read_ossm();

	//	フッタ
	$foot = read_foot();


	$title = "商品検索";
	if ($_GET['word']) {
		$word = htmlspecialchars($_GET['word']);
		$title .= " ".trim($word);
	}

	//	検索結果
	$main_contents = goodssearch($word);



	$INPUTS = array();
	$DEL_INPUTS = array();

	$INPUTS['TITLE'] = $title;					//	ページタイトル
	$INPUTS['KEYWORDS'] = $title;				//	ページキーワード
	$INPUTS['DESCRIPTION'] = $description;		//	Description
	$INPUTS['HEAD'] = $head;					//	ヘッダ
	$INPUTS['NAVI'] = $navi;					//	ナビ
	$INPUTS['PANKUZULIST'] = $link;				//	パンくずリスト
	$INPUTS['MAINCONTENTS'] = $main_contents;	//	コンテンツ
	$INPUTS['AFFURL'] = $aff_url;				//	アフェリエイトURL表示
	$INPUTS['OSSM'] = $ossm;					//	お勧め商品
	$INPUTS['FOOT'] = $foot;					//	フッタ

	//	html作成・置換
	$make_html = new read_html();
	$make_html->set_dir(TEMPLATE_DIR);
	$make_html->set_file("default.htm");
	$make_html->set_rep_cmd($INPUTS);
	$make_html->set_del_cmd($DEL_INPUTS);
	$html = $make_html->replace();

	echo $html;

	exit;
?>