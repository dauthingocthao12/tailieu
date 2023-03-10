<?PHP
//	会員脱会
function secession() {

	$mode = $_POST['mode'];

	if ($mode == "check") { $ERROR = sent(); }

	$html = first($ERROR);

	return $html;

}



//	初期画面
function first($ERROR) {
global $PHP_SELF;

	if ($ERROR) {
		$errors = ERROR($ERROR);
		$c_email = $_POST['c_email'];
	}

	$html = <<<WAKABA
		<h2 class="sub-title-prod">会員脱会</h2>

$errors

	<TABLE class="table-resp table-resp-nogap spaced">
		<TBODY>
		<TR bgcolor="#ffcc00">
			<TD>●<B>注意事項</B></TD>
		</TR>
		<TR bgcolor="#ffffff">
			<TD>
				<p class="spaced">
					　<FONT color="#ffcc00">●</FONT>会員脱会を希望される方は、下の入力欄に確認のためメールアドレスとパスワードを入力後、「<B>会員脱会</B>」というボタンをクリックして先へ進んでください。
				</p>
			</TD>
		</TR>
		</TBODY>
	</TABLE>

	<FORM action="$PHP_SELF" method="POST">
		<INPUT type="hidden" name="mode" value="check">

		<CENTER class="login-form spaced">
			<TABLE class="table-resp">
			<TBODY>
				<TR>
					<th>メールアドレス</th>
					<TD><INPUT class="input-block" name="c_email" value="$c_email"></TD>
				</TR>
				<TR>
					<th>パスワード</th>
					<TD><INPUT class="input-block" type="password" name="c_pass" value=""></TD>
				</TR>
			</TBODY>
			</TABLE>

			<BR>
			<INPUT type="image" alt="会員脱会" src="/images/secession.gif"><BR>
			<BR>
		</CENTER>
	</FORM>

WAKABA;

	return $html;

}



//	ユーザー確認
function sent() {
global $db,$admin_mail,$admin_name,$m_footer;

	$c_email = $_POST['c_email'];
	## $c_email = mb_convert_kana($c_email,"as","EUC-JP");
	$c_email = mb_convert_kana($c_email,"as","UTF-8");
	$c_email = trim($c_email);
	$c_pass = $_POST['c_pass'];
	## $c_pass = mb_convert_kana($c_pass,"as","EUC-JP");
	$c_pass = mb_convert_kana($c_pass,"as","UTF-8");
	$c_pass = trim($c_pass);
	if (!$c_email) { $ERROR[] = "メールアドレスが入力されておりません。"; }
	if (!$c_pass) { $ERROR[] = "パスワードが入力されておりません。"; }

	if (!$ERROR) {
		$check = 0;
		$sql =	"SELECT kojin_num, name_s, name_n, email, pass FROM kojin" .
				" WHERE saku!='1' AND email='$c_email' AND pass='$c_pass' AND kojin_num BETWEEN 100001 AND 600000 LIMIT 1;";
		if ($result = pg_query($db,$sql)) {
			$list = pg_fetch_array($result);
			$kojin_num = $list['kojin_num'];
			$name_s = $list['name_s'];
			$name_n = $list['name_n'];
			$email = $list['email'];
			$pass = $list['pass'];
		}
		if (!$kojin_num) { $ERROR[] = "入力された情報は登録されていないか間違っています。"; }
	}

	if (!$ERROR) {
		$sql =  "UPDATE kojin SET " .
				"saku='1' " .
				"WHERE kojin_num='$kojin_num';";
		if (!$result = pg_query($db,$sql)) { $ERROR[] = "脱会処理ができませんでした。"; }
	}

	if (!$ERROR) {

		// お客様送信用
		//	未送信

		// 受け取りよう
		$subject = "脱会のお知らせ ( No.$kojin_num )";

		$addr = getenv("REMOTE_ADDR");
		$host = gethostbyaddr($addr);
		if (!$host) { $host = $addr; }

		$msr = <<<OZZYS
$subject
------------------------------------------------------

会員番号：No.$kojin_num
お名前：$name_s $name_n
E-mail：$email

以上の方が退会されました。

------------------------------------------------------
$host ($addr)
OZZYS;

		$send_email = $email;
		$send_name = "$name_s $name_n";
		$email = $admin_mail;
		send_email($send_email,$send_name,$mail_get,$get_email,$email,$subject,$msr);

		unset($_SESSION['idpass']);
		setcookie("idpass");

		$url = "/endhtml/secession.htm";
		header ("Location: $url\n\n");
		exit;
	}

	return $ERROR;

}



//	エラー表示
function ERROR($ERROR) {

	foreach ($ERROR AS $VAL) {
		if ($VAL) { $err .= "・$VAL<BR>\n"; }
	}

	$errors = <<<WAKABA
      <TABLE border="0" width="95%">
        <TBODY>
          <TR>
            <TD>
            <B><FONT color="#ff0000">エラー</FONT></B><BR>
$err
            </TD>
          </TR>
        </TBODY>
      </TABLE>
      <BR>

WAKABA;

	return $errors;

}
?>
