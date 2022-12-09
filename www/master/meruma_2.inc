<?PHP
//	メールマガジン管理	************************************************************************
//	メール送信
function meruma_2()
{
	global $ERROR;
	$mode = $_POST['mode'];
	$subject = $_POST['subject'];
	$msr = $_POST['msr'];

	if ($mode == "") {
		first_00();
	} elseif ($mode == "send") {
		if (!$subject) {
			$ERROR[] = "メールタイトルが入力されておりません。";
		}
		if (!$msr) {
			$ERROR[] = "メッセージが入力されておりません。";
		}
		if ($ERROR) {
			first_00();
		} else {
			first_01();
		}
	}
}



function first_00()
{
	global $PHP_SELF;
	global $msg; // @source first_01()
	$main = $_POST['main'];
	$mail = $_POST['mail'];
	$mail_num = $_POST['mail_num'];
	$subject = $_POST['subject'];
	$msr = $_POST['msr'];

	include './array.inc';
	include "../../cone.inc";

	$sql = "select * from mail where s_num='0';";
	$sql1 = pg_exec($conn_id, $sql);
	$count = pg_numrows($sql1);

	if ($count >= 1) {
		list($mail_num, $subject, $msr, $s_date, $s_num) = pg_fetch_array($sql1, 0);

		$sql = "select kojin_num from kojin where meruma='1' AND saku='0';";
		$sql1 = pg_exec($conn_id, $sql);
		$sy_num = pg_numrows($sql1);

		echo <<<ALPHA
$msg
<BR>
下記の内容でメッセージを送信します。<BR>
よろしければ送信を押して下さい。<BR>
<BR>
送信予定数：$sy_num 件<BR>
<FORM action="$PHP_SELF" method="POST">
<INPUT type="hidden" name="main" value="$main">
<INPUT type="hidden" name="mail" value="$mail">
<INPUT type="hidden" name="mode" value="send">
<INPUT type="hidden" name="mail_num" value="$mail_num">
<TABLE border="0" bgcolor="#999999">
  <TBODY>
    <TR>
      <TD bgcolor="#cccccc" align="center"><B>メールタイトル</B></TD>
      <TD bgcolor="#ffffff"><INPUT size="60" type="text" name="subject" value="$subject"></TD>
    </TR>
    <TR>
      <TD bgcolor="#cccccc" colspan="2"><B>メッセージ</B></TD>
    </TR>
    <TR>
      <TD colspan="2" bgcolor="#ffffff"><TEXTAREA rows="20" cols="80" name="msr">$msr</TEXTAREA></TD>
    </TR>
    <TR>
      <TD colspan="2" bgcolor="#cccccc" align="center"><INPUT type="submit" value="送信">　<INPUT type="reset"></TD>
    </TR>
  </TBODY>
</TABLE>
</FORM>
ALPHA;
	} else {
		echo <<<ALPHA
<BR>
現在メッセージは登録されておりません。<BR>
メッセージ作成で作成して下さい。<BR>
ALPHA;
	}

	pg_close($conn_id);
}


function first_01()
{
	global $msg;
	$mail_num = $_POST['mail_num'];
	$subject = $_POST['subject'];
	$msr = $_POST['msr'];
	include './array.inc';
	include "../../cone.inc";
	include "../sub/array.inc";
	include "../sub/mail.inc";

	$sql = "select name_s, name_n, email from kojin where meruma='1' AND saku='0';";
	$sql1 = pg_exec($conn_id, $sql);
	$count = pg_numrows($sql1);
	$max = $count - 1;

	$max_time = $max * 2 + 60;
	if ($max_time < 600) {
		$max_time = 600;
	}
	set_time_limit($max_time);

	$subject = $subject;
	$send_email = $admin_mail_l;
	$send_name = $admin_name;
	$msg = $msr;

	unset($sent_list);
	for ($i = 0; $i <= $max; $i++) {
		list($name_s, $name_n, $email) = pg_fetch_array($sql1, $i);

		$get_email = $email;
		send_email($send_email, $send_name, $mail_bcc, $bcc_email, $get_email, $subject, $msg);

		$ban = $i + 1;
		$user_line = "[ $ban ] $name_s $name_n 様 e-mail : $email\n";
		echo ("$user_line<BR><hr>\n");
		$sent_list .= $user_line . "\n";
		//		sleep(1);
	}

	$subject = "　メルマガ送信完了リスト";
	$msg = $sent_list . "\n\n送信完了\n";

	$send_email = $admin_mail_l;
	$send_name = $admin_name;
	$get_email = $admin_mail_l;
	send_email($send_email, $send_name, $mail_bcc, $bcc_email, $get_email, $subject, $msg);

	$sql =  "update mail set " .
		" s_date='now()'," .
		" s_num='$count'" .
		" where mail_num='$mail_num';";
	$sql1 = pg_exec($conn_id, $sql);

	echo <<<ALPHA
<BR>
$count 件のメールを送信終了しました。
ALPHA;
}
