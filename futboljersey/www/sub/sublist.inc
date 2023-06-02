<?PHP
//	サブカテゴリー一覧
function sublist($VALUE,$CHECK) {
global $DIR_CATE,$LOG_DIR,$script,
		$conn_id,$cate_table;

	foreach ($VALUE AS $VAL) {
		$VAL = trim($VAL);
		if (!$VAL) { continue; }
		if ($c_file) { $c_file .= "_"; }
		$c_file .= (int)$VAL;
	}
##	if (eregi(".htm",$c_file)) { $htm = ""; }
	if (preg_match("/\.htm/",$c_file)) { $htm = ""; }
	else { $htm = ".htm"; }
	$c_file = "./$LOG_DIR/$c_file$htm";

	if (file_exists($c_file)) {
		$fp = fopen ($c_file, "r");
		if ($fp) {
			while (!feof ($fp)) {
				$html .= fgets($fp, 4096);
			}
		}
	}
	else {
##		$cate1 = (int)eregi_replace("[^0-9]","",$CHECK['main']);
		$cate1 = (int)preg_replace("/[^0-9]/","",$CHECK['main']);
		
		//	表示可能カテゴリー読み込み
//	後で削除AND cate1<'20'
		$sql  = "SELECT cate1, cate2, cate3 FROM $cate_table" .
				" WHERE cate1='$cate1' AND display='1' AND state!='3'".
//				" AND cate1<'20'".
				" GROUP BY cate1, cate2, cate3;";
		if ($result = mysqli_query($conn_id,$sql)) {
			while ($list = mysqli_fetch_array($result)) {
				$cate1_ = $list['cate1'];
				$cate2_ = $list['cate2'];
				$cate3_ = $list['cate3'];
				$CATE[$cate1_][$cate2_][$cate3_] = 1;
			}
		}

		//	表示確認
		$file = "./$DIR_CATE/category.inc";
		$LIST = file($file);
		foreach ($LIST AS $VAL) {
			$VAL = trim($VAL);
			list($h_num_,$mc_num_,$cate_name_,$cate_title_) = explode("<>",$VAL);
			if ($CHECK['main'] && $CHECK['main'] != $mc_num_) { continue; }
			else {
				$mc_num = $mc_num_;
				$cate_name = $cate_name_;
				$cate_state = $cate_state_;
				break;
			}
		}

		//	カテゴリー表示
		if ($mc_num) {
			$html = <<<WAKABA
<div class="con_name"><div class="con_text">$cate_name_ カテゴリー 一覧</div></div>

WAKABA;

			if ($CATE[$mc_num]) {
				//	サブカテゴリー表示
				unset($LIST2);
				unset($sc_html);
				$file = "./$DIR_CATE/$mc_num.dat";
				if (file_exists($file)) {
					$LIST2 = file($file);
				}
				if ($LIST2) { $sc_html = subcategory($CATE,$mc_num,$LIST2); }
			}
			if ($sc_html) {
					$html .= <<<WAKABA
<table width="750px">
$sc_html
</table>
<br />

WAKABA;
			}
			else { unset($html); }
		}

		if ($html) {
//			$OUT = fopen("$c_file","w");
//			fwrite($OUT,$html);
//			fclose($OUT);
//			@chmod($c_file,0666);
		}
	}

	if (!$html) {
		$sent_url = "/$script/";
		header ("Location: $sent_url\n\n");
	}

	return $html;

}



//	サブカテゴリー表示
function subcategory($CATE,$mc_num,$LIST2) {
global $DIR_CATE,$script,$index;

	$SUB_CATEGORY = array();
	foreach ($LIST2 AS $VAL2) {
		$VAL2 = trim($VAL2);
		if (!$VAL2) { continue; }
		list($h_num2,$sc_num,$sc_name,$cate_title2) = explode("<>",$VAL2);
		$SUB_CATEGORY[$h_num2] = "$h_num2<>$sc_num<>$sc_name<>$cate_title2<>";
	}
	krsort($SUB_CATEGORY);

	foreach ($SUB_CATEGORY AS $val) {
		list($h_num2,$sc_num,$sc_name,$cate_title2) = explode("<>",$val);
		if ($CATE[$mc_num][$sc_num]) {
			$link = (int)sprintf("%02d",$sc_num);
			$html .= <<<WAKABA
<tr>
		<th colspan="3" class="cate1"><B><A href="/$script/$mc_num/$link/$index" title="$sc_name">$sc_name</A></B></th>
</tr>
WAKABA;
			unset($LIST3);
			unset($ssc_html);
			$file = "./$DIR_CATE/" . $mc_num . "_" . $sc_num . ".dat";
			if (file_exists($file)) {
				$LIST3 = file($file);
			}
			if ($LIST3) { $ssc_html = sub2category($CATE,$mc_num,$sc_num,$LIST3); }
			$html .= <<<WAKABA
$ssc_html

WAKABA;
		}
	}

	if (!$ssc_html) { unset($html); }

	return $html;

}



//	サブカテゴリー2表示
function sub2category($CATE,$mc_num,$sc_num,$LIST3) {
global $DIR_CATE,$script,$index;

	$i = 1;
	$flag = 0;
	$SUB_CATEGORY2 = array();
	foreach ($LIST3 AS $VAL) {
		$VAL = trim($VAL);
		if (!$VAL) { continue; }
		list($h_num,$s2c_num,$s2c_name,$s2c_file) = explode("<>",$VAL);
		$SUB_CATEGORY2[$h_num] = "$h_num<>$s2c_num<>$s2c_name<>$s2c_file<>";
	}
	krsort($SUB_CATEGORY2);

	foreach ($SUB_CATEGORY2 AS $val) {
		list($h_num,$s2c_num,$s2c_name,$s2c_file) = explode("<>",$val);
		if ($CATE[$mc_num][$sc_num][$s2c_num]) {
			$flag = 1;
			$amari = $i % 3;
			if ($amari == 1 && $amari != "$b_amari") {
					$html .= "<tr>\n";
			}
			$link = (int)sprintf("%02d",$sc_num);
			$link2 = (int)sprintf("%02d",$s2c_num);
			$html .= <<<WAKABA
<td class="cate3-3">
      <A href="/$script/$mc_num/$link/$link2/$index" title="$s2c_name">$s2c_name</A>
</td>

WAKABA;
			if ($amari == 0 && $amari != "$b_amari") {
				$html .= "</tr>\n";
			}
		}
		else { continue; }

		$b_amari = $amari;
		$i++;
	}
	++$b_amari;
	if ($b_amari == 3) { $b_amari = 0; }
	if ($flag != 1) {
		unset($html);
	}
	elseif ($amari != 0 && $amari != "$b_amari") {
		$max = 3 - $amari;
		for ($ii=1; $ii<=$max; $ii++) {
			$html .= "<td class=\"cate3-3\">&nbsp;</td>\n";
		}
		$html .= "</tr>\n";
	}

	return $html;

}
?>
