<?PHP
//	ログインチェック
function pass_check($main,$ADMIN_ID,$ADMIN_PASS) {

	if ($_SESSION['midpass']) {
		list($id,$pass) = explode("<>",$_SESSION['midpass']);
	}

	if (!$id && ($_POST['id'] || $_GET['a'])) {
		if ($_POST['id']) { $id = $_POST['id']; }
		else { $id = $_GET['a']; }
		$id = $_POST['id'] = $id;
		$id = trim($id);
	}
	if (!$pass && ($_POST['pass'] || $_GET['b'])) {
		if ($_POST['pass']) { $pass = $_POST['pass']; }
		else { $pass = $_GET['b']; }
		$pass = $_POST['pass'] = $pass;
		$pass = trim($pass);
	}

	if ($main == "login") {
		if (!$id) { $ERROR[] = "IDが入力されておりません。"; }
		if (!$pass) { $ERROR[] = "パスワードが入力されておりません。"; }
	}

	if ($main == "logout") {
		unset($_SESSION['midpass']);
		setcookie("enter_check");
		unset($id);
		unset($pass);
//		unset($main);
	}
	elseif ($main && $main != "login" && (!$id || !$pass)) {
		$ERROR[] = "タイムオーバーです。";
		unset($main);
	}
	elseif ($main && $id && $pass) {
		if ($id != $ADMIN_ID) { unset($id); }
		if ($pass != $ADMIN_PASS) { unset($pass); }
		if (!$id || !$pass) { $ERROR[] = "ID又はパスワードが間違っています。"; }

	}

	if (!$ERROR && $id && $pass) {
		$_SESSION['midpass'] = "$id<>$pass<>";
	}

	return array($ERROR,$main);

}
?>
