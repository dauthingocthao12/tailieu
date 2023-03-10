<?PHP
//	入荷情報
function new_goods($main,$sub) {

	$mode = $_POST['mode'];
	$mode = stripslashes($mode);

	if ($mode == "regist") { list($msg,$ERROR) = regist(); }

	$html = first($main,$sub,$msg,$ERROR);

	return $html;

}



//	初期画面
function first($main,$sub,$msg,$ERROR) {
global $PHP_SELF,$LOGDATA_DIR,$news_file;

	$file = "../$LOGDATA_DIR/$news_file";
	if ($ERROR) {
		$new_goods = $_POST['new_goods'];
		$errors = ERROR($ERROR);
	}
	elseif (file_exists($file)) {
		$LIST = file($file);
		unset($LIST[0]);
		if ($LIST) {
			foreach ($LIST AS $VAL) {
				$new_goods .= $VAL;
			}
		}
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
商品番号を、１行１商品を入れ良ければ登録ボタンを押してください。
<table border="0" bgcolor="#666666" cellspacing="1">
  <tbody>
    <tr bgcolor="#ffffff">
      <td valign="top" bgcolor="#cccccc">商品番号</td>
      <td><textarea rows="10" cols="20" name="new_goods">$new_goods</textarea></td>
    </tr>
    <tr bgcolor="#ffffff">
      <td colspan="2" align="center"><input type="submit" value="登録">　<input type="reset"></td>
    </tr>
  </tbody>
</table>
</FORM>

WAKABA;

	return $html;

}



//	登録処理
function regist() {
global $db,$LOGDATA_DIR,$news_file;

	$file = "../$LOGDATA_DIR/$news_file";

	$new_goods = $_POST['new_goods'];
	if ($new_goods) {
		$LIST = explode("\n",$new_goods);
		if ($LIST) {
			foreach ($LIST AS $VAL) {
				$VAL = trim($VAL);
				if (!$VAL) { continue; }
				unset($count);
##				$list_num = ereg_replace("[^0-9]","",$VAL);
				$list_num = preg_replace("/[^0-9]/","",$VAL);
				$sql  = "SELECT count(*) AS count FROM list a, goods b" .
						" WHERE a.pluid=b.pluid AND a.list_num='$list_num' AND a.display='2';";
				if ($result = pg_query($db,$sql)) {
					$list = pg_fetch_array($result);
					$count = $list['count'];
				}
				if ($count < 1) { $ERROR[] = "入力された商品番号<B>{$VAL}</B>は、登録されてないか表示可能になっておりません。"; }
				$n_new_goods .= "$VAL\n";
			}
		}
	}

	if (!$ERROR) {
		if ($n_new_goods) {
			$date = date("Y/m/d");
			$n_new_goods = "$date\n$n_new_goods";
			$n_new_goods = trim($n_new_goods);

			$OUT = fopen($file,"w");
			fwrite($OUT, $n_new_goods);
			fclose($OUT);
			@chmod($file,0666);
		}
		elseif (file_exists($file)) {
			unlink($file);
		}

		$msg = "<BR>\n<B>入荷情報を更新しました。</B><BR>\n";
	}

	return array($msg,$ERROR);

}
?>
