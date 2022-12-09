<?PHP
/*

	ネイバーズスポーツ　ログイン

*/
//	ログインページ
function login($ERROR) {

	$html = "";

	if ($ERROR) {
		$error_html = error_html($ERROR);
	}

	$email = $_POST['email'];
	$pass = $_POST['pass'];
	$check = $_POST['check'];

	$checked = "";
	if ($check) {
		$checked = "checked";
	}

	$INPUTS = array();
	$DEL_INPUTS = array();

	$INPUTS['ERROR'] = $error_html;		//	エラーメッセージ
	$INPUTS['EMAIL'] = $email;			//	入力メールアドレス
	$INPUTS['PASS'] = $pass;			//	入力パスワード
	$INPUTS['CHECKED'] = $checked;		//	自動ログインする
/*
	if (!$ERROR) {
		$DEL_INPUTS['ERRORMSG'] = 1;
	}
*/
	//	html作成・置換
	$make_html = new read_html();
	$make_html->set_dir(TEMPLATE_DIR);
	$make_html->set_file("login.htm");
	$make_html->set_rep_cmd($INPUTS);
	$make_html->set_del_cmd($DEL_INPUTS);
	$html = $make_html->replace();

	return $html;
}



//	ユーザー確認
function checkuser(&$ERROR) {

	$email = $_POST['email'];
#	$email = mb_convert_kana($email, "as", "EUC-JP");
	$email = mb_convert_kana($email, "as", "UTF-8");
	$email = trim($email);
	$pass = $_POST['pass'];
#	$pass = mb_convert_kana($pass, "as", "EUC-JP");
	$pass = mb_convert_kana($pass, "as", "UTF-8");
	$pass = trim($pass);
	$check = $_POST['check'];

	if (!$email) { $ERROR[] = "メールアドレスが入力されておりません。"; }
	if (!$pass) { $ERROR[] = "パスワードが入力されておりません。"; }
	if (is_array($ERROR)) {
		return;
	}

	$kojin_num = 0;
	$sql  = "SELECT kojin_num, name_s, point FROM ".T_KOJIN.
			" WHERE email='".$email."'".
			" AND pass='".$pass."'".
			" AND saku!='1'".
			" AND kojin_num<'100000'".
			" ORDER BY kojin_num;";
	if ($result = pg_query(DB, $sql)) {
		$list = pg_fetch_array($result);
		$kojin_num = $list['kojin_num'];
		$name_s = $list['name_s'];
#		$name_s = mb_convert_encoding($name_s, "utf-8", "euc-jp");
		$point = $list['point'];
	}

	if ($kojin_num < 1) {
		$ERROR[] = "登録されてないか入力された情報が間違っています。";
		return;
	}

	//	アフィリエイトユーザーチェック
	$af_num = 0;
	$sql  = "SELECT af_num FROM ".T_AFUSER.
			" WHERE kojin_num='".$kojin_num."'".
			" AND state!='1';";
	if ($result = pg_query(DB, $sql)) {
		$list = pg_fetch_array($result);
		$af_num = $list['af_num'];
	}


	//	情報セッション・クッキセット
	$idpass = $email."<>".$pass."<>".$check."<>".$af_num."<>".$name_s."<>".$point."<>";
	$_SESSION['idpass'] = $idpass;
	unset($_COOKIE['idpass']);
	setcookie("idpass");
	if ($check == 1) {
		setcookie("idpass", $idpass, time() + 60*60*24*30, "/", ".futboljersey.com");
	}


	$url = "/";
	if ($_SESSION['idpass'] && $_SESSION['blurl']) {
		$url = $_SESSION['blurl'];
		unset($_SESSION['blurl']);
	}

	header ("Location: $url\n\n");

	exit;
}
?>