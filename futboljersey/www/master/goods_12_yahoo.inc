<?php
//	Yahoo用CSV作成ファイル




function make_csv_yahoo( &$msg = null,&$ERROR = null) {
    global $conn_id;

	//	YAHOOデーター作成
	//	商品データー読み込み
	unset($y_csv);
	unset($new_y_csv);
	unset($new_yz_csv);	//	add ookawara 2014/07/31
	unset($YAHOO_LIST);
	unset($YAHOO_GOODS_LIST);
	$sql = "SELECT i.num, i.cate1, i.cate2, i.cate3, i.g_num, i.ypath, i.display, i.copy_num, i.state," .
			//" j.g_name, j.code, j.price, j.sale_price, j.options, j.caption, j.brand, j.headline," .	//	del ookawara 2015/09/24
			" coalesce(j.name_head, '') || coalesce(j.g_name, '') ||  coalesce(j.name_foot, '') AS g_name,".	//	add ookawara 2015/09/24
			" j.code, j.price, j.sale_price, j.options, j.caption, j.brand, j.headline," .	//	add ookawara 2015/09/24
			" j.yd_id,".
			//" j.zaiko, j.size_list, j.name_head".			//	add ookawara 2015/09/17	//	del ookawara 2015/09/24
			" j.zaiko, j.size_list".						//	add ookawara 2015/09/24
			" FROM ".T_CATEGORY." i, ".T_GOODS." j" .
			" WHERE i.g_num=j.g_num AND j.yahooshop='1' AND j.soldout!='1' AND i.display='1'" .
			" AND i.cate1>='20' AND i.state<'3'".
			" AND i.cate1 NOT IN ('98','99')".	//	add ookawara 2013/08/23
			" ORDER BY i.cate1, i.cate2, i.cate3 DESC;";
	if ($result = mysqli_query($conn_id,$sql)) {
		while ($list = mysqli_fetch_array($result)) {
			$num_ = $list['num'];
			$g_num_ = $list['g_num'];
			$YAHOO_LIST[$num_] = $list;
			$YAHOO_GOODS_LIST[$g_num_][] = $num_;

		}
	}

	if ($YAHOO_LIST) {
		foreach ($YAHOO_LIST AS $num_ => $list) {
			$g_num_ = $list['g_num'];
			if ($CHECK_GOODS_NUM[$g_num_]) { continue; }

			//	カテゴリー作成
			unset($category_list);
			foreach ($YAHOO_GOODS_LIST[$g_num_] AS $key => $num_) {
				$category_list .= make_category_list($num_,$YAHOO_LIST,$YAHOO_GOODS_LIST,$L_CATE);
			}
			$category_list = trim($category_list);

			//	change ookawara 2010/05/25
			$set_csv = "";
			$set_zaiko_csv = "";	//	add ookawara 2014/07/31
			//$set_csv = new_yahoo_csv($list,$L_CATE,$L_BRAND,$L_NEW_YPATH,$L_Y_BEAND,$category_list,$SPEC_ID_LIST, $UP_PIC_LIST);	//	add $UP_PIC_LIST ookawara 2014/02/06	//	del ookawara 2014/07/31
			list($set_csv, $set_zaiko_csv) = new_yahoo_csv($list,$L_CATE,$L_BRAND,$L_NEW_YPATH,$L_Y_BEAND,$category_list,$SPEC_ID_LIST, $UP_PIC_LIST);	//	add ookawara 2014/07/31
			if ($set_csv) {
				$new_y_csv .= $set_csv;
				unset($YAHOO_GOODS_LIST[$g_num_]);
				$CHECK_GOODS_NUM[$g_num_] = 1;
			}
			//	add ookawara 2014/07/31
			if ($set_zaiko_csv) {
				$new_yz_csv .= $set_zaiko_csv;
			}
		}
	}
	if ($new_y_csv) {
		//	add cal ohkawara 2017/04/10	lead-time-instock,lead-time-outstock
		$new_y_csv = "\"path\",\"name\",\"code\",\"sub-code\",\"original-price\",\"price\",\"sale-price\",\"options\",\"headline\",\"caption\",\"abstract\",\"explanation\",\"additional1\",\"additional2\",\"additional3\",\"relevant-links\",\"ship-weight\",\"taxable\",\"release-date\",\"temporary-point-term\",\"meta-key\",\"meta-desc\",\"display\",\"template\",\"sale-period-start\",\"sale-period-end\",\"sale-limit\",\"sp-code\",\"brand-code\",\"person-code\",\"yahoo-product-code\",\"product-code\",\"jan\",\"isbn\",\"delivery\",\"product-category\",\"spec1\",\"spec2\",\"spec3\",\"spec4\",\"spec5\",\"sp-additional\",\"lead-time-instock\",\"lead-time-outstock\"\r\n$new_y_csv";
		$new_y_csv = mb_convert_encoding($new_y_csv,"sjis-win","UTF-8");			//	ohkawara	2022/10/27
		$OUT = fopen(NEW_YAHOO_ALL_FILE,"w");
		fwrite($OUT,$new_y_csv);
		fclose($OUT);
		@chmod(NEW_YAHOO_ALL_FILE,0666);
		$msg = "新YahooCSVファイル作成完了いたしました。<br>\n<br>\n";
	} elseif (file_exists(NEW_YAHOO_ALL_FILE)) {
		unlink(NEW_YAHOO_ALL_FILE);
	}

	//	Yahoo在庫
	//	add ookawara 2014/07/31
	if ($new_yz_csv) {
		$new_yz_csv = "\"code\",\"sub-code\",\"quantity\",\"mode\"\r\n$new_yz_csv";
		$new_yz_csv = mb_convert_encoding($new_yz_csv,"sjis-win","UTF-8");			//	ohkawara	2022/10/24
		$OUT = fopen(NEW_YAHOO_ZAIKO_FILE,"w");
		fwrite($OUT,$new_yz_csv);
		fclose($OUT);
		@chmod(NEW_YAHOO_ZAIKO_FILE,0666);
		$msg .= "新Yahoo在庫CSVファイル作成完了いたしました。<br>\n<br>\n";
	} elseif (file_exists(NEW_YAHOO_ZAIKO_FILE)) {
		unlink(NEW_YAHOO_ZAIKO_FILE);
	}

	if ($y_csv) {
		$y_csv = "Path,Name,Code,Price,Sale-Price,Options,Headline,Caption,Abstract,Relevant-links,Ypath,Ship-weight,TAXABLE\r\n$y_csv";
		$y_csv = mb_convert_encoding($y_csv,"sjis-win","UTF-8");					//	ohkawara	2022/10/24
		$OUT = fopen(YAHOO_ALL_FILE,"w");
		fwrite($OUT,$y_csv);
		fclose($OUT);
		@chmod(YAHOO_ALL_FILE,0666);
		$msg .= "旧YahooCSVファイル作成完了いたしました。<br>\n<br>\n";
	} elseif (file_exists(YAHOO_ALL_FILE)) {
		unlink(YAHOO_ALL_FILE);
	}

	return array($msg,$ERROR);

}


//-------------------------------------------------------------------------------------
//
//	Yahoo
//
//-------------------------------------------------------------------------------------

//	新yahoo用CSV
function new_yahoo_csv($list,$L_CATE,$L_BRAND,$L_YPATH,$L_Y_BEAND,$category_list,$SPEC_ID_LIST, $UP_PIC_LIST) {	//	add $UP_PIC_LIST ookawara 2014/02/06
	global $attention,$NEWCATE,$NEW_YAHOO_GOODS_DISCOUNT_CATE;
	global $MOVIE_LIST;	//	add ookawara 2014/10/10
	global $nouki_days_msg;		//	add ookawara 2015/01/30

	$num = $list['num'];
	$cate1 = $list['cate1'];
	$cate2 = $list['cate2'];
	$cate3 = $list['cate3'];
	$g_num = $list['g_num'];
	$ypath = $list['ypath'];
	$display = $list['display'];
	$copy_num = $list['copy_num'];
	$state = $list['state'];
	$g_name = $list['g_name'];
	$code = $list['code'];
	$base_price = $list['price'];
	$base_sale_price = $list['sale_price'];
	$options = $list['options'];
	$caption = $list['caption'];
	$brand = $list['brand'];
	$headline = $list['headline'];
	$yd_id = $list['yd_id'];
	$zaiko = $list['zaiko'];			//	add ookawara 2015/09/17
	$size_list = $list['size_list'];	//	add ookawara 2015/09/17
	//$name_head = $list['name_head'];	//	add ookawara 2015/09/17	//	del ookawara 2015/09/24

	//	price,sale_price
	//if (TAXABLE == 1) {	//	del ookawara 2014/03/10
		//	割引設定項目
		$GOODS_DISCOUNT_C = NEW_YAHOO_GOODS_DISCOUNT_C;
		$GOODS_DISCOUNT_CATE = $NEW_YAHOO_GOODS_DISCOUNT_CATE;
		$DISCOUNT_PAR = NEW_YAHOO_DISCOUNT_PAR;
		$DISCOUNT_PAR2 = NEW_YAHOO_DISCOUNT_PAR2;

		if ($base_price > 0 && $base_sale_price > 0 && $base_price < $base_sale_price) {
			$sale_price = "";
			$price = $base_price;
		} elseif ($base_price > 0 && $base_sale_price > 0 && $base_price == $base_sale_price) {
			if ($GOODS_DISCOUNT_C == 1 && $DISCOUNT_PAR2 > 0) {
				$flag = 0;
				if ($GOODS_DISCOUNT_CATE) {
					foreach ($GOODS_DISCOUNT_CATE AS $val) {
						$val = trim($val);
						if ($cate1 == $val) { $flag = 1; }
					}
				}
				if ($flag != 1) {
					$sale_price = floor($base_price * ((100 - $DISCOUNT_PAR2) / 100));
				} else {
					$sale_price = "";
				}
			} else {
				$sale_price = "";
			}
			$price = $base_price;
		} elseif ($GOODS_DISCOUNT_C == 1) {
			$flag = 0;
			if ($GOODS_DISCOUNT_CATE) {
				foreach ($GOODS_DISCOUNT_CATE AS $val) {
					$val = trim($val);
					if ($cate1 == $val) { $flag = 1; }
				}
			}
			if ($flag != 1) {
				$sale_price_ = floor($base_price * ((100 - $DISCOUNT_PAR) / 100));
				if ($base_sale_price < 1) {
					$price = $base_price;
					$sale_price = $sale_price_;
				} elseif ($base_sale_price > $sale_price_) {
					$price = $base_sale_price;
					$sale_price = $sale_price_;
				} else {
					$price = $base_price;
					$sale_price = $base_sale_price;
				}
			} else {
				if ($base_sale_price) {
					$price = $base_sale_price;
					$sale_price = "";
				} else {
					$price = $base_price;
					$sale_price = "";
				}
			}
		}

		if (TAXABLE == 1) {	//	add ookawara 2014/03/10
			$price = floor($price * ((TAX / 100) + 1) + 0.5);
			$sale_price = floor($sale_price * ((TAX / 100) + 1) + 0.5);
		}	//	add ookawara 2014/03/10
		if ($sale_price < 1) { unset($sale_price); }
/*
	//	del ookawara 2014/03/10
	} else {
		if ($base_price > 0 && $base_sale_price > 0 && $base_price == $base_sale_price) {
			$sale_price = "";
		}
		$price = floor($base_price);
		$sale_price = floor($base_sale_price);
	}
*/

	//	options
	//	add ookawara 2015/09/17	start
	$L_SIZE = array();
	if ($size_list) {
		$op_size = unserialize($size_list);
	}
	$jan_code = "";		//	add ookawara 2016/04/19
	if ($op_size) {
		foreach ($op_size AS $key => $VAL) {

			//	jan_code設定
			//	add ookawara 2016/04/19
			if ($VAL['jan'] != "none" || $VAL['jan'] > 0) {
				if (!preg_match("/[^0-9]/", $VAL['jan'])) {
					if (strlen($VAL['jan']) == 13 && preg_match("/^4/", $VAL['jan'])) {
						$jan_code = $VAL['jan'];
					}
				}
			}

			if ($VAL['type'] == 3) {
				continue;
			}
			$L_SIZE[] = $VAL['size'];
		}
	} else {
	//	add ookawara 2015/09/17	end
		$options = trim($options);
		$L_SIZE = explode("\n",$options);
	}	//	add ookawara 2015/09/17

	$flag = 0;
	$size_check = 0;
	$spec_id = "";
	$options = "";
	$new_pic_check = 0;
	if ($L_SIZE) {
		foreach ($L_SIZE AS $val) {
			$val = trim($val);
			if ($val == "") { continue; }
##			if (ereg("^\*",$val)) {
			if (preg_match("/^\*/",$val)) {
				$size_check = 1;
				continue;
			}
##			if (ereg("^\/",$val)) { $new_pic_check = 1; }
			if (preg_match("/^\//",$val)) { $new_pic_check = 1; }
##			$val = eregi_replace("^\/i","",$val);
			$val = preg_replace("/^\//","",$val);
##			$val = eregi_replace("\[.*\]","",$val);
			$val = preg_replace("/\[.*\]/i","",$val);
			if ($val != "F" || 
				$val != "フリー"  || 
				$val != "フリーサイズ" || 
				$val != "--" || 
				$val != "フットサルサイズ" || 
				$val != "設定なし"
			) {
				$val = trim($val);
				$id = $yd_id;
				$spec_value_id = "";
				if ($SPEC_ID_LIST[$id]) {
					foreach ($SPEC_ID_LIST[$id] AS $key1 => $val1) {
						if ($SPEC_ID_LIST[$id][$key1]) {
							foreach ($SPEC_ID_LIST[$id][$key1] AS $key2 => $val2) {
								$check_value = moji_seiton($val);

								//	add ookawara 2012/03/07
								if (!$check_value) {
									echo $g_num."<br>\n";
									continue;
								}

##								if (ereg($check_value,$val2)) {
								if (preg_match($check_value,$val2)) {
										$spec_value_id = $key2;
										$spec_id = $key1;
										break;
								}
							}
							if ($spec_value_id) { break; }
						}
					}

				}
				$options .= "{$val}|{$spec_value_id}| ";
				$flag = 1;
			}
		}
		$options = trim($options);
##		$options = ereg_replace(":","：",$options);
		$options = preg_replace("/:/","：",$options);
	}

	//	add ookawara 2010/05/25
	if ($size_check == 1 && $options == "") {
		return ;
	}

	if ($flag != 1) {
		unset($options);
	} else {
##		if (ereg("^UBS8260",$code)) {
		if (preg_match("/^UBS8260/",$code)) {
			$options = "カラー|{$spec_id}| ".$options;
		} else {
			$options = "サイズ|{$spec_id}| ".$options;
		}
	}

	//	カテゴリー
	$cate1_name = trim($L_CATE[$cate1][0][0]);
	$cate2_name = trim($L_CATE[$cate1][$cate2][0]);
	$cate3_name = trim($L_CATE[$cate1][$cate2][$cate3]);
	$cate0_name = trim($NEWCATE[$cate1]['name']);

	$meta_key = $cate0_name;
	if ($cate0_name != $cate1_name) {
		$meta_key .= "|".$cate1_name;
	}
	if ($cate1_name != $cate2_name) {
		$meta_key .= "|".$cate2_name;
	}
	if ($cate2_name != $cate3_name) {
		$meta_key .= "|".$cate3_name;
	}
	$meta_key .= "|サッカー";

	//	画像チェック
	$imagef_file = IMG_F_DIR."/$code" . ".jpg";
	$c_imagef_file = UP_IMG_DIR."/$code" . ".jpg";
	//if (file_exists($imagef_file) && !file_exists($c_imagef_file) && (CHECK_TIME < filemtime($imagef_file) || $new_pic_check == 1)) {	//	del ookawara 2014/02/06
	if (file_exists($imagef_file) && !file_exists($c_imagef_file) && (CHECK_TIME < filemtime($imagef_file) || $new_pic_check == 1 || $UP_PIC_LIST[$g_num] != "")) {	//	add ookawara 2014/02/06
		copy($imagef_file,$c_imagef_file);
	}

	$imageb_file = IMG_B_DIR."/$code" . ".jpg";
	$c_imageb_file = UP_IMG_DIR."/$code" . "_1.jpg";
	//if (file_exists($imageb_file) && !file_exists($c_imageb_file) && (CHECK_TIME < filemtime($imageb_file) || $new_pic_check == 1)) {	//	del ookawara 2014/02/06
	if (file_exists($imageb_file) && !file_exists($c_imageb_file) && (CHECK_TIME < filemtime($imageb_file) || $new_pic_check == 1 || $UP_PIC_LIST[$g_num] != "")) {	//	add ookawara 2014/02/06
		copy($imageb_file,$c_imageb_file);
	}

	//	add ookawara 2013/07/03
	$image03_file = IMG_03_DIR."/$code" . ".jpg";
	$c_image03_file = UP_IMG_DIR."/$code" . "_2.jpg";
	//if (file_exists($image03_file) && !file_exists($c_image03_file) && (CHECK_TIME < filemtime($image03_file) || $new_pic_check == 1)) {	//	del ookawara 2014/02/06
	if (file_exists($image03_file) && !file_exists($c_image03_file) && (CHECK_TIME < filemtime($image03_file) || $new_pic_check == 1 || $UP_PIC_LIST[$g_num] != "")) {	//	add ookawara 2014/02/06
		copy($image03_file,$c_image03_file);
	}

	$image04_file = IMG_04_DIR."/$code" . ".jpg";
	$c_image04_file = UP_IMG_DIR."/$code" . "_3.jpg";
	//if (file_exists($image04_file) && !file_exists($c_image04_file) && (CHECK_TIME < filemtime($image04_file) || $new_pic_check == 1)) {	//	del ookawara 2014/02/06
	if (file_exists($image04_file) && !file_exists($c_image04_file) && (CHECK_TIME < filemtime($image04_file) || $new_pic_check == 1 || $UP_PIC_LIST[$g_num] != "")) {	//	add ookawara 2014/02/06
		copy($image04_file,$c_image04_file);
	}

	$image05_file = IMG_05_DIR."/$code" . ".jpg";
	$c_image05_file = UP_IMG_DIR."/$code" . "_4.jpg";
	//if (file_exists($image05_file) && !file_exists($c_image05_file) && (CHECK_TIME < filemtime($image05_file) || $new_pic_check == 1)) {	//	del ookawara 2014/02/06
	if (file_exists($image05_file) && !file_exists($c_image05_file) && (CHECK_TIME < filemtime($image05_file) || $new_pic_check == 1 || $UP_PIC_LIST[$g_num] != "")) {	//	add ookawara 2014/02/06
		copy($image05_file,$c_image05_file);
	}

	//	add ookawara 2015/09/27
	$image06_file = IMG_06_DIR."/$code" . ".jpg";
	$c_image06_file = UP_IMG_DIR."/$code" . "_5.jpg";
	//if (file_exists($image06_file) && !file_exists($c_image06_file) && (CHECK_TIME < filemtime($image06_file) || $new_pic_check == 1)) {	//	del ookawara 2014/02/06
	if (file_exists($image06_file) && !file_exists($c_image06_file) && (CHECK_TIME < filemtime($image06_file) || $new_pic_check == 1 || $UP_PIC_LIST[$g_num] != "")) {
		copy($image06_file,$c_image06_file);
	}


	//	caption
	$att_flg = 0;	//	add ookawara 2015/01/30
	$check = 0;
##	$caption = eregi_replace("<br>","\n",$caption);
    $caption = preg_replace("/<br>/i","\n",$caption);
##	$caption = eregi_replace("<br />","\n",$caption);
	$caption = preg_replace("/<br \/>/i","\n",$caption);
##	$caption = ereg_replace("\r","",$caption);
	$caption = preg_replace("/\r/","",$caption);
##	$caption  = ereg_replace("&","＆",$caption);			//	add ookawara 2013/04/11
    $caption  = preg_replace("/&/","＆",$caption);			//	add ookawara 2013/04/11
##	$caption  = ereg_replace("<","&lt;",$caption);			//	add ookawara 2013/04/11
	$caption  = preg_replace("/</","&lt;",$caption);			//	add ookawara 2013/04/11
##	$caption  = ereg_replace(">","&gt;",$caption);			//	add ookawara 2013/04/11
    $caption  = preg_replace("/>/","&gt;",$caption);			//	add ookawara 2013/04/11


##	if (ereg("&lt;!-- Attention --&gt;",$caption)) {		//	add ookawara 2013/09/13
    if (preg_match("/&lt;!-- Attention --&gt;/",$caption)) {		//	add ookawara 2013/09/13
//	2011/07/12	コメント表示希望依頼が来た為表示するように変更（元に戻す）
##		$caption = ereg_replace("&lt;!-- Attention --&gt;", "", $caption);	//	add ookawara 2015/01/30
		$caption = preg_replace("/&lt;!-- Attention --&gt;/", "", $caption);	//	add ookawara 2015/01/30
##		//$caption = ereg_replace("&lt;!-- Attention --&gt;","$attention",$caption);	//	add ookawara 2013/09/13	//	del ookawara 2015/01/30
		//$caption = preg_replace("/&lt;!-- Attention --&gt;/","$attention",$caption);	//	add ookawara 2013/09/13	//	del ookawara 2015/01/30
##		//$caption = ereg_replace("<!-- Attention -->","$attention",$caption);		//	del ookawara 2013/09/13
		//$caption = preg_replace("/<!-- Attention -->/","$attention",$caption);		//	del ookawara 2013/09/13
##		//$caption = ereg_replace("<!-- Attention -->","",$caption);
		//$caption = preg_replace("/<!-- Attention -->/","",$caption);
		$caption = trim($caption);
		$check = 1;
		$att_flg = 1;	//	add ookawara 2015/01/30
	}
	$L_CAPTION = explode("\n",$caption);

	if ($L_CAPTION) {
		$flag = 0;
		foreach ($L_CAPTION AS $val) {
			$val = trim($val);
			if ($flag != 1 && !$val) {
				$flag = 1;
				continue;
			}
			if ($flag == 0) {
				$explanation .= $val." ";
			} else {
				$caption_msg .= $val."<br />";
			}
		}
	}

	$caption = <<<WAKABA
$attation
ブランド名：$L_BRAND[$brand]<br />
$caption_msg
WAKABA;
##	$caption = ereg_replace("\r","",$caption);
    $caption = preg_replace("/\r/","",$caption);
##	$caption = ereg_replace("\n","",$caption);
	$caption = preg_replace("/\n/","",$caption);


	//	設定
	$path = $category_list;
	$name = $g_name;				//	del ookawara 2015/09/17		//	add ookawara 2015/09/24
	//$name = $name_head.$g_name;		//	add ookawara 2015/09/17	//	del ookawara 2015/09/24
	$code = $code;

	if (TAXABLE == 1) {
		$original_price = floor($base_price * ((TAX / 100) + 1) + 0.5);
	} else {
		$original_price = $base_price;
	}
	$price = $price;
	$sale_price = $sale_price;
	if ($sale_price < 1) { $sale_price = ""; }
	if ($sale_price > 0) { $check_price = $sale_price; } else { $check_price = $price; }
	$wariritu = wariritu($original_price,$check_price);
	$headline = "";
	if ($wariritu && strlen($headline) < 42) {
		$headline .= "★".mb_convert_kana($wariritu,"N")."％ＯＦＦ★";	//	add ookawara 2013/12/09
		//$headline .= "【".mb_convert_kana($wariritu,"N")."％ＯＦＦ】";	//	del ookawara 2013/12/09
		if (defined(YAHOO_DISCOUNT_TIME) && $wariritu <= NEW_YAHOO_DISCOUNT_PAR) {
			$headline .= "【".YAHOO_DISCOUNT_TIME."】";
		}
	}

	if ($sale_price && defined("sale_period_start")) {
		$sale_period_start = sale_period_start;
	} else {
		$sale_period_start = "";
	}
	if ($sale_price && defined("sale_period_end")) {
		$sale_period_end = sale_period_end;
	} else {
		$sale_period_end = "";
	}
	$options = $options;

	$caption = $caption;
	$explanation = $explanation;

	$meta_key = $meta_key;
	$meta_desc = $g_name;	//	add ookawara 2013/04/11

	$yahoo_product_code = "";
	$product_code = "";

	$product_category = $yd_id;

	$brand_code = $L_Y_BEAND[$brand];
	$headline = trim($headline);
	$taxable = "1";
	$display = "1";

	$abstract = "";
	$sub_code = "";
	$additional1 = "";
	$additional2 = "";
	$additional3 = "";
	$relevant_links = "";
	$ship_weight = "";
	$release_date = "";
	$temporary_point_term = "";
	$template = "";
	$sale_limit = "";
	$sp_code = "";
	$person_code = "";
	//$jan = "";		//	del ookawara 2016/04/19
	$jan = $jan_code;	//	add ookawara 2016/04/19
	$isbn = "";
	$delivery = "0";
	$spec1 = $spec2 = $spec3 = $spec4 = $spec5 = "";

	//	期間対策
	if ($price && $sale_price) {
		$price = $sale_price;
		$sale_price = "";
	}
	$sale_period_start = "";
	$sale_period_end = "";

	//	金額確認送料無料コメントセット
	//	add ookawara 2013/12/09
	if (
		($sale_price >= YAHOO_SOURYOU_PRICE)
		|| (!$sale_price && $price >= YAHOO_SOURYOU_PRICE)
	) {
		$headline = YAHOO_SOURYOU_MESSAGE.$headline;
		$delivery = "1";	//	add ookawara 2014/05/02

		//	在庫1商品表記	//	add ookawara 2014/09/22
		//if (preg_match("/^特価商品:/", $path)) {												//	del ookawara 2014/12/26
		if (preg_match("/^特価商品:/", $path) || preg_match("/^限定入荷一点限り:/", $path)) {	//	add ookawara 2014/12/26
			$headline = YAHOO_ZIKO1_MESSAGE.$headline;
		}

	} else {
		//	在庫1商品表記	//	add ookawara 2014/09/22
		//if (preg_match("/^特価商品:/", $path)) {												//	del ookawara 2014/12/26
		if (preg_match("/特価商品:/", $path) || preg_match("/限定入荷一点限り:/", $path)) {	//	add ookawara 2014/12/26
			$headline = YAHOO_ZIKO1_MESSAGE.$headline;
		} else {
			$headline = YAHOO_ATO_SOURYOU_MESSAGE.$headline;
		}
		//$headline = YAHOO_ATO_SOURYOU_MESSAGE.$headline;	//	del ookawara 2014/09/22
	}

/*
	//	del ookawara 2013/12/09
	if ($sale_price >= YAHOO_SOURYOU_PRICE) {
		$headline = YAHOO_SOURYOU_MESSAGE.$headline;
	} elseif (!$sale_price && $price >= YAHOO_SOURYOU_PRICE) {
		$headline = YAHOO_SOURYOU_MESSAGE.$headline;
	}
*/
	$path  = htmlspecialchars($path);
##	$meta_key  = ereg_replace("&","＆",$meta_key);
	$meta_key  = preg_replace("/&/","＆",$meta_key);
##	$meta_desc  = ereg_replace("&","＆",$meta_desc);
	$meta_desc  = preg_replace("/&/","＆",$meta_desc);
##	$name  = ereg_replace("&","＆",$name);						//	add ookawara 2013/04/11
	$name  = preg_replace("/&/","＆",$name);						//	add ookawara 2013/04/11
##	$name  = ereg_replace("<","&lt;",$name);					//	add ookawara 2013/04/11
	$name  = preg_replace("/</","&lt;",$name);					//	add ookawara 2013/04/11
##	$name  = ereg_replace(">","&gt;",$name);					//	add ookawara 2013/04/11
	$name  = preg_replace("/>/","&gt;",$name);					//	add ookawara 2013/04/11
	$name  = preg_replace("/\,/","、",$name);					//	add ookawara 2013/05/22
##	$explanation  = ereg_replace("&","＆",$explanation);		//	add ookawara 2013/04/11
	$explanation  = preg_replace("/&/","＆",$explanation);		//	add ookawara 2013/04/11
##	$explanation  = ereg_replace("<","&lt;",$explanation);	//	add ookawara 2013/04/11
	$explanation  = preg_replace("/</","&lt;",$explanation);	//	add ookawara 2013/04/11
##	$explanation  = ereg_replace(">","&gt;",$explanation);	//	add ookawara 2013/04/11
	$explanation  = preg_replace("/>/","&gt;",$explanation);	//	add ookawara 2013/04/11

	//	動画埋め込み
	//	add ookawara 2014/10/10
	preg_match_all("|＆lt;!-- MOVIE=(.*) --＆gt;|U", $explanation, $MOVIE);
	if ($MOVIE) {
		foreach ($MOVIE[1] AS $key => $VAL) {
			$movie_name = $MOVIE[0][$key];
			$movie_name = change_en($movie_name);

			$movie_num = trim($VAL);
			$movie_code = "";
			$movie_code = $MOVIE_LIST[$movie_num]['Y'];
			if ($movie_code) {
				$movie_code = $movie_code;
			}

			$explanation = preg_replace("/{$movie_name}/", "", $explanation);
			$caption = $movie_code.$caption;
		}
	}

#	$meta_key = mb_substr($meta_key, 0, 160, 'eucJP-win');	//	add ookawara 2013/04/11
	$meta_key = mb_substr($meta_key, 0, 160, 'UTF-8');
#	$meta_desc = mb_substr($meta_desc, 0, 160, 'eucJP-win');	//	add ookawara 2013/04/11
	$meta_desc = mb_substr($meta_desc, 0, 160, 'UTF-8');
	//	add ookawara 2014/02/19
//	if ($original_price < $price) {
//		$price = $original_price;
//	}


	//	商品情報納期表示
	//	add	ookawara 2015/01/30
	$caption .= "<b>納期：</b>";
	$lead_time_instock = 2000;		//	add ookawara 2017/04/10
	$lead_time_outstock = 4000;		//	add ookawara 2017/04/10
	if (preg_match("/特価商品:/", $path) || preg_match("/限定入荷一点限り:/", $path)) {
		//	特価商品、限定入荷一点限りのカテゴリーの商品は即納
		$caption .= $nouki_days_msg;
		$lead_time_outstock = "";	//	add ookawara 2017/04/10
	} elseif ($att_flg == 1) {
		//	商品説明にAttentionが入っていた場合
		$caption .= $attention;
		$lead_time_instock = 1;		//	add ookawara 2017/04/10
		$lead_time_outstock = "";	//	add ookawara 2017/04/10
	} else {
		$caption .= $nouki_days_msg;
	}

	//	add cal ohkawara lead-time-instock,lead-time-outstock
	$csv = "\"$path\",\"$name\",\"$code\",\"$sub_code\",\"$original_price\",\"$price\",\"$sale_price\",\"$options\",\"$headline\",\"$caption\",\"$abstract\",\"$explanation\",\"$additional1\",\"$additional2\",\"$additional3\",\"$relevant_links\",\"$ship_weight\",\"$taxable\",\"$release_date\",\"$temporary_point_term\",\"$meta_key\",\"$meta_desc\",\"$display\",\"$template\",\"$sale_period_start\",\"$sale_period_end\",\"$sale_limit\",\"$sp_code\",\"$brand_code\",\"$person_code\",\"$yahoo_product_code\",\"$product_code\",\"$jan\",\"$isbn\",\"$delivery\",\"$product_category\",\"$spec1\",\"$spec2\",\"$spec3\",\"$spec4\",\"$spec5\",\"$caption\",\"$lead_time_instock\",\"$lead_time_outstock\"\r\n";

	//	在庫csv
	//	add ookawara 2014/07/31
	//if (preg_match("/^特価商品:/", $path)) {												//	del ookawara 2014/12/24
	//if (preg_match("/特価商品:/", $path) || preg_match("/限定入荷一点限り:/", $path)) {	//	add ookawara 2014/12/24	//	del ookawara 2015/09/17
	if (preg_match("/特価商品:/", $path) || preg_match("/限定入荷一点限り:/", $path) || $zaiko == 1) {	//	add ookawara 2015/09/17
		$csv_zaiko = "\"$code\",\"$sub_code\",\"1\",\"\"\r\n";
	} else {																			//	add ohkawara 2017/04/10
		$csv_zaiko = "\"$code\",\"$sub_code\",\"".YAHOO_DEFO_ZAIKO."\",\"\"\r\n";		//	add ohkawara 2017/04/10
	}

	//return $csv;
	return array($csv, $csv_zaiko);

}

//	YAHOO	カテゴリー作成
function make_category_list($num,$YAHOO_LIST,$YAHOO_GOODS_LIST,$L_CATE) {
global $NEWCATE;

	$cate1 = $YAHOO_LIST[$num]['cate1'];
	$cate2 = $YAHOO_LIST[$num]['cate2'];
	$cate3 = $YAHOO_LIST[$num]['cate3'];

	$cate1_name = trim($L_CATE[$cate1][0][0]);
	$cate2_name = trim($L_CATE[$cate1][$cate2][0]);
	$cate3_name = trim($L_CATE[$cate1][$cate2][$cate3]);
	$cate0_name = trim($NEWCATE[$cate1]['name']);

	$category_list = $cate0_name;
	if ($cate0_name != $cate1_name) {
		$category_list .= ":".$cate1_name;
	}
	if ($cate1_name != $cate2_name) {
		$category_list .= ":".$cate2_name;
	}
	if ($cate2_name != $cate3_name) {
		$category_list .= ":".$cate3_name;
	}

	return $category_list."\r\n";

}


//	YAHOO	カテゴリー作成
function moji_seiton($val) {

#	$val = mb_convert_kana($val,"asKV","EUC-JP");
	$val = mb_convert_kana($val,"asKV","UTF-8");
##	$val = eregi_replace("センチ","cm",$val);
	$val = preg_replace("/センチ/i","cm",$val);

	return $val;
}












