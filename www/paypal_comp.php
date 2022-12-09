<?PHP
/*

	ネイバーズスポーツ	買い物かごプログラム

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
	include_once(INCLUDE_DIR."cago.php");
	include_once(INCLUDE_DIR."paypal_module.php");	//	add ookawara 2013/11/19



	session_start();

	//	ログインチェック
	login_check();

	//	アフェリエイトidが存在すればセッション埋め込み
	if ($_COOKIE['affid']) {
		$_SESSION['affid'] = $_COOKIE['affid'];
	}

	//	リファラを取得
	$refere_ = $_SERVER['HTTP_REFERER'];
	//	買い物かごページ遷移以前のリファラをセッションに保持する
	$domein = DOMEIN;
	if (!preg_match("/cago.php/" , $refere_) && preg_match("/$domein/" , $refere_)) {
		$_SESSION['refere'] = $refere_;
	}

	//	支払が完了したかチェック
	paypal_comp_check($ERROR);
	if ($ERROR) {
		//	支払が完了しなかった場合
		$mode = "user_data";
	}

	//	商品配列が空で渡るとエラーになるので記述する
	$KAGOS = array();
	$OPTIONS = array();
	if ($_SESSION['customer']) {
		$KAGOS = explode("<>", $_SESSION['customer']);
	}
	if ($_SESSION['opt']) {
	 	$OPTIONS = explode("<>", $_SESSION['opt']);
	}

	//	処理
	if ($mode == "user_data") {
		//	ログイン処理
		if ($action == "login_check") {
			checkuser($ERROR);
		}
	} else {
		//	ご注文＆メール送信
		cago_sent($KAGOS , $OPTIONS , $mode);
	}

	//	表示
	if ($mode == "user_data") {
		//	お届け先情報フォーム
		if (($action == "idpass" || $action == "login_check") && !$_SESSION['idpass']) {
			//	会員でログインしていなかった場合
			$main_contents = login_html($ERROR);
		} else {
			$main_contents = cago_form_html($ERROR);
		}
	} else {
		//	カード払い処理＆お礼メッセージ
		$main_contents = cago_thank();
	}

	//	ページタイトル
	$title = "買い物かご";
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
