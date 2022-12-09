<?PHP
//	商品一覧
function goodslist($VALUE,$CHECK) {
global $DIR_CATE,$LOG_DIR,$script,
		$conn_id,$cate_table;

	$c_file = "l";
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
##		$cate2 = (int)eregi_replace("[^0-9]","",$CHECK['s']);
		$cate2 = (int)preg_replace("/[^0-9]/","",$CHECK['s']);
##		$cate3 = (int)eregi_replace("[^0-9]","",$CHECK['l']);
		$cate3 = (int)preg_replace("/[^0-9]/","",$CHECK['l']);
		//	表示可能カテゴリー読み込み
		$sql  = "SELECT cate1, cate2, cate3 FROM $cate_table" .
				" WHERE cate1='$cate1' AND cate2='$cate2' AND cate3='$cate3' AND display='1' AND state!='3'".
				" GROUP BY cate1, cate2, cate3;";
		if ($result = pg_query($conn_id,$sql)) {
			while ($list = pg_fetch_array($result)) {
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
			if ($cate1 && $cate1 != $mc_num_) { continue; }
			else {
				$mc_num = $mc_num_;
				$cate_name = $cate_name_;
				break;
			}
		}

		//	カテゴリー表示
		if ($mc_num) {
			if ($CATE[$mc_num]) {
				//	サブカテゴリー表示
				unset($LIST2);
				unset($sc_html);
				$file = "./$DIR_CATE/$mc_num.dat";
				if (file_exists($file)) {
					$LIST2 = file($file);
				}
				if ($LIST2) { list($sc_html,$sc_name,$s2c_name) = subcategory($CATE,$mc_num,$LIST2,$CHECK); }
			}
			if ($sc_html) {
				$html = <<<WAKABA
<div class="con_name"><div class="con_text">{$cate_name_}　{$sc_name}　{$s2c_name} 商品一覧</div></div>
$sc_html

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
function subcategory($CATE,$mc_num,$LIST2,$CHECK) {
global $DIR_CATE,$script;

	unset($check);
	if ($CHECK['s']) {
##		$check = (int)eregi_replace("[^0-9]","",$CHECK['s']);
		$check = (int)preg_replace("/[^0-9]/","",$CHECK['s']);
	}

	foreach ($LIST2 AS $VAL2) {
		$VAL2 = trim($VAL2);
		if (!$VAL2) { continue; }
		list($h_num2_,$sc_num_,$sc_name_,$cate_title2_) = explode("<>",$VAL2);
		if ($check && $check != $sc_num_) { continue; }
		else {
			$sc_num = $sc_num_;
			$sc_name = $sc_name_;
			break;
		}
	}

	if ($CATE[$mc_num][$sc_num]) {
		unset($LIST3);
		unset($ssc_html);
		$file = "./$DIR_CATE/" . $mc_num . "_" . $sc_num . ".dat";
		if (file_exists($file)) {
			$LIST3 = file($file);
		}
		if ($LIST3) { list($ssc_html,$s2c_name) = sub2category($CATE,$mc_num,$sc_num,$LIST3,$CHECK); }
	}

	$html .= $ssc_html;

	if (!$ssc_html) { unset($html); }

	return array($html,$sc_name,$s2c_name);

}



//	サブカテゴリー2表示
function sub2category($CATE,$mc_num,$sc_num,$LIST3,$CHECK) {

	unset($check);
	if ($CHECK['l']) {
##		$check = (int)eregi_replace("[^0-9]","",$CHECK['l']);
		$check = (int)preg_replace("/[^0-9]/","",$CHECK['l']);
	}

	$i = 1;
	$flag = 0;
	foreach ($LIST3 AS $VAL) {
		$VAL = trim($VAL);
		if (!$VAL) { continue; }
		list($h_num_,$s2c_num_,$s2c_name_) = explode("<>",$VAL);
		if ($check && $check != $s2c_num_) { continue; }
		else {
			$s2c_num = $s2c_num_;
			$s2c_name = $s2c_name_;
			break;
		}
	}

	if ($CATE[$mc_num][$sc_num][$s2c_num]) {
		$html = goods_list($mc_num,$sc_num,$s2c_num);
	}

	return array($html,$s2c_name);

}



//	商品リスト
function goods_list($mc_num,$sc_num,$s2c_num) {
global $conn_id,$cate_table,$goods_table,$script,$IMAGE,$IMAGEF,$index,
		$waribiki,$TAX_,$b_file,$DIR_CATE,$DISCOUNT_C,
		$GOODS_DISCOUNT_C,$GOODS_DISCOUNT_CATE,$DISCOUNT_PAR,$DISCOUNT_PAR2;

	if (!$index) { unset($index); }

	$sql =	"SELECT i.num, j.g_num, j.g_name, j.code, j.price, j.sale_price, j.options,".
			" j.caption, j.brand, j.comment, j.soldout" .
			" FROM $cate_table i,$goods_table j" .
			" WHERE  i.cate1='$mc_num' AND i.cate2='$sc_num' AND i.cate3='$s2c_num'".
			" AND i.g_num=j.g_num AND i.display='1'" .
			" ORDER BY j.g_num;";
	if ($result = pg_query($conn_id,$sql)) {
		$count = pg_num_rows($result);
	}

	if ($count > 0) {
		//	ブランド名抽出
		if (file_exists($b_file)) {
			$B_LIST = file($b_file);
			foreach ($B_LIST AS $val) {
				list($b_num_,$b_name_,$del_) = explode("<>",$val);
				$B_LINE[$b_num_] = $b_name_;
			}
		}

		$html = <<<WAKABA

      <BR>
<table id="itemlist">
	<tr>
		<th rowspan="3" class="itemlist01">画像</th>
		<th colspan="2">商品名</th>
		<th rowspan="3" class="itemlist03">詳細</th>
		<th rowspan="3" class="itemlist04">サイズ<br>購入</th>
	</tr>
	<tr>
		<th colspan="2">商品番号</th>
	</tr>
	<tr>
		<th class="itemlist02">ブランド名</th>
		<th class="itemlist02">価格</th>
	</tr>


WAKABA;

		while ($list = pg_fetch_array($result)) {
			$g_num = $list['g_num'];
			$GLIST[$g_num]['num'] = $list['num'];
			$GLIST[$g_num]['g_num'] = $list['g_num'];
			$GLIST[$g_num]['g_name'] = $list['g_name'];
			$GLIST[$g_num]['code'] = $list['code'];
			$GLIST[$g_num]['price'] = $list['price'];
			$GLIST[$g_num]['sale_price'] = $list['sale_price'];
			$GLIST[$g_num]['options'] = $list['options'];
			$GLIST[$g_num]['caption'] = $list['caption'];
			$GLIST[$g_num]['brand'] = $list['brand'];
			$GLIST[$g_num]['comment'] = $list['comment'];
			$GLIST[$g_num]['soldout'] = $list['soldout'];
		}

		$file = "./$DIR_CATE/" . $mc_num . "_" . $sc_num . "_" . $s2c_num . ".dat";
		if (file_exists($file)) {
			$LIST4 = file($file);
		}

		if ($LIST4) {
			foreach ($LIST4 AS $VAL) {
				$VAL = trim($VAL);
				list($g_num) = explode("<>",$VAL);
				$num = $GLIST[$g_num]['num'];
				$g_num = $GLIST[$g_num]['g_num'];
				$g_name = $GLIST[$g_num]['g_name'];
				$code = $GLIST[$g_num]['code'];
				$price = $GLIST[$g_num]['price'];
				$sale_price = $GLIST[$g_num]['sale_price'];
				$options = $GLIST[$g_num]['options'];
				$caption = $GLIST[$g_num]['caption'];
				$brand = $GLIST[$g_num]['brand'];
				$comment = $GLIST[$g_num]['comment'];
				$soldout = $GLIST[$g_num]['soldout'];
				if (!$g_num) { continue; }

##				$options = ereg_replace("\r", "",$options);
				$options = preg_replace("/\r/", "",$options);
				$OPTION = explode("\n",$options);
				$opt = "";
				$size = "";
				foreach ($OPTION AS $val) {
##					if (ereg("^\*",$val) || $val == "") { continue; }
					if (preg_match("/^\*/",$val) || $val == "") { continue; }
##					$val = ereg_replace("^/","",$val);
					$val = preg_replace("/^\//","",$val);
					if ($size != "") { $size .= ", "; }
##					$val = eregi_replace("\[.*\]","",$val);
					$val = preg_replace("/\[.*\]/","",$val);
					$opt .= "            <OPTION value=\"$val\">$val</OPTION>\n";
					$size .= "$val";
				}
				if (!$opt) { $soldout = 1; }

				$imgf_file = "./$IMAGEF/$code.jpg";
//				if ($soldout == 1) {
//					$IMGF = "<IMG src=\"/$IMAGE/soldout90x105.gif\" width=\"90\" height=\"105\" border=\"0\">";
//				}
//				elseif (file_exists($imgf_file)) {
				if (file_exists($imgf_file)) {
					$IMGF = "<IMG src=\"/$IMAGEF/$code.jpg\" width=\"90\" height=\"105\" border=\"0\" alt=\"".$g_name."\">";
				}
				else {
					$IMGF = "<IMG src=\"/$IMAGE/nowprinting90x105.gif\" width=\"90\" height=\"105\" border=\"0\" alt=\"nowprinting\">";
				}

				$price_msg = "";
				if ($price > 0 && $sale_price > 0 && $price < $sale_price) {
					$sale_price = 0;
					$s_price = $price;
				} elseif ($price > 0 && $sale_price > 0 && $price == $sale_price) {
					if ($GOODS_DISCOUNT_C == 1 && $DISCOUNT_PAR2 > 0) {
						$flag = 0;
						if ($GOODS_DISCOUNT_CATE) {
							foreach ($GOODS_DISCOUNT_CATE AS $VAL) {
								$VAL = trim($VAL);
								if ($mc_num == $VAL) { $flag = 1; }
							}
						}
						if ($flag != 1) {
							$sale_price_ = floor($price * ((100 - $DISCOUNT_PAR2) / 100));
							$sale_price = $sale_price_;
							$s_price = $sale_price;
						}
						else {
							$sale_price = 0;
							$s_price = $price;
						}
					} else {
						$sale_price = 0;
						$s_price = $price;
					}
				} elseif ($GOODS_DISCOUNT_C == 1) {
					$flag = 0;
					if ($GOODS_DISCOUNT_CATE) {
						foreach ($GOODS_DISCOUNT_CATE AS $VAL) {
							$VAL = trim($VAL);
							if ($mc_num == $VAL) { $flag = 1; }
						}
					}
					if ($flag != 1) {
						$sale_price_ = floor($price * ((100 - $DISCOUNT_PAR) / 100));
						if ($sale_price < 1 || ($sale_price > 0 && $sale_price > $sale_price_)) {
							$sale_price = $sale_price_;
						}
						$s_price = $sale_price;
					} else {
						$sale_price = 0;
						$s_price = $price;
					}
				} else {
					if (!$waribiki && $sale_price != 0) { $s_price = $sale_price; } else { $s_price = $price; }
					if ($DISCOUNT_C == 1) {
						$sale_price = 0;
						$s_price = $price;
					}
				}
				$price_ = floor($price * ($TAX_ + 1) + 0.5);
				$sale_price_ = floor($sale_price * ($TAX_ + 1) + 0.5);
				unset($wariritu);
				if ($price_ && $sale_price_) {
					$wariritu = 100 - round($sale_price_/$price_*100);
				}

				$souryou_muryou_check_price = $price_;
				if ($price_ && $sale_price_) { $souryou_muryou_check_price = $sale_price_; }

				$price_ = number_format($price_);
				$sale_price_ = number_format($sale_price_);
				if (!$waribiki && $sale_price > 0) {
					unset($wariritu_msg);
					if ($wariritu) { $wariritu_msg = "<br>".$wariritu."%割引"; }
					$price_msg = "<S>$price_" . "円(税込み)</S> <FONT color=\"#ff0000\">$sale_price_" . "円(税込み)$wariritu_msg</FONT>";
//					$price_msg = "<FONT color=\"#ff0000\">$sale_price_" . "円(税込み)$wariritu_msg</FONT>";
				}
				else {
					$price_msg = "$price_" . "円(税込み)";
				}

				//	リンク
				$as = "<A href=\"/$script/$mc_num/$sc_num/$s2c_num/$g_num/$index\" title=\"{$g_name}\nクリックすると詳細が見られます。\">";
				$af = "</A>";

				$amari = $i % 2;
				if ($amari == 0) { $bgcolor = "#ffffff"; } else { $bgcolor = "#e4e995"; }

				//	soldoutチェック
				if ($soldout != 1) {
					$soldout_msg = <<<WAKABA
            <SELECT name="size">
$opt
            </SELECT><BR><BR>
            <INPUT type="submit" value="購入">
WAKABA;
				}
				else {
					$soldout_msg = <<<WAKABA
            <B><FONT color="#ff0000">SOLD OUT</FONT></B>
WAKABA;
				}

				//	送料無料表示
				$souryou_muryou = "";
				if (free_shipping > 0 && free_shipping <= $souryou_muryou_check_price) {
					$souryou_muryou = "<font color=\"#ff0000\">【送料無料】</font>";
				}

				$html .= <<<WAKABA
<FORM action="/cago.php" method="POST">
<INPUT type="hidden" name="code" value="$code">
<INPUT type="hidden" name="name" value="$g_name">
<INPUT type="hidden" name="kakaku" value="$s_price">
<tr>
<td rowspan="3" class="itemlist01">$as$IMGF$af</td>
<td colspan="2">{$souryou_muryou}{$g_name}</td>
<td rowspan="3">{$as}<IMG src="/image/ditaile.gif" width="38" height="32" border="0">{$af}</td>
<td rowspan="3">$soldout_msg</td>
</tr>
<tr>
<td colspan="2">$code</td>
</tr>
<tr>
<td>$B_LINE[$brand]</td>
<td>$price_msg</td>
</tr>
<tr>
</FORM>
WAKABA;
				$i++;
			}
		}

		$html .= <<<WAKABA
</table>
      <BR>

WAKABA;


	}

	return $html;

}
?>
