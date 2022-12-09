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
	include_once(INCLUDE_DIR."payjp_module.php");

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
	if (!preg_match("/cago.php/" , $refere_) && !preg_match("/cago2.php/" , $refere_) && preg_match("/$domein/" , $refere_)) {
		$_SESSION['refere'] = $refere_;
	}

	$mode = $_POST['mode'];
	$action = $_POST['action'];		//	add yoshizawa 2013/11/25

	//	PayPal戻り値セット・確認
	if ($_GET['m'] == "cancel") {
		$mode = "user_data";
	} elseif ($_GET['m'] == "comp") {
		//	支払が完了したかチェック
		paypal_comp_check($ERROR);
		if ($ERROR) {
			//	支払が完了しなかった場合
			$mode = "user_data";
		} else {
			//$mode = "send";	//	del ookawara 2014/03/25
			$mode = "paypal_last_check";	//	add ookawara 2014/03/25
		}
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
	if ($mode == "check") {
		//	お届け先情報フォーム確認
		cago_check($ERROR);
	} elseif ($mode == "user_data") {
		//	ログイン処理
		if ($action == "login_check") {
			checkuser($ERROR);
		}
	} elseif ($mode == "send") {
		//	受注処理
		if ($action == "paypal") {
			//	PayPal支払処理
			paypal($ERROR);
		} else {
			// カードトークンをセッションに追加
			if ($_POST["payjp-token"]){
				$_SESSION["payjp"]["card_token"]=$_POST["payjp-token"];
			}

			//	ご注文＆メール送信
			cago_sent($KAGOS , $OPTIONS , $mode);
		}
	} else {
		//	買い物かご
		if ($action == "hen") {
			//	商品情報変更
			hen($KAGOS,$ERROR);					//	update yoshizawa 2013/11/22
		} elseif ($action == "del") {
			//	商品情報削除
			del($KAGOS);						//	update yoshizawa 2013/11/22
		} elseif ($action == "del_op") {
			//	商品マーキングオーダー削除
			del_op($OPTIONS);					//	update yoshizawa 2013/11/22
		} elseif ($action == "add") {
			//	商品追加
			add($KAGOS, $OPTIONS, $ERROR);		//	update yoshizawa 2013/11/22
		}
		//	追加・変更・削除した商品情報をセッションに記録
		if (count($KAGOS) > 0) {
			$_SESSION['customer'] = implode("<>",$KAGOS);
		} else {
			unset($_SESSION['customer']);
		}
		if (count($OPTIONS) > 0) {
			$_SESSION['opt'] = implode("<>",$OPTIONS);
		} else {
			unset($_SESSION['opt']);
		}
	}

	//	表示
	if ($_GET['num'] != "") {
		//	カード払い処理＆お礼メッセージ
		$main_contents = cago_thank();
	} elseif (!$KAGOS && !$OPTIONS) {	//	add ookawara 2014/02/26
	//if (!$KAGOS && !$OPTIONS){		//	del ookawara 2014/02/26
		//	商品無し
		//	買い物かご
		$main_contents = cago_empty_html();
	} else {
		//	商品在り
		//if ($mode == "user_data" || $mode == "modoru" || ($ERROR && ($mode == "check" || $mode == "send"))) {	//	del ookawara 2014/01/06
		if ($mode == "user_data" || ($ERROR && ($mode == "check" || $mode == "send"))) {	//	add ookawara 2014/01/06
			//	お届け先情報フォーム
			if (($action == "idpass" || $action == "login_check") && !$_SESSION['idpass']) {
				//	会員でログインしていなかった場合
				$main_contents = login_html($ERROR);
			} else {
				//$main_contents = cago_form_html($ERROR , $mode);	//	del ookawara 2013/01/06
				$main_contents = cago_form_html($ERROR);	//	add ookawara 2013/01/06
			}
		//} elseif ($mode == "check") {	//	del ookawara 2014/03/25
		} elseif ($mode == "check" || $mode == "paypal_last_check") {	//	add ookawara 2014/03/25
			//	確認画面
			$main_contents = cago_kakunin_html($KAGOS , $OPTIONS , $ERROR , $mode);
		} elseif ($mode == "send") {
			//	カード払い処理＆お礼メッセージ
			$main_contents = cago_thank();
		//} else {					//	del ookawara 2014/05/13	
		} elseif( empty($mode) ) {	//	add ookawara 2014/05/13	
			//	買い物かご
			$main_contents = cago_goods_html($KAGOS , $OPTIONS , $ERROR);
		}
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

	if (DB) { pg_close(DB); }	//	ad ookawara 2014/02/25
	exit;

?>