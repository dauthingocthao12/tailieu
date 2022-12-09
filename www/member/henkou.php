<?PHP
/*

	ネイバーズスポーツ	会員登録変更プログラム

*/
	//	サブルーチンフォルダー
	define("SUB_DIR", "../sub");

	//	新ルーチンフォルダー
	define("INCLUDE_DIR", "../include/");

	//	テンプレートフォルダー
	define("TEMPLATE_DIR", "../template/");

	include_once("../../cone.inc");
	include_once(SUB_DIR."/setup.inc");
	include_once(SUB_DIR."/array.inc");

	include_once(INCLUDE_DIR."common.php");
	include_once(INCLUDE_DIR."logincheck.php");
	include_once(INCLUDE_DIR."head.php");
	include_once(INCLUDE_DIR."navi.php");
	include_once(INCLUDE_DIR."ossm.php");
	include_once(INCLUDE_DIR."foot.php");
	include_once(INCLUDE_DIR."henkou.php");



	session_start();

	//	ログインチェック
	login_check();

	$ERROR = array();

	//	コンテンツ
	if ($_POST['modes'] == "hen") {
		email_pass_check($ERROR);	//	メールエラーチェック
	}
	if ($_POST['modes'] == "send") {
		member_check($ERROR);		//	変更情報エラーチェック
	}

	if ($_SESSION['idpass']) {

		if ($_POST['modes'] == "hen" && count($ERROR) == 0 || $_POST['modes'] == "send" && count($ERROR) > 0 || $_POST['modes'] == "back") {

			$main_contents = member_form($ERROR);	//	変更情報入力フォーム

		} else {

			$main_contents = member_henkou($ERROR);	//	メール＆パスワード入力

		}
		if ($_POST['modes'] == "send" && count($ERROR) == 0) {

			$main_contents = member_kakunin();		//	変更内容確認

		}
		if ($_POST['modes'] == "henkou") {

			$main_contents = member_touroku();		//	登録内容変更

		}

	} else {

#		header ("Location: http://www.futboljersey.com");
		header ("Location: $URL");
		exit();

	}



	//	ページタイトル
	$title = "会員登録";
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
