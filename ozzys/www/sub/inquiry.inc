<?PHP
//	お問合わせ
function inquiry() {

	$mode = $_POST['mode'];

	if ($mode == "check") { $ERROR = sent(); }

	$html = first($ERROR);

	return $html;

}



//	初期画面
function first($ERROR) {
global $PHP_SELF,$PRF_L;

	if ($ERROR) {
		$errors = ERROR($ERROR);
		$name = $_POST['name'];
		$email = $_POST['email'];
		$zip1 = $_POST['zip1'];
		$zip2 = $_POST['zip2'];
		$prf = $_POST['prf'];
		$add = $_POST['add'];
		$tel = $_POST['tel'];
		$msg = $_POST['msg'];
	}

	if (!$prf) { $selected = "selected"; } else { $selected = ""; }
	$l_prf = "                  <OPTION value=\"\" $selected>選択してください</OPTION>\n";
	for($i=1; $i<=47; $i++) {
		if ($prf == $i) { $selected = "selected"; } else { $selected = ""; }
		$l_prf .= "                  <OPTION value=\"$i\" $selected>$PRF_L[$i]</OPTION>\n";
	}

	$html = <<<WAKABA
		<h2 class="sub-title-prod">お問合わせ</h2>
$errors
      <FORM action="$PHP_SELF" method="POST">
      <INPUT type="hidden" name="mode" value="check">
		<TABLE class="table-inquiry spaced">
			<TBODY>
				<TR>
					<th>
						<DIV class="form">氏名 </DIV>
					</th>
					<TD>
						<INPUT class="input-inline" type="text" name="name" value="$name"> <FONT color="#ff0000">（必須項目）</FONT>
					</TD>
				</TR>
				<TR>
					<th>
						<DIV class="form">E-Mail </DIV>
					</th>
					<TD>
						<INPUT class="input-inline" type="text" name="email" value="$email"> <FONT color="#ff0000">（必須項目）</FONT>
					</TD>
				</TR>
				<TR>
					<th>
						<DIV class="form">住所 </DIV>
					</th>
					<TD>
						<div>郵便番号</div>
						<div class="form-group">
							<INPUT class="input-inline" size="6" type="text" name="zip1" maxlength="3" value="$zip1">-<INPUT class="input-inline" size="8" type="text" name="zip2" maxlength="4" value="$zip2"></div>
						<div>都道府県</div>
						<div class="form-group">
							<SELECT class="input-inline" size="1" name="prf">
								$l_prf
							</SELECT>
						</div>
						<div>市町村名・町名・番地・建物名等</div>
						<TEXTAREA class="input-block" rows="3" name="add">$add</TEXTAREA>
					</TD>
				</TR>
				<TR>
					<th>
						<DIV class="form">TEL </DIV>
					</th>
					<TD><INPUT class="input-inline" type="text" name="tel" value="$tel"></TD>
				</TR>
				<TR>
					<th>
						<DIV class="form">お問合せ内容 </DIV>
					</th>
					<TD>
						<TEXTAREA class="input-block" rows="10" name="msg">$msg</TEXTAREA>
					</TD>
				</TR>
			</TBODY>
		</TABLE>

	  <p class="text-center spaced">
		  <button class="btn btn-submit" type="submit">送信</button>　<button class="btn btn-reset" type="reset">リセット</button>
	  </p>

	</FORM>

WAKABA;

	return $html;

}



//	ユーザー確認
function sent() {
global $PRF_L,$admin_mail,$admin_name,$m_footer;

	$name = $_POST['name'];
	## $name = mb_convert_kana($name,"AsKV","EUC-JP");
	$name = mb_convert_kana($name,"AsKV","UTF-8");
	$name = trim($name);
	$email = $_POST['email'];
	## $email = mb_convert_kana($email,"as","EUC-JP");
	$email = mb_convert_kana($email,"as","UTF-8");
	$email = trim($email);
	$email = strtolower($email);
	$zip1 = $_POST['zip1'];
	## $zip1 = mb_convert_kana($zip1,"na","EUC-JP");
	$zip1 = mb_convert_kana($zip1,"na","UTF-8");
	$zip1 = trim($zip1);
	$zip2 = $_POST['zip2'];
	## $zip2 = mb_convert_kana($zip2,"na","EUC-JP");
	$zip2 = mb_convert_kana($zip2,"na","UTF-8");
	$zip2 = trim($zip2);
	$prf = $_POST['prf'];
	$add = $_POST['add'];
	## $add = mb_convert_kana($add,"AsKV","EUC-JP");
	$add = mb_convert_kana($add,"AsKV","UTF-8");
	$add = trim($add);
	$tel = $_POST['tel'];
	## $tel = mb_convert_kana($tel,"ns","EUC-JP");
	$tel = mb_convert_kana($tel,"ns","UTF-8");
	$tel = trim($tel);
	$msg = $_POST['msg'];
	## $msg = mb_convert_kana($msg,"AsKV","EUC-JP");
	$msg = mb_convert_kana($msg,"AsKV","UTF-8");
	$msg = trim($msg);

	if (!$name) { $ERROR[] = "氏名が入力されておりません。"; }
	if (!$email) { $ERROR[] = "E-mailが入力されておりません。"; }
##	if ($email && !eregi("^[a-zA-Z0-9_\.\-]+@(([a-zA-Z0-9_\-]+\.)+[a-zA-Z0-9]+$)",$email,$regs)) { $ERROR[] = "E-mailが不正です。"; }
	if ($email && !preg_match("/^[a-zA-Z0-9_\.\-]+@(([a-zA-Z0-9_\-]+\.)+[a-zA-Z0-9]+$)/i",$email,$regs)) { $ERROR[] = "E-mailが不正です。"; }
//	if (!$zip1) { $ERROR[] = "郵便番号３桁が入力されておりません。"; }
	$zip1_n = strlen($zip1);
##	if ($zip1 && (!eregi("[0-9]",$zip1) || ($zip1_n != 3))) { $ERROR[] = "郵便番号３桁が不正です。"; }
	if ($zip1 && (!preg_match("/[0-9]/i",$zip1) || ($zip1_n != 3))) { $ERROR[] = "郵便番号３桁が不正です。"; }
//	if (!$zip2) { $ERROR[] = "郵便番号４桁が入力されておりません。"; }
	$zip2_n = strlen($zip2);
##	if ($zip2 && (!eregi("[0-9]",$zip2) || ($zip2_n != 4))) { $ERROR[] = "郵便番号４桁が不正です。"; }
	if ($zip2 && (!preg_match("/[0-9]/i",$zip2) || ($zip2_n != 4))) { $ERROR[] = "郵便番号４桁が不正です。"; }
//	if (!$prf) { $ERROR[] = "都道府県名が選択されておりません。"; }
//	if (!$add) { $ERROR[] = "市町村名・町名・番地・建物名等が入力されておりません。"; }
//	if (!$tel) { $ERROR[] = "電話番号が入力されておりません。"; }
	if (!$msg) { $ERROR[] = "お問い合せ内容が入力されておりません。"; }

	if (!$ERROR) {
		// お客様送信用
		$subject = "お問合せ有り難う御座います。 - Ozzy's -";

		$msr = <<<OZZYS
$name 様
以下の内容でよろしいでしょうか？
変更がある場合は、お手数ですが再度ご連絡お願い致します。
----------------------------------------------------------
・お名前
 $name
・E-Mailアドレス
 $email
・住所
 〒$zip1 - $zip2
 $PRF_L[$prf] $add
・電話番号
 $tel
----------------------------------------------------------
・お問合せ内容
 $msg
----------------------------------------------------------

$m_footer
OZZYS;

		$send_email = $admin_mail;
		$send_name = $admin_name;
		send_email($send_email,$send_name,$mail_get,$get_email,$email,$subject,$msr);

		//	受け取り用
		$subject = "お問合せ - ozzys -";

		$addr = getenv("REMOTE_ADDR");
		$host = gethostbyaddr($addr);
		if (!$host) { $host = $addr; }

		$msr = <<<OZZYS
$subject
----------------------------------------------------------
・お名前
 $name
・E-Mailアドレス
 $email
・住所
 〒$zip1 - $zip2
 $PRF_L[$prf] $add
・電話番号
 $tel
----------------------------------------------------------
・お問合せ内容
 $msg
----------------------------------------------------------
$host ($addr)
OZZYS;

		$send_email = $email;
		$send_name = "$name";
		$email = $admin_mail;
		send_email($send_email,$send_name,$mail_get,$get_email,$email,$subject,$msr);

		$url = "/endhtml/thank_inq.htm";
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
