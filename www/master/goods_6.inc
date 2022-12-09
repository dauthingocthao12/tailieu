<?PHP
//	ブランド名登録
function goods_6() {
// global	$mode,$b_file;
global $b_file;
$mode=$_POST['mode'];

	$b_file = "../data/brand.dat";

	if ($mode == "news") { news(); }
	elseif ($mode == "変更") { renew(); }
	elseif ($mode == "削除") { delete(); }

	first00();

}



function first00() {
// global	$PHP_SELF,$ERROR,$main,$goods,$ERROR,$msg,$b_file;
global	$PHP_SELF,$ERROR,$b_file;
$main=$_POST['main'];
$goods=$_POST['goods'];
global $msg; // @source news(), renew(), delete()

	if ($ERROR) { ERROR($ERROR); }

	echo <<<WAKABA
$msg
<br>
<b>※ブランド名表記は、「英表記(日本語表記)」で記載して下さい。</b><br>
<br>
<FORM action="$PHP_SELF" method="POST">
<INPUT type="hidden" name="main" value="$main">
<INPUT type="hidden" name="goods" value="$goods">
<INPUT type="hidden" name="mode" value="news">
新規に登録するブランド名を入力して下さい。<BR>
<TABLE border="0" bgcolor="#666666" cellspacing="1">
  <TBODY>
    <TR bgcolor="#ffffff">
      <TD bgcolor="#cccccc">ブランド名</TD>
      <TD><INPUT size="40" type="text" name="b_name"></TD>
      <TD bgcolor="#cccccc">Yahooブランド番号</TD>
      <TD><INPUT size="10" type="text" name="b_key"></TD>
      <TD><INPUT type="submit" value="登録"></TD>
    </TR>
  </TBODY>
</TABLE>
</FORM>

WAKABA;

	$html = "";
	if (file_exists($b_file)) {
		$LIST = file($b_file);
		foreach ($LIST AS $val) {
			list($b_num_,$b_name_,$del_,$b_key_) = explode("<>",$val);

			if ($del_ == 1) { continue;}

			$html .= <<<WAKABA
    <TR bgcolor="#ffffff">
      <FORM action="$PHP_SELF" method="POST">
      <INPUT type="hidden" name="main" value="$main">
      <INPUT type="hidden" name="goods" value="$goods">
      <INPUT type="hidden" name="b_num" value="$b_num_">
      <TD>
      <INPUT size="40" type="text" name="b_name" value="$b_name_"> / <INPUT size="10" type="text" name="b_key" value="$b_key_">
      <INPUT type="submit" name="mode" value="変更">
      </TD>
      <TD align="center"><INPUT type="submit" name="mode" value="削除"></TD>
      </FORM>
    </TR>

WAKABA;
		}

		if ($html) {
			echo <<<WAKABA
ブランド名を変更する場合はブランド名を変更して変更ボタンを、<BR>
削除する場合は、削除するブランド名の削除ボタンを押して下さい。<BR>
<TABLE border="0" style="font-size : 12px;" bgcolor="#666666" cellspacing="1" cellpadding="4">
  <TBODY>
    <TR bgcolor="#cccccc">
      <TH>ブランド名 / Yahooブランド番号</TH>
      <TH>削除</TH>
    </TR>
$html
  </TBODY>
</TABLE>

WAKABA;
		}

	}

	if (!$html) {
		echo <<<WAKABA
今現在ブランドは登録されておりません。<BR>
<BR>

WAKABA;
	}

}



function news() {
// global	$b_file,$b_name,$b_key,$ERROR,$msg;
global $ERROR,$b_file;
$b_name=$_POST['b_name'];
$b_key=$_POST['b_key'];
global $msg;

	if (!$b_name) { $ERROR[] = "新規に登録するブランド名が入力されておりません。"; }

	$flag = 0;
	$n_b_num = 0;
	$n_list = "";
	if (!$ERROR && file_exists($b_file)) {

#		$b_name = mb_convert_kana($b_name, "asKV", "eucjp-win");	//	add , "eucjp-win"	ookawara 2015/08/10
		$b_name = mb_convert_kana($b_name, "asKV", "UTF-8");
#		$b_key = mb_convert_kana($b_key, "n", "eucjp-win");			//	add , "eucjp-win"	ookawara 2015/08/10
		$b_key = mb_convert_kana($b_key, "n", "UTF-8");
		$b_key = trim($b_key);

		$LIST = file($b_file);
		foreach ($LIST AS $val) {
			list($b_num_,$b_name_,$del_,$b_key_) = explode("<>",$val);
			if ($n_b_num < $b_num_) { $n_b_num = $b_num_; }
			if ($b_name == $b_name_ && $del_ != 1) {
				$ERROR[] = "入力されたブランド名は既に登録されております。";
				break;
			}
			else {
				$N_LIST[$b_name_] = $val;
			}
		}

	}

##	if (ereg("[^0-9]",$b_key)) { $ERROR[] = "Yahooブランド番号が数字以外の文字が入力されております。"; }
	if (preg_match("/[^0-9]/",$b_key)) { $ERROR[] = "Yahooブランド番号が数字以外の文字が入力されております。"; }

	if ($flag != 1) {
		$n_b_num++;
		$N_LIST[$b_name] = "$n_b_num<>$b_name<><>$b_key<>\n";
	}

	if ($N_LIST) {
		ksort($N_LIST,SORT_STRING);
		foreach ($N_LIST AS $val) {
			$n_list .= $val;
		}
	}

	if (!$ERROR && $n_list) {
		$OUT = fopen($b_file,"w");
		fwrite ($OUT,$n_list);
		fclose($OUT);
		@chmod($b_file,0666);

		$msg = "$b_name を追加しました。";

	}
}



function renew() {
// global	$b_file,$b_name,$b_num,$b_key,$ERROR,$msg;
global $ERROR,$b_file;
$b_name=$_POST['b_name'];
$b_num=$_POST['b_num'];
$b_key=$_POST['b_key'];
global $msg;

	if (!$b_name) { $ERROR[] = "変更するブランド名が入力されておりません。"; }

	$n_list = "";
	if (!$ERROR && file_exists($b_file)) {
#		$b_name = mb_convert_kana($b_name, "asKV", "eucjp-win");	//	add , "eucjp-win"	ookawara 2015/08/10
		$b_name = mb_convert_kana($b_name, "asKV", "UTF-8");
#		$b_key = mb_convert_kana($b_key, "n", "eucjp-win");			//	add , "eucjp-win"	ookawara 2015/08/10
		$b_key = mb_convert_kana($b_key, "n", "UTF-8");
		$b_key = trim($b_key);

		$LIST = file($b_file);
		foreach ($LIST AS $val) {
			list($b_num_,$b_name_,$del_,$b_key_) = explode("<>",$val);
			if ($b_name == $b_name_ && $b_num != $b_num_ && $del_ != 1) {
				$ERROR[] = "入力されたブランド名は既に登録されております。";
				break;
			}
			elseif ($b_num == $b_num_) {
				$N_LIST[$b_name] = "$b_num_<>$b_name<><>$b_key<>\n";
				$o_b_name = $b_name_;
			}
			else {
				$N_LIST[$b_name_] = $val;
			}
		}
	}

##	if (ereg("[^0-9]",$b_key)) { $ERROR[] = "Yahooブランド番号が数字以外の文字が入力されております。"; }
	if (preg_match("/[^0-9]/",$b_key)) { $ERROR[] = "Yahooブランド番号が数字以外の文字が入力されております。"; }

	if ($N_LIST) {
		ksort($N_LIST,SORT_STRING);
		foreach ($N_LIST AS $val) {
			$n_list .= $val;
		}
	}

	if (!$ERROR && $n_list) {
		$OUT = fopen($b_file,"w");
		fwrite ($OUT,$n_list);
		fclose($OUT);
		@chmod($b_file,0666);

		$msg = "$o_b_name から $b_name に変更しました。";

	}
}



function delete() {
// global	$b_file,$b_num,$b_key,$ERROR,$msg;
global $ERROR,$b_file;
$b_num=$_POST['b_num'];
global $msg;

	$n_list = "";
	if (file_exists($b_file)) {
		$LIST = file($b_file);
		foreach ($LIST AS $val) {
			list($b_num_,$b_name_,$del_,$b_key_) = explode("<>",$val);
			if ($b_num != $b_num_) {
				$n_list .= $val;
			}
			else {
				$n_list .= "$b_num_<>$b_name_<>1<>$b_key_<>\n";
				$b_name = $b_name_;
			}
		}
	}

	if (!$ERROR && $n_list) {
		$OUT = fopen($b_file,"w");
		fwrite ($OUT,$n_list);
		fclose($OUT);
		@chmod($b_file,0666);

		$msg = "$b_name を削除しました。";

	}
}
?>
