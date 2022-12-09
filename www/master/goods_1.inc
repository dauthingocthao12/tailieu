<?PHP
//	カテゴリー登録
function goods_1() {
// global	$PHP_SELF,$CATE_DIR,$main,$goods,$mc,$sc,$mode,$name,$num,$url,$conn_id;
global $CATE_DIR;
$mc=$_POST['mc'];
$sc=$_POST['sc'];
$mode=$_POST['mode'];
$num=$_POST['num'];
$url=$_POST['url'];
$name=$_POST['name'];
	$CATE_DIR = "../category";
	include ("../../cone.inc");

	if ($mode == "追加") { news($mc,$sc,$name,$url); }
	elseif ($mode == "変更") { renew($mc,$sc,$name,$num,$url); }
	elseif ($mode == "削除") { del($mc,$sc,$num); }
	elseif ($mode == "↑") { up($mc,$sc,$num); }
	elseif ($mode == "↓") { down($mc,$sc,$num); }

	first_00();

}



function first_00() {
// global	$PHP_SELF,$CATE_DIR,$main,$goods,$mc,$mc_r,$sc,$ERROR,$msg,$conn_id;
global	$PHP_SELF,$CATE_DIR,$ERROR,$conn_id;
global $msg; // @source news(),renew(),del(),up(),down()
$main=$_POST['main'];
$goods=$_POST['goods'];
$mc=$_POST['mc'];
$sc=$_POST['sc'];
$mc_r=$_POST['mc_r'];
$name=$_POST['name'];

	$file = "$CATE_DIR/category.inc";
	$LIST = file($file);
	foreach ($LIST AS $val) {
		list($h_num_,$num_,$mc_name_) = explode("<>",$val);
		$CATEGORY[$h_num_] = "$num_<>$mc_name_<>";
	}
	ksort($CATEGORY);

	if (!$mc) { $selected = "selected"; } else { $selected = ""; }
	$mc_l = "        <OPTION value=\"\" $selected>選択して下さい。</OPTION>\n";
	foreach ($CATEGORY AS $val) {
		$list = explode("<>",$val);
		if ($mc == $list[0]) { $selected = "selected"; $mc_name = $list[1]; } else { $selected = ""; }
		$mc_l .= "        <OPTION value=\"$list[0]\" $selected>$list[1]</OPTION>\n";
	}

	if ($mc != $mc_r) { $sc = ""; }

	$file = "$CATE_DIR/$mc.dat";
	if ($mc && file_exists($file)) {
		if (!$sc) { $selected = "selected"; } else { $selected = ""; }
		$sc_l = "        <OPTION value=\"\" $selected>-------------</OPTION>\n";
		$SUB_LIST = file($file);

		$SUB_CATEGORY = array();
		foreach ($SUB_LIST AS $val) {
			list($h_num_,$num_,$sc_name_) = explode("<>",$val);
			$SUB_CATEGORY[$h_num_] = "$num_<>$sc_name_<>";
		}
		krsort($SUB_CATEGORY);

		foreach ($SUB_CATEGORY AS $val) {
			$list = explode("<>",$val);
			if ($sc == $list[0]) { $selected = "selected"; $sc_name = $list[1]; } else { $selected = ""; }
			$sc_l .= "        <OPTION value=\"$list[0]\" $selected>$list[1]</OPTION>\n";
		}
	}

	$file = "$CATE_DIR/$mc" . "_$sc.dat";
	if ($mc && $sc && file_exists($file)) {
		$LAST_LIST = file($file);
	}

	if ($ERROR) {
		$error = "<FONT color=\"#ff0000\"><B>エラー</B></FONT><BR>\n";
		foreach ($ERROR AS $val) {
			$error .= "・$val<BR>\n";
		}
		$error .= "<BR>\n";
		unset($ERROR);
	}
	echo <<<WAKABA
$error
$msg
<FORM action="$PHP_SELF" method="POST">
<INPUT type="hidden" name="main" value="$main">
<INPUT type="hidden" name="goods" value="$goods">
<INPUT type="hidden" name="mc_r" value="$mc">
<INPUT type="hidden" name="sc" value="$sc">
登録するカテゴリーを選択して下さい。<BR>
<TABLE border="0">
  <TBODY>
    <TR>
      <TD><SELECT name="mc" onchange='submit();'>
$mc_l
      </SELECT></TD>

WAKABA;
	if ($sc_l) {
		echo <<<WAKABA
      <TD><SELECT name="sc" onchange='submit();'>
$sc_l
      </SELECT></TD>

WAKABA;
	}

	echo <<<WAKABA
      <TD><INPUT type="submit" value="選択"></TD>
    </TR>
  </TBODY>
</TABLE>
</FORM>

WAKABA;

	if ($mc_name && $sc_name) {
		$cate_name = "$mc_name - $sc_name";
	}
	elseif ($mc_name) {
		$cate_name = "$mc_name";
	}

	if ($mc) {

//		if ($sc) {
//			$f_name = " / ファイル名";
//			$url_msg = " / <INPUT size=\"20\" type=\"text\" name=\"url\" value=\"$url\">";
//
//			$sql = "SELECT c_id FROM r_cate WHERE cate1='$mc' AND cate2='$sc' AND cate3='0';";
//			if ($result = pg_exec($conn_id,$sql)) {
//				$list = pg_fetch_array($result);
//				$c_id = $list['c_id'];
//			}
//		}

//		if ($sc && !$c_id) {
//			echo ("上のカテゴリーを、楽天に登録してから作業をしてください。<BR>\n");
//		}
//		else {
			echo <<<WAKABA
<FORM action="$PHP_SELF" method="POST">
<INPUT type="Hidden" name="main" value="$main">
<INPUT type="Hidden" name="goods" value="$goods">
<INPUT type="Hidden" name="mc" value="$mc">
<INPUT type="Hidden" name="sc" value="$sc">
<INPUT type="Hidden" name="mc_r" value="$mc_r">
このカテゴリー($cate_name)に、新規追加する場合は、カテゴリー名を入力し追加を押して下さい。<BR>
<font color="#ff0000"><b>半角40文字、全角20文字以内</b></font><br>
<TABLE border="0" bgcolor="#666666" cellspacing="1" cellpadding="4">
  <TBODY>
    <TR bgcolor="#ffffff">
      <TD bgcolor="#cccccc">新規カテゴリー名$f_name</TD>
      <TD><INPUT size="40" type="text" name="name" value="$name">$url_msg<INPUT type="submit" name="mode" value="追加"></TD>
    </TR>
  </TBODY>
</TABLE>
</FORM>

WAKABA;
//		}
	}

	if ($mc && $sc) {
		if (!$LAST_LIST) {
			echo ("今現在、<B>$cate_name</B>には、何も登録されておりません。<BR>\n");
		}
		else {
			lists($mc,$sc,$LAST_LIST,$cate_name);
		}
	}
	elseif ($mc && !$sc) {
		if (!$SUB_LIST) {
			echo ("今現在、<B>$cate_name</B>には、何も登録されておりません。<BR>\n");
		}
		else {
			lists($mc,$sc,$SUB_LIST,$cate_name);
		}
	}

}



function lists($mc,$sc,$LIST,$cate_name) {
// global $PHP_SELF,$main,$goods,$mc,$mc_r,$sc;
global $PHP_SELF;
$main=$_POST['main'];
$goods=$_POST['goods'];
$mc_r=$_POST['mc_r'];
//	if ($mc && $sc) { $f_name = " / ファイル名"; }

	echo <<<WAKABA
<TABLE border="0" cellspacing="1" bgcolor="#666666">
  <TBODY>
    <TR bgcolor="#cccccc">
      <TD colspan="5">メインカテゴリー：$cate_name</TD>
    </TR>
    <TR bgcolor="#cccccc" align="center">
      <TD>&nbsp;</TD>
      <TD>↑</TD>
      <TD>↓</TD>
      <TD>カテゴリー名$f_name</TD>
      <TD>削除</TD>
    </TR>

WAKABA;

	$max = count($LIST);
	$set_i = $last = $max - 1;
	$NEW_LIST = array();
	foreach ($LIST AS $key => $val) {
		$NEW_LIST[$set_i] = $val;
		$set_i--;
	}

	for($i=0; $i<$max; $i++) {
		$nums = $i + 1;
		list($h_num_,$num_,$name_,$url_) = explode("<>",$NEW_LIST[$i]);
		$url_ = rtrim($url_);
		if ($i != 0) { $up = "<INPUT type=\"submit\" name=\"mode\" value=\"↑\">"; } else { $up = "&nbsp;"; }
		if ($i != $last) { $down = "<INPUT type=\"submit\" name=\"mode\" value=\"↓\">"; } else { $down = "&nbsp;"; }

		if ($mc && $sc) {
			$url_msg = "<INPUT type=\"hidden\" name=\"url\" value=\"$url_\">";
		}

		echo <<<WAKABA
    <TR bgcolor="#ffffff">
      <FORM action="$PHP_SELF" method="POST">
      <INPUT type="hidden" name="main" value="$main">
      <INPUT type="hidden" name="goods" value="$goods">
      <INPUT type="hidden" name="mc" value="$mc">
      <INPUT type="hidden" name="sc" value="$sc">
      <INPUT type="hidden" name="mc_r" value="$mc_r">
      <INPUT type="hidden" name="num" value="$num_">
      <TD>$nums</TD>
      <TD>$up</TD>
      <TD>$down</TD>
      <TD><INPUT size="40" type="text" name="name" value="$name_">$url_msg<INPUT type="submit" name="mode" value="変更"></TD>
      <TD><INPUT type="submit" name="mode" value="削除"></TD>
      </FORM>
    </TR>

WAKABA;

	}

	echo <<<WAKABA
  </TBODY>
</TABLE>

WAKABA;

}



function news($mc,$sc,$name,$url) {
// global $CATE_DIR,$msg,$ERROR,$conn_id;
global $CATE_DIR,$ERROR,$conn_id;
global $msg;

	if (!$name) { $ERROR[] = "新規に追加するカテゴリー名が入力されておりません。"; }
//	if ($mc && $sc && !$url) { $ERROR[] = "新規に追加するカテゴリーのファイル名が入力されておりません。"; }

	if ($mc && $sc) { $file = "$CATE_DIR/$mc". "_$sc.dat"; }
	elseif ($mc) { $file = "$CATE_DIR/$mc.dat"; }
	else { $ERROR[] = "カテゴリー番号が認識できません。"; }

	if (!$ERROR) {
		$name = mb_convert_kana($name,"asKV");
		$name = trim($name);
		$n_h_num = $n_num = 0;
		if (file_exists($file)) {
			$LIST = file($file);
			foreach ($LIST AS $val) {
				list($h_num_,$num_,$name_,$url_) = explode("<>",$val);
				$url_ = rtrim($url_);
				if ($n_h_num < $h_num_) { $n_h_num = $h_num_; }
				if ($n_num < $num_) { $n_num = $num_; }
				if ($name == $name_) { $ERROR[] = "既にそのカテゴリー名は登録されております。"; break; }
//				if ($url && $url == $url_) { $ERROR[] = "既にそのファイル名は登録されております。"; break; }
			}
		}
	}

	if (!$ERROR) {
		$n_h_num += 1;
		$n_num += 1;

		$n_line = "$n_h_num<>$n_num<>$name<>$url<>\n";

		$OUT = fopen($file,"a");
		fwrite($OUT,$n_line);
		fclose($OUT);
		@chmod($file,0666);
	}

	if (!$ERROR) {
		if ($mc && $sc) {
			$cate1 = $mc;
			$cate2 = $sc;
			$cate2_ = $cate2;
			$cate3 = $n_num;
		}
		elseif ($mc) {
			$cate1 = $mc;
			$cate2 = $n_num;
			$cate2_ = 0;
			$cate3 = 0;
		}

//		$sql = "SELECT c_id FROM r_cate WHERE cate1='$cate1' AND cate2='$cate2_' AND cate3='0';";
//		if ($result = pg_exec($conn_id,$sql)) {
//			$list = pg_fetch_array($result);
//			$c_id = $list['c_id'];
//		}

		$sql  = "INSERT INTO r_cate" .
				" (c_name,cate1,cate2,cate3)" .
				" VALUES('$name','$cate1','$cate2','$cate3');";
		if (!pg_exec($conn_id,$sql)) { $ERROR[] = "カテゴリーデーターを登録できませんでした。"; }
	}

	if (!$ERROR) {
		$msg = "$name を登録しました。<BR>\n<BR>\n";
	}

}


function renew($mc,$sc,$name,$num,$url) {
// global $CATE_DIR,$msg,$ERROR,$conn_id;
global $CATE_DIR,$ERROR,$conn_id;
global $msg;

	if (!$name) { $ERROR[] = "変更するカテゴリー名が入力されておりません。"; }
//	if ($mc && $sc && !$url) { $ERROR[] = "新規に追加するカテゴリーのファイル名が入力されておりません。"; }

	if ($mc && $sc) { $file = "$CATE_DIR/$mc". "_$sc.dat"; }
	elseif ($mc) { $file = "$CATE_DIR/$mc.dat"; }
	else { $ERROR[] = "カテゴリー番号が認識できません。"; }

	if (!$ERROR) {
		$name = mb_convert_kana($name,"asKV");
		$name = trim($name);
		$N_LIST = "";
		$flag = 0;
		if (file_exists($file)) {
			$LIST = file($file);
			foreach ($LIST AS $val) {
				list($h_num_,$num_,$name_,$url_) = explode("<>",$val);
				$url_ = rtrim($url_);
				if ($num == $num_) {
//					if ($name == $name_ && $url == $url_) { $flag = 1; break; }
					if ($name == $name_) { $flag = 1; break; }
					$n_line = "$h_num_<>$num_<>$name<>$url<>\n";
					$name_r = $name_;
					$N_LIST .= $n_line;
				}
				elseif ($num != $num_ && $name == $name_) {
					$ERROR[] = "既にそのカテゴリー名は登録されております。";
					$flag = 1; break;
				}
//				elseif ($sc && $num != $num_ && $url == $url_) {
//				elseif ($sc && $num != $num_) {
//					$ERROR[] = "既にそのファイル名は登録されております。";
//					$flag = 1; break;
//				}
				else { $N_LIST .= $val; }
			}
		}

		if ($flag == 0) {
			$OUT = fopen($file,"w");
			fwrite($OUT,$N_LIST);
			fclose($OUT);

			if ($mc && $sc) {
				$cate1 = $mc;
				$cate2 = $sc;
				$cate3 = $num;
			}
			elseif ($mc) {
				$cate1 = $mc;
				$cate2 = $num;
				$cate3 = 0;
			}

//			$sql = "SELECT state FROM r_cate WHERE cate1='$cate1' AND cate2='$cate2' AND cate3='$cate3';";
//			if ($result = pg_exec($conn_id,$sql)) {
//				$list = pg_fetch_array($result);
//				$state = $list['state'];
//			}

//			if ($state != 1) { $state = 2; }

			$sql  = "UPDATE r_cate set" .
					" c_name='$name'" .
					" WHERE cate1='$cate1' AND cate2='$cate2' AND cate3='$cate3';";
			if (!pg_exec($conn_id,$sql)) { $ERROR[] = "カテゴリー名を変更できませんでした。"; }

			if (!$ERROR) {
				$msg = "$name_r から $name に変更しました。<BR>\n<BR>\n";
			}
		}
	}

}



function del($mc,$sc,$num) {
// global $CATE_DIR,$msg,$ERROR,$conn_id;
global $CATE_DIR,$ERROR,$conn_id;
global $msg;

	if ($mc && $sc) { $file = "$CATE_DIR/$mc". "_$sc.dat"; }
	elseif ($mc) { $file = "$CATE_DIR/$mc.dat"; }
	else { $ERROR[] = "カテゴリー番号が認識できません。"; }

	if (!$ERROR) {
		$flag = 0;
		if (file_exists($file)) {
			$LIST = file($file);
			foreach ($LIST AS $val) {
				list($h_num_,$num_,$name_,$url_) = explode("<>",$val);
				if ($num != $num_) { $N_LIST[$h_num_] = "$num_<>$name_<>$url_<>\n"; }
				else {
					$name = $name_;
					if ($mc && $sc) { $flag = check1($mc,$sc,$num); }
					elseif ($mc) { $flag = check2($mc,$num); }
				}
				if ($flag == 1) { break; }
			}
		}

		if ($flag == 0) {
			if ($N_LIST) {
				ksort($N_LIST);
				$n_list = "";
				$i = 1;
				foreach ($N_LIST AS $val) {
					$n_list .= "$i<>$val";
					$i++;
				}
				$OUT = fopen($file,"w");
				fwrite($OUT,$n_list);
				fclose($OUT);
			}
			else {
				unlink($file);
			}

			if ($mc && $sc > 0) {
				$cate1 = $mc;
				$cate2 = $sc;
				$cate3 = $num;
			}
			elseif ($mc) {
				$cate1 = $mc;
				$cate2 = $num;
				$cate3 = 0;
			}

//			$sql = "SELECT state FROM r_cate WHERE cate1='$cate1' AND cate2='$cate2' AND cate3='$cate3';";
//			if ($result = pg_exec($conn_id,$sql)) {
//				$list = pg_fetch_array($result);
//				$state = $list['state'];
//			}
//			if ($state == 0 || $state == 2) {
//				if ($cate3 > 0) {
//					$sql = "UPDATE r_cate SET state='3' WHERE cate1='$cate1' AND cate2='$cate2' AND cate3='$cate3';";
//				}
//				else {
//					$sql = "UPDATE r_cate SET state='3' WHERE cate1='$cate1' AND cate2='$cate2';";
//				}
//			}
//			else {

				if ($cate3 > 0) {
					$sql = "DELETE FROM r_cate WHERE cate1='$cate1' AND cate2='$cate2' AND cate3='$cate3';";
				}
				else {
					$sql = "DELETE FROM r_cate WHERE cate1='$cate1' AND cate2='$cate2';";
				}

//			}
			if (!pg_exec($conn_id,$sql)) { $ERROR[] = "カテゴリーを削除できませんでした。"; }
			if (!$ERROR) {
				$msg = "$name を削除しました。<BR>\n<BR>\n";
			}
		}
		else {
			$ERROR[] = "削除しようとしているカテゴリーに、サブカテゴリー又は商品が登録されています。全て削除又は移動してから削除して下さい。";
		}
	}

}



function down($mc,$sc,$num) {
// global $CATE_DIR,$msg,$ERROR;
global $CATE_DIR,$ERROR;
global $msg;

	if ($mc && $sc) { $file = "$CATE_DIR/$mc". "_$sc.dat"; }
	elseif ($mc) { $file = "$CATE_DIR/$mc.dat"; }
	else { $ERROR[] = "カテゴリー番号が認識できません。"; }
	if (!$ERROR) {
		if (file_exists($file)) {
			$LIST = file($file);
			foreach ($LIST AS $val) {
				list($h_num_,$num_,$name_,$url_) = explode("<>",$val);
				$url_ = rtrim($url_);
				$N_LIST[$h_num_] = "$num_<>$name_<>$url_<>\n";
				if ($num == $num_) {
					$down_num = $h_num_ - 1;
					$up_num = $h_num_;
					$CU_LIST = "$num_<>$name_<>$url_<>\n";
			}
		}
		$CD_LIST = $N_LIST[$down_num];

		$N_LIST[$down_num] = $CU_LIST;
		$N_LIST[$up_num] = $CD_LIST;
		ksort($N_LIST);

		$i = 1;
		foreach ($N_LIST AS $val) {
			$n_list .= "$i<>$val";
			$i++;
		}
		$OUT = fopen($file,"w");
		fwrite($OUT,$n_list);
		fclose($OUT);
		}
	}
}



function up($mc,$sc,$num) {
// global $CATE_DIR,$msg,$ERROR;
global $CATE_DIR,$ERROR;
global $msg;

	if ($mc && $sc) { $file = "$CATE_DIR/$mc". "_$sc.dat"; }
	elseif ($mc) { $file = "$CATE_DIR/$mc.dat"; }
	else { $ERROR[] = "カテゴリー番号が認識できません。"; }
	if (!$ERROR) {
		if (file_exists($file)) {
			$LIST = file($file);
			foreach ($LIST AS $val) {
				list($h_num_,$num_,$name_,$url_) = explode("<>",$val);
				$url_ = rtrim($url_);
				$N_LIST[$h_num_] = "$num_<>$name_<>$url_<>\n";
				if ($num == $num_) {
					$up_num = $h_num_ + 1;
					$down_num = $h_num_;
					$CD_LIST = "$num_<>$name_<>$url_<>\n";
			}
		}
		$CU_LIST = $N_LIST[$up_num];

		$N_LIST[$down_num] = $CU_LIST;
		$N_LIST[$up_num] = $CD_LIST;
		ksort($N_LIST);

		$i = 1;
		foreach ($N_LIST AS $val) {
			$n_list .= "$i<>$val";
			$i++;
		}
		$OUT = fopen($file,"w");
		fwrite($OUT,$n_list);
		fclose($OUT);

		}
	}
}



function check1($mc,$sc,$num) {
global $CATE_DIR,$conn_id;

	$count = 0;
	$sql = "SELECT count(*) FROM category WHERE cate1='$mc' AND cate2='$sc' AND cate3='$num' AND state<'3';";
	if ($result = pg_exec($conn_id,$sql)) {
		list($count) = pg_fetch_array($result,0);
	}

	if ($count > 0) { $flag = 1; }
	else { $flag = 0; }

	return $flag;

}



function check2($mc,$num) {
global $CATE_DIR;

	$file = "$CATE_DIR/$mc" . "_$num.dat";
	if (file_exists($file)) { $flag = 1; }
	else { $flag = 0; }

	return $flag;

}
?>
