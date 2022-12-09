<?PHP
/*

	ネイバーズスポーツ　ヘッダルーチン

*/

function read_head() {
	global $conn_id;

	$INPUTS = array();
	$DEL_INPUTS = array();

	//	戻るページURL
	$burl = $_SERVER['REQUEST_URI'];
	if ($_SESSION['blurl']) {
		$burl = $_SESSION['blurl'];
	}

	$idpass = "";
	if ($_SESSION['idpass']) {
		$idpass = $_SESSION['idpass'];
	} elseif ($_COOKIE['idpass']) {
		$idpass = $_COOKIE['idpass'];
	}

	$user_name = "";
	if ($idpass) {
		list($email, $pass, $check, $af_num, $name_s, $point) = explode("<>", $idpass);

		$point = number_format($point);

		$ai_time = date("H");
		if ($ai_time >= 6 && $ai_time < 10) { $aisatu = "　おはようございます。"; }
		elseif ($ai_time >= 10 && $ai_time < 18) { $aisatu = "　こんにちは。"; }
		else { $aisatu = "　こんばんは。"; }

#		$name_s = mb_convert_encoding($name_s,"euc-jp","auto");

		$user_name .= $name_s."様 ".$aisatu."<br />\n";
		$user_name .= "保有割引ポイントは、".$point." pt/円 です。 \n";
		$login_chk = "LOGIN";
	} else {
		$user_name = "ようこそゲストさん";
		$login_chk = "LOGOUT";
	}

	$INPUTS['HTTP'] = HTTP;
	$INPUTS['HTTPS'] = HTTPS;
	$INPUTS['BURL'] = urlencode($burl);
	$INPUTS['USERNAME'] = $user_name;

	$DEL_INPUTS[$login_chk] = 1;

	//	html作成・置換
	$make_html = new read_html();
	$make_html->set_dir(TEMPLATE_DIR);
	$make_html->set_file("head.htm");
	$make_html->set_rep_cmd($INPUTS);
	$make_html->set_del_cmd($DEL_INPUTS);
	$html = $make_html->replace();

	return $html;
}
?>
