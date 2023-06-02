<?PHP
/*

	ネイバーズスポーツ	アフィリエイトページ

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
	include_once(SUB_DIR."/souryou_muryou.php");

	include_once(INCLUDE_DIR."common.php");
	include_once(INCLUDE_DIR."logincheck.php");
	include_once(INCLUDE_DIR."head.php");
	include_once(INCLUDE_DIR."navi.php");
	include_once(INCLUDE_DIR."ossm.php");
	include_once(INCLUDE_DIR."foot.php");
	include_once(INCLUDE_DIR."affiliate.php");

	include_once(INCLUDE_DIR."aff_link.php");



	session_start();

	//	ログインチェック
	login_check();
/* アフィリエイトシステム部分 start */

	//	アフェリエイトリンク元ファイル名
	$aff_cb_url = $URL . "/aff";

	//	クライアント/プロキシのキャッ シュは無効になります
	// session_cache_limiter('nocache');
	session_start();

	$PHP_SELF = $_SERVER['PHP_SELF'];

	//	ログインチェック
	if (!$_SESSION['idpass']) {
		header ("Location: $URL/\n\n");
		exit;
	}

	//	スラッシュチェック
##	if (!eregi(".html$",$PHP_SELF) && !eregi(".htm$",$PHP_SELF) && !eregi("/$",$PHP_SELF)) {
	if (!preg_match("/\.html$/i",$PHP_SELF) && !preg_match("/\.htm$/i",$PHP_SELF) && !preg_match("/\/$/i",$PHP_SELF)) {
		$sent_url = $URL . $PHP_SELF . "/";
		header ("Location: $sent_url\n\n");
		exit;
	}

	//	メインデーター読み込み
	list($title,$main_contents) = aff_link();

/* アフィリエイトシステム部分 end */

	//	ページタイトル
//	$title = "ネイバーズスポーツ アフィリエイト(Affiliate)";
	//	ページキーワード
	$keywords = "";
	//	Description
	$description = "";

	//	ヘッダ
	$head = read_head();

	//	ナビ
	$navi = read_navi();

	//	アフィリエイトURL表示
	//$aff_url = aff_url($CHECK);

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
	//$INPUTS['PANKUZULIST'] = $link;			//	パンくずリスト
	$INPUTS['MAINCONTENTS'] = $main_contents;	//	コンテンツ
	//$INPUTS['AFFURL'] = $aff_url;				//	アフェリエイトURL表示
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
