<?PHP
/*

	ネイバーズスポーツ	会員登録プログラム

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
	include_once(INCLUDE_DIR."member.php");



	session_start();

	//	ログインチェック
	login_check();

	$ERROR = array();



	//	コンテンツ
	if($_POST['modes'] == "new"){

		email_check($ERROR);		//	emailエラーチェック

	}
	if($_POST['modes'] == "send"){

		member_check($ERROR);		//	入力情報エラーチェック

	}
	if($_POST['modes'] == "touroku"){

		member_touroku();			//	登録してメール送信

	}

	if(!$_SESSION['idpass']){

		if($_POST['modes'] == "new" && count($ERROR) == 0 || $_POST['modes'] == "send" && count($ERROR) > 0 || $_POST['modes'] == "back"){

			$main_contents = member_form($ERROR);		//	入力フォーム

		} else {

			$main_contents = member_summary($ERROR);	//	登録概要

		}
		if($_POST['modes'] == "send" && count($ERROR) == 0){

			$main_contents = member_kakunin();			//	確認画面

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
