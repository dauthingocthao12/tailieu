<?PHP
//	ログイン
function login() {

	if ($_GET['url']) {
		$burl = $_GET['url'];
		$_SESSION['blurl'] = $burl;
	}
	$idpass = $_SESSION['idpass'];
	$burl = $_SESSION['blurl'];
	$mode = $_POST['mode'];

	if ($mode == "checkuser") { checkuser(); }

	if ($idpass) {
		unset($_SESSION['blurl']);
		if (!$burl) { $burl = "/"; }
		header ("Location: $burl\n\n");
	}

	$html = first();

	return $html;

}



//	ログインページ
function first() {
global $PHP_SELF;

	$ERROR = $_SESSION['ERROR'];
	if ($ERROR) {
		$errors = "<FONT color=\"#ff0000\"><B>エラー</B></FONT><BR>\n";
		foreach ($ERROR AS $VAL) {
			if ($VAL) { $errors .= $VAL . "<BR>\n"; }
		}
		$errors .= "<BR>\n";
		unset($_SESSION['ERROR']);

		if ($_SESSION['emailpassmemo']) {
			$emailpassmemo = $_SESSION['emailpassmemo'];
			list($email,$pass,$memo) = explode("<>",$emailpassmemo);
		}
		unset($_SESSION['emailpassmemo']);
	}

	if ($memo) { $checked = "checked"; } else { $checked = ""; }

	$html = <<<WAKABA
		<h2 class="sub-title-prod">ログイン</h2>

		<br>
		<div class="login-form">
			<FORM action="$PHP_SELF" method="POST">
				<INPUT type="hidden" name="mode" value="checkuser">

				<div class="alert alert-danger">$errors</div>

				<p class="spaced">
					会員登録時に登録したメールアドレスと、パスワードを入力しログインボタンを押してください。<BR>
					自動ログインにチェックを入れますと、ブラウザを終了してもログアウトしないように設定されます。
				</p>

				<TABLE class="table-resp">
					<TBODY>
					<TR>
						<TH>メールアドレス</TH>
						<TD><INPUT type="text" name="email" class="input-block" value="$email"> (半角)</TD>
					</TR>
					<TR>
						<TH>パスワード</TH>
						<TD><INPUT size="16" type="password" class="input-block" maxlength="8" name="pass" value="$pass"> (半角)</TD>
					</TR>
					<TR>
						<TH>自動ログイン</TH>
						<TD><INPUT type="checkbox" name="memo" value="1" $checked></TD>
					</TR>
					<TR>
						<td colspan="2" class="text-center">
							<button type="submit" class="btn btn-submit">ログイン</button>　<button type="reset" class="btn btn-reset">クリア</button>
						</td>
					</TR>
					</TBODY>
				</TABLE>

				<p class="login-form-footmessage">
					会員登録をしていない方は<A href="/member.php">こちら</A>から登録してください。<BR>
					<A href="/member.php">会員登録</A>
				</p>

			</FORM>
		</div>

WAKABA;

	return $html;
}



//	ユーザー確認
function checkuser() {
global $PHP_SELF,$db;

	$email = $_POST['email'];
	$email = mb_convert_kana($email,"as","EUC-JP");
	$email = trim($email);
	$pass = $_POST['pass'];
	$pass = mb_convert_kana($pass,"as","EUC-JP");
	$pass = trim($pass);
	$memo = $_POST['memo'];

	if (!$email) { $ERROR[] = "メールアドレスが入力されておりません。"; }
	if (!$pass) { $ERROR[] = "パスワードが入力されておりません。"; }

	if (!$ERROR) {
		$check = 0;
		$sql =	"SELECT name_s FROM kojin" .
				" WHERE saku!='1' AND email='$email' AND pass='$pass' AND kojin_num BETWEEN 100001 AND 600000 LIMIT 1;";
		if ($result = pg_query($db,$sql)) {
			$list = pg_fetch_array($result);
			$name_s = $list['name_s'];
			if ($name_s) { $check = 1; }
		}
		if ($check == 0) { $ERROR[] = "登録されてないか入力された情報が間違っています。"; }
	}

	if (!$ERROR) {
		$_SESSION['idpass'] = "$email<>$pass<>$memo<>";
		if ($memo == 1) {
			setcookie("idpass","$email<>$pass<>$memo<>",time() + 60*60*24*30);
		}
	}

	if ($ERROR) {
		unset($_SESSION['idpass']);
		$_SESSION['ERROR'] = $ERROR;
		$_SESSION['emailpassmemo'] = $idpass;
	}

	header ("Location: $PHP_SELF\n\n");

	exit;

}
?>
