<?PHP
//	メール配信サブルーチン
function send_email($send_email,$send_name,$mail_get,$get_email,$email,$subject,$msr) {

	mb_language("ja");
	## mb_internal_encoding("EUC-JP");
	mb_internal_encoding("UTF-8");
	$send_name = header_base64_encode($send_name);
	$from = "From: $send_name <$send_email>\nReply-To: $send_email\n";
	if ($mail_get == 1) {
		$from .= "Bcc: $get_email\n";
	}

	//	サーバー変更対策05/11/01
	$subject = "　" . $subject;

	if (!mb_send_mail ( $email, $subject, $msr , $from , "-f$send_email")) {
		$check = 1;
		return $check;
	}

}



function header_base64_encode($str) {

	## $result = iconv("EUC-JP", "ISO-2022-JP", $str).chr(27).'(B';
	$result = iconv("UTF-8", "ISO-2022-JP", $str).chr(27).'(B';
	$result = '=?ISO-2022-JP?B?'.base64_encode($result).'?=';

	return $result;

}
?>
