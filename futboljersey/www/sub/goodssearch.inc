<?PHP
//	商品検索
function goodssearch($word) {
global $PHP_SELF,$conn_id,$cate_table,$goods_table,$search_view,$script,$index,
		$IMAGE,$IMAGEF,$waribiki,$TAX_,$b_file,
		$DISCOUNT_C,$GOODS_DISCOUNT_C,$GOODS_DISCOUNT_CATE,$DISCOUNT_PAR,$DISCOUNT_PAR2;

	$word = $_GET['word'];
	$word = trim($word);
#	$words = mb_convert_kana($word,"asKV","EUC-JP");
	$words = mb_convert_kana($word,"asKV","UTF-8");
	$WORD = explode(" ",$words);
	if ($WORD) {
		foreach ($WORD AS $VAL) {
			$VAL = trim($VAL);
//			$VAL = strtoupper($VAL);
			if (!$VAL) { continue; }
			$where .= " AND (j.g_name like '%$VAL%' OR j.code like '%$VAL%')";
		}
	}

	$html = <<<WAKABA
<div class="con_name"><div class="con_text">商品検索</div></div>
      <table width="750px">
          <TR>
            <TD align="center">
            <FORM action="$PHP_SELF" method="GET">
            <INPUT size="40" type="text" name="word" value="$word"><INPUT type="submit" value="検索">
            </FORM>
            </TD>
          </TR>
      </TABLE>

WAKABA;

	//	ページ
	$page = $_GET['page'];
	$page = trim($page);
#	$page = mb_convert_kana($page,"n","EUC-JP");
	$page = mb_convert_kana($page,"n","UTF-8");
	if  (!$page) { $page = 1; }

	if ($where) {
		$sql =	"SELECT count(distinct j.g_num) as count FROM $cate_table i,$goods_table j" .
//	soldout表示
//				" WHERE i.g_num=j.g_num AND i.display='1' AND i.state!='3' AND j.soldout='0' $where;";
				" WHERE i.g_num=j.g_num AND i.display='1' AND i.state!='3' $where;";
		if ($result = mysqli_query($conn_id,$sql)) {
			$list = mysqli_fetch_array($result);
			$max = $list['count'];
		}
		if ($max > 0) {
			$max_page = ceil($max/$search_view);
			$s = $search_view * ($page - 1);
			$e = ($search_view * $page) - 1;
			if ($e >= $max) { $e = $max - 1; }
			$view_s = $s + 1;
			$view_e = $e+1;

			$page_msg = $max . "件キーワードに当てはまる商品がありました。<br> ";
			if ($max_page > 1) { $page_msg .= $view_s . "-" . $view_e . "件目 " . $page . "/" . $max_page . "ページ"; }
		}
	}

	if (!$word) { $s_msg = "検索する商品名を入力し検索ボタンを押してください。<BR>\n<BR>\n"; }
	elseif (!$page_msg) { $s_msg = "キーワードに当てはまる商品はありませんでした。<BR>\n<BR>\n"; }

	if ($s_msg) {
		$html .= <<<WAKABA
      <TABLE border="0" width="95%" cellspacing="0" cellpadding="0">
        <TBODY>
          <TR>
            <TD align="center">$s_msg</TD>
          </TR>
        </TBODY>
      </TABLE>

WAKABA;
	}
	else {
		//	ブランド名抽出
		if (file_exists($b_file)) {
			$B_LIST = file($b_file);
			foreach ($B_LIST AS $val) {
				list($b_num_,$b_name_,$del_) = explode("<>",$val);
				$B_LINE[$b_num_] = $b_name_;
			}
		}

		$html .= <<<WAKABA

<TABLE border="0" width="95%" cellpadding="0" cellspacing="1">
<TBODY>
<TR>
<TD colspan="3">$page_msg</TD>
</TR>
</TBODY>
</TABLE>
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

		//	商品読み込み
		$offset = ($page - 1) * $search_view;
		$limit_max = $offset + $search_view;
		if ($max < $limit_max) {
			$limit = $max % $search_view;
		}
		else {
			$limit = $search_view;
		}
		$limit_num = " LIMIT $limit";
		if ($offset != 0) {
			$limit_num .= " OFFSET $offset";
		}

/*
		$sql =	"SELECT distinct i.cate1, i.num, j.g_num, j.g_name, j.code, j.price, j.sale_price,".
				" j.options, j.caption, j.brand, j.comment, j.soldout" .
				" FROM $cate_table i,$goods_table j" .
				" WHERE i.g_num=j.g_num AND i.display='1' $where" .
				" ORDER BY i.num DESC, j.g_name DESC $limit_num;";
*/
		$sql =	"SELECT distinct j.g_num, j.g_name, j.code, j.price, j.sale_price,".
				" j.options, j.caption, j.brand, j.comment, j.soldout" .
				" FROM $cate_table i,$goods_table j" .
				" WHERE i.g_num=j.g_num AND i.display='1' $where" .
				" $limit_num;";
//				" ORDER BY i.num DESC, j.g_name DESC $limit_num;";
		if ($result = mysqli_query($conn_id,$sql)) {
			$i = 0;
			$bgoods = "";
			while ($list = mysqli_fetch_array($result)) {
				$mc_num = $list['cate1'];
				$num = $list['num'];
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
					$IMGF = "<IMG src=\"/$IMAGEF/$code.jpg\" width=\"90\" height=\"105\" border=\"0\">";
				}
				else {
					$IMGF = "<IMG src=\"/$IMAGE/nowprinting90x105.gif\" width=\"90\" height=\"105\" border=\"0\">";
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
				}
				elseif ($GOODS_DISCOUNT_C == 1) {
					$flag = 0;
					if ($GOODS_DISCOUNT_CATE) {
						foreach ($GOODS_DISCOUNT_CATE AS $VAL) {
							$VAL = trim($VAL);
							if ($mc_num == $VAL) { $flag = 1; }
						}
					}
					if ($flag != 1) {
						$sale_price_ = floor($price * ((100 - $DISCOUNT_PAR) / 100));
						if (!$sale_price || ($sale_price > 0 && $sale_price > $sale_price_)) {
							$sale_price = $sale_price_;
						}
						$s_price = $sale_price;
					}
					else {
						$sale_price = 0;
						$s_price = $price;
					}
				}
				else {
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
				} else {
					$price_msg = "$price_" . "円(税込み)";
				}

				//	リンク
				$as = "<A href=\"/$script/g$g_num/$index\" title=\"{$g_name}\nクリックすると詳細が見られます。\">";
//				$as = "<A href=\"/$script/$mc_num/$sc_num/$s2c_num/$g_num/$index\" title=\"{$g_name} 詳細\">";
				$af = "</A>";

				$amari = $i % 2;
				if ($amari == 0) { $bgcolor = "#ffffff"; } else { $bgcolor = "#e4e995"; }

				//	soldoutチェック
				if ($soldout != 1) {
					$soldout_msg = <<<WAKABA
            <TD rowspan="3">
            <SELECT name="size">
$opt
            </SELECT><BR><BR>
            <INPUT type="submit" value="購入">
            </TD>
WAKABA;
				}
				else {
					$soldout_msg = <<<WAKABA
            <TD rowspan="3"><B><FONT color="#ff0000">SOLD OUT</FONT></B></TD>
WAKABA;
				}

				//	送料無料表示
				$souryou_muryou = "";
				if (free_shipping > 0 && free_shipping <= $souryou_muryou_check_price) {
					$souryou_muryou = "<font color=\"#ff0000\">【送料無料】</font>";
				}

				$html .= <<<WAKABA
          <FORM action="/cago.php" method="POST">
          <INPUT type="hidden" name="action" value="add" />
          <INPUT type="hidden" name="code" value="$code">
          <INPUT type="hidden" name="name" value="$g_name">
          <INPUT type="hidden" name="kakaku" value="$s_price">
          <TR align="center">
            <TD height="90" rowspan="3">$as$IMGF$af</TD>
            <TD height="30" colspan="2">{$souryou_muryou}{$g_name}</TD>
            <TD rowspan="3">{$as}<IMG src="/image/ditaile.gif" width="38" height="32" border="0">{$af}</TD>
$soldout_msg
          </TR>
          <TR align="center">
            <TD height="30" colspan="2">$code</TD>
          </TR>
          <TR align="center">
            <TD height="30" width="50%">$B_LINE[$brand]</TD>
            <TD width="50%">$price_msg</TD>
          </TR>
          <TR>
            <TD colspan="5"></TD>
          </TR>
          </FORM>

WAKABA;
				$i++;
			}

			$html .= <<<WAKABA
      </TABLE>
      <BR>

WAKABA;

			//	ページ処理
			if ($max_page > 1) {

				$html .= <<<WAKABA
      <TABLE border="0" width="95%">
        <TBODY>
          <TR bgcolor="#ffffff">

WAKABA;

				if ($page != 1) {
					$page_b = $page-1;
					$b_url = "$PHP_SELF?word=" . urlencode($words);
					if ($page_b > 1) { $b_url .= "&page=$page_b"; }

					$html .= <<<WAKABA
            <TD align="center">
            <A href="$b_url"><B>前の $search_view 件</B></A>
            </TD>

WAKABA;
				}

				if ($max_page != $page && $max_page != 1) {
					$page_n = $page + 1;
					$view_n = $max-($page*$search_view);
					if ($view_n > $view) { $view_n = $search_view; } else { $view_n = $view_n; }
					$n_url = "$PHP_SELF?word=" . urlencode($words);
					if ($page_n > 1) { $n_url .= "&page=$page_n"; }
					$html .= <<<WAKABA
            <TD align="center">
            <A href="$n_url"><B>次の $view_n 件</B></A>
            </TD>

WAKABA;
				}

				$html .= <<<WAKABA
　         </TR>
        </TBODY>
      </TABLE>
      <BR>

WAKABA;

			}
		}
	}
/*
	//	注意事項など追加
	$cautions_file = "foot_cautions.htm";
	if (file_exists($cautions_file)) {
		$html .= file_get_contents($cautions_file);
	}
	//	メーカー一覧
	$maker_file = "./sub/maker_list.inc";
	if (file_exists($maker_file)) {
		include("$maker_file");
		$html .= maker_list();
	}
*/

	return $html;

}
?>
