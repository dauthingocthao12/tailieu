<?PHP
/*

	ネイバーズスポーツ	会員脱会プログラム

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
	include_once(SUB_DIR."/mail.inc");

	include_once(INCLUDE_DIR."common.php");
	include_once(INCLUDE_DIR."logincheck.php");
	include_once(INCLUDE_DIR."head.php");
	include_once(INCLUDE_DIR."navi.php");
	include_once(INCLUDE_DIR."ossm.php");
	include_once(INCLUDE_DIR."foot.php");
	include_once(INCLUDE_DIR."dakai.php");



	session_start();


	//	ログインチェック
	login_check();

	$ERROR = array();

	//	パラメーターを取得
	$mode = $_POST["mode"];
	$action = $_POST["action"];

	//echo('$mode=>'.$mode."<br />");
	//echo('$action=>'.$action."<br />");
	//echo('$_SESSION[\'idpass\']=>');
	//pre($_SESSION['idpass']);
	//echo("<br />");
	//echo('$ERROR=>');
	//pre($ERROR);
	//echo("<br />");

	//----------//
	//   処理   //
	//----------//
	if($mode == "dakai"){
		//	入力内容確認
		if($action == "check"){
			list($kojin_num,$name_s,$name_n,$email,$pass) = dakai_check($ERROR);
		}
		//	脱会処理&メール送信
		if($action == "check" && !$ERROR){
			dakai_start($kojin_num,$name_s,$name_n,$email,$pass);
		}
	}

	//----------//
	//   表示   //
	//----------//
	if($_SESSION['idpass']){
		//	TOPページを表示
		$main_contents = dakai_html($ERROR);
	}





	//	ページタイトル
	$title = "会員脱会";
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

