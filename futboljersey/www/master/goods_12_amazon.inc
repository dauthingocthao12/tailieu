<?php
//	Amazon用CSV作成ファイル

function make_csv_amazon(&$msg = null,&$ERROR = null) {

	//	Amazon処理-----------------------------------------------------------------------------------------
	//	add ookawara 2015/07/27
	//	アップ済み出品レポートファイル取得
	$AMZ_UP_LIST = array();
	if (file_exists(AMAZON_B_ITEM_FILE)) {
		$LIST = file(AMAZON_B_ITEM_FILE);
#		mb_convert_variables("eucjp-win", "sjis-win", $LIST);
		mb_convert_variables("UTF-8", "sjis-win", $LIST);
		foreach ($LIST AS $key => $val) {
			if ($key < 1) { continue; }
			list($sku, $asin, $price, $cnt) = explode("\t", $val);
			list($g_num, $size) = explode("-", $sku);
			if ($size == "") { $size = "none"; }
			$AMZ_UP_LIST[$sku]['asin'] = $asin;
			$AMZ_UP_LIST[$sku]['price'] = $price;
			$AMZ_UP_LIST[$sku]['cnt'] = $cnt;		//	add ohkawara 2019/11/15
		}
	}

	//	add ohkawara 2019/11/20
	//	反映されていないASINコードを登録する。
	regist_asin($AMZ_UP_LIST);


	//	出品商品取得
	$amz0_tsv = "";
	$amz1_tsv = "";
	$amz2_tsv = "";
	$sql  = "CREATE TEMPORARY TABLE tmp_category AS".
			" SELECT g_num,cate1 FROM category".
			" WHERE display='1'".
			" AND state<'3'".
			" GROUP BY g_num,cate1;";
	mysqli_query($conn_id,$sql);

	//	ASINが有る商品は、出品ファイルでアップする。
	$AMZ_SYU_LIST = array();

	//	ファッション＆シューズ
	$amz_tsv1 = "";
	$sql  = "SELECT goods.*,category.cate1".
			" FROM goods goods".
			" INNER JOIN tmp_category category ON category.g_num=goods.g_num".
			" WHERE goods.soldout!='1'".
			" AND goods.amazonshop='1'".
			" AND (goods.ad_id like '1_%' OR goods.ad_id like '3_%')".
			";";
	if ($result = mysqli_query($conn_id,$sql)) {
		while ($list = mysqli_fetch_array($result)) {
			$set_tsv = "";
			$set_tsv = amazon_item_tsv($list, $L_BRAND, $AMZ_UP_LIST, $AMZ_SYU_LIST);
			if ($set_tsv == "") { continue; }
			$amz_tsv1 .= $set_tsv;
		}
	}

	//	スポーツ
	$amz_tsv2 = "";
	$sql  = "SELECT goods.*".
			" FROM goods goods".
			" INNER JOIN tmp_category category ON category.g_num=goods.g_num".
			" WHERE goods.soldout!='1'".
			" AND goods.amazonshop='1'".
			" AND goods.ad_id like '2_%'".
			";";
	if ($result = mysqli_query($conn_id,$sql)) {
		while ($list = mysqli_fetch_array($result)) {
			$set_tsv = "";
			$set_tsv = amazon_item_tsv($list, $L_BRAND, $AMZ_UP_LIST, $AMZ_SYU_LIST);
			if ($set_tsv == "") { continue; }
			$amz_tsv2 .= $set_tsv;
		}
	}


	//	出品ファイルL作成
	$amz_tsv0 = "";
	if (count($AMZ_UP_LIST) > 0 || $AMZ_SYU_LIST) {
		$amz_tsv0 = amazon_zaiko_item_tsv($AMZ_UP_LIST, $AMZ_SYU_LIST);
	}

	if ($amz_tsv1) {
		$amazon_tsv_head = amazon_tsv_head1();
		$amz_tsv1 = $amazon_tsv_head.$amz_tsv1;
#		$amz_tsv1 = mb_convert_encoding($amz_tsv1,"sjis-win","eucjp-win");
		$amz_tsv1 = mb_convert_encoding($amz_tsv1,"sjis-win","UTF-8");
		$OUT = fopen(AMAZON_AP_ITEM_FILE,"w");
		fwrite($OUT,$amz_tsv1);
		fclose($OUT);
		@chmod(AMAZON_AP_ITEM_FILE,0666);
		$msg .= "Amazonファッション＆シューズ在庫tsvファイル作成完了いたしました。<br>\n<br>\n";
	}

	//	JANで登録されるので基本利用されることはない
	if ($amz_tsv2) {
		$amazon_tsv_head = amazon_tsv_head2();
		$amz_tsv2 = $amazon_tsv_head.$amz_tsv2;
#		$amz_tsv2 = mb_convert_encoding($amz_tsv2,"sjis-win","eucjp-win");
		$amz_tsv2 = mb_convert_encoding($amz_tsv2,"sjis-win","UTF-8");
		$OUT = fopen(AMAZON_SP_ITEM_FILE,"w");
		fwrite($OUT,$amz_tsv2);
		fclose($OUT);
		@chmod(AMAZON_SP_ITEM_FILE,0666);
		$msg .= "Amazonスポーツ在庫tsvファイル作成完了いたしました。<br>\n<br>\n";
	}

	//	JANコードがある商品
	if ($amz_tsv0) {
		$amazon_tsv_head = amazon_tsv_head0();
		$amz_tsv0 = $amazon_tsv_head.$amz_tsv0;
#		$amz_tsv0 = mb_convert_encoding($amz_tsv0,"sjis-win","eucjp-win");
		$amz_tsv0 = mb_convert_encoding($amz_tsv0,"sjis-win","UTF-8");
		$OUT = fopen(AMAZON_JAN_ITEM_FILE,"w");
		fwrite($OUT,$amz_tsv0);
		fclose($OUT);
		@chmod(AMAZON_JAN_ITEM_FILE,0666);
		$msg .= "Amazon出品tsvファイル作成完了いたしました。<br>\n<br>\n";
	}

	//	配列・csvデータ削除	add ohkawara 2018/08/31
	unset($L_BRAND);
	unset($AMZ_UP_LIST);
	unset($AMZ_SYU_LIST);
	unset($amz_tsv1);
	unset($amz_tsv2);
	unset($amz_tsv0);

	return array($msg,$ERROR);

}

//-------------------------------------------------------------------------------------
//
//	Amazon
//
//-------------------------------------------------------------------------------------

function amazon_item_tsv($list, $L_BRAND, &$AMZ_UP_LIST, &$AMZ_SYU_LIST) {
	global $AMAZON_GOODS_DISCOUNT_CATE;

/*
	前回更新した情報を（update商品のみ）ログテーブルに保存しておく
	保存される情報は、ログ管理番号、商品管理番号、サイズ、削除フラグ、または、在庫０にしたcsvの情報を保存しておく。
	ログ保存は商品毎にまとめて行う

	親子関係が無い商品は・・・・・・
amazon_csv_log		log_num
amazon_csv_log_tmp	log_num

log_num_seq

nextval('"log_num_seq"'::text::regclass)
*/

	$tsv = "";

	$g_num = $list['g_num'];
	$g_name = $list['g_name'];
	$g_name = preg_replace("/\//", " ", $g_name);
	$code = $list['code'];
	$base_price = $list['price'];
	$base_sale_price = $list['sale_price'];
	$headline = $list['headline'];
	$caption = $list['caption'];
	$brand = $list['brand'];
	$comment = $list['comment'];

	$ad_id = $list['ad_id'];
	$ad_id2 = $list['ad_id2'];
	$size_list = $list['size_list'];
	$name_head = $list['name_head'];
	$bullet_point1 = $list['bullet_point1'];
	$bullet_point2 = $list['bullet_point2'];
	$bullet_point3 = $list['bullet_point3'];
	$bullet_point4 = $list['bullet_point4'];
	$bullet_point5 = $list['bullet_point5'];
	$generic_keywords1 = $list['generic_keywords1'];
	$generic_keywords2 = $list['generic_keywords2'];
	$generic_keywords3 = $list['generic_keywords3'];
	$generic_keywords4 = $list['generic_keywords4'];
	$generic_keywords5 = $list['generic_keywords5'];
	$generic_keywords = $generic_keywords1;	//	add ohkawara 2017/02/16

	$product_subtype = $list['product_subtype'];
	$department_name = $list['department_name'];

	$distribution_designation = $list['distribution_designation'];
	$model_year = $list['model_year'];
	$seasons = $list['seasons'];

	$amz_price = $list['amz_price'];	//	add ookawara 2015/10/08

	$zaiko = $list['zaiko'];		//	add ohkawara	2017/06/15


/*
asinが記載されている商品も一緒に処理をする！
削除ファイルも管理番号をまとめてサイズの配列を作っておいた方がよいかも？


*/
$ck_dis_flg = 0;
if ($g_num == "xxxxxx") {
	$ck_dis_flg = 1;
}

if ($ck_dis_flg == 1) {
	echo "start g_num<>".$g_num."<br>\n";
	echo "<br>\n";
}

	//	サイズがある商品か確認する！　有れば親を作成する!


	//	サイズ毎に商品管理番号を設定する為、サイズ確認を最初に行う
	$op_size = unserialize($size_list);
	$ALL_SIZE = array();
	if ($op_size) {
		$all_cnt = count($op_size);
		if ($all_cnt > 1) {
			$ALL_SIZE[0]['size'] = "";
			$ALL_SIZE[0]['jan'] = "";
			$ALL_SIZE[0]['asin'] = "";
			$ALL_SIZE[0]['type'] = "";		//	add ohkawara 2019/11/15
		}
		$i = 1;
		$asin_flg = 0;

if ($ck_dis_flg == 1) {
	echo "aaasin_flg<>".$asin_flg."<br>\n";
	echo "<br>\n";
}
		foreach ($op_size AS $key => $VAL) {
			if ($VAL['type'] == 3) {
				//	サイズが非表示だったら飛ばす	駄目！
				continue;
			}

if ($ck_dis_flg == 1) {
	echo "jan<>".$VAL['jan']."<br>\n";
	echo "asin<>".$VAL['asin']."<br>\n";
	echo "<br>\n";
}

			//	add ohkawara 2019/11/18
			//	JANがnoneでなく、登録されていて、ASINが登録されていたら出品ファイルに保存する。
			if ($VAL['jan'] != "par" && $VAL['jan'] != "none" && $VAL['asin'] != "") {
				$asin_flg = 1;
			}

			if ($VAL['jan'] == "none") {
				$VAL['jan'] = "";
			}

			//	del ohkawara 2019/11/18
			//if ($VAL['asin'] != "") {
			//	//	asinが1サイズでも登録されていた場合は、出品ファイルに保存する。
			//	//	その時にasinが入っていたサイズは販売しない。
			//	$asin_flg = 1;
			//}

			if ($VAL['jan'] && !$VAL['asin']) {
				//	janコードが登録してありasinコードが登録されていない場合はアップしない
				continue;
			}

			//	サイズ確認	空設定用
			$check_size = $VAL['size'];
			$check_size = preg_replace("/\(日本サイズ\)/", "", $check_size);

			$check_size = preg_replace("/^F$/", "", $check_size);
			$check_size = preg_replace("/フリーサイズ/", "", $check_size);
			$check_size = preg_replace("/フリー/", "", $check_size);
			$check_size = preg_replace("/--/", "", $check_size);
			$check_size = preg_replace("/フットサルサイズ/", "", $check_size);
			$check_size = preg_replace("/設定なし/", "", $check_size);

			if ($check_size) {
				if ($i == 1 && !$ALL_SIZE[0] && $check_size) {
					$ALL_SIZE[0]['size'] = "";
					$ALL_SIZE[0]['jan'] = "";
					$ALL_SIZE[0]['asin'] = "";
					$ALL_SIZE[0]['type'] = "";				//	add ohkawara 2019/11/15
				}
				$ALL_SIZE[$i]['size'] = $check_size;
				$ALL_SIZE[$i]['jan'] = $VAL['jan'];
				$ALL_SIZE[$i]['asin'] = $VAL['asin'];
				$ALL_SIZE[$i]['type'] = $VAL['type'];		//	add ohkawara 2019/11/15
				$i++;
			} else {
				$ALL_SIZE[0]['size'] = "";
				$ALL_SIZE[0]['jan'] = $VAL['jan'];
				$ALL_SIZE[0]['asin'] = $VAL['asin'];
				$ALL_SIZE[0]['type'] = $VAL['type'];		//	add ohkawara 2019/11/15
			}
		}

		//	出品ファイルでアップする商品の場合中途半端な情報を削除する
		if($asin_flg == 1) {
			foreach ($ALL_SIZE AS $key => $VAL) {
				if ($key == 0) {
					unset($ALL_SIZE[0]);
				} elseif ($ALL_SIZE[$key]['jan'] && !$ALL_SIZE[$key]['asin']) {
					unset($ALL_SIZE[$key]);
				}
			}
		}
	}
	//	$ALL_SIZEの設定がなければこの商品の登録はしない（不具合があった場合）
	if (count($ALL_SIZE) < 1) {
		return;
	}

	//	固定設定値 -----------------------------------------------------------------------------------

	//	node_id
	$node_id = preg_replace("/^1_|^2_|^3_/", "", $ad_id);
	$node_id2 = preg_replace("/^1_|^2_|^3_/", "", $ad_id2);
	$node_type = 1;
	if (preg_match("/^2_/", $ad_id)) {
		$node_type = 2;
	} elseif (preg_match("/^3_/", $ad_id)) {
		$node_type = 3;
	}

	//	ブランド名設定(必須)
	$brand_name = $L_BRAND[$brand];

	//	商品タイプ(必須)
	//	服やシューズのみ必要
	//	$product_subtypeの値を表示

	//	メーカー型番(推奨)
	//	管理画面で設定していない為空設定
	$part_number = "";

	//	商品説明文(推奨)
	$att_check = 0;
	if (preg_match("/\<!-- Attention --\>/", $caption)) {
		$att_check = 1;
	}
	$caption = preg_replace("/\<!-- Attention --\>/", "", $caption);
	$caption = preg_replace("/\<!-- Attention--\>/", "", $caption);

	$product_description = $caption;

	//	メーカー名(必須)
	$manufacturer = $brand_name;

	//	メーカー型番(推奨)
	//	情報が無い為空
	$part_number = "";

	//	商品タイプ(必須)
	//	通常固定	ゴルフなどは適切な値をセット
	//$feed_product_type = "SportingGoods";		//	del ohkawara 2019/12/13
	$feed_product_type = $product_subtype;		//	add ohkawara 2019/12/13

	//	商品の販売価格(推奨)
	//	消費税を含んだ金額にする。
	//	割引設定項目
	$GOODS_DISCOUNT_C = AMAZON_GOODS_DISCOUNT_C;
	$GOODS_DISCOUNT_CATE = $AMAZON_GOODS_DISCOUNT_CATE;
	$DISCOUNT_PAR = AMAZON_DISCOUNT_PAR;
	$DISCOUNT_PAR2 = AMAZON_DISCOUNT_PAR2;

	if ($amz_price) {				//	add ookawara 2015/10/08
		$org_sale_price = "";		//	add ookawara 2015/10/08
		$price = $amz_price;		//	add ookawara 2015/10/08
	} elseif ($base_price > 0 && $base_sale_price > 0 && $base_price < $base_sale_price) {
		$org_sale_price = "";
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
				$org_sale_price = floor($base_price * ((100 - $DISCOUNT_PAR2) / 100));
			} else {
				$org_sale_price = "";
			}
		} else {
			$org_sale_price = "";
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
			$org_sale_price_ = floor($base_price * ((100 - $DISCOUNT_PAR) / 100));
			if ($base_sale_price < 1) {
				$price = $base_price;
				$org_sale_price = $org_sale_price_;
			} elseif ($base_sale_price > $org_sale_price_) {
				$price = $base_sale_price;
				$org_sale_price = $org_sale_price_;
			} else {
				$price = $base_price;
				$org_sale_price = $base_sale_price;
			}
		} else {
			if ($base_sale_price) {
				$price = $base_sale_price;
				$org_sale_price = "";
			} else {
				$price = $base_price;
				$org_sale_price = "";
			}
		}
	}
	$price = floor($price * ((TAX / 100) + 1) + 0.5);
	$org_sale_price = floor($org_sale_price * ((TAX / 100) + 1) + 0.5);
	if ($org_sale_price < 1) {
		$standard_price = $price;
	} else {
		$standard_price = $org_sale_price;
	}

	//	通貨コード(推奨)
	//	値は固定
	//$currency = "JPY";	//	del ohkawara 2017/02/16	設定無くなった

	//	商品の公開日(任意)
	$product_site_launch_date = "";

	//	商品のコンディション(推奨)
	//	新品のみの販売の他、「New」を固定とする。中古商品を販売する場合はシステム変更必要
	$condition_type = "New";

	//	商品のコンディション説明(推奨)
	//	新品のみの販売の他、「New」を固定とする。中古商品を販売する場合はシステム変更必要
	$condition_note = "新品";

	//	メーカー希望小売価格(推奨)
	$list_price = floor($base_price * ((TAX / 100) + 1) + 0.5);

	//	使用しない支払い方法(推奨)
	$optional_payment_type_exclusion = "";

	//	配送日時指定SKUリスト(推奨)
	$delivery_schedule_group_id = AMAZON_DEL_SCHE_GR_ID;

	//	セール価格(推奨)
	$sale_price = "";

	//	セール時ポイント(任意)
	$sale_price_points = "";

	//	セール開始日(任意)
	$sale_from_date = "";

	//	セール終了日(任意)
	$sale_end_date = "";

	//	最大注文個数(任意)
	$max_order_quantity = "";

	//	最大注文個数(任意)
	$item_package_quantity = "";

	//	ギフトメッセージ(任意)
	$offering_can_be_gift_messaged = "";

	//	ギフト包装(任意)
	$offering_can_be_giftwrapped = "";

	//	メーカー製造中止(任意)
	$is_discontinued_by_manufacturer = "";

	//	商品コードなしの理由(任意)
	//$missing_keyset_reason = "";	//	del ohkawara 2017/02/16	設定無くなった

	//	配送パターン(必須)
	//	add ohkawara 2017/02/16
	$merchant_shipping_group_name = "通常配送パターン";

	//	販売形態(並行輸入品)(推奨)
	//	ユニフォームは輸入品があるのでは？
	if ($distribution_designation == 1) {
		$distribution_designation = "JP Parallel Import";
	} else {
		$distribution_designation = "";
	}

	//	商品タックスコード(任意)
	//	add ohkawara 2017/08/01
	$product_tax_code = "";

	//	配送重量の単位(任意)
	$website_shipping_weight_unit_of_measure = "";

	//	配送重量(任意)
	$website_shipping_weight = "";

	//	商品の仕様1 - 商品の仕様5(推奨)
	//	上部で設定
	//$bullet_point1 = "";
	//$bullet_point2 = "";
	//$bullet_point3 = "";
	//$bullet_point4 = "";
	//$bullet_point5 = "";

	//	推奨ブラウズノード1(任意)
	$recommended_browse_nodes1 = $node_id;
	$recommended_browse_nodes  = $node_id;

	//	推奨ブラウズノード2(任意)
	$recommended_browse_nodes2 = $node_id2;

	//	検索キーワード1 - 検索キーワード5(任意)
	//	上部で設定
	//$generic_keywords1 = "";
	//$generic_keywords2 = "";
	//$generic_keywords3 = "";
	//$generic_keywords4 = "";
	//$generic_keywords5 = "";

	//	スタイルキーワード1 - スタイルキーワード5(任意)値有り
	$style_keywords  = "";	//	add ohkawara 2017/02/16
	$style_keywords1 = "";
	$style_keywords2 = "";
	$style_keywords3 = "";
	$style_keywords4 = "";
	$style_keywords5 = "";

	//	プラチナキーワード1 - プラチナキーワード5(任意)
	//	add ohkawara 2017/02/16	設定無くなった
	$platinum_keywords  = "";
	$platinum_keywords1 = "";
	$platinum_keywords2 = "";
	$platinum_keywords3 = "";
	$platinum_keywords4 = "";
	$platinum_keywords5 = "";

	//	対象(任意)
	//	add ohkawara 2017/02/16
	$target_audience_base = "";

	//	コート・ワンピース・チュニック着丈(任意)
	$specific_uses_keywords = "";

	//	add ohkawara 2019/12/12
	$specific_uses_keywords1 = "";
	$specific_uses_keywords2 = "";
	$specific_uses_keywords3 = "";
	$specific_uses_keywords4 = "";
	$specific_uses_keywords5 = "";

	//	商品メイン画像URL(必須)
	$PIC_LIST = array();
#	$g_name_ = mb_convert_kana("$g_name","S","EUC-JP");
	$g_name_ = mb_convert_kana("$g_name","S","UTF-8");
	$main_image_url = "";
	$imagef_amz_file = IMG_F_AMZ_DIR."/$code" . ".jpg";
	$imagef_file = IMG_F_DIR."/$code" . ".jpg";
	$image_main_file = IMG_M_AMZ_DIR."/$code" . ".jpg";	//	add ohkawara 2018/10/23

	//	update ohkawara 2019/12/13	http→httpsに変更　

	if (file_exists($image_main_file)) {										//	add ohkawara 2018/10/23
		$PIC_LIST[] = "https://www.futboljersey.com/image_main_amz/$code.jpg";	//	add ohkawara 2018/10/23
	} elseif (file_exists($imagef_amz_file)) {									//	add ohkawara 2018/10/23
	//if (file_exists($imagef_amz_file)) {										//	del ohkawara 2018/10/23
		$PIC_LIST[] = "https://www.futboljersey.com/imagef_amz/$code.jpg";
	} elseif (file_exists($imagef_file)) {
		$PIC_LIST[] = "https://www.futboljersey.com/imagef/$code.jpg";
	}

	//	カラーサンプル画像URL(任意)
	$swatch_image_url = "";

	//	商品のサブ画像URL1 - 商品のサブ画像URL8(任意)
	$other_image_url1 = "";
	$other_image_url2 = "";
	$other_image_url3 = "";
	$other_image_url4 = "";
	$other_image_url5 = "";
	$other_image_url6 = "";
	$other_image_url7 = "";
	$other_image_url8 = "";

	//	1枚目画像
	$imageb_amz_file = IMG_B_AMZ_DIR."/$code" . ".jpg";
	$imageb_file = IMG_B_DIR."/$code" . ".jpg";
	if (file_exists($imageb_amz_file)) {
		$PIC_LIST[] = "https://www.futboljersey.com/imageb_amz/$code.jpg";
	} elseif (file_exists($imageb_file)) {
		$PIC_LIST[] = "https://www.futboljersey.com/imageb/$code.jpg";
	}
	//	2枚目画像
	$image03_amz_file = IMG_03_AMZ_DIR."/$code" . ".jpg";
	$image03_file = IMG_03_DIR."/$code" . ".jpg";
	if (file_exists($image03_amz_file)) {
		$PIC_LIST[] = "https://www.futboljersey.com/image03_amz/$code.jpg";
	} elseif (file_exists($image03_file)) {
		$PIC_LIST[] = "https://www.futboljersey.com/image03/$code.jpg";
	}
	//	3枚目画像
	$image04_amz_file = IMG_04_AMZ_DIR."/$code" . ".jpg";
	$image04_file = IMG_04_DIR."/$code" . ".jpg";
	if (file_exists($image04_amz_file)) {
		$PIC_LIST[] = "https://www.futboljersey.com/image04_amz/$code.jpg";
	} elseif (file_exists($image04_file)) {
		$PIC_LIST[] = "https://www.futboljersey.com/image04/$code.jpg";
	}
	//	4枚目画像
	$image05_amz_file = IMG_05_AMZ_DIR."/$code" . ".jpg";
	$image05_file = IMG_05_DIR."/$code" . ".jpg";
	if (file_exists($image05_amz_file)) {
		$PIC_LIST[] = "https://www.futboljersey.com/image05_amz/$code.jpg";
	} elseif (file_exists($image05_file)) {
		$PIC_LIST[] = "https://www.futboljersey.com/image05/$code.jpg";
	}
	//	5枚目画像
	$image06_file = IMG_06_DIR."/$code" . ".jpg";
	if (file_exists($image06_file)) {
		$PIC_LIST[] = "https://www.futboljersey.com/image06/$code.jpg";
	}
	//	6枚目画像
	$image07_file = IMG_07_DIR."/$code" . ".jpg";
	if (file_exists($image07_file)) {
		$PIC_LIST[] = "https://www.futboljersey.com/image07/$code.jpg";
	}
	//	7枚目画像
	$image08_file = IMG_08_DIR."/$code" . ".jpg";
	if (file_exists($image08_file)) {
		$PIC_LIST[] = "https://www.futboljersey.com/image08/$code.jpg";
	}
	//	8枚目画像
	$image09_file = IMG_09_DIR."/$code" . ".jpg";
	if (file_exists($image09_file)) {
		$PIC_LIST[] = "https://www.futboljersey.com/image09/$code.jpg";
	}

	//	メイン画像が無かった場合の対応処理
	$PIC_NAME_LIST[0] = "main_image_url";
	$PIC_NAME_LIST[1] = "other_image_url1";
	$PIC_NAME_LIST[2] = "other_image_url2";
	$PIC_NAME_LIST[3] = "other_image_url3";
	$PIC_NAME_LIST[4] = "other_image_url4";
	$PIC_NAME_LIST[5] = "other_image_url5";
	$PIC_NAME_LIST[6] = "other_image_url6";
	$PIC_NAME_LIST[7] = "other_image_url7";
	$PIC_NAME_LIST[8] = "other_image_url8";

	if ($PIC_LIST) {
		foreach ($PIC_LIST AS $key => $val) {
			$$PIC_NAME_LIST[$key] = $val;
		}
	}

	//	対象年齢・性別(必須)
	//$department_name = "";	上部で設定

	//	モデル年(発売年・発表年)	Amazonから指定があった場合	西暦数字のみ
	//$model_year = "";	上部で設定

	//	シーズン	Amazonから指定があった場合	指定した値　春　夏　秋　冬　春夏　等
	//$seasons = "";	上部で設定


	//	スポーツ用設定値	上記は洋服専用または、スポーツ共通情報	----------------------

	//	商品タイプ(必須)
	//$feed_product_type = "SportingGoods";	//	del ohkawara 2019/12/13	上部で設定済み


	//	対象1 - 対象4(任意)
	$target_audience_keywords1 = "";
	$target_audience_keywords2 = "";
	$target_audience_keywords3 = "";
	$target_audience_keywords4 = "";
	$DE_NM = explode(",", $department_name);
	if ($DE_NM) {
		$i = 0;
		foreach ($DE_NM AS $val) {
			if (!$val) { continue; }
			if ($val != "メンズ" && $val != "レディース" && $val != "ボーイズ" && $val != "ガールズ") { continue; }
			$set_name = "target_audience_keywords".$i;
			$$set_name = $val;
			$i++;
		}
	}

	//	対象年齢・性別(必須)
	$department_name1 = "";
	$department_name2 = "";
	$department_name3 = "";
	$department_name4 = "";
	$department_name5 = "";
	$DE_NM = explode(",", $department_name);
	if ($DE_NM) {
		$i = 0;
		foreach ($DE_NM AS $val) {
			if (!$val) { continue; }
			$set_name = "department_name".$i;
			$$set_name = $val;
			$i++;
		}
	}

	$JUNIOR_CATEGORY=[68,71,80];
	

	//	未設定項目リセット
	$fulfillment_center_id = "";
	$package_length = "";
	$package_width = "";
	$package_height = "";
	$package_length_unit_of_measure = "";
	$package_weight = "";
	$package_weight_unit_of_measure = "";
	$color_name = "";
	$color_map = "";
	$style_name = "";
	$shoe_width = "";
	$heel_height_unit_of_measure = "";
	$heel_height = "";
	$strap_type = "";
	$toe_shape = "";
	$waist_style = "";
	$opacity = "";
	$heel_type = "";
	$shaft_height = "";
	$outer_material_type = "";
	$shaft_diameter = "";
	$sleeve_type = "";
	$closure_type = "";
	$lifestyle1 = "";
	$lifestyle2 = "";
	$lifestyle3 = "";
	$lifestyle4 = "";
	$lifestyle5 = "";
	$fabric_type = "商品説明文参照"; // 2022/12/9 DBでこの値定義されていない
	$is_adult_product = "";
	$minimum_height_recommendation_unit_of_measure = "";
	$minimum_height_recommendation = "";
	$maximum_height_recommendation_unit_of_measure = "";
	$maximum_height_recommendation = "";
	$waist_size_unit_of_measure = "";
	$waist_size = "";
	$inseam_length_unit_of_measure = "";
	$inseam_length = "";
	$sleeve_length_unit_of_measure = "";
	$sleeve_length = "";
	$collar_style = "";
	$neck_style = "";
	$neck_size_unit_of_measure = "";
	$neck_size = "";
	$bottom_style = "";
	$chest_size_unit_of_measure = "";
	$chest_size = "";
	$cup_size = "";
	$furisode_length_unit_of_measure = "";
	$furisode_length = "";
	$furisode_width_unit_of_measure = "";
	$furisode_width = "";
	$obi_length_unit_of_measure = "";
	$obi_length = "";
	$obi_width_unit_of_measure = "";
	$obi_width = "";
	$tsukeobi_width_unit_of_measure = "";
	$tsukeobi_width = "";
	$tsukeobi_height_unit_of_measure = "";
	$tsukeobi_height = "";
	$pillow_size = "";
	$pillow_size_unit_of_measure = "";
	$fit_to_size_description = "";

	//	スポーツ未設定
	$volume_capacity_name = "";
	$item_display_diameter = "";
	$item_display_diameter_unit_of_measure = "";
	$item_display_height = "";
	$item_display_height_unit_of_measure = "";
	$item_display_length = "";
	$item_display_length_unit_of_measure = "";
	$item_display_width = "";
	$item_display_width_unit_of_measure = "";
	$item_display_weight = "";
	$item_display_weight_unit_of_measure = "";
	$safety_warning = "";
	$country_of_origin = "";
	$legal_disclaimer_description = "";
	$mfg_warranty_description_type = "";
	$seller_warranty_description = "";
	$pattern_name = "";
	$material_type = "";
	$item_package_contents = "";
	$rise_style = "";
	$golf_club_flex = "";
	$golf_club_loft = "";
	$grip_size = "";
	$grip_type = "";
	$grip_material_type = "";
	$rim_size = "";
	$rim_size_unit_of_measure = "";
	$wheel_size = "";
	$wheel_size_unit_of_measure = "";
	$diving_clothing_thickness = "";
	$line_weight = "";
	$tension_level = "";
	$fishing_line_type = "";
	$unknown_subject = "";
	$caliber = "";
	$number_of_rounds = "";
	$boot_size = "";
	$calf_size = "";
	$capacity = "";
	$capacity_unit_of_measure = "";
	$flavor_name = "";
	$lens_color = "";
	$fuel_type = "";
	$is_autographed = "";
	$curvature = "";
	$fencing_pommel_type = "";
	$hand_orientation = "";
	$head_size = "";
	$is_memorabilia = "";
	$shaft_length = "";
	$shaft_length_unit_of_measure = "";
	$shaft_material = "";
	$shaft_style_type = "";
	$item_shape = "";
	$wattage = "";
	$display_maximum_weight_recommendation = "";
	$display_maximum_weight_recommendation_unit_of_measure = "";
	$specific_uses_for_product = "";
	$item_thickness_derived = "";
	$item_thickness_unit_of_measure = "";
	$are_batteries_included = "";
	$batteries_required = "";
	$battery_type = "";		//	add ohkawara 2017/02/16
	$battery_type1 = "";
	$battery_type2 = "";
	$battery_type3 = "";
	$number_of_batteries = "";		//	add ohkawara 2017/02/16
	$number_of_batteries1 = "";
	$number_of_batteries2 = "";
	$number_of_batteries3 = "";
	$lithium_battery_energy_content = "";
	$lithium_battery_packaging = "";
	$lithium_battery_voltage = "";
	$lithium_battery_weight = "";
	$number_of_lithium_ion_cells = "";
	$number_of_lithium_metal_cells = "";
	$team_name = "";			//	add ohkawara 2017/02/16
	$shipping_options = "";		//	add ohkawara 2017/02/16
	$seating_group = "";		//	add ohkawara 2017/02/16

	//	add ohkawara	2019/12/12
	$specific_uses_keywords1 = "";
	$specific_uses_keywords2 = "";
	$specific_uses_keywords3 = "";
	$specific_uses_keywords4 = "";
	$specific_uses_keywords5 = "";
	$item_length_description = "";
	$material_composition1 = "";
	$material_composition2 = "";
	$material_composition3 = "";
	$material_composition4 = "";
	$material_composition5 = "";
	$material_composition6 = "";
	$material_composition7 = "";
	$material_composition8 = "";
	$material_composition9 = "";
	$material_composition10 = "";
	$special_features1 = "";
	$special_features2 = "";
	$special_features3 = "";
	$special_features4 = "";
	$special_features5 = "";
	$band_size_num = "";
	$band_size_num_unit_of_measure = "";
	$inner_material_type1 = "";
	$inner_material_type2 = "";
	$inner_material_type3 = "";
	$inner_material_type4 = "";
	$inner_material_type5 = "";
	$sole_material = "";
	$target_gender = "ユニセックス"; // 2022/12/9 DBでこの値定義されていない	
	$age_range_description = in_array($list["cate1"], $JUNIOR_CATEGORY)?"キッズ":"大人";
	$footwear_size_system = "";
	$footwear_age_group = "";
	$footwear_size_class = "";
	$footwear_width = "";
	$footwear_size = "";
	$footwear_to_size = "";
	$platinum_keywords1 = "";
	$platinum_keywords2 = "";
	$platinum_keywords3 = "";
	$platinum_keywords4 = "";
	$platinum_keywords5 = "";
	$material_type = "";
	$shaft_style_type = "";
	$fit_type = "";
	$shaft_height_unit_of_measure = "";
	$item_width = "";
	$item_height = "";
	$shoe_width_unit_of_measure = "";
	$item_dimensions_unit_of_measure = "";
	$item_length = "";
	$batteries_required = "";
	$are_batteries_included = "";
	$battery_cell_composition = "";
	$battery_type1 = "";
	$battery_type2 = "";
	$battery_type3 = "";
	$number_of_batteries1 = "";
	$umber_of_batteries2 = "";
	$number_of_batteries3 = "";
	$number_of_lithium_metal_cells = "";
	$number_of_lithium_ion_cells = "";
	$lithium_battery_packaging = "";
	$lithium_battery_energy_content = "";
	$lithium_battery_energy_content_unit_of_measure = "";
	$lithium_battery_weight = "";
	$lithium_battery_weight_unit_of_measure = "";
	$item_weight = "";
	$item_weight_unit_of_measure = "";
	$item_volume = "";
	$item_volume_unit_of_measure = "";
	$country_of_origin = "";
	$offering_end_date = "";
	$offering_start_date = "";
	$business_price = "";
	$quantity_price_type = "";
	$quantity_lower_bound1 = "";
	$quantity_price1 = "";
	$quantity_lower_bound2 = "";
	$quantity_price2 = "";
	$quantity_lower_bound3 = "";
	$quantity_price3 = "";
	$quantity_lower_bound4 = "";
	$quantity_price4 = "";
	$quantity_lower_bound5 = "";
	$quantity_price5 = "";
	$pricing_action = "";
	$unspsc_code = "";
	$national_stock_number = "";

if ($ck_dis_flg == 1) {
	echo "asin_flg<>".$asin_flg."<br>\n";
	echo "<br>\n";
	pre($ALL_SIZE);
	echo "<br>\n";
}
	
	//シャツサイズ
	$shirt_size_system_group="JP";
	$shirt_size_class_group="";
	foreach ($ALL_SIZE as $record){
		if (preg_match("/^\\d+$/",$record['size'] )){
			if (in_array((int)$record['size'],[120,130,140,150,160,170])){
				$shirt_size_class_group="身長";
				break;
			} else {
				$shirt_size_class_group="数学サイズ";
				continue;
			}
		} else {
			if (strpos($record['size'], 'O')!==false){
				$shirt_size_class_group="アルファベット ジャスポ";
				break;
			} else {
				$shirt_size_class_group="アルファベット";
				continue;
			}
		}
	}

	//	各設定 -----------------------------------------------------------------------------------
	foreach ($ALL_SIZE AS $key => $val) {

		if ($asin_flg == 1 && count($ALL_SIZE) > 1 && $key == 0) {
			//	出品ファイルでは親情報はいらない為スキップさせる
			continue;
		}

		//	管理番号設定(必須)
		//	管理番号に枝記号を入れるか入れないか判断する。（サイズが無い場合や、親商品の場合は枝記号は無し）
		$size = $ALL_SIZE[$key]['size'];
		$ext_pro_id = $ALL_SIZE[$key]['jan'];
		$sku_size = "";
		$item_sku = "";
		if ($key == 0 && $size == "") {
			$item_sku = "g".$g_num;
		} else {
			$sku_size = $size;
			$set_sku_size = preg_replace("/[^0-9a-zA-Z.]/", "", $sku_size);
			$item_sku = "g".$g_num."-".$set_sku_size;
		}

		//	登録済み商品の配列削除
		$set_g_num = "g".$g_num;
		if ($key == 0) {
			$set_size = "none";
		} else {
			$set_size = $size;
		}
		unset($AMZ_UP_LIST[$item_sku]);

		//	商品名設定(必須)
		if ($key == 0) {
			$item_name = $g_name;
		} else {
			$item_name = $g_name." ".$size;
		}

		//	商品コード設定(必須)
		$external_product_id = "";
		if (count($ALL_SIZE) == 1) {	//	サイズが無い場合
			if ($ext_pro_id != "") {
				$external_product_id = $ext_pro_id;
			}
		} elseif ($key > 0 && $ext_pro_id != "") {	//	サイズが複数ある場合で子商品の場合
			$external_product_id = $ext_pro_id;
		}

		//	商品コード(JANコード等)設定(必須)
		$external_product_id_type = "";
		$len = 0;
		if ($external_product_id != "") {
			$len = strlen($external_product_id);
			if ($len == 16) {
				$external_product_id_type = "GCID";
			} elseif ($len == 12) {
				$external_product_id_type = "UPC";
			} elseif ($len == 13) {
				$external_product_id_type = "EAN";
			}
		}

		//	アップデート・削除(必須)
		//	Update or PartialUpdate or Delete 通常 Update or Delete
		$update_delete = "Update";

		//	自社で管理しているSKUごとの番号(推奨)
		//	出品者独自のSKUごとの識別コード。商品管理番号（item_sku）と同じでもよい。
		$codabar = $item_sku;

		//	在庫数(推奨)
		$quantity = 0;
		if (count($ALL_SIZE) > 1 && $key == 0 && $size == "") {
			//	親商品の場合は在庫数を入れない
			$quantity = "";
		//} elseif ($att_check == 1) {					//	del ohkawara 2017/06/15
		} elseif ($att_check == 1 || $zaiko == 1) {		//	add ohkawara 2017/06/15
			$quantity = 1;
		} else {
			$quantity = AMAZON_DEFO_ZAIKO;
		}

		//	リードタイム(出荷までにかかる作業日数)
		$fulfillment_latency = "";
		if ($att_check == 1) {
			$fulfillment_latency = AMAZON_MARK_DAYS;
		}
		if (count($ALL_SIZE) > 1 && $key == 0 && $size == "") {
			//	親商品の場合はリードタイムを入れない
			$fulfillment_latency = "";
		}

		//	商品の公開日(任意)
		$product_site_launch_date = "";

		//	予約商品の販売開始日(任意)
		$merchant_release_date = "";

		//	メーカー希望価格(任意)
		$list_price = "";

		//	商品の販売価格(推奨)
		//	親情報の時には金額を設定しない為の設定
		if (count($ALL_SIZE) > 1 && $key == 0) {	//	サイズがある場合で親商品の場合は価格を入れない
			$set_standard_price = "";
		} else {
			$set_standard_price = $standard_price;
		}

		//	ポイント(任意)
		$standard_price_points = "";
		if ($standard_price > 0) {
			$standard_price_points = round($standard_price * (AMAZON_POINT / 100));
		}
		//	ポイント(パーセント)
		$standard_points_percent=1;
		$sale_points_percent=0;


		//	商品の入荷予定日(任意)
		$restock_date = "";

		//	親子関係の指定(任意)	（現在日本では使用不可）
		$parent_child = "child";
		if (count($ALL_SIZE) > 1 && $key == 0) {
			$parent_child = "parent";
		} elseif (count($ALL_SIZE) == 1) {
			$parent_child = "";
		}

		//	親商品のSKU(商品管理番号)(任意)
		$parent_sku = "g".$g_num;
		if ($parent_child != "child") {
			$parent_sku = "";
		}

		//	親子関係のタイプ(任意)
		$relationship_type = "Variation";
		if (count($ALL_SIZE) > 1 && $key == 0) {
			$relationship_type = "";
		}

		//	バリエーションテーマ(任意)
		$variation_theme = "Size";
		if (count($ALL_SIZE) == 1) {
			$variation_theme = "";
		}


		//	サイズ(推奨)
		$size_name = "";
		if ($sku_size) {
			$size_name = $sku_size;
		}

		if ($size_name==""){
			$shirt_size_system="";
			$shirt_size_class="";
		} else{
			$shirt_size_system=$shirt_size_system_group;
			$shirt_size_class=$shirt_size_class_group;
			if ($shirt_size_class_group=="身長"){
				$size_name=	"Height {$size_name}";
			} elseif ($shirt_size_class_group=="アルファベット ジャスポ"){
			$size_name=	"JASPO {$size_name}";
			}
		}

		//	サイズマップ(任意)
		$size_map = "";
		if ($sku_size) {
			$size_map = $sku_size;
		}

		//	ブランド名設定(必須)	小文字→大文字変換
		//	add ohkawara 2017/08/01
		$brand_name = strtoupper($brand_name);

		if ($asin_flg == 1) {
			$set_tsv = "";

			$sku = $item_sku;
			$price = $set_standard_price;
			$product_id = $ALL_SIZE[$key]['asin'];
			$product_id_type = "ASIN";
			$condition_type = $condition_type;
			$operation_type = $update_delete;
			$sale_price = $sale_price;
			//$sale_price_points = $sale_price_points;
			$sale_start_date = $sale_from_date;
			//$sale_end_date = $sale_end_date;
			$leadtime_to_ship = $fulfillment_latency;
			$launch_date = $product_site_launch_date;
			$is_giftwrap_available = $offering_can_be_giftwrapped;
			$is_gift_message_available = $offering_can_be_gift_messaged;
			$optional_payment_type_exclusion = $optional_payment_type_exclusion;
			$main_offer_image = $main_image_url;
			$offer_image1 = $other_image_url1;
			$offer_image2 = $other_image_url2;
			$offer_image3 = $other_image_url3;
			$offer_image4 = $other_image_url4;
			$offer_image5 = $other_image_url5;

			if (!$product_id) {
				continue;
			}

			//	未設定変数
			$condition_note = "";
			$ASIN_hint = "";
			$title = "";
			$fulfillment_center_id = "";

			$set_tsv  = "\"".$sku."\"\t".
						"\"".$price."\"\t".
						"\"".$standard_points_percent."\"\t".
						"\"".$quantity."\"\t".
						"\"".$product_id."\"\t".
						"\"".$product_id_type."\"\t".
						"\"".$condition_type."\"\t".
						"\"".$condition_note."\"\t".
						"\"".$ASIN_hint."\"\t".
						"\"".$title."\"\t".
						"\"".$operation_type."\"\t".
						"\"".$sale_price."\"\t".
						"\"".$sale_points_percent."\"\t".
						"\"".$sale_start_date."\"\t".
						"\"".$sale_end_date."\"\t".
						"\"".$leadtime_to_ship."\"\t".
						"\"".$launch_date."\"\t".
						"\"".$is_giftwrap_available."\"\t".
						"\"".$is_gift_message_available."\"\t".
						"\"".$fulfillment_center_id."\"\t".
						"\"".$optional_payment_type_exclusion."\"\t".
						"\"".$main_offer_image."\"\t".
						"\"".$offer_image1."\"\t".
						"\"".$offer_image2."\"\t".
						"\"".$offer_image3."\"\t".
						"\"".$offer_image4."\"\t".
						"\"".$offer_image5."\"\t".
						"\"".$merchant_shipping_group_name."\"\n";		//	add ohkawara 2017/08/04
			$AMZ_SYU_LIST[] = $set_tsv;
		} elseif ($node_type != 2) {
			//	ファッション

			//	add ohkawara 2019/12/12	マーキングシャツのみなので
			$feed_product_type = "shirt";
			$lifestyle1 = "カジュアル";
			$lifestyle2 = "制服";
			// $department_name = "メンズ";
			$department_name = "男性";






			//	マーキング商品などオリジナル商品になってしまう商品の子商品（サイズ情報有り）は、独自のメーカー型番（g○○○○-○○）を記載する。
			//	add ohkawara 2017/11/16
			if ($parent_child == "child") {
				if ($part_number == "") {
					$part_number = $item_sku;
				}
			}

			$tsv .= "\"".$feed_product_type."\"\t". //	商品タイプ  feed_product_type
			"\"".$item_sku."\"\t". //	出品者SKU  item_sku
			"\"".$brand_name."\"\t". //	ブランド名  brand_name
			"\"".$item_name."\"\t". //	商品名  item_name
			"\"".$external_product_id."\"\t". //	商品コード(JANコード等)  external_product_id
			"\"".$external_product_id_type."\"\t". //	商品コードのタイプ  external_product_id_type
			"\"\"\t". //!!	モデル名  model_name
			"\"\"\t". //!!	模範  model
			"\"".$bullet_point1."\"\t". // 	商品の仕様  bullet_point1
			"\"".$bullet_point2."\"\t". // 	商品の仕様  bullet_point2
			"\"".$bullet_point3."\"\t". // 	商品の仕様  bullet_point3
			"\"".$bullet_point4."\"\t". // 	商品の仕様  bullet_point4
			"\"".$bullet_point5."\"\t". // 	商品の仕様  bullet_point5
			"\"".$department_name."\"\t". //	部門名  department_name
			"\"".$fabric_type."\"\t". //	素材構成  fabric_type
			"\"".$quantity."\"\t". //	在庫数  quantity
			"\"".$standard_price."\"\t". //	商品の販売価格  standard_price
			"\"".$main_image_url."\"\t". //	商品メイン画像URL  main_image_url
			"\"".$target_gender."\"\t". //	性別  target_gender
			"\"".$age_range_description."\"\t". //	対象年齢層  age_range_description
			"\"\"\t". //!!	アパレルサイズ表記単位(国)  apparel_size_system
			"\"\"\t". //!!	アパレルサイズの表記方法  apparel_size_class
			"\"\"\t". //	アパレルサイズの値  apparel_size
			"\"\"\t". //	アパレルサイズ(最大レンジ)  apparel_size_to
			"\"\"\t". //	アパレルサイズのボディタイプ  apparel_body_type
			"\"\"\t". //	アパレルサイズの身長タイプ  apparel_height_type
			"\"".$outer_material_type."\"\t". //	表地素材  outer_material_type
			"\"".$size_name."\"\t". //	サイズ  size_name
			"\"".$color_name."\"\t". //	カラー  color_name
			"\"".$color_map."\"\t". //	カラーマップ  color_map
			"\"".$size_map."\"\t". //	サイズマップ  size_map
			"\"".$is_adult_product."\"\t". //	アダルト商品  is_adult_product
			"\"".$recommended_browse_nodes."\"\t". //	推奨ブラウズノード  recommended_browse_nodes
			"\"\"\t". //!!	ユニット数  unit_count
			"\"\"\t". //!!	ユニット数のタイプ  unit_count_type
			"\"\"\t". //!!	この商品はAmazon.co.jp限定商品ですか？  is_exclusive_product
			"\"\"\t". //!! 	商品に適用される危険物規制の種類  supplier_declared_material_regulation1
			"\"\"\t". //!! 	商品に適用される危険物規制の種類  supplier_declared_material_regulation2
			"\"\"\t". //!! 	商品に適用される危険物規制の種類  supplier_declared_material_regulation3
			"\"".$product_description."\"\t". //	商品説明文  product_description
			"\"".$part_number."\"\t". //	メーカー型番  part_number
			"\"".$closure_type."\"\t". //	closure_type  closure_type
			"\"".$manufacturer."\"\t". //	manufacturer  manufacturer
			"\"".$style_name."\"\t". //	スタイル名  style_name
			"\"\"\t". //!!	種類  item_type_name
			"\"".$item_package_quantity."\"\t". //	パッケージ商品数  item_package_quantity
			"\"\"\t". //!!	care_instructions  care_instructions
			"\"\"\t". //!!	品目の前から後ろまでの奥行き  depth_front_to_back
			"\"\"\t". //!!	品目の奥行きの単位  depth_front_to_back_unit_of_measure
			"\"\"\t". //!!	品目の左右の幅  depth_width_side_to_side
			"\"\"\t". //!!	品目の幅単位  depth_width_side_to_side_unit_of_measure
			"\"\"\t". //!!	底部から上部までの高さ  depth_height_floor_to_top
			"\"\"\t". //!!	品目の高さ単位  depth_height_floor_to_top_unit_of_measure
			"\"\"\t". //!!	付属品総数  number_of_items
			"\"".$footwear_size_system."\"\t". //	靴サイズ表記単位（国）  footwear_size_system
			"\"".$footwear_age_group."\"\t". //	対象年齢層(詳細)  footwear_age_group
			"\"".$footwear_size_class."\"\t". //	靴サイズ表記単位  footwear_size_class
			"\"".$footwear_size."\"\t". //	靴サイズ  footwear_size
			"\"\"\t". //!!	シェイプウェアのサイズ システム  shapewear_size_system
			"\"\"\t". //!!	シェイプウェアのサイズ クラス  shapewear_size_class
			"\"\"\t". //	シェイプウェアのサイズ値  shapewear_size
			"\"\"\t". //	シェイプウェアのサイズ範囲  shapewear_size_to
			"\"\"\t". //	バンドサイズの値  shapewear_band_size
			"\"\"\t". //	バンドサイズ(最大レンジ)  shapewear_band_size_to
			"\"\"\t". //	カップサイズの値  shapewear_cup_size
			"\"\"\t". //	シェイプウェア ボディタイプ  shapewear_body_type
			"\"\"\t". //	シェイプウェアの身長タイプ  shapewear_height_type
			"\"\"\t". //!!	ボトムスサイズシステム  bottoms_size_system
			"\"\"\t". //!!	ボトムスサイズクラス  bottoms_size_class
			"\"\"\t". //	ボトムスサイズ値  bottoms_size
			"\"\"\t". //	ボトムスのサイズ範囲  bottoms_size_to
			"\"\"\t". //	ウエストサイズ値  bottoms_waist_size
			"\"\"\t". //	股下サイズ値  bottoms_inseam_size
			"\"\"\t". //	ボトムス体型タイプ  bottoms_body_type
			"\"\"\t". //	ボトムス身長タイプ  bottoms_height_type
			"\"\"\t". //!!	スカートサイズシステム  skirt_size_system
			"\"\"\t". //!!	スカートサイズクラス  skirt_size_class
			"\"\"\t". //	スカートサイズ値  skirt_size
			"\"\"\t". //	スカートのサイズ範囲  skirt_size_to
			"\"\"\t". //	ウエストサイズ値  skirt_waist_size
			"\"\"\t". //	スカート体型タイプ  skirt_body_type
			"\"\"\t". //	スカート身長タイプ  skirt_height_type
			"\"".$shirt_size_system."\"\t". //	シャツサイズシステム  shirt_size_system
			"\"".$shirt_size_class."\"\t". //	シャツサイズクラス  shirt_size_class
			"\"".$size_name."\"\t". //	シャツサイズ値  shirt_size
			"\"\"\t". //	シャツのサイズ範囲  shirt_size_to
			"\"\"\t". //	首回りサイズ値  shirt_neck_size
			"\"\"\t". //	裄丈サイズ値  shirt_sleeve_length
			"\"\"\t". //	シャツ体型タイプ  shirt_body_type
			"\"\"\t". //	シャツ身長タイプ  shirt_height_type
			"\"\"\t". //!!	水平方向の長端の長さ  length_head_to_toe
			"\"\"\t". //!!	長さ単位  length_head_to_toe_unit_of_measure
			"\"\"\t". //!!	水平方向の短い方の端の幅  length_width_side_to_side
			"\"\"\t". //!!	幅単位  length_width_side_to_side_unit_of_measure
			"\"\"\t". //!!	底部から上部までの高さ  length_height_floor_to_top
			"\"\"\t". //!!	高さの単位  length_height_floor_to_top_unit_of_measure
			"\"".$special_features1."\"\t". // 	機能性  special_features1
			"\"".$special_features2."\"\t". // 	機能性  special_features2
			"\"".$special_features3."\"\t". // 	機能性  special_features3
			"\"".$special_features4."\"\t". // 	機能性  special_features4
			"\"".$special_features5."\"\t". // 	機能性  special_features5
			"\"\"\t". // 	機能性  special_features6
			"\"\"\t". // 	機能性  special_features7
			"\"\"\t". // 	機能性  special_features8
			"\"\"\t". // 	機能性  special_features9
			"\"\"\t". // 	機能性  special_features10
			"\"\"\t". // 	機能性  special_features11
			"\"\"\t". // 	機能性  special_features12
			"\"\"\t". // 	機能性  special_features13
			"\"\"\t". // 	機能性  special_features14
			"\"".$band_size_num."\"\t". //	バンドサイズ  band_size_num
			"\"".$band_size_num_unit_of_measure."\"\t". //	バンドサイズの単位  band_size_num_unit_of_measure
			"\"".$lifestyle1."\"\t". // 	ライフスタイル  lifestyle1
			"\"".$lifestyle2."\"\t". //     ライフスタイル  lifestyle2
			"\"\"\t". //	ライフスタイル  lifestyle3
			"\"\"\t". //	ライフスタイル  lifestyle4
			"\"\"\t". //	ライフスタイル  lifestyle5
			"\"\"\t". //	ライフスタイル  lifestyle6
			"\"\"\t". //	ライフスタイル  lifestyle7
			"\"\"\t". //	ライフスタイル  lifestyle8
			"\"\"\t". //	ライフスタイル  lifestyle9
			"\"\"\t". //	ライフスタイル  lifestyle10
			"\"".$neck_style."\"\t". //	首のタイプ  neck_style
			"\"".$item_length_description."\"\t". //	長さ  item_length_description
			"\"".$material_composition1."\"\t". //?	素材構成  material_composition
			"\"\"\t". //!!	帽子サイズ表記単位(国)   headwear_size_system
			"\"\"\t". //!!	帽子サイズの表記方法  headwear_size_class
			"\"\"\t". //	帽子サイズの値  headwear_size
			"\"\"\t". //	帽子のサイズ範囲  headwear_size_to
			//  画像 - 商品画像を表示させるために必要な項目。詳しくは画像説明タブを参照
			"\"".$other_image_url1."\"\t". // 	商品のサブ画像URL  other_image_url1
			"\"".$other_image_url2."\"\t". // 	商品のサブ画像URL  other_image_url2
			"\"".$other_image_url3."\"\t". // 	商品のサブ画像URL  other_image_url3
			"\"".$other_image_url4."\"\t". // 	商品のサブ画像URL  other_image_url4
			"\"".$other_image_url5."\"\t". // 	商品のサブ画像URL  other_image_url5
			"\"".$other_image_url6."\"\t". // 	商品のサブ画像URL  other_image_url6
			"\"".$other_image_url7."\"\t". // 	商品のサブ画像URL  other_image_url7
			"\"".$other_image_url8."\"\t". // 	商品のサブ画像URL  other_image_url8
			"\"".$swatch_image_url."\"\t". //	カラーサンプル画像URL  swatch_image_url
			//   バリエーション - 商品の色・サイズなどのバリエーションを作成する際に必須の項目   
			"\"".$parent_child."\"\t". //	親子関係の指定  parent_child
			"\"".$parent_sku."\"\t". //	親商品のSKU(商品管理番号)  parent_sku
			"\"".$relationship_type."\"\t". //	子関係タイプ  relationship_type
			"\"".$variation_theme."\"\t". //	バリエーション種類  variation_theme
			"\"\"\t". //	パッケージレベル  package_level
			"\"\"\t". //	パッケージ内包数量  package_contains_quantity
			"\"\"\t". //	パッケージには属性が含まれています  package_contains_identifier
			 //      商品基本情報 - すべての商品に必須の項目
			"\"".$update_delete."\"\t". //	アップデート・削除  update_delete
			"\"".$codabar."\"\t". //	自社で管理しているSKUごとの番号  codabar
			"\"".$heel_type."\"\t". //	ヒールのタイプ  heel_type
			"\"".$model_year."\"\t". //	モデル年(発売年・発表年)  model_year
			"\"".$inner_material_type1."\"\t". //	インナーの素材  inner_material_type
			"\"\"\t". //	バーコード適用除外理由  gtin_exemption_reason
			"\"\"\t". //	版  edition
			"\"".$sole_material."\"\t". //	ソール素材  sole_material
			"\"".$heel_height_unit_of_measure."\"\t". //	ヒールの高さの単位  heel_height_unit_of_measure
			"\"".$heel_height."\"\t". //	ヒールの高さ  heel_height
			"\"\"\t". //	footwear_gender  footwear_gender
			"\"\"\t". //	footwear_width_unisex  footwear_width_unisex
			"\"\"\t". //	footwear_to_size_unisex  footwear_to_size_unisex
			"\"\"\t". //	footwear_gender_unisex  footwear_gender_unisex
			"\"\"\t". //	footwear_size_unisex  footwear_size_unisex
			"\"\"\t". //	ヒール高  heel_height_string
			"\"".$footwear_width."\"\t". //	靴幅  footwear_width
			"\"".$footwear_to_size."\"\t". //	靴サイズの上限値  footwear_to_size
			 //      商品検索情報 - サーチ上で商品を検索されやすくするために必要な項目
			"\"".$generic_keywords."\"\t". //	検索キーワード  generic_keywords
			"\"".$is_autographed."\"\t". //	サイン入り商品  is_autographed
			"\"\"\t". // 	行事  occasion_type1
			"\"\"\t". // 	行事  occasion_type2
			"\"\"\t". // 	行事  occasion_type3
			"\"\"\t". // 	行事  occasion_type4
			"\"\"\t". // 	行事  occasion_type5
			"\"\"\t". // 	スポーツタイプ  sport_type1
			"\"\"\t". // 	スポーツタイプ  sport_type2
			"\"\"\t". // 	スポーツタイプ  sport_type3
			"\"\"\t". //	プレーヤー名  athlete
			"\"\"\t". //	チーム名  team_name
			"\"\"\t". //	コレクション名  collection_name
			"\"".$material_type."\"\t". //	素材の種類  material_type
			"\"".$target_audience_keywords1."\"\t". // 	ターゲットユーザーのキーワード  target_audience_keywords1
			"\"".$target_audience_keywords2."\"\t". // 	ターゲットユーザーのキーワード  target_audience_keywords2
			"\"\"\t". // 	ターゲットユーザーのキーワード  target_audience_keywords3
			"\"\"\t". // 	ターゲットユーザーのキーワード  target_audience_keywords4
			"\"\"\t". // 	ターゲットユーザーのキーワード  target_audience_keywords5
			"\"\"\t". //	毛皮の説明  fur_description
			"\"".$fit_type."\"\t". // 	フィットタイプ  fit_type1
			"\"\"\t". // 	フィットタイプ  fit_type2
			"\"\"\t". // 	フィットタイプ  fit_type3
			"\"\"\t". // 	フィットタイプ  fit_type4
			"\"\"\t". // 	フィットタイプ  fit_type5
			"\"".$collar_style."\"\t". //	襟のタイプ  collar_style
			"\"\"\t". //	織り方  weave_type
			"\"\"\t". //	ポケット記述  pocket_description
			"\"\"\t". //	テーマ  theme
			"\"\"\t". //	ライニングの説明  lining_description
			"\"\"\t". //	リーグ名  league_name
			"\"\"\t". // 	特殊サイズタイプ  special_size_type1
			"\"\"\t". // 	特殊サイズタイプ  special_size_type2
			"\"\"\t". // 	特殊サイズタイプ  special_size_type3
			"\"\"\t". // 	特殊サイズタイプ  special_size_type4
			"\"\"\t". // 	特殊サイズタイプ  special_size_type5
			"\"\"\t". // 	特殊サイズタイプ  special_size_type6
			"\"\"\t". // 	特殊サイズタイプ  special_size_type7
			"\"\"\t". // 	特殊サイズタイプ  special_size_type8
			"\"\"\t". // 	特殊サイズタイプ  special_size_type9
			"\"\"\t". // 	特殊サイズタイプ  special_size_type10
			"\"\"\t". // 	商品の補充情報  lifecycle_supply_type1
			"\"\"\t". // 	商品の補充情報  lifecycle_supply_type2
			"\"\"\t". // 	商品の補充情報  lifecycle_supply_type3
			"\"\"\t". // 	商品の補充情報  lifecycle_supply_type4
			"\"\"\t". // 	商品の補充情報  lifecycle_supply_type5
			"\"".$pattern_name."\"\t". //	パターン名  pattern_name
			"\"\"\t". //	オーダー締切日  item_booking_date
			"\"".$style_keywords."\"\t". // 	スタイルキーワード  style_keywords1
			"\"\"\t". // 	スタイルキーワード  style_keywords2
			"\"\"\t". // 	スタイルキーワード  style_keywords3
			"\"".$seasons."\"\t". //	シーズン  seasons
			"\"".$platinum_keywords1."\"\t". // 	プラチナキーワード  platinum_keywords1
			"\"".$platinum_keywords2."\"\t". // 	プラチナキーワード  platinum_keywords2
			"\"".$platinum_keywords3."\"\t". // 	プラチナキーワード  platinum_keywords3
			"\"".$platinum_keywords4."\"\t". // 	プラチナキーワード  platinum_keywords4
			"\"".$platinum_keywords5."\"\t". // 	プラチナキーワード  platinum_keywords5
			"\"".$specific_uses_keywords1."\"\t". // 	特定用途キーワード  specific_uses_keywords1
			"\"".$specific_uses_keywords2."\"\t". // 	特定用途キーワード  specific_uses_keywords2
			"\"".$specific_uses_keywords3."\"\t". // 	特定用途キーワード  specific_uses_keywords3
			"\"".$specific_uses_keywords4."\"\t". // 	特定用途キーワード  specific_uses_keywords4
			"\"".$specific_uses_keywords5."\"\t". // 	特定用途キーワード  specific_uses_keywords5
			"\"".$shaft_style_type."\"\t". //	シャフトスタイルタイプ  shaft_style_type
			"\"\"\t". //	UV加工  ultraviolet_light_protection
			"\"\"\t". //	要組み立て  is_assembly_required
			"\"\"\t". //	レンズのコーティング  lens_coating_description
			"\"\"\t". //	レンズ色  lens_color
			"\"\"\t". // 	適合車両の種類  compatible_with_vehicle_type1
			"\"\"\t". // 	適合車両の種類  compatible_with_vehicle_type2
			"\"\"\t". // 	適合車両の種類  compatible_with_vehicle_type3
			"\"\"\t". // 	適合車両の種類  compatible_with_vehicle_type4
			"\"\"\t". // 	適合車両の種類  compatible_with_vehicle_type5
			"\"\"\t". // 	適合車両の種類  compatible_with_vehicle_type6
			"\"\"\t". // 	適合車両の種類  compatible_with_vehicle_type7
			"\"\"\t". // 	適合車両の種類  compatible_with_vehicle_type8
			"\"\"\t". // 	適合車両の種類  compatible_with_vehicle_type9
			"\"\"\t". //	箱数  number_of_boxes
			"\"\"\t". // 	同梱商品  included_components1
			"\"\"\t". // 	同梱商品  included_components2
			"\"\"\t". // 	同梱商品  included_components3
			"\"\"\t". // 	同梱商品  included_components4
			"\"\"\t". // 	同梱商品  included_components5
			"\"\"\t". //	商品の個数（ピース数）  number_of_pieces
			"\"\"\t". //	耐水レベル  water_resistance_level
			"\"\"\t". //	シャーシ素材  frame_material_type
			"\"\"\t". //	バッグストラップの高さ  shoulder_strap_drop
			"\"\"\t". //	ショルダーストラップドロップの測定単位  shoulder_strap_drop_unit_of_measure
			"\"\"\t". //	防汚  is_stain_resistant
			"\"\"\t". //	ラベルに記載されている国/地域  country_as_labeled
			"\"\"\t". //	パターンのスタイル  pattern_type
			"\"\"\t". //	出品者カタログ番号  catalog_number
			"\"\"\t". //	メーカー対象最高年齢  mfg_maximum_unit_of_measure
			"\"\"\t". //	組み立て時間の単位  assembly_time_unit_of_measure
			"\"\"\t". //	assembly_time  assembly_time
			"\"\"\t". //	thesaurus_attribute_keywords1
			"\"\"\t". //	thesaurus_attribute_keywords2
			"\"\"\t". //	thesaurus_attribute_keywords3
			"\"\"\t". //	thesaurus_attribute_keywords4
			"\"\"\t". //	thesaurus_attribute_keywords5
			"\"\"\t". //	top_style  top_style
			"\"\"\t". // 	thesaurus_subject_keywords1
			"\"\"\t". // 	thesaurus_subject_keywords2
			"\"\"\t". // 	thesaurus_subject_keywords3
			"\"\"\t". // 	thesaurus_subject_keywords4
			"\"\"\t". // 	thesaurus_subject_keywords5
			"\"\"\t". //	メーカー対象最低年齢  mfg_minimum_unit_of_measure
			"\"\"\t". //	推奨使用用途  recommended_uses_for_product
			"\"\"\t". //	キャスター数  number_of_wheels
			"\"\"\t". //	正味個数単位  net_content_count_unit_of_measure
			"\"\"\t". //    防カビ加工  material_type_free1
			"\"\"\t". //    防カビ加工  material_type_free2
			"\"\"\t". //    防カビ加工  material_type_free3
			"\"\"\t". //    防カビ加工  material_type_free4
			"\"\"\t". //    防カビ加工  material_type_free5
			"\"\"\t". //	正味個数  net_content_count
			"\"\"\t". //	利き手  hand_orientation
			"\"\"\t". //	股上のスタイル  rise_style
			"\"\"\t". //	フロントスタイル  front_style
			"\"\"\t". //	ノイズ制御  noise_control
			"\"\"\t". //	ノイズ減衰単位  noise_attenuation_unit_of_measure
			"\"\"\t". //	ノイズ減衰値  noise_attenuation
			"\"\"\t". //	クラスタイプ  clasp_type
			"\"\"\t". // 	装飾機能  embellishment_feature1
			"\"\"\t". // 	装飾機能  embellishment_feature2
			"\"\"\t". // 	装飾機能  embellishment_feature3
			"\"\"\t". // 	装飾機能  embellishment_feature4
			"\"\"\t". // 	装飾機能  embellishment_feature5
			"\"\"\t". //	背面のスタイル  back_style
			"\"\"\t". //	ハンドル素材  handle_material
			"\"\"\t". //	シャフトの円周  shaft_circumference
			"\"\"\t". //	シャフトの円周の単位  shaft_circumference_unit_of_measure
			"\"\"\t". //	つま先のスタイル  toe_style
			"\"\"\t". //	ベルトの種類  belt_style
			"\"\"\t". //	抗菌加工  item_styling
			"\"\"\t". //	商品の利点  product_benefit
			"\"\"\t". //	ワット数の単位  wattage_unit_of_measure
			"\"\"\t". //	グリップタイプ  grip_type
			"\"\"\t". //	ワット数  wattage
			"\"\"\t". //	土踏まずの特徴  arch_type
			//      推奨ブラウズノード別の情報 - これらの項目は、より豊富な商品情報をカスタマーに提供するためのものです。選択した推奨ブラウズノードにより、必要となる項目が異なります。
			"\"\"\t". //	キャラクター  subject_character
			"\"".$sleeve_type."\"\t". //	袖のタイプ  sleeve_type
			"\"\"\t". //	ファブリックウォッシュ  fabric_wash
			"\"".$strap_type."\"\t". //	ストラップのタイプ  strap_type
			"\"".$toe_shape."\"\t". //	つま先の形状(トゥシェープ)  toe_shape
			"\"".$waist_style."\"\t". //	ウエストスタイル  waist_style
			"\"".$opacity."\"\t". //	不透明度  opacity
			"\"".$bottom_style."\"\t". //	ボトムススタイル  bottom_style
			"\"".$fit_to_size_description."\"\t". //	収納可能サイズ  fit_to_size_description
			"\"\"\t". //	サブブランド  sub_brand_name
			"\"\"\t". //	内蔵ライト  has_builtin_light
			"\"\"\t". //	表示サイズ  size_info_display_name
			"\"\"\t". //	underwire_type  underwire_type
			"\"\"\t". //	opacity_unit_of_measure  opacity_unit_of_measure
			"\"\"\t". //	leg_style  leg_style
			"\"\"\t". //	mfg_maximum  mfg_maximum
			"\"\"\t". //	プレーヤー数  number_of_players
			"\"\"\t". //	mfg_minimum  mfg_minimum
			"\"\"\t". //	ポケットの数  number_of_pockets
			"\"\"\t". //	正味容積単位  net_content_volume_unit_of_measure
			"\"\"\t". //	正味重量単位  net_content_weight_unit_of_measure
			"\"\"\t". //	正味の長さ  net_content_length
			"\"\"\t". //	正味の長さの単位  net_content_length_unit_of_measure
			"\"\"\t". //	正味面積  net_content_area
			"\"\"\t". //	正味重量  net_content_weight
			"\"\"\t". //	正味面積単位  net_content_area_unit_of_measure
			"\"\"\t". //	正味容積  net_content_volume
			"\"\"\t". //	ジャケットの長さ  jacket_length
			"\"\"\t". //	バンドサイズ  band_size
			"\"\"\t". //	ベルト幅の単位  belt_width_unit_of_measure
			"\"\"\t". //	ベルト幅  belt_width_string
			"\"\"\t". //	最低広告表示価格  map_policy
			"\"\"\t". //	ベルト幅の十進数値  belt_width_derived
			"\"\"\t". //	bra_padding_level  bra_padding_level
			"\"\"\t". //	梱包タイプ  customer_package_type
			"\"\"\t". //	つや消しスタイル  matte_style
			//      寸法 - 商品のサイズや重量を入力する項目
			"\"\"\t". //	商品の幅の単位  item_width_unit_of_measure
			"\"".$chest_size_unit_of_measure."\"\t". //	チェストサイズの単位  chest_size_unit_of_measure
			"\"".$chest_size."\"\t". //	チェストサイズ  chest_size
			"\"\"\t". //	商品の高さの単位  item_height_unit_of_measure
			"\"\"\t". //	えきたいりょう  liquid_volume
			"\"\"\t". //	液量単位  liquid_volume_unit_of_measure
			"\"\"\t". //	商品の長さの単位  item_length_unit_of_measure
			"\"".$shoe_width."\"\t". //	靴の幅  shoe_width
			"\"".$shaft_height."\"\t". //	シャフト(軸)の丈  shaft_height
			"\"".$waist_size_unit_of_measure."\"\t". //	ウエストサイズの単位  waist_size_unit_of_measure
			"\"".$waist_size."\"\t". //	ウエストのサイズ  waist_size
			"\"".$inseam_length_unit_of_measure."\"\t". //	仕立ての長さの単位  inseam_length_unit_of_measure
			"\"".$inseam_length."\"\t". //	股下の長さ  inseam_length
			"\"".$sleeve_length_unit_of_measure."\"\t". //	袖の長さの単位  sleeve_length_unit_of_measure
			"\"".$sleeve_length."\"\t". //	袖の長さ  sleeve_length
			"\"".$neck_size_unit_of_measure."\"\t". //	首のサイズの単位  neck_size_unit_of_measure
			"\"".$neck_size."\"\t". //	首のサイズ  neck_size
			"\"".$website_shipping_weight_unit_of_measure."\"\t". //	配送重量の単位  website_shipping_weight_unit_of_measure
			"\"".$website_shipping_weight."\"\t". //	配送重量  website_shipping_weight
			"\"".$shaft_height_unit_of_measure."\"\t". //	シャフトの高さの測定単位  shaft_height_unit_of_measure
			"\"".$item_width."\"\t". //	項目幅  item_width
			"\"".$item_height."\"\t". //	項目の高さ  item_height
			"\"".$shoe_width_unit_of_measure."\"\t". //	靴幅  shoe_width_unit_of_measure
			"\"".$item_length."\"\t". //	品目の長さ  item_length
			"\"\"\t". //	商品の重量（パッケージを含まない）  item_display_weight
			"\"\"\t". //	商品の重量の単位  item_display_weight_unit_of_measure
			"\"".$item_dimensions_unit_of_measure."\"\t". //	商品の寸法の単位  item_dimensions_unit_of_measure
			"\"".$cup_size."\"\t". //	カップサイズ  cup_size
			"\"\"\t". //	長さ  item_display_length
			"\"\"\t". //	商品の長さの単位  item_display_length_unit_of_measure
			"\"\"\t". //	幅  item_display_width
			"\"\"\t". //	商品の高さの単位  item_display_height_unit_of_measure
			"\"\"\t". //	商品の幅の単位  item_display_width_unit_of_measure
			"\"\"\t". //	容量の単位  capacity_unit_of_measure
			"\"\"\t". //	商品の高さ  item_display_height
			"\"\"\t". //	ディスプレイサイズ  display_size
			"\"\"\t". //	ディスプレイ最大解像度の単位  display_size_unit_of_measure
			"\"\"\t". //	性能・容量  capacity
			"\"\"\t". //	leg_diameter_unit_of_measure  leg_diameter_unit_of_measure
			"\"\"\t". //	推奨最大重量の単位  maximum_weight_recommendation_unit_of_measure
			"\"\"\t". //	メーカー推奨最大重量  maximum_weight_recommendation
			"\"\"\t". //	leg_diameter  leg_diameter
			"\"\"\t". //	商品の形状  item_shape
			"\"\"\t". //	ストレージ容量の測定単位  storage_volume_unit_of_measure
			"\"\"\t". //	ストレージボリューム  storage_volume
			"\"\"\t". //	商品の形状  item_form
			"\"\"\t". //	高さマップ  height_map
			"\"\"\t". //	股上の高さ  rise_height
			"\"\"\t". //	股上の高さの単位  rise_height_unit_of_measure
			"\"\"\t". //	伸びた長さの測定単位  extended_length_unit_of_measure
			"\"\"\t". //	拡張された長さ  extended_length
			"\"\"\t". //	minimum_neck_size_unit_of_measure  minimum_neck_size_unit_of_measure
			"\"\"\t". //	ヘッドサイズ名  head_size_name
			//      出荷関連情報 - フルフィルメント by Amazon (FBA) の利用、あるいは自社出荷の注文に関する出荷関連情報を、この項目に記入してください。FBA を利用する場合には必須の項目。
			"\"\"\t". //	商品パッケージの長さの単位  package_length_unit_of_measure
			"\"\"\t". //	長さ  package_length
			"\"\"\t". //	幅  package_width
			"\"\"\t". //	高さ  package_height
			"\"\"\t". //	商品パッケージの重量の単位  package_weight_unit_of_measure
			"\"\"\t". //	商品パッケージの重量  package_weight
			"\"\"\t". //	フルフィルメントセンターID  fulfillment_center_id
			"\"\"\t". //	商品パッケージの高さの単位  package_height_unit_of_measure
			"\"\"\t". //	商品パッケージの幅の単位  package_width_unit_of_measure
			"\"\"\t". //	商品パッケージの寸法の単位  package_dimensions_unit_of_measure
			//      コンプライアンス情報 - 商品を販売する国または地域の特定商取引に遵守するために利用される項目
			"\"".$batteries_required."\"\t". //	この商品は電池本体ですか？または電池を使用した商品ですか？  batteries_required
			"\"".$are_batteries_included."\"\t". //	電池付属  are_batteries_included
			"\"".$battery_cell_composition."\"\t". //	電池の組成  battery_cell_composition
			"\"".$battery_type1."\"\t". // 	電池の種類、サイズ  battery_type1
			"\"".$battery_type2."\"\t". // 	電池の種類、サイズ  battery_type2
			"\"".$battery_type3."\"\t". // 	電池の種類、サイズ  battery_type3
			"\"".$number_of_batteries1."\"\t". // 	電池の数  number_of_batteries1
			"\"".$number_of_batteries2."\"\t". // 	電池の数  number_of_batteries2
			"\"".$number_of_batteries3."\"\t". // 	電池の数  number_of_batteries3
			"\"\"\t". //!!	電池の重量(グラム)  battery_weight
			"\"\"\t". //!!	電池の重量の測定単位  battery_weight_unit_of_measure
			"\"".$number_of_lithium_metal_cells."\"\t". //	リチウムメタル電池単数  number_of_lithium_metal_cells
			"\"".$number_of_lithium_ion_cells."\"\t". //	リチウムイオン電池単数  number_of_lithium_ion_cells
			"\"".$lithium_battery_packaging."\"\t". //	リチウム電池パッケージ  lithium_battery_packaging
			"\"".$lithium_battery_energy_content."\"\t". //	電池当たりのワット時  lithium_battery_energy_content
			"\"".$lithium_battery_energy_content_unit_of_measure."\"\t". //	リチウム電池のエネルギー量測定単位  lithium_battery_energy_content_unit_of_measure
			"\"".$lithium_battery_weight."\"\t". //	リチウム含有量(グラム)  lithium_battery_weight
			"\"".$lithium_battery_weight_unit_of_measure."\"\t". //	リチウム電池の重量の測定単位  lithium_battery_weight_unit_of_measure
			"\"\"\t". //!! 	適用される危険物関連法令  supplier_declared_dg_hz_regulation1
			"\"\"\t". //!! 	適用される危険物関連法令  supplier_declared_dg_hz_regulation2
			"\"\"\t". //!! 	適用される危険物関連法令  supplier_declared_dg_hz_regulation3
			"\"\"\t". //!! 	適用される危険物関連法令  supplier_declared_dg_hz_regulation4
			"\"\"\t". //!! 	適用される危険物関連法令  supplier_declared_dg_hz_regulation5
			"\"\"\t". //!!	国連(UN)番号  hazmat_united_nations_regulatory_id
			"\"\"\t". //!!	安全データシート(SDS) URL  safety_data_sheet_url
			"\"".$item_weight."\"\t". //	商品の重量  item_weight
			"\"".$item_weight_unit_of_measure."\"\t". //	商品の重量の単位  item_weight_unit_of_measure
			"\"".$item_volume."\"\t". //	商品の体積  item_volume
			"\"".$item_volume_unit_of_measure."\"\t". //	商品の容量の単位  item_volume_unit_of_measure
			"\"".$country_of_origin."\"\t". //	原産国/地域  country_of_origin
			"\"\"\t". //	商品に適用される保管規制の区分  fulfillment_center_special_storage
			"\"\"\t". //	引火点(°C)  flash_point
			"\"\"\t". //!! 	分類／危険物ラベル(適用されるものをすべて選択)  ghs_classification_class1
			"\"\"\t". //!! 	分類／危険物ラベル(適用されるものをすべて選択)  ghs_classification_class2
			"\"\"\t". //!! 	分類／危険物ラベル(適用されるものをすべて選択)  ghs_classification_class3
			"\"\"\t". //	内用液は含まれていますか?  contains_liquid_contents
			"\"\"\t". //	メーカー保証の説明  warranty_description
			"\"\"\t". //	CPSIA警告の説明  cpsia_cautionary_description
			"\"\"\t". //	並行輸入品  import_designation
			"\"\"\t". // 	CPSIA警告  cpsia_cautionary_statement1
			"\"\"\t". // 	CPSIA警告  cpsia_cautionary_statement2
			"\"\"\t". // 	CPSIA警告  cpsia_cautionary_statement3
			"\"\"\t". // 	CPSIA警告  cpsia_cautionary_statement4
			"\"\"\t". //	法規上の免責条項  legal_disclaimer_description
			"\"\"\t". //	安全上のお知らせ  safety_warning
			"\"\"\t". //	external_testing_certification  external_testing_certification
			//       出品情報 - 商品をサイト上で販売可能にする際に必要な項目
			"\"".$fulfillment_latency."\"\t". //	出荷作業日数  fulfillment_latency
			"\"".$condition_type."\"\t". //	コンディション  condition_type
			"\"\"\t". //	取り寄せ  is_sourcing_on_demand
			"\"".$condition_note."\"\t". //	商品のコンディション説明  condition_note
			"\"\"\t". //	通貨コード  currency
			"\"".$merchant_release_date."\"\t". //	予約商品の販売開始日  merchant_release_date
			"\"".$list_price."\"\t". //	メーカー希望価格  list_price
			"\"".$optional_payment_type_exclusion."\"\t". //	使用しない支払い方法  optional_payment_type_exclusion
			"\"".$sale_price."\"\t". //	セール価格  sale_price
			"\"".$sale_from_date."\"\t". //	セール開始日  sale_from_date
			"\"".$sale_end_date."\"\t". //	セール終了日  sale_end_date
			"\"".$restock_date."\"\t". //	商品の入荷予定日  restock_date
			"\"".$max_order_quantity."\"\t". //	最大注文個数  max_order_quantity
			"\"".$offering_can_be_gift_messaged."\"\t". //	ギフトメッセージ  offering_can_be_gift_messaged
			"\"".$offering_can_be_giftwrapped."\"\t". //	ギフト包装  offering_can_be_giftwrapped
			"\"".$merchant_shipping_group_name."\"\t". //	配送パターン  merchant_shipping_group_name
			"\"\"\t". //	地図価格  map_price
			"\"".$distribution_designation."\"\t". //	販売形態(並行輸入品)  distribution_designation
			"\"".$offering_end_date."\"\t". //	終了日を提案する  offering_end_date
			"\"".$offering_start_date."\"\t". //	開始日を提案する  offering_start_date
			"\"".$product_tax_code."\"\t". //	商品タックスコード  product_tax_code
			"\"".$standard_points_percent."\"\t". //	ポイントパーセント  standard_price_points_percent
			"\"".$sale_points_percent."\"\t". //	セール時ポイントパーセント  sale_price_points_percent
			"\"".$product_site_launch_date."\"\t". //	商品の公開日  product_site_launch_date
			"\"".$delivery_schedule_group_id."\"\t". //	配送日時指定SKUリスト  delivery_schedule_group_id
			"\"".$is_discontinued_by_manufacturer."\"\t". //	メーカー製造中止  is_discontinued_by_manufacturer
			"\"\"\t". //	同梱発送時の最大個数  max_aggregate_ship_quantity
			"\"\"\t". //	メーカー希望価格  list_price_with_tax
			"\"\"\t". //	ポイント  standard_price_points
			"\"\"\t". //	セール時ポイント  sale_price_points
			"\n";
		} else {

			//	add ohkawara 2019/12/13
			//	商品タイプ(必須)
			$feed_product_type = "sportinggoods";


			//	スポーツ
			$tsv.="\"".$feed_product_type."\"\t". //	商品タイプ feed_product_type
			"\"".$item_sku."\"\t". //	出品者SKU item_sku
			"\"".$external_product_id."\"\t". //	商品コード(JANコード等) external_product_id
			"\"".$external_product_id_type."\"\t". //	商品コードのタイプ external_product_id_type
			"\"".$item_name."\"\t". //	商品名 item_name
			"\"".$department_name."\"\t". //	対象年齢・性別 department_name
			"\"".$standard_price."\"\t". //	商品の販売価格 standard_price
			"\"".$quantity."\"\t". //	在庫数 quantity
			"\"".$main_image_url."\"\t". //	商品メイン画像URL main_image_url
			"\"".$brand_name."\"\t". //	ブランド名 brand_name
			"\"".$manufacturer."\"\t". //	メーカー名 manufacturer
			"\"".$recommended_browse_nodes."\"\t". //	推奨ブラウズノード recommended_browse_nodes
			"\"\"\t". //!!	前から後ろまでのアイテムの奥行き depth_front_to_back
			"\"\"\t". //!!	品目の奥行きの単位 depth_front_to_back_unit_of_measure
			"\"\"\t". //!!	品目の左右の幅 depth_width_side_to_side
			"\"\"\t". //!!	品目の幅単位 depth_width_side_to_side_unit_of_measure
			"\"\"\t". //!!	品目の底部から上部までの高さ depth_height_floor_to_top
			"\"\"\t". //!!	品目の高さ単位 depth_height_floor_to_top_unit_of_measure
			"\"\"\t". //	お客様の性別 target_gender
			"\"\"\t". //!!	unit_count unit_count
			"\"\"\t". //!!	unit_count_type unit_count_type
			"\"\"\t". //	お客様の年齢層 age_range_description
			"\"".$size_name."\"\t". //	サイズ size_name
			"\"\"\t". //!!	品目の上端から下端までの長さ length_longer_edge
			"\"\"\t". //!!	品目の長さ単位 length_longer_edge_unit_of_measure
			"\"\"\t". //!!	品目の左右の幅 width_shorter_edge
			"\"\"\t". //!!	幅単位 width_shorter_edge_unit_of_measure
			"\"".$color_name."\"\t". //	カラー color_name
			"\"\"\t". //!!	賞味期限管理商品 is_expiration_dated_product
			"\"\"\t". //!!	品目の上端から下端までの長さ thickness_head_to_toe
			"\"\"\t". //!!	品目の長さ単位 thickness_head_to_toe_unit_of_measure
			"\"\"\t". //!!	品目の左右の幅 thickness_width_side_to_side
			"\"\"\t". //!!	品目の幅単位 thickness_width_side_to_side_unit_of_measure
			"\"\"\t". //!!	品目の厚さ thickness_floor_to_top
			"\"\"\t". //!!	品目の厚さ単位 thickness_floor_to_top_unit_of_measure
			"\"\"\t". //!!	組み立て時の長辺の長さ length_head_to_toe
			"\"\"\t". //!!	長さ単位 length_head_to_toe_unit_of_measure
			"\"\"\t". //!!	組み立て時の短辺の幅 length_width_side_to_side
			"\"\"\t". //!!	幅単位 length_width_side_to_side_unit_of_measure
			"\"\"\t". //!!	組み立て時のベースからトップまでの高さ length_height_floor_to_top
			"\"\"\t". //!!	高さの単位 length_height_floor_to_top_unit_of_measure
			"\"\"\t". //!!	商品の直径 item_diameter_derived
			"\"\"\t". //!!	商品の直径の単位 item_diameter_unit_of_measure
			"\"\"\t". //!!	item_width_numeric item_width_numeric
			"\"\"\t". //!!	item_width_numeric_unit_of_measure item_width_numeric_unit_of_measure
			"\"\"\t". //!!	品目の左右の幅 height_width_side_to_side
			"\"\"\t". //!!	品目の幅単位 height_width_side_to_side_unit_of_measure
			"\"\"\t". //!!	底部から上部までの高さ height_floor_top
			"\"\"\t". //!!	品目の高さ単位 height_floor_top_unit_of_measure
			"\"".$other_image_url1."\"\t". //	商品のサブ画像URL1 other_image_url1
			"\"".$other_image_url2."\"\t". //	商品のサブ画像URL1 other_image_url2
			"\"".$other_image_url3."\"\t". //	商品のサブ画像URL1 other_image_url3
			"\"".$other_image_url4."\"\t". //	商品のサブ画像URL1 other_image_url4
			"\"".$other_image_url5."\"\t". //	商品のサブ画像URL1 other_image_url5
			"\"".$other_image_url6."\"\t". //	商品のサブ画像URL1 other_image_url6
			"\"".$other_image_url7."\"\t". //	商品のサブ画像URL1 other_image_url7
			"\"".$other_image_url8."\"\t". //	商品のサブ画像URL1 other_image_url8
			"\"".$swatch_image_url."\"\t". //	カラーサンプル画像URL swatch_image_url
			"\"".$parent_child."\"\t". //	親子関係の指定 parent_child
			"\"".$parent_sku."\"\t". //	親商品のSKU(商品管理番号) parent_sku
			"\"".$relationship_type."\"\t". //	子関係タイプ relationship_type
			"\"".$variation_theme."\"\t". //	バリエーションテーマ variation_theme
			"\"\"\t". //	パッケージレベル package_level
			"\"\"\t". //	パッケージ内包数量 package_contains_quantity
			"\"\"\t". //	パッケージには属性が含まれています package_contains_identifier
			"\"\"\t". //	アップデート・削除 update_delete
			"\"".$product_description."\"\t". //	商品説明文 product_description
			"\"".$part_number."\"\t". //	メーカー型番 part_number
			"\"\"\t". //	リールの型式名 model_name
			"\"\"\t". //	モデル model
			"\"\"\t". //	バーコード適用除外理由 gtin_exemption_reason
			"\"\"\t". //	自社で管理しているSKUごとの番号 codabar
			"\"\"\t". //	発売年・モデルイヤー model_year
			"\"\"\t". //	梱包タイプ closure_type
			"\"\"\t". //	表地素材 outer_material_type
			"\"\"\t". //	インナーの素材 inner_material_type
			"\"\"\t". //	使用する燃料タイプ fuel_type
			"\"\"\t". //	ヒールのタイプ heel_type
			"\"\"\t". //	ヒールの高さの単位 heel_height_unit_of_measure
			"\"\"\t". //	ソール材料 sole_material
			"\"\"\t". //	ヒールの高さ heel_height
			"\"\"\t". //	温度の定格 temperature_rating
			"\"\"\t". //	お手入れ方法 care_instructions1
			"\"\"\t". //	お手入れ方法 care_instructions2
			"\"\"\t". //	お手入れ方法 care_instructions3
			"\"\"\t". //	お手入れ方法 care_instructions4
			"\"".$bullet_point1."\"\t". // 	商品の仕様 bullet_point1
			"\"".$bullet_point2."\"\t". // 	商品の仕様 bullet_point2
			"\"".$bullet_point3."\"\t". // 	商品の仕様 bullet_point3
			"\"".$bullet_point4."\"\t". // 	商品の仕様 bullet_point4
			"\"".$bullet_point5."\"\t". // 	商品の仕様 bullet_point5
			"\"\"\t". // 	商品の仕様 bullet_point6
			"\"\"\t". // 	商品の仕様 bullet_point7
			"\"\"\t". // 	商品の仕様 bullet_point8
			"\"\"\t". // 	商品の仕様 bullet_point9
			"\"\"\t". // 	商品の仕様 bullet_point10
			"\"".$style_name."\"\t". //	スタイル名 style_name
			"\"".$generic_keywords."\"\t". //	検索キーワード generic_keywords
			"\"".$color_map."\"\t". //	カラーマップ color_map
			"\"".$material_type."\"\t". //	素材の種類 material_type
			"\"\"\t". // 	シーズン seasons1
			"\"\"\t". // 	シーズン seasons2
			"\"\"\t". // 	シーズン seasons3
			"\"\"\t". // 	シーズン seasons4
			"\"\"\t". // 	スポーツの名称 sport_type1
			"\"\"\t". // 	スポーツの名称 sport_type2
			"\"".$golf_club_loft."\"\t". //	ゴルフグラブのロフト（クラブフェースの傾斜角度） golf_club_loft
			"\"".$grip_type."\"\t". //	グリップタイプ grip_type
			"\"".$lens_color."\"\t". //	レンズの色 lens_color
			"\"\"\t". //	レンズの素材 lens_material_type
			"\"\"\t". //	サイン入り商品 is_autographed
			"\"\"\t". //	リーグ名 league_name
			"\"".$hand_orientation."\"\t". //	ハンド hand_orientation
			"\"".$shaft_length_unit_of_measure."\"\t". //	シャフト(軸)の素材 shaft_material
			"\"\"\t". //	プレーヤー名 athlete
			"\"".$team_name."\"\t". //	チーム名 team_name
			"\"\"\t". // 	箱の中身 included_components1
			"\"\"\t". // 	箱の中身 included_components2
			"\"\"\t". // 	箱の中身 included_components3
			"\"\"\t". // 	箱の中身 included_components4
			"\"\"\t". // 	箱の中身 included_components5
			"\"\"\t". // 	互換デバイス compatible_devices1
			"\"\"\t". // 	互換デバイス compatible_devices2
			"\"\"\t". // 	互換デバイス compatible_devices3
			"\"\"\t". // 	互換デバイス compatible_devices4
			"\"\"\t". // 	互換デバイス compatible_devices5
			"\"\"\t". //	毛皮の説明 fur_description
			"\"\"\t". //	golf_club_loft_unit_of_measure golf_club_loft_unit_of_measure
			"\"".$seating_group."\"\t". //	席種 seating_group
			"\"\"\t". //	商品種別 item_type_name
			"\"\"\t". //	オプション option_type
			"\"".$target_audience_base."\"\t". //	対象 target_audience_base
			"\"\"\t". //	この商品はAmazon.co.jp限定商品ですか？ is_exclusive_product
			"\"\"\t". //	箱数 number_of_boxes
			"\"\"\t". //	グローブのタイプ glove_type
			"\"\"\t". //	防水タイプか否か water_resistance_level
			"\"\"\t". //	ライニングの説明 lining_description
			"\"\"\t". //	BMXバイクのタイプ bike_type
			"\"\"\t". //	フレームの素材 frame_material_type
			"\"\"\t". //	設定数 num_of_settings
			"\"".$target_audience_keywords1."\"\t". // 	ターゲットユーザーのキーワード target_audience_keywords1
			"\"".$target_audience_keywords2."\"\t". // 	ターゲットユーザーのキーワード target_audience_keywords2
			"\"\"\t". // 	ターゲットユーザーのキーワード target_audience_keywords3
			"\"\"\t". // 	ターゲットユーザーのキーワード target_audience_keywords4
			"\"\"\t". // 	ターゲットユーザーのキーワード target_audience_keywords5
			"\"".$pattern_name."\"\t". //	デザイン pattern_name
			"\"".$style_keywords."\"\t". //	スタイルキーワード style_keywords
			"\"\"\t". //	シャフトスタイルタイプ shaft_style_type
			"\"".$lithium_battery_voltage."\"\t". //	リチウム電池ボルト数 lithium_battery_voltage
			"\"\"\t". // 	プラチナキーワード platinum_keywords1
			"\"\"\t". // 	プラチナキーワード platinum_keywords2
			"\"\"\t". // 	プラチナキーワード platinum_keywords3
			"\"\"\t". // 	プラチナキーワード platinum_keywords4
			"\"\"\t". // 	プラチナキーワード platinum_keywords15
			"\"\"\t". //	lithium_battery_voltage_unit_of_measure lithium_battery_voltage_unit_of_measure
			"\"\"\t". // 	ゴルフパターのライ角の単位 golf_putter_lie_angle_unit1
			"\"\"\t". // 	ゴルフパターのライ角の単位 golf_putter_lie_angle_unit2
			"\"\"\t". // 	ゴルフパターのライ角の単位 golf_putter_lie_angle_unit3
			"\"\"\t". // 	ゴルフパターのライ角の単位 golf_putter_lie_angle_unit4
			"\"\"\t". // 	ゴルフパターのライ角の単位 golf_putter_lie_angle_unit5
			"\"\"\t". //	メーカー対象最高年齢 mfg_maximum_unit_of_measure
			"\"\"\t". //	商品のオフセットの単位 item_offset_unit_of_measure
			"\"\"\t". //	オフセット item_offset
			"\"\"\t". // 	パターのライ角を指定する golf_putter_lie_angle1
			"\"\"\t". // 	パターのライ角を指定する golf_putter_lie_angle2
			"\"\"\t". // 	パターのライ角を指定する golf_putter_lie_angle3
			"\"\"\t". // 	パターのライ角を指定する golf_putter_lie_angle4
			"\"\"\t". // 	パターのライ角を指定する golf_putter_lie_angle5
			"\"\"\t". //	メーカー対象最低年齢 mfg_minimum_unit_of_measure
			"\"\"\t". //	ポケットの説明 pocket_description
			"\"\"\t". //	ウェイトベンチの種類 weight_bench_type
			"\"\"\t". //	対応スキルレベル skill_level
			"\"\"\t". // 	フィットタイプ fit_type1
			"\"\"\t". // 	フィットタイプ fit_type2
			"\"\"\t". // 	フィットタイプ fit_type3
			"\"\"\t". // 	フィットタイプ fit_type4
			"\"\"\t". // 	フィットタイプ fit_type5
			"\"\"\t". //	床の使用面積 floor_area
			"\"\"\t". //	首周りの形状(襟の形状) neck_style
			"\"\"\t". //	パターンのスタイル pattern_type
			"\"\"\t". //	靴下の長さ rise_style
			"\"\"\t". //	テーマ theme
			"\"\"\t". //	トップのスタイル top_style
			"\"\"\t". //	UVプロテクションのレベル ultraviolet_light_protection
			"\"\"\t". //	ホイールタイプ wheel_type
			"\"\"\t". //	フレームタイプ frame_type
			"\"\"\t". //	ブレーキのタイプ brake_style
			"\"\"\t". //	サスペンションのタイプ suspension_type
			"\"\"\t". //	ドアの数 number_of_doors
			"\"\"\t". //	詰め物素材のタイプ fill_material_type
			"\"\"\t". //	商品の据え付けタイプ mounting_type
			"\"\"\t". //	保温レベル temperature_rating_degrees
			"\"\"\t". //	レンズの形状 lens_type
			"\"\"\t". //	絶縁タイプ insulation_material_type
			"\"\"\t". //	パッドのタイプ pad_type
			"\"\"\t". //	ポール数 number_of_poles
			"\"\"\t". //	電球タイプ light_source_type
			"\"\"\t". //	座席数 seating_capacity
			"\"\"\t". //	電源 power_source_type
			"\"\"\t". // 	機能性 special_features1
			"\"\"\t". // 	機能性 special_features2
			"\"\"\t". // 	機能性 special_features3
			"\"\"\t". // 	機能性 special_features4
			"\"\"\t". // 	機能性 special_features5
			"\"\"\t". //	ラップトップ容量 capacity_name
			"\"\"\t". // 	バッテリ寿命 battery_average_life1
			"\"\"\t". // 	バッテリ寿命 battery_average_life2
			"\"\"\t". // 	バッテリ寿命 battery_average_life3
			"\"\"\t". //	magnification_strength magnification_strength
			"\"\"\t". //	wheelbase wheelbase
			"\"\"\t". //	wheelbase_unit_of_measure wheelbase_unit_of_measure
			"\"\"\t". //	shell_type shell_type
			"\"\"\t". //	thesaurus_attribute_keywords thesaurus_attribute_keywords
			"\"\"\t". //	推奨使用用途 recommended_uses_for_product
			"\"\"\t". // 	特定用途キーワード specific_uses_keywords1
			"\"\"\t". // 	特定用途キーワード specific_uses_keywords2
			"\"\"\t". // 	特定用途キーワード specific_uses_keywords3
			"\"\"\t". // 	特定用途キーワード specific_uses_keywords4
			"\"\"\t". // 	特定用途キーワード specific_uses_keywords5
			"\"\"\t". //    thesaurus_subject_keywords1
			"\"\"\t". //    thesaurus_subject_keywords2
			"\"\"\t". //    thesaurus_subject_keywords3
			"\"\"\t". //    thesaurus_subject_keywords4
			"\"\"\t". //    thesaurus_subject_keywords5
			"\"\"\t". //	magnification_strength_unit_of_measure magnification_strength_unit_of_measure
			"\"\"\t". //	ラベルに記載されている国/地域 country_as_labeled
			"\"\"\t". //	釣り技法 fishing_technique
			"\"\"\t". // 	商品の補充情報 lifecycle_supply_type1
			"\"\"\t". // 	商品の補充情報 lifecycle_supply_type2
			"\"\"\t". // 	商品の補充情報 lifecycle_supply_type3
			"\"\"\t". // 	商品の補充情報 lifecycle_supply_type4
			"\"\"\t". // 	商品の補充情報 lifecycle_supply_type5
			"\"\"\t". //	床の使用面積の単位 floor_area_unit_of_measure
			"\"\"\t". //	測定単位 temperature_rating_degrees_unit_of_measure
			"\"\"\t". // 	バッテリ平均持続時間の単位 battery_average_life_unit_of_measure1
			"\"\"\t". // 	バッテリ平均持続時間の単位 battery_average_life_unit_of_measure2
			"\"\"\t". // 	バッテリ平均持続時間の単位 battery_average_life_unit_of_measure3
			"\"\"\t". //	シャフトの円周 shaft_circumference
			"\"\"\t". //	シャフトの円周の単位 shaft_circumference_unit_of_measure
			"\"\"\t". //	つま先のスタイル toe_style
			"\"\"\t". //	ミッドソール素材 midsole_material_type
			"\"\"\t". //	仕切りの数 number_of_dividers
			"\"\"\t". //	銃器の口径 caliber
			"\"\"\t". // 	エアガンパワータイプ air_gun_power_type1
			"\"\"\t". // 	エアガンパワータイプ air_gun_power_type2
			"\"\"\t". // 	エアガンパワータイプ air_gun_power_type3
			"\"\"\t". // 	エアガンパワータイプ air_gun_power_type4
			"\"\"\t". // 	エアガンパワータイプ air_gun_power_type5
			"\"\"\t". //	絶縁抵抗の単位 insulation_resistance_unit_of_measure
			"\"\"\t". //	BTU で測定した絶縁 insulation_resistance
			"\"\"\t". //	組み立て方法 assembly_instructions
			"\"\"\t". //	ディスプレイタイプ display_type
			"\"\"\t". //	運動器具のアームタイプ exercise_machine_arm_type
			"\"\"\t". // 	スペース効率 space_efficiency1
			"\"\"\t". // 	スペース効率 space_efficiency2
			"\"\"\t". //	心拍数センサーのタイプ heart_rate_sensor_type
			"\"\"\t". //	商品の個数（ピース数） number_of_pieces
			"\"\"\t". //	溝深さ単位 tread_depth_unit_of_measure
			"\"\"\t". //	溝深さ tread_depth
			"\"\"\t". //	ロックタイプ lock_type
			"\"\"\t". //	ハンドル素材 handle_material
			"\"\"\t". //	収納時サイズ folded_size
			"\"\"\t". // 	ポール素材タイプ pole_material_type1
			"\"\"\t". // 	ポール素材タイプ pole_material_type2
			"\"\"\t". // 	ポール素材タイプ pole_material_type3
			"\"\"\t". // 	ポール素材タイプ pole_material_type4
			"\"\"\t". // 	ポール素材タイプ pole_material_type5
			"\"\"\t". //	ハンドルの直径 handle_diameter
			"\"\"\t". //	ハンドル長さの単位 handle_length_unit
			"\"\"\t". //	ハンドル長さの十進数値 handle_length_derived
			"\"\"\t". //	ハンドルのタイプ handle_type
			"\"\"\t". //	ホイール素材 wheel_material
			"\"\"\t". //	ホイールの数 number_of_wheels
			"\"\"\t". // 	対象となる動植物 target_species1
			"\"\"\t". // 	対象となる動植物 target_species2
			"\"\"\t". // 	対象となる動植物 target_species3
			"\"\"\t". // 	対象となる動植物 target_species4
			"\"\"\t". // 	対象となる動植物 target_species5
			"\"\"\t". //	チャンバーの深さの単位 chamber_depth_unit_of_measure
			"\"\"\t". //	チャンバーの深さ chamber_depth
			"\"\"\t". //	バンド長 band_length
			"\"\"\t". //	バンド材質タイプ band_material_type
			"\"\"\t". //	バンド色 band_color
			"\"\"\t". //	バンド長の単位 band_length_unit
			"\"\"\t". //	速度測定 speed_rating
			"\"\"\t". //	プログラム数 number_of_programs
			"\"\"\t". //	モーターサイズ maximum_horsepower
			"\"\"\t". //	最大馬力単位 maximum_horsepower_unit_of_measure
			"\"\"\t". //	最低速度の単位 minimum_speed_unit_of_measure
			"\"\"\t". //	組み立てる必要がありますか? is_assembly_required
			"\"\"\t". //	最大速度の単位 maximum_speed_unit_of_measure
			"\"\"\t". //	最大速度 maximum_speed
			"\"\"\t". //	組み立て時間の単位 assembly_time_unit_of_measure
			"\"\"\t". //	電源プラグ power_plug_type
			"\"\"\t". //	組み立て時間 assembly_time
			"\"\"\t". //	最低速度 minimum_speed
			"\"\"\t". //	動作モード operation_mode
			"\"\"\t". //	気流量 air_flow_capacity
			"\"\"\t". //	空気流量の測定単位 air_flow_capacity_unit_of_measure
			"\"\"\t". // 	適合車両の種類 compatible_with_vehicle_type1
			"\"\"\t". // 	適合車両の種類 compatible_with_vehicle_type2
			"\"\"\t". // 	適合車両の種類 compatible_with_vehicle_type3
			"\"\"\t". // 	適合車両の種類 compatible_with_vehicle_type4
			"\"\"\t". // 	適合車両の種類 compatible_with_vehicle_type5
			"\"\"\t". // 	適合車両の種類 compatible_with_vehicle_type6
			"\"\"\t". // 	適合車両の種類 compatible_with_vehicle_type7
			"\"\"\t". // 	適合車両の種類 compatible_with_vehicle_type8
			"\"\"\t". // 	適合車両の種類 compatible_with_vehicle_type9
			"\"".$golf_club_flex."\"\t". //	ゴルフクラブのシャフトの硬さ golf_club_flex
			"\"".$tension_level."\"\t". //	張力（テンション） tension_level
			"\"\"\t". //	代金引換 is_cod_included
			"\"\"\t". //	FCの保存可能期間の単位 fc_shelf_life_unit_of_measure
			"\"".$shipping_options."\"\t". //	配送オプション shipping_options
			"\"\"\t". //	コンビニ決済 is_cvs_included
			"\"\"\t". //	レザーの種類 leather_type
			"\"\"\t". //	標準抵抗メカニズムの値 resistance_mechanism
			"\"".$grip_material_type."\"\t". //	グリップ素材タイプ grip_material_type
			"\"".$fishing_line_type."\"\t". //	釣り糸素材のタイプ fishing_line_type
			"\"\"\t". //	軸径の十進数値 shaft_diameter_derived
			"\"\"\t". //	軸径の単位 shaft_diameter_unit_of_measure
			"\"\"\t". //	シャフト素材タイプ shaft_material_type
			"\"\"\t". //	メーカー推奨最高年齢 mfg_maximum
			"\"\"\t". //	構成 configuration
			"\"\"\t". //	メーカー推奨最少年齢 mfg_minimum
			"\"\"\t". //	ポケットの数 number_of_pockets
			"\"\"\t". //	Item Depth Unit Of Measure item_depth_unit_of_measure
			"\"\"\t". //	スラット付きフレーム調整タイプ adjustment_type
			"\"\"\t". //	調節可能な長さ has_adjustable_length
			"\"\"\t". //	ストラップのタイプ strap_type
			"\"\"\t". //	構造 construction_type
			"\"\"\t". //	ボトムのタイプ bottom_style
			"\"\"\t". //	脚のスタイル leg_style
			"\"\"\t". //	袖のタイプ sleeve_type
			"\"\"\t". //	サポートタイプ support_type
			"\"\"\t". //	線の太さ line_weight
			"\"\"\t". //	速度の数 number_of_speeds
			"\"\"\t". //	レベル数 number_of_power_levels
			"\"\"\t". //	装具タイプ fitting_type
			"\"\"\t". //	ギアループの数 number_of_gear_loops
			"\"\"\t". //	最小胴体適合サイズ minimum_torso_measurement
			"\"\"\t". //	就寝人数 occupancy
			"\"\"\t". //	最大倍率 magnification_maximum
			"\"\"\t". //	使用容量の単位 capacity_name_unit_of_measure
			"\"\"\t". //	台座タイプ strand_type
			"\"\"\t". //	反射技術 reflecting_technology
			"\"\"\t". //	base_material_type base_material_type
			"\"\"\t". // 	仕上げ finish_type1
			"\"\"\t". // 	仕上げ finish_type2
			"\"\"\t". //	シャフト長（10進値） shaft_length_derived
			"\"\"\t". //	取付穴の単位 mounting_hole_diameter_unit_of_measure
			"\"\"\t". //	取り付け穴の直径の十進数値 mounting_hole_diameter_derived
			"\"\"\t". //	ライフベストのタイプ life_vest_type
			"\"\"\t". //	インソール素材 insole_material_type
			"\"\"\t". // 	防しわ性 wrinkle_resistance1
			"\"\"\t". // 	防しわ性 wrinkle_resistance2
			"\"\"\t". // 	防しわ性 wrinkle_resistance3
			"\"\"\t". // 	防しわ性 wrinkle_resistance4
			"\"\"\t". // 	防しわ性 wrinkle_resistance5
			"\"\"\t". //	legal_compliance_certification_certifying_authority_name legal_compliance_certification_certifying_authority_name
			"\"\"\t". //	legal_compliance_certification_geographic_jurisdiction legal_compliance_certification_geographic_jurisdiction
			"\"\"\t". //	ラウンド数 number_of_rounds
			"\"\"\t". //	バレル素材タイプ barrel_material_type
			"\"\"\t". //	品種 variety
			"\"\"\t". //	抵抗レベル数 number_of_resistance_levels
			"\"\"\t". //	駆動システム drive_system
			"\"\"\t". //	トレッドタイプ tread_type
			"\"\"\t". //	タイヤタイプ tire_type
			"\"\"\t". //	レイヤ数 number_of_layers
			"\"\"\t". //	耐水技術 water_resistance_technology
			"\"\"\t". //	材料の特徴 material_features
			"\"\"\t". //	ケーブル長（10進値） cable_length_derived
			"\"\"\t". //	十字穴番号 screw_point_style
			"\"\"\t". //	期間の単位 duration_unit_of_measure
			"\"\"\t". //	期間 duration
			"\"\"\t". //	バンド長 band_size
			"\"\"\t". //	引張バネの数 number_of_springs
			"\"\"\t". //	ベアリング材質のタイプ bearing_material_type
			"\"\"\t". //	変速機減速比 gearbox_ratio
			"\"\"\t". //	デュロメーターの硬さ durometer_hardness
			"\"\"\t". //	トレッドミルの最大傾斜 maximum_incline_percentage
			"\"\"\t". //	サイクルタイプ motor_type
			"\"\"\t". //	ベルト長派生 belt_length_derived
			"\"\"\t". //	長ベルト長の測定単位 belt_length_unit_of_measure
			"\"\"\t". //	ベルト幅の単位 belt_width_unit_of_measure
			"\"\"\t". //	belt_length_string belt_length_string
			"\"\"\t". //	ベルト幅 belt_width_string
			"\"\"\t". //	ベルト幅の十進数値 belt_width_derived
			"\"".$size_map."\"\t". //	サイズマップ size_map
			"\"".$grip_size."\"\t". //	グリップサイズ grip_size
			"\"".$volume_capacity_name."\"\t". //	定格内容積（総容積） volume_capacity_name
			"\"".$shaft_length."\"\t". //	シャフト(軸)の長さ shaft_length
			"\"".$shaft_length_unit_of_measure."\"\t". //	シャフト(軸)の長さの単位 shaft_length_unit_of_measure
			"\"".$item_shape."\"\t". //	形状 item_shape
			"\"".$display_maximum_weight_recommendation."\"\t". //	対応重量 display_maximum_weight_recommendation
			"\"".$display_maximum_weight_recommendation_unit_of_measure."\"\t". //	対応重量の単位 display_maximum_weight_recommendation_unit_of_measure
			"\"".$item_display_length."\"\t". //	ロッド(棒)の長さ item_display_length
			"\"".$item_display_length_unit_of_measure."\"\t". //	ロッド(棒)の長さの単位 item_display_length_unit_of_measure
			"\"".$item_display_width."\"\t". //	幅 item_display_width
			"\"".$item_display_width_unit_of_measure."\"\t". //	幅の単位 item_display_width_unit_of_measure
			"\"".$item_display_weight."\"\t". //	商品の重量（パッケージを含まない） item_display_weight
			"\"".$item_display_weight_unit_of_measure."\"\t". //	ロッド(棒)の重量の単位 item_display_weight_unit_of_measure
			"\"\"\t". //	ヘッドサイズ名 head_size_name
			"\"\"\t". //	商品の高さの単位 item_height_unit_of_measure
			"\"\"\t". //	液体の容量 liquid_volume
			"\"\"\t". //	液体量の単位 liquid_volume_unit_of_measure
			"\"\"\t". //	商品本体サイズ：長さ item_length
			"\"\"\t". //	商品の幅の単位 item_width_unit_of_measure
			"\"\"\t". //	容積容量名の単位 volume_capacity_name_unit_of_measure
			"\"\"\t". //	商品本体サイズ：幅 item_width
			"\"\"\t". //	商品本体サイズ：高さ item_height
			"\"\"\t". //	商品の長さの単位 item_length_unit_of_measure
			"\"".$wheel_size."\"\t". //	ホイールサイズ wheel_size
			"\"".$wheel_size_unit_of_measure."\"\t". //	ホイールサイズの単位 wheel_size_unit_of_measure
			"\"\"\t". //	負荷容量 load_capacity
			"\"\"\t". //	負荷容量の単位 load_capacity_unit_of_measure
			"\"".$website_shipping_weight."\"\t". //	配送重量 website_shipping_weight
			"\"".$website_shipping_weight_unit_of_measure."\"\t". //	配送重量の単位 website_shipping_weight_unit_of_measure
			"\"".$rim_size."\"\t". //	リムサイズ rim_size
			"\"".$rim_size_unit_of_measure."\"\t". //	バイクのリムサイズの単位 rim_size_unit_of_measure
			"\"".$capacity."\"\t". //	最大の高さ（ピーク）　 capacity
			"\"".$capacity_unit_of_measure."\"\t". //	容量の単位 capacity_unit_of_measure
			"\"".$item_display_diameter."\"\t". //	商品の直径 item_display_diameter
			"\"".$item_display_diameter_unit_of_measure."\"\t". //	リールの直径の単位 item_display_diameter_unit_of_measure
			"\"".$item_display_height."\"\t". //	商品の高さ item_display_height
			"\"".$item_display_height_unit_of_measure."\"\t". //	高さの単位 item_display_height_unit_of_measure
			"\"\"\t". //	シャフト(軸)の丈 shaft_height
			"\"\"\t". //	シャフトの高さの測定単位 shaft_height_unit_of_measure
			"\"\"\t". //	商品の寸法の単位 item_dimensions_unit_of_measure
			"\"\"\t". //	商品の深さ item_depth
			"\"\"\t". //	最大積載可能重量の単位 maximum_weight_capacity
			"\"\"\t". //	最大荷重の単位 maximum_weight_capacity_unit_of_measure
			"\"\"\t". //	メーカー推奨最大重量 maximum_weight_recommendation
			"\"\"\t". //	推奨最大重量の単位 maximum_weight_recommendation_unit_of_measure
			"\"\"\t". //	最低の高さの単位 minimum_height_unit_of_measure
			"\"\"\t". //	最大高さ maximum_height
			"\"\"\t". //	最小の高さ minimum_height
			"\"\"\t". //	最大高さの単位 maximum_height_unit_of_measure
			"\"\"\t". //	ブレーキの幅 brake_width
			"\"\"\t". //	ブレーキの幅の単位 brake_width_unit_of_measure
			"\"\"\t". //	フロアの長さ floor_length
			"\"\"\t". //	フロアの幅 floor_width
			"\"\"\t". //	靴の幅 shoe_width
			"\"\"\t". //	袖の長さ sleeve_length
			"\"\"\t". //	ウエストのサイズ waist_size
			"\"\"\t". //	クランクの長さ crank_length
			"\"\"\t". //	トップチューブの長さ top_tube_length
			"\"\"\t". //	股下の長さ inseam_length
			"\"\"\t". //	有効エッジの長さ effective_edge_length
			"\"\"\t". //	刃の長さ blade_length
			"\"\"\t". //	メーカー推奨最大身長 maximum_height_recommendation
			"\"\"\t". //	破断強度 breaking_strength
			"\"\"\t". //	破断強度の単位 breaking_strength_unit_of_measure
			"\"\"\t". //	対物レンズのサイズ objective_lens_diameter
			"\"\"\t". //	商品対応範囲 item_area
			"\"\"\t". //	商品の厚さ item_thickness_derived
			"\"\"\t". //	袖の長さの単位 sleeve_length_unit_of_measure
			"\"\"\t". //	フロアの長さの単位 floor_length_unit_of_measure
			"\"\"\t". //	フロアの幅の単位 floor_width_unit_of_measure
			"\"\"\t". //	ウエストサイズの単位 waist_size_unit_of_measure
			"\"\"\t". //	クランクの長さの単位 crank_length_unit_of_measure
			"\"\"\t". //	トップチューブの長さの単位 top_tube_length_unit_of_measure
			"\"\"\t". //	有効エッジの長さの単位 effective_edge_length_unit_of_measure
			"\"\"\t". //	刃の断面の形状の単位 blade_length_unit_of_measure
			"\"\"\t". //	推奨最高身長の単位 maximum_height_recommendation_unit_of_measure
			"\"\"\t". //	対物レンズのサイズの単位 objective_lens_diameter_unit_of_measure
			"\"\"\t". //	商品対応範囲の単位 item_area_unit_of_measure
			"\"\"\t". //	商品の厚さの単位 item_thickness_unit_of_measure
			"\"\"\t". //	股下の長さの単位 inseam_length_unit_of_measure
			"\"\"\t". //	靴幅の単位 shoe_width_unit_of_measure
			"\"\"\t". //	高さマップ height_map
			"\"\"\t". //	胸囲サイズの単位 chest_size_unit_of_measure
			"\"\"\t". //	胸囲サイズ chest_size
			"\"\"\t". //	強度 tensile_strength
			"\"\"\t". //	強度の単位 tensile_strength_unit_of_measure
			"\"\"\t". //	タイヤの直径 tire_diameter
			"\"\"\t". //	タイヤ直径の単位 tire_diameter_unit_of_measure
			"\"\"\t". //	拡張された長さ extended_length
			"\"\"\t". //	伸びた長さの測定単位 extended_length_unit_of_measure
			"\"\"\t". //	ケーブル長さの単位 cable_length_unit_of_measure
			"\"\"\t". //	ハンドルの高さ handle_height
			"\"\"\t". //	ハンドルの高さの単位 handle_height_unit_of_measure
			"\"\"\t". //	チャンバーの高さの単位 chamber_height_unit_of_measure
			"\"\"\t". //	チャンバーの幅単位 chamber_width_unit_of_measure
			"\"\"\t". //	チャンバーの幅 chamber_width
			"\"\"\t". //	チャンバーの高さ chamber_height
			"\"\"\t". //	バンドの幅の単位 band_width_unit_of_measure
			"\"\"\t". //	バンド幅 band_width
			"\"\"\t". //	フレームの高さ frame_size
			"\"\"\t". //	フレームの高さの単位 frame_size_unit_of_measure
			"\"\"\t". //	デッキの長さ deck_length
			"\"\"\t". //	デッキの幅 deck_width
			"\"\"\t". //	デッキ長の単位 deck_length_unit_of_measure
			"\"\"\t". //	デッキ幅の単位 deck_width_unit_of_measure
			"\"\"\t". //	ディスプレイサイズ display_size
			"\"\"\t". //	ディスプレイ最大解像度の単位 display_size_unit_of_measure
			"\"\"\t". //	ディスプレイサイズ monitor_size
			"\"\"\t". //	モニターサイズの単位 monitor_size_unit_of_measure
			"\"\"\t". //	最大圧力の単位 maximum_pressure_unit_of_measure
			"\"\"\t". //	最大圧力 maximum_pressure 	
			"\"".$fulfillment_center_id."\"\t". //	フルフィルメントセンターID fulfillment_center_id
			"\"".$package_height."\"\t". //	商品パッケージの高さ package_height
			"\"".$package_width."\"\t". //	商品パッケージの幅 package_width
			"\"".$package_length."\"\t". //	商品パッケージの長さ package_length
			"\"".$package_length_unit_of_measure."\"\t". //	商品パッケージの長さの単位 package_length_unit_of_measure
			"\"".$package_weight."\"\t". //	商品パッケージの重量 package_weight
			"\"".$package_weight_unit_of_measure."\"\t". //	商品パッケージの重量の単位 package_weight_unit_of_measure
			"\"\"\t". //	商品パッケージの高さの単位 package_height_unit_of_measure
			"\"\"\t". //	商品パッケージの幅の単位 package_width_unit_of_measure
			"\"\"\t". //	商品パッケージの寸法の単位 package_dimensions_unit_of_measure
			"\"".$country_of_origin."\"\t". //	原産国/地域 country_of_origin
			"\"\"\t". // 	布タイプ fabric_type1
			"\"\"\t". // 	布タイプ fabric_type2
			"\"\"\t". // 	布タイプ fabric_type3
			"\"\"\t". // 	布タイプ fabric_type4
			"\"\"\t". // 	布タイプ fabric_type5
			"\"".$are_batteries_included."\"\t". //	電池付属 are_batteries_included
			"\"".$batteries_required."\"\t". //	この商品は電池本体ですか？または電池を使用した商品ですか？ batteries_required
			"\"".$battery_type."\"\t". // 	電池の種類、サイズ battery_type1
			"\"\"\t". //!! 	電池の種類、サイズ battery_type2
			"\"\"\t". //!! 	電池の種類、サイズ battery_type3
			"\"".$number_of_batteries."\"\t". // 	電池の数 number_of_batteries1
			"\"\"\t". //!! 	電池の数 number_of_batteries2
			"\"\"\t". //!! 	電池の数 number_of_batteries3
			"\"".$lithium_battery_energy_content."\"\t". //	電池当たりのワット時 lithium_battery_energy_content
			"\"".$lithium_battery_packaging."\"\t". //	リチウム電池パッケージ lithium_battery_packaging
			"\"".$lithium_battery_weight."\"\t". //	リチウム含有量(グラム) lithium_battery_weight
			"\"\"\t". //!!	リチウムイオン電池単数 number_of_lithium_ion_cells
			"\"\"\t". //!!	リチウムメタル電池単数 number_of_lithium_metal_cells
			"\"\"\t". //!!	電池の組成 battery_cell_composition
			"\"\"\t". //!!	電池の重量(グラム) battery_weight
			"\"\"\t". //!!	電池の重量の測定単位 battery_weight_unit_of_measure
			"\"\"\t". //!!	リチウム電池のエネルギー量測定単位 lithium_battery_energy_content_unit_of_measure
			"\"\"\t". //!!	リチウム電池の重量の測定単位 lithium_battery_weight_unit_of_measure
			"\"\"\t". //!! 	適用される危険物関連法令 supplier_declared_dg_hz_regulation1
			"\"\"\t". //!! 	適用される危険物関連法令 supplier_declared_dg_hz_regulation2
			"\"\"\t". //!! 	適用される危険物関連法令 supplier_declared_dg_hz_regulation3
			"\"\"\t". //!! 	適用される危険物関連法令 supplier_declared_dg_hz_regulation4
			"\"\"\t". //!! 	適用される危険物関連法令 supplier_declared_dg_hz_regulation5
			"\"\"\t". //!!	国連(UN)番号 hazmat_united_nations_regulatory_id
			"\"\"\t". //!!	安全データシート(SDS) URL safety_data_sheet_url
			"\"\"\t". //!!	商品の重量 item_weight
			"\"\"\t". //!!	商品の重量の単位 item_weight_unit_of_measure
			"\"\"\t". //!!	商品の体積 item_volume
			"\"\"\t". //!!	商品の容量の単位 item_volume_unit_of_measure
			"\"\"\t". //!!	内用液は含まれていますか? contains_liquid_contents
			"\"\"\t". //	引火点(°C) flash_point
			"\"\"\t". //	商品に適用される保管規制の区分 fulfillment_center_special_storage
			"\"\"\t". //	メーカー保証の説明 warranty_description
			"\"\"\t". //!! 	分類／危険物ラベル(適用されるものをすべて選択) ghs_classification_class1
			"\"\"\t". //!! 	分類／危険物ラベル(適用されるものをすべて選択) ghs_classification_class2
			"\"\"\t". //!! 	分類／危険物ラベル(適用されるものをすべて選択) ghs_classification_class3
			"\"".$mfg_warranty_description_type."\"\t". //	保証 mfg_warranty_description_type
			"\"".$safety_warning."\"\t". //	安全性に関する注意 safety_warning
			"\"".$legal_disclaimer_description."\"\t". //	法規上の免責条項 legal_disclaimer_description
			"\"".$specific_uses_for_product."\"\t". //	特徴・用途 specific_uses_for_product
			"\"\"\t". //	legal_compliance_certification_date_of_issue legal_compliance_certification_date_of_issue
			"\"\"\t". //	import_designation import_designation
			"\"\"\t". //	legal_compliance_certification_metadata legal_compliance_certification_metadata
			"\"\"\t". //	legal_compliance_certification_expiration_date legal_compliance_certification_expiration_date
			"\"\"\t". //	cpsia_cautionary_statement cpsia_cautionary_statement
			"\"\"\t". //	cpsia_cautionary_description cpsia_cautionary_description
			"\"\"\t". //	適合性の認証の状況 legal_compliance_certification_status
			"\"\"\t". //	規制機関の名前 legal_compliance_certification_regulatory_organization_name
			"\"\"\t". //	コンプライアンス認証値 legal_compliance_certification_value
			"\"\"\t". //	保証タイプ warranty_type
			"\"\"\t". //	熱に弱い is_heat_sensitive
			"\"\"\t". //	通貨コード currency
			"\"".$condition_type."\"\t". //	コンディション condition_type
			"\"".$condition_note."\"\t". //	商品のコンディション説明 condition_note
			"\"".$list_price."\"\t". //	メーカー希望価格 list_price
			"\"".$product_site_launch_date."\"\t". //	商品の公開日 product_site_launch_date
			"\"".$merchant_release_date."\"\t". //	予約商品の販売開始日 merchant_release_date
			"\"".$restock_date."\"\t". //	商品の入荷予定日 restock_date
			"\"".$fulfillment_latency."\"\t". //	出荷作業日数 fulfillment_latency
			"\"\"\t". //	取り寄せ is_sourcing_on_demand
			"\"".$optional_payment_type_exclusion."\"\t". //	使用しない支払い方法 optional_payment_type_exclusion
			"\"".$sale_price."\"\t". //	セール価格 sale_price
			"\"".$sale_from_date."\"\t". //	セール開始日 sale_from_date
			"\"".$sale_end_date."\"\t". //	セール終了日 sale_end_date
			"\"".$max_order_quantity."\"\t". //	最大注文個数 max_order_quantity
			"\"\"\t". //	パッケージ数 item_package_quantity
			"\"".$offering_can_be_gift_messaged."\"\t". //	ギフトメッセージ offering_can_be_gift_messaged
			"\"".$offering_can_be_gift_messaged."\"\t". //	ギフト包装 offering_can_be_giftwrapped
			"\"".$merchant_shipping_group_name."\"\t". //	配送パターン merchant_shipping_group_name
			"\"".$distribution_designation."\"\t". //	販売形態(並行輸入品) distribution_designation
			"\"\"\t". //	在庫期間 fc_shelf_life
			"\"\"\t". //	終了日を提案する offering_end_date
			"\"\"\t". //	最低広告表示価格 map_price
			"\"\"\t". //	賞味期限もしくは消費期限タイプ product_expiration_type
			"\"\"\t". //	付属品総数 number_of_items
			"\"\"\t". //	開始日を提案する offering_start_date
			"\"\"\t". //	商品タックスコード product_tax_code
			"\"".$standard_points_percent."\"\t". //	ポイントパーセント standard_price_points_percent
			"\"".$sale_points_percent."\"\t". //	セール時ポイントパーセント sale_price_points_percent
			"\"".$delivery_schedule_group_id."\"\t". //	配送日時指定SKUリスト delivery_schedule_group_id
			"\"".$is_discontinued_by_manufacturer."\"\t". //	メーカー製造中止 is_discontinued_by_manufacturer
			"\"\"\t". //	サイン入り signed_by
			"\"\"\t". //	max_aggregate_ship_quantity max_aggregate_ship_quantity			
			"\n";

		}
	}

if ($ck_dis_flg == 1) {
	echo "end g_num<>".$g_num."<br>\n";
	echo "<br>\n";
	echo $tsv."<br>\n";
	echo "<br>\n";
}

	return $tsv;
}



//	出品ファイルL用
function amazon_zaiko_item_tsv($AMZ_UP_LIST, $AMZ_SYU_LIST) {

	$tsv = "";
	if ($AMZ_SYU_LIST) {
		foreach ($AMZ_SYU_LIST AS $val) {
			$tsv .= $val;
		}
	}

	//	削除リスト作成
	if ($AMZ_UP_LIST) {
		foreach ($AMZ_UP_LIST AS $sku => $VAL) {
			//$sku = "";
			$price = "";
			$standard_price_points = "";
			$quantity = "";
			$product_id = "";
			$product_id_type = "";
			$condition_type = "";
			$condition_note = "";
			$ASIN_hint = "";
			$title = "";
			$operation_type = "";
			$sale_price = "";
			$sale_price_points = "";
			$sale_start_date = "";
			$sale_end_date = "";
			$leadtime_to_ship = "";
			$launch_date = "";
			$is_giftwrap_available = "";
			$is_gift_message_available = "";
			$fulfillment_center_id = "";
			$optional_payment_type_exclusion = "";
			$main_offer_image = "";
			$offer_image1 = "";
			$offer_image2 = "";
			$offer_image3 = "";
			$offer_image4 = "";
			$offer_image5 = "";
			$merchant_shipping_group_name = "";		//	add ohkawara 2017/08/04

			$price = $VAL['price'];
			$product_id = $VAL['asin'];
			$quantity = 0;
			//$operation_type = "Update";	//	後にDeleteになるかも？	//	del ohkawara 2019/11/18
			$operation_type = "Delete";									//	add ohkawara 2019/11/18

			if ($product_id != "") {
				$len = strlen($product_id);
				if ($len == 16) {
					$product_id_type = "GCID";
				} elseif ($len == 12) {
					$product_id_type = "UPC";
				} elseif ($len == 13) {
					$product_id_type = "EAN";
				} elseif ($len == 10) {
					$product_id_type = "ASIN";
				}
			} else {
				continue;
			}

			$tsv .= "\"".$sku."\"\t".  // 	商品管理番号sku
			"\"".$price."\"\t".  // 	販売価格  price
			"\"".$standard_price_points."\"\t".  //	ポイントパーセント  standard-price-points-percent
			"\"".$quantity."\"\t".  // 	在庫数  quantity
			"\"".$product_id."\"\t".  // 	商品コード(JANコード等)  product-id
			"\"".$product_id_type."\"\t".  // 	商品コードのタイプ  product-id-type
			"\"".$condition_type."\"\t".  // 	商品のコンディション  condition-type
			"\"".$condition_note."\"\t".  // 	商品のコンディション説明  condition-note
			"\"".$ASIN_hint."\"\t".  // 	対象  ASIN-hint
			"\"".$title."\"\t".  // 	商品名  title
			"\"".$operation_type."\"\t".  // 	動作タイプ  operation-type
			"\"".$sale_price."\"\t".  // 	セール価格  sale-price
			"\"".$sale_price_points."\"\t".  // 	セール時ポイントパーセントsale-price-points-percent
			"\"".$sale_start_date."\"\t".  // 	セール開始日  sale-start-date
			"\"".$sale_end_date."\"\t".  // 	セール終了日  sale-end-date
			"\"".$leadtime_to_ship."\"\t".  // 	リードタイム(出荷までにかかる作業日数)  leadtime-to-ship
			"\"".$launch_date."\"\t".  // 	商品の公開日  launch-date
			"\"".$is_giftwrap_available."\"\t".  // 	ギフト包装  is-giftwrap-available
			"\"".$is_gift_message_available."\"\t".  // 	ギフトメッセージ  is-gift-message-available
			"\"".$fulfillment_center_id."\"\t".  // 	フルフィルメントセンターID  fulfillment-center-id
			"\"".$optional_payment_type_exclusion."\"\t".  // 	使用しない支払い方法  optional-payment-type-exclusion
			"\"".$main_offer_image."\"\t".  // 	出品者SKUのメイン画像  URLmain-offer-image
			"\"".$offer_image1."\"\t".  //  	出品者SKUのサブ画像  URL1 - 出品者SKUのサブ画像URL5offer-image1- offer-image5
			"\"".$merchant_shipping_group_name."\"\t".  // 	配送パターン merchant_shipping_group_name
			"\"\"\t".  // 	商品タックスコード  product_tax_code
			"\"\"\t".  // 	電池の有無  batteries_required
			"\"\"\t".  // 	電池の付属有無  are_batteries_included
			"\"\"\t".  // 	電池の種類  battery_cell_composition
			"\"\"\t".  // 	電池の規格  battery_type
			"\"\"\t".  // 	付属する電池の数  number_of_batteries
			"\"\"\t".  // 	電池の重量(グラム)  battery_weight
			"\"\"\t".  // 	リチウムイオンのセル数  number_of_lithium_ion_cells
			"\"\"\t".  // 	リチウム金属のセル数  number_of_lithium_metal_cells
			"\"\"\t".  // 	リチウム電池の梱包状態  lithium_battery_packaging
			"\"\"\t".  // 	リチウム電池のエネルギー量  lithium_battery_energy_content
			"\"\"\t".  // 	リチウム電池の重量  lithium_battery_weight
			"\"\"\t".  // 	危険物該当の該否 supplier_declared_dg_hz_regulation1-supplier_declared_dg_hz_regulation5
			"\"\"\t".  // 	国連(UN)番号  hazmat_united_nations_regulatory_id
			"\"\"\t".  // 	安全データシート(SDS) のURL  safety_data_sheet_url
			"\"\"\t".  // 	商品の重量  item_weight
			"\"\"\t".  // 	商品の体積  item_volume
			"\"\"\t".  // 	引火点(°C)  flash_point
			"\"\"\t".  // 	危険物ラベル(該当するものをすべて選択)  ghs_classification_class1-ghs_classification_class3
			"\"\"\t".  // 	商品の重量の単位  item_weight_unit_of_measure
			"\"\"\t".  // 	商品の容積の単位  item_volume_unit_of_measure
			"\"\"\t".  // 	リチウム電池のエネルギー量の単位  lithium_battery_energy_content_unit_of_measure
			"\"\"\t".  // 	リチウム電池の重量単位  lithium_battery_weight_unit_of_measure
			"\"\"\t".  // 	電池の重量単位  battery_weight_unit_of_measure
			"\n";		//	add ohkawara 2017/08/04
		}
	}

	return $tsv;
}



//	出品ファイルL用ヘッダ
function amazon_tsv_head0() {

	$head  = "TemplateType=Offer	Version=2020.000\n";
	$head .= "商品管理番号	販売価格	ポイントパーセント	在庫数	商品コード(JANコード等)	商品コードのタイプ	商品のコンディション	商品のコンディション説明	対象ASIN	商品名	動作タイプ	セール価格	セール時ポイントパーセント	セール開始日	セール終了日	リードタイム(出荷までにかかる作業日数)	商品の公開日	ギフト包装	ギフトメッセージ	フルフィルメントセンターID	使用しない支払い方法	出品者SKUのメイン画像URL	出品者SKUのサブ画像URL1 	出品者SKUのサブ画像URL2	出品者SKUのサブ画像URL3	出品者SKUのサブ画像URL4	出品者SKUのサブ画像URL5	 配送パターン	商品タックスコード	\"電池の有無\"	\"電池付属\"	\"バッテリーセルタイプ\"	\"電池のタイプ\"	\"付属バッテリ個数\"	電池の重量(グラム)	\"リチウムイオン電池単数\"	\"リチウムメタル電池単数\"	\"リチウム電池パッケージ\"	\"リチウム電池エネルギー量\"	\"リチウム電池重量\"	この商品は、危険物に該当しますか？詳細については危険物確認ガイド{X}をご覧ください。1	この商品は、危険物に該当しますか？詳細については危険物確認ガイド{X}をご覧ください。2	この商品は、危険物に該当しますか？詳細については危険物確認ガイド{X}をご覧ください。3	この商品は、危険物に該当しますか？詳細については危険物確認ガイド{X}をご覧ください。4	この商品は、危険物に該当しますか？詳細については危険物確認ガイド{X}をご覧ください。5	国連(UN)番号	安全データシート(SDS) URL	\"商品の重量\"	\"体積\"	引火点(°C)	分類／危険物ラベル(適用されるものをすべて選択)1	分類／危険物ラベル(適用されるものをすべて選択)2	分類／危険物ラベル(適用されるものをすべて選択)3	商品の重量の単位	商品の容量の単位	リチウム電池のエネルギー量測定単位	リチウム電池の重量の測定単位	リチウム電池の重量の測定単位\n";
	//$head .= "商品管理番号	販売価格	ポイント	在庫数	商品コード(JANコード等)	商品コードのタイプ	商品のコンディション	商品のコンディション説明	対象ASIN	商品名	動作タイプ	セール価格	セール時ポイント	セール開始日	セール終了日	リードタイム(出荷までにかかる作業日数)	商品の公開日	ギフト包装	ギフトメッセージ	フルフィルメントセンターID	使用しない支払い方法	出品者SKUのメイン画像URL	出品者SKUのサブ画像URL1 	出品者SKUのサブ画像URL2	出品者SKUのサブ画像URL3	出品者SKUのサブ画像URL4	出品者SKUのサブ画像URL5	配送パターン\n";
	$head .= "sku	price	standard-price-points-percent	quantity	product-id	product-id-type	condition-type	condition-note	ASIN-hint	title	operation-type	sale-price	sale-price-points-percent	sale-start-date	sale-end-date	leadtime-to-ship	launch-date	is-giftwrap-available	is-gift-message-available	fulfillment-center-id	optional-payment-type-exclusion	main-offer-image	offer-image1	offer-image2	offer-image3	offer-image4	offer-image5	merchant_shipping_group_name	product-tax-code	batteries_required	are_batteries_included	battery_cell_composition	battery_type	number_of_batteries	battery_weight	number_of_lithium_ion_cells	number_of_lithium_metal_cells	lithium_battery_packaging	lithium_battery_energy_content	lithium_battery_weight	supplier_declared_dg_hz_regulation1	supplier_declared_dg_hz_regulation2	supplier_declared_dg_hz_regulation3	supplier_declared_dg_hz_regulation4	supplier_declared_dg_hz_regulation5	hazmat_united_nations_regulatory_id	safety_data_sheet_url	item_weight	item_volume	flash_point	ghs_classification_class1	ghs_classification_class2	ghs_classification_class3	item_weight_unit_of_measure	item_volume_unit_of_measure	lithium_battery_energy_content_unit_of_measure	lithium_battery_weight_unit_of_measure	battery_weight_unit_of_measure\n";
	//$head .= "sku	price	standard-price-points	quantity	product-id	product-id-type	condition-type	condition-note	ASIN-hint	title	operation-type	sale-price	sale-price-points	sale-start-date	sale-end-date	leadtime-to-ship	launch-date	is-giftwrap-available	is-gift-message-available	fulfillment-center-id	optional-payment-type-exclusion	main-offer-image	offer-image1	offer-image2	offer-image3	offer-image4	offer-image5	merchant_shipping_group_name\n";

	return $head;

}



//	アパレル＆シューズ用ヘッダ
function amazon_tsv_head1() {

	//	ohkawara 2017/08/01	更新
	//	ohkawara 2017/08/07	更新

	//	ohkawara 2019/12/12
	// $head  = "TemplateType=fptcustomcustom	Version=2019.1210	TemplateSignature=Q09BVCxUVU5JQyxERVNJR05FUl9ZVUtBVEEsT1VURVJXRUFSLFNXRUFURVIsU0FTSCxORUNLVElFLERVUEFUVEEsQ0hJTERSRU5TX0NPU1RVTUUsU09DS1NIT1NJRVJZLEVUSE5JQ19XRUFSLEFQUk9OLEJBQ0tQQUNLLExFSEVOR0EsQkxBWkVSLEJBQllfQ09TVFVNRSxTVUlULFBST1RFQ1RJVkVfR0VBUixERVNJR05FUl9DSEFOQ0hBTktPLFBFUlNPTkFMQk9EWUNBUkUsREVTSUdORVJfT0JJLFNXSU1XRUFSLERFU0lHTkVSX0tJTU9OTyxBUFBBUkVMX0dMT1ZFUyxTT0NLUyxQQU5UUyxIT01FQUNDRVNTT1JZLENBTUlTT0xFLENIQU5DSEFOS08sQ0xPVEhfV1JBUCxOT05BUFBBUkVMTUlTQyxTS0lSVCxDT1JTRVQsU0hPUlRTLEhPU0lFUlksTEVPVEFSRCxPUkNBX1NISVJULFVOREVSV0VBUixTTEVFUFdFQVIsU0NBUkYsU0FSRUUsQ0hPTEksTEVIRU5HQV9DSE9MSV9TRVQsT0JJLEtVUlRBLFNBTFdBUl9TVUlUX1NFVCxBRFVMVF9DT1NUVU1FLFNISVJULEtJTU9OTyxST0JFLFVOSU9OX1NVSVQsQVBQQVJFTCxUSUdIVFMsQkFHLEFQUEFSRUxfQkVMVCxZVUtBVEEsQlJBLE9WRVJBTExTLERSRVNTLEJPRFlTVE9DS0lORyxBQ0NFU1NPUlksQkxPVVNFLFZFU1QsSklOQkVJLEhBVCxTSE9FUw==	上3行は Amazon.com 記入用です。上3行は変更または削除しないでください。.																																				画像									バリエーション				商品基本情報																				商品検索情報																						推奨ブラウズノード別の情報								寸法																					コンプライアンス情報																						出品情報																							b2b														\n";
	// $head .= "feed_product_type	出品者SKU	ブランド名	商品名	商品コード(JANコード等)	商品コードのタイプ	表地素材	推奨ブラウズノード	item_length_description	素材構成	素材構成	素材構成	素材構成	素材構成	素材構成	素材構成	素材構成	素材構成	素材構成	サイズ	カラー	カラー	スタイル名	lifestyle1	lifestyle2	department_name	機能性	機能性	機能性	機能性	機能性	サイズマップ	バンドサイズ	バンドサイズの単位	アダルト商品	在庫数	商品の販売価格	item_package_quantity	商品メイン画像URL	商品のサブ画像URL1	商品のサブ画像URL2	商品のサブ画像URL3	商品のサブ画像URL4	商品のサブ画像URL5	商品のサブ画像URL6	商品のサブ画像URL7	商品のサブ画像URL8	カラーサンプル画像URL	親子関係の指定	親商品のSKU(商品管理番号)	親子関係のタイプ	バリエーションテーマ	アップデート・削除	メーカー型番	商品説明文	ヒールのタイプ	留め具のタイプ	モデル年(発売年・発表年)	inner_material_type1	inner_material_type2	inner_material_type3	inner_material_type4	inner_material_type5	sole_material	性別	対象年齢層	靴サイズ表記単位（国）	対象年齢層(詳細)	靴サイズ表記単位	靴幅	靴サイズ	靴サイズの上限値	商品説明の箇条書き	商品説明の箇条書き	商品説明の箇条書き	商品説明の箇条書き	商品説明の箇条書き	検索キーワード	スタイルキーワード	首のタイプ	シーズン	プラチナキーワード	プラチナキーワード	プラチナキーワード	プラチナキーワード	プラチナキーワード	コート・ワンピース・チュニック着丈	コート・ワンピース・チュニック着丈	コート・ワンピース・チュニック着丈	コート・ワンピース・チュニック着丈	コート・ワンピース・チュニック着丈	material_type	shaft_style_type	fit_type	ストラップのタイプ	つま先の形状(トゥシェープ)	ウエストのスタイル	袖のタイプ	シャツカラースタイル	ボトムススタイル	fit_to_size_description	opacity	靴の幅	シャフト(軸)の丈	カップサイズ	配送重量の単位	配送重量	inseam_length	inseam_length_unit_of_measure	sleeve_length	袖の長さの単位	waist_size	neck_size_unit_of_measure	ウエストサイズの単位	chest_size_unit_of_measure	chest_size	neck_size	シャフトの高さの測定単位	item_width	item_height	shoe_width_unit_of_measure	商品の寸法の単位	item_length	素材または繊維	この商品は電池本体ですか？または電池を使用した商品ですか？	電池付属	電池の組成	電池の種類、サイズ	電池の種類、サイズ	電池の種類、サイズ	電池の数	電池の数	電池の数	リチウムメタル電池単数	リチウムイオン電池単数	リチウム電池パッケージ	電池当たりのワット時	リチウム電池のエネルギー量測定単位	リチウム含有量(グラム)	リチウム電池の重量の測定単位	商品の重量	商品の重量の単位	体積	商品の容量の単位	原産国/地域	出荷作業日数	商品のコンディション	商品のコンディション説明	ポイント	セール時ポイント	商品の公開日	予約商品の販売開始日	メーカー希望価格	使用しない支払い方法	配送日時指定SKUリスト	セール価格	セール開始日	セール終了日	商品の入荷予定日	最大注文個数	ギフトメッセージ	ギフト包装	メーカー製造中止	配送パターン	販売形態(並行輸入品)	廃盤日	販売開始日	商品タックスコード	法人価格	数量割引のタイプ	数量の下限1	数量割引1	数量の下限2	数量割引2	数量の下限3	数量割引3	数量の下限4	数量割引4	数量の下限5	数量割引5	価格設定操作	unspsc_code	national_stock_number\n";
	// $head .= "feed_product_type	item_sku	brand_name	item_name	external_product_id	external_product_id_type	outer_material_type	recommended_browse_nodes	item_length_description	material_composition1	material_composition2	material_composition3	material_composition4	material_composition5	material_composition6	material_composition7	material_composition8	material_composition9	material_composition10	size_name	color_name	color_map	style_name	lifestyle1	lifestyle2	department_name	special_features1	special_features2	special_features3	special_features4	special_features5	size_map	band_size_num	band_size_num_unit_of_measure	is_adult_product	quantity	standard_price	item_package_quantity	main_image_url	other_image_url1	other_image_url2	other_image_url3	other_image_url4	other_image_url5	other_image_url6	other_image_url7	other_image_url8	swatch_image_url	parent_child	parent_sku	relationship_type	variation_theme	update_delete	part_number	product_description	heel_type	closure_type	model_year	inner_material_type1	inner_material_type2	inner_material_type3	inner_material_type4	inner_material_type5	sole_material	target_gender	age_range_description	footwear_size_system	footwear_age_group	footwear_size_class	footwear_width	footwear_size	footwear_to_size	bullet_point1	bullet_point2	bullet_point3	bullet_point4	bullet_point5	generic_keywords	style_keywords	neck_style	seasons	platinum_keywords1	platinum_keywords2	platinum_keywords3	platinum_keywords4	platinum_keywords5	specific_uses_keywords1	specific_uses_keywords2	specific_uses_keywords3	specific_uses_keywords4	specific_uses_keywords5	material_type	shaft_style_type	fit_type	strap_type	toe_shape	waist_style	sleeve_type	collar_style	bottom_style	fit_to_size_description	opacity	shoe_width	shaft_height	cup_size	website_shipping_weight_unit_of_measure	website_shipping_weight	inseam_length	inseam_length_unit_of_measure	sleeve_length	sleeve_length_unit_of_measure	waist_size	neck_size_unit_of_measure	waist_size_unit_of_measure	chest_size_unit_of_measure	chest_size	neck_size	shaft_height_unit_of_measure	item_width	item_height	shoe_width_unit_of_measure	item_dimensions_unit_of_measure	item_length	fabric_type	batteries_required	are_batteries_included	battery_cell_composition	battery_type1	battery_type2	battery_type3	number_of_batteries1	number_of_batteries2	number_of_batteries3	number_of_lithium_metal_cells	number_of_lithium_ion_cells	lithium_battery_packaging	lithium_battery_energy_content	lithium_battery_energy_content_unit_of_measure	lithium_battery_weight	lithium_battery_weight_unit_of_measure	item_weight	item_weight_unit_of_measure	item_volume	item_volume_unit_of_measure	country_of_origin	fulfillment_latency	condition_type	condition_note	standard_price_points	sale_price_points	product_site_launch_date	merchant_release_date	list_price	optional_payment_type_exclusion	delivery_schedule_group_id	sale_price	sale_from_date	sale_end_date	restock_date	max_order_quantity	offering_can_be_gift_messaged	offering_can_be_giftwrapped	is_discontinued_by_manufacturer	merchant_shipping_group_name	distribution_designation	offering_end_date	offering_start_date	product_tax_code	business_price	quantity_price_type	quantity_lower_bound1	quantity_price1	quantity_lower_bound2	quantity_price2	quantity_lower_bound3	quantity_price3	quantity_lower_bound4	quantity_price4	quantity_lower_bound5	quantity_price5	pricing_action	unspsc_code	national_stock_number\n";
	$head = "TemplateType=fptcustom	Version=2022.1208	Category=clothing	TemplateSignature=Q09BVCxUVU5JQyxTQUZFVFlfR0xBU1NFUyxTSE9FTEFDRSxPVVRFUldFQVIsU1dFQVRFUixORUNLVElFLFVOREVSUEFOVFMsSEFOREJBRyxDSElMRFJFTlNfQ09TVFVNRSxOSUdIVEdPV05fTklHSFRTSElSVCxTT0NLU0hPU0lFUlksRVRITklDX1dFQVIsQVBST04sQkFDS1BBQ0ssQkxBWkVSLFNIT0VfQUNDRVNTT1JZLFBST1RFQ1RJVkVfR0xPVkUsQVJNX1NMRUVWRSxCQUJZX0NPU1RVTUUsVEVDSE5JQ0FMX1NQT1JUX1NIT0UsU1VJVCxGT09EX0JJQixTV0lNV0VBUixBUFBBUkVMX0dMT1ZFUyxTT0NLUyxQQU5UUyxDQU1JU09MRSxDSEFOQ0hBTktPLEVBUk1VRkYsU1dFQVRTSElSVCxTVVNQRU5ERVIsU0tJUlQsQ09SU0VULFNIT1JUUyxMRU9UQVJELE9SQ0FfU0hJUlQsVU5ERVJXRUFSLFNMRUVQV0VBUixTQ0FSRixTQVJFRSxDSE9MSSxMRUhFTkdBX0NIT0xJX1NFVCxPQkksUEFKQU1BUyxLVVJUQSxVTkRFUkdBUk1FTlRfU0xJUCxTQUxXQVJfU1VJVF9TRVQsQURVTFRfQ09TVFVNRSxTSElSVCxLSU1PTk8sUk9CRSxUSUdIVFMsQkFHLFRPVEVfQkFHLEJPT1QsVFJBQ0tfU1VJVCxBUFBBUkVMX0JFTFQsWVVLQVRBLEJSQSxXQUlTVF9DSU5DSEVSLE9WRVJBTExTLERSRVNTLEJPRFlTVE9DS0lORyxLRVlDSEFJTixTQU5EQUwsQUNDRVNTT1JZLFZFU1QsSklOQkVJLEhBVCxTSE9FUw==	settings=attributeRow=3&contentLanguageTag=ja_JP&dataRow=4&feedType=113&headerLanguageTag=ja_JP&isEdit=false&isExpose=false&isProcessingSummary=false&labelRow=2&primaryMarketplaceId=amzn1.mp.o.A1VC38T7YXB528&templateIdentifier=9dab827a-a66f-43f8-9689-9d3148d0847a&timestamp=2022-12-08T00%3A00%3A49.563Z	     上3行はAmazon.comのみで使用します。上3行は変更または削除しないでください。																																																																																																																												画像									バリエーション							商品基本情報																		商品検索情報																																																																																																																																											推奨ブラウズノード別の情報																																				寸法																																																					出荷関連情報										コンプライアンス情報																																														出品情報																													\n"; 
	$head .= "商品タイプ	出品者SKU	ブランド名	商品名	商品コード(JANコード等)	商品コードのタイプ	モデル名	模範	商品の仕様	商品の仕様	商品の仕様	商品の仕様	商品の仕様	部門名	素材構成	在庫数	商品の販売価格	商品メイン画像URL	性別	対象年齢層	アパレルサイズ表記単位(国)	アパレルサイズの表記方法	アパレルサイズの値	アパレルサイズ(最大レンジ)	アパレルサイズのボディタイプ	アパレルサイズの身長タイプ	表地素材	サイズ	カラー	カラーマップ	サイズマップ	アダルト商品	推奨ブラウズノード	ユニット数	ユニット数のタイプ	この商品はAmazon.co.jp限定商品ですか？	商品に適用される危険物規制の種類	商品に適用される危険物規制の種類	商品に適用される危険物規制の種類	商品説明文	メーカー型番	closure_type	manufacturer	スタイル名	種類	パッケージ商品数	care_instructions	品目の前から後ろまでの奥行き	品目の奥行きの単位	品目の左右の幅	品目の幅単位	底部から上部までの高さ	品目の高さ単位	付属品総数	靴サイズ表記単位（国）	対象年齢層(詳細)	靴サイズ表記単位	靴サイズ	シェイプウェアのサイズ システム	シェイプウェアのサイズ クラス	シェイプウェアのサイズ値	シェイプウェアのサイズ範囲	バンドサイズの値	バンドサイズ(最大レンジ)	カップサイズの値	シェイプウェア ボディタイプ	シェイプウェアの身長タイプ	ボトムスサイズシステム	ボトムスサイズクラス	ボトムスサイズ値	ボトムスのサイズ範囲	ウエストサイズ値	股下サイズ値	ボトムス体型タイプ	ボトムス身長タイプ	スカートサイズシステム	スカートサイズクラス	スカートサイズ値	スカートのサイズ範囲	ウエストサイズ値	スカート体型タイプ	スカート身長タイプ	シャツサイズシステム	シャツサイズクラス	シャツサイズ値	シャツのサイズ範囲	首回りサイズ値	裄丈サイズ値	シャツ体型タイプ	シャツ身長タイプ	水平方向の長端の長さ	長さ単位	水平方向の短い方の端の幅	幅単位	底部から上部までの高さ	高さの単位	機能性	機能性	機能性	機能性	機能性	機能性	機能性	機能性	機能性	機能性	機能性	機能性	機能性	機能性	バンドサイズ	バンドサイズの単位	ライフスタイル	ライフスタイル	ライフスタイル	ライフスタイル	ライフスタイル	ライフスタイル	ライフスタイル	ライフスタイル	ライフスタイル	ライフスタイル	首のタイプ	長さ	素材構成	帽子サイズ表記単位(国) 	帽子サイズの表記方法	帽子サイズの値	帽子のサイズ範囲	商品のサブ画像URL1	商品のサブ画像URL2	商品のサブ画像URL3	商品のサブ画像URL4	商品のサブ画像URL5	商品のサブ画像URL6	商品のサブ画像URL7	商品のサブ画像URL8	カラーサンプル画像URL	親子関係の指定	親商品のSKU(商品管理番号)	子関係タイプ	バリエーション種類	パッケージレベル	パッケージ内包数量	パッケージには属性が含まれています	アップデート・削除	自社で管理しているSKUごとの番号	ヒールのタイプ	モデル年(発売年・発表年)	インナーの素材	バーコード適用除外理由	版	ソール素材	ヒールの高さの単位	ヒールの高さ	footwear_gender	footwear_width_unisex	footwear_to_size_unisex	footwear_gender_unisex	footwear_size_unisex	ヒール高	靴幅	靴サイズの上限値	検索キーワード	サイン入り商品	行事	行事	行事	行事	行事	スポーツタイプ	スポーツタイプ	スポーツタイプ	プレーヤー名	チーム名	コレクション名	素材の種類	ターゲットユーザーのキーワード	ターゲットユーザーのキーワード	ターゲットユーザーのキーワード	ターゲットユーザーのキーワード	ターゲットユーザーのキーワード	毛皮の説明	フィットタイプ	フィットタイプ	フィットタイプ	フィットタイプ	フィットタイプ	襟のタイプ	織り方	ポケット記述	テーマ	ライニングの説明	リーグ名	特殊サイズタイプ	特殊サイズタイプ	特殊サイズタイプ	特殊サイズタイプ	特殊サイズタイプ	特殊サイズタイプ	特殊サイズタイプ	特殊サイズタイプ	特殊サイズタイプ	特殊サイズタイプ	商品の補充情報	商品の補充情報	商品の補充情報	商品の補充情報	商品の補充情報	パターン名	オーダー締切日	スタイルキーワード	スタイルキーワード	スタイルキーワード	シーズン	プラチナキーワード	プラチナキーワード	プラチナキーワード	プラチナキーワード	プラチナキーワード	特定用途キーワード	特定用途キーワード	特定用途キーワード	特定用途キーワード	特定用途キーワード	シャフトスタイルタイプ	UV加工	要組み立て	レンズのコーティング	レンズ色	適合車両の種類	適合車両の種類	適合車両の種類	適合車両の種類	適合車両の種類	適合車両の種類	適合車両の種類	適合車両の種類	適合車両の種類	箱数	同梱商品	同梱商品	同梱商品	同梱商品	同梱商品	商品の個数（ピース数）	耐水レベル	シャーシ素材	バッグストラップの高さ	ショルダーストラップドロップの測定単位	防汚	ラベルに記載されている国/地域	パターンのスタイル	出品者カタログ番号	メーカー対象最高年齢	組み立て時間の単位	assembly_time	thesaurus_attribute_keywords1	thesaurus_attribute_keywords2	thesaurus_attribute_keywords3	thesaurus_attribute_keywords4	thesaurus_attribute_keywords5	top_style	thesaurus_subject_keywords1	thesaurus_subject_keywords2	thesaurus_subject_keywords3	thesaurus_subject_keywords4	thesaurus_subject_keywords5	メーカー対象最低年齢	推奨使用用途	キャスター数	正味個数単位	防カビ加工	防カビ加工	防カビ加工	防カビ加工	防カビ加工	正味個数	利き手	股上のスタイル	フロントスタイル	ノイズ制御	ノイズ減衰単位	ノイズ減衰値	クラスタイプ	装飾機能	装飾機能	装飾機能	装飾機能	装飾機能	背面のスタイル	ハンドル素材	シャフトの円周	シャフトの円周の単位	つま先のスタイル	ベルトの種類	抗菌加工	商品の利点	ワット数の単位	グリップタイプ	ワット数	土踏まずの特徴	キャラクター	袖のタイプ	ファブリックウォッシュ	ストラップのタイプ	つま先の形状(トゥシェープ)	ウエストスタイル	不透明度	ボトムススタイル	収納可能サイズ	サブブランド	内蔵ライト	表示サイズ	underwire_type	opacity_unit_of_measure	leg_style	mfg_maximum	プレーヤー数	mfg_minimum	ポケットの数	正味容積単位	正味重量単位	正味の長さ	正味の長さの単位	正味面積	正味重量	正味面積単位	正味容積	ジャケットの長さ	バンドサイズ	ベルト幅の単位	ベルト幅	最低広告表示価格	ベルト幅の十進数値	bra_padding_level	梱包タイプ	つや消しスタイル	商品の幅の単位	チェストサイズの単位	チェストサイズ	商品の高さの単位	えきたいりょう	液量単位	商品の長さの単位	靴の幅	シャフト(軸)の丈	ウエストサイズの単位	ウエストのサイズ	仕立ての長さの単位	股下の長さ	袖の長さの単位	袖の長さ	首のサイズの単位	首のサイズ	配送重量の単位	配送重量	シャフトの高さの測定単位	項目幅	項目の高さ	靴幅	品目の長さ	商品の重量（パッケージを含まない）	商品の重量の単位	商品の寸法の単位	カップサイズ	長さ	商品の長さの単位	幅	商品の高さの単位	商品の幅の単位	容量の単位	商品の高さ	ディスプレイサイズ	ディスプレイ最大解像度の単位	性能・容量	leg_diameter_unit_of_measure	推奨最大重量の単位	メーカー推奨最大重量	leg_diameter	商品の形状	ストレージ容量の測定単位	ストレージボリューム	商品の形状	高さマップ	股上の高さ	股上の高さの単位	伸びた長さの測定単位	拡張された長さ	minimum_neck_size_unit_of_measure	ヘッドサイズ名	商品パッケージの長さの単位	長さ	幅	高さ	商品パッケージの重量の単位	商品パッケージの重量	フルフィルメントセンターID	商品パッケージの高さの単位	商品パッケージの幅の単位	商品パッケージの寸法の単位	この商品は電池本体ですか？または電池を使用した商品ですか？	電池付属	電池の組成	電池の種類、サイズ	電池の種類、サイズ	電池の種類、サイズ	電池の数	電池の数	電池の数	電池の重量(グラム)	電池の重量の測定単位	リチウムメタル電池単数	リチウムイオン電池単数	リチウム電池パッケージ	電池当たりのワット時	リチウム電池のエネルギー量測定単位	リチウム含有量(グラム)	リチウム電池の重量の測定単位	適用される危険物関連法令	適用される危険物関連法令	適用される危険物関連法令	適用される危険物関連法令	適用される危険物関連法令	国連(UN)番号	安全データシート(SDS) URL	商品の重量	商品の重量の単位	商品の体積	商品の容量の単位	原産国/地域	商品に適用される保管規制の区分	引火点(°C)	分類／危険物ラベル(適用されるものをすべて選択)	分類／危険物ラベル(適用されるものをすべて選択)	分類／危険物ラベル(適用されるものをすべて選択)	内用液は含まれていますか?	メーカー保証の説明	CPSIA警告の説明	並行輸入品	CPSIA警告	CPSIA警告	CPSIA警告	CPSIA警告	法規上の免責条項	安全上のお知らせ	external_testing_certification	出荷作業日数	コンディション	取り寄せ	商品のコンディション説明	通貨コード	予約商品の販売開始日	メーカー希望価格	使用しない支払い方法	セール価格	セール開始日	セール終了日	商品の入荷予定日	最大注文個数	ギフトメッセージ	ギフト包装	配送パターン	地図価格	販売形態(並行輸入品)	終了日を提案する	開始日を提案する	商品タックスコード	ポイントパーセント	セール時ポイントパーセント	商品の公開日	配送日時指定SKUリスト	メーカー製造中止	同梱発送時の最大個数	メーカー希望価格	ポイント	セール時ポイント\n";
	$head .="feed_product_type	item_sku	brand_name	item_name	external_product_id	external_product_id_type	model_name	model	bullet_point1	bullet_point2	bullet_point3	bullet_point4	bullet_point5	department_name	fabric_type	quantity	standard_price	main_image_url	target_gender	age_range_description	apparel_size_system	apparel_size_class	apparel_size	apparel_size_to	apparel_body_type	apparel_height_type	outer_material_type	size_name	color_name	color_map	size_map	is_adult_product	recommended_browse_nodes	unit_count	unit_count_type	is_exclusive_product	supplier_declared_material_regulation1	supplier_declared_material_regulation2	supplier_declared_material_regulation3	product_description	part_number	closure_type	manufacturer	style_name	item_type_name	item_package_quantity	care_instructions	depth_front_to_back	depth_front_to_back_unit_of_measure	depth_width_side_to_side	depth_width_side_to_side_unit_of_measure	depth_height_floor_to_top	depth_height_floor_to_top_unit_of_measure	number_of_items	footwear_size_system	footwear_age_group	footwear_size_class	footwear_size	shapewear_size_system	shapewear_size_class	shapewear_size	shapewear_size_to	shapewear_band_size	shapewear_band_size_to	shapewear_cup_size	shapewear_body_type	shapewear_height_type	bottoms_size_system	bottoms_size_class	bottoms_size	bottoms_size_to	bottoms_waist_size	bottoms_inseam_size	bottoms_body_type	bottoms_height_type	skirt_size_system	skirt_size_class	skirt_size	skirt_size_to	skirt_waist_size	skirt_body_type	skirt_height_type	shirt_size_system	shirt_size_class	shirt_size	shirt_size_to	shirt_neck_size	shirt_sleeve_length	shirt_body_type	shirt_height_type	length_head_to_toe	length_head_to_toe_unit_of_measure	length_width_side_to_side	length_width_side_to_side_unit_of_measure	length_height_floor_to_top	length_height_floor_to_top_unit_of_measure	special_features1	special_features2	special_features3	special_features4	special_features5	special_features6	special_features7	special_features8	special_features9	special_features10	special_features11	special_features12	special_features13	special_features14	band_size_num	band_size_num_unit_of_measure	lifestyle1	lifestyle2	lifestyle3	lifestyle4	lifestyle5	lifestyle6	lifestyle7	lifestyle8	lifestyle9	lifestyle10	neck_style	item_length_description	material_composition	headwear_size_system	headwear_size_class	headwear_size	headwear_size_to	other_image_url1	other_image_url2	other_image_url3	other_image_url4	other_image_url5	other_image_url6	other_image_url7	other_image_url8	swatch_image_url	parent_child	parent_sku	relationship_type	variation_theme	package_level	package_contains_quantity	package_contains_identifier	update_delete	codabar	heel_type	model_year	inner_material_type	gtin_exemption_reason	edition	sole_material	heel_height_unit_of_measure	heel_height	footwear_gender	footwear_width_unisex	footwear_to_size_unisex	footwear_gender_unisex	footwear_size_unisex	heel_height_string	footwear_width	footwear_to_size	generic_keywords	is_autographed	occasion_type1	occasion_type2	occasion_type3	occasion_type4	occasion_type5	sport_type1	sport_type2	sport_type3	athlete	team_name	collection_name	material_type	target_audience_keywords1	target_audience_keywords2	target_audience_keywords3	target_audience_keywords4	target_audience_keywords5	fur_description	fit_type1	fit_type2	fit_type3	fit_type4	fit_type5	collar_style	weave_type	pocket_description	theme	lining_description	league_name	special_size_type1	special_size_type2	special_size_type3	special_size_type4	special_size_type5	special_size_type6	special_size_type7	special_size_type8	special_size_type9	special_size_type10	lifecycle_supply_type1	lifecycle_supply_type2	lifecycle_supply_type3	lifecycle_supply_type4	lifecycle_supply_type5	pattern_name	item_booking_date	style_keywords1	style_keywords2	style_keywords3	seasons	platinum_keywords1	platinum_keywords2	platinum_keywords3	platinum_keywords4	platinum_keywords5	specific_uses_keywords1	specific_uses_keywords2	specific_uses_keywords3	specific_uses_keywords4	specific_uses_keywords5	shaft_style_type	ultraviolet_light_protection	is_assembly_required	lens_coating_description	lens_color	compatible_with_vehicle_type1	compatible_with_vehicle_type2	compatible_with_vehicle_type3	compatible_with_vehicle_type4	compatible_with_vehicle_type5	compatible_with_vehicle_type6	compatible_with_vehicle_type7	compatible_with_vehicle_type8	compatible_with_vehicle_type9	number_of_boxes	included_components1	included_components2	included_components3	included_components4	included_components5	number_of_pieces	water_resistance_level	frame_material_type	shoulder_strap_drop	shoulder_strap_drop_unit_of_measure	is_stain_resistant	country_as_labeled	pattern_type	catalog_number	mfg_maximum_unit_of_measure	assembly_time_unit_of_measure	assembly_time	thesaurus_attribute_keywords1	thesaurus_attribute_keywords2	thesaurus_attribute_keywords3	thesaurus_attribute_keywords4	thesaurus_attribute_keywords5	top_style	thesaurus_subject_keywords1	thesaurus_subject_keywords2	thesaurus_subject_keywords3	thesaurus_subject_keywords4	thesaurus_subject_keywords5	mfg_minimum_unit_of_measure	recommended_uses_for_product	number_of_wheels	net_content_count_unit_of_measure	material_type_free1	material_type_free2	material_type_free3	material_type_free4	material_type_free5	net_content_count	hand_orientation	rise_style	front_style	noise_control	noise_attenuation_unit_of_measure	noise_attenuation	clasp_type	embellishment_feature1	embellishment_feature2	embellishment_feature3	embellishment_feature4	embellishment_feature5	back_style	handle_material	shaft_circumference	shaft_circumference_unit_of_measure	toe_style	belt_style	item_styling	product_benefit	wattage_unit_of_measure	grip_type	wattage	arch_type	subject_character	sleeve_type	fabric_wash	strap_type	toe_shape	waist_style	opacity	bottom_style	fit_to_size_description	sub_brand_name	has_builtin_light	size_info_display_name	underwire_type	opacity_unit_of_measure	leg_style	mfg_maximum	number_of_players	mfg_minimum	number_of_pockets	net_content_volume_unit_of_measure	net_content_weight_unit_of_measure	net_content_length	net_content_length_unit_of_measure	net_content_area	net_content_weight	net_content_area_unit_of_measure	net_content_volume	jacket_length	band_size	belt_width_unit_of_measure	belt_width_string	map_policy	belt_width_derived	bra_padding_level	customer_package_type	matte_style	item_width_unit_of_measure	chest_size_unit_of_measure	chest_size	item_height_unit_of_measure	liquid_volume	liquid_volume_unit_of_measure	item_length_unit_of_measure	shoe_width	shaft_height	waist_size_unit_of_measure	waist_size	inseam_length_unit_of_measure	inseam_length	sleeve_length_unit_of_measure	sleeve_length	neck_size_unit_of_measure	neck_size	website_shipping_weight_unit_of_measure	website_shipping_weight	shaft_height_unit_of_measure	item_width	item_height	shoe_width_unit_of_measure	item_length	item_display_weight	item_display_weight_unit_of_measure	item_dimensions_unit_of_measure	cup_size	item_display_length	item_display_length_unit_of_measure	item_display_width	item_display_height_unit_of_measure	item_display_width_unit_of_measure	capacity_unit_of_measure	item_display_height	display_size	display_size_unit_of_measure	capacity	leg_diameter_unit_of_measure	maximum_weight_recommendation_unit_of_measure	maximum_weight_recommendation	leg_diameter	item_shape	storage_volume_unit_of_measure	storage_volume	item_form	height_map	rise_height	rise_height_unit_of_measure	extended_length_unit_of_measure	extended_length	minimum_neck_size_unit_of_measure	head_size_name	package_length_unit_of_measure	package_length	package_width	package_height	package_weight_unit_of_measure	package_weight	fulfillment_center_id	package_height_unit_of_measure	package_width_unit_of_measure	package_dimensions_unit_of_measure	batteries_required	are_batteries_included	battery_cell_composition	battery_type1	battery_type2	battery_type3	number_of_batteries1	number_of_batteries2	number_of_batteries3	battery_weight	battery_weight_unit_of_measure	number_of_lithium_metal_cells	number_of_lithium_ion_cells	lithium_battery_packaging	lithium_battery_energy_content	lithium_battery_energy_content_unit_of_measure	lithium_battery_weight	lithium_battery_weight_unit_of_measure	supplier_declared_dg_hz_regulation1	supplier_declared_dg_hz_regulation2	supplier_declared_dg_hz_regulation3	supplier_declared_dg_hz_regulation4	supplier_declared_dg_hz_regulation5	hazmat_united_nations_regulatory_id	safety_data_sheet_url	item_weight	item_weight_unit_of_measure	item_volume	item_volume_unit_of_measure	country_of_origin	fulfillment_center_special_storage	flash_point	ghs_classification_class1	ghs_classification_class2	ghs_classification_class3	contains_liquid_contents	warranty_description	cpsia_cautionary_description	import_designation	cpsia_cautionary_statement1	cpsia_cautionary_statement2	cpsia_cautionary_statement3	cpsia_cautionary_statement4	legal_disclaimer_description	safety_warning	external_testing_certification	fulfillment_latency	condition_type	is_sourcing_on_demand	condition_note	currency	merchant_release_date	list_price	optional_payment_type_exclusion	sale_price	sale_from_date	sale_end_date	restock_date	max_order_quantity	offering_can_be_gift_messaged	offering_can_be_giftwrapped	merchant_shipping_group_name	map_price	distribution_designation	offering_end_date	offering_start_date	product_tax_code	standard_price_points_percent	sale_price_points_percent	product_site_launch_date	delivery_schedule_group_id	is_discontinued_by_manufacturer	max_aggregate_ship_quantity	list_price_with_tax	standard_price_points	sale_price_points\n"; 
	//	del ohkawara 2019/12/12
	//$head  = "TemplateType=clothing	Version=2017.1102	上3行は Amazon.com 記入用です。上3行は変更または削除しないでください。.								出品情報-商品をサイト上で販売可能にする際に必要な項目																						寸法-商品のサイズや重量を入力する項目		商品検索情報-サーチ上で商品を検索されやすくするために必要な項目									画像-商品画像を表示させるために必要な項目。詳しくは画像説明タブを参照										出荷関連情報-フルフィルメント by Amazon (FBA) の利用、あるいは自社出荷の注文に関する出荷関連情報を、この項目に記入してください。FBA を利用する場合には必須の項目。							バリエーション-商品の色・サイズなどのバリエーションを作成する際に必須の項目				ProductTypeによって必須となる項目																																				\n";

	//$head .= "出品者SKU	商品名	商品コード(JANコード等)	商品コードのタイプ	ブランド名	商品タイプ	メーカー型番	商品説明文	アップデート・削除	自社で管理しているSKUごとの番号	在庫数	出荷作業日数	商品のコンディション	商品のコンディション説明	商品の販売価格	ポイント	商品の公開日	予約商品の販売開始日	メーカー希望価格	使用しない支払い方法	配送日時指定SKUリスト	セール価格	セール時ポイント	セール開始日	セール終了日	商品の入荷予定日	最大注文個数	ギフトメッセージ	ギフト包装	メーカー製造中止	配送パターン	販売形態(並行輸入品)	商品タックスコード	配送重量の単位	配送重量	商品説明の箇条書き1	商品説明の箇条書き2	商品説明の箇条書き3	商品説明の箇条書き4	商品説明の箇条書き5	検索キーワード	推奨ブラウズノード	スタイルキーワード	コート・ワンピース・チュニック着丈	商品メイン画像URL	カラーサンプル画像URL	商品のサブ画像URL11	商品のサブ画像URL12	商品のサブ画像URL13	商品のサブ画像URL14	商品のサブ画像URL15	商品のサブ画像URL16	商品のサブ画像URL17	商品のサブ画像URL18	商品パッケージの長さの単位	商品パッケージの長さ	商品パッケージの幅	商品パッケージの高さ	商品パッケージの重量の単位	商品パッケージの重量	フルフィルメントセンターID	親子関係の指定	親商品のSKU(商品管理番号)	親子関係のタイプ	バリエーションテーマ	サイズ	サイズマップ	カラー	カラーマップ	スタイル名	靴の幅	ストラップのタイプ	つま先の形状(トゥシェープ)	ウエストのスタイル	素材不透明度	ヒールのタイプ	シャフト(軸)の丈	表地素材	袖のタイプ	留め具のタイプ	ライフスタイル1	ライフスタイル2	素材または繊維	対象年齢・性別	アダルト商品	ウエストサイズの単位	ウエストサイズ	仕立ての長さの単位	仕立ての長さ	袖の長さの単位	袖の長さ	シャツカラースタイル	首のタイプ	首のサイズの単位	首のサイズ	ボトムススタイル	胸囲サイズの単位	胸囲サイズ	モデル年(発売年・発表年)	シーズン	収納可能サイズ	カップサイズ\n";

	//$head .= "item_sku	item_name	external_product_id	external_product_id_type	brand_name	product_subtype	part_number	product_description	update_delete	codabar	quantity	fulfillment_latency	condition_type	condition_note	standard_price	standard_price_points	product_site_launch_date	merchant_release_date	list_price	optional_payment_type_exclusion	delivery_schedule_group_id	sale_price	sale_price_points	sale_from_date	sale_end_date	restock_date	max_order_quantity	offering_can_be_gift_messaged	offering_can_be_giftwrapped	is_discontinued_by_manufacturer	merchant_shipping_group_name	distribution_designation	product_tax_code	website_shipping_weight_unit_of_measure	website_shipping_weight	bullet_point1	bullet_point2	bullet_point3	bullet_point4	bullet_point5	generic_keywords	recommended_browse_nodes	style_keywords	specific_uses_keywords	main_image_url	swatch_image_url	other_image_url1	other_image_url2	other_image_url3	other_image_url4	other_image_url5	other_image_url6	other_image_url7	other_image_url8	package_length_unit_of_measure	package_length	package_width	package_height	package_weight_unit_of_measure	package_weight	fulfillment_center_id	parent_child	parent_sku	relationship_type	variation_theme	size_name	size_map	color_name	color_map	style_name	shoe_width	strap_type	toe_shape	waist_style	opacity	heel_type	shaft_height	outer_material_type	sleeve_type	closure_type	lifestyle1	lifestyle2	fabric_type	department_name	is_adult_product	waist_size_unit_of_measure	waist_size	inseam_length_unit_of_measure	inseam_length	sleeve_length_unit_of_measure	sleeve_length	collar_style	neck_style	neck_size_unit_of_measure	neck_size	bottom_style	chest_size_unit_of_measure	chest_size	model_year	seasons	fit_to_size_description	cup_size\n";

	return $head;
}


//	スポーツ用ヘッダ
function amazon_tsv_head2() {

	// $head  = "TemplateType=sports	Version=2017.0109	上3行は Amazon.com 記入用です。上3行は変更または削除しないでください。.								出品情報-商品をサイト上で販売可能にする際に必要な項目																						寸法-商品のサイズや重量を入力する項目													商品検索情報-サーチ上で商品を検索されやすくするために必要な項目											画像-商品画像を表示させるために必要な項目。詳しくは画像説明タブを参照										出荷関連情報-フルフィルメント by Amazon (FBA) の利用、あるいは自社出荷の注文に関する出荷関連情報を、この項目に記入してください。FBA を利用する場合には必須の項目。							バリエーション-商品の色・サイズなどのバリエーションを作成する際に必須の項目				コンプライアンス情報-商品を販売する国または地域の特定商取引に遵守するために利用される項目			ProductTypeによって必須となる項目																																								\n";

	// $head .= "出品者SKU	商品コード(JANコード等)	商品コードのタイプ	商品名	商品説明文	メーカー名	メーカー型番	商品タイプ	ブランド名	アップデート・削除	商品の販売価格	ポイント	商品のコンディション	商品のコンディション説明	在庫数	メーカー希望価格	商品の公開日	予約商品の販売開始日	商品の入荷予定日	リードタイム(出荷までにかかる作業日数)	使用しない支払い方法	セール価格	セール時ポイント	配送日時指定SKUリスト	セール開始日	セール終了日	最大注文個数	ギフトメッセージ	ギフト包装	メーカー製造中止	配送パターン	販売形態(並行輸入品)	配送重量	配送重量の単位	ラインの巻き容量	リールの直径	リールの直径の単位	高さ	高さの単位	ロッド(棒)の長さ	ロッド(棒)の長さの単位	幅	幅の単位	重量	ロッド(棒)の重量の単位	商品の仕様1	商品の仕様2	商品の仕様3	商品の仕様4	商品の仕様5	推奨ブラウズノード	検索キーワード	対象1	対象2	スタイルキーワード	対象	商品メイン画像URL	カラーサンプル画像URL	商品のサブ画像URL1	商品のサブ画像URL2	商品のサブ画像URL3	商品のサブ画像URL4	商品のサブ画像URL5	商品のサブ画像URL6	商品のサブ画像URL7	商品のサブ画像URL8	フルフィルメントセンターID	商品パッケージの高さ	商品パッケージの幅	商品パッケージの長さ	商品パッケージの長さの単位	商品パッケージの重量	商品パッケージの重量の単位	親子関係の指定	親商品のSKU(商品管理番号)	親子関係のタイプ	バリエーションテーマ	安全性に関する注意	原産国	法規上の免責条項	保証	スタイル	カラー	カラーマップ	デザイン	素材	サイズマップ	サイズ	ゴルフクラブのシャフトの硬さ	ゴルフグラブのロフト（クラブフェースの傾斜角度）	グリップのサイズ	グリップのタイプ	グリップの素材タイプ	バイクのリムサイズ	バイクのリムサイズの単位	ホイールサイズ	ホイールサイズの単位	張力（テンション）	釣り糸素材のタイプ	最大の高さ（ピーク）　	容量の単位	レンズの色	ハンド	シャフト(軸)の長さ	シャフト(軸)の長さの単位	シャフト(軸)の素材	形状	対応重量	対応重量の単位	チーム名	魚種	電池付属	電池のタイプ	付属バッテリ個数	リチウム電池エネルギー含有量	リチウム電池パッケージ仕様	リチウム電池ボルト数	リチウム電池重量	配送オプション	席種	オプション\n";

	// $head .= "item_sku	external_product_id	external_product_id_type	item_name	product_description	manufacturer	part_number	feed_product_type	brand_name	update_delete	standard_price	standard_price_points	condition_type	condition_note	quantity	list_price	product_site_launch_date	merchant_release_date	restock_date	fulfillment_latency	optional_payment_type_exclusion	sale_price	sale_price_points	delivery_schedule_group_id	sale_from_date	sale_end_date	max_order_quantity	offering_can_be_gift_messaged	offering_can_be_giftwrapped	is_discontinued_by_manufacturer	merchant_shipping_group_name	distribution_designation	website_shipping_weight	website_shipping_weight_unit_of_measure	volume_capacity_name	item_display_diameter	item_display_diameter_unit_of_measure	item_display_height	item_display_height_unit_of_measure	item_display_length	item_display_length_unit_of_measure	item_display_width	item_display_width_unit_of_measure	item_display_weight	item_display_weight_unit_of_measure	bullet_point1	bullet_point2	bullet_point3	bullet_point4	bullet_point5	recommended_browse_nodes	generic_keywords	target_audience_keywords1	target_audience_keywords2	style_keywords	target_audience_base	main_image_url	swatch_image_url	other_image_url1	other_image_url2	other_image_url3	other_image_url4	other_image_url5	other_image_url6	other_image_url7	other_image_url8	fulfillment_center_id	package_height	package_width	package_length	package_length_unit_of_measure	package_weight	package_weight_unit_of_measure	parent_child	parent_sku	relationship_type	variation_theme	safety_warning	country_of_origin	legal_disclaimer_description	mfg_warranty_description_type	style_name	color_name	color_map	pattern_name	material_type	size_map	size_name	golf_club_flex	golf_club_loft	grip_size	grip_type	grip_material_type	rim_size	rim_size_unit_of_measure	wheel_size	wheel_size_unit_of_measure	tension_level	fishing_line_type	capacity	capacity_unit_of_measure	lens_color	hand_orientation	shaft_length	shaft_length_unit_of_measure	shaft_material	item_shape	display_maximum_weight_recommendation	display_maximum_weight_recommendation_unit_of_measure	team_name	specific_uses_for_product	are_batteries_included	battery_type	number_of_batteries	lithium_battery_energy_content	lithium_battery_packaging	lithium_battery_voltage	lithium_battery_weight	shipping_options	seating_group	option_type\n";

	$head ="TemplateType=fptcustom	Version=2022.1208	Category=sports	TemplateSignature=QklPTUVUUklDX01PTklUT1IsU1BPUlRfTUlUVCxCSUNZQ0xFX1RSQUlORVIsR09MRl9DTFVCX1BVVFRFUixVVElMSVRZX0hPTFNURVJfUE9VQ0gsRklUTkVTU19CRU5DSCxTUE9SVF9SQUNLRVQsU05PV1NIT0UsQUlNSU5HX1NDT1BFX1NJR0hULFNQT1JUX0JJTkRJTkcsU1BPUlRJTkdfR09PRFMsRklTSElOR19MSU5FLEdPTEZfQ0xVQl9XRURHRSxTUE9SVF9BQ1RJVklUWV9HTE9WRSxCQVJCRUxMLEZJU0hJTkdfRVFVSVBNRU5ULFdBVEVSX0ZMT1RBVElPTl9ERVZJQ0UsU05PV0JPQVJEX0JPT1QsR09MRl9DTFVCX0JBRyxTUE9SVF9XRVRTVUlULFJFQ1JFQVRJT05fQkFMTCxTTEVFUElOR19CQUcsQUlSX0dVTixHT0xGX0NMVUJfSVJPTixCQVNLRVRCQUxMX0hPT1AsQUJET01JTkFMX0VYRVJDSVNFUixFWEVSQ0lTRV9NQVQsU0xFRVBJTkdfTUFULFNUQVRJT05BUllfQklDWUNMRSxTUE9SVF9UQUJMRV9HQU1FLEdMT1ZFUyxST1dJTkdfTUFDSElORSxNRVNTX0tJVCxHT0xGX0NMVUJfV09PRCxHVU5fSE9MU1RFUixDWUNMSU5HX1RJUkUsR09MRl9DTFVCX0hZQlJJRCxXQUxLSU5HX1NUSUNLLFRFTlQsSlVNUF9ST1BFLEVYRVJDSVNFX0JBTkQsS0lDS19TQ09PVEVSLFJPTExFUl9TS0FURSxGSVNISU5HX0hPT0ssSUNFX0NIRVNULFNXRUFUQkFORCxDQU1QSU5HX0VRVUlQTUVOVCxDWUNMSU5HX0VRVUlQTUVOVCxUUkFNUE9MSU5FLEdPTEZfQ0xVQixCT1hJTkdfR0xPVkUsRklTSElOR19SRUVMLFNLQVRFQk9BUkQsVFJFQURNSUxMLFBVTkNISU5HX0JBRyxBSVJfUFVNUCxTUE9SVF9IRUxNRVQ=	settings=attributeRow=3&contentLanguageTag=ja_JP&dataRow=4&feedType=113&headerLanguageTag=ja_JP&isEdit=false&isExpose=false&isProcessingSummary=false&labelRow=2&primaryMarketplaceId=amzn1.mp.o.A1VC38T7YXB528&templateIdentifier=d7c254e1-fd65-4aaf-b2f8-89022f05935f&timestamp=2022-12-08T00%3A01%3A32.927Z	     上3行はAmazon.comのみで使用します。上3行は変更または削除しないでください。																																												画像									バリエーション							商品基本情報																					商品検索情報																																																																																																																																																																																																																														推奨ブラウズノード別の情報																																																																															寸法																																																																																																																出荷関連情報										コンプライアンス情報																																																									出品情報																															\n";
	$head .= "商品タイプ	出品者SKU	商品コード(JANコード等)	商品コードのタイプ	商品名	対象年齢・性別	商品の販売価格	在庫数	商品メイン画像URL	ブランド名	メーカー名	推奨ブラウズノード	前から後ろまでのアイテムの奥行き	品目の奥行きの単位	品目の左右の幅	品目の幅単位	品目の底部から上部までの高さ	品目の高さ単位	お客様の性別	unit_count	unit_count_type	お客様の年齢層	サイズ	品目の上端から下端までの長さ	品目の長さ単位	品目の左右の幅	幅単位	カラー	賞味期限管理商品	品目の上端から下端までの長さ	品目の長さ単位	品目の左右の幅	品目の幅単位	品目の厚さ	品目の厚さ単位	組み立て時の長辺の長さ	長さ単位	組み立て時の短辺の幅	幅単位	組み立て時のベースからトップまでの高さ	高さの単位	商品の直径	商品の直径の単位	item_width_numeric	item_width_numeric_unit_of_measure	品目の左右の幅	品目の幅単位	底部から上部までの高さ	品目の高さ単位	商品のサブ画像URL1	商品のサブ画像URL2	商品のサブ画像URL3	商品のサブ画像URL4	商品のサブ画像URL5	商品のサブ画像URL6	商品のサブ画像URL7	商品のサブ画像URL8	カラーサンプル画像URL	親子関係の指定	親商品のSKU(商品管理番号)	子関係タイプ	バリエーションテーマ	パッケージレベル	パッケージ内包数量	パッケージには属性が含まれています	アップデート・削除	商品説明文	メーカー型番	リールの型式名	モデル	バーコード適用除外理由	自社で管理しているSKUごとの番号	発売年・モデルイヤー	梱包タイプ	表地素材	インナーの素材	使用する燃料タイプ	ヒールのタイプ	ヒールの高さの単位	ソール材料	ヒールの高さ	温度の定格	お手入れ方法	お手入れ方法	お手入れ方法	お手入れ方法	商品の仕様	商品の仕様	商品の仕様	商品の仕様	商品の仕様	商品の仕様	商品の仕様	商品の仕様	商品の仕様	商品の仕様	スタイル名	検索キーワード	カラーマップ	素材の種類	シーズン	シーズン	シーズン	シーズン	スポーツの名称	スポーツの名称	ゴルフグラブのロフト（クラブフェースの傾斜角度）	グリップタイプ	レンズの色	レンズの素材	サイン入り商品	リーグ名	ハンド	シャフト(軸)の素材	プレーヤー名	チーム名	箱の中身	箱の中身	箱の中身	箱の中身	箱の中身	互換デバイス	互換デバイス	互換デバイス	互換デバイス	互換デバイス	毛皮の説明	golf_club_loft_unit_of_measure	席種	商品種別	オプション	対象	この商品はAmazon.co.jp限定商品ですか？	箱数	グローブのタイプ	防水タイプか否か	ライニングの説明	BMXバイクのタイプ	フレームの素材	設定数	ターゲットユーザーのキーワード	ターゲットユーザーのキーワード	ターゲットユーザーのキーワード	ターゲットユーザーのキーワード	ターゲットユーザーのキーワード	デザイン	スタイルキーワード	シャフトスタイルタイプ	リチウム電池ボルト数	プラチナキーワード	プラチナキーワード	プラチナキーワード	プラチナキーワード	プラチナキーワード	lithium_battery_voltage_unit_of_measure	ゴルフパターのライ角の単位	ゴルフパターのライ角の単位	ゴルフパターのライ角の単位	ゴルフパターのライ角の単位	ゴルフパターのライ角の単位	メーカー対象最高年齢	商品のオフセットの単位	オフセット	パターのライ角を指定する	パターのライ角を指定する	パターのライ角を指定する	パターのライ角を指定する	パターのライ角を指定する	メーカー対象最低年齢	ポケットの説明	ウェイトベンチの種類	対応スキルレベル	フィットタイプ	フィットタイプ	フィットタイプ	フィットタイプ	フィットタイプ	床の使用面積	首周りの形状(襟の形状)	パターンのスタイル	靴下の長さ	テーマ	トップのスタイル	UVプロテクションのレベル	ホイールタイプ	フレームタイプ	ブレーキのタイプ	サスペンションのタイプ	ドアの数	詰め物素材のタイプ	商品の据え付けタイプ	保温レベル	レンズの形状	絶縁タイプ	パッドのタイプ	ポール数	電球タイプ	座席数	電源	機能性	機能性	機能性	機能性	機能性	ラップトップ容量	バッテリ寿命	バッテリ寿命	バッテリ寿命	magnification_strength	wheelbase	wheelbase_unit_of_measure	shell_type	thesaurus_attribute_keywords	推奨使用用途	特定用途キーワード	特定用途キーワード	特定用途キーワード	特定用途キーワード	特定用途キーワード	thesaurus_subject_keywords1	thesaurus_subject_keywords2	thesaurus_subject_keywords3	thesaurus_subject_keywords4	thesaurus_subject_keywords5	magnification_strength_unit_of_measure	ラベルに記載されている国/地域	釣り技法	商品の補充情報	商品の補充情報	商品の補充情報	商品の補充情報	商品の補充情報	床の使用面積の単位	測定単位	バッテリ平均持続時間の単位	バッテリ平均持続時間の単位	バッテリ平均持続時間の単位	シャフトの円周	シャフトの円周の単位	つま先のスタイル	ミッドソール素材	仕切りの数	銃器の口径	エアガンパワータイプ	エアガンパワータイプ	エアガンパワータイプ	エアガンパワータイプ	エアガンパワータイプ	絶縁抵抗の単位	BTU で測定した絶縁	組み立て方法	ディスプレイタイプ	運動器具のアームタイプ	スペース効率	スペース効率	心拍数センサーのタイプ	商品の個数（ピース数）	溝深さ単位	溝深さ	ロックタイプ	ハンドル素材	収納時サイズ	ポール素材タイプ	ポール素材タイプ	ポール素材タイプ	ポール素材タイプ	ポール素材タイプ	ハンドルの直径	ハンドル長さの単位	ハンドル長さの十進数値	ハンドルのタイプ	ホイール素材	ホイールの数	対象となる動植物	対象となる動植物	対象となる動植物	対象となる動植物	対象となる動植物	チャンバーの深さの単位	チャンバーの深さ	バンド長	バンド材質タイプ	バンド色	バンド長の単位	速度測定	プログラム数	モーターサイズ	最大馬力単位	最低速度の単位	組み立てる必要がありますか?	最大速度の単位	最大速度	組み立て時間の単位	電源プラグ	組み立て時間	最低速度	動作モード	気流量	空気流量の測定単位	適合車両の種類	適合車両の種類	適合車両の種類	適合車両の種類	適合車両の種類	適合車両の種類	適合車両の種類	適合車両の種類	適合車両の種類	ゴルフクラブのシャフトの硬さ	張力（テンション）	代金引換	FCの保存可能期間の単位	配送オプション	コンビニ決済	レザーの種類	標準抵抗メカニズムの値	グリップ素材タイプ	釣り糸素材のタイプ	軸径の十進数値	軸径の単位	シャフト素材タイプ	メーカー推奨最高年齢	構成	メーカー推奨最少年齢	ポケットの数	Item Depth Unit Of Measure	スラット付きフレーム調整タイプ	調節可能な長さ	ストラップのタイプ	構造	ボトムのタイプ	脚のスタイル	袖のタイプ	サポートタイプ	線の太さ	速度の数	レベル数	装具タイプ	ギアループの数	最小胴体適合サイズ	就寝人数	最大倍率	使用容量の単位	台座タイプ	反射技術	base_material_type	仕上げ	仕上げ	シャフト長（10進値）	取付穴の単位	取り付け穴の直径の十進数値	ライフベストのタイプ	インソール素材	防しわ性	防しわ性	防しわ性	防しわ性	防しわ性	legal_compliance_certification_certifying_authority_name	legal_compliance_certification_geographic_jurisdiction	ラウンド数	バレル素材タイプ	品種	抵抗レベル数	駆動システム	トレッドタイプ	タイヤタイプ	レイヤ数	耐水技術	材料の特徴	ケーブル長（10進値）	十字穴番号	期間の単位	期間	バンド長	引張バネの数	ベアリング材質のタイプ	変速機減速比	デュロメーターの硬さ	トレッドミルの最大傾斜	サイクルタイプ	ベルト長派生	長ベルト長の測定単位	ベルト幅の単位	belt_length_string	ベルト幅	ベルト幅の十進数値	サイズマップ	グリップサイズ	定格内容積（総容積）	シャフト(軸)の長さ	シャフト(軸)の長さの単位	形状	対応重量	対応重量の単位	ロッド(棒)の長さ	ロッド(棒)の長さの単位	幅	幅の単位	商品の重量（パッケージを含まない）	ロッド(棒)の重量の単位	ヘッドサイズ名	商品の高さの単位	液体の容量	液体量の単位	商品本体サイズ：長さ	商品の幅の単位	容積容量名の単位	商品本体サイズ：幅	商品本体サイズ：高さ	商品の長さの単位	ホイールサイズ	ホイールサイズの単位	負荷容量	負荷容量の単位	配送重量	配送重量の単位	リムサイズ	バイクのリムサイズの単位	最大の高さ（ピーク）　	容量の単位	商品の直径	リールの直径の単位	商品の高さ	高さの単位	シャフト(軸)の丈	シャフトの高さの測定単位	商品の寸法の単位	商品の深さ	最大積載可能重量の単位	最大荷重の単位	メーカー推奨最大重量	推奨最大重量の単位	最低の高さの単位	最大高さ	最小の高さ	最大高さの単位	ブレーキの幅	ブレーキの幅の単位	フロアの長さ	フロアの幅	靴の幅	袖の長さ	ウエストのサイズ	クランクの長さ	トップチューブの長さ	股下の長さ	有効エッジの長さ	刃の長さ	メーカー推奨最大身長	破断強度	破断強度の単位	対物レンズのサイズ	商品対応範囲	商品の厚さ	袖の長さの単位	フロアの長さの単位	フロアの幅の単位	ウエストサイズの単位	クランクの長さの単位	トップチューブの長さの単位	有効エッジの長さの単位	刃の断面の形状の単位	推奨最高身長の単位	対物レンズのサイズの単位	商品対応範囲の単位	商品の厚さの単位	股下の長さの単位	靴幅の単位	高さマップ	胸囲サイズの単位	胸囲サイズ	強度	強度の単位	タイヤの直径	タイヤ直径の単位	拡張された長さ	伸びた長さの測定単位	ケーブル長さの単位	ハンドルの高さ	ハンドルの高さの単位	チャンバーの高さの単位	チャンバーの幅単位	チャンバーの幅	チャンバーの高さ	バンドの幅の単位	バンド幅	フレームの高さ	フレームの高さの単位	デッキの長さ	デッキの幅	デッキ長の単位	デッキ幅の単位	ディスプレイサイズ	ディスプレイ最大解像度の単位	ディスプレイサイズ	モニターサイズの単位	最大圧力の単位	最大圧力	フルフィルメントセンターID	商品パッケージの高さ	商品パッケージの幅	商品パッケージの長さ	商品パッケージの長さの単位	商品パッケージの重量	商品パッケージの重量の単位	商品パッケージの高さの単位	商品パッケージの幅の単位	商品パッケージの寸法の単位	原産国/地域	布タイプ	布タイプ	布タイプ	布タイプ	布タイプ	電池付属	この商品は電池本体ですか？または電池を使用した商品ですか？	電池の種類、サイズ	電池の種類、サイズ	電池の種類、サイズ	電池の数	電池の数	電池の数	電池当たりのワット時	リチウム電池パッケージ	リチウム含有量(グラム)	リチウムイオン電池単数	リチウムメタル電池単数	電池の組成	電池の重量(グラム)	電池の重量の測定単位	リチウム電池のエネルギー量測定単位	リチウム電池の重量の測定単位	適用される危険物関連法令	適用される危険物関連法令	適用される危険物関連法令	適用される危険物関連法令	適用される危険物関連法令	国連(UN)番号	安全データシート(SDS) URL	商品の重量	商品の重量の単位	商品の体積	商品の容量の単位	内用液は含まれていますか?	引火点(°C)	商品に適用される保管規制の区分	メーカー保証の説明	分類／危険物ラベル(適用されるものをすべて選択)	分類／危険物ラベル(適用されるものをすべて選択)	分類／危険物ラベル(適用されるものをすべて選択)	保証	安全性に関する注意	法規上の免責条項	特徴・用途	legal_compliance_certification_date_of_issue	import_designation	legal_compliance_certification_metadata	legal_compliance_certification_expiration_date	cpsia_cautionary_statement	cpsia_cautionary_description	適合性の認証の状況	規制機関の名前	コンプライアンス認証値	保証タイプ	熱に弱い	通貨コード	コンディション	商品のコンディション説明	メーカー希望価格	商品の公開日	予約商品の販売開始日	商品の入荷予定日	出荷作業日数	取り寄せ	使用しない支払い方法	セール価格	セール開始日	セール終了日	最大注文個数	パッケージ数	ギフトメッセージ	ギフト包装	配送パターン	販売形態(並行輸入品)	在庫期間	終了日を提案する	最低広告表示価格	賞味期限もしくは消費期限タイプ	付属品総数	開始日を提案する	商品タックスコード	ポイントパーセント	セール時ポイントパーセント	配送日時指定SKUリスト	メーカー製造中止	サイン入り	max_aggregate_ship_quantity\n";
	$head .= "feed_product_type	item_sku	external_product_id	external_product_id_type	item_name	department_name	standard_price	quantity	main_image_url	brand_name	manufacturer	recommended_browse_nodes	depth_front_to_back	depth_front_to_back_unit_of_measure	depth_width_side_to_side	depth_width_side_to_side_unit_of_measure	depth_height_floor_to_top	depth_height_floor_to_top_unit_of_measure	target_gender	unit_count	unit_count_type	age_range_description	size_name	length_longer_edge	length_longer_edge_unit_of_measure	width_shorter_edge	width_shorter_edge_unit_of_measure	color_name	is_expiration_dated_product	thickness_head_to_toe	thickness_head_to_toe_unit_of_measure	thickness_width_side_to_side	thickness_width_side_to_side_unit_of_measure	thickness_floor_to_top	thickness_floor_to_top_unit_of_measure	length_head_to_toe	length_head_to_toe_unit_of_measure	length_width_side_to_side	length_width_side_to_side_unit_of_measure	length_height_floor_to_top	length_height_floor_to_top_unit_of_measure	item_diameter_derived	item_diameter_unit_of_measure	item_width_numeric	item_width_numeric_unit_of_measure	height_width_side_to_side	height_width_side_to_side_unit_of_measure	height_floor_top	height_floor_top_unit_of_measure	other_image_url1	other_image_url2	other_image_url3	other_image_url4	other_image_url5	other_image_url6	other_image_url7	other_image_url8	swatch_image_url	parent_child	parent_sku	relationship_type	variation_theme	package_level	package_contains_quantity	package_contains_identifier	update_delete	product_description	part_number	model_name	model	gtin_exemption_reason	codabar	model_year	closure_type	outer_material_type	inner_material_type	fuel_type	heel_type	heel_height_unit_of_measure	sole_material	heel_height	temperature_rating	care_instructions1	care_instructions2	care_instructions3	care_instructions4	bullet_point1	bullet_point2	bullet_point3	bullet_point4	bullet_point5	bullet_point6	bullet_point7	bullet_point8	bullet_point9	bullet_point10	style_name	generic_keywords	color_map	material_type	seasons1	seasons2	seasons3	seasons4	sport_type1	sport_type2	golf_club_loft	grip_type	lens_color	lens_material_type	is_autographed	league_name	hand_orientation	shaft_material	athlete	team_name	included_components1	included_components2	included_components3	included_components4	included_components5	compatible_devices1	compatible_devices2	compatible_devices3	compatible_devices4	compatible_devices5	fur_description	golf_club_loft_unit_of_measure	seating_group	item_type_name	option_type	target_audience_base	is_exclusive_product	number_of_boxes	glove_type	water_resistance_level	lining_description	bike_type	frame_material_type	num_of_settings	target_audience_keywords1	target_audience_keywords2	target_audience_keywords3	target_audience_keywords4	target_audience_keywords5	pattern_name	style_keywords	shaft_style_type	lithium_battery_voltage	platinum_keywords1	platinum_keywords2	platinum_keywords3	platinum_keywords4	platinum_keywords5	lithium_battery_voltage_unit_of_measure	golf_putter_lie_angle_unit1	golf_putter_lie_angle_unit2	golf_putter_lie_angle_unit3	golf_putter_lie_angle_unit4	golf_putter_lie_angle_unit5	mfg_maximum_unit_of_measure	item_offset_unit_of_measure	item_offset	golf_putter_lie_angle1	golf_putter_lie_angle2	golf_putter_lie_angle3	golf_putter_lie_angle4	golf_putter_lie_angle5	mfg_minimum_unit_of_measure	pocket_description	weight_bench_type	skill_level	fit_type1	fit_type2	fit_type3	fit_type4	fit_type5	floor_area	neck_style	pattern_type	rise_style	theme	top_style	ultraviolet_light_protection	wheel_type	frame_type	brake_style	suspension_type	number_of_doors	fill_material_type	mounting_type	temperature_rating_degrees	lens_type	insulation_material_type	pad_type	number_of_poles	light_source_type	seating_capacity	power_source_type	special_features1	special_features2	special_features3	special_features4	special_features5	capacity_name	battery_average_life1	battery_average_life2	battery_average_life3	magnification_strength	wheelbase	wheelbase_unit_of_measure	shell_type	thesaurus_attribute_keywords	recommended_uses_for_product	specific_uses_keywords1	specific_uses_keywords2	specific_uses_keywords3	specific_uses_keywords4	specific_uses_keywords5	thesaurus_subject_keywords1	thesaurus_subject_keywords2	thesaurus_subject_keywords3	thesaurus_subject_keywords4	thesaurus_subject_keywords5	magnification_strength_unit_of_measure	country_as_labeled	fishing_technique	lifecycle_supply_type1	lifecycle_supply_type2	lifecycle_supply_type3	lifecycle_supply_type4	lifecycle_supply_type5	floor_area_unit_of_measure	temperature_rating_degrees_unit_of_measure	battery_average_life_unit_of_measure1	battery_average_life_unit_of_measure2	battery_average_life_unit_of_measure3	shaft_circumference	shaft_circumference_unit_of_measure	toe_style	midsole_material_type	number_of_dividers	caliber	air_gun_power_type1	air_gun_power_type2	air_gun_power_type3	air_gun_power_type4	air_gun_power_type5	insulation_resistance_unit_of_measure	insulation_resistance	assembly_instructions	display_type	exercise_machine_arm_type	space_efficiency1	space_efficiency2	heart_rate_sensor_type	number_of_pieces	tread_depth_unit_of_measure	tread_depth	lock_type	handle_material	folded_size	pole_material_type1	pole_material_type2	pole_material_type3	pole_material_type4	pole_material_type5	handle_diameter	handle_length_unit	handle_length_derived	handle_type	wheel_material	number_of_wheels	target_species1	target_species2	target_species3	target_species4	target_species5	chamber_depth_unit_of_measure	chamber_depth	band_length	band_material_type	band_color	band_length_unit	speed_rating	number_of_programs	maximum_horsepower	maximum_horsepower_unit_of_measure	minimum_speed_unit_of_measure	is_assembly_required	maximum_speed_unit_of_measure	maximum_speed	assembly_time_unit_of_measure	power_plug_type	assembly_time	minimum_speed	operation_mode	air_flow_capacity	air_flow_capacity_unit_of_measure	compatible_with_vehicle_type1	compatible_with_vehicle_type2	compatible_with_vehicle_type3	compatible_with_vehicle_type4	compatible_with_vehicle_type5	compatible_with_vehicle_type6	compatible_with_vehicle_type7	compatible_with_vehicle_type8	compatible_with_vehicle_type9	golf_club_flex	tension_level	is_cod_included	fc_shelf_life_unit_of_measure	shipping_options	is_cvs_included	leather_type	resistance_mechanism	grip_material_type	fishing_line_type	shaft_diameter_derived	shaft_diameter_unit_of_measure	shaft_material_type	mfg_maximum	configuration	mfg_minimum	number_of_pockets	item_depth_unit_of_measure	adjustment_type	has_adjustable_length	strap_type	construction_type	bottom_style	leg_style	sleeve_type	support_type	line_weight	number_of_speeds	number_of_power_levels	fitting_type	number_of_gear_loops	minimum_torso_measurement	occupancy	magnification_maximum	capacity_name_unit_of_measure	strand_type	reflecting_technology	base_material_type	finish_type1	finish_type2	shaft_length_derived	mounting_hole_diameter_unit_of_measure	mounting_hole_diameter_derived	life_vest_type	insole_material_type	wrinkle_resistance1	wrinkle_resistance2	wrinkle_resistance3	wrinkle_resistance4	wrinkle_resistance5	legal_compliance_certification_certifying_authority_name	legal_compliance_certification_geographic_jurisdiction	number_of_rounds	barrel_material_type	variety	number_of_resistance_levels	drive_system	tread_type	tire_type	number_of_layers	water_resistance_technology	material_features	cable_length_derived	screw_point_style	duration_unit_of_measure	duration	band_size	number_of_springs	bearing_material_type	gearbox_ratio	durometer_hardness	maximum_incline_percentage	motor_type	belt_length_derived	belt_length_unit_of_measure	belt_width_unit_of_measure	belt_length_string	belt_width_string	belt_width_derived	size_map	grip_size	volume_capacity_name	shaft_length	shaft_length_unit_of_measure	item_shape	display_maximum_weight_recommendation	display_maximum_weight_recommendation_unit_of_measure	item_display_length	item_display_length_unit_of_measure	item_display_width	item_display_width_unit_of_measure	item_display_weight	item_display_weight_unit_of_measure	head_size_name	item_height_unit_of_measure	liquid_volume	liquid_volume_unit_of_measure	item_length	item_width_unit_of_measure	volume_capacity_name_unit_of_measure	item_width	item_height	item_length_unit_of_measure	wheel_size	wheel_size_unit_of_measure	load_capacity	load_capacity_unit_of_measure	website_shipping_weight	website_shipping_weight_unit_of_measure	rim_size	rim_size_unit_of_measure	capacity	capacity_unit_of_measure	item_display_diameter	item_display_diameter_unit_of_measure	item_display_height	item_display_height_unit_of_measure	shaft_height	shaft_height_unit_of_measure	item_dimensions_unit_of_measure	item_depth	maximum_weight_capacity	maximum_weight_capacity_unit_of_measure	maximum_weight_recommendation	maximum_weight_recommendation_unit_of_measure	minimum_height_unit_of_measure	maximum_height	minimum_height	maximum_height_unit_of_measure	brake_width	brake_width_unit_of_measure	floor_length	floor_width	shoe_width	sleeve_length	waist_size	crank_length	top_tube_length	inseam_length	effective_edge_length	blade_length	maximum_height_recommendation	breaking_strength	breaking_strength_unit_of_measure	objective_lens_diameter	item_area	item_thickness_derived	sleeve_length_unit_of_measure	floor_length_unit_of_measure	floor_width_unit_of_measure	waist_size_unit_of_measure	crank_length_unit_of_measure	top_tube_length_unit_of_measure	effective_edge_length_unit_of_measure	blade_length_unit_of_measure	maximum_height_recommendation_unit_of_measure	objective_lens_diameter_unit_of_measure	item_area_unit_of_measure	item_thickness_unit_of_measure	inseam_length_unit_of_measure	shoe_width_unit_of_measure	height_map	chest_size_unit_of_measure	chest_size	tensile_strength	tensile_strength_unit_of_measure	tire_diameter	tire_diameter_unit_of_measure	extended_length	extended_length_unit_of_measure	cable_length_unit_of_measure	handle_height	handle_height_unit_of_measure	chamber_height_unit_of_measure	chamber_width_unit_of_measure	chamber_width	chamber_height	band_width_unit_of_measure	band_width	frame_size	frame_size_unit_of_measure	deck_length	deck_width	deck_length_unit_of_measure	deck_width_unit_of_measure	display_size	display_size_unit_of_measure	monitor_size	monitor_size_unit_of_measure	maximum_pressure_unit_of_measure	maximum_pressure	fulfillment_center_id	package_height	package_width	package_length	package_length_unit_of_measure	package_weight	package_weight_unit_of_measure	package_height_unit_of_measure	package_width_unit_of_measure	package_dimensions_unit_of_measure	country_of_origin	fabric_type1	fabric_type2	fabric_type3	fabric_type4	fabric_type5	are_batteries_included	batteries_required	battery_type1	battery_type2	battery_type3	number_of_batteries1	number_of_batteries2	number_of_batteries3	lithium_battery_energy_content	lithium_battery_packaging	lithium_battery_weight	number_of_lithium_ion_cells	number_of_lithium_metal_cells	battery_cell_composition	battery_weight	battery_weight_unit_of_measure	lithium_battery_energy_content_unit_of_measure	lithium_battery_weight_unit_of_measure	supplier_declared_dg_hz_regulation1	supplier_declared_dg_hz_regulation2	supplier_declared_dg_hz_regulation3	supplier_declared_dg_hz_regulation4	supplier_declared_dg_hz_regulation5	hazmat_united_nations_regulatory_id	safety_data_sheet_url	item_weight	item_weight_unit_of_measure	item_volume	item_volume_unit_of_measure	contains_liquid_contents	flash_point	fulfillment_center_special_storage	warranty_description	ghs_classification_class1	ghs_classification_class2	ghs_classification_class3	mfg_warranty_description_type	safety_warning	legal_disclaimer_description	specific_uses_for_product	legal_compliance_certification_date_of_issue	import_designation	legal_compliance_certification_metadata	legal_compliance_certification_expiration_date	cpsia_cautionary_statement	cpsia_cautionary_description	legal_compliance_certification_status	legal_compliance_certification_regulatory_organization_name	legal_compliance_certification_value	warranty_type	is_heat_sensitive	currency	condition_type	condition_note	list_price	product_site_launch_date	merchant_release_date	restock_date	fulfillment_latency	is_sourcing_on_demand	optional_payment_type_exclusion	sale_price	sale_from_date	sale_end_date	max_order_quantity	item_package_quantity	offering_can_be_gift_messaged	offering_can_be_giftwrapped	merchant_shipping_group_name	distribution_designation	fc_shelf_life	offering_end_date	map_price	product_expiration_type	number_of_items	offering_start_date	product_tax_code	standard_price_points_percent	sale_price_points_percent	delivery_schedule_group_id	is_discontinued_by_manufacturer	signed_by	max_aggregate_ship_quantity\n";
	return $head;
}



//	Amazon ASIN登録
//	add ohkawara 2019/11/20
function regist_asin(&$AMZ_UP_LIST) {
	global $conn_id;

	//	商品情報を取得
	$sql  = "SELECT g_num, size_list FROM goods;";
	if ($result = mysqli_query($conn_id,$sql)) {
		while ($list = mysqli_fetch_array($result)) {

			$g_num = $list['g_num'];
			$c_p_sku = "g".$g_num;				//	親SKU作成
			$size_list = $list['size_list'];

			$op_size = unserialize($size_list);
			if ($op_size) {
				$up_flg = 0;
				$none_flg = 0;

				//	親情報のSKU登録が有った場合
				if ($AMZ_UP_LIST[$c_p_sku]['asin'] != "") {
					$op_size[0]['jan'] = "par";
					$op_size[0]['asin'] = $AMZ_UP_LIST[$c_p_sku]['asin'];
					$up_flg = 1;
					$none_flg = 1;
				}

				foreach ($op_size AS $key => $VAL) {
					$c_jan = $VAL['jan'];				//	変更するサイズを取得
					$c_size = $VAL['size'];				//	変更するサイズを取得
					$c_sku  = $c_p_sku."-".$c_size;		//	SKUを作成

					//	親情報だったら場合
					if ($c_jan == "par") {
						continue;
					}

					//	オリジナル商品か確認
					if ($c_jan == "none") {
						$none_flg = 1;
					}

					//	出品レポートにASINが記録されていたならば配列に入れる
					if ($AMZ_UP_LIST[$c_sku]['asin'] != "") {
						$op_size[$key]['asin'] = $AMZ_UP_LIST[$c_sku]['asin'];
						$up_flg = 1;
					}

					//	サイズの状態がNEW(2)になっていたら販売(1)に変える
					if ($op_size[$key]['type'] == 2) {
						$op_size[$key]['type'] = 1;
						$up_flg = 1;
					}
				}

				//	オリジナル商品情報に更新があったらDBに反映する。
				if ($up_flg == 1 && $none_flg == 1) {
					$size_list = serialize($op_size);

					$sql  = "UPDATE ".T_GOODS." SET".
							" size_list='".$size_list."'".
							" WHERE g_num='".$g_num."';";
					if (!mysqli_query($conn_id,$sql)) { $ERROR[] = "商品データーを変更できませんでした。(1):".$g_num; }
				}
			}

		}
	}

}



