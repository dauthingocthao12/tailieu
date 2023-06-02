<?PHP
/*

	ネイバーズスポーツ　商品詳細

*/
function goods_syousai($VALUE, $CHECK)
{
	global	$waribiki, $TAX_, $b_file, $LOG_DIR, $DISCOUNT_C, $attention, $conn_id,
		$GOODS_DISCOUNT_C, $GOODS_DISCOUNT_CATE, $DISCOUNT_PAR, $DISCOUNT_PAR2;
	global $nouki_days_msg;		//	add ookawara 2015/01/30

	$INPUTS = array();
	$DEL_INPUTS = array();

	$num = (int)preg_replace("/[^0-9]/i", "", $CHECK['g']);

	//	ブランド名抽出
	$B_LINE = get_brand_name($b_file);

	//	商品詳細データー
	//$sql  = "SELECT g_num, g_name, code, price, sale_price, options, caption, brand, comment, soldout".											//	del ookawara 2015/09/24
	$sql  = "SELECT g_num, coalesce(name_head, '') || coalesce(g_name, '') ||  coalesce(name_foot, '') AS g_name, code, price, sale_price, options, caption, brand, comment, soldout" .		//	add ookawara 2015/09/24
		", size_list" .					//	add ookawara 2015/09/15
		" FROM " . T_GOODS .
		" WHERE g_num='" . $num . "'" .
		" LIMIT 1;";
	#	echo $sql;
	if ($result = mysqli_query($conn_id, $sql)) {
		$list = mysqli_fetch_array($result);
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
		$size_list = $list['size_list'];	//	add ookawara 2015/09/15
	}

	if ($g_num) {
		$sql  = "SELECT cate1 FROM " . T_CATE .
			" WHERE g_num='" . $g_num . "'" .
			" AND display='1';";
		if ($result = mysqli_query($conn_id, $sql)) {
			while ($list = mysqli_fetch_array($result)) {
				$cate1 = $list['cate1'];
				$CATE[$cate1] = $cate1;
			}
		}
	}

	//	add ookawara 2015/01/29
	if ($cate1 == 99) {
		//$sent_url = "/".GOODS_SCRIPT."/";		//	del ookawara 2016/01/21
		//header ("Location: $sent_url\n\n");	//	del ookawara 2016/01/21
		header404();							//	add ookawara 2016/01/21
	}

	if ($g_name) {
		//	商品名
		$goods_name = $g_name;

		//	画像設定
		$IMG_L = array();
		$imgf_file = "/" . IMAGEF . "/" . $code . ".jpg";
		if (file_exists("." . $imgf_file)) {
			$IMG_L[] = $imgf_file;
		}

		$imgb_file = "/" . IMAGEB . "/" . $code . ".jpg";
		if (file_exists("." . $imgb_file)) {
			$IMG_L[] = $imgb_file;
		}

		$img03_file = "/" . IMAGE03 . "/" . $code . ".jpg";
		if (file_exists("." . $img03_file)) {
			$IMG_L[] = $img03_file;
		}

		$img04_file = "/" . IMAGE04 . "/" . $code . ".jpg";
		if (file_exists("." . $img04_file)) {
			$IMG_L[] = $img04_file;
		}

		$img05_file = "/" . IMAGE05 . "/" . $code . ".jpg";
		if (file_exists("." . $img05_file)) {
			$IMG_L[] = $img05_file;
		}

		//	add ookawara 2015/09/28
		$img06_file = "/" . IMAGE06 . "/" . $code . ".jpg";
		if (file_exists("." . $img06_file)) {
			$IMG_L[] = $img06_file;
		}
		$img07_file = "/" . IMAGE07 . "/" . $code . ".jpg";
		if (file_exists("." . $img07_file)) {
			$IMG_L[] = $img07_file;
		}
		$img08_file = "/" . IMAGE08 . "/" . $code . ".jpg";
		if (file_exists("." . $img08_file)) {
			$IMG_L[] = $img08_file;
		}
		$img09_file = "/" . IMAGE09 . "/" . $code . ".jpg";
		if (file_exists("." . $img09_file)) {
			$IMG_L[] = $img09_file;
		}

		$img_flg = 0;
		$image_00 = "";
		$image_01 = "";
		$image_02 = "";
		$image_03 = "";
		$image_04 = "";
		$image_05 = "";

		//	add ookawara 2015/09/28
		$image_06 = "";
		$image_07 = "";
		$image_08 = "";
		$image_09 = "";


		if (count($IMG_L) > 0) {
			foreach ($IMG_L as $key => $file_name) {

				// add uenishi 2022/12/22
				$key += 1;
				$set_image = "image_0".($key - 1);
				$$set_image   = "<div>\n";
				$$set_image  .= "	<img src=\"".$file_name."\" class=\"img-responsive modal\" alt=\"".$goods_name."\" />\n";
				$$set_image  .= "</div>\n";

			}
		}
		else {
			$image_00  = "<div>\n";
			$image_00 .= "   <img src=\"/" . IMAGE . "/FILLER.jpg\" class=\"img-responsive noimage\" width=\"300\" alt=\"FUTBOLJERSEY.com\" />";
			$image_00 .= "</div>\n";
		}

		//	価格
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
					if ($CATE[$VAL]) {
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
			if ($wariritu) {
				$price_msg .= "<span class=\"font-size important\">" . $wariritu . "%割引</span>\n";
			}
			$price_msg .= "<div class=\"box-column flex-item-gap\">\n";
			$price_msg .= "<span class=\"bold line-through\">" . $price_ . "円(税込み)</span>\n";
			$price_msg .= "<span class=\"font-size important\">" . $sale_price_ . "円(税込み)</span>\n";
			$price_msg .= "</div>\n";
		} else {
			$price_msg .= "<span class=\"font-size important\">" . $price_ . "円(税込み)</span>\n";
		}

		//	サイズ
		//	add ookawara 2015/09/15	start
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

		$option = "";
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
			$option .= "<option value=\"" . $val . "\">" . $val . "</option>\n";
			$size .= $val;
		}

		if (!$option) {
			$soldout = 1;
		}

		//	送料無料表示
		$souryou_muryou = "";
		if (free_shipping > 0 && free_shipping <= $souryou_muryou_check_price) {
			$souryou_muryou = "<span class=\"red\">【送料無料】</span><br />\n";
		}

		//	ブランド
		$brand_msg = "";
		$brand_msg = $B_LINE[$brand];

		//	品番
		$code_msg = "";
		$code_msg = $code;

		//	商品説明
		$att_flg = 0;																	//	add ookawara 2015/01/30
		if ($waribiki || $DISCOUNT_C == 1) {
			$comment = preg_replace("/([0-9])+\%OFF/", "", $comment);
		}
		if (preg_match("/<!-- Attention -->/", $caption)) {
			//$caption = preg_replace("/\<\!-- Attention --\>/", $attention, $caption);	//	del ookawara 2015/01/30
			$caption = preg_replace("/\<\!-- Attention --\>/", "", $caption);			//	add ookawara 2015/01/30
			$caption = trim($caption);
			$att_flg = 1;																//	add ookawara 2015/01/30
		}

		//	動画埋め込み
		//	add ookawara 2014/10/10
		preg_match_all("|<!-- MOVIE=(.*) -->|U", $caption, $MOVIE);
		if ($MOVIE) {
			include_once(INCLUDE_DIR . "movie_list.php");

			foreach ($MOVIE[1] as $key => $VAL) {

				$movie_name = $MOVIE[0][$key];
				$movie_name = change_en($movie_name);

				$movie_num = trim($VAL);
				$movie_code = "";
				$movie_code = $MOVIE_LIST[$movie_num]['R'];
				if ($movie_code) {
					$movie_code = "<br />\n" . $movie_code . "\n";
				}

				$caption = preg_replace("/{$movie_name}/", $movie_code, $caption);
			}
		}

		$setsumei = "";
		if ($caption || $comment) {
			$caption = nl2br($caption);
			$comment = nl2br($comment);
			$br = "";
			if ($caption != "") {
				$br = "<br />";
			}
			$setsumei .= $caption . $br;
			$setsumei .= "<span class=\"red\">\n";
			$setsumei .= $comment;
			$setsumei .= "</span>\n";
		}

		//	篭URL
		$kago_url = "/cago.php";

		//	販売価格
		$sale_price_msg = 0;
		$sale_price_msg = $s_price;

		//	納期
		//	add ookawara 2015/01/30
		$nouki = "";
		$SOKU_CATE = array(23, 24);	//	特価、限定カテゴリー設定
		$soku_cate_flg = 0;
		foreach ($SOKU_CATE as $val) {
			if (array_search($val, $CATE) !== false) {
				$soku_cate_flg = 1;
				break;
			}
		}
		if ($soldout == 1) {
			$nouki = "--";
		} elseif ($soku_cate_flg == 1) {
			//	特価商品、限定入荷一点限りのカテゴリーの商品は即納
			$nouki = $nouki_days_msg;
		} elseif ($att_flg == 1) {
			//	商品説明にAttentionが入っていた場合
			$nouki = $attention;
		} else {
			$nouki = $nouki_days_msg;
		}

		$INPUTS['GOODSNAME'] = $goods_name;
		$INPUTS['IMAGE00'] = $image_00;
		$INPUTS['IMAGE01'] = $image_01;
		$INPUTS['IMAGE02'] = $image_02;
		$INPUTS['IMAGE03'] = $image_03;
		$INPUTS['IMAGE04'] = $image_04;
		$INPUTS['IMAGE05'] = $image_05;

		//	add ookawara 2015/09/28
		$INPUTS['IMAGE06'] = $image_06;
		$INPUTS['IMAGE07'] = $image_07;
		$INPUTS['IMAGE08'] = $image_08;
		$INPUTS['IMAGE09'] = $image_09;

		$INPUTS['PRICE'] = $price_msg;
		$INPUTS['OPTION'] = $option;
		$INPUTS['SOURYOUMURYOU'] = $souryou_muryou;
		$INPUTS['BRAND'] = $brand_msg;
		$INPUTS['CODE'] = $code_msg;
		$INPUTS['SIZE'] = $size;
		$INPUTS['SETSUMEI'] = $setsumei;
		$INPUTS['KAGOURL'] = $kago_url;
		$INPUTS['SPRICE'] = $sale_price_msg;
		$INPUTS['NOUKI'] = $nouki;						//	add ookawara 2015/01/30

		if ($soldout == 1) {
			$DEL_INPUTS['NOTSOLDOUT'] = 1;
		} else {
			$DEL_INPUTS['SOLDOUT'] = 1;
		}

		//	html作成・置換
		$make_html = new read_html();
		$make_html->set_dir(TEMPLATE_DIR);
		$make_html->set_file("goods_syousai.htm");
		$make_html->set_rep_cmd($INPUTS);
		$make_html->set_del_cmd($DEL_INPUTS);
		$html = $make_html->replace();
	}

	if (!$html || !$cate1) {
		//$sent_url = "/".GOODS_SCRIPT."/";		//	del ookawara 2016/01/21
		//header ("Location: $sent_url\n\n");	//	del ookawara 2016/01/21
		header404();							//	add ookawara 2016/01/21
	}

	//	add ookawara 2016/12/02
	//	description
	$description = "";
	if ($setsumei) {
		$description = strip_tags($brand_msg . " " . $setsumei);
		$description = preg_replace("/\n|\r|\r\n/", "", $description);
		$description = mb_strimwidth($description, 0, 100, "...");
	}

	//return $html;						//	del ookawara 2016/12/02
	return array($html, $description);	//	add ookawara 2016/12/02

}


//	[]()変換
//	add ookawara 2014/10/10
function change_en($text)
{

	##	$text = ereg_replace("\[","\\[",$text);
	$text = preg_replace("/\[/", "\\[", $text);
	##	$text = ereg_replace("\{","\\{",$text);	//	add ookawara 2009/10/02
	$text = preg_replace("/\{/", "\\{", $text);
	##	$text = ereg_replace("\(","\\(",$text);
	$text = preg_replace("/\(/", "\\(", $text);
	##	$text = ereg_replace("\)","\\)",$text);
	$text = preg_replace("/\)/", "\\)", $text);
	##	$text = ereg_replace("\}","\\}",$text);	//	add ookawara 2009/10/02
	$text = preg_replace("/\}/", "\\}", $text);
	##	$text = ereg_replace("\]","\\]",$text);
	$text = preg_replace("/\]/", "\\]", $text);
	##	$text = ereg_replace("\*","\\*",$text);
	$text = preg_replace("/\*/", "\\*", $text);
	##	$text = ereg_replace("\!","\\!",$text);
	$text = preg_replace("/\!/", "\\!", $text);
	return $text;
}
