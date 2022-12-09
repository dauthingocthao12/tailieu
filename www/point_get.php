<?PHP
/*

	ポイントゲットシステム


*/

	include "./sub/menu_p.inc";
	include "./sub/cago.inc";
	include "../cone.inc";
	include "./sub/head.inc";
	include "./sub/foot.inc";
	include "./sub/array.inc";

	session_start();

	//	サブルーチンフォルダー
	$SUB_DIR = "./sub/";
	include ("$SUB_DIR/base.php");
	$title .= "ポイントゲット サッカーショップ サッカーユニフォーム ネイバーズスポーツ";

	//	メインコンテンツ
	$main = point_get();

	$html .= head_html($title);
	$html .= head_menu_html();
	$html .= head_login_html();
	$html .= special_html();
	$html .= side_menu_html();

	//	メインコンテンツ
	$html .= $main;

	$html .= foot_html();

	echo("$html");

	exit();



//	メインコンテンツ
function point_get() {

	$html = "<div class=\"con_name\"><div class=\"con_text\"><B>ボーナスポイントプレゼントイベント！</B></div></div>\n";
	$html .= "<br>\n";

	if (!check_event_day()) {
		$html .= not_point_get();
		return $html;
	}


	if ($_POST['mode'] == "send_point") {
		send_point($ERROR);
	}

	if ($_SESSION['idpass']) {
		if (!$ERROR && $_GET['mode'] == "end") {
			$html .= point_get_end();
		} else {
			$html .= get_login($ERROR);
		}
	} else {
		$html .= not_get_login();
	}

	return $html;

}


//	ポイントゲット確認期間
function check_event_day() {

	$flag = 1;
	if (!POINT_GET_START || !POINT_GET_END || POINT_GET_POINT < 1) {
		$flag = 0;
	} elseif (POINT_GET_START && POINT_GET_END) {
		//	開始期間
		list($s_year,$s_mon,$s_day) = explode("/",POINT_GET_START);
		$s_time = mktime(0,0,0,$s_mon,$s_day,$s_year);

		//	終了期間
		list($f_year,$f_mon,$f_day) = explode("/",POINT_GET_END);
		$f_time = mktime(0,0,0,$f_mon,$f_day+1,$f_year);

		//	今の時間
		$n_time = time();

		if ($s_time > $n_time || $n_time > $f_time) {
			$flag = 0;
		}

	}

	return $flag;
}


//	ポイント未配布
function not_point_get() {

	$html  = "<h3>只今ボーナスポイントプレゼントイベントは行っておりません。</h3>\n";


	return $html;
}

//	未ログイン
function not_get_login() {

	$html .= "<table border=\"0\" width=\"100%\" cellpadding=\"0\" cellspacing=\"0\">\n";
//	$html .= "<tr height=\"20\">\n";
//	$html .= "<th class=\"cate2\">未ログイン中</th>\n";
//	$html .= "</tr>\n";
	$html .= "<tr>\n";
	$html .= "<td style=\"padding:5px;\">\n";
	$html .= POINT_GET_START."～".POINT_GET_END."の間、条件を満たしして頂いた会員様に限り、ボーナスポイント<b>".POINT_GET_POINT."pt</b>をプレゼント!<br>\n<br>\n";
	$html .= "ログインをしますと、このページに「ボーナスポイント」ボタンが表示されますので、クリックでポイント追加！<br>\n<br>\n";
	$html .= "※ただし、メルマガに記載させて頂いた条件に設定させて頂く必要がございます。<br>\n";
	$html .= "</td>\n";
	$html .= "</tr>\n";
	$html .= "</table>\n";

	return $html;

}


//	ログイン中
function get_login($ERROR) {

	if ($ERROR) {
		$html  = "<font color=\"#ff0000\"><b>エラー</b></font><br>\n";
		foreach ($ERROR AS $val) {
			$html .= "・".$val."<br>\n";
		}
		$html .= "<br>\n";
	}

	//	会員管理番号取得
	list($kojin_num) = get_kojin_num();

	//	ポイント配布済みかチェック
	$count = point_get_check($kojin_num);

	if ($count < 1) {
		//	ポイント配布前
		$html .= "<table border=\"0\" width=\"100%\" cellpadding=\"0\" cellspacing=\"0\">\n";
//		$html .= "<tr height=\"20\">\n";
//		$html .= "<th class=\"cate2\">ログイン中</th>\n";
//		$html .= "</tr>\n";
		$html .= "<tr>\n";
		$html .= "<td style=\"padding:5px;\">\n";
		$html .= "<form action=\"./point_get.php\" method=\"POST\">\n";
		$html .= "<input type=\"hidden\" name=\"mode\" value=\"send_point\" />\n";
		$html .= "<br>\n";
		$html .= POINT_GET_START."～".POINT_GET_END."の間、条件を満たしして頂いた会員様に限り、ボーナスポイント<b>".POINT_GET_POINT."pt</b>をプレゼント!<br>\n";

		$html .= "<br>\n";
		$html .= "「ボーナスポイント」ボタンをクリックして頂くと1回のみプレゼントさせて頂きます。<br>\n";

		$html .= "<br>\n";
		$html .= "<center><input type=\"submit\" value=\"ボーナスポイント\" /></center>\n";
		$html .= "<br>\n";
		$html .= "</form>\n";
		$html .= "</td>\n";
		$html .= "</tr>\n";
		$html .= "</table>\n";
	} else {
		//	ポイント配布済み
		$html .= "<table border=\"0\" width=\"100%\" cellpadding=\"0\" cellspacing=\"0\">\n";
//		$html .= "<tr height=\"20\">\n";
//		$html .= "<th class=\"cate2\">ログイン中</th>\n";
//		$html .= "</tr>\n";
		$html .= "<tr>\n";
		$html .= "<td style=\"padding:5px;\">\n";
		$html .= "<br>\n";
		$html .= "ボーナスポイントは、一回のみとさせて頂きます。<br>\n";
		$html .= "<br>\n";
		$html .= "</td>\n";
		$html .= "</tr>\n";
		$html .= "</table>\n";
	}

	return $html;

}


//	ポイント配布
function send_point(&$ERROR) {
	global $conn_id;

	//	会員管理番号取得
	list($kojin_num,$read_magazine) = get_kojin_num();
	if (!$kojin_num) {
		$ERROR[] = "ユーザー情報が確認できませんでした。";
		return $ERROR;
	}

	//	ポイントプレゼントの会員かチェック
	if ($kojin_num > LAST_KOJIN_NUM) {
		$ERROR[] = "ボーナスポイントプレゼント会員条件を満たしておりません。";
		return $ERROR;
	}

	//	メルマガ購読者かチェック
	if ($read_magazine != 1) {
		$ERROR[] = "ボーナスポイントプレゼント条件を満たしておりません。";
		return $ERROR;
	}

	//	ポイント配布済みかチェック
	$count = point_get_check($kojin_num);
	if ($count > 0) {
		$ERROR[] = "既にポイントは、配布されております。";
		return $ERROR;
	}

	//	point_get記録
	$sql  = "INSERT INTO point_get VALUES (".
				" '".GET_POINT_NUM."'".
				",'".$kojin_num."'".
				",now()".
				",'".POINT_GET_POINT."'".
				",'0'".
			");";
	if (!pg_exec($conn_id,$sql)) {
		$ERROR[] = "ポイント情報を記録できませんでした。";
		return $ERROR;
	}

	//	kojin　point　追加
	$sql  = "UPDATE kojin SET".
			" point=point+".POINT_GET_POINT.
			" WHERE kojin_num='".$kojin_num."';";
	if (!pg_exec($conn_id,$sql)) {
		$ERROR[] = "ポイントを追加できませんでした。";
		return $ERROR;
	}

	//	終了画面に飛ばす
	header ("Location: ./point_get.php?mode=end\n\n");

	exit;

}


//	会員番号取得
function get_kojin_num() {
	global $conn_id;

	//	ユーザー情報設定
	list($email,$pass) = explode("<>",$_SESSION['idpass']);

	//	会員管理番号取得
	$kojin_num = "";
	$read_magazine = 2;
	$sql  = "SELECT kojin_num, meruma FROM kojin".
			" WHERE email='".$email."'".
			" AND pass='".$pass."'".
			" AND saku!='1'".
			" AND kojin_num<'100000'".
			" LIMIT 1;";
	if ($result = pg_query($conn_id,$sql)) {
		$list = pg_fetch_array($result);
		$kojin_num = $list['kojin_num'];
		$read_magazine = $list['meruma'];
	}

	return array($kojin_num,$read_magazine);
}


//	ポイント取得チェック
function point_get_check($kojin_num) {
	global $conn_id;

	$count = 0;
	$sql  = "SELECT count(*) AS count FROM point_get".
			" WHERE kojin_num='".$kojin_num."'".
			" AND point_get_num='".GET_POINT_NUM."'".
			" LIMIT 1;";
	if ($result = pg_query($conn_id,$sql)) {
		$list = pg_fetch_array($result);
		$count = $list['count'];
	}

	return $count;
}


//	ポイント配布終了
function point_get_end() {

	$html .= "<table border=\"0\" width=\"100%\" cellpadding=\"0\" cellspacing=\"0\">\n";
//	$html .= "<tr height=\"20\">\n";
//	$html .= "<th class=\"cate2\">ログイン中</th>\n";
//	$html .= "</tr>\n";
	$html .= "<tr>\n";
	$html .= "<td style=\"padding:5px;\">\n";
	$html .= "<br>\n";
	$html .= "ポイントをプレゼント致しましたのでご確認ください。<br>\n";
	$html .= "<br>\n";
	$html .= "今後ともネイバーズスポーツを宜しくお願いいたします。<br>\n\n";
	$html .= "<br>\n";
	$html .= "</td>\n";
	$html .= "</tr>\n";
	$html .= "</table>\n";

	return $html;
}
?>
