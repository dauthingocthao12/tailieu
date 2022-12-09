<?PHP
//	メール配信サブルーチン
function send_email($send_email,$send_name,$mail_bcc,$bcc_email,$get_email,$subject,$msg) {

	mb_language("Japanese");
#	mb_internal_encoding("EUC-JP");
	mb_internal_encoding("UTF-8");
	$send_name = header_base64_encode($send_name);
	$from = "From: $send_name <$send_email>\nReply-To: $send_email\n";
	if ($mail_bcc == 1) {
		$from .= "Bcc: $bcc_email\n";
	}

	//	文字化け対策
	$subject = "　" . $subject;

	mb_send_mail ( $get_email, $subject, $msg , $from , "-f$send_email");

}



function header_base64_encode($str) {

#	$result = iconv("EUC-JP", "ISO-2022-JP", $str).chr(27).'(B';
	$result = iconv("UTF-8", "ISO-2022-JP", $str).chr(27).'(B';
	$result = '=?ISO-2022-JP?B?'.base64_encode($result).'?=';

	return $result;

}
?>
