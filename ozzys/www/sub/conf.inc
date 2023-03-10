<?PHP
//	パスワード確認
function conf() {

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
		$email = $_POST['email'];
	}

	$html = <<<WAKABA
		<h2 class="sub-title-prod">パスワード確認</h2>

$errors

      <FORM action="$PHP_SELF" method="POST">
      <INPUT type="hidden" name="mode" value="check">
      <TABLE class="table-resp table-resp-nogap">
        <TBODY>
          <TR bgcolor="#ffcc00">
            <TD class="tdpadding">●<B>注意事項</B></TD>
          </TR>
          <TR bgcolor="#ffffff">
			<TD class="tdpadding">
				<p class="spaced">
					　<FONT color="#ffcc00">●</FONT>会員登録時に登録したメールアドレスを入力し確認を押して下さい。その入力されたメールアドレスに、パスワードが送信されます。<BR>
				</p>

				<p class="spaced">
					　<FONT color="#ffcc00">●</FONT>登録時のメールアドレスを忘れた場合は、お手数ですがお問い合せのページからお問い合せ頂くか、新規に登録お願い致します。<BR>
					　　 （新規登録した場合は、獲得したポイントがある場合無効になります。）
				</p>

				<p class="text-center">
					<label>メールアドレス : </label>
					<INPUT class="input-inline" name="email" value="$email">
				</p>

            </TD>
          </TR>
        </TBODY>
      </TABLE>
      <BR>

	  <center>
		  <INPUT type="image" alt="パスワード確認" src="/images/puss_k.gif"><BR>
	  </center>

      <BR>
      </FORM>

WAKABA;

	return $html;

}



//	ユーザー確認
function sent() {
global $db,$admin_mail,$admin_name,$m_footer;

	$email = $_POST['email'];
	## $email = mb_convert_kana($email,"as","EUC-JP");
	$email = mb_convert_kana($email,"as","UTF-8");
	$email = trim($email);
	if (!$email) { $ERROR[] = "メールアドレスが入力されておりません。"; }

	if (!$ERROR) {
		$check = 0;
		$sql =	"SELECT kojin_num, name_s, name_n, email, pass FROM kojin" .
				" WHERE saku!='1' AND email='$email' AND kojin_num BETWEEN 100001 AND 600000 LIMIT 1;";
		if ($result = pg_query($db,$sql)) {
			$list = pg_fetch_array($result);
			$kojin_num = $list['kojin_num'];
			$name_s = $list['name_s'];
			$name_n = $list['name_n'];
			$email = $list['email'];
			$pass = $list['pass'];
		}
		if (!$kojin_num) { $ERROR[] = "入力されたメールアドレスは登録されていないか間違っています。"; }
	}


	if (!$ERROR) {

		// お客様送信用
		$subject = "パスワードのご連絡 - ozzys -";

		$msr = <<<OZZYS
$subject
$name_s $name_n 様 パスワードをご連絡させていただきます。
------------------------------------------------------

登録E-mailアドレス
　$email

登録パスワード
　$pass

$m_footer
OZZYS;

		$send_email = $admin_mail;
		$send_name = $admin_name;
		send_email($send_email,$send_name,$mail_get,$get_email,$email,$subject,$msr);

		// 受け取りよう
		$subject = "パスワード確認されました。(No.$kojin_num) - ozzys -";

		$addr = getenv("REMOTE_ADDR");
		$host = gethostbyaddr($addr);
		if (!$host) { $host = $addr; }

		$msr = <<<OZZYS
$name_s $name_n 様が $subject
------------------------------------------------------

登録E-mailアドレス
　$email

登録パスワード
　$pass

------------------------------------------------------
$host ($addr)
OZZYS;

		$send_email = $email;
		$send_name = "$name_s $name_n";
		$email = $admin_mail;
		send_email($send_email,$send_name,$mail_get,$get_email,$email,$subject,$msr);

		$url = "/endhtml/thank_pass.htm";
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
