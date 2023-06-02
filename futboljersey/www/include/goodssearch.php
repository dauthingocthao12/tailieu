<?PHP
/*

	ネイバーズスポーツ　商品検索表示

*/
function goodssearch($word)
{
	global $PHP_SELF, $conn_id, $cate_table, $goods_table, $search_view, $script, $index,
		$IMAGE, $IMAGEF, $waribiki, $TAX_, $b_file,
		$DISCOUNT_C, $GOODS_DISCOUNT_C, $GOODS_DISCOUNT_CATE, $DISCOUNT_PAR, $DISCOUNT_PAR2;

	$word = $_GET['word'];
	$word = trim($word);
#	$words = mb_convert_kana($word,"asKV","EUC-JP");
	$words = mb_convert_kana($word,"asKV","UTF-8");
	$WORD = explode(" ",$words);
	$where="";
	if ($WORD) {
		foreach ($WORD as $VAL) {
			$VAL = trim($VAL);
			if (!$VAL) {
				continue;
			}
			//$where .= " AND (j.g_name like '%$VAL%' OR j.code like '%$VAL%')";															//	del ookawara 2015/09/24
			$where .= " AND (j.g_name like '%$VAL%' OR j.code like '%$VAL%' OR j.name_head like '%$VAL%' OR j.name_foot like '%$VAL%')";	//	add ookawara 2015/09/24
		}
	}

	$html = <<<WAKABA
<h2 class="title-nbs">商品検索</h2>
<FORM action="$PHP_SELF" method="GET">
<div class="search-box">
<INPUT type="text" name="word" value="$word" class="search-bar input"><INPUT type="submit" value="検索" class="search-button">
</div>
</FORM>

WAKABA;

	//	ページ
	$page = $_GET['page'];
	$page = trim($page);
	#	$page = mb_convert_kana($page,"n","EUC-JP");
	$page = mb_convert_kana($page, "n", "UTF-8");
	if (!$page) {
		$page = 1;
	}

	if ($where) {
		$sql =	"SELECT count(distinct j.g_num) as count FROM $cate_table i,$goods_table j" .
				" WHERE i.g_num=j.g_num".
				" AND i.display='1'".
				" AND i.state!='3'".
				" AND i.cate1 NOT IN ('99')".	//	add ookawara 2015/01/29
				$where.";";
		if ($result = mysqli_query($conn_id,$sql)) {
			$list = mysqli_fetch_array($result);
			$max = $list['count'];
		}
		if ($max > 0) {
			$max_page = ceil($max / $search_view);
			$s = $search_view * ($page - 1);
			$e = ($search_view * $page) - 1;
			if ($e >= $max) {
				$e = $max - 1;
			}
			$view_s = $s + 1;
			$view_e = $e + 1;

			$page_msg = $max . "件キーワードに当てはまる商品がありました。<br> ";
			if ($max_page > 1) {
				$page_msg .= $view_s . "-" . $view_e . "件目 " . $page . "/" . $max_page . "ページ";
			}
		}
	}

	if (!$word) {
		$s_msg = "検索する商品名を入力し検索ボタンを押してください。<BR>\n<BR>\n";
	} elseif (!$page_msg) {
		$s_msg = "キーワードに当てはまる商品はありませんでした。<BR>\n<BR>\n";
	}

	if ($s_msg) {
		$html .= <<<WAKABA
      <TABLE border="0" width="95%" cellspacing="0" cellpadding="0">
          <TR>
            <TD align="center">$s_msg</TD>
          </TR>
      </TABLE>

WAKABA;
	} else {
		//	ブランド名抽出
		if (file_exists($b_file)) {
			$B_LIST = file($b_file);
			foreach ($B_LIST as $val) {
				list($b_num_, $b_name_, $del_) = explode("<>", $val);
				$B_LINE[$b_num_] = $b_name_;
			}
		}

		$html .= "<form action=\"" . "/cago.php\" method=\"POST\" id=\"set_cart\">\n";
		$html .= "	<input type=\"hidden\" name=\"action\" value=\"add\" />\n";
		$html .= "	<input type=\"hidden\" name=\"code\" id=\"goods_code\" />\n";
		$html .= "	<input type=\"hidden\" name=\"name\" id=\"goods_name\" />\n";
		$html .= "	<input type=\"hidden\" name=\"kakaku\" id=\"goods_kakaku\" />\n";
		$html .= "	<input type=\"hidden\" name=\"size\" id=\"goods_size\" />\n";

		$html .= "	<span class=\"search-count\">" . $page_msg . "</span>\n";
		$html .= "	<div class=\"item-list\" >\n";

		//	商品読み込み
		$offset = ($page - 1) * $search_view;
		$limit_max = $offset + $search_view;
		if ($max < $limit_max) {
			$limit = $max % $search_view;
		} else {
			$limit = $search_view;
		}
		$limit_num = " LIMIT $limit";
		if ($offset != 0) {
			$limit_num .= " OFFSET $offset";
		}

		//$sql =	"SELECT distinct j.g_num, j.g_name, j.code, j.price, j.sale_price,".												//	del ookawara 2015/09/24
		$sql =	"SELECT distinct j.g_num, coalesce(j.name_head, '') || coalesce(j.g_name, '') ||  coalesce(j.name_foot, '') AS g_name, j.code, j.price, j.sale_price,".	//	add ookawara 2015/09/24
				" j.options, j.caption, j.brand, j.comment, j.soldout" .
				", j.size_list".														//	add ookawara 2015/09/15
				" FROM $cate_table i,$goods_table j" .
				" WHERE i.g_num=j.g_num AND i.display='1' $where" .
				" $limit_num;";
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
				$size_list = $list['size_list'];			//	add ookawara 2015/09/15

				//	add ookawara 2015/09/15	start
				$OPTION = array();
				$op_size = "";
				if ($size_list) {
					$op_size = unserialize($size_list);
				}
				if ($op_size) {
					foreach ($op_size as $key => $VAL) {
						if ($VAL['type'] == 3) {
							continue;
						}
						$OPTION[] = $VAL['size'];
					}
				} else {
					//	add ookawara 2015/09/15	end
					$options = preg_replace("/\r/", "", $options);
					$OPTION = explode("\n", $options);
				}	//	add ookawara 2015/09/15

				$opt = "";
				$size = "";
				foreach ($OPTION as $val) {
					if (preg_match("/^\*/", $val) || $val == "") {
						continue;
					}
					$val = preg_replace("/^\//", "", $val);
					if ($size != "") {
						$size .= ", ";
					}
					$val = preg_replace("/\[.*\]/", "", $val);
					$opt .= "<option value=\"" . $val . "\">$val</option>\n";
					$size .= "$val";
				}
				if (!$opt) {
					$soldout = 1;
				}

				$imgf_file = "./" . IMAGEF . "/" . $code . ".jpg";
				if (file_exists($imgf_file)) {
					$IMGF = "<img src=\"/" . IMAGEF . "/" . $code . "\" width=\"90\" height=\"105\" border=\"0\" alt=\"" . $g_name . "\">";
				} else {
					$IMGF = "<img src=\"/" . IMAGE . "/nowprinting90x105.gif\" width=\"90\" height=\"105\" border=\"0\" alt=\"nowprinting\">";
				}

				$price_msg = "";
				if ($price > 0 && $sale_price > 0 && $price < $sale_price) {
					$sale_price = 0;
					$s_price = $price;
				} elseif ($price > 0 && $sale_price > 0 && $price == $sale_price) {
					if ($GOODS_DISCOUNT_C == 1 && $DISCOUNT_PAR2 > 0) {
						$flag = 0;
						if ($GOODS_DISCOUNT_CATE) {
							foreach ($GOODS_DISCOUNT_CATE as $VAL) {
								$VAL = trim($VAL);
								if ($mc_num == $VAL) {
									$flag = 1;
								}
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
						foreach ($GOODS_DISCOUNT_CATE as $VAL) {
							$VAL = trim($VAL);
							if ($mc_num == $VAL) {
								$flag = 1;
							}
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
					if (!$waribiki && $sale_price != 0) {
						$s_price = $sale_price;
					} else {
						$s_price = $price;
					}
					if ($DISCOUNT_C == 1) {
						$sale_price = 0;
						$s_price = $price;
					}
				}
				$price_ = floor($price * ($TAX_ + 1) + 0.5);
				$sale_price_ = floor($sale_price * ($TAX_ + 1) + 0.5);
				unset($wariritu);
				if ($price_ && $sale_price_) {
					$wariritu = 100 - round($sale_price_ / $price_ * 100);
				}

				$souryou_muryou_check_price = $price_;
				if ($price_ && $sale_price_) {
					$souryou_muryou_check_price = $sale_price_;
				}

				$price_ = number_format($price_);
				$sale_price_ = number_format($sale_price_);
				if (!$waribiki && $sale_price > 0) {
					unset($wariritu_msg);
					if ($wariritu) {
						$wariritu_msg = "<br>" . $wariritu . "%割引";
					}
					$price_msg = "<s>$price_" . "円(税込み)</s> <font color=\"#ff0000\">" . $sale_price_ . "円(税込み)" . $wariritu_msg . "</font>";
				} else {
					$price_msg = "$price_" . "円(税込み)";
				}

				//	リンク
				$as = "<a href=\"" . "/" . GOODS_SCRIPT . "/g" . $g_num . "/" . $index . "\" title=\"" . $g_name . "\nクリックすると詳細が見られます。\">";
				$af = "</a>";

				$amari = $i % 2;
				if ($amari == 0) {
					$bgcolor = "#ffffff";
				} else {
					$bgcolor = "#e4e995";
				}

				//	soldoutチェック
				if ($soldout != 1) {
					$soldout_msg  = "<select name=\"size_" . $code . "\" id=\"size_" . $code . "\" class=\"roundness\">\n";
					$soldout_msg .= $opt;
					$soldout_msg .= "</select>\n";
					$soldout_msg .= "<input type=\"button\" value=\"購入\" class=\"btn-standard buy-button\" OnClick=\"set_goods_data('" . $code . "', '" . $g_name . "', '" . $s_price . "'); return false;\">\n";
				} else {
					$soldout_msg  = "<b><font color=\"#ff0000\">SOLD OUT</font></b>\n";
				}

				//	送料無料表示
				$souryou_muryou = "";
				if (free_shipping > 0 && free_shipping <= $souryou_muryou_check_price) {
					$souryou_muryou = "<span class=\"red\">【送料無料】</span>";
				}


				$html .= "		<div class=\"item-entry\">\n";
				$html .= "			<div class=\"item-entry-details\">\n";
				$html .= "				<div class=\"item-entry-pic\">\n";
				$html .= 					$as . $IMGF . "<br>\n[商品詳細]" . $af . "\n";
				$html .= "				</div>\n";
				$html .= "				<dl>\n";
				$html .= "					<dt>商品名</dt>\n";
				$html .= "						<dd>" . $souryou_muryou . $g_name . "</dd>\n";
				$html .= "					<dt>商品番号</dt>\n";
				$html .= "						<dd>" . $code . "</dd>\n";
				$html .= "					<dt>ブランド名</dt>\n";
				$html .= "						<dd>" . $B_LINE[$brand] . "</dd>\n";
				$html .= "				</dl>\n";
				$html .= "			</div>\n";
				$html .= "			<div class=\"item-entry-buy\">\n";
				$html .= "				<div class=\"item-entry-price\">\n";
				$html .= "					$price_msg";
				$html .= "				</div>\n";
				$html .= "					$soldout_msg";
				$html .= "			</div>\n";
				$html .= "		</div>\n";
				$i++;
			}


			// $html .= "</table>\n";
			$html .= "	</div>\n";
			$html .= "</form>\n";
			$html .= "<br />\n";

			//	ページ処理
			if ($max_page > 1) {

				$html .= <<<WAKABA

		<DIV class="box-row box-content equally-spaced">

WAKABA;

				if ($page != 1) {
					$page_b = $page - 1;
					$b_url = "$PHP_SELF?word=" . urlencode($words);
					if ($page_b > 1) {
						$b_url .= "&page=$page_b";
					}

					$html .= <<<WAKABA
            <DIV>
            <A href="$b_url" class="btn-standard"><B>前の $search_view 件</B></A>
            </DIV>

WAKABA;
				}

				if ($max_page != $page && $max_page != 1) {
					$page_n = $page + 1;
					$view_n = $max - ($page * $search_view);
					if ($view_n > $view) {
						$view_n = $search_view;
					} else {
						$view_n = $view_n;
					}
					$n_url = "$PHP_SELF?word=" . urlencode($words);
					if ($page_n > 1) {
						$n_url .= "&page=$page_n";
					}
					$html .= <<<WAKABA
            <DIV">
            	<A href="$n_url" class="btn-standard"><B>次の $view_n 件</B></A>
            </DIV>

WAKABA;
				}

				$html .= <<<WAKABA

WAKABA;
			}
		}
	}

	return $html;
}
