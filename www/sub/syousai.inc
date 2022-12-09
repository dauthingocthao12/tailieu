<?PHP
//	商品詳細
function syousai($VALUE,$CHECK) {
global $conn_id,$cate_table,$goods_table,$script,$IMAGE,$IMAGEF,$IMAGEB,
		$waribiki,$TAX_,$b_file,$LOG_DIR,$DISCOUNT_C,$attention,
		$GOODS_DISCOUNT_C,$GOODS_DISCOUNT_CATE,$DISCOUNT_PAR,$DISCOUNT_PAR2;

	$c_file = "g";
	foreach ($VALUE AS $VAL) {
		$VAL = trim($VAL);
		if (!$VAL) { continue; }
		if ($c_file) { $c_file .= "_"; }
		$c_file .= (int)$VAL;
	}
##	if (eregi(".htm",$c_file)) {
	if (preg_match("/\.htm/i",$c_file)) {
		$htm = "";
	} else {
		$htm = ".htm";
	}
	$c_file = "./$LOG_DIR/$c_file$htm";

	if (file_exists($c_file)) {
		$fp = fopen ($c_file, "r");
		if ($fp) {
			while (!feof ($fp)) {
				$html .= fgets($fp, 4096);
			}
		}
	} else {
##		$num = (int)eregi_replace("[^0-9]","",$CHECK['g']);
		$num = (int)preg_replace("/[^0-9]/i","",$CHECK['g']);

		//	ブランド名抽出
		if (file_exists($b_file)) {
			$B_LIST = file($b_file);
			foreach ($B_LIST AS $val) {
				list($b_num_,$b_name_,$del_) = explode("<>",$val);
				$B_LINE[$b_num_] = $b_name_;
			}
		}

		//	商品詳細データー
/*
		$sql =	"SELECT i.cate1, j.g_num, j.g_name, j.code, j.price, j.sale_price, j.options,".
				" j.caption, j.brand, j.comment, j.soldout" .
				" FROM $cate_table i,$goods_table j" .
				" WHERE i.g_num=j.g_num AND j.num='$num' AND i.display='1'".
				" LIMIT 1";
*/
		$sql  = "SELECT g_num, g_name, code, price, sale_price, options, caption, brand, comment, soldout".
				" FROM $goods_table".
				" WHERE g_num='$num' LIMIT 1;";
		if ($result = pg_query($conn_id,$sql)) {
			$list = pg_fetch_array($result);
			$g_num = $list['g_num'];
			$g_name = $list['g_name'];
			$code = $list['code'];
			$price = $list['price'];
			$sale_price = $list['sale_price'];
			$options = $list['options'];
			$caption = $list['caption'];
			$brand = $list['brand'];
			$comment = $list['comment'];
			$soldout = $list['soldout'];
		}

		if ($g_num) {
			$sql  = "SELECT cate1 FROM $cate_table".
					" WHERE g_num='$g_num' AND display='1';";
			if ($result = pg_query($conn_id,$sql)) {
				while ($list = pg_fetch_array($result)) {
					$cate1 = $list['cate1'];
					$CATE[$cate1] = $cate1;
				}
			}
		}

		if ($g_name) {
##			$options = ereg_replace("\r", "",$options);
			$options = preg_replace("/\r/", "",$options);
			$OPTION = explode("\n",$options);
			$opt = "";
			$size = "";
			foreach ($OPTION AS $val) {
##				if (ereg("^\*",$val) || $val == "") { continue; }
				if (preg_match("/^\*/",$val) || $val == "") { continue; }
##				$val = ereg_replace("^/","",$val);
				$val = preg_replace("/^\//","",$val);
				if ($size != "") { $size .= ", "; }
##				$val = eregi_replace("\[.*\]","",$val);
				$val = preg_replace("/\[.*\]/i","",$val);
				$opt .= "            <OPTION value=\"$val\">$val</OPTION>\n";
				$size .= "$val";
			}
			if (!$opt) { $soldout = 1; }

			$img_no_file = "<img src=\"/".$IMAGE."/FILLER.gif\" width=\"300\" height=\"30\" border=\"0\" alt=\"FUTBOLJERSEY.com\" />";	//	add ookawara 2013/07/04

			$imgf_file = "./$IMAGEF/$code.jpg";
			$IMGF = $img_no_file;	//	add ookawara 2013/07/04
/*
			//if ($soldout == 1) {
			//	$IMGF = "<IMG src=\"/$IMAGE/SOLDOUT-300X350.gif\" width=\"300\" height=\"350\" border=\"0\" alt=\"SOLD OUT\">";
			//}
			//elseif (file_exists($imgf_file)) {
*/
			if (file_exists($imgf_file)) {
				$IMGF = "<IMG src=\"/$IMAGEF/$code.jpg\" width=\"300\" height=\"350\" border=\"0\" alt=\"$g_name\">";
			//} else {	//	del ookawara 2013/07/04
				//$IMGF = "<IMG src=\"/$IMAGE/FILLER.gif\" width=\"300\" height=\"30\" border=\"0\" alt=\"FUTBOLJERSEY.com\">";	//	del ookawara 2013/07/04
			}
			$imgb_file = "./$IMAGEB/$code.jpg";
			$IMGB = $img_no_file;	//	add ookawara 2013/07/04
			//if ($soldout != 1 && file_exists($imgb_file)) {
			if (file_exists($imgb_file)) {
				$IMGB = "<IMG src=\"/$IMAGEB/$code.jpg\" width=\"300\" height=\"350\" alt=\"$g_name\">";
			//} else {	//	del ookawara 2013/07/04
				//$IMGB = "<IMG src=\"/$IMAGE/FILLER.gif\" width=\"300\" height=\"30\" alt=\"FUTBOLJERSEY.com\">";	//	del ookawara 2013/07/04
			}

			//	add ookawara 2013/07/04
			$img03_file = "./".IMAGE03."/".$code.".jpg";
			$IMG03 = $img_no_file;
			if (file_exists($img03_file)) {
				$IMG03 = "<IMG src=\"/".IMAGE03."/".$code.".jpg\" width=\"300\" height=\"350\" border=\"0\" alt=\"".$g_name."\" />";
			}
			$img04_file = "./".IMAGE04."/".$code.".jpg";
			$IMG04 = $img_no_file;
			if (file_exists($img04_file)) {
				$IMG04 = "<IMG src=\"/".IMAGE04."/".$code.".jpg\" width=\"300\" height=\"350\" border=\"0\" alt=\"".$g_name."\" />";
			}
			$img05_file = "./".IMAGE05."/".$code.".jpg";
			$IMG05 = $img_no_file;
			if (file_exists($img05_file)) {
				$IMG05 = "<IMG src=\"/".IMAGE05."/".$code.".jpg\" width=\"300\" height=\"350\" border=\"0\" alt=\"".$g_name."\" />";
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
					} else {
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
						if ($CATE[$VAL]) { $flag = 1; }
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
			if ($price_ && $sale_price_) {
				$wariritu = 100 - round($sale_price_/$price_*100);
			}

			$souryou_muryou_check_price = $price_;
			if ($price_ && $sale_price_) { $souryou_muryou_check_price = $sale_price_; }

			$price_ = number_format($price_);
			$sale_price_ = number_format($sale_price_);
			if (!$waribiki && $sale_price > 0) {
				if ($wariritu) { $wariritu_msg = "<br><br>".$wariritu."%割引"; }
				$price_msg = "<S>$price_" . "円(税込み)</S> <FONT color=\"#ff0000\"><b>$sale_price_" . "円(税込み)</b>$wariritu_msg</FONT>";
				//$price_msg = "<FONT color=\"#ff0000\"><b>$sale_price_" . "円(税込み)</b>$wariritu_msg</FONT>";
			} else {
				$price_msg = "$price_" . "円(税込み)";
			}

			if ($waribiki || $DISCOUNT_C == 1) {
##				$comment = eregi_replace("([0-9])+\%OFF","",$comment);
				$comment = preg_replace("/([0-9])+\%OFF/i","",$comment);
			}

##			if (ereg("<!-- Attention -->",$caption)) {
			if (preg_match("/<!-- Attention -->/",$caption)) {
##				$caption = ereg_replace("<!-- Attention -->","$attention",$caption);
				$caption = preg_replace("/<!-- Attention -->/","$attention",$caption);
				$caption = trim($caption);
			}

			$msg = "";
			if ($caption || $comment) {
				$caption = nl2br($caption);
				$comment = nl2br($comment);
				$br = "";
				if ($caption != "") { $br = "<BR>"; }
				$msg = <<<WAKABA
      <BLOCKQUOTE><BR>
$caption$br
<FONT color="#ff0000">
$comment
</FONT>
      </BLOCKQUOTE>

WAKABA;
			}

			//	送料無料表示
			$souryou_muryou = "";
			if (free_shipping > 0 && free_shipping <= $souryou_muryou_check_price) {
				$souryou_muryou = "<font color=\"#ff0000\">【送料無料】</font><br>\n";
			}

			$html = <<<WAKABA
<FORM action="/cago.php" method="POST">
<INPUT type="hidden" name="code" value="$code">
<INPUT type="hidden" name="name" value="$g_name">
<INPUT type="hidden" name="kakaku" value="$s_price">

<table id="item">
	<tr>
		<th colspan="3"><A name="$code"></A><FONT size="+1">$g_name</FONT></th>
	</tr>
	<tr>
		<td colspan="2" class="item01">$IMGF</td>
		<td class="item01">$IMGB</td>
	</tr>
	<tr>
		<th  class="item02">商品名</th>
		<td  class="item03">{$souryou_muryou}{$g_name}</td>
		<td rowspan="5" class="item04">$msg

WAKABA;


			if ($soldout != 1) {
				$html .= <<<WAKABA
      <P align="center">サイズ：<SELECT name="size">
$opt
      </SELECT>　<INPUT type="submit" value="買い物かごに入れる"><BR>
      <BR>
      </P>

WAKABA;
			}
			else {
				$html .= <<<WAKABA
      <P align="center">
      <FONT size="+2" color="#ff0000"><B><I>SOLD OUT</I></B></FONT>
      </P>

WAKABA;
			}

			$html .= <<<WAKABA
</td>
	</tr>
	<tr>
		<th>商品番号</th>
		<td  class="item03">$code</td>
	</tr>
	<tr>
		<th>ブランド名</th>
		<td  class="item03">$B_LINE[$brand]</td>
	</tr>
	<tr>
		<th>サイズ</th>
		<td  class="item03">$size</td>
	</tr>
	<tr>
		<th>価格</th>
		<td  class="item03">$price_msg</td>
	</tr>
</table>

WAKABA;
		}
	}

	if ($ERROR) {
		$html = <<<WAKABA
<TABLE border="0" width="630" height="630" cellpadding="0" cellspacing="0">
    <TR align="center">
      <TD>
      <FONT color=\"#ff0000\"><B>エラー</B></FONT><BR>

WAKABA;

		foreach ($ERROR AS $val) {
			$html .= "$val<BR>\n";
		}

		$html .= <<<WAKABA
      </TD>
    </TR>
  </TBODY>
</TABLE>

WAKABA;
	}


	if ($html) {
		//$OUT = fopen("$c_file","w");
		//fwrite($OUT,$html);
		//fclose($OUT);
		//@chmod($c_file,0666);
	}

	if (!$html || !$cate1) {
		$sent_url = "/$script/";
		header ("Location: $sent_url\n\n");
	}

	return $html;

}
?>
