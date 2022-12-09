<?PHP
//	メールマガジン管理	************************************************************************
//	メール作成
function meruma_1()
{
	global $ERROR;
	$mode = $_POST['mode'];
	$subject = $_POST['subject'];
	$msr = $_POST['msr'];

	if ($mode == "") {
		first_00();
	} elseif ($mode == "write") {
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
	// global $PHP_SELF, $main, $mail, $ERROR, $mail_num, $subject, $msr;

	global $PHP_SELF,$ERROR;
	global $msg; //@source first_01()
	$main=$_POST['main'];
	$mail=$_POST['mail'];
	$mail_num=$_POST['mail_num'];
	$subject=$_POST['subject'];
	$msr=$_POST['msr'];

	$subject = $_POST['subject'];
	$msr = $_POST['msr'];
	include './array.inc';
	include "../../cone.inc";

	$sql = "select * from mail where s_num='0';";
	$sql1 = pg_exec($conn_id, $sql);
	$count = pg_numrows($sql1);

	if ($count >= 1) {
		list($mail_num, $subject, $msr, $s_date, $s_num) = pg_fetch_array($sql1, 0);
	} else {
		$sql = "select mail_num from mail ORDER BY mail_num;";
		$sql1 = pg_exec($conn_id, $sql);
		$count = pg_numrows($sql1);
		if ($count >= 1) {
			$sql = "select MAX(mail_num) from mail;";
			$sql1 = pg_exec($conn_id, $sql);
			$count = pg_numrows($sql1);
			if ($count >= 1) {
				list($mail_num) = pg_fetch_array($sql1, 0);
				++$mail_num;
			}
		} else {
			$mail_num = 1;
		}
	}

	if ($subject == "") {
		$subject = "ネイバーズスポーツ メールマガジン";
	}

	if ($ERROR) {
		echo ("<FONT color=\"#ff0000\">エラー</FONT><BR>\n");
		foreach ($ERROR as $val) {
			echo ("・$val <BR>\n");
		}
	}

	echo <<<OZZYS
$msg
<BR>
メッセージを作成し登録を押して下さい。
<FORM action="$PHP_SELF" method="POST">
<INPUT type="hidden" name="main" value="$main">
<INPUT type="hidden" name="mail" value="$mail">
<INPUT type="hidden" name="mode" value="write">
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
      <TD colspan="2" bgcolor="#cccccc" align="center"><INPUT type="submit" value="登録">　<INPUT type="reset"></TD>
    </TR>
  </TBODY>
</TABLE>
</FORM>
OZZYS;

	pg_close($conn_id);
}


function first_01()
{
	// global $PHP_SELF,$main,$mail,$mode,$ERROR,$mail_num,$subject,$msr,$msg;
	global $msg;
	$mail_num=$_POST['mail_num'];
	$subject=$_POST['subject'];
	$msr=$_POST['msr'];
	
	include "../../cone.inc";

	if (!$s_date) {
		$s_date = "1000-01-01";
	}
	if (!$s_num) {
		$s_num = 0;
	}

	$sql = "select mail_num from mail where mail_num='$mail_num';";
	$sql1 = pg_exec($conn_id, $sql);
	$count = pg_numrows($sql1);

	if ($count >= 1) {
		$sql  = "update mail set" .
			" subject='$subject'," .
			" msr='$msr'" .
			" where mail_num='$mail_num';";
	} else {
		$sql =  "insert into mail values (" .
			"'$mail_num'," .
			"'$subject'," .
			"'$msr'," .
			"'$s_date'," .
			"'$s_num');";
	}
	$sql1 = pg_exec($conn_id, $sql);
	$count = pg_numrows($sql1);

	if ($count >= 0) {
		$msg = "メッセージを登録しました。";
	} else {
		$msg = "メッセージを登録できませんでした。";
	}

	first_00();
}
