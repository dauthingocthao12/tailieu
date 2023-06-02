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
	include_once(SUB_DIR."/mail.inc");

	include_once(INCLUDE_DIR."common.php");
	include_once(INCLUDE_DIR."logincheck.php");
	include_once(INCLUDE_DIR."head.php");
	include_once(INCLUDE_DIR."navi.php");
	include_once(INCLUDE_DIR."ossm.php");
	include_once(INCLUDE_DIR."foot.php");
	include_once(INCLUDE_DIR."affiliate.php");



	session_start();

	//	ログインチェック
	login_check();

	//	ユーザー情報を取得する
	if ($_SESSION['idpass']) {
		$idpass = $_SESSION['idpass'];
	} elseif ($_COOKIE['idpass']) {
		$idpass = $_COOKIE['idpass'];
	}
	list($email,$pass,$check,$af_num)  = explode("<>",$idpass);

	//	現在実行しているスクリプトのファイル名を取得
	$PHP_SELF = $_SERVER['PHP_SELF'];
	unset($mode);
	unset($action);
	$mode = $_POST['mode'];
	$action = $_POST['action'];
	//	リダイレクトでページ遷移した時はGETでパラメーターを受け取る
	if($_GET['mode']){				//	add 2013/12/27
		$mode = $_GET['mode'];
		$action = $_GET['action'];	//	add 2013/12/27
	}								//	add 2013/12/27

	//	エラー配列
	$ERROR = array();

//echo('$mode=>'.$mode."<br>");
//echo('$action=>'.$action."<br>");
//echo('$ERROR=>');
//pre($ERROR);
//echo("<br />");
//----------//
//   処理   //
//----------//
	if($mode == "regist"){
	//	アフィリエイト会員登録
		if($action == "regist"){
			//	登録処理
			af_regist($ERROR);

		}
	} elseif($mode == "p_apply"){
	//	ポイント変換申請
		if($action == "p_apply_check"){
			//	確認
			p_apply_check($af_num,$ERROR);

		}elseif($action == "p_apply_regist"){
			// 申請処理＆メール送信
			p_apply_regist($af_num,$ERROR);

		}

	}elseif($mode == "p_update"){
	//	ポイント変換履歴・変更

		if($action == "p_update_check"){
			//	確認
			p_update_check($af_num,$ERROR);

		}elseif($action == "p_update_regist"){
			//	変更処理＆メール送信
			p_update_regist($af_num,$ERROR);

		}elseif($action == "p_cancel_start"){	//	update 2013/12/27
			//	キャンセル処理＆メール送信
			p_cancel($af_num,$ERROR);
		}

	}
//----------//
//   表示   //
//----------//
	if(!$idpass){
	// 非会員
		list($title,$main_contents) = nonmember_html();

	}elseif($af_num > 0){
	// アフィリエイト会員

		if($mode == "p_check"){
		//	ポイント詳細

			if($action == "day"){
				//	日別一覧
				list($title,$main_contents) = pc_day_html($af_num);

			}elseif($action == "mon"){
				//	月別一覧
				list($title,$main_contents) = pc_mon_html($af_num);

			}elseif($action == "year"){
				//	年別一覧
				list($title,$main_contents) = pc_defaults_html($af_num);

			}

		}elseif($mode == "p_apply"){
		//	ポイント変換申請

			if($action == "p_apply_check" && !$ERROR){
				//	確認ページ
				list($title,$main_contents) = p_apply_check_html($af_num,$ERROR);

			}else{
				//	変換申請ページ
				list($title,$main_contents) = p_apply_html($af_num,$ERROR);

			}

		}elseif($mode == "p_update"){
		//	申請履歴・変更
			if($action == "p_update_change" || ($action == "p_update_check" && $ERROR)){
				//	変更入力
				list($title,$main_contents) = p_update_change_html($af_num,$ERROR);

			}elseif($action == "p_update_check" && !$ERROR){
				//	確認
				list($title,$main_contents) = p_update_check_html($ERROR);

			}elseif($action == "p_cancel"){
				//	キャンセル
				list($title,$main_contents) = p_cancel_html($af_num,$ERROR);

			}else{
				//	履歴一覧
				list($title,$main_contents) = p_update_html($af_num,$ERROR);
			}

		}else{
		// TOPページ
			list($title,$main_contents) = afmember_html($af_num);
		}

	}else{
	// 会員だが非アフィリエイト会員
		if($mode == "regist"){
			//	アフィリエイト会員登録
			list($title,$main_contents) = re_defaults_html($ERROR);
		}else{
			//	初期画面
			list($title,$main_contents) = member_html();
		}
	}
	if($mode == "kiyaku"){
		//	規約ページ
		list($title,$main_contents) = kiyaku();

	}



	//	ページタイトル
	//$title = "ネイバーズスポーツ アフィリエイト(Affiliate)";

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
	// $ossm = read_ossm();  

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
	// $INPUTS['OSSM'] = $ossm;					//	お勧め商品  
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
