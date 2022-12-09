<?PHP
/*

	ネイバーズスポーツ　商品一覧

*/
function goods_list($VALUE, $CHECK) {

	$cate1 = (int)preg_replace("/[^0-9]/", "", $CHECK['main']);
	$cate2 = (int)preg_replace("/[^0-9]/", "", $CHECK['s']);
	$cate3 = (int)preg_replace("/[^0-9]/", "", $CHECK['l']);
	//	表示可能カテゴリー読み込み
	$sql  = "SELECT cate1, cate2, cate3 FROM ".T_CATE.
			" WHERE cate1='".$cate1."'".
			" AND cate2='".$cate2."'".
			" AND cate3='".$cate3."'".
			" AND display='1'".
			" AND state!='3'".
			" AND cate1 NOT IN ('99')".	//	add ookawara 2015/01/29
			" GROUP BY cate1, cate2, cate3;";
	if ($result = pg_query(DB, $sql)) {
		while ($list = pg_fetch_array($result)) {
			$cate1_ = $list['cate1'];
			$cate2_ = $list['cate2'];
			$cate3_ = $list['cate3'];
			$CATE[$cate1_][$cate2_][$cate3_] = 1;
		}
	}

	//	表示確認
	$file = "./".DIR_CATE."/category.inc";
	$LIST = file($file);
	foreach ($LIST AS $VAL) {
		$VAL = trim($VAL);
		list($h_num_,$mc_num_,$cate_name_,$cate_title_) = explode("<>", $VAL);
		if ($mc_num_ == 99) { continue; }	//	add ookawara 2015/01/29
		if ($cate1 && $cate1 != $mc_num_) {
			continue;
		} else {
			$mc_num = $mc_num_;
			$cate_name = $cate_name_;
			break;
		}
	}

	//	カテゴリー表示
	$html = "";
	if ($mc_num) {
		if ($CATE[$mc_num]) {
			//	サブカテゴリー表示
			unset($LIST2);
			unset($sc_html);
			$file = "./".DIR_CATE."/".$mc_num.".dat";
			if (file_exists($file)) {
				$LIST2 = file($file);
			}
			if ($LIST2) { list($sc_html, $sc_name, $s2c_name) = subcategory($CATE, $mc_num, $LIST2, $CHECK); }
		}
		if ($sc_html) {
			$html  = "<h2 class=\"title-nbs\">".$cate_name_."　".$sc_name."　".$s2c_name." 商品一覧</h2>\n";
			$html .= $sc_html;
		} else {
			unset($html);
		}
	}

	if (!$html) {
		//$sent_url = "/".GOODS_SCRIPT."/";		//	del ookawara 2016/01/21
		//header ("Location: $sent_url\n\n");	//	del ookawara 2016/01/21
		header404();							//	add ookawara 2016/01/21
	}

	return $html;

}



//	サブカテゴリー表示
function subcategory($CATE, $mc_num, $LIST2, $CHECK) {

	//	add ookawara 2015/01/29
	if ($mc_num == 99) {
		return;
	}

	unset($check);
	if ($CHECK['s']) {
		$check = (int)preg_replace("/[^0-9]/", "", $CHECK['s']);
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
		$file = "./".DIR_CATE."/" . $mc_num . "_" . $sc_num . ".dat";
		if (file_exists($file)) {
			$LIST3 = file($file);
		}
		if ($LIST3) { list($ssc_html, $s2c_name) = sub2category($CATE, $mc_num, $sc_num, $LIST3, $CHECK); }
	}

	$html .= $ssc_html;

	if (!$ssc_html) { unset($html); }

	return array($html, $sc_name, $s2c_name);

}



//	サブカテゴリー2表示
function sub2category($CATE, $mc_num, $sc_num, $LIST3, $CHECK) {

	//	add ookawara 2015/01/29
	if ($mc_num == 99) {
		return;
	}

	unset($check);
	if ($CHECK['l']) {
		$check = (int)preg_replace("/[^0-9]/", "", $CHECK['l']);
	}

	$i = 1;
	$flag = 0;
	foreach ($LIST3 AS $VAL) {
		$VAL = trim($VAL);
		if (!$VAL) { continue; }
		list($h_num_, $s2c_num_, $s2c_name_) = explode("<>", $VAL);
		if ($check && $check != $s2c_num_) {
			continue;
		} else {
			$s2c_num = $s2c_num_;
			$s2c_name = $s2c_name_;
			break;
		}
	}

	if ($CATE[$mc_num][$sc_num][$s2c_num]) {
		$html = goods_list_html($mc_num, $sc_num, $s2c_num);
	}

	return array($html, $s2c_name);

}



//	商品リスト
function goods_list_html($mc_num,$sc_num,$s2c_num) {
global $index, $waribiki, $TAX_, $DISCOUNT_C,
		$GOODS_DISCOUNT_C, $GOODS_DISCOUNT_CATE, $DISCOUNT_PAR, $DISCOUNT_PAR2;

	//	add ookawara 2015/01/29
	if ($mc_num == 99) {
		return;
	}

	if (!$index) { unset($index); }

	//$sql =	"SELECT i.num, j.g_num, j.g_name, j.code, j.price, j.sale_price, j.options,".											//	del ookawara 2015/09/24
	$sql =	"SELECT i.num, j.g_num, coalesce(j.name_head, '') || coalesce(j.g_name, '') ||  coalesce(j.name_foot, '') AS g_name, j.code, j.price, j.sale_price, j.options,".	//	add ookawara 2015/09/24
					" j.caption, j.brand, j.comment, j.soldout".
					", j.size_list".					//	add ookawara 2015/09/15
			" FROM ".T_CATE." i, ".T_GOODS." j".
			" WHERE  i.cate1='".$mc_num."'".
			" AND i.cate2='".$sc_num."'".
			" AND i.cate3='".$s2c_num."'".
			" AND i.g_num=j.g_num".
			" AND i.display='1'".
			" ORDER BY j.g_num;";
	if ($result = pg_query(DB, $sql)) {
		$count = pg_num_rows($result);
	}

	if ($count > 0) {
		//	ブランド名抽出
		$b_file = BRAND_FILE;
		if (file_exists($b_file)) {
			$B_LIST = file($b_file);
			foreach ($B_LIST AS $val) {
				list($b_num_, $b_name_, $del_) = explode("<>", $val);
				$B_LINE[$b_num_] = $b_name_;
			}
		}

		$html .= "<form action=\""."/cago.php\" method=\"POST\" id=\"set_cart\">\n";
		$html .= "<input type=\"hidden\" name=\"action\" value=\"add\" />\n";
		$html .= "<input type=\"hidden\" name=\"code\" id=\"goods_code\" />\n";
		$html .= "<input type=\"hidden\" name=\"name\" id=\"goods_name\" />\n";
		$html .= "<input type=\"hidden\" name=\"kakaku\" id=\"goods_kakaku\" />\n";
		$html .= "<input type=\"hidden\" name=\"size\" id=\"goods_size\" />\n";

		// 2022/11/24 レスポンシブ対応につき、コメントアウト uenishi
		// $html .= "<table>\n";
		// $html .= "  <tr class=\"title\">\n";
		// $html .= "    <th rowspan=\"3\" class=\"list10\">画像</th>\n";
		// $html .= "    <th colspan=\"2\" class=\"list20\">商品名</th>\n";
		// $html .= "    <th rowspan=\"3\" class=\"list30\">サイズ<br>購入</th>\n";
		// $html .= "  </tr>\n";
		// $html .= "  <tr class=\"title\">\n";
		// $html .= "    <th colspan=\"2\" class=\"list20\">商品番号</th>\n";
		// $html .= "  </tr>\n";
		// $html .= "  <tr class=\"title\">\n";
		// $html .= "    <th class=\"list40\">ブランド名</th>\n";
		// $html .= "    <th class=\"list40\">価格</th>\n";
		// $html .= "  </tr>\n";

		$html .= "<div class=\"item-list\" >\n";

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
			$GLIST[$g_num]['size_list'] = $list['size_list'];			//	add ookawara 2015/09/15
		}

		$file = "./".DIR_CATE."/".$mc_num."_".$sc_num."_".$s2c_num.".dat";
		if (file_exists($file)) {
			$LIST4 = file($file);
		}

		if ($LIST4) {
			foreach ($LIST4 AS $VAL) {
				$VAL = trim($VAL);
				list($g_num) = explode("<>",$VAL);
				$g_num = preg_replace("/\xEF\xBB\xBF/", "", $g_num);	//thao-debug
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
				$size_list = $GLIST[$g_num]['size_list'];				//	add ookawara 2015/09/15
				if (!$g_num) { continue; }

				//	add ookawara 2015/09/15	start
				$OPTION = array();
				$op_size = "";
				if ($size_list) {
					$op_size = unserialize($size_list);
				}
				if ($op_size) {
					foreach ($op_size AS $key => $VAL) {
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
				foreach ($OPTION AS $val) {
					if (preg_match("/^\*/", $val) || $val == "") { continue; }
					$val = preg_replace("/^\//", "", $val);
					if ($size != "") { $size .= ", "; }
					$val = preg_replace("/\[.*\]/", "", $val);
					$opt .= "<option value=\"".$val."\">$val</option>\n";
					$size .= "$val";
				}
				if (!$opt) { $soldout = 1; }

				$imgf_file = "./".IMAGEF."/".$code.".jpg";
				if (file_exists($imgf_file)) {
					//$IMGF = "<img src=\"/".IMAGEF."/".$code."\" width=\"90\" height=\"105\" border=\"0\" alt=\"".$g_name."\">";	//	del ookawara 2015/10/09
					$IMGF = "<img src=\"/".IMAGEF."/".$code."\" width=\"90\" border=\"0\" alt=\"".$g_name."\">";					//	add ookawara 2015/10/09
				} else {
					$IMGF = "<img src=\"/".IMAGE."/nowprinting90x105.gif\" width=\"90\" height=\"105\" border=\"0\" alt=\"nowprinting\">";
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
					$price_msg = "<s>$price_" . "円(税込み)</s> <font color=\"#ff0000\">".$sale_price_."円(税込み)".$wariritu_msg."</font>";
				} else {
					$price_msg = "$price_"."円(税込み)";
				}

				//	リンク
				$as = "<a href=\""."/".GOODS_SCRIPT."/".$mc_num."/".$sc_num."/".$s2c_num."/".$g_num."/".$index."\" title=\"".$g_name."\nクリックすると詳細が見られます。\">";
				$af = "</a>";

				$amari = $i % 2;
				if ($amari == 0) { $bgcolor = "#ffffff"; } else { $bgcolor = "#e4e995"; }

				//	soldoutチェック
				if ($soldout != 1) {
					$soldout_msg  = "<select name=\"size_".$code."\" id=\"size_".$code."\">\n";
					$soldout_msg .= $opt;
					$soldout_msg .= "</select>\n";
					$soldout_msg .= "<input type=\"button\" value=\"購入\" class=\"submit buy-button\" OnClick=\"set_goods_data('".$code."', '".$g_name."', '".$s_price."'); return false;\">\n";
				} else {
					$soldout_msg  = "<b><font color=\"#ff0000\">SOLD OUT</font></b>\n";
				}

				//	送料無料表示
				$souryou_muryou = "";
				if (free_shipping > 0 && free_shipping <= $souryou_muryou_check_price) {
					$souryou_muryou = "<span class=\"red\">【送料無料】</span>";
				}

				// 2022/11/24 レスポンシブ対応につき、コメントアウト uenishi
				// $html .= "<tr class=\"goods\">\n";
				// $html .= "  <td rowspan=\"3\">".$as.$IMGF."<br />\n[商品詳細]".$af."</td>\n";
				// $html .= "  <td colspan=\"2\">".$souryou_muryou.$g_name."</td>\n";
				// $html .= "  <td rowspan=\"3\">".$soldout_msg."</td>\n";
				// $html .= "</tr>\n";
				// $html .= "<tr class=\"goods\">\n";
				// $html .= "  <td colspan=\"2\">".$code."</td>\n";
				// $html .= "</tr>\n";
				// $html .= "<tr class=\"goods\">\n";
				// $html .= "  <td>".$B_LINE[$brand]."</td>\n";
				// $html .= "  <td>".$price_msg."</td>\n";
				// $html .= "</tr>\n";

				$html .= "<div class=\"item-entry\">\n";
				$html .= "<div class=\"item-entry-details\">\n";
				$html .= "<div class=\"item-entry-pic\">\n";
				$html .= $as . $IMGF . "<br>\n[商品詳細]" . $af . "\n";
				$html .= "</div>\n";
				$html .= "<dl>\n";
				$html .= "<dt>商品名</dt>\n";
				$html .= "<dd>" . $souryou_muryou . $g_name . "</dd>\n";
				$html .= "<dt>商品番号</dt>\n";
				$html .= "<dd>" . $code . "</dd>\n";
				$html .= "<dt>ブランド名</dt>\n";
				$html .= "<dd>" . $B_LINE[$brand] . "</dd>\n";
				$html .= "</dl>\n";
				$html .= "</div>\n";
				$html .= "<div class=\"item-entry-buy\">\n";
				$html .= "<div class=\"item-entry-price\">\n";
				$html .= "$price_msg";
				$html .= "</div>\n";
				$html .= "$soldout_msg";
				$html .= "</div>\n";
				$html .= "</div>\n";
				$i++;
			}
		}

		$html .= "</table>\n";
		$html .= "</form>\n";
		$html .= "</div>\n";
		// $html .= "<br />\n";
	}

	return $html;
}
?>
