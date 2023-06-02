<?PHP
//	Ypath登録
function goods_7() {
global $DATA_DIR,$list_file,$del_file,$ERROR;
$mode=$_POST['mode'];
$ypath=$_POST['ypath'];

	$DATA_DIR = "./data";
	$list_file = "$DATA_DIR/ypath.dat";
	$del_file = "$DATA_DIR/ypath_d.dat";

	if ($mode == 'up') {
		if ($ypath) { up(); }
		else { $ERROR[] = "ファイルが選択されておりません。"; }
	}
	elseif ($mode == "非表示する") { del(); }
	elseif ($mode == "表示する") { cansel(); }
	elseif ($mode == "登録") { touroku(); }

	first00();

}



function first00() {
// global $PHP_SELF,$ERROR,$msg,$main,$goods,$list_file,$del_file;
global $PHP_SELF,$ERROR,$list_file,$del_file;
global $msg; // @source up()
$main=$_POST['main'];
$goods=$_POST['goods'];

	if ($ERROR) { ERROR($ERROR); }

	echo <<<WAKABA
$msg
YpathのCSVリストをダウンロードしアップして下さい。<BR>
<A href="http://g1.shop.yahoo.co.jp:8080/cgi-bin/tools/ypath_search" target="_blank">Ypathページ</A><BR>
<FORM action="$PHP_SELF" method="POST" enctype="multipart/form-data">
<INPUT type="hidden" name="main" value="$main">
<INPUT type="hidden" name="goods" value="$goods">
<INPUT type="hidden" name="mode" value="up">
<TABLE border="0" bgcolor="#666666" cellspacing="1">
  <TBODY>
    <TR bgcolor="#ffffff">
      <TD bgcolor="#cccccc">YpathのCSVファイル</TD>
      <TD><INPUT size="40" type="file" name="ypath"></TD>
      <TD><INPUT type="submit" value="登録"></TD>
    </TR>
  </TBODY>
</TABLE>
</FORM>

WAKABA;

	if (file_exists($list_file)) {
		$LIST = file($list_file);
		foreach ($LIST AS $val) {
			$val = rtrim($val);
			list($y_name_,$ypath_,$del_,$live_key_,$rakuten_key_,$amazon_key_) = explode("<>",$val);
			$H_LIST[$y_name_] = $ypath_;
			$L_LIST[$y_name_] = $live_key_;
			$R_LIST[$y_name_] = $rakuten_key_;
			$A_LIST[$y_name_] = $amazon_key_;
		}
	}
	if (file_exists($del_file)) {
		$DLIST = file($del_file);
		foreach ($DLIST AS $val) {
			list($y_name_) = explode("<>",$val);
			$HD_LIST[$y_name_] = $val;
		}
	}

	if ($H_LIST) {
		echo <<<WAKABA
商品登録時、Ypathリストに表示させたくない場合は非表示にして下さい。<BR>
<TABLE border="0" style="font-size : 12px;" bgcolor="#666666" cellspacing="1" cellpadding="4">
  <TBODY>
    <TR bgcolor="#cccccc">
      <TH>カテゴリー名</TH>
      <TH>新Yahoo用カテゴリーコード</TH>
      <TH>楽天デェレクトリーID</TH>
      <TH>amazonID</TH>
      <TH>リスト表示</TH>
    </TR>

WAKABA;

		foreach ($H_LIST AS $key => $val) {
			if ($HD_LIST[$key]) {
				$submit = "非表示中 <INPUT type=\"submit\" name=\"mode\" value=\"表示する\">";
				$bg_color = "#f6f6f6";
			}
			else {
				$submit = "表示中 <INPUT type=\"submit\" name=\"mode\" value=\"非表示する\">";
				$bg_color = "#ffffff";
			}
			$key = mb_convert_encoding($key,"UTF-8","EUC_JP");
			echo <<<WAKABA
    <TR bgcolor="$bg_color">
      <TD>$key</TD>
      <TD>
      <FORM action="$PHP_SELF" method="POST">
      <INPUT type="hidden" name="main" value="$main">
      <INPUT type="hidden" name="goods" value="$goods">
      <INPUT type="hidden" name="ypath_name" value="$key">
      <INPUT type="text" size="20" name="live_key" value="$L_LIST[$key]">
      <INPUT type="submit" name="mode" value="登録">
      </TD></FORM>
      <TD>
      <FORM action="$PHP_SELF" method="POST">
      <INPUT type="hidden" name="main" value="$main">
      <INPUT type="hidden" name="goods" value="$goods">
      <INPUT type="hidden" name="ypath_name" value="$key">
      <INPUT type="text" size="20" name="rakuten_key" value="$R_LIST[$key]">
      <INPUT type="submit" name="mode" value="登録">
      </TD></FORM>
      <TD>
      <FORM action="$PHP_SELF" method="POST">
      <INPUT type="hidden" name="main" value="$main">
      <INPUT type="hidden" name="goods" value="$goods">
      <INPUT type="hidden" name="ypath_name" value="$key">
      <INPUT type="text" size="20" name="amazon_key" value="$A_LIST[$key]">
      <INPUT type="submit" name="mode" value="登録">
      </TD></FORM>
      <TD align="center">
      <FORM action="$PHP_SELF" method="POST">
      <INPUT type="hidden" name="main" value="$main">
      <INPUT type="hidden" name="goods" value="$goods">
      <INPUT type="hidden" name="ypath_name" value="$key">
      $submit
      </TD></FORM>
    </TR>

WAKABA;
		}

		echo <<<WAKABA
  </TBODY>
</TABLE>

WAKABA;

	}
	else {
		echo <<<WAKABA
今現在、Ypathリストは登録されておりません。<BR>

WAKABA;
	}

}


function del() {
// global	$del_file,$ypath_name,$list_file;
global $list_file,$del_file;
$ypath_name=$_POST['ypath_name'];

	if ($ypath_name) {
		$OUT = fopen($del_file,"a");
		fwrite($OUT,"$ypath_name<>\n");
		fclose($OUT);
		@chmod($del_file,0666);

		if (file_exists($list_file)) {
			$LIST = file($list_file);
			$n_list = "";
			foreach ($LIST AS $val) {
				$val = trim($val);
				list($y_name_,$ypath_,$del_,$live_key_,$rakuten_key_,$amazon_key_) = explode("<>",$val);
				if ($ypath_name == $y_name_) {
					$n_list .= "$y_name_<>$ypath_<>1<>$live_key_<>$rakuten_key_<>$amazon_key_<>\n";
				}
				else { $n_list .= "$val\n"; }
			}
			$OUT = fopen($list_file,"w");
			fwrite ($OUT,$n_list);
			fclose ($OUT);
		}
	}

}



function cansel() {
// global	$del_file,$ypath_name,$list_file;
global $list_file,$del_file;
$ypath_name=$_POST['ypath_name'];

	if ($ypath_name) {
		if (file_exists($del_file)) {
			$DLIST = file($del_file);
			$n_dlist = "";
			foreach ($DLIST AS $val) {
				list($ypath_name_) = explode("<>",$val);
				if ($ypath_name != $ypath_name_) {
					$n_dlist .= $val;
				}
			}
			if ($n_dlist) {
				$OUT = fopen($del_file,"w");
				fwrite($OUT,$n_dlist);
				fclose($OUT);
			}
			else {
				unlink($del_file);
			}
		}

		if (file_exists($list_file)) {
			$LIST = file($list_file);
			$n_list = "";
			foreach ($LIST AS $val) {
				$val = trim($val);
				list($y_name_,$ypath_,$del_,$live_key_,$rakuten_key_,$amazon_key_) = explode("<>",$val);
				if ($ypath_name == $y_name_) {
					$n_list .= "$y_name_<>$ypath_<><>$live_key_<>$rakuten_key_<>$amazon_key_<>\n";
				}
				else { $n_list .= "$val\n"; }
			}
			$OUT = fopen($list_file,"w");
			fwrite ($OUT,$n_list);
			fclose ($OUT);
		}
	}

}



function touroku() {
// global	$ypath_name,$list_file,$live_key,$rakuten_key,$amazon_key;
global $list_file;
$ypath_name=$_POST['ypath_name'];
$live_key=$_POST['live_key'];
$rakuten_key=$_POST['rakuten_key'];
$amazon_key=$_POST['amazon_key'];


#	$live_key = mb_convert_kana($live_key,"as","EUC-JP");
	$live_key = mb_convert_kana($live_key,"as","UTF-8");
	$live_key = trim($live_key);
#	$rakuten_key = mb_convert_kana($rakuten_key,"as","EUC-JP");
	$rakuten_key = mb_convert_kana($rakuten_key,"as","UTF-8");
	$rakuten_key = trim($rakuten_key);
#	$amazon_key = mb_convert_kana($amazon_key,"as","EUC-JP");
	$amazon_key = mb_convert_kana($amazon_key,"as","UTF-8");
	$amazon_key = trim($amazon_key);

	if (file_exists($list_file)) {
		$LIST = file($list_file);
		$n_list = "";
		foreach ($LIST AS $val) {
			$val = rtrim($val);
			list($y_name_,$ypath_,$del_,$live_key_,$rakuten_key_,$amazon_key_) = explode("<>",$val);
			if ($ypath_name == $y_name_) {
				if ($live_key) {
					$rakuten_key = $rakuten_key_;
					$amazon_key = $amazon_key_;
				} elseif ($rakuten_key) {
					$live_key = $live_key_;
					$amazon_key = $amazon_key_;
				} elseif ($amazon_key) {
					$rakuten_key = $rakuten_key_;
					$live_key = $live_key_;
				} else {
					$rakuten_key = $rakuten_key_;
					$live_key = $live_key_;
					$amazon_key = $amazon_key_;
				}

				$n_list .= "$y_name_<>$ypath_<>$del_<>$live_key<>$rakuten_key<>$amazon_key<>\n";
			}
			else { $n_list .= "$val\n"; }
		}
		$OUT = fopen($list_file,"w");
		fwrite ($OUT,$n_list);
		fclose ($OUT);
	}

}



function up() {
// global $DATA_DIR,$list_file,$del_file,$ypath,$msg,$ERROR;
global $list_file,$ypath,$del_file,$DATA_DIR,$ERROR;
global $msg;
$ypath=$_POST['ypath'];

	$word = "サッカー";

	$temp_file = "$DATA_DIR/temp.dat";
	if (!copy($ypath,$temp_file)) {
		$ERROR[] = "ファイルが取り込みできませんでした。";
	}
	if (!$ERROR) {
		@chmod($temp_file,0666);
		if (file_exists($del_file)) {
			$DLIST = file($del_file);
			foreach ($DLIST AS $val) {
				list($y_name_) = explode("<>",$val);
				$HD_LIST[$y_name_] = $val;
			}
		}
		$LIST = file($temp_file);
		foreach ($LIST AS $val) {
#			$val = mb_convert_encoding($val,"EUC-JP","SJIS");
			$val = mb_convert_encoding($val,"UTF-8","SJIS");
			$val = trim($val);
			list($ypath_,$y_num_,$y_name_) = explode(",",$val);
##			$ypath_ = ereg_replace("\"","",$ypath_);
			$ypath_ = preg_replace("/\"/","",$ypath_);
##			$y_name_ = ereg_replace("\"","",$y_name_);
			$y_name_ = preg_replace("/\"/","",$y_name_);
##			if (ereg($word,$y_name_)) {
			if (preg_match($word,$y_name_)) {
				if ($HD_LIST[$y_name_]) { $del = 1; }
				else { $del = ""; }
				$N_LIST[] = "$y_name_<>$ypath_<>$del<><><>\n";
			}
		}
		$n_list = "";
		if ($N_LIST) {
			foreach ($N_LIST AS $val) {
				$n_list .= $val;
			}
			$OUT = fopen($list_file,"w");
			fwrite ($OUT,$n_list);
			fclose ($OUT);
			@chmod($list_file,0666);
			$msg = "Ｙpathデーターを更新しました。<BR>\n<BR>\n";
		}
		elseif (file_exists($list_file)) {
			unlink($list_file);
		}

	}

	if (file_exists($temp_file)) {
		unlink ($temp_file);
	}

}
?>
