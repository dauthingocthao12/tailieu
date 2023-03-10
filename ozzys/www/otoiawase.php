<?PHP

$PRF_N	= array('','北海道','青森県','岩手県','宮城県','秋田県','山形県','福島県','茨城県','栃木県','群馬県',
	'埼玉県','千葉県','東京都','神奈川県','新潟県','富山県','石川県','福井県','山梨県','長野県',
	'岐阜県','静岡県','愛知県','三重県','滋賀県','京都府','大阪府','兵庫県','奈良県','和歌山県',
	'鳥取県','島根県','岡山県','広島県','山口県','徳島県','香川県','愛媛県','高知県','福岡県',
	'佐賀県','長崎県','熊本県','大分県','宮崎県','鹿児島県','沖縄県');

//$admin_mail = 'ookawara@alphatec.co.jp';	// 管理用メールアドレス
$admin_mail = 'info@ozzys.jp';	// 管理用メールアドレス
$admin_url = 'http://www.ozzys.jp';	// 管理用メールアドレス

$admin_name = 'ozzys';	// メール送信者名

$m_footer = <<<OZZYS
*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/
Fishing Pro Shop Ozzy's

〒373-0853
群馬県太田市浜町63-31
TEL    : 0276-49-2021
URL    : http://www.ozzys.jp/
E-mail : $admin_mail
*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/
OZZYS;


if ($mode == "check") {
	## $zip1	= mb_convert_kana($zip1,n,"EUC-JP");
	## $zip2	= mb_convert_kana($zip2,n,"EUC-JP");
	## $tel	= mb_convert_kana($tel,n,"EUC-JP");
  $zip1	= mb_convert_kana($zip1,'n',"UTF-8");
	$zip2	= mb_convert_kana($zip2,'n',"UTF-8");
	 $tel	= mb_convert_kana($tel,'n',"UTF-8");
	if (!$name) { $ERROR[] = "お名前が入力されておりません。"; }
	if (!$email) { $ERROR[] = "E-mailアドレスが入力されておりません。"; }
##	if ($email && !eregi("^[a-zA-Z0-9_\.\-]+@(([a-zA-Z0-9_\-]+\.)+[a-zA-Z0-9]+$)",$email,$regs)) { $ERROR[] = "E-mailアドレスが不正です。"; }
if ($email && !preg_match("/^[a-zA-Z0-9_\.\-]+@(([a-zA-Z0-9_\-]+\.)+[a-zA-Z0-9]+$)/i",$email,$regs)) { $ERROR[] = "E-mailアドレスが不正です。"; }
	$mail_host = $regs[1];
	if ($email &&!getmxrr($mail_host,$mxhostarr)) { $ERROR[] = "E-mailアドレスのホスト名が見つかりませんでした。"; }
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
	if (!$msr) { $ERROR[] = "お問い合せ内容が入力されておりません。"; }
	}

	if ($mode == "check" && !$ERROR) { send(); }
	else { first(); }

	exit();

function first() {
global $PHP_SELF,$mode,$name,$email,$zip1,$zip2,$prf,$PRF_N,$add,$tel,$msr,$ERROR;

	if ($ERROR) {
		$er_msr = "";
		foreach($ERROR AS $val) {
			$er_msr .= "・$val <BR>\n";
			}
		$ERROR_M = <<<OZZYS
                  <TABLE border="0" cellpadding="0" cellspacing="0" width="200">
                    <TBODY>
                      <TR>
                        <TD class="bor3" nowrap><FONT color="#ff0000"><B>エラー</B></FONT><BR>
                        $er_msr</TD>
                      </TR>
                      <TR>
                        <TD class="bor3"><BR>
                        </TD>
                      </TR>
                    </TBODY>
                  </TABLE>
OZZYS;

		}


	echo <<<OZZYS
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<HTML>
<HEAD>
<META http-equiv="Content-Type" content="text/html; charset=UTF-8">
<META http-equiv="Content-Style-Type" content="text/css">
<TITLE>お問い合せ</TITLE>
<STYLE type="text/css">
<!--
BODY{
  margin-top : 10px;
  margin-left : 10px;
  margin-right : 10px;
  margin-bottom : 10px;
}
FONT{
  font-size:12px;
}
DIV{
  background-color:#ffcc00;
  padding:2px;
  border-style:solid;
  border-color:#666666;
  border-width:1px;
}
TD.bor1{
  border-width:1px 1px 0px;
  border-color:#333333;
  border-style:solid;
}
TD.bor2{
  border-width:1px;
  border-color:#333333;
  border-style:solid;
}
TD.bor3{
  border-style:double;
  border-color:#666666;
  border-width:3px 0px 0px 0px;
  padding:4px;
  margin:4px;
}
TD.bor4{
  border-style:double;
  border-color:#666666;
  border-width:3px 0px 3px 0px;
  padding:4px;
  margin:4px;
}
-->
</STYLE>
</HEAD>
<BODY>
<TABLE border="0" width="630" cellpadding="0" cellspacing="0">
  <TBODY>
    <TR>
      <TD align="center">
      <TABLE border="0" width="550" cellpadding="1" cellspacing="0">
        <TBODY>
          <TR>
            <TD class="bor1" bgcolor="#999999" align="center" background="image/line_1.gif"><IMG src="image/conte_04.gif" width="135" height="21" border="0" alt="お問合せ"></TD>
          </TR>
          <TR>
            <TD class="bor2" align="center">
            <FORM action="$PHP_SELF" method="POST"><INPUT type="hidden" name="mode" value="check"><BR>
$ERROR_M
            <TABLE border="0" cellpadding="0" cellspacing="0">
              <TBODY>
                <TR>
                  <TD class="bor3" valign="middle" bgcolor="#cccccc">
                  <DIV><FONT color="#333333" size="-1"><B>氏名 </B></FONT></DIV>
                  </TD>
                  <TD class="bor3" width="20"><IMG src="image/1pixel.gif" width="1" height="1" border="0"></TD>
                  <TD class="bor3"><INPUT size="20" type="text" name="name" value="$name"><FONT color="#ff0000">（必須項目）</FONT></TD>
                </TR>
                <TR>
                  <TD class="bor3" valign="middle" bgcolor="#cccccc">
                  <DIV><FONT color="#333333" size="-1"><B>E-Mail </B></FONT></DIV>
                  </TD>
                  <TD class="bor3"><IMG src="image/1pixel.gif" width="1" height="1" border="0"></TD>
                  <TD class="bor3"><INPUT size="40" type="text" name="email" value="$email"><FONT color="#ff0000">（必須項目）</FONT></TD>
                </TR>
                <TR>
                  <TD class="bor3" bgcolor="#cccccc">
                  <DIV><B><FONT color="#333333" size="-1">住所 </FONT></B></DIV>
                  </TD>
                  <TD class="bor3"><IMG src="image/1pixel.gif" width="1" height="1" border="0"></TD>
                  <TD class="bor3">
                  <TABLE border="0" cellpadding="0" cellspacing="2">
                    <TBODY>
                      <TR>
                        <TD width="60"><FONT color="#000000">郵便番号</FONT></TD>
                        <TD><INPUT size="6" type="text" name="zip1" maxlength="3" value="$zip1">-<INPUT size="8" type="text" name="zip2" maxlength="4" value="$zip2"></TD>
                      </TR>
                      <TR>
                        <TD><FONT color="#000000">都道府県</FONT></TD>
                        <TD><SELECT size="1" name="prf">
OZZYS;

	if (!$prf) { $selected = "selected"; } else { $selected = ""; }

	echo ("                                <OPTION value=\"\" $selected>選択してください</OPTION>\n");

	for($i=1; $i<=47; $i++) {
		if ($prf == $i) { $selected = "selected"; } else { $selected = ""; }
		echo ("                                <OPTION value=\"$i\" $selected>$PRF_N[$i]</OPTION>\n");
		}

	echo <<<OZZYS
                        </SELECT></TD>
                      </TR>
                      <TR>
                        <TD colspan="2"><FONT color="#000000">市町村名・町名・番地・建物名等</FONT></TD>
                      </TR>
                    </TBODY>
                  </TABLE>
                  <TEXTAREA rows="3" cols="40" name="add">$add</TEXTAREA></TD>
                </TR>
                <TR>
                  <TD class="bor3" valign="middle" bgcolor="#cccccc">
                  <DIV><B><FONT color="#333333">TEL </FONT></B></DIV>
                  </TD>
                  <TD class="bor3"><IMG src="image/1pixel.gif" width="1" height="1" border="0"></TD>
                  <TD class="bor3"><INPUT size="20" type="text" name="tel" value="$tel"></TD>
                </TR>
                <TR>
                  <TD class="bor4" valign="middle" bgcolor="#cccccc">
                  <DIV><FONT color="#333333"><B>お問合せ内容 </B></FONT></DIV>
                  </TD>
                  <TD class="bor4"><IMG src="image/1pixel.gif" width="1" height="1" border="0"></TD>
                  <TD class="bor4"><TEXTAREA rows="10" cols="50" name="msr">$msr</TEXTAREA></TD>
                </TR>
              </TBODY>
            </TABLE>
            <BR>
            <INPUT type="submit" value="送信">　<INPUT type="reset" value="リセット"></FORM>
            </TD>
          </TR>
        </TBODY>
      </TABLE>
      </TD>
    </TR>
  </TBODY>
</TABLE>
</BODY>
</HTML>
OZZYS;

exit();

}


function send() {
global $PHP_SELF,$mode,$name,$email,$zip1,$zip2,$prf,$PRF_N,$add,$tel,$msr,$ERROR,$admin_mail,$admin_url,$m_footer,$admin_name;

// 受け取りよう

$subject = "お問合せ - ozzys -";

$addr = getenv("REMOTE_ADDR");
$host = gethostbyaddr($addr);
if (!$host) { $host = $addr; }

$mail_msg = <<<OZZYS
$subject
----------------------------------------------------------
・お名前
 $name
・E-Mailアドレス
 $email
・住所
 〒$zip1 - $zip2
 $PRF_N[$prf] $add
・電話番号
 $tel
----------------------------------------------------------
・お問合せ内容
 $msr
----------------------------------------------------------
$host ($addr)
OZZYS;

$from_mail = "From: $email <$email>\nReply-To:$email";

mb_send_mail ( $admin_mail, $subject, $mail_msg , $from_mail , "-f$admin_mail");

// お客様送信用

$from_mail = "From: $admin_name <$admin_mail>\nReply-To:$admin_mail";

$subject = "お問合せ有り難う御座いました。 - Ozzy's -";


$mail_msg = <<<OZZYS
$name 様 $mail_sub
以下の内容で間違いありませんでしょうか。
間違いがあるようでしたらご連絡ください。
----------------------------------------------------------
・お名前
 $name
・E-Mailアドレス
 $email
・住所
 〒$zip1 - $zip2
 $PRF_N[$prf] $add
・電話番号
 $tel
----------------------------------------------------------
・お問合せ内容
 $msr
----------------------------------------------------------

$m_footer
OZZYS;

mb_send_mail ( $email, $subject, $mail_msg , $from_mail , "-f$admin_mail");

header ("Location: $admin_url/thank_toi.htm\n\n");

}

?>
