<?PHP
//	ログイン
function login() {
	global $conn_id; // mysql DB

	if ($_GET['url']) {
		$burl = $_GET['url'];
		$_SESSION['blurl'] = $burl;
	}

	$idpass = $_SESSION['idpass'];
	$burl = $_SESSION['blurl'];
	$mode = $_POST['mode'];

	if ($mode == "checkuser") { checkuser(); }

	if ($idpass) {
		//	ログインチェック
		//	add ookawara 2013/07/29 start
		$kojin_num = 0;
		list($email, $pass, $check, $af_num) = explode("<>", $idpass);
		$sql  = "SELECT kojin_num, name_s FROM kojin".
				" WHERE email='".$email."'".
				" AND pass='".$pass."'".
				" AND saku!='1'".
				" AND kojin_num<'100000'".
				" ORDER BY kojin_num;";
		if ($result = mysqli_query($conn_id, $sql)) {
			$list = mysqli_fetch_array($result);
			$kojin_num = $list['kojin_num'];
			$name_s = $list['name_s'];
#			$name_s = mb_convert_encoding($name_s,"utf-8","euc-jp");
		}
		if ($kojin_num < 1) {
			unset($idpass);
			unset($_SESSION['idpass']);
			unset($_COOKIE['idpass']);
			setcookie("idpass");
			unset($af_num);
		} else {
		//	add ookawara 2013/07/29 end

			unset($_SESSION['blurl']);
			if (!$burl) { $burl = "/"; }
			header ("Location: $burl\n\n");
		}	//	add ookawara 2013/07/29
	}

	$html = first();

	return $html;

}



//	ログインページ
function first() {
global $PHP_SELF;

//	unset($_SESSION['idpass']);

	$ERROR = $_SESSION['INERROR'];
	if ($ERROR) {
		$errors = "<FONT color=\"#ff0000\"><B>エラー</B></FONT><BR>\n";
		foreach ($ERROR AS $VAL) {
			if ($VAL) { $errors .= $VAL . "<BR>\n"; }
		}
		$errors .= "<BR>\n";
		unset($_SESSION['INERROR']);
		unset($ERROR);

		if ($_SESSION['emailpassmemo']) {
			$emailpassmemo = $_SESSION['emailpassmemo'];
			list($email,$pass,$check) = explode("<>",$emailpassmemo);
		}
		unset($_SESSION['emailpassmemo']);
	}

	if ($check) { $checked = "checked"; } else { $checked = ""; }

	$html = <<<WAKABA
<div class="con_name"><div class="con_text">ログイン</div></div>
<FORM action="$PHP_SELF" method="POST">
<INPUT type="hidden" name="mode" value="checkuser">
$errors<br />
<p>会員登録時に登録したメールアドレスと、パスワードを入力しログインボタンを押してください。<BR>
自動ログインにチェックを入れますと、ブラウザを終了してもログアウトしないように設定されます。</p>
<table>
<tr>
<th width="150px" class="cate1">メールアドレス</th>
<td class="cate3"><INPUT size="40" type="text" name="email" value="$email"> (半角)</td>
</tr>
<tr>
<th class="cate1">パスワード</th>
<td class="cate3"><input size="16" type="password" maxlength="8" name="pass" value="$pass"> (半角)</td>
</tr>
<tr>
<th class="cate1">自動ログイン</th>
<td class="cate3"><INPUT type="checkbox" name="check" value="1" $checked>：自動ログインする</td>
</tr>
<tr>
<td align="center" colspan="2"><INPUT type="submit" value="ログイン">　<INPUT type="reset"></Td>
</tr>
</table>
</form>
<p>
会員登録をしていない方は
<A href="/member/">こちらから会員登録</A>してください。<BR>
</p>

</div>		
<div class="clear"><br></div>

WAKABA;

	return $html;

}



//	ユーザー確認
function checkuser() {
global $PHP_SELF,$conn_id,$admin_url;

	$email = $_POST['email'];
#	$email = mb_convert_kana($email,"as","EUC-JP");
	$email = mb_convert_kana($email,"as","UTF-8");
	$email = trim($email);
	$pass = $_POST['pass'];
#	$pass = mb_convert_kana($pass,"as","EUC-JP");
	$pass = mb_convert_kana($pass,"as","UTF-8");
	$pass = trim($pass);
	$check = $_POST['check'];

	if (!$email) { $ERROR[] = "メールアドレスが入力されておりません。"; }
	if (!$pass) { $ERROR[] = "パスワードが入力されておりません。"; }

	if (!$ERROR) {
		$flag = 0;
		$sql  = "SELECT kojin_num, name_s FROM kojin" .
				" WHERE email='$email' AND pass='$pass' AND saku!='1' AND kojin_num<'100000' ORDER BY kojin_num;";
		if ($result = mysqli_query($conn_id,$sql)) {
			$list = mysqli_fetch_array($result);
			$kojin_num = $list['kojin_num'];
			$name_s = $list['name_s'];
			if ($name_s) { $flag = 1; }
		}
		if ($flag == 0) { $ERROR[] = "登録されてないか入力された情報が間違っています。"; }
		else {
			$sql = "SELECT af_num FROM afuser WHERE kojin_num='$kojin_num' AND state!='1';";
			if ($result = mysqli_query($conn_id,$sql)) {
				$list = mysqli_fetch_array($result);
				$af_num = $list['af_num'];
			}
		}
	}

	if (!$ERROR) {
		$idpass = "$email<>$pass<>$check<>$af_num<>";
		$_SESSION['idpass'] = $idpass;
		unset($_COOKIE['idpass']);
		setcookie("idpass");
		if ($check == 1) {
			setcookie("idpass",$idpass,time() + 60*60*24*30,"/",".futboljersey.com");
		}
	}

	if ($ERROR) {
		unset($_SESSION['idpass']);
		unset($_COOKIE['idpass']);
		setcookie("idpass");
		$_SESSION['INERROR'] = $ERROR;
		$_SESSION['emailpassmemo'] = $idpass;
	}

//	$url = $admin_url.preg_match("/^\//","",$PHP_SELF);	//	del ookawara 2009/09/29

	//	add ookawara 2009/09/29
	$url = "/";
	if ($_SESSION['idpass'] && $_SESSION['blurl']) {
		$url = $_SESSION['blurl'];
		unset($_SESSION['blurl']);
	}

	header ("Location: $url\n\n");

	exit;

}
?>
