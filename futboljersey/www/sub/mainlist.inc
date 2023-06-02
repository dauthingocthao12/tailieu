<?PHP
//	メインカテゴリー一覧
function mainlist($VALUE,$CHECK) {
global $DIR_CATE,$LOG_DIR,$m_cate_file,$script,$index,
		$conn_id,$cate_table;

	$c_file = "./$LOG_DIR/$m_cate_file";

	$flag = 0;
	if (file_exists($c_file)) {
		$fp = fopen ($c_file, "r");
		if ($fp) {
			while (!feof ($fp)) {
				$html .= fgets($fp, 4096);
			}
			$flag = 1;
		}
	}
	else {
		//	表示確認
		unset($CLASS_M_L);
		$file = "./$DIR_CATE/category.inc";
		$LIST = file($file);
		foreach ($LIST AS $VAL) {
			$VAL = trim($VAL);
			list($h_num,$mc_num,$cate_name,$cate_title) = explode("<>",$VAL);
			$CLASS_M_L[$h_num]['mc_num'] = $mc_num;
			$CLASS_M_L[$h_num]['cate_name'] = $cate_name;
		}

		//	カテゴリー表示
		if ($CLASS_M_L) {

			$html = <<<WAKABA

<div class="con_name"><div class="con_text">カテゴリー 一覧</div></div>

WAKABA;

			//	表示可能カテゴリー読み込み
//	AND cate1<'20'後で削除
			$sql  = "SELECT cate1, cate2, cate3 FROM $cate_table" .
					" WHERE display='1' AND state!='3'".
//					" AND cate1<'20'".
					" GROUP BY cate1, cate2, cate3;";
			if ($result = mysqli_query($conn_id,$sql)) {
				while ($list = mysqli_fetch_array($result)) {
					$cate1_ = $list['cate1'];
					$cate2_ = $list['cate2'];
					$cate3_ = $list['cate3'];
					$CATE[$cate1_][$cate2_][$cate3_] = 1;
				}
			}

			//	サブカテゴリー読込 表示
			foreach ($CLASS_M_L AS $KEY => $VAL) {
				unset($mc_html);
				unset($c_html);
				unset($sc_html);
				$flag = 0;
				$mc_num = (int)$CLASS_M_L[$KEY]['mc_num'];
				$cate_name = $CLASS_M_L[$KEY]['cate_name'];
				if ($CATE[$mc_num]) {
					//	メインカテゴリー表示
					$c_html = <<<WAKABA
	<tr>
		<th colspan="3" class="cate1"><A href="/$script/$mc_num/$index" title="$cate_name">$cate_name</A></th>
	</tr>

WAKABA;
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
$c_html
$sc_html
      </TABLE>
      <BR>

WAKABA;
					$flag = 1;
				}
			}
		}

		if ($html) {
//			$OUT = fopen("$c_file","w");
//			fwrite($OUT,$html);
//			fclose($OUT);
//			@chmod($c_file,0666);
		}

	}

	if ($flag == 0) {
		$html .= <<<WAKABA
<BR>
今現在表示出来るカテゴリーはありません。<BR>

WAKABA;
	}

	return $html;

}



//	サブカテゴリー表示
function subcategory($CATE,$mc_num,$LIST2) {
global $DIR_CATE,$script,$index;

	foreach ($LIST2 AS $VAL2) {
		$VAL2 = trim($VAL2);
		if (!$VAL2) { continue; }
		list($h_num2,$sc_num,$sc_name,$cate_title2) = explode("<>",$VAL2);
		if ($CATE[$mc_num][$sc_num]) {
			$link = (int)sprintf("%02d",$sc_num);
			$html .= <<<WAKABA
	<tr>
		<th colspan="3" class="cate2">　<B><A href="/$script/$mc_num/$link/$index" title="$sc_name">$sc_name</B></th>
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
	foreach ($LIST3 AS $VAL) {
		$VAL = trim($VAL);
		if (!$VAL) { continue; }
		list($h_num,$s2c_num,$s2c_name,$s2c_file) = explode("<>",$VAL);
		if ($CATE[$mc_num][$sc_num][$s2c_num]) {
			$flag = 1;
			$amari = $i % 3;
			if ($amari == 1 && $amari != "$b_amari") {
					$html .= "    <tr>\n";
			}
			$link = (int)sprintf("%02d",$sc_num);
			$link2 = (int)sprintf("%02d",$s2c_num);
			$html .= <<<WAKABA
		<td class="cate3-3"><A href="/$script/$mc_num/$link/$link2/$index" title="$s2c_name">$s2c_name</A></td>

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
