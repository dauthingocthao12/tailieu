<?PHP
//	お知らせ
function oshirase_goods($main,$sub) {

	$mode = $_POST['mode'];
	$mode = stripslashes($mode);

	if ($mode == "regist") { list($msg,$ERROR) = regist(); }

	$html = first($main,$sub,$msg,$ERROR);

	return $html;

}



//	初期画面
function first($main,$sub,$msg,$ERROR) {
global $PHP_SELF,$LOGDATA_DIR,$oshirase_file;

	$file = "../$LOGDATA_DIR/$oshirase_file";
	if ($ERROR) {
		$oshirase_goods = $_POST['oshirase_goods'];
		$errors = ERROR($ERROR);
	}
	elseif (file_exists($file)) {
		$oshirase_goods = file_get_contents($file);
	}

	$html = <<<WAKABA
$msg
$errors
<BR>
<FORM action="$PHP_SELF" method="POST">
<INPUT type="hidden" name="main" value="$main">
<INPUT type="hidden" name="b_main" value="$main">
<INPUT type="hidden" name="sub" value="$sub">
<INPUT type="hidden" name="mode" value="regist">
お知らせを入力し登録ボタンを押してください。
<table border="0" bgcolor="#666666" cellspacing="1">
  <tbody>
    <tr bgcolor="#cccccc">
      <td align="center">お知らせ</td>
    </tr>
    <tr bgcolor="#ffffff">
      <td><textarea rows="30" cols="50" name="oshirase_goods">$oshirase_goods</textarea></td>
    </tr>
    <tr bgcolor="#ffffff">
      <td align="center"><input type="submit" value="登録">　<input type="reset"></td>
    </tr>
  </tbody>
</table>
</FORM>

WAKABA;

	return $html;

}



//	登録処理
function regist() {
global $LOGDATA_DIR,$oshirase_file;

	$file = "../$LOGDATA_DIR/$oshirase_file";

	$oshirase_goods = trim($_POST['oshirase_goods']);

	if ($oshirase_goods) {
		$OUT = fopen($file,"w");
		fwrite($OUT, $oshirase_goods);
		fclose($OUT);
		@chmod($file,0666);
	}
	elseif (file_exists($file)) {
		unlink($file);
	}

	$msg = "<BR>\n<B>お知らせを更新しました。</B><BR>\n";

	return array($msg,$ERROR);

}
?>
