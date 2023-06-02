<?PHP
//	アフェリエイト管理
//　連絡メール送信
function aff_4() {
global $PHP_SELF,$conn_id;
include "../../cone.inc";

	$mode = $_POST['mode'];

	if ($mode == "check") {
		check($mode);
	}
	elseif ($mode == "send") {
		send($mode);
	}
	else {
		first($mode);
	}

}



//	初期画面
function first($mode) {
global $PHP_SELF,$conn_id;
include './array.inc';
include '../sub/array.inc';
include '../sub/setup.inc';

	if ($mode) {
		$subject = $_SESSION['msg_info']['subject'];
		$msr = $_SESSION['msg_info']['msr'];
	}

	unset($_SESSION['msg_info']);

	$main = $_POST['main'];
	$main_r = $_POST['main_r'];
	$aff = $_POST['aff'];
	$aff_r = $_POST['aff_r'];

	echo <<<WAKABA
<table border="0" cellpadding="0" cellspacing="0">
  <tbody>
    <tr>
      <td>
<FORM action="$PHP_SELF" method="POST">
<INPUT type="hidden" name="main" value="$main">
<INPUT type="hidden" name="main_r" value="$main">
<INPUT type="hidden" name="aff" value="$aff">
<INPUT type="hidden" name="aff_r" value="$aff">
<INPUT type="hidden" name="mode" value="check">
メールタイトル、メッセージを入力し確認ボタンを押して下さい。<br>
<TABLE border="0" bgcolor="#999999">
  <TBODY>
    <TR>
      <TD bgcolor="#cccccc" align="center"><B>メールタイトル</B></TD>
      <TD bgcolor="#ffffff"><INPUT size="40" type="text" name="subject" value="$subject"></TD>
    </TR>
    <TR>
      <TD bgcolor="#cccccc" colspan="2"><B>メッセージ</B></TD>
    </TR>
    <TR>
      <TD colspan="2" bgcolor="#ffffff"><TEXTAREA rows="20" cols="60" name="msr">$msr</TEXTAREA></TD>
    </TR>
    <TR>
      <TD colspan="2" bgcolor="#cccccc" align="center"><INPUT type="submit" value="確認">　<INPUT type="reset"></TD>
    </TR>
  </TBODY>
</TABLE>
</FORM>
      </td>
      <td valign="top"><br>
      <table border="0" bgcolor="#666666" cellspacing="1">
        <tbody>
          <tr bgcolor="#ffffff">
            <td>キー</td>
            <td></td>
          </tr>
          <tr bgcolor="#ffffff">
            <td>[--NAME--]</td>
            <td>お客様名</td>
          </tr>
          <tr bgcolor="#ffffff">
            <td>[--SEND--]</td>
            <td>送信日時</td>
          </tr>
          <tr bgcolor="#ffffff">
            <td>[--NO--]</td>
            <td>記事番号</td>
          </tr>
          <tr bgcolor="#ffffff">
            <td colspan="2">上記のキーを、<br>
            メールタイトル又はメッセージに<br>
            入力すると送信時変換され送信されます。</td>
          </tr>
        </tbody>
      </table>
      </td>
    </tr>
  </tbody>
</table>

WAKABA;

}



//	確認処理
function check($mode) {
global $PHP_SELF,$conn_id;
include './array.inc';
include '../sub/array.inc';
include '../sub/setup.inc';

	unset($_SESSION['msg_info']);

	$main = $_POST['main'];
	$main_r = $_POST['main_r'];
	$aff = $_POST['aff'];
	$aff_r = $_POST['aff_r'];

	$subject = $_POST['subject'];
#	$subject = mb_convert_kana($subject,"asKV","EUC-JP");
	$subject = mb_convert_kana($subject,"asKV","UTF-8");
	$subject = trim($subject);
	$msr = $_POST['msr'];
#	$msr = mb_convert_kana($msr,"asKV","EUC-JP");
	$msr = mb_convert_kana($msr,"asKV","UTF-8");
	$msr = trim($msr);

	$_SESSION['msg_info']['subject'] = $subject;
	$_SESSION['msg_info']['msr'] = $msr;

	if (!$subject) { $ERROR[] = "メールタイトルが入力されておりません。"; }
	if (!$msr) { $ERROR[] = "メッセージが入力されておりません。"; }

	if ($ERROR) {
		ERROR($ERROR);
		first($mode);
	}
	else {

		$msr = nl2br($msr);

		echo <<<WAKABA
<FORM action="$PHP_SELF" method="POST">
<INPUT type="hidden" name="main" value="$main">
<INPUT type="hidden" name="main_r" value="$main">
<INPUT type="hidden" name="aff" value="$aff">
<INPUT type="hidden" name="aff_r" value="$aff">
<INPUT type="hidden" name="mode" value="send">
以下の内容をそうしてよろしければ送信ボタンを押して下さい。<br>
<TABLE border="0" bgcolor="#999999">
  <TBODY>
    <TR>
      <TD bgcolor="#cccccc" align="center"><B>メールタイトル</B></TD>
      <TD bgcolor="#ffffff">$subject</TD>
    </TR>
    <TR>
      <TD bgcolor="#cccccc" colspan="2"><B>メッセージ</B></TD>
    </TR>
    <TR>
      <TD colspan="2" bgcolor="#ffffff">$msr</TD>
    </TR>
    <TR>
      <TD colspan="2" bgcolor="#cccccc" align="center"><INPUT type="submit" value="送信"></TD>
    </TR>
  </TBODY>
</TABLE>
</FORM>
<FORM action="$PHP_SELF" method="POST">
<INPUT type="hidden" name="main" value="$main">
<INPUT type="hidden" name="main_r" value="$main">
<INPUT type="hidden" name="aff" value="$aff">
<INPUT type="hidden" name="aff_r" value="$aff">
<INPUT type="hidden" name="mode" value="back">
<input type="submit" value="修正する">
</form>

WAKABA;
	}

}



//	送信処理
function send($mode) {
global $PHP_SELF,$conn_id;
include './array.inc';
include '../sub/array.inc';
include '../sub/setup.inc';
include '../sub/mail.inc';

	if (!$_SESSION['msg_info']) { $ERROR[] = "送信情報が確認出来ません。"; }
	else {
		$subject = $_SESSION['msg_info']['subject'];
		$msr = $_SESSION['msg_info']['msr'];
	}

	//	データー記録
	if (!$ERROR) {
		$sql  = "INSERT INTO $af_mail_table" .
				" (subject,msr,s_date)" .
				" VALUES('$subject','$msr',now());";
		if (!$result = mysqli_query($conn_id,$sql)) { $ERROR[] = "送信情報を登録出来ませんでした。"; }
		else {
			$sql  = "SELECT max(af_mail_num) AS max FROM $af_mail_table;";
			if ($result = mysqli_query($conn_id,$sql)) {
				$list = mysqli_fetch_array($result);
				$max = $list['max'];
			}
		}
	}

	//	メール送信
	$count = 0;
	if (!$ERROR) {
		//	日時
		$date = date("Y/m/d H:i:s");
##		$msr = ereg_replace("\[--SEND--\]",$date,$msr);
        $msr = preg_replace("/\[--SEND--\]/",$date,$msr);
		//	記事番号
##		$msr = ereg_replace("\[--NO--\]",$max,$msr);
        $msr = preg_replace("/\[--NO--\]/",$max,$msr);

		$msr .= "\n\n" . $m_footer;

		$sql  = "SELECT a.name_s, a.name_n, a.email FROM $member_table a, $afuser_table afu" .
				" WHERE a.kojin_num=afu.kojin_num AND afu.state='0' ORDER BY afu.af_num;";
		if ($result = mysqli_query($conn_id,$sql)) {
			WHILE ($list = mysqli_fetch_array($result)) {
				$msr_ = $msr;
				$name = $list['name_s']  . " " . $list['name_n'];
				$email = $list['email'];
##				$msr_ = ereg_replace("\[--NAME--\]",$name,$msr_);
			    $msr_ = preg_replace("/\[--NAME--\]/",$name,$msr_);

				$send_email = $admin_mail_a;
				$send_name = $admin_name;
				$get_email = $email;
				if ($email) {
					send_email($send_email,$send_name,$mail_bcc,$bcc_email,$get_email,$subject,$msr_);
				}
				$count++;
			}
		}
	}

	//	送信数記録
	if (!$ERROR) {
		$sql  = "UPDATE $af_mail_table SET" .
				" s_num='$count'" .
				" WHERE af_mail_num='$max';";
		if (!$result = mysqli_query($conn_id,$sql)) { $ERROR[] = "配信数を登録出来ませんでした。"; }
	}

	if ($ERROR) {
		ERROR($ERROR);
		first($mode);
	}
	else {
		echo <<<WAKABA
送信完了致しました。<br>
送信数：{$count}件<br>

WAKABA;
	}

}
?>
