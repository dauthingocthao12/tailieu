<?PHP
//	商品コピー
function goods_4() {
	// global	$CATE_DIR,$c_file,$b_file,$list_file,
	// 		$mode,$cate_table,$goods_table,$IMAGEF,$IMAGEB,
	// 		$mc,$sc,$lc;
	global	$CATE_DIR,$IMAGEF,$IMAGEB,$cate_table,$c_file,$b_file,$list_file,$goods_table;
	$mode=$_POST['mode'];
	$CATE_DIR = "../category";
	$c_file = "$CATE_DIR/category.inc";
	$b_file = "../data/brand.dat";
	$list_file = "./data/ypath.dat";
	$IMAGEF = "../imagef";
	$IMAGEB = "../imageb";

	$cate_table = "category";
	$goods_table = "goods";

	if ($mode == "コピー") { copys(); }

	first00();

}



function first00() {
// global	$PHP_SELF,$ERROR,$msg,$cate_table,$goods_table,
// 		$c_file,$CATE_DIR,
// 		$main,$goods,
// 		$mc,$sc,$lc,$mc_r,$sc_r,
// 		$mc2,$sc2,$lc2,$mc2_r,$sc2_r;
global	$PHP_SELF,$ERROR,$CATE_DIR,$cate_table,$c_file,$goods_table;
global $msg; // @source copys()
$main=$_POST['main'];
$goods=$_POST['goods'];
$mc=$_POST['mc'];
$sc=$_POST['sc'];
$lc=$_POST['lc'];
$mc_r=$_POST['mc_r'];
$sc_r=$_POST['sc_r'];
$mc2=$_POST['mc2'];
$sc2=$_POST['sc2'];
$lc2=$_POST['lc2'];
$mc2_r=$_POST['mc2_r'];
$sc2_r=$_POST['sc2_r'];

	include ("../../cone.inc");

	if ($mc != $mc_r) { $sc = $lc = $mc2 = $sc2 = $lc2 = ""; }
	elseif ($sc != $sc_r) { $lc = $mc2 = $sc2 = $lc2 = ""; }

	if ($mc2 != $mc2_r) { $sc2 = $lc2 = ""; }
	elseif ($sc2 != $sc2_r) { $lc2 = ""; }

	$C_LIST = file($c_file);
	if ($mc) { $selected = "selected"; } else { $selected = ""; }
	$l_mc = "        <OPTION value=\"\" $selected>選択して下さい</OPTION>\n";
	foreach ($C_LIST AS $val) {
		list($h_num_,$mc_num_,$mc_name_) = explode("<>",$val);
		if ($mc == $mc_num_) { $selected = "selected"; $mc_name = $mc_name_; } else { $selected = ""; }
		$l_mc .= "        <OPTION value=\"$mc_num_\" $selected>$mc_name_</OPTION>\n";
	}

	$mc_file = "$CATE_DIR/$mc.dat";
	if ($mc && file_exists($mc_file)) {
		$SC_LIST = file($mc_file);
		if ($sc) { $selected = "selected"; } else { $selected = ""; }
		$l_sc = "        <OPTION value=\"\" $selected>選択して下さい</OPTION>\n";

		$SUB_CATEGORY = array();
		foreach ($SC_LIST AS $val) {
			list($h_num_,$num_,$sc_name_) = explode("<>",$val);
			$SUB_CATEGORY[$h_num_] = "$num_<>$sc_name_<>";
		}
		krsort($SUB_CATEGORY);

		foreach ($SUB_CATEGORY AS $val) {
			$list = explode("<>",$val);
			if ($sc == $list[0]) { $selected = "selected"; $sc_name = $list[1]; } else { $selected = ""; }
			$l_sc .= "        <OPTION value=\"$list[0]\" $selected>$list[1]</OPTION>\n";
		}
	}
	else {
		$l_sc = "        <OPTION value=\"\" $selected>-----</OPTION>\n";
	}

	$sc_file = "$CATE_DIR/$mc" . "_$sc.dat";
	if ($sc && file_exists($sc_file)) {
		$LC_LIST = file($sc_file);
		if ($lc) { $selected = "selected"; } else { $selected = ""; }
		$l_lc = "        <OPTION value=\"\" $selected>選択して下さい</OPTION>\n";

		$SUB_CATEGORY2 = array();
		foreach ($LC_LIST AS $val) {
			list($h_num_,$lc_num_,$lc_name_) = explode("<>",$val);
			$SUB_CATEGORY2[$h_num_] = "$lc_num_<>$lc_name_<>";
		}
		krsort($SUB_CATEGORY2);

		foreach ($SUB_CATEGORY2 AS $val) {
			$list = explode("<>",$val);
			if ($lc == $list[0]) { $selected = "selected"; $sc_name = $list[1]; } else { $selected = ""; }
			$l_lc .= "        <OPTION value=\"$list[0]\" $selected>$list[1]</OPTION>\n";
		}
	}
	else {
		$l_lc = "        <OPTION value=\"\" $selected>-----</OPTION>\n";
	}

	$sql = "SELECT count(*) FROM $cate_table";
	$sql .= " WHERE state<'3'";
	if ($mc) { $sql .= " AND cate1='$mc'"; }
	if ($sc) { $sql .= " AND cate2='$sc'"; }
	if ($lc) { $sql .= " AND cate3='$lc'"; }
	$sql .= ";";
	if ($result = pg_exec($conn_id,$sql)) {
		list($max) = pg_fetch_array($result,0);
		if (!$mc) { $tourokusu = "総登録数： $max 件<BR>\n<BR>\n"; }
		else { $tourokusu = "このカテゴリーの総登録数： $max 件<BR>\n<BR>\n"; }
	}

	if ($max > 0 && $mc && $sc && $lc) {

		if (!$mc2 && !$sc2 && !$lc2) {
			$l_mc2 = $l_mc;
			$l_sc2 = $l_sc;
			$l_lc2 = $l_lc;
			$mc2 = $mc;
			$sc2 = $sc;
		}
		else {

			$C_LIST = file($c_file);
			if ($mc2) { $selected = "selected"; } else { $selected = ""; }
			$l_mc2 = "        <OPTION value=\"\" $selected>選択して下さい</OPTION>\n";
			foreach ($C_LIST AS $val) {
				list($h_num_,$mc_num_,$mc_name_) = explode("<>",$val);
				if ($mc2 == $mc_num_) { $selected = "selected"; $mc_name = $mc_name_; } else { $selected = ""; }
				$l_mc2 .= "        <OPTION value=\"$mc_num_\" $selected>$mc_name_</OPTION>\n";
			}

			$mc2_file = "$CATE_DIR/$mc2.dat";
			if ($mc2 && file_exists($mc2_file)) {
				$SC_LIST = file($mc2_file);
				if ($sc2) { $selected = "selected"; } else { $selected = ""; }
				$l_sc2 = "        <OPTION value=\"\" $selected>選択して下さい</OPTION>\n";

				$SUB_CATEGORY = array();
				foreach ($SC_LIST AS $val) {
					list($h_num_,$num_,$sc_name_) = explode("<>",$val);
					$SUB_CATEGORY[$h_num_] = "$num_<>$sc_name_<>";
				}
				krsort($SUB_CATEGORY);

				foreach ($SUB_CATEGORY AS $val) {
					$list = explode("<>",$val);
					if ($sc2 == $list[0]) { $selected = "selected"; $sc_name = $list[1]; } else { $selected = ""; }
					$l_sc2 .= "        <OPTION value=\"$list[0]\" $selected>$list[1]</OPTION>\n";
				}
			}
			else {
				$l_sc2 = "        <OPTION value=\"\" $selected>-----</OPTION>\n";
			}

			$sc2_file = "$CATE_DIR/$mc2" . "_$sc2.dat";
			if ($sc2 && file_exists($sc2_file)) {
				$LC_LIST = file($sc2_file);
				if ($lc2) { $selected = "selected"; } else { $selected = ""; }
				$l_lc2 = "        <OPTION value=\"\" $selected>選択して下さい</OPTION>\n";

				$SUB_CATEGORY2 = array();
				foreach ($LC_LIST AS $val) {
					list($h_num_,$lc_num_,$lc_name_) = explode("<>",$val);
					$SUB_CATEGORY2[$h_num_] = "$lc_num_<>$lc_name_<>";
				}
				krsort($SUB_CATEGORY2);

				foreach ($SUB_CATEGORY2 AS $val) {
					$list = explode("<>",$val);
					if ($lc2 == $list[0]) { $selected = "selected"; $sc_name = $list[1]; } else { $selected = ""; }
					$l_lc2 .= "        <OPTION value=\"$list[0]\" $selected>$list[1]</OPTION>\n";
				}
			}
			else {
				$l_lc2 = "        <OPTION value=\"\" $selected>-----</OPTION>\n";
			}
		}
	}

	if ($ERROR) {
		ERROR($ERROR);
		unset($_SESSION['ERROR']);
	}

	echo <<<WAKABA
$msg
<FORM action="$PHP_SELF" method="POST">
<INPUT type="hidden" name="main" value="$main">
<INPUT type="hidden" name="goods" value="$goods">
<INPUT type="hidden" name="mc_r" value="$mc">
<INPUT type="hidden" name="sc_r" value="$sc">
<INPUT type="hidden" name="mc2_r" value="$mc2">
<INPUT type="hidden" name="sc2_r" value="$sc2">
コピーをする商品のカテゴリー選択し、決定ボタンを押して下さい。<BR>
※コピーをしている商品で、コピー元商品、コピーの商品の変更した場合<BR>
　変更商品だけに変更が適応されるのは、その商品のカテゴリー、ypath、表示の切替のみで、<BR>
　他の部分を変更した場合、管理商品番号が同じ商品は全て変更されます。<BR>
<TABLE border="0" style="font-size : 12px;" cellpadding="1" cellspacing="1" bgcolor="#666666">
  <TBODY>
    <TR bgcolor="#ffffff">
      <TH bgcolor="#cccccc" width="80">カテゴリー</TH>
      <TD><SELECT name="mc" onchange="submit();">
$l_mc
      </SELECT><SELECT name="sc" onchange="submit();">
$l_sc
      </SELECT><SELECT name="lc">
$l_lc
      </SELECT></TD>
      <TD align="center"><INPUT type="submit" value="決定"></TD>
    </TR>
  </TBODY>
</TABLE>
</FORM>
$tourokusu

WAKABA;

	if ($max > 0 && $mc && $sc && $lc) {

		echo <<<WAKABA
$msg
<FORM action="$PHP_SELF" method="POST">
<INPUT type="hidden" name="main" value="$main">
<INPUT type="hidden" name="goods" value="$goods">
<INPUT type="hidden" name="mc" value="$mc">
<INPUT type="hidden" name="sc" value="$sc">
<INPUT type="hidden" name="lc" value="$lc">
<INPUT type="hidden" name="mc_r" value="$mc">
<INPUT type="hidden" name="sc_r" value="$sc">
<INPUT type="hidden" name="mc2_r" value="$mc2">
<INPUT type="hidden" name="sc2_r" value="$sc2">
コピー先のカテゴリーを選択し、移動する商品にチェックをいれ移動を押して下さい。<BR>
<TABLE border="0" style="font-size : 12px;" cellpadding="1" cellspacing="1" bgcolor="#666666">
  <TBODY>
    <TR bgcolor="#ffffff">
      <TH bgcolor="#cccccc" width="80">カテゴリー</TH>
      <TD><SELECT name="mc2" onchange="submit();">
$l_mc2
      </SELECT><SELECT name="sc2" onchange="submit();">
$l_sc2
      </SELECT><SELECT name="lc2">
$l_lc2
      </SELECT></TD>
    </TR>
  </TBODY>
</TABLE><BR>

WAKABA;


		$sql =	"SELECT i.num, j.g_num, j.g_name, j.code, j.price" .
				" FROM $cate_table i,$goods_table j" .
				" WHERE  i.cate1='$mc' AND i.cate2='$sc' AND i.cate3='$lc' AND i.g_num=j.g_num;";
		if (!$result = pg_exec($conn_id,$sql)) { $ERROR[] = "登録商品情報を読み込みできませんでした。"; }
		else {
			echo <<<WAKABA
<BR>
<TABLE border="0" style="font-size : 12px;" cellspacing="1" bgcolor="#666666" cellpadding="3">
  <TBODY>
    <TR bgcolor="#cccccc">
      <TH></TH>
      <TH>管理商品番号</TH>
      <TH>商品名</TH>
      <TH>商品番号</TH>
      <TH>金額</TH>
    </TR>

WAKABA;

			$lc_file = "$CATE_DIR/$mc" . "_$sc" . "_$lc.dat";
			if (file_exists($lc_file)) {
				$LIST = file($lc_file);
				$i = 1;
				foreach ($LIST AS $val) {
					list($g_num_) = explode("<>",$val);
					$GOODS[$i] = $g_num_;
					$i++;
				}
			}

			$max = pg_numrows($result);
			for($i=0; $i<$max; $i++) {
				list($num,$g_num,$name,$code,$price) = pg_fetch_array($result,$i);
				$GOODS_L[$g_num] = "$num<>$g_num<>$name<>$code<>$price<>\n";
			}

			$count = count($GOODS);
			$last = $count - 1;
			$i = 0;
			foreach ($GOODS AS $val) {
				list($num,$g_num,$name,$code,$price) = explode("<>",$GOODS_L[$val]);

				echo <<<WAKABA
    <TR bgcolor="#ffffff" align="center">
      <TD><INPUT type="checkbox" name="num[]" value="$num"></TD>
      <TD>$g_num</TD>
      <TD>$name</TD>
      <TD>$code</TD>
      <TD>$price 円</TD>
    </TR>

WAKABA;
				$i++;
			}

			echo <<<WAKABA
    <TR bgcolor="#ffffff" align="center">
      <TD colspan="5"><INPUT type="submit" name="mode" value="コピー">　<INPUT type="reset"></TD>
    </TR>
  </TBODY>
</TABLE>

WAKABA;
		}

		echo ("</FORM>\n");

	}
	elseif ($mc && $sc && $lc) {
		echo("このカテゴリーには商品は登録されておりません。<BR>\n");
	}

	pg_close($conn_id);

}


function copys() {
// global	$ERROR,$msg,$cate_table,$goods_table,$CATE_DIR,
// 		$mc,$sc,$lc,$IMAGEF,$IMAGEB,
// 		$mc2,$sc2,$lc2,$num;
global	$ERROR,$IMAGEF,$IMAGEB,$CATE_DIR,$cate_table,$goods_table;
global $msg;
$mc=$_POST['mc'];
$sc=$_POST['sc'];
$lc=$_POST['lc'];
$mc2=$_POST['mc2'];
$sc2=$_POST['sc2'];
$lc2=$_POST['lc2'];
$num=$_POST['num'];

	if (!$num) { $ERROR[] = "コピーする商品が選択されておりません。"; }
	if ($mc == $mc2 && $sc == $sc2 && $lc == $lc2) { $ERROR[] = "コピー先のカテゴリーが変更されておりません。"; }

	if (!$ERROR) {
		include ("../../cone.inc");

		foreach ($num AS $num_) {
			$sql = "SELECT g_num, ypath , display FROM $cate_table WHERE num='$num_';";
			if ($result = pg_exec($conn_id,$sql)) {
				list($g_num_,$ypath_,$display_) = pg_fetch_array($result,0);
				$sql =	"SELECT count(*) FROM $cate_table" .
						" WHERE cate1='$mc2' AND cate2='$sc2' AND cate3='$lc2' AND g_num='$g_num_';";
				if ($result = pg_exec($conn_id,$sql)) {
					list($count) = pg_fetch_array($result,0);
					if ($count > 0) { $ERROR[] = "既に選択された商品はコピー先のカテゴリーに登録されております。"; break; }
				}
				else { break; }

				$sql = "SELECT MAX(copy_num) FROM $cate_table WHERE g_num='$g_num_';";
				if ($result = pg_exec($conn_id,$sql)) {
					list($max) = pg_fetch_array($result,0);
				}
				else { break; }
				$copy_num = $max + 1;

				if (!$display_) { $display_ = "0"; }
				$sql = "INSERT INTO $cate_table (cate1,cate2,cate3,g_num,ypath,display,copy_num) VALUES (" .
						"'$mc2'," .
						"'$sc2'," .
						"'$lc2'," .
						"'$g_num_'," .
						"'$ypath_'," .
						"'$display_'," .
						"'$copy_num');";
				if (!pg_exec($conn_id,$sql)) {
					$ERROR[] = "商品データーをコピーできませんでした。";
					break;
				}
				else {
					$lc2_file = "$CATE_DIR/$mc2" . "_$sc2" . "_$lc2.dat";
					$OUT = fopen($lc2_file,"a");
					fwrite ($OUT,"$g_num_<>\n");
					fclose ($OUT);
					@chmod($lc2_file,0666);

					$sql = "SELECT code FROM $goods_table WHERE g_num='$g_num_';";
					if ($result = pg_exec($conn_id,$sql)) {
/*
						list($code) = pg_fetch_array($result,0);
						$img_fm = "$IMAGEF/$code.jpg";
						$img_fc = "$IMAGEF/$code" . "-$copy_num.jpg";
						if (file_exists($img_fm)) {
							copy($img_fm,$img_fc);
							@chmod($img_fc,0666);
						}
						$img_bm = "$IMAGEB/$code.jpg";
						$img_bc = "$IMAGEB/$code" . "-$copy_num.jpg";
						if (file_exists($img_bm)) {
							copy($img_bm,$img_bc);
							@chmod($img_bc,0666);
						}
*/
					}
					else { break; }
				}
			}
			else { break; }
		}

		pg_close($conn_id);

	}

	if (!$ERROR) {
		$msg = "商品のコピーが完了しました。";
//		$mc2 = $sc2 = $lc2 = "";
	}

}
?>
