<?PHP
/*

	ネイバーズスポーツ　ログインチェック


*/
function login_check() {

	$id_pass = "";
	$kojin_num = 0;
	$af_num = 0;
	$point = 0;
	$check = 0;
	if ($_SESSION['idpass']) {
		$idpass = $_SESSION['idpass'];
	} elseif ($_COOKIE['idpass']) {
		$idpass = $_COOKIE['idpass'];
	}

	//	_xxが付いているカラムは取得した値を利用しない為
	list($email, $pass, $check, $af_num_xx, $name_s_xx, $point_xx) = explode("<>", $idpass);

	//	セッションにログイン情報を保持して無く、クッキーで情報がありログイン状態を維持しない場合は削除
	if (!$_SESSION['idpass'] && $_COOKIE['idpass'] && $check != 1) {
		$idpass = "";
	}

	if (!$idpass) {
		unset($_SESSION['idpass']);
		unset($_COOKIE['idpass']);
		setcookie("idpass");
		return;
	}

	//	ユーザー情報確認
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
		unset($_SESSION['idpass']);
		unset($_COOKIE['idpass']);
		setcookie("idpass");
		return;
	}

	//	アフィリエイトユーザーチェック
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

}
?>
