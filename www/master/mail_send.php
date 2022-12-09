<?PHP

	$check=$_POST["check"];
	$email=$_POST["email"];
	$subject=$_POST["subject"];
	$msg=$_POST["msg"];

	if ($check != "check") { $ERROR[] = "不正表示の可能性があります。"; }
	if (!$email) { $ERROR[] = "メールアドレスが入力されておりません。"; }
	if (!$subject) { $ERROR[] = "メールタイトルが入力されておりません。"; }
	if (!$msg) { $ERROR[] = "メール本文が入力されておりません。"; }

	if ($ERROR) { ERROR(); }

	//	サーバー変更対策05/11/01
	$subject = $subject;
	include '../sub/array.inc';
	include '../../cone.inc';
	include '../sub/mail.inc';

	$send_email = $admin_mail;
	$send_name = $admin_name;
	$get_email = $email;
	send_email($send_email,$send_name,$mail_bcc,$bcc_email,$get_email,$subject,$msg);

##	$msg = ereg_replace("\r","",$msg);
	$msg = preg_replace("/\r/","",$msg);
##	$msg = ereg_replace("\n","<BR>\n",$msg);
	$msg = preg_replace("/\n/","<BR>\n",$msg);

	echo<<<ALPHA
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<HTML>
<HEAD>
<META http-equiv="Content-Type" content="text/html; charset=UTF-8">
<META http-equiv="Content-Style-Type" content="text/css">
<TITLE>発送メール完了</TITLE>
</HEAD>
<BODY>
<FORM>
以下の内容でメールをお送りしました。
<TABLE border="0" cellpadding="4" cellspacing="1" bgcolor="#666666">
  <TBODY>
    <TR>
      <TD bgcolor="#cccccc">メールアドレス</TD>
      <TD bgcolor="#ffffff">$email</TD>
    </TR>
    <TR>
      <TD bgcolor="#cccccc">メールタイトル</TD>
      <TD bgcolor="#ffffff">$subject</TD>
    </TR>
    <TR>
      <TD bgcolor="#cccccc">メール本文</TD>
      <TD bgcolor="#ffffff">$msg</TD>
    </TR>
    <TR>
      <TD bgcolor="#ffffff" colspan="2" align="center"><INPUT type="submit" value="閉じる" onClick="window.close()"></TD>
    </TR>
  </TBODY>
</TABLE>
</FORM>

ALPHA;


function ERROR() {
global $ERROR;

	$max = count($ERROR);
	for($i=0; $i<$max; $i++) {
		$errors .= "・$ERROR[$i] <BR>\n";
	}

	echo <<<ALPHA
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<HTML>
<HEAD>
<META http-equiv="Content-Type" content="text/html; charset=UTF-8">
<META http-equiv="Content-Style-Type" content="text/css">
<TITLE>エラー 発送メール</TITLE>
</HEAD>
<BODY>
<BR>
<B><FONT color="#ff0000">エラー</FONT></B><BR>
$errors
</BODY>
</HTML>

ALPHA;

exit;

}

?>
