<?PHP
//YahooSHOPお勧め商品 全角→半角プログラム

	$PHP_SELF = $_SERVER['PHP_SELF'];
	$mode = $_POST['mode'];
	$msg = $_POST['msg'];
	if ($mode == "hen") { $main = hen($msg); }
	else { $main = first(); }

	$html = head();
	$html .= $main;
	$html .= footer();

	echo ($html);

	exit;



function first() {
global $PHP_SELF;

	$html = <<<WAKABA
<FORM action="$PHP_SELF" method="POST">
<INPUT type="hidden" name="mode" value="hen">
お勧め商品のＩＤを１行１ＩＤで入力して変換を押して下さい。<BR>
<TEXTAREA rows="15" cols="60" name="msg"></TEXTAREA><BR>
<INPUT type="submit" value="変換">　<INPUT type="reset">
</FORM>

WAKABA;

	return $html;

}



function hen($msg) {
global $PHP_SELF;

	$msg = strtolower($msg);
##	$msg = eregi_replace("\r","",$msg);
	$msg = preg_replace("/\r/i","",$msg);
	$LIST = explode("\n",$msg);
	$n_msg = "";
	if ($LIST) {
		foreach ($LIST AS $VAL) {
			$VAL = trim($VAL);
			if ($VAL != "") {
				$n_msg .= " $VAL";
			}
		}
		$n_msg = trim($n_msg);
	}

	$html = <<<WAKABA
<FORM>
以下をコピーしてお勧めリストに貼り付けてください。<BR>
<TEXTAREA rows="15" cols="60" readonly>$n_msg</TEXTAREA><BR>
<INPUT type="button" value="戻る" onclick="history.back();">
</FORM>

WAKABA;

	return $html;

}



function head() {

	$html = <<<WAKABA
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<HTML>
<HEAD>
<META http-equiv="Content-Type" content="text/html; charset=UTF-8">
<META http-equiv="Content-Style-Type" content="text/css">
<TITLE>YahooSHOPお勧め商品 全角→半角プログラム</TITLE>
<STYLE type="text/css">
<!--
BODY,TABLE{
  font-size : 12px;
}
-->
</STYLE>
</HEAD>
<BODY>
<B>YahooSHOPお勧め商品 全角→半角プログラム</B><BR>
<BR>

WAKABA;

	return $html;

}



function footer() {

	$html = <<<WAKABA
</BODY>
</HTML>
WAKABA;

	return $html;

}
?>
