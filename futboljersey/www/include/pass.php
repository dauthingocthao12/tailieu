<?php
//	メール配信サブルーチン
function send_email($send_email,$send_name,$mail_bcc,$bcc_email,$get_email,$subject,$msg) {

	//	文字コード宣言
	mb_language("Japanese");
#	mb_internal_encoding("EUC-JP");
	mb_internal_encoding("UTF-8");
	$send_name = header_base64_encode($send_name);
	$from = "From: ".$send_name." <".$send_email.">\nReply-To: ".$send_email."\n";
	if ($mail_bcc == 1) {
		$from .= "Bcc: ".$bcc_email."\n";
	}

	//	文字化け対策
	$subject = "　" . $subject;

	mb_send_mail ( $get_email, $subject, $msg , $from , "-f$send_email");

}
function header_base64_encode($str) {

#	$result = iconv("EUC-JP", "ISO-2022-JP", $str).chr(27).'(B';	//iconv 文字列を指定した文字エンコーディングに変換する
	$result = iconv("UTF-8", "ISO-2022-JP", $str).chr(27).'(B';		//iconv 文字列を指定した文字エンコーディングに変換する
	$result = '=?ISO-2022-JP?B?'.base64_encode($result).'?=';

	return $result;

}



//	メール入力画面
function pass_forget($ERROR){

	$html = "";

	$email = $_SESSION['member']['email'];

	if ($ERROR) {
		$error_html = error_html($ERROR);
	}

	$INPUTS['ERRORMSG'] = $error_html;		//	エラーメッセージ
	$INPUTS['EMAIL'] = $email;				//	メールエラーメッセージ

	//	html作成・置換
	$make_html = new read_html();
	$make_html->set_dir(TEMPLATE_DIR);
	$make_html->set_file("pass.htm");
	$make_html->set_rep_cmd($INPUTS);
	$make_html->set_del_cmd($DEL_INPUTS);
	$html = $make_html->replace();

	return $html;

}



//	メールアドレスエラーチェック
function email_check(&$ERROR){
	global $conn_id; // mysql DB

	$_SESSION['member'] = array();

	//	メールアドレス取得
	$email = $_POST['email'];
#	$email = mb_convert_kana($email, "as", "EUC-JP");
	$email = mb_convert_kana($email, "as", "UTF-8");
	$email = trim($email);

	$_SESSION['psaa_send']['email'] = $email;

	//	メールエラーチェック
	if ($email == "") {
		$ERROR[] = "メールアドレスが入力されておりません。";
	}
	if ($email && !preg_match("/^[a-zA-Z0-9_\.\-]+@(([a-zA-Z0-9_\-]+\.)+[a-zA-Z0-9]+$)/",$email,$regs)) {
		$ERROR[] = "メールアドレスが不正です。";
	}

	if (!$ERROR) {
		$sql  = "SELECT kojin_num, name_s, name_n, email, pass FROM ".T_KOJIN.
				" WHERE email='".$email."'".
				" AND saku='0'".
				" AND kojin_num<='100000';";
		$result = mysqli_query($conn_id,$sql);
		$count = mysqli_num_rows($result);
		if ($count < 1) {
			$ERROR[] = "メールアドレスが間違っているか登録されておりません。";
		}

		$list = mysqli_fetch_array($result);
		$kojin_num = $list['kojin_num'];
		$name_s = $list['name_s'];
		$name_n = $list['name_n'];
		$email = $list['email'];
		$pass = $list['pass'];

		$_SESSION['pass_send']['kojin_num'] = $kojin_num;
		$_SESSION['pass_send']['name_s'] = $name_s;
		$_SESSION['pass_send']['name_n'] = $name_n;
		$_SESSION['pass_send']['email'] = $email;
		$_SESSION['pass_send']['pass'] = $pass;

	}

}



//	メール送信
function pass_kakunin(){

	global $admin_name , $admin_mail_m , $m_footer;

	$kojin_num = $_SESSION['pass_send']['kojin_num'];
	$name_s = $_SESSION['pass_send']['name_s'];
	$name_n = $_SESSION['pass_send']['name_n'];
	$email = $_SESSION['pass_send']['email'];
	$pass = $_SESSION['pass_send']['pass'];

// お客様送信用
	$subject = "パスワードのご連絡 - ネイバーズスポーツ -";

	$msg = <<<EOT
{$subject}
{$name_s} {$name_n} 様 パスワードをご連絡させていただきます。
------------------------------------------------------

登録メールアドレス
　{$email}

登録パスワード
　{$pass}

{$m_footer}
EOT;

	$send_email = $admin_mail_m;
	$send_name = $admin_name;
	$get_email = $email;
	send_email($send_email,$send_name,$mail_bcc,$bcc_email,$get_email,$subject,$msg);



// 受け取り用
	$men = " ( No.".$kojin_num." )";
	$subject = "パスワード確認されました。".$men." - ネイバーズスポーツ -";

	$ip = getenv("REMOTE_ADDR");
	$host = gethostbyaddr($ip);
	if (!$host) {
		$host = $ip;
	}

	$msg = <<<EOT
{$name_s} {$name_n} 様が {$subject}
------------------------------------------------------

登録メールアドレス
　{$email}

登録パスワード
　{$pass}

------------------------------------------------------
{$host} ({$ip})
EOT;


	$send_email = $email;
	$send_name = $name_s . $name_n;
	$get_email = $admin_mail_m;
	send_email($send_email,$send_name,$mail_bcc,$bcc_email,$get_email,$subject,$msg);



	unset($_SESSION['pass_send']);

	header ("Location: ../template/thank_p.htm");
	exit();

}

?>
