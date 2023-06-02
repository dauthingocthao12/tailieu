<?php
//	Wowma用CSV作成ファイル

function make_csv_wowma(&$msg = null,&$ERROR = null) {
global $conn_id;

	//	Wowma処理-----------------------------------------------------------------------------------------
	//	add ohkawara 2017/02/09

	//	アップ済み出品レポートファイル取得
	setlocale( LC_ALL, 'ja_JP' );
	//	登録済み在庫情報
	$WOWMA_UP_LIST = array();
	$WOWMA_UP_NAME_LIST = array();
	//if (file_exists(WOWMA_B_ITEM_FILE)) {		//	del ohkawara 2018/04/03
	//	$LIST = file(WOWMA_B_ITEM_FILE);		//	del ohkawara 2018/04/03
	if (file_exists(WOWMA_B_STOCK_FILE)) {		//	add ohkawara 2018/04/03
		$LIST = file(WOWMA_B_STOCK_FILE);		//	add ohkawara 2018/04/03
#		mb_convert_variables("eucjp-win", "sjis-win", $LIST);
		mb_convert_variables("UTF-8", "sjis-win", $LIST);
		foreach ($LIST AS $key => $VAL) {
			if ($key < 1) {
				$WOWMA_UP_NAME_LIST = str_getcsv($VAL,',','"');
			} else {
				$SET_LIST = array();
				$list = str_getcsv($VAL,',','"');
				foreach ($WOWMA_UP_NAME_LIST AS $keys => $keysname) {
					$SET_LIST[$keysname] = $list[$keys];
				}

				// add ohkawara 2018/04/03
				//	サイズ設定が有る場合上書きされてしまう為、サイズの入れ替え作業を行う
				if ($SET_LIST['choicesStockHorizontalName']) {
					$size = $SET_LIST['choicesStockHorizontalName'];
					$SET_LIST['size'][$size] = $SET_LIST['choicesStockCount'];
				}

				//	資料
				//	$SET_LIST['itemCode']	g_num
				//	$SET_LIST['stockSegment']	1 or 2
				//	$SET_LIST['stockCount']	サイズ無　数
				//	$SET_LIST['choicesStockHorizontalName']	サイズ名
				//	$SET_LIST['choicesStockCount']	サイズ有　数

				//$Code = $SET_LIST['Code'];	//	del ohkawara 2018/04/02
				$Code = $SET_LIST['itemCode'];	//	add ohkawara 2018/04/02
				$g_num = preg_replace("/[^0-9]/", "", $Code);

				$WOWMA_UP_LIST[$g_num] = $SET_LIST;
			}
/*
			//	カテゴリー変更時処理	普段はコメント	ohkawara 2017/04/14
			$BCategID = $SET_LIST[BCategID];
			if ($g_num && $BCategID) {
				$sql  = "UPDATE ".T_GOODS." SET wd_id='".$BCategID."'".
						" WHERE g_num='".$g_num."';";
				if (!mysqcli_query($conn_id,$sql)) { $ERROR[] = "Wowma!商品オプションデーターを更新できませんでした。$g_num"; }
			}
			$g_num = "";
			$BCategID = "";
*/

		}
	}

	//	登録済み商品情報	add ohkawara 2018/04/12
	$WOWMA_UP_ITEM_LIST = array();
	$WOWMA_UP_ITEM_NAME_LIST = array();
	if (file_exists(WOWMA_B_ITEM_FILE)) {
		$LIST = file(WOWMA_B_ITEM_FILE);
#		mb_convert_variables("eucjp-win", "sjis-win", $LIST);
		mb_convert_variables("UTF-8", "sjis-win", $LIST);
		foreach ($LIST AS $key => $VAL) {
			if ($key < 1) {
				$WOWMA_UP_ITEM_NAME_LIST = str_getcsv($VAL,',','"');
			} else {
				$SET_LIST = array();
				$list = str_getcsv($VAL,',','"');
				foreach ($WOWMA_UP_ITEM_NAME_LIST AS $keys => $keysname) {
					$SET_LIST[$keysname] = $list[$keys];
				}
			}

			$check_g_num = $SET_LIST['itemCode'];
			$check_g_num = preg_replace("/[^0-9]/", "", $check_g_num);
			//$check_state = $SET_LIST['saleStatus'];
			if ($WOWMA_UP_LIST[$check_g_num]) {
			//	if ($check_state == 2) {
			//		unset($WOWMA_UP_LIST[$check_g_num]);
			//	} else {
					foreach ($SET_LIST AS $key_name => $VALUES) {
						$WOWMA_UP_LIST[$check_g_num][$key_name] = $VALUES;
					}
			//	}
			}
		}
	}


	$list_item_csv = "";
	$list_zaiko_csv = "";
	$sql  = "SELECT goods.* FROM ".T_GOODS." goods".
			" INNER JOIN (".
			"	select distinct g_num from ".T_CATEGORY.
			"	 where category.display='1' AND cate1>='20' AND state!='3' AND cate1 NOT IN ('98','99')".
			"	) category_s ON goods.g_num=category_s.g_num".
			//" INNER JOIN mool_wd mool_wd ON goods.wd_id=mool_wd.dir_id".
			" INNER JOIN mool_wd mool_wd ON cast(goods.wd_id AS SIGNED)=mool_wd.dir_id".
			" WHERE goods.wowmashop='1'".
			" AND goods.soldout!='1'".
			" ORDER BY goods.g_num;";
	if ($result = mysqli_query($conn_id,$sql)) {
		while ($list = mysqli_fetch_array($result)) {
			$g_num = $list['g_num'];
			$del_flg = 0;
			make_wowma_csv($list, $WOWMA_UP_LIST, $list_item_csv, $list_zaiko_csv, $del_flg);
		}
	}

	//	add ohkawara 2018/04/03
	//	$WOWMA_UP_LIST整理	販売終了商品データー作成
	if (count($WOWMA_UP_LIST)) {
		del_wowma_data($WOWMA_UP_LIST, $list_item_csv, $list_zaiko_csv);
	}


	//	削除した商品の未処理の時の処理
	//	del ohkawara 2018/04/09
	//if ($WOWMA_UP_LIST) {
	//	foreach ($WOWMA_UP_LIST AS $g_num => $list) {
	//		$del_flg = 1;
	//		make_wowma_csv($list, $WOWMA_UP_LIST, $list_item_csv, $list_zaiko_csv, $del_flg);
	//	}
	//	unset($WOWMA_UP_LIST);
	//}

	$wowma_item_csv = "";
	if ($list_item_csv) {
		//$wowma_item_csv  = "Code,BCategID,KtaiTitle,KtaiPrice,KtaiCaption,KtaiDealType,KtaiChargeType,AdmID,StockNum,Keyword,KtaiUserRate,KtaiImageLinkName1,KtaiImageType1,KtaiImageURL1,KtaiImageLinkName2,KtaiImageType2,KtaiImageURL2,KtaiImageLinkName3,KtaiImageType3,KtaiImageURL3,KtaiImageLinkName4,KtaiImageType4,KtaiImageURL4,KtaiImageLinkName5,KtaiImageType5,KtaiImageURL5,KtaiImageLinkName6,KtaiImageType6,KtaiImageURL6,KtaiImageLinkName7,KtaiImageType7,KtaiImageURL7,KtaiImageLinkName8,KtaiImageType8,KtaiImageURL8,KtaiImageLinkName9,KtaiImageType9,KtaiImageURL9,KtaiImageLinkName10,KtaiImageType10,KtaiImageURL10,ShipFeeType,ShipFee,DeliveryType,KtaiShipComment,KtaiDealComment,KtaiPriceLabel1,KtaiPriceLabel2,KtaiCaptionPC,KtaiCaptionSP,KtaiDetailLinkName,KtaiDetailComment,Option1,Option2,Option3,Option4,Option5,SalesStartDate,StartDate,ExhibitStatus,AdmTitle,JanCode,IsbnCode,NameModel,SecretPagePassword,SecretPageTitle,SecretPageMessage,StockShow,KtaiNoStockMessage,KtaiSpecLinkName,KtaiSpec1,KtaiSpec2,KtaiSpec3,KtaiSpec4,KtaiSpec5,MaxBuyNum,KtaiStockShowStartNum,KtaiStockMessage1,KtaiStockMessage2,Tax,StockType,SkuOptionNameCol,SkuOptionValueCol,SkuOptionTypeCol,SkuOptionNameRow,SkuOptionValueRow,SkuOptionTypeRow,SkuHeader,SkuFooter,SkuStockShowType,SkuLimitNum,exhibittype\r\n";		//	del ohkawara 2018/04/02

		$wowma_item_csv  = "ctrlCol,lotNumber,itemName,itemManagementId,itemManagementName,itemCode,itemPrice,sellMethodSegment,releaseDate,reserveRegstDate,taxSegment,postageSegment,postage,deliveryId,deliveryMethodId1,deliveryMethodId2,deliveryMethodId3,deliveryMethodId4,deliveryMethodId5,deliveryMethodName1,deliveryMethodName2,deliveryMethodName3,deliveryMethodName4,deliveryMethodName5,publicStartDate,limitedOrderSegment,limitedOrderCount,description,descriptionForSP,descriptionForPC,detailTitle,detailDescription,specTitle,spec1,spec2,spec3,spec4,spec5,searchKeyword1,searchKeyword2,searchKeyword3,imageName1,imageName2,imageName3,imageName4,imageName5,imageName6,imageName7,imageName8,imageName9,imageName10,imageUrl1,imageUrl2,imageUrl3,imageUrl4,imageUrl5,imageUrl6,imageUrl7,imageUrl8,imageUrl9,imageUrl10,categoryId,tagId,jan,isbn,itemModel,limitedPasswd,limitedPasswdPageTitle,limitedPasswdPageMessage,saleStatus,itemOption1,itemOption2,itemOption3,itemOption4,itemOption5,itemOption6,itemOption7,itemOption8,itemOption9,itemOption10,itemOption11,itemOption12,itemOption13,itemOption14,itemOption15,itemOption16,itemOption17,itemOption18,itemOption19,itemOption20,pointRate,stockSegment,stockShippingDayId,stockShippingDayDispTxt,displayBackorderMessage,stockCount,displayStockSegment,displayStockThreshold,choicesStockHorizontalItemName,choicesStockVerticalItemName,choicesStockUpperDescription,choicesStockLowerDescription,displayChoicesStockSegment,displayChoicesStockThreshold\r\n";	//	add ohkawara 2018/04/02

		$wowma_item_csv .= $list_item_csv;

#		$wowma_item_csv = mb_convert_encoding($wowma_item_csv,"sjis-win","eucjp-win");
		$wowma_item_csv = mb_convert_encoding($wowma_item_csv,"sjis-win","UTF-8");

		$OUT = fopen(WOWMA_ITEM_FILE,"w");
		fwrite($OUT,$wowma_item_csv);
		fclose($OUT);
		@chmod(WOWMA_ITEM_FILE,0666);
		$msg .= "Wowma商品csvファイル作成完了いたしました。<br>\n<br>\n";
	} else {
		if (file_exists(WOWMA_ITEM_FILE)) { unlink(WOWMA_ITEM_FILE); }
		$msg .= "最新の情報がないのでWowma商品csvファイル作成しませんでした。<br>\n<br>\n";
	}

	$wowma_zaiko_csv = "";
	if ($list_zaiko_csv) {
		//$wowma_zaiko_csv  = "Code,SkuOptionTypeCol,SkuOptionTypeRow,SkuItemNum\r\n";		//	del ohkawara 2018/04/02
		//$wowma_zaiko_csv  = "ctrlCol,lotNumber,itemCode,stockSegment,stockCount,choicesStockHorizontalName,choicesStockHorizontalCode,choicesStockHorizontalSeq,choicesStockVerticalName,choicesStockVerticalCode,choicesStockVerticalSeq,choicesStockCount,choicesStockShippingDayId,choicesStockShippingDayDispTxt\r\n";		//	add ohkawara 2018/04/02	//	del ohkawara 2018/12/17
		$wowma_zaiko_csv  = "ctrlCol,lotNumber,itemCode,stockSegment,stockCount,choicesStockHorizontalName,choicesStockHorizontalCode,choicesStockHorizontalSeq,choicesStockVerticalName,choicesStockVerticalCode,choicesStockVerticalSeq,choicesStockCount,choicesStockShippingDayId,choicesStockShippingDayDispTxt,choicesStockImageUrl,choicesStockColorSegment\r\n";		//	add ohkawara 2018/12/17
		$wowma_zaiko_csv .= $list_zaiko_csv;

#		$wowma_zaiko_csv = mb_convert_encoding($wowma_zaiko_csv,"sjis-win","eucjp-win");
		$wowma_zaiko_csv = mb_convert_encoding($wowma_zaiko_csv,"sjis-win","UTF-8");

		$OUT = fopen(WOWMA_ZAIKO_FILE,"w");
		fwrite($OUT,$wowma_zaiko_csv);
		fclose($OUT);
		@chmod(WOWMA_ZAIKO_FILE,0666);
		$msg .= "Wowma在庫csvファイル作成完了いたしました。<br>\n<br>\n";
	} else {
		if (file_exists(WOWMA_ZAIKO_FILE)) { unlink(WOWMA_ZAIKO_FILE); }
		$msg .= "最新の情報がないのでWowma在庫csvファイル作成しませんでした。<br>\n<br>\n";
	}

	//	add ohkawara 2018/04/03
	//	メモリークリアー
	unset($list_item_csv);
	unset($list_zaiko_csv);
	unset($WOWMA_UP_LIST);

/*-----------------------------------------------------------------------------*/

return array($msg,$ERROR);
}

//	Wowma
//	add ohkawara 2017/02/09
function make_wowma_csv($list, &$WOWMA_UP_LIST, &$list_item_csv, &$list_zaiko_csv, $del_flg) {
	global $WOWMA_GOODS_DISCOUNT_CATE;
	global $attention_wowma;
	global $L_BRAND;

	$size_list = $list['size_list'];

	$g_num = $list['g_num'];
	$wd_id = $list['wd_id'];
	$g_name = $list['g_name'];
	$base_price = $list['price'];
	$base_sale_price = $list['sale_price'];
	$caption = $list['caption'];
	$brand = $list['brand'];
	$code = $list['code'];
	$generic_keywords1 = $list['generic_keywords1'];
	$generic_keywords2 = $list['generic_keywords2'];
	$generic_keywords3 = $list['generic_keywords3'];
	$generic_keywords4 = $list['generic_keywords4'];
	$generic_keywords5 = $list['generic_keywords5'];

	$zaiko = $list['zaiko'];		//	add ohkawara 2017/06/15




	//--------------------------------------------//

//	アップ済み商品のチェックとフラグを追加
	//	サイズが有る無しによって設定が有るので、サイズ確認を最初に行う
	$op_size = unserialize($size_list);

	$ALL_SIZE = array();
	$hanbai_flg = 0;	//	最終的に0だったら販売終了商品
	if ($op_size) {
		$i = 1;
		$asin_flg = 0;
		foreach ($op_size AS $key => $VAL) {
			if ($VAL['jan'] == "none") {
				$VAL['jan'] = "";
			}
			if ($VAL['type'] != 3) {
				$hanbai_flg = 1;
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
				if ($i == 1 && !$ALL_SIZE[0]) {
					$ALL_SIZE[0]['size'] = "";
					$ALL_SIZE[0]['jan'] = "";
					$ALL_SIZE[0]['asin'] = "";
					$ALL_SIZE[0]['type'] = "";
				}
				$ALL_SIZE[$i]['size'] = $check_size;
				$ALL_SIZE[$i]['jan'] = $VAL['jan'];
				$ALL_SIZE[$i]['asin'] = $VAL['asin'];
				$ALL_SIZE[$i]['type'] = $VAL['type'];
				$i++;
			} else {
				$ALL_SIZE[0]['size'] = "";
				$ALL_SIZE[0]['jan'] = $VAL['jan'];
				$ALL_SIZE[0]['asin'] = $VAL['asin'];
				$ALL_SIZE[0]['type'] = $VAL['type'];
			}
		}
	}
	//	$ALL_SIZEの設定がなければこの商品の登録はしない（不具合があった場合）
	if (count($ALL_SIZE) < 1) {
		unset($WOWMA_UP_LIST[$g_num]);	//	add ohkawara 2018/09/14
		return;
	}

	$size_option_flg = 0;
	//if ($ALL_SIZE[1]['size'] != "") {		//	del ohkawara 2018/12/17
	if (!empty($ALL_SIZE[1]['size'])) {		//	add ohkawara 2018/12/17
		$size_option_flg = 1;
	}

	//--------------------------------------------//

	//	コントロールカラム
	$ctrlCol = "N";		//	N:新規登録	U:更新
	if ($WOWMA_UP_LIST[$g_num]) {
		$ctrlCol = "U";
	}

	//	ロットナンバー
	$lotNumber = $WOWMA_UP_LIST[$g_num]['lotNumber'];

	//	商品名
	$itemName = $g_name;

	//	管理用ID
	$itemManagementId = $code;

	//	管理用商品名	未設定
	$itemManagementName = "";

	//	商品コード
	$itemCode = "g".$g_num;

	//	販売価格
	//	割引設定項目
	$GOODS_DISCOUNT_C = WOWMA_GOODS_DISCOUNT_C;
	$GOODS_DISCOUNT_CATE = $WOWMA_GOODS_DISCOUNT_CATE;
	$DISCOUNT_PAR = WOWMA_DISCOUNT_PAR;
	$DISCOUNT_PAR2 = WOWMA_DISCOUNT_PAR2;

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

	$price = floor($price * ((TAX / 100) + 1) + 0.5);
	$sale_price = floor($sale_price * ((TAX / 100) + 1) + 0.5);
	if ($sale_price < 1) { unset($sale_price); }
	$itemPrice = $price;
	if ($sale_price > 0) {
		$itemPrice = $sale_price;
	}

	//	販売方法区分		1:通常販売（空白で入力しても自動的に1（通常販売）で登録する）	2:予約販売
	$sellMethodSegment = "";

	//	発売(入荷)予定日	日付フォーマット　"yyyyMMdd"	※15日後から1年先までの日付を指定できる。
	$releaseDate = "";

	//	予約登録日	日付フォーマット　"yyyyMMdd"	※15日後から1年先までの日付を指定できる。
	$reserveRegstDate = "";

	//	税区分	1:内税	2:税なし	半角英字 大文字	※外税指定はありません。
	$taxSegment = 1;

	//	送料設定区分	1:送料別	2:送料無料	3:個別送料
	$postageSegment = "1";

	//	個別送料
	$postage = "";	//	設定無し

	//	(旧)配送方法ID	M:メール便
	$deliveryId = "";	//	設定無し

	//	(新)配送方法ID1～5		//	設定無し
	$deliveryMethodId1 = "";
	$deliveryMethodId2 = "";
	$deliveryMethodId3 = "";
	$deliveryMethodId4 = "";
	$deliveryMethodId5 = "";

	//	(新)配送方法名1～5		//	設定無し
	$deliveryMethodName1 = "";
	$deliveryMethodName2 = "";
	$deliveryMethodName3 = "";
	$deliveryMethodName4 = "";
	$deliveryMethodName5 = "";

	//	掲載開始日時	//	設定無し
	$publicStartDate = "";

	//	最大購入設定区分	1:指定なし	2:指定あり
	$att_check = 0;
	if (preg_match("/<!--[\s]*Attention[\s]*-->/", $caption)) {
		$att_check = 1;
	}
	$limitedOrderSegment = "1";
	if ($att_check == 1) {
		$limitedOrderSegment = 2;
	}
	//	最大購入数をすべて制限する為
	$limitedOrderSegment = 2;

	//	最大購入個数
	$limitedOrderCount = "";
	if ($att_check == 1) {
		$limitedOrderCount = 1;
	} else {
		$limitedOrderCount = WOWMA_DEFO_ZAIKO;
	}

	//	商品説明(共通)
	$description = "";
	if ($att_check == 1) {
		$description .= $attention_wowma."<br>";
	}
	$caption = preg_replace("/<!--[\s]*Attention[\s]*-->/", "", $caption);
	$caption = preg_replace("/<br \/>/", "<br>", $caption);
	$description .= "ブランド名：".$L_BRAND[$brand]."<br>";
	$description .= $caption."<br>";
	$description = nl2br($description, false);
	$description = preg_replace('/(?:\n|\r|\r\n)/', '', $description );
	strip_tags($description, '<br><center><font><hr><a><b>');
	$description = trim($description);
	if (strlen($description) > 980) {	//	本当は1024
#		$description = mb_strimwidth($description, 0, 980, "", "eucjp-win");	//	1024だけど縮まらないので980
		$description = mb_strimwidth($description, 0, 980, "", "UTF-8");	//	1024だけど縮まらないので980
	}
	$description = trim($description);

	//	SP用商品説明
	$descriptionForSP = "";	//	設定無し

	//	PC用商品説明
	$descriptionForPC = "";	//	設定無し

	//	商品詳細タイトル
	$detailTitle = "";	//	設定無し

	//	商品詳細説明
	$detailDescription = "";	//	設定無し

	//	スペックタイトル
	$specTitle = "";	//	設定無し

	//	スペック1～5
	$spec1 = "";	//	設定無し
	$spec2 = "";	//	設定無し
	$spec3 = "";	//	設定無し
	$spec4 = "";	//	設定無し
	$spec5 = "";	//	設定無し


	//	検索キーワード１～３
	$searchKeyword1 = "";
	$searchKeyword2 = "";
	$searchKeyword3 = "";
	for ($i=1; $i<=3; $i++) {
		$searchKeyword_name = "searchKeyword".$i;
		$key_names = "generic_keywords".$i;
		$names = $$key_names;
		if (strlen($names) <= 20) {
			$$searchKeyword_name = $names;
		}
	}

	//	商品画像名1～10
	$imageName1 = "";
	$imageName2 = "";
	$imageName3 = "";
	$imageName4 = "";
	$imageName5 = "";
	$imageName6 = "";
	$imageName7 = "";
	$imageName8 = "";
	$imageName9 = "";
	$imageName10 = "";

	//	商品画像URL1～10
	$imageUrl1 = "";
	$imageUrl2 = "";
	$imageUrl3 = "";
	$imageUrl4 = "";
	$imageUrl5 = "";
	$imageUrl6 = "";
	$imageUrl7 = "";
	$imageUrl8 = "";
	$imageUrl9 = "";
	$imageUrl10 = "";
	$ii = 1;
	for ($i=1; $i<=10; $i++) {
		$KILN = "KtaiImageLinkName".$i;
		$KIT  = "KtaiImageType".$i;

		$image_file = "";

		if ($i == 1) {
			$image_file = IMG_F_DIR."/$code" . ".jpg";
		} elseif ($i == 2) {
			$image_file = IMG_B_DIR."/$code" . ".jpg";
		} elseif ($i == 3) {
			$image_file = IMG_03_DIR."/$code" . ".jpg";
		} elseif ($i == 4) {
			$image_file = IMG_04_DIR."/$code" . ".jpg";
		} elseif ($i == 5) {
			$image_file = IMG_05_DIR."/$code" . ".jpg";
		} elseif ($i == 6) {
			$image_file = IMG_06_DIR."/$code" . ".jpg";
		} elseif ($i == 7) {
			$image_file = IMG_07_DIR."/$code" . ".jpg";
		} elseif ($i == 8) {
			$image_file = IMG_08_DIR."/$code" . ".jpg";
		} elseif ($i == 9) {
			$image_file = IMG_09_DIR."/$code" . ".jpg";
		} elseif ($i == 10) {
			//	10枚目は無し
			$image_file = IMG_10_DIR."/$code" . ".jpg";
		}

		$$KILN = "NULL";
		$$KIT  = "N";
		if (file_exists($image_file)) {
			$$KIT  = "L";

			$fd_name = "";
			if ($i == 1) {
				$fd_name = "imagef";
			} elseif ($i == 2) {
				$fd_name = "imageb";
			} else {
				$fd_name = "image".sprintf("%02s", $i);
			}
			$KIU  = "imageUrl".$ii;
			++$ii;

			$$KIU = "https://www.futboljersey.com/".$fd_name."/".$code.".jpg";
		}
	}

	//	カテゴリID
	$categoryId = $wd_id;

	//	検索タグID
	$tagId = "";

	//	JANコード
	//	ISBNコード
	//	両方とも13桁になった為判別できない為書籍関係は出品却下！
	$jan = "";
	$isbn = "";
	foreach ($ALL_SIZE AS $key => $VAL) {
		if ($ALL_SIZE[$key]['jan']) {
			if (strlen($ALL_SIZE[$key]['jan']) == 13) {
				$jan = $ALL_SIZE[$key]['jan'];
				break;
			} elseif (strlen($ALL_SIZE[$key]['jan']) == 10) {
				$isbn = $ALL_SIZE[$key]['jan'];
				break;
			}
		}
	}

	//	型番
	$itemModel = "";	//	設定無し

	//	パスワード制限設定
	$limitedPasswd = "";	//	設定無し

	//	パスワード制限ページタイトル
	$limitedPasswdPageTitle = "";	//	設定無し

	//	パスワード制限ページメッセージ
	$limitedPasswdPageMessage = "";	//	設定無し

	//	販売ステータス	1:販売中	2:販売終了
	$saleStatus = 1;	//	出品中

	//	購入オプション1～20
	$itemOption1 = "";
	$itemOption2 = "";
	$itemOption3 = "";
	$itemOption4 = "";
	$itemOption5 = "";
	$itemOption6 = "";
	$itemOption7 = "";
	$itemOption8 = "";
	$itemOption9 = "";
	$itemOption10 = "";
	$itemOption11 = "";
	$itemOption12 = "";
	$itemOption13 = "";
	$itemOption14 = "";
	$itemOption15 = "";
	$itemOption16 = "";
	$itemOption17 = "";
	$itemOption18 = "";
	$itemOption19 = "";
	$itemOption20 = "";

	//	ポイント倍率
	$pointRate = WOWMA_POINT;

	//	在庫区分	1:通常在庫	2:選択肢別在庫
	$stockSegment = 1;
	if ($size_option_flg == 1) {
		$stockSegment = 2;
	}

	//	選択肢別在庫発送日ID
	$stockShippingDayId = "";				//	設定無し

	//	選択肢別在庫発送日表示文言
	$stockShippingDayDispTxt = "";			//	設定無し

	//	在庫切れメッセージ
	$displayBackorderMessage = "";			//	設定無し

	//	通常在庫数
	$stockCount = "";
	if ($stockSegment == 1) {
		if ($att_check == 1 || $zaiko == 1) {
			$stockCount = 1;
		} else {
			$stockCount = WOWMA_DEFO_ZAIKO;
		}
	}

	//	通常在庫発送日ID
	$stockShippingDayId = "";

	//	残在庫表示区分	1:在庫数を表示しない。	2:在庫数を表示する。	3:指定した在庫数以下の場合表示する。
	$displayStockSegment = "";
	if ($stockSegment != 2) {
		$displayStockSegment = 1;	//	非表示設定
	}

	//	在庫表示閾値
	$displayStockThreshold = "";	//	設定しない

	//	選択肢別在庫横軸項目名
	$choicesStockHorizontalItemName = "";
	if ($stockSegment == 2) {
		$choicesStockHorizontalItemName = "サイズ";
	}

	//	選択肢別在庫縦軸項目名
	$choicesStockVerticalItemName = "";

	//	選択肢別在庫上説明
	$choicesStockUpperDescription = "";

	//	選択肢別在庫下説明
	$choicesStockLowerDescription = "";

	//	残選択肢別在庫表示区分	1:在庫数を表示しない。	2:在庫数を表示する。	3:指定した在庫数以下の場合、「△」を表示。
	$displayChoicesStockSegment = "";
	if ($stockSegment == 2) {
		$displayChoicesStockSegment = 1;	//	在庫数の数を数多く設定しない為無し
	}

	//	選択肢別在庫表示閾値
	$displayChoicesStockThreshold = "";

	//--------------------------------------------//

	//	削除用の商品の場合
	if ($del_flg == 1 && $list) {
		foreach ($list AS $key => $val) {
			$$key = $val;
		}
		$StockNum = "0";
		$StockType = "B";
		$SkuOptionNameCol = "";
		$SkuOptionValueCol = "";
		$SkuOptionTypeCol = "";
		$SkuStockShowType = "";
		$SkuLimitNum = "";
	}

	//	csv設定
	$list_item_csv	.= "\"".$ctrlCol."\",";							//	
	$list_item_csv	.= "\"".$lotNumber."\",";						//	
	$list_item_csv	.= "\"".$itemName."\",";						//	
	$list_item_csv	.= "\"".$itemManagementId."\",";				//	
	$list_item_csv	.= "\"".$itemManagementName."\",";				//	
	$list_item_csv	.= "\"".$itemCode."\",";						//	
	$list_item_csv	.= "\"".$itemPrice."\",";						//	
	$list_item_csv	.= "\"".$sellMethodSegment."\",";				//	
	$list_item_csv	.= "\"".$releaseDate."\",";						//	
	$list_item_csv	.= "\"".$reserveRegstDate."\",";				//	
	$list_item_csv	.= "\"".$taxSegment."\",";						//	
	$list_item_csv	.= "\"".$postageSegment."\",";					//	
	$list_item_csv	.= "\"".$postage."\",";							//	
	$list_item_csv	.= "\"".$deliveryId."\",";						//	
	$list_item_csv	.= "\"".$deliveryMethodId1."\",";				//	
	$list_item_csv	.= "\"".$deliveryMethodId2."\",";				//	
	$list_item_csv	.= "\"".$deliveryMethodId3."\",";				//	
	$list_item_csv	.= "\"".$deliveryMethodId4."\",";				//	
	$list_item_csv	.= "\"".$deliveryMethodId5."\",";				//	
	$list_item_csv	.= "\"".$deliveryMethodName1."\",";				//	
	$list_item_csv	.= "\"".$deliveryMethodName2."\",";				//	
	$list_item_csv	.= "\"".$deliveryMethodName3."\",";				//	
	$list_item_csv	.= "\"".$deliveryMethodName4."\",";				//	
	$list_item_csv	.= "\"".$deliveryMethodName5."\",";				//	
	$list_item_csv	.= "\"".$publicStartDate."\",";					//	
	$list_item_csv	.= "\"".$limitedOrderSegment."\",";				//	
	$list_item_csv	.= "\"".$limitedOrderCount."\",";				//	
	$list_item_csv	.= "\"".$description."\",";						//	
	$list_item_csv	.= "\"".$descriptionForSP."\",";				//	
	$list_item_csv	.= "\"".$descriptionForPC."\",";				//	
	$list_item_csv	.= "\"".$detailTitle."\",";						//	
	$list_item_csv	.= "\"".$detailDescription."\",";				//	
	$list_item_csv	.= "\"".$specTitle."\",";						//	
	$list_item_csv	.= "\"".$spec1."\",";							//	
	$list_item_csv	.= "\"".$spec2."\",";							//	
	$list_item_csv	.= "\"".$spec3."\",";							//	
	$list_item_csv	.= "\"".$spec4."\",";							//	
	$list_item_csv	.= "\"".$spec5."\",";							//	
	$list_item_csv	.= "\"".$searchKeyword1."\",";					//	
	$list_item_csv	.= "\"".$searchKeyword2."\",";					//	
	$list_item_csv	.= "\"".$searchKeyword3."\",";					//	
	$list_item_csv	.= "\"".$imageName1."\",";						//	
	$list_item_csv	.= "\"".$imageName2."\",";						//	
	$list_item_csv	.= "\"".$imageName3."\",";						//	
	$list_item_csv	.= "\"".$imageName4."\",";						//	
	$list_item_csv	.= "\"".$imageName5."\",";						//	
	$list_item_csv	.= "\"".$imageName6."\",";						//	
	$list_item_csv	.= "\"".$imageName7."\",";						//	
	$list_item_csv	.= "\"".$imageName8."\",";						//	
	$list_item_csv	.= "\"".$imageName9."\",";						//	
	$list_item_csv	.= "\"".$imageName10."\",";						//	
	$list_item_csv	.= "\"".$imageUrl1."\",";						//	
	$list_item_csv	.= "\"".$imageUrl2."\",";						//	
	$list_item_csv	.= "\"".$imageUrl3."\",";						//	
	$list_item_csv	.= "\"".$imageUrl4."\",";						//	
	$list_item_csv	.= "\"".$imageUrl5."\",";						//	
	$list_item_csv	.= "\"".$imageUrl6."\",";						//	
	$list_item_csv	.= "\"".$imageUrl7."\",";						//	
	$list_item_csv	.= "\"".$imageUrl8."\",";						//	
	$list_item_csv	.= "\"".$imageUrl9."\",";						//	
	$list_item_csv	.= "\"".$imageUrl10."\",";						//	
	$list_item_csv	.= "\"".$categoryId."\",";						//	
	$list_item_csv	.= "\"".$tagId."\",";							//	
	$list_item_csv	.= "\"".$jan."\",";								//	
	$list_item_csv	.= "\"".$isbn."\",";							//	
	$list_item_csv	.= "\"".$itemModel."\",";						//	
	$list_item_csv	.= "\"".$limitedPasswd."\",";					//	
	$list_item_csv	.= "\"".$limitedPasswdPageTitle."\",";			//	
	$list_item_csv	.= "\"".$limitedPasswdPageMessage."\",";		//	
	$list_item_csv	.= "\"".$saleStatus."\",";						//	
	$list_item_csv	.= "\"".$itemOption1."\",";						//	
	$list_item_csv	.= "\"".$itemOption2."\",";						//	
	$list_item_csv	.= "\"".$itemOption3."\",";						//	
	$list_item_csv	.= "\"".$itemOption4."\",";						//	
	$list_item_csv	.= "\"".$itemOption5."\",";						//	
	$list_item_csv	.= "\"".$itemOption6."\",";						//	
	$list_item_csv	.= "\"".$itemOption7."\",";						//	
	$list_item_csv	.= "\"".$itemOption8."\",";						//	
	$list_item_csv	.= "\"".$itemOption9."\",";						//	
	$list_item_csv	.= "\"".$itemOption10."\",";					//	
	$list_item_csv	.= "\"".$itemOption11."\",";					//	
	$list_item_csv	.= "\"".$itemOption12."\",";					//	
	$list_item_csv	.= "\"".$itemOption13."\",";					//	
	$list_item_csv	.= "\"".$itemOption14."\",";					//	
	$list_item_csv	.= "\"".$itemOption15."\",";					//	
	$list_item_csv	.= "\"".$itemOption16."\",";					//	
	$list_item_csv	.= "\"".$itemOption17."\",";					//	
	$list_item_csv	.= "\"".$itemOption18."\",";					//	
	$list_item_csv	.= "\"".$itemOption19."\",";					//	
	$list_item_csv	.= "\"".$itemOption20."\",";					//	
	$list_item_csv	.= "\"".$pointRate."\",";						//	
	$list_item_csv	.= "\"".$stockSegment."\",";					//	
	$list_item_csv	.= "\"".$stockShippingDayId."\",";				//	
	$list_item_csv	.= "\"".$stockShippingDayDispTxt."\",";			//	
	$list_item_csv	.= "\"".$displayBackorderMessage."\",";			//	
	$list_item_csv	.= "\"".$stockCount."\",";						//	
	$list_item_csv	.= "\"".$displayStockSegment."\",";				//	
	$list_item_csv	.= "\"".$displayStockThreshold."\",";			//	
	$list_item_csv	.= "\"".$choicesStockHorizontalItemName."\",";	//	
	$list_item_csv	.= "\"".$choicesStockVerticalItemName."\",";	//	
	$list_item_csv	.= "\"".$choicesStockUpperDescription."\",";	//	
	$list_item_csv	.= "\"".$choicesStockLowerDescription."\",";	//	
	$list_item_csv	.= "\"".$displayChoicesStockSegment."\",";		//	
	$list_item_csv	.= "\"".$displayChoicesStockThreshold."\"";		//	
	$list_item_csv	.= "\r\n";

	//	add ohkawara 2018/05/23
	//	在庫区分が1:通常在庫の場合、在庫CSVは作らなくてよい？はず。
	if ($stockSegment == 1) {
		unset($WOWMA_UP_LIST[$g_num]);	//	add ohkawara 2018/09/14
		return;
	}

	//	在庫ファイル
	//	$lotNumber	//	設定済み
	//	$itemCode	//	設定済み
	$i = 1;
	foreach ($ALL_SIZE AS $VAL) {

		if ($att_check == 1 || $zaiko == 1) {
			$SkuItemNum = 1;
			$choicesStockCount = 1;
		} else {
			$SkuItemNum = WOWMA_DEFO_ZAIKO;
			$choicesStockCount = WOWMA_DEFO_ZAIKO;
		}

		$choicesStockHorizontalName = "";
		$choicesStockHorizontalCode = "";
		$choicesStockHorizontalSeq = "";

		//	販売終了商品またはサイズが非表示だった場合は在庫0にする
		if ($saleStatus == 2 || $VAL['type'] == 3) {
			$SkuItemNum = 0;
			$choicesStockCount = 0;
		}

		if ($VAL['size'] != "") {
			//	サイズが有る場合
			$stockCount = "";
			$stockSegment = 2;
			$choicesStockHorizontalName = $VAL['size'];
			$choicesStockHorizontalCode = "-".$VAL['size'];
			$choicesStockHorizontalSeq = $i;
			//$i++;	//	del ohkawara 2018/05/21
		} else {
			//	サイズが無い場合
			$choicesStockCount = "";
			$stockSegment = 1;

			//	add ohkawara 2018/05/21
			if ($WOWMA_UP_LIST[$g_num]['stockSegment'] == 1 && $WOWMA_UP_LIST[$g_num]['stockCount'] == 2) {
				continue;
			}
		}

		$choicesStockVerticalName = "";
		$choicesStockVerticalCode = "";
		$choicesStockVerticalSeq = "";

		//	在庫区分が選択があるのに通常の設定になっていた場合データーを作らない
		if ($stockSegment == 1 && $stockCount === "") {
			continue;
		}

		//	すでに在庫設定が0になっている場合はデーターを作らない
		$check_stockSegment = $WOWMA_UP_LIST[$g_num]['stockSegment'];
		$check_stockCount = $WOWMA_UP_LIST[$g_num]['stockCount'];
		$check_choicesStockCount = $WOWMA_UP_LIST[$g_num]['size'][$choicesStockHorizontalName];
		if ($check_stockSegment && $VAL['type'] == 3) {
			if ($stockSegment == 1 && $stockCount === 0 && $check_stockCount < 1) {
				continue;
			} elseif ($stockSegment == 2 && $choicesStockCount === 0 && $check_choicesStockCount < 1) {
				continue;
			}
		}

		//	項目軸を変更する場合はすべてNで登録する必要がある。
		$ctrlCol = "N";
		/*	//	2018/05/18	とりあえずすべてNで登録してみて大丈夫か確認してみる。
		//if (count($WOWMA_UP_LIST[$g_num]['size'][$choicesStockHorizontalName]) > 0) {		//	del ohkawara 2018/05/18	NとUを混合してよい場合
		if (count($WOWMA_UP_LIST[$g_num]['size']) > 0) {									//	add ohkawara 2018/05/18	一つでもUが合ったらすべてUにする。但しまだ不具合有り
		//if ($WOWMA_UP_LIST[$g_num]['lotNumber'] > 0) {
			$ctrlCol = "U";
		}
		*/

		$choicesStockShippingDayId = "";

		$choicesStockImageUrl = "";			//	add ohkawara 2018/12/17
		$choicesStockColorSegment = "";		//	add ohkawara 2018/12/17

		$list_zaiko_csv	.= "\"".$ctrlCol."\",";							//	ctrlCol
		$list_zaiko_csv	.= "\"".$lotNumber."\",";						//	lotNumber
		$list_zaiko_csv	.= "\"".$itemCode."\",";						//	itemCode		Code
		$list_zaiko_csv	.= "\"".$stockSegment."\",";					//	stockSegment	SkuOptionTypeCol
		$list_zaiko_csv	.= "\"".$stockCount."\",";						//	stockCount		SkuItemNum
		$list_zaiko_csv	.= "\"".$choicesStockHorizontalName."\",";		//	choicesStockHorizontalName
		$list_zaiko_csv	.= "\"".$choicesStockHorizontalCode."\",";		//	choicesStockHorizontalCode
		$list_zaiko_csv	.= "\"".$choicesStockHorizontalSeq."\",";		//	choicesStockHorizontalSeq
		$list_zaiko_csv	.= "\"".$choicesStockVerticalName."\",";		//	choicesStockVerticalName	空
		$list_zaiko_csv	.= "\"".$choicesStockVerticalCode."\",";		//	choicesStockVerticalCode	空
		$list_zaiko_csv	.= "\"".$choicesStockVerticalSeq."\",";			//	choicesStockVerticalSeq		空
		$list_zaiko_csv	.= "\"".$choicesStockCount."\",";				//	choicesStockCount
		$list_zaiko_csv	.= "\"".$choicesStockShippingDayId."\",";		//	choicesStockShippingDayId	設定しない
		$list_zaiko_csv	.= "\"".$choicesStockShippingDayDispTxt."\",";	//	
		$list_zaiko_csv	.= "\"".$choicesStockImageUrl."\",";			//	add ohkawara 2018/12/17
		$list_zaiko_csv	.= "\"".$choicesStockColorSegment."\"";			//	add ohkawara 2018/12/17
		$list_zaiko_csv	.= "\r\n";
		$i++;	//	add ohkawara 2018/05/21
	}

//if ($g_num == 1052) {
//exit;
//}
	unset($WOWMA_UP_LIST[$g_num]);

}


//	wowma!アップ済み商品で、在庫0設定商品、オリジナルで販売終了設定商品を削除
function del_wowma_data(&$WOWMA_UP_LIST, &$list_item_csv, &$list_zaiko_csv) {

	//	削除データー作成
	if ($WOWMA_UP_LIST) {
		foreach ($WOWMA_UP_LIST AS $key_num => $LIST) {

			foreach ($LIST AS $key_name => $value) {
				$$key_name = $value;
			}
			$ctrlCol = "U";
			$saleStatus = 2;


			//	スペックタイトル
			$specTitle = "";	//	設定無し
			//	スペック1～5
			$spec1 = "";	//	設定無し
			$spec2 = "";	//	設定無し
			$spec3 = "";	//	設定無し
			$spec4 = "";	//	設定無し
			$spec5 = "";	//	設定無し


			//	csv設定
			$list_item_csv	.= "\"".$ctrlCol."\",";							//	
			$list_item_csv	.= "\"".$lotNumber."\",";						//	
			$list_item_csv	.= "\"".$itemName."\",";						//	
			$list_item_csv	.= "\"".$itemManagementId."\",";				//	
			$list_item_csv	.= "\"".$itemManagementName."\",";				//	
			$list_item_csv	.= "\"".$itemCode."\",";						//	
			$list_item_csv	.= "\"".$itemPrice."\",";						//	
			$list_item_csv	.= "\"".$sellMethodSegment."\",";				//	
			$list_item_csv	.= "\"".$releaseDate."\",";						//	
			$list_item_csv	.= "\"".$reserveRegstDate."\",";				//	
			$list_item_csv	.= "\"".$taxSegment."\",";						//	
			$list_item_csv	.= "\"".$postageSegment."\",";					//	
			$list_item_csv	.= "\"".$postage."\",";							//	
			$list_item_csv	.= "\"".$deliveryId."\",";						//	
			$list_item_csv	.= "\"".$deliveryMethodId1."\",";				//	
			$list_item_csv	.= "\"".$deliveryMethodId2."\",";				//	
			$list_item_csv	.= "\"".$deliveryMethodId3."\",";				//	
			$list_item_csv	.= "\"".$deliveryMethodId4."\",";				//	
			$list_item_csv	.= "\"".$deliveryMethodId5."\",";				//	
			$list_item_csv	.= "\"".$deliveryMethodName1."\",";				//	
			$list_item_csv	.= "\"".$deliveryMethodName2."\",";				//	
			$list_item_csv	.= "\"".$deliveryMethodName3."\",";				//	
			$list_item_csv	.= "\"".$deliveryMethodName4."\",";				//	
			$list_item_csv	.= "\"".$deliveryMethodName5."\",";				//	
			$list_item_csv	.= "\"".$publicStartDate."\",";					//	
			$list_item_csv	.= "\"".$limitedOrderSegment."\",";				//	
			$list_item_csv	.= "\"".$limitedOrderCount."\",";				//	
			$list_item_csv	.= "\"".$description."\",";						//	
			$list_item_csv	.= "\"".$descriptionForSP."\",";				//	
			$list_item_csv	.= "\"".$descriptionForPC."\",";				//	
			$list_item_csv	.= "\"".$detailTitle."\",";						//	
			$list_item_csv	.= "\"".$detailDescription."\",";				//	
			$list_item_csv	.= "\"".$specTitle."\",";						//	
			$list_item_csv	.= "\"".$spec1."\",";							//	
			$list_item_csv	.= "\"".$spec2."\",";							//	
			$list_item_csv	.= "\"".$spec3."\",";							//	
			$list_item_csv	.= "\"".$spec4."\",";							//	
			$list_item_csv	.= "\"".$spec5."\",";							//	
			$list_item_csv	.= "\"".$searchKeyword1."\",";					//	
			$list_item_csv	.= "\"".$searchKeyword2."\",";					//	
			$list_item_csv	.= "\"".$searchKeyword3."\",";					//	
			$list_item_csv	.= "\"".$imageName1."\",";						//	
			$list_item_csv	.= "\"".$imageName2."\",";						//	
			$list_item_csv	.= "\"".$imageName3."\",";						//	
			$list_item_csv	.= "\"".$imageName4."\",";						//	
			$list_item_csv	.= "\"".$imageName5."\",";						//	
			$list_item_csv	.= "\"".$imageName6."\",";						//	
			$list_item_csv	.= "\"".$imageName7."\",";						//	
			$list_item_csv	.= "\"".$imageName8."\",";						//	
			$list_item_csv	.= "\"".$imageName9."\",";						//	
			$list_item_csv	.= "\"".$imageName10."\",";						//	
			$list_item_csv	.= "\"".$imageUrl1."\",";						//	
			$list_item_csv	.= "\"".$imageUrl2."\",";						//	
			$list_item_csv	.= "\"".$imageUrl3."\",";						//	
			$list_item_csv	.= "\"".$imageUrl4."\",";						//	
			$list_item_csv	.= "\"".$imageUrl5."\",";						//	
			$list_item_csv	.= "\"".$imageUrl6."\",";						//	
			$list_item_csv	.= "\"".$imageUrl7."\",";						//	
			$list_item_csv	.= "\"".$imageUrl8."\",";						//	
			$list_item_csv	.= "\"".$imageUrl9."\",";						//	
			$list_item_csv	.= "\"".$imageUrl10."\",";						//	
			$list_item_csv	.= "\"".$categoryId."\",";						//	
			$list_item_csv	.= "\"".$tagId."\",";							//	
			$list_item_csv	.= "\"".$jan."\",";								//	
			$list_item_csv	.= "\"".$isbn."\",";							//	
			$list_item_csv	.= "\"".$itemModel."\",";						//	
			$list_item_csv	.= "\"".$limitedPasswd."\",";					//	
			$list_item_csv	.= "\"".$limitedPasswdPageTitle."\",";			//	
			$list_item_csv	.= "\"".$limitedPasswdPageMessage."\",";		//	
			$list_item_csv	.= "\"".$saleStatus."\",";						//	
			$list_item_csv	.= "\"".$itemOption1."\",";						//	
			$list_item_csv	.= "\"".$itemOption2."\",";						//	
			$list_item_csv	.= "\"".$itemOption3."\",";						//	
			$list_item_csv	.= "\"".$itemOption4."\",";						//	
			$list_item_csv	.= "\"".$itemOption5."\",";						//	
			$list_item_csv	.= "\"".$itemOption6."\",";						//	
			$list_item_csv	.= "\"".$itemOption7."\",";						//	
			$list_item_csv	.= "\"".$itemOption8."\",";						//	
			$list_item_csv	.= "\"".$itemOption9."\",";						//	
			$list_item_csv	.= "\"".$itemOption10."\",";					//	
			$list_item_csv	.= "\"".$itemOption11."\",";					//	
			$list_item_csv	.= "\"".$itemOption12."\",";					//	
			$list_item_csv	.= "\"".$itemOption13."\",";					//	
			$list_item_csv	.= "\"".$itemOption14."\",";					//	
			$list_item_csv	.= "\"".$itemOption15."\",";					//	
			$list_item_csv	.= "\"".$itemOption16."\",";					//	
			$list_item_csv	.= "\"".$itemOption17."\",";					//	
			$list_item_csv	.= "\"".$itemOption18."\",";					//	
			$list_item_csv	.= "\"".$itemOption19."\",";					//	
			$list_item_csv	.= "\"".$itemOption20."\",";					//	
			$list_item_csv	.= "\"".$pointRate."\",";						//	
			$list_item_csv	.= "\"".$stockSegment."\",";					//	
			$list_item_csv	.= "\"".$stockShippingDayId."\",";				//	
			$list_item_csv	.= "\"".$stockShippingDayDispTxt."\",";			//	
			$list_item_csv	.= "\"".$displayBackorderMessage."\",";			//	
			$list_item_csv	.= "\"".$stockCount."\",";						//	
			$list_item_csv	.= "\"".$displayStockSegment."\",";				//	
			$list_item_csv	.= "\"".$displayStockThreshold."\",";			//	
			$list_item_csv	.= "\"".$choicesStockHorizontalItemName."\",";	//	
			$list_item_csv	.= "\"".$choicesStockVerticalItemName."\",";	//	
			$list_item_csv	.= "\"".$choicesStockUpperDescription."\",";	//	
			$list_item_csv	.= "\"".$choicesStockLowerDescription."\",";	//	
			$list_item_csv	.= "\"".$displayChoicesStockSegment."\",";		//	
			$list_item_csv	.= "\"".$displayChoicesStockThreshold."\"";		//	
			$list_item_csv	.= "\r\n";

			foreach ($size AS $size_name => $size_cnt) {
				if ($size_cnt < 1 || ($stockCount < 1 && $choicesStockCount < 1)) {
					continue;
				}

				if ($stockSegment == 1) {
					$stockCount = 0;
				} else {
					$choicesStockCount = 0;
				}

				$choicesStockShippingDayId = "";

				$list_zaiko_csv	.= "\"U\",";									//	ctrlCol
				$list_zaiko_csv	.= "\"".$lotNumber."\",";						//	lotNumber
				$list_zaiko_csv	.= "\"".$itemCode."\",";						//	itemCode		Code
				$list_zaiko_csv	.= "\"".$stockSegment."\",";					//	stockSegment	SkuOptionTypeCol
				$list_zaiko_csv	.= "\"".$stockCount."\",";						//	stockCount		SkuItemNum
				$list_zaiko_csv	.= "\"".$choicesStockHorizontalName."\",";		//	choicesStockHorizontalName
				$list_zaiko_csv	.= "\"".$choicesStockHorizontalCode."\",";		//	choicesStockHorizontalCode
				$list_zaiko_csv	.= "\"".$choicesStockHorizontalSeq."\",";		//	choicesStockHorizontalSeq
				$list_zaiko_csv	.= "\"".$choicesStockVerticalName."\",";		//	choicesStockVerticalName	空
				$list_zaiko_csv	.= "\"".$choicesStockVerticalCode."\",";		//	choicesStockVerticalCode	空
				$list_zaiko_csv	.= "\"".$choicesStockVerticalSeq."\",";			//	choicesStockVerticalSeq		空
				$list_zaiko_csv	.= "\"".$choicesStockCount."\",";				//	choicesStockCount
				$list_zaiko_csv	.= "\"".$choicesStockShippingDayId."\",";		//	choicesStockShippingDayId	設定しない
				$list_zaiko_csv	.= "\"".$choicesStockShippingDayDispTxt."\",";	//	
				$list_zaiko_csv	.= "\"".$choicesStockImageUrl."\",";			//	add ohkawara 2018/12/17
				$list_zaiko_csv	.= "\"".$choicesStockColorSegment."\"";			//	add ohkawara 2018/12/17
				$list_zaiko_csv	.= "\r\n";
			}
		}
	}
}

//	Wowma
//	del ohkawara 2018/04/02
/*
function make_wowma_csv_bak($list, &$WOWMA_UP_LIST, &$list_item_csv, &$list_zaiko_csv, $del_flg) {
	global $WOWMA_GOODS_DISCOUNT_CATE;
	global $attention_wowma;
	global $L_BRAND;

	$size_list = $list['size_list'];

	$g_num = $list['g_num'];
	unset($WOWMA_UP_LIST[$g_num]);
	$wd_id = $list['wd_id'];
	$g_name = $list['g_name'];
	$base_price = $list['price'];
	$base_sale_price = $list['sale_price'];
	$caption = $list['caption'];
	$brand = $list['brand'];
	$code = $list['code'];
	$generic_keywords1 = $list['generic_keywords1'];
	$generic_keywords2 = $list['generic_keywords2'];
	$generic_keywords3 = $list['generic_keywords3'];
	$generic_keywords4 = $list['generic_keywords4'];
	$generic_keywords5 = $list['generic_keywords5'];

	$zaiko = $list['zaiko'];		//	add ohkawara 2017/06/15

	//--------------------------------------------//

	//	アップ済み商品のチェックとフラグを追加
	//	サイズが有る無しによって設定が有るので、サイズ確認を最初に行う
	$op_size = unserialize($size_list);
	$ALL_SIZE = array();
	if ($op_size) {
		$all_cnt = count($op_size);
		if ($all_cnt > 1) {
			$ALL_SIZE[0]['size'] = "";
			$ALL_SIZE[0]['jan'] = "";
			$ALL_SIZE[0]['asin'] = "";
		}
		$i = 1;
		$asin_flg = 0;
		foreach ($op_size AS $key => $VAL) {
			if ($VAL['type'] == 3) {
				//	サイズが非表示だったら飛ばす
				continue;
			}
			if ($VAL['jan'] == "none") {
				$VAL['jan'] = "";
			}

			if ($VAL['asin'] != "") {
				//	asinが1サイズでも登録されていた場合は、出品ファイルに保存する。
				//	その時にasinが入っていたサイズは販売しない。
				$asin_flg = 1;
			}

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
				}
				$ALL_SIZE[$i]['size'] = $check_size;
				$ALL_SIZE[$i]['jan'] = $VAL['jan'];
				$ALL_SIZE[$i]['asin'] = $VAL['asin'];
				$i++;
			} else {
				$ALL_SIZE[0]['size'] = "";
				$ALL_SIZE[0]['jan'] = $VAL['jan'];
				$ALL_SIZE[0]['asin'] = $VAL['asin'];
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
	if (count($ALL_SIZE) < 1 && $del_flg != 1) {
		return;
	}

	$size_option_flg = 0;
	if ($ALL_SIZE[1]['size'] != "") {
		$size_option_flg = 1;
	}

	//--------------------------------------------//

	//	商品コード
	$Code = "g".$g_num;

	//	商品属性
	$BCategID = $wd_id;

	//	商品名
	$KtaiTitle = $g_name;

	//	販売価格
	//	割引設定項目
	$GOODS_DISCOUNT_C = WOWMA_GOODS_DISCOUNT_C;
	$GOODS_DISCOUNT_CATE = $WOWMA_GOODS_DISCOUNT_CATE;
	$DISCOUNT_PAR = WOWMA_DISCOUNT_PAR;
	$DISCOUNT_PAR2 = WOWMA_DISCOUNT_PAR2;

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

	//if (TAXABLE == 1) {												//	del ookawara 2017/04/21
	$price = floor($price * ((TAX / 100) + 1) + 0.5);
	$sale_price = floor($sale_price * ((TAX / 100) + 1) + 0.5);
	//}																	//	del ookawara 2017/04/21
	if ($sale_price < 1) { unset($sale_price); }
	$KtaiPrice = $price;
	if ($sale_price > 0) {
		$KtaiPrice = $sale_price;
	}

	//	商品説明
	$KtaiCaption = "";
	$att_check = 0;
	if (preg_match("/<!--[\s]*Attention[\s]*-->/", $caption)) {
		$KtaiCaption .= $attention_wowma."<br>";
		$att_check = 1;
	}
	$caption = preg_replace("/<!--[\s]*Attention[\s]*-->/", "", $caption);
	$caption = preg_replace("/<br \/>/", "<br>", $caption);
	$KtaiCaption .= "ブランド名：".$L_BRAND[$brand]."<br>";
	$KtaiCaption .= $caption."<br>";
	$KtaiCaption = nl2br($KtaiCaption, false);
	$KtaiCaption = preg_replace('/(?:\n|\r|\r\n)/', '', $KtaiCaption );
	strip_tags($KtaiCaption, '<br><center><font><hr><a><b>');
	$KtaiCaption = trim($KtaiCaption);
	if (strlen($KtaiCaption) > 1024) {
#		$KtaiCaption = mb_strimwidth($KtaiCaption, 0, 1000, "", "eucjp-win");	//	1024だけど縮まらないので1000
		$KtaiCaption = mb_strimwidth($KtaiCaption, 0, 1000, "", "UTF-8");	//	1024だけど縮まらないので1000
	}
	$KtaiCaption = trim($KtaiCaption);

	//	決済方法
	$KtaiDealType = "CBMrtsPhic";	//	DeNAショッピング決済サービス 全サービス利用

	//	決算手数料
	$KtaiChargeType = "CBPhi";	//	DeNAショッピング決済サービス利用後

	//	管理ID
	$AdmID = $code;	//	品番

	//	基本在庫数
	$StockNum = "";
	if ($size_option_flg != 1) {
		//if ($att_check == 1) {					//	del ohkawara 2017/06/16
		if ($att_check == 1 || $zaiko == 1) {		//	add ohkawara 2017/06/16
			$StockNum = 1;
		} else {
			$StockNum = WOWMA_DEFO_ZAIKO;
		}
	}

	//	検索キーワード(3つまで)
	$Keyword = "";
	for ($i=1; $i<=3; $i++) {
		$key_names = "generic_keywords".$i;
		if ($Keyword != "") {
			$Keyword .= ":";
		}
		$names = $$key_names;
		$size_flg = 0;
		if (strlen($names) > 20) {
			$size_flg = 1;
		}
		if ($names && $size_flg != 1) {
			$Keyword .= $names;
		} else {
			$Keyword .= "NULL";
		}
	}

	//	ポイント付与率
	$KtaiUserRate = WOWMA_POINT;

	//	画像リンク名1-10
	//	画像タイプ1-10
	//	画像URL1-10
	for ($i=1; $i<=10; $i++) {
		$KILN = "KtaiImageLinkName".$i;
		$KIT  = "KtaiImageType".$i;
		$KIU  = "KtaiImageURL".$i;

		$image_file = "";

		if ($i == 1) {
			$image_file = IMG_F_DIR."/$code" . ".jpg";
		} elseif ($i == 2) {
			$image_file = IMG_B_DIR."/$code" . ".jpg";
		} elseif ($i == 3) {
			$image_file = IMG_03_DIR."/$code" . ".jpg";
		} elseif ($i == 4) {
			$image_file = IMG_04_DIR."/$code" . ".jpg";
		} elseif ($i == 5) {
			$image_file = IMG_05_DIR."/$code" . ".jpg";
		} elseif ($i == 6) {
			$image_file = IMG_06_DIR."/$code" . ".jpg";
		} elseif ($i == 7) {
			$image_file = IMG_07_DIR."/$code" . ".jpg";
		} elseif ($i == 8) {
			$image_file = IMG_08_DIR."/$code" . ".jpg";
		} elseif ($i == 9) {
			$image_file = IMG_09_DIR."/$code" . ".jpg";
		} elseif ($i == 10) {
			//	10枚目は無し
			$image_file = IMG_10_DIR."/$code" . ".jpg";
		}

		$$KILN = "NULL";
		$$KIT  = "N";
		$$KIU  = "";
		if (file_exists($image_file)) {
			$$KIT  = "L";

			$fd_name = "";
			if ($i == 1) {
				$fd_name = "imagef";
			} elseif ($i == 2) {
				$fd_name = "imageb";
			} else {
				$fd_name = "image".sprintf("%02s", $i);
			}
			$$KIU = "http://www.futboljersey.com/".$fd_name."/".$code.".jpg";
		}
	}

	//	送料設定
	$ShipFeeType = "B";	//	送料別

	//	個別送料
	$ShipFee = "";	//	設定無し

	//	メール便設定
	$DeliveryType = "";	//	設定無し

	//	送料コメント
	$KtaiShipComment = "";	//	設定無し

	//	決算方法コメント
	$KtaiDealComment = "";	//	設定無し

	//	販売価格コメント1
	$KtaiPriceLabel1 = "";	//	設定無し

	//	販売価格コメント2
	$KtaiPriceLabel2 = "";	//	設定無し

	//	PC用商品説明文
	$KtaiCaptionPC = "";	//	設定無し

	//	スマートフォン用商品説明文
	$KtaiCaptionSP = "";	//	設定無し

	//	商品詳細説明
	$KtaiDetailLinkName = "";	//	設定無し

	//	商品詳細説明
	$KtaiDetailComment = "";	//	設定無し

	//	購入オプション1-5	//	サイズ選択で必要時は利用する
	$Option1 = "";
	$Option2 = "";
	$Option3 = "";
	$Option4 = "";
	$Option5 = "";

	//	発売入荷予定日
	$SalesStartDate = "";	//	設定無し

	//	掲載開始日時
	$StartDate = "";

	//	販売状態
	$ExhibitStatus = "O";	//	出品中

	//	管理用商品名
	$AdmTitle = "";	//	設定無し

	//	JANコード
	//	ISBNコード
	//	両方とも13桁になった為判別できない為書籍関係は出品却下！
	$JanCode = "";
	$IsbnCode = "";
	foreach ($ALL_SIZE AS $key => $VAL) {
		if ($ALL_SIZE[$key]['jan']) {
			if (strlen($ALL_SIZE[$key]['jan']) == 13) {
				$JanCode = $ALL_SIZE[$key]['jan'];
				break;
			} elseif (strlen($ALL_SIZE[$key]['jan']) == 10) {
				$IsbnCode = $ALL_SIZE[$key]['jan'];
				break;
			}
		}
	}

	//	型番
	$NameModel = "";	//	設定無し

	//	秘密ページパスワード
	$SecretPagePassword = "";	//	設定無し

	//	秘密ページタイトル
	$SecretPageTitle = "";	//	設定無し

	//	秘密ページメッセージ
	$SecretPageMessage = "";	//	設定無し

	//	在庫表示タイプ
	$StockShow = "H";	//	非表示設定

	//	在庫切れメッセージ
	$KtaiNoStockMessage = "";	//	設定無し

	//	スペックリンク名
	$KtaiSpecLinkName = "";	//	設定無し

	//	スペック1-5
	$KtaiSpec1 = "";	//	設定無し
	$KtaiSpec2 = "";	//	設定無し
	$KtaiSpec3 = "";	//	設定無し
	$KtaiSpec4 = "";	//	設定無し
	$KtaiSpec5 = "";	//	設定無し

	//	最大購入個数
	$MaxBuyNum = "";
	if ($att_check == 1) {
		$MaxBuyNum = 1;
	} else {
		$MaxBuyNum = WOWMA_DEFO_ZAIKO;
	}

	//	在庫数表示許容数
	$KtaiStockShowStartNum = "";	//	設定しない

	//	在庫表示メッセージ1－2
	$KtaiStockMessage1 = "";	//	設定無し
	$KtaiStockMessage2 = "";	//	設定無し

	//	消費税
	$Tax = "I";	//	内税

	//	在庫管理タイプ
	$StockType = "B";
	if ($size_option_flg == 1) {
		$StockType = "S";
	}

	//	SKU横軸項目名
	$SkuOptionNameCol = "";
	if ($size_option_flg == 1) {
		$SkuOptionNameCol = "サイズ";
	}

	//	SKU横軸選択枠
	$SkuOptionValueCol = "";
	//	SKU横軸管理ID
	$SkuOptionTypeCol = "";

	$SET_LIST_V = array();
	$SET_LIST_T = array();
	if ($size_option_flg == 1) {
		foreach ($ALL_SIZE AS $key => $VAL) {
			if ($VAL['size']) {
				$SET_LIST_V[] = $VAL['size'];
				$SET_LIST_T[] = "-".$VAL['size'];
			}
		}
		$SkuOptionValueCol = implode(":", $SET_LIST_V);
		$SkuOptionTypeCol = implode(":", $SET_LIST_T);
	}

	//	SKU縦軸項目名
	$SkuOptionNameRow = "";	//	特になし

	//	SKU縦軸選択枠
	$SkuOptionValueRow = "";	//	特になし

	//	SKU縦軸管理ID
	$SkuOptionTypeRow = "";	//	特になし

	//	SKUヘッダ情報
	$SkuHeader = "";	//	特になし

	//	SKUフッダー情報
	$SkuFooter = "";	//	特になし

	//	SKU在庫表示タイプ
	$SkuStockShowType = "H";	//	在庫数の数を数多く設定しない為無し
	if ($StockType == "B") {
		$SkuStockShowType = "";
	}

	//	SKU在庫表示閾値
	$SkuLimitNum = "";	//	在庫数の数を数多く設定しない為無し

	//	出品先
	$exhibittype = "MX";

	//--------------------------------------------//

	//	削除用の商品の場合
	if ($del_flg == 1 && $list) {
		foreach ($list AS $key => $val) {
			$$key = $val;
		}
		$StockNum = "0";
		$StockType = "B";
		$SkuOptionNameCol = "";
		$SkuOptionValueCol = "";
		$SkuOptionTypeCol = "";
		$SkuStockShowType = "";
		$SkuLimitNum = "";
	}

	//	csv設定
	$list_item_csv	.= "\"".$Code."\",";					//	Code
	$list_item_csv	.= "\"".$BCategID."\",";				//	BCategID
	$list_item_csv	.= "\"".$KtaiTitle."\",";				//	KtaiTitle
	$list_item_csv	.= "\"".$KtaiPrice."\",";				//	KtaiPrice
	$list_item_csv	.= "\"".$KtaiCaption."\",";				//	KtaiCaption
	$list_item_csv	.= "\"".$KtaiDealType."\",";			//	KtaiDealType
	$list_item_csv	.= "\"".$KtaiChargeType."\",";			//	KtaiChargeType
	$list_item_csv	.= "\"".$AdmID."\",";					//	AdmID
	$list_item_csv	.= "\"".$StockNum."\",";				//	StockNum
	$list_item_csv	.= "\"".$Keyword."\",";					//	Keyword
	$list_item_csv	.= "\"".$KtaiUserRate."\",";			//	KtaiUserRate
	$list_item_csv	.= "\"".$KtaiImageLinkName1."\",";		//	KtaiImageLinkName1
	$list_item_csv	.= "\"".$KtaiImageType1."\",";			//	KtaiImageType1
	$list_item_csv	.= "\"".$KtaiImageURL1."\",";			//	KtaiImageURL1
	$list_item_csv	.= "\"".$KtaiImageLinkName2."\",";		//	KtaiImageLinkName2
	$list_item_csv	.= "\"".$KtaiImageType2."\",";			//	KtaiImageType2
	$list_item_csv	.= "\"".$KtaiImageURL2."\",";			//	KtaiImageURL2
	$list_item_csv	.= "\"".$KtaiImageLinkName3."\",";		//	KtaiImageLinkName3
	$list_item_csv	.= "\"".$KtaiImageType3."\",";			//	KtaiImageType3
	$list_item_csv	.= "\"".$KtaiImageURL3."\",";			//	KtaiImageURL3
	$list_item_csv	.= "\"".$KtaiImageLinkName4."\",";		//	KtaiImageLinkName4
	$list_item_csv	.= "\"".$KtaiImageType4."\",";			//	KtaiImageType4
	$list_item_csv	.= "\"".$KtaiImageURL4."\",";			//	KtaiImageURL4
	$list_item_csv	.= "\"".$KtaiImageLinkName5."\",";		//	KtaiImageLinkName5
	$list_item_csv	.= "\"".$KtaiImageType5."\",";			//	KtaiImageType5
	$list_item_csv	.= "\"".$KtaiImageURL5."\",";			//	KtaiImageURL5
	$list_item_csv	.= "\"".$KtaiImageLinkName6."\",";		//	KtaiImageLinkName6
	$list_item_csv	.= "\"".$KtaiImageType6."\",";			//	KtaiImageType6
	$list_item_csv	.= "\"".$KtaiImageURL6."\",";			//	KtaiImageURL6
	$list_item_csv	.= "\"".$KtaiImageLinkName7."\",";		//	KtaiImageLinkName7
	$list_item_csv	.= "\"".$KtaiImageType7."\",";			//	KtaiImageType7
	$list_item_csv	.= "\"".$KtaiImageURL7."\",";			//	KtaiImageURL7
	$list_item_csv	.= "\"".$KtaiImageLinkName8."\",";		//	KtaiImageLinkName8
	$list_item_csv	.= "\"".$KtaiImageType8."\",";			//	KtaiImageType8
	$list_item_csv	.= "\"".$KtaiImageURL8."\",";			//	KtaiImageURL8
	$list_item_csv	.= "\"".$KtaiImageLinkName9."\",";		//	KtaiImageLinkName9
	$list_item_csv	.= "\"".$KtaiImageType9."\",";			//	KtaiImageType9
	$list_item_csv	.= "\"".$KtaiImageURL9."\",";			//	KtaiImageURL9
	$list_item_csv	.= "\"".$KtaiImageLinkName10."\",";		//	KtaiImageLinkName10
	$list_item_csv	.= "\"".$KtaiImageType10."\",";			//	KtaiImageType10
	$list_item_csv	.= "\"".$KtaiImageURL10."\",";			//	KtaiImageURL10
	$list_item_csv	.= "\"".$ShipFeeType."\",";				//	ShipFeeType
	$list_item_csv	.= "\"".$ShipFee."\",";					//	ShipFee
	$list_item_csv	.= "\"".$DeliveryType."\",";			//	DeliveryType
	$list_item_csv	.= "\"".$KtaiShipComment."\",";			//	KtaiShipComment
	$list_item_csv	.= "\"".$KtaiDealComment."\",";			//	KtaiDealComment
	$list_item_csv	.= "\"".$KtaiPriceLabel1."\",";			//	KtaiPriceLabel1
	$list_item_csv	.= "\"".$KtaiPriceLabel2."\",";			//	KtaiPriceLabel2
	$list_item_csv	.= "\"".$KtaiCaptionPC."\",";			//	KtaiCaptionPC
	$list_item_csv	.= "\"".$KtaiCaptionSP."\",";			//	KtaiCaptionSP
	$list_item_csv	.= "\"".$KtaiDetailLinkName."\",";		//	KtaiDetailLinkName
	$list_item_csv	.= "\"".$KtaiDetailComment."\",";		//	KtaiDetailComment
	$list_item_csv	.= "\"".$Option1."\",";					//	Option1
	$list_item_csv	.= "\"".$Option2."\",";					//	Option2
	$list_item_csv	.= "\"".$Option3."\",";					//	Option3
	$list_item_csv	.= "\"".$Option4."\",";					//	Option4
	$list_item_csv	.= "\"".$Option5."\",";					//	Option5
	$list_item_csv	.= "\"".$SalesStartDate."\",";			//	SalesStartDate
	$list_item_csv	.= "\"".$StartDate."\",";				//	StartDate
	$list_item_csv	.= "\"".$ExhibitStatus."\",";			//	ExhibitStatus
	$list_item_csv	.= "\"".$AdmTitle."\",";				//	AdmTitle
	$list_item_csv	.= "\"".$JanCode."\",";					//	JanCode
	$list_item_csv	.= "\"".$IsbnCode."\",";				//	IsbnCode
	$list_item_csv	.= "\"".$NameModel."\",";				//	NameModel
	$list_item_csv	.= "\"".$SecretPagePassword."\",";		//	SecretPagePassword
	$list_item_csv	.= "\"".$SecretPageTitle."\",";			//	SecretPageTitle
	$list_item_csv	.= "\"".$SecretPageMessage."\",";		//	SecretPageMessage
	$list_item_csv	.= "\"".$StockShow."\",";				//	StockShow
	$list_item_csv	.= "\"".$KtaiNoStockMessage."\",";		//	KtaiNoStockMessage
	$list_item_csv	.= "\"".$KtaiSpecLinkName."\",";		//	KtaiSpecLinkName
	$list_item_csv	.= "\"".$KtaiSpec1."\",";				//	KtaiSpec1
	$list_item_csv	.= "\"".$KtaiSpec2."\",";				//	KtaiSpec2
	$list_item_csv	.= "\"".$KtaiSpec3."\",";				//	KtaiSpec3
	$list_item_csv	.= "\"".$KtaiSpec4."\",";				//	KtaiSpec4
	$list_item_csv	.= "\"".$KtaiSpec5."\",";				//	KtaiSpec5
	$list_item_csv	.= "\"".$MaxBuyNum."\",";				//	KtaMaxBuyNumiSpec5
	$list_item_csv	.= "\"".$KtaiStockShowStartNum."\",";	//	KtaiStockShowStartNum
	$list_item_csv	.= "\"".$KtaiStockMessage1."\",";		//	KtaiStockMessage1
	$list_item_csv	.= "\"".$KtaiStockMessage2."\",";		//	KtaiStockMessage2
	$list_item_csv	.= "\"".$Tax."\",";						//	Tax
	$list_item_csv	.= "\"".$StockType."\",";				//	StockType
	$list_item_csv	.= "\"".$SkuOptionNameCol."\",";		//	SkuOptionNameCol
	$list_item_csv	.= "\"".$SkuOptionValueCol."\",";		//	SkuOptionValueCol
	$list_item_csv	.= "\"".$SkuOptionTypeCol."\",";		//	SkuOptionTypeCol
	$list_item_csv	.= "\"".$SkuOptionNameRow."\",";		//	SkuOptionNameRow
	$list_item_csv	.= "\"".$SkuOptionValueRow."\",";		//	SkuOptionValueRow
	$list_item_csv	.= "\"".$SkuOptionTypeRow."\",";		//	SkuOptionTypeRow
	$list_item_csv	.= "\"".$SkuHeader."\",";				//	SkuHeader
	$list_item_csv	.= "\"".$SkuFooter."\",";				//	SkuFooter
	$list_item_csv	.= "\"".$SkuStockShowType."\",";		//	SkuStockShowType
	$list_item_csv	.= "\"".$SkuLimitNum."\",";				//	SkuLimitNum
	$list_item_csv	.= "\"".$exhibittype."\"";				//	exhibittype
	$list_item_csv	.= "\r\n";


	if (count($SET_LIST_T) < 1 || $del_flg == 1) {	//	選択肢が無い場合、または削除用の場合
		return;
	}

	//	在庫ファイル
	//	$Code	//	設定済み
	$SkuItemNum = 0;
	//if ($att_check == 1) {					//	del ohkawara 2017/06/16
	if ($att_check == 1 || $zaiko == 1) {		//	add ohkawara 2017/06/16
		$SkuItemNum = 1;
	} else {
		$SkuItemNum = WOWMA_DEFO_ZAIKO;
	}
	$SkuOptionTypeRow = "";	//	設定無し

	foreach ($SET_LIST_T AS $val) {
		$SkuOptionTypeCol = $val;

		$list_zaiko_csv	.= "\"".$Code."\",";					//	Code
		$list_zaiko_csv	.= "\"".$SkuOptionTypeCol."\",";		//	SkuOptionTypeCol
		$list_zaiko_csv	.= "\"".$SkuOptionTypeRow."\",";		//	SkuOptionTypeRow
		$list_zaiko_csv	.= "\"".$SkuItemNum."\"";				//	SkuItemNum
		$list_zaiko_csv	.= "\r\n";
	}

}
*/

