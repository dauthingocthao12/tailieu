<?PHP
/*

	ネイバーズスポーツ	マーキングプログラム

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
	include_once(INCLUDE_DIR."head.php");
	include_once(INCLUDE_DIR."navi.php");
	include_once(INCLUDE_DIR."ossm.php");
	include_once(INCLUDE_DIR."foot.php");
	include_once(INCLUDE_DIR."marking.php");
	include_once(INCLUDE_DIR."cago.php");	//	function checkを使用

	session_start();

	//	ログインチェック
	login_check();

	$mode = $_POST['mode'];

	list($KAGOS,$OPTIONS) = check($KAGOS,$OPTIONS);

	$ERROR = array();

	//	エラーチェック
	if ( $mode == "add" ) {
		marking_check( $ERROR );

	}

	//	エラーがなければ「買い物かご」ページへリダイレクト
	if ( $mode == "add" && !$ERROR ) {
		marking_redirect( $KAGOS, $OPTIONS );

	//	マーキングTOP
	} else {
		$main_contents = marking( $KAGOS, $ERROR );

	}



	//	ページタイトル
	$title = "チームオーダー";
	//	ページキーワード
	$keywords = "";
	//	Description
	$description = "";

	//	ヘッダ
	$head = read_head();

	//	ナビ
	$navi = read_navi();

	//	お勧め商品
	$ossm = read_ossm();

	//	フッタ
	$foot = read_foot();

	$INPUTS = array();
	$DEL_INPUTS = array();

	$INPUTS['TITLE'] = $title;					//	ページタイトル
	$INPUTS['KEYWORDS'] = $keywords;			//	ページキーワード
	$INPUTS['DESCRIPTION'] = $description;		//	Description
	$INPUTS['HEAD'] = $head;					//	ヘッダ
	$INPUTS['NAVI'] = $navi;					//	ナビ
	$INPUTS['MAINCONTENTS'] = $main_contents;	//	コンテンツ
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
