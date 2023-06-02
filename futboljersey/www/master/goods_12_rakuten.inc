<?php
error_reporting(E_ERROR);
//	楽天用CSV作成ファイル

function make_csv_rakuten(&$msg = null,&$ERROR = null) {
global $conn_id;

	//	楽天商品データー作成
	//	楽天既存商品データー読み込み
	
	unset($RAKUTEN_NORMAL_ITEM_FILE);
	// unset($RAKU_SP_LIST);  old
	if (file_exists(RAKUTEN_B_NORMAL_ITEM_FILE)) {
		$LIST = file(RAKUTEN_B_NORMAL_ITEM_FILE);
		mb_convert_variables("UTF-8", "sjis-win", $LIST);
		unset($LIST[0]);
		// die;
		if ($LIST) {
			foreach ($LIST AS $VAL) {
				$VAL = trim($VAL);
				list($g_num,$a,$rd_id,$a,$a,$a,$a,$a,$r_sale_price) = explode(",",$VAL);	//	add $a, ookawara 2014/03/17	タグIDが追加されずれた為追加
				$g_num = preg_replace("/[^0-9]/","",$g_num);
				$r_sale_price = preg_replace("/[^0-9]/","",$r_sale_price);
				if ($g_num) {
					$RAKU_LIST[$g_num] = $VAL;
					if ($r_sale_price) { $RAKU_SP_LIST[$g_num] = $r_sale_price; }

					//	楽天デェレクトリー変更
					//$rd_id = preg_replace("/\"/", "" , $rd_id);
					//if ($rd_id) {
					//	$sql  = "UPDATE ".T_GOODS." SET rd_id='".$rd_id."'".
					//			" WHERE g_num='".$g_num."';";
					//	if (!pg_exec($conn_id,$sql)) { $ERROR[] = "商品オプションデーターを更新できませんでした。$g_num"; }
					//}

				}
			}
		}
	}

// 	//	楽天サイズデーター読み込み
// 	unset($RAKU_S_LIST);
// 	if (file_exists(RAKUTEN_B_SELECT_FILE)) {
// 		$LIST = file(RAKUTEN_B_SELECT_FILE);
// #		mb_convert_variables("eucjp-win", "sjis-win", $LIST);
// 		mb_convert_variables("UTF-8", "sjis-win", $LIST);
// 		unset($LIST[0]);
// 		if ($LIST) {
// 			foreach ($LIST AS $VAL) {
// 				$VAL = trim($VAL);
// 				list($a,$g_num,$a,$a,$size) = explode(",",$VAL);
// ##				$g_num = ereg_replace("[^0-9]","",$g_num);
// 				$g_num = preg_replace("/[^0-9]/","",$g_num);
// ##				$size = ereg_replace("\"","",$size);
// 				$size = preg_replace("/\"/","",$size);
// 				$size = trim($size);
// 				if ($g_num) {
// 					$RAKU_S_LIST[$g_num][$size] = $VAL;
// 				}
// 			}
// 		}
// 	}

	//	楽天カテゴリーデーター読み込み
	unset($RAKU_C_LIST);
	if (file_exists(RAKUTEN_B_CAT_FILE)) {
		$LIST = file(RAKUTEN_B_CAT_FILE);
#		mb_convert_variables("eucjp-win", "sjis-win", $LIST);
		mb_convert_variables("UTF-8", "sjis-win", $LIST);
		unset($LIST[0]);
		if ($LIST) {
			foreach ($LIST AS $VAL) {
				$VAL = trim($VAL);
				list($a,$g_num,$a,$ctname) = explode(",",$VAL);
##				$g_num = ereg_replace("[^0-9]","",$g_num);
				$g_num = preg_replace("/[^0-9]/","",$g_num);
##				$ctname = ereg_replace("^\"|\"$","",$ctname);
			    $ctname = preg_replace("/^\"|\"$/","",$ctname);
				if ($g_num && $ctname) {
					$RAKU_C_LIST[$g_num][$ctname] = $VAL;
				}
			}
		}
	}

	//	タグID情報取得
	//	add ohkawara 2017/02/21
	$TAG_SIZE_LIST = array();
	$TAG_BRAND_LIST = array();
	$sql = "SELECT * FROM ".T_MOOL_RT.";";
	if ($result = mysqli_query($conn_id,$sql)) {
		while ($list = mysqli_fetch_array($result)) {
			$rd_id = $list['rd_id'];						//	add ohkawara 2017/04/06
			$tag_id = $list['tag_id'];
			$tag = $list['tag'];
			$tag_class = $list['tag_class'];

			if (preg_match("/ブランド/", $tag_class)) {
				//$TAG_BRAND_LIST[$tag] = $tag_id;			//	del ohkawara 2017/04/06
				$TAG_BRAND_LIST[$rd_id][$tag] = $tag_id;	//	add ohkawara 2017/04/06
			} else {
				//$TAG_SIZE_LIST[$tag] = $tag_id;			//	del ohkawara 2017/04/06
				$TAG_SIZE_LIST[$rd_id][$tag] = $tag_id;	//	add ohkawara 2017/04/06
			}
		}
	}

	//	楽天ディレクトリ取得
	//	addd ohkawara 2017/02/22
	$RKT_DIR_LIST = array();
	$sql = "SELECT * FROM ".T_MOOL_RD.";";
	if ($result = mysqli_query($conn_id,$sql)) {
		while ($list = mysqli_fetch_array($result)) {
			$dir_id = $list['dir_id'];
			$flg = $list['flg'];
			$RKT_DIR_LIST[$dir_id] = $flg;
		}
	}

	//	アイテム登録データー読み込み
	unset($CHECK_ITEM);
	$sql = "SELECT i.num, i.cate1, i.cate2, i.cate3, i.g_num, i.ypath, i.display, i.copy_num, i.state," .
			//" j.g_name, j.code, j.price, j.sale_price, j.options, j.caption, j.brand," . 
			" coalesce(j.name_head, '') || coalesce(j.g_name, '') ||  coalesce(j.name_foot, '') AS g_name,".	//	add ookawara 2015/09/24
			" j.code, j.price, j.sale_price, j.options, j.caption, j.brand," . 	//	add ookawara 2015/09/24
			" j.rd_id,".
			//" j.zaiko, j.size_list, j.name_head".			//	add ookawara 2015/09/17	//	del ookawara 2015/09/24
			" j.zaiko, j.size_list".						//	add ookawara 2015/09/24
			" FROM ".T_CATEGORY." i, ".T_GOODS." j" .
			//" WHERE i.g_num=j.g_num AND j.yahooshop='1' AND j.soldout!='1' AND i.display='1'" .		//	del ohkawara 2020/12/07	yahooと楽天の出品の変数が共通だった為
			" WHERE i.g_num=j.g_num AND j.wowmashop='1' AND j.soldout!='1' AND i.display='1'" .			//	add ohkawara 2020/12/07	yahooでナイキが出品が出来なくなったのでwowmaと共通にした。
			" AND i.cate1>='20' AND i.state!='3'".
			" AND i.cate1 NOT IN ('98','99')".	//	add ookawara 2013/08/23
			" ORDER BY i.cate1, i.cate2, i.cate3 DESC;";

	if ($result = mysqli_query($conn_id,$sql)) {
		while ($list = mysqli_fetch_array($result)) {
			unset($CHECK_SELECT);
			unset($check_cate_name);
			$g_num = $list['g_num'];
			$att_check = 0;	//	add ookawara 2015/01/15

			if (!$CHECK_ITEM[$g_num]) {
				//	アイテム
				$set_csv = "";
				//$set_csv = rakuten_item_csv($list,$L_CATE,$L_BRAND,$L_RAKU,$RAKU_LIST,$RAKU_SP_LIST, $UP_PIC_LIST);	//	add $UP_PIC_LIST ookawara 2014/02/06	//	del ookawara 2015/01/15
				//$set_csv = rakuten_item_csv($list,$L_CATE,$L_BRAND,$L_RAKU,$RAKU_LIST,$RAKU_SP_LIST, $UP_PIC_LIST, $att_check);	//	add ookawara 2015/01/15		//	del ohkawara 2017/02/21
				$set_csv = rakuten_item_csv($list,$L_CATE,$L_BRAND,$L_RAKU,$RAKU_LIST,$RAKU_SP_LIST, $UP_PIC_LIST, $att_check, $TAG_BRAND_LIST, $TAG_SIZE_LIST, $RKT_DIR_LIST);	//	add ohkawara 2017/02/21-22

				if ($set_csv == "") { continue; }
				$rakuten_item_csv .= $set_csv;

				//	セレクト
				//list($select_csv,$CHECK_SELECT) = rakuten_select_csv($list,$RAKU_S_LIST);				//	del ookawara 2015/01/15
				list($select_csv,$CHECK_SELECT) = rakuten_select_csv($list,$RAKU_S_LIST, $att_check);	//	add ookawara 2015/01/15
				if ($select_csv) { $rakuten_select_csv .= $select_csv; }
			}

			//	カテゴリー
			list($cat_csv,$check_cate_name) = rakuten_cat_csv($list,$L_CATE,$RAKU_C_LIST);
			if ($cat_csv) { $rakuten_cat_csv .= $cat_csv; }


			unset($RAKU_LIST[$g_num]);
			if ($CHECK_SELECT) {
				foreach ($CHECK_SELECT AS $val) {
					$val = trim($val);
					if (!$val) { continue; }
					unset($RAKU_S_LIST[$g_num][$val]);
				}
			}
			if ($check_cate_name) { unset($RAKU_C_LIST[$g_num][$check_cate_name]); }

			$CHECK_ITEM[$g_num] = $g_num;
		}
	}

	//	楽天商品データー作成
	//	楽天既存商品データー読み込み
	unset($RAKU_NORMAL_ITEM_LIST);
	unset($RAKU_SP_LIST);
	if (file_exists(RAKUTEN_B_NORMAL_ITEM_FILE)) {
		$LIST = file(RAKUTEN_B_NORMAL_ITEM_FILE);
#		mb_convert_variables("eucjp-win", "sjis-win", $LIST);
		mb_convert_variables("UTF-8", "sjis-win", $LIST);
		unset($LIST[0]);
		if ($LIST) {
			foreach ($LIST AS $VAL) {
				$VAL = trim($VAL);
				list($a,$g_num,$a,$rd_id,$a,$a,$a,$a,$a,$r_sale_price) = explode(",",$VAL);	//	add $a, ookawara 2014/03/17	タグIDが追加されずれた為追加
##				$g_num = ereg_replace("[^0-9]","",$g_num);
				$g_num = preg_replace("/[^0-9]/","",$g_num);
##				$r_sale_price = ereg_replace("[^0-9]","",$r_sale_price);
				$r_sale_price = preg_replace("/[^0-9]/","",$r_sale_price);
				if ($g_num) {
					$RAKU_LIST[$g_num][] = $VAL;
					if ($r_sale_price) { $RAKU_SP_LIST[$g_num][] = $r_sale_price; }

					//	楽天デェレクトリー変更
					//$rd_id = preg_replace("/\"/", "" , $rd_id);
					//if ($rd_id) {
					//	$sql  = "UPDATE ".T_GOODS." SET rd_id='".$rd_id."'".
					//			" WHERE g_num='".$g_num."';";
					//	if (!pg_exec($conn_id,$sql)) { $ERROR[] = "商品オプションデーターを更新できませんでした。$g_num"; }
					//}

				}
			}
		}
	}

    //	親番号を取得し子供の商品情報データー読み込み
	$RAKU_CHILD_LIST=[];  // add thao 2023/05/22
	$sql = "SELECT j.pg_num,i.num, i.cate1, i.cate2, i.cate3, i.g_num, i.ypath, i.display, i.copy_num, i.state," .
			" coalesce(j.name_head, '') || coalesce(j.g_name, '') ||  coalesce(j.name_foot, '') AS g_name,".	//	add ookawara 2015/09/24
			" j.code, j.price, j.sale_price, j.options, j.caption, j.brand," . 	
			" j.rd_id,".
			" j.zaiko, j.size_list"."j.gp_option_name".						
			" FROM ".T_CATEGORY." i, ".T_GOODS." j" .
			" WHERE i.g_num=j.g_num AND j.wowmashop='1' AND j.soldout!='1' AND i.display='1'" .		
			" AND i.cate1>='20' AND i.state!='3'".
			" AND i.cate1 NOT IN ('98','99')".
			" ORDER BY i.cate1, i.cate2, i.cate3 DESC;";
	if ($result = mysqli_query($conn_id,$sql)) {
		while ($list = mysqli_fetch_array($result)) {
			$g_num = $list['g_num'];
			$pg_num = $list['pg_num']; // add thao 2023/05/22
	
			unset($list['pg_num']);
			unset($list['g_num']);
			$RAKU_CHILD_LIST[$pg_num][$g_num] = $list; // add thao 2023/05/22
		}
	}
	//add thao 2023/05/22
	//	楽天既存アイテムデーター残り削除
	if ($RAKU_LIST) {
		foreach ($RAKU_LIST AS $key => $val) {
			$g_num = trim($key);
			$val = trim($val);
			if ($val) {
				$CALM = array();
				$CALM = explode(",",$val);
				$keys_max = count($CALM)-1;	//	add ohkawara 2020/02/11
				$val = "";
				foreach ($CALM AS $keys => $vals) {
					//if ($keys >= 52) { break; }	//	del ookawara 2015/07/06
					//if ($keys >= 53) { break; }	//	add ookawara 2015/07/06	//	del ohkawara 2017/02/21
					//if ($keys >= 60) { break; }		//	add ohkawara 2017/02/21	//	ohkawara 2018/10/22
					//if ($keys >= 61) { break; }		//	add ohkawara 2018/10/22	//	del  ohkawara 2020/02/11
					if ($keys == 0) { $vals = "d"; }
					$val .= $vals;
					if ($keys_max > $keys) { $val .= ","; }
				}

				$rakuten_item_csv .= $val."\n";
			}
		}
	}


	//	楽天既存カテゴリーデーター残り削除
	if ($RAKU_C_LIST) {
		foreach ($RAKU_C_LIST AS $KEY => $VAL) {
			if ($VAL) {
				foreach ($RAKU_C_LIST[$KEY] AS $key => $val) {
					if ($val) {
						$CHECK_CATEGORYS = explode(",",$val);
						if ($CHECK_CATEGORYS['3'] == "\"その他\"") { continue; }
						if ($CHECK_CATEGORYS['3'] == "\"オークション\"") { continue; }
##						$val = ereg_replace("^\"\"","\"d\"",$val);
						$val = preg_replace("/^\"\"/","\"d\"",$val);
##						$val = ereg_replace("999999999","",$val);
						$val = preg_replace("/999999999/","",$val);
##						$val = ereg_replace(",\"\"$","",$val);
						$val = preg_replace("/,\"\"$/","",$val);
						$rakuten_cat_csv .= $val.",\"\"\n";
					}
				}
			}
		}
	}

	if ($rakuten_item_csv) {
		//$rakuten_item_csv = "コントロールカラム,商品管理番号（商品URL）,商品番号,全商品ディレクトリID,PC用キャッチコピー,モバイル用キャッチコピー,商品名,販売価格,表示価格,消費税,送料,個別送料,送料区分1,送料区分2,代引料,倉庫指定,商品情報レイアウト,注文ボタン,資料請求ボタン,商品問い合わせボタン,モバイル表示,のし対応,PC用商品説明文,モバイル用商品説明文,PC用販売説明文,商品画像URL,商品画像名（ALT）,動画,販売期間指定,注文受付数,在庫タイプ,在庫数,在庫数表示,項目選択肢別在庫用横軸項目名,項目選択肢別在庫用縦軸項目名,項目選択肢別在庫用残り表示閾値,RAC番号,闇市パスワード,カタログID,在庫戻しフラグ,在庫切れ時の注文受付,在庫あり時納期管理番号,在庫切れ時納期管理番号,予約商品発売日,ポイント変倍率,ポイント変倍率適用期間,ヘッダー・フッター・レフトナビ,表示項目の並び順,共通説明文（小）,目玉商品,共通説明文（大）,レビュー本文表示\n$rakuten_item_csv";		//	del ookawara 2015/07/06
		//$rakuten_item_csv = "コントロールカラム,商品管理番号（商品URL）,商品番号,全商品ディレクトリID,PC用キャッチコピー,モバイル用キャッチコピー,商品名,販売価格,表示価格,消費税,送料,個別送料,送料区分1,送料区分2,代引料,倉庫指定,商品情報レイアウト,注文ボタン,資料請求ボタン,商品問い合わせボタン,モバイル表示,のし対応,PC用商品説明文,モバイル用商品説明文,スマートフォン用商品説明文,PC用販売説明文,商品画像URL,商品画像名（ALT）,動画,販売期間指定,注文受付数,在庫タイプ,在庫数,在庫数表示,項目選択肢別在庫用横軸項目名,項目選択肢別在庫用縦軸項目名,項目選択肢別在庫用残り表示閾値,RAC番号,闇市パスワード,カタログID,在庫戻しフラグ,在庫切れ時の注文受付,在庫あり時納期管理番号,在庫切れ時納期管理番号,予約商品発売日,ポイント変倍率,ポイント変倍率適用期間,ヘッダー・フッター・レフトナビ,表示項目の並び順,共通説明文（小）,目玉商品,共通説明文（大）,レビュー本文表示\n$rakuten_item_csv";		//	add ookawara 2015/07/06		//	del ohkawara 2017/02/21

		//$rakuten_item_csv = "コントロールカラム,商品管理番号（商品URL）,商品番号,全商品ディレクトリID,タグID,PC用キャッチコピー,モバイル用キャッチコピー,商品名,販売価格,表示価格,消費税,送料,個別送料,送料区分1,送料区分2,代引料,倉庫指定,商品情報レイアウト,注文ボタン,資料請求ボタン,商品問い合わせボタン,再入荷お知らせボタン,のし対応,PC用商品説明文,モバイル用商品説明文,スマートフォン用商品説明文,PC用販売説明文,商品画像URL,商品画像名（ALT）,動画,販売期間指定,注文受付数,在庫タイプ,在庫数,在庫数表示,項目選択肢別在庫用横軸項目名,項目選択肢別在庫用縦軸項目名,項目選択肢別在庫用残り表示閾値,RAC番号,サーチ非表示,闇市パスワード,カタログID,在庫戻しフラグ,在庫切れ時の注文受付,在庫あり時納期管理番号,在庫切れ時納期管理番号,予約商品発売日,ポイント変倍率,ポイント変倍率適用期間,ヘッダー・フッター・レフトナビ,表示項目の並び順,共通説明文（小）,目玉商品,共通説明文（大）,レビュー本文表示,海外配送管理番号,サイズ表リンク,二重価格文言管理番号,カタログIDなしの理由,配送方法セット管理番号\n$rakuten_item_csv";		//	add ohkawara 2017/02/21	//	del ohkawara 2018/10/22

		//$rakuten_item_csv = "コントロールカラム,商品管理番号（商品URL）,商品番号,全商品ディレクトリID,タグID,PC用キャッチコピー,モバイル用キャッチコピー,商品名,販売価格,表示価格,消費税,送料,個別送料,送料区分1,送料区分2,代引料,倉庫指定,商品情報レイアウト,注文ボタン,資料請求ボタン,商品問い合わせボタン,再入荷お知らせボタン,のし対応,PC用商品説明文,スマートフォン用商品説明文,PC用販売説明文,商品画像URL,商品画像名（ALT）,動画,販売期間指定,注文受付数,在庫タイプ,在庫数,在庫数表示,項目選択肢別在庫用横軸項目名,項目選択肢別在庫用縦軸項目名,項目選択肢別在庫用残り表示閾値,RAC番号,サーチ非表示,闇市パスワード,カタログID,在庫戻しフラグ,在庫切れ時の注文受付,在庫あり時納期管理番号,在庫切れ時納期管理番号,予約商品発売日,ポイント変倍率,ポイント変倍率適用期間,ヘッダー・フッター・レフトナビ,表示項目の並び順,共通説明文（小）,目玉商品,共通説明文（大）,レビュー本文表示,海外配送管理番号,サイズ表リンク,二重価格文言管理番号,カタログIDなしの理由,配送方法セット管理番号,白背景画像URL,メーカー提供情報表示\n$rakuten_item_csv";		//	add ohkawara 2018/10/22	del ohkawara 2020/02/07

		//$rakuten_item_csv = "コントロールカラム,商品管理番号（商品URL）,商品番号,全商品ディレクトリID,タグID,PC用キャッチコピー,モバイル用キャッチコピー,商品名,販売価格,表示価格,消費税,送料,個別送料,送料区分1,送料区分2,代引料,倉庫指定,商品情報レイアウト,注文ボタン,資料請求ボタン,商品問い合わせボタン,再入荷お知らせボタン,のし対応,PC用商品説明文,スマートフォン用商品説明文,PC用販売説明文,商品画像URL,商品画像名（ALT）,動画,販売期間指定,注文受付数,在庫タイプ,在庫数,在庫数表示,項目選択肢別在庫用横軸項目名,項目選択肢別在庫用縦軸項目名,項目選択肢別在庫用残り表示閾値,RAC番号,サーチ非表示,闇市パスワード,カタログID,在庫戻しフラグ,在庫切れ時の注文受付,在庫あり時納期管理番号,在庫切れ時納期管理番号,予約商品発売日,ポイント変倍率,ポイント変倍率適用期間,ヘッダー・フッター・レフトナビ,表示項目の並び順,共通説明文（小）,目玉商品,共通説明文（大）,レビュー本文表示,海外配送管理番号,サイズ表リンク,二重価格文言管理番号,カタログIDなしの理由,配送方法セット管理番号,白背景画像URL,メーカー提供情報表示,地域別個別送料管理番号,消費税率\n$rakuten_item_csv";		//	add ohkawara 2020/02/07	//	del ohkawara 2020/02/18

		$rakuten_item_csv = "コントロールカラム,商品管理番号（商品URL）,商品番号,全商品ディレクトリID,タグID,PC用キャッチコピー,モバイル用キャッチコピー,商品名,販売価格,表示価格,消費税,送料,個別送料,送料区分1,送料区分2,代引料,倉庫指定,商品情報レイアウト,注文ボタン,資料請求ボタン,商品問い合わせボタン,再入荷お知らせボタン,のし対応,PC用商品説明文,スマートフォン用商品説明文,PC用販売説明文,商品画像URL,商品画像名（ALT）,動画,販売期間指定,注文受付数,在庫タイプ,在庫数,在庫数表示,項目選択肢別在庫用横軸項目名,項目選択肢別在庫用縦軸項目名,項目選択肢別在庫用残り表示閾値,RAC番号,サーチ非表示,闇市パスワード,カタログID,在庫戻しフラグ,在庫切れ時の注文受付,在庫あり時納期管理番号,在庫切れ時納期管理番号,予約商品発売日,ポイント変倍率,ポイント変倍率適用期間,ヘッダー・フッター・レフトナビ,表示項目の並び順,共通説明文（小）,目玉商品,共通説明文（大）,レビュー本文表示,海外配送管理番号,サイズ表リンク,二重価格文言管理番号,カタログIDなしの理由,配送方法セット管理番号,白背景画像URL,メーカー提供情報表示,地域別個別送料管理番号,消費税率,単品配送設定,単品配送設定使用理由\n$rakuten_item_csv";		//	add ohkawara 2020/02/18

#		$rakuten_item_csv = mb_convert_encoding($rakuten_item_csv,"sjis-win","eucjp-win");
		$rakuten_item_csv = mb_convert_encoding($rakuten_item_csv,"sjis-win","UTF-8");
		$OUT = fopen(RAKUTEN_ITEM_FILE,"w");
		fwrite($OUT,$rakuten_item_csv);
		fclose($OUT);
		@chmod(RAKUTEN_ITEM_FILE,0666);
		$msg .= "楽天アイテムcsvファイル作成完了いたしました。<br>\n<br>\n";
	} else {
		if (file_exists(RAKUTEN_ITEM_FILE)) { unlink(RAKUTEN_ITEM_FILE); }
		$msg .= "最新の情報がないので楽天アイテムcsvファイル作成しませんでした。<br>\n<br>\n";
	}

	if ($rakuten_select_csv) {
		//$rakuten_select_csv = "項目選択肢用コントロールカラム,商品管理番号（商品URL）,選択肢タイプ,Select/Checkbox用項目名,Select/Checkbox用選択肢,項目選択肢別在庫用横軸選択肢,項目選択肢別在庫用横軸選択肢子番号,項目選択肢別在庫用縦軸選択肢,項目選択肢別在庫用縦軸選択肢子番号,項目選択肢別在庫用取り寄せ可能表示,項目選択肢別在庫用在庫数\n$rakuten_select_csv";	//	del ookawara 2015/01/15
		//$rakuten_select_csv = "項目選択肢用コントロールカラム,商品管理番号（商品URL）,選択肢タイプ,Select/Checkbox用項目名,Select/Checkbox用選択肢,項目選択肢別在庫用横軸選択肢,項目選択肢別在庫用横軸選択肢子番号,項目選択肢別在庫用縦軸選択肢,項目選択肢別在庫用縦軸選択肢子番号,項目選択肢別在庫用取り寄せ可能表示,項目選択肢別在庫用在庫数,在庫あり時納期管理番号\n$rakuten_select_csv";	//	add ookawara 2015/01/15	//	del ohkawara  2020/02/07
		$rakuten_select_csv = "項目選択肢用コントロールカラム,商品管理番号（商品URL）,選択肢タイプ,項目選択肢項目名,項目選択肢,項目選択肢別在庫用横軸選択肢,項目選択肢別在庫用横軸選択肢子番号,項目選択肢別在庫用縦軸選択肢,項目選択肢別在庫用縦軸選択肢子番号,項目選択肢別在庫用取り寄せ可能表示,項目選択肢別在庫用在庫数,在庫戻しフラグ,在庫切れ時の注文受付,在庫あり時納期管理番号,在庫切れ時納期管理番号,タグID,画像URL,項目選択肢選択必須\n$rakuten_select_csv";	//	add ohkawara 2020/02/07

#		$rakuten_select_csv = mb_convert_encoding($rakuten_select_csv,"sjis-win","eucjp-win");
		$rakuten_select_csv = mb_convert_encoding($rakuten_select_csv,"sjis-win","UTF-8");
		$OUT = fopen(RAKUTEN_SELECT_FILE,"w");
		fwrite($OUT,$rakuten_select_csv);
		fclose($OUT);
		@chmod(RAKUTEN_SELECT_FILE,0666);
		$msg .= "楽天セレクトcsvファイル作成完了いたしました。<br>\n<br>\n";
	} else {
		if (file_exists(RAKUTEN_SELECT_FILE)) { unlink(RAKUTEN_SELECT_FILE); }
		$msg .= "最新の情報がないので楽天セレクトcsvファイル作成しませんでした。<br>\n<br>\n";
	}

	if ($rakuten_cat_csv) {
		$rakuten_cat_csv = "コントロールカラム,商品管理番号（商品URL）,商品名,表示先カテゴリ,優先度,URL,1ページ複数形式\n$rakuten_cat_csv";

#		$rakuten_cat_csv = mb_convert_encoding($rakuten_cat_csv,"sjis-win","eucjp-win");
		$rakuten_cat_csv = mb_convert_encoding($rakuten_cat_csv,"sjis-win","UTF-8");
		$OUT = fopen(RAKUTEN_CAT_FILE,"w");
		fwrite($OUT,$rakuten_cat_csv);
		fclose($OUT);
		@chmod(RAKUTEN_CAT_FILE,0666);
		$msg .= "楽天カテゴリーcsvファイル作成完了いたしました。<br>\n<br>\n";
	} else {
		if (file_exists(RAKUTEN_CAT_FILE)) { unlink(RAKUTEN_CAT_FILE); }
		$msg .= "最新の情報がないので楽天カテゴリーcsvファイル作成しませんでした。<br>\n<br>\n";
	}
    rakuten_parent_child_item_csv($pg_num);
	//	配列削除
	//	add ohkawara 2017/02/22
	unset($TAG_SIZE_LIST);
	unset($RKT_DIR_LIST);


	//	配列・csvデータ削除	add ohkawara 2018/08/31
	unset($RAKU_C_LIST);
	unset($RAKU_S_LIST);
	unset($RAKU_LIST);
	unset($CHECK_ITEM);
	unset($UP_PIC_LIST);
	unset($TAG_BRAND_LIST);
	unset($L_CATE);
	unset($UP_PIC_LIST);

	unset($rakuten_item_csv);
	unset($rakuten_select_csv);
	unset($rakuten_cat_csv);
	//	空画像フォルダー削除
	del_dir();

	return array($msg,$ERROR);

}

function rakuten_parent_child_item_csv(){
	global $conn_id;
	$sql = "SELECT  i.g_num, i.ypath, i.display, i.copy_num, i.state," .
			" coalesce(j.name_head, '') || coalesce(j.g_name, '') ||  coalesce(j.name_foot, '') AS g_name,".	//	add ookawara 2015/09/24
			" j.code, j.price, j.sale_price, j.options, j.caption, j.brand," . 	
			" j.rd_id,".
			" j.zaiko, j.size_list"."j.gp_option_name".								
			" FROM ".T_CATEGORY." i, ".T_GOODS." j" .
			" WHERE i.g_num=j.g_num AND j.wowmashop='1' AND j.soldout!='1' AND i.display='1'" .			//	add ohkawara 2020/12/07	yahooでナイキが出品が出来なくなったのでwowmaと共通にした。
			" AND i.cate1>='20' AND i.state!='3'".
			" AND i.cate1 NOT IN ('98','99')".	
			" ORDER BY i.cate1, i.cate2, i.cate3 DESC;";

	if ($result = mysqli_query($conn_id,$sql)) {
		while ($list = mysqli_fetch_array($result)) {
			unset($CHECK_SELECT);
			unset($check_cate_name);
			$g_num = $list['g_num'];
			$pg_num = $list['pg_num'];
		    unset($list['pg_num']);
		    unset($list['g_num']);
			$RAKU_PARENT_CHILD_LIST[$pg_num][$g_num]=[
				"size_list" => $list['size_list'],
    			"gp_option_name" => $list['gp_option_name']
			];

		}
	}
	return $RAKU_PARENT_CHILD_LIST;

}
//	楽天アイテム
function rakuten_item_csv($list, $L_CATE, $L_BRAND, $L_RAKU, &$RAKU_LIST, $RAKU_SP_LIST, $UP_PIC_LIST, &$att_check, $TAG_BRAND_LIST, $TAG_SIZE_LIST, $RKT_DIR_LIST) {	//	add ohkawara 2017/02/21-22
	global $attention,$NEWCATE,$RAKUTEN_GOODS_DISCOUNT_CATE;
	global $MOVIE_LIST;	//	add ookawara 2014/10/10
	global $conn_id;	//	add ookawara 2014/12/29
	global $NON_BRAND;	//	add ookawara 2015/01/09
	global $nouki_days_msg;	//	add ookawara 2015/01/15
	global $attention_wowma;	//	add ohkawara 2017/04/07

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
	$price = $list['price'];
	$sale_price = $list['sale_price'];
	$options = $list['options'];
	$caption = $list['caption'];
	$brand = $list['brand'];
	$rd_id = $list['rd_id'];
	$zaiko = $list['zaiko'];		//	add ookawara 2015/09/17
	$size_list = $list['size_list'];	//	add ookawara 2015/09/17
	//$name_head = $list['name_head'];	//	add ookawara 2015/09/17	//	del ookawara 2015/09/24
	//$g_name = $name_head.$g_name;	//	add ookawara 2015/09/17		//	del ookawara 2015/09/24

	//	該当するブランドは削除
	//	add ookawara 2015/01/08
	if ($NON_BRAND) {
		if ($NON_BRAND[$brand] == 1) {
			return;
		}
	}

//	消費税別で表示する為コメント化	2009/10/30
//	$taxable = TAXABLE;
	$taxable = "0";

	//	コントロールカラム	concaramu
	if (!$RAKU_LIST[$g_num] && $state < 3) { $concaramu = "n"; }
	elseif ($RAKU_LIST[$g_num] && $state > 0 && $state < 3) { $concaramu = "u"; }
	elseif ($RAKU_LIST[$g_num] && $state == 3) { $concaramu = "d"; }

	//	ポイント設定
	$point = "";
	$point_start = "";
	$point_last = "";
	$point_type = 0;
	if (RAKUTEN_POINT_KIKAN != "") {
		if ($price > 0 && $sale_price > 0 && $price < $sale_price && RAKUTEN_POINT_TE > 0) {
			$point = RAKUTEN_POINT_TE;
			$point_start = RAKUTEN_POINT_START;
			$point_last = RAKUTEN_POINT_LAST;
			$point_type = 2;
		} elseif (RAKUTEN_POINT_TU > 0) {
			$point = RAKUTEN_POINT_TU;
			$point_start = RAKUTEN_POINT_START;
			$point_last = RAKUTEN_POINT_LAST;
			$point_type = 1;
		}
	}

	//	price,sale_price
//	if (TAXABLE == 1) {	//	del ohkawara 2021/03/23
		//	消費税込みで表示する場合

		//	割引設定項目
		$GOODS_DISCOUNT_C = RAKUTEN_GOODS_DISCOUNT_C;
		$GOODS_DISCOUNT_CATE = $RAKUTEN_GOODS_DISCOUNT_CATE;
		$DISCOUNT_PAR = RAKUTEN_DISCOUNT_PAR;
		$DISCOUNT_PAR2 = RAKUTEN_DISCOUNT_PAR2;

		if ($price > 0 && $sale_price > 0 && $price < $sale_price) {
			//	金額と特価共に値が有、金額より特価の金額が大きい場合　定価（金額）売り
//			$sale_price = 0;
			$sale_price = $price;
		} elseif ($price > 0 && $sale_price > 0 && $price == $sale_price) {
			//	金額と特価共に値が有、金額と特価が同じ場合で、セールスしないカテゴリーの商品では無い場合　定価・割引共通商品割引率売り
			if ($GOODS_DISCOUNT_C == 1 && $DISCOUNT_PAR2 > 0) {
				$flag = 0;
				if ($GOODS_DISCOUNT_CATE) {
					foreach ($GOODS_DISCOUNT_CATE AS $val) {
						$val = trim($val);
						if ($cate1 == $val) { $flag = 1; }
					}
				}
				if ($flag != 1) {
					$sale_price = floor($price * ((100 - $DISCOUNT_PAR2) / 100));
				} else {
					$sale_price = 0;
				}
			} else {
				$sale_price = 0;
			}
		} elseif ($GOODS_DISCOUNT_C == 1) {
			//	割引有の場合で、セールスしないカテゴリーの商品では無い場合　割引率売り
			$flag = 0;
			if ($GOODS_DISCOUNT_CATE) {
				foreach ($GOODS_DISCOUNT_CATE AS $val) {
					$val = trim($val);
					if ($cate1 == $val) { $flag = 1; }
				}
			}
			if ($flag != 1) {
				$sale_price_ = floor($price * ((100 - $DISCOUNT_PAR) / 100));
				if ($sale_price < 1 || ($sale_price > 0 && $sale_price > $sale_price_)) {
					$sale_price = $sale_price_;
				}
			} else {
				$sale_price = 0;
			}
		}
//	消費税別で表示する為コメント化	2009/10/30
//		$price = floor($price * ((TAX / 100) + 1) + 0.5);
//		$sale_price = floor($sale_price * ((TAX / 100) + 1) + 0.5);

//	del ohkawara 2021/03/23
//	} else {
//		//	消費税抜きで表示する場合
//		if ($price > 0 && $sale_price > 0 && $price < $sale_price) {
//			$sale_price = $price;
//		} elseif ($price > 0 && $sale_price > 0 && $price == $sale_price) {
//			$sale_price = "";
//		}
//		$price = floor($price);
//		$sale_price = floor($sale_price);
//	}

	//	add ohkawara 2021/03/23
	if (TAXABLE == 1) {
		//	税込み表示が必要になった場合
		$price = floor($price * ((TAX / 100) + 1) + 0.5);
		$sale_price = floor($sale_price * ((TAX / 100) + 1) + 0.5);
	}

	if ($sale_price > 0) {
		$price1 = $sale_price;
		$price2 = $price;
	} else {
		$price1 = $price;
		$price2 = "0";
	}
	$wariritu = wariritu($price2,$price1);

	//	メーカー小売価格のみ消費税を追加する為の処理	2009/11/02
	if ($price2 > 0) {
		$price2 = floor($price2 * ((TAX / 100) + 1) + 0.5);
	}

/*
	//	下方に移動
	//	del ookawara 2013/12/17
	//	PC用キャッチコピー
	$pcccopy = "";
	if ($L_BRAND[$brand]) { $pcccopy = $L_BRAND[$brand]; }
	if ($wariritu) {
		//$pcccopy .= "【".mb_convert_kana($wariritu,"N")."％ＯＦＦ】";	//	del ookawara 2014/04/30
		if (defined(RAKUTEN_DISCOUNT_TIME) && $wariritu <= RAKUTEN_DISCOUNT_PAR) {
			$pcccopy .= "【".RAKUTEN_DISCOUNT_TIME."】";
		}
	}

	//	モバイル用キャッチコピー
	$moccopy = "";
	if ($L_BRAND[$brand]) { $moccopy = $L_BRAND[$brand]; }
	if ($wariritu && strlen($moccopy) < 42) {
		//$moccopy .= "【".mb_convert_kana($wariritu,"N")."％ＯＦＦ】";	//	del ookawara 2014/04/30
		if (defined(RAKUTEN_DISCOUNT_TIME) && $wariritu <= RAKUTEN_DISCOUNT_PAR) {
			$moccopy .= "【".RAKUTEN_DISCOUNT_TIME."】";
		}
	}
*/

	//	倉庫指定	souko
	$souko = "0";

    //サーチ表示  add thao 2023/05/31
	$search_display= "1";

	//	カテゴリー
	$cate1_name = $L_CATE[$cate1][0][0];
	$cate2_name = $L_CATE[$cate1][$cate2][0];
	$cate3_name = $L_CATE[$cate1][$cate2][$cate3];
	$cate0_name = $NEWCATE[$cate1]['name'];

	$category_name = $cate0_name;
	$category_name2 = $cate0_name;
	$category_list = $cate0_name;
	if ($cate0_name != $cate1_name) {
		$category_name .= " / ".$cate1_name;
		$category_name2 .= " / ".$cate1_name;
		$category_list .= "\\".$cate1_name;
	}
	if ($cate1_name != $cate2_name) {
		$category_name .= " / ".$cate2_name;
		$category_name2 .= " / ".$cate2_name;
		$category_list .= "\\".$cate2_name;
	}
	if ($cate2_name != $cate3_name) {
		$category_name .= " / ".$cate3_name;
		$category_list .= "\\".$cate3_name;
	}


	//	動画埋め込み
	//	add ookawara 2014/10/10
	$movie_msg = "";
	preg_match_all("|<!-- MOVIE=(.*) -->|U", $caption, $MOVIE);
	if ($MOVIE) {
		foreach ($MOVIE[1] AS $key => $VAL) {
			$movie_name = $MOVIE[0][$key];
			$movie_name = change_en($movie_name);

			$movie_num = trim($VAL);
			$movie_code = "";
			$movie_code = $MOVIE_LIST[$movie_num]['R'];
			if ($movie_code) {
				$movie_code = $movie_code;
			}

			$caption = preg_replace("/{$movie_name}/", "", $caption);
			$movie_msg .= $movie_code;
		}
	}


	//	商品説明文
	//$att_check = 0;	//	del ookawara 2015/01/15
	//	PC用商品説明文	pc_msg
	$caption_ = $caption;
##	if (ereg("<!-- Attention -->",$caption)) {
	if (preg_match("/<!-- Attention -->/",$caption)) {
##		$caption = ereg_replace("<!-- Attention -->","$attention_wowma",$caption);	//	add ohkawara 2017/04/07
		$caption = preg_replace("/<!-- Attention -->/","$attention_wowma",$caption);	//	add ohkawara 2017/04/07
##		//$caption = ereg_replace("<!-- Attention -->","$attention",$caption);	//	del ookawara 2015/01/16
		//$caption = preg_replace("/<!-- Attention -->/","$attention",$caption);	//	del ookawara 2015/01/16
##		//$caption = ereg_replace("<!-- Attention -->", "", $caption);			//	add ookawara 2015/01/16		//	del ohkawara 2017/04/05
		//$caption = preg_replace("/<!-- Attention -->/", "", $caption);			//	add ookawara 2015/01/16		//	del ohkawara 2017/04/05
		$caption = trim($caption);
		$att_check = 1;
	} else {			//	add ookawara 2015/01/15
		//$caption  .= $nouki_days_msg;	//	add ookawara 2015/01/15		//	del ookawara 2015/01/16
		//$caption_ .= $nouki_days_msg;	//	add ookawara 2015/01/15		//	del ookawara 2015/01/16

		$caption  .= $nouki_days_msg2;	//	add ohkawara 2017/04/07
		$caption_ .= $nouki_days_msg2;	//	add ohkawara 2017/04/07
	}


##	$caption = ereg_replace("<","&lt;",$caption);
	$caption = preg_replace("/</","&lt;",$caption);
##	$caption = ereg_replace(">","&gt;",$caption);
	$caption = preg_replace("/>/","&gt;",$caption);

	$caption = preg_replace("/&lt;font color=#FF0000&gt;/", "<font color=#FF0000>", $caption);	//	add ohkawara 2017/04/07
	$caption = preg_replace("/&lt;\/font&gt;/", "</font>", $caption);							//	add ohkawara 2017/04/07

##	$caption = ereg_replace("&lt;font","<font",$caption);
	$caption = preg_replace("/&lt;font/","<font",$caption);
##	$caption = ereg_replace("\"&gt;","\">",$caption);
	$caption = preg_replace("/\"&gt;/","\">",$caption);
##	$caption = ereg_replace("&lt;\/font&gt;","</font>",$caption);
	$caption = preg_replace("/&lt;\/font&gt;/","</font>",$caption);
##	//$caption = ereg_replace("&lt;\br /&gt;","</font>",$caption);	//	del ookawara 2015/08/07
	//$caption = preg_replace("/&lt;\br \/&gt;/","</font>",$caption);	//	del ookawara 2015/08/07
##	//$caption = ereg_replace("&lt;\br&gt;","</font>",$caption);	//	del ookawara 2015/08/07
	//$caption = preg_replace("/&lt;\br&gt;/","</font>",$caption);	//	del ookawara 2015/08/07
##	$caption = ereg_replace("&lt;\br /&gt;","<br />",$caption);		//	add ookawara 2015/08/07
	$caption = preg_replace("/&lt;\br \/&gt;/","<br />",$caption);		//	add ookawara 2015/08/07
##	$caption = ereg_replace("&lt;\br&gt;","<br>",$caption);			//	add ookawara 2015/08/07
	$caption = preg_replace("/&lt;\br&gt;/","<br>",$caption);			//	add ookawara 2015/08/07



	$caption = nl2br($caption);
	$caption = <<<WAKABA
$attation
ブランド名：$L_BRAND[$brand]<br />
$caption
WAKABA;
##	$caption = ereg_replace(",","、",$caption);
	$caption = preg_replace("/,/","、",$caption);
	$pc_msg = "<p style=\"width: 500px;\">$caption";
##	$pc_msg = ereg_replace("\r","",$pc_msg);
	$pc_msg = preg_replace("/\r/","",$pc_msg);
##	$pc_msg = ereg_replace("\n","",$pc_msg);
	$pc_msg = preg_replace("/\n/","",$pc_msg);
##	$pc_msg = ereg_replace("<br />","<br>",$pc_msg);
	$pc_msg = preg_replace("/<br \/>/","<br>",$pc_msg);

	//	モバイル用商品説明文	mobile_msg
	$caption_ = <<<WAKABA
$attention_wowma
ブランド名：$L_BRAND[$brand]<br />
$caption_
WAKABA;


	if ($att_check == 1) {
##		//$caption_ = ereg_replace("<!-- Attention -->", "", $caption_);	//	del ookawara 2015/01/16
		//$caption_ = preg_replace("/<!-- Attention -->/", "", $caption_);	//	del ookawara 2015/01/16
##		$caption_ = ereg_replace("<!-- Attention -->", "", $caption_);	//	add ookawara 2015/01/16
		$caption_ = preg_replace("/<!-- Attention -->/", "", $caption_);	//	add ookawara 2015/01/16
	}
##	$caption_ = ereg_replace("<","&lt;",$caption_);
	$caption_ = preg_replace("/</","&lt;",$caption_);
##	$caption_ = ereg_replace(">","&gt;",$caption_);
	$caption_ = preg_replace("/>/","&gt;",$caption_);
	$caption_ = nl2br($caption_);
	$mobile_msg = strip_tags($caption_,"<br /><br />");
	$m_len = strlen($mobile_msg);
	if ($m_len > 512) {
		$mobile_msg = <<<WAKABA
$attation
ブランド名：$L_BRAND[$brand]
WAKABA;

		if ($att_check == 1) {
			//$mobile_msg2 = "$mobile_msg<br />$attention2";	//	del ookawara 2015/01/16
			$mobile_msg2 = "$mobile_msg";						//	add ookawara 2015/01/16
			$m_len = strlen($mobile_msg2);
			if ($m_len <= 512) {
				$mobile_msg = $mobile_msg2;
			} else {
				//$mobile_msg = "$attention2";					//	del ookawara 2015/01/16
			}
		}

	}

	$mobile_msg = nl2br($mobile_msg);																				//	add ohkawara 2017/04/07
	$mobile_msg = preg_replace("/&lt;br \/&gt;/", "<br />", $mobile_msg);											//	add ohkawara 2017/04/06
	$mobile_msg = preg_replace("/&lt;br&gt;/", "<br>", $mobile_msg);												//	add ohkawara 2017/04/06
	$mobile_msg = preg_replace("/&lt;font color=#FF0000&gt;/", "<font color=#FF0000>", $mobile_msg);				//	add ohkawara 2017/04/07
	$mobile_msg = preg_replace("/&lt;\/font&gt;/", "</font>", $mobile_msg);											//	add ohkawara 2017/04/07

##	$mobile_msg = ereg_replace("\r","",$mobile_msg);
    $mobile_msg = preg_replace("/\r/","",$mobile_msg);
##	$mobile_msg = ereg_replace("\n","",$mobile_msg);
	$mobile_msg = preg_replace("/\n/","",$mobile_msg);
##	$mobile_msg = ereg_replace("<br />","<br>",$mobile_msg);
    $mobile_msg = preg_replace("/<br \/>/","<br>",$mobile_msg);




	//	画像	img_msg	img_alt_msg
	if (!file_exists(UP_IMG_01_DIR)) {
		mkdir(UP_IMG_01_DIR,0777);
		@chmod(UP_IMG_01_DIR,0777);
	}
	if (!file_exists(UP_IMG_02_DIR)) {
		mkdir(UP_IMG_02_DIR,0777);
		@chmod(UP_IMG_02_DIR,0777);
	}
	if (!file_exists(UP_IMG_03_DIR)) {
		mkdir(UP_IMG_03_DIR,0777);
		@chmod(UP_IMG_03_DIR,0777);
	}
	if (!file_exists(UP_IMG_04_DIR)) {
		mkdir(UP_IMG_04_DIR,0777);
		@chmod(UP_IMG_04_DIR,0777);
	}
	if (!file_exists(UP_IMG_05_DIR)) {
		mkdir(UP_IMG_05_DIR,0777);
		@chmod(UP_IMG_05_DIR,0777);
	}

	//	add ookawara 2015/09/27
	if (!file_exists(UP_IMG_06_DIR)) {
		mkdir(UP_IMG_06_DIR,0777);
		@chmod(UP_IMG_06_DIR,0777);
	}
	if (!file_exists(UP_IMG_07_DIR)) {
		mkdir(UP_IMG_07_DIR,0777);
		@chmod(UP_IMG_07_DIR,0777);
	}
	if (!file_exists(UP_IMG_08_DIR)) {
		mkdir(UP_IMG_08_DIR,0777);
		@chmod(UP_IMG_08_DIR,0777);
	}
	if (!file_exists(UP_IMG_09_DIR)) {
		mkdir(UP_IMG_09_DIR,0777);
		@chmod(UP_IMG_09_DIR,0777);
	}

	$fd_num = floor($g_num / 2000);	//	update 500→2000 ookawara 2013/12/04
	$amari = $g_num % 2000;	//	update 500→2000 ookawara 2013/12/04
	if ($amari > 0) { $fd_num += 1; }
	$fd_num = $fd_num * 2000;	//	update 500→2000 ookawara 2013/12/04
	if ($fd_num > 0) {
		$image01 = "img01_$fd_num";
		$image02 = "img02_$fd_num";
		$image03 = "img03_$fd_num";	//	add ookawara 2013/07/03
		$image04 = "img04_$fd_num";	//	add ookawara 2013/10/10
		$image05 = "img05_$fd_num";	//	add ookawara 2013/10/10
		$image06 = "img06_$fd_num";	//	add thao 2023/06/01
		$image07 = "img07_$fd_num";	//	add thao 2023/06/01
		$image08 = "img08_$fd_num";	//	add thao 2023/06/01
		$image09 = "img09_$fd_num";	//	add thao 2023/06/01
		$dir01 = UP_IMG_01_DIR."/$image01";
		$dir02 = UP_IMG_02_DIR."/$image02";
		$dir03 = UP_IMG_03_DIR."/$image03";	//	add ookawara 2013/07/03
		$dir04 = UP_IMG_04_DIR."/$image04";	//	add ookawara 2013/10/10
		$dir05 = UP_IMG_05_DIR."/$image05";	//	add ookawara 2013/10/10

		$dir06 = UP_IMG_06_DIR."/$image06";	//	add ookawara 2015/09/27
		$dir07 = UP_IMG_07_DIR."/$image07";	//	add ookawara 2015/09/27
		$dir08 = UP_IMG_08_DIR."/$image08";	//	add ookawara 2015/09/27
		$dir09 = UP_IMG_09_DIR."/$image09";	//	add ookawara 2015/09/27

		if (!file_exists($dir01)) {
			mkdir($dir01,0777);
			@chmod($dir01,0777);
		}
		if (!file_exists($dir02)) {
			mkdir($dir02,0777);
			@chmod($dir02,0777);
		}

		//	add ookawara 2013/07/04
		if (!file_exists($dir03)) {
			mkdir($dir03,0777);
			@chmod($dir03,0777);
		}

		//	add ookawara 2013/10/10
		if (!file_exists($dir04)) {
			mkdir($dir04,0777);
			@chmod($dir04,0777);
		}

		//	add ookawara 2013/10/10
		if (!file_exists($dir05)) {
			mkdir($dir05,0777);
			@chmod($dir05,0777);
		}

		//	add ookawara 2015/09/27
		if (!file_exists($dir06)) {
			mkdir($dir06,0777);
			@chmod($dir06,0777);
		}
		if (!file_exists($dir07)) {
			mkdir($dir07,0777);
			@chmod($dir07,0777);
		}
		if (!file_exists($dir08)) {
			mkdir($dir08,0777);
			@chmod($dir08,0777);
		}
		if (!file_exists($dir09)) {
			mkdir($dir09,0777);
			@chmod($dir09,0777);
		}

	}

	//	全商品ディレクトリID	category_id
	$category_id = $rd_id;
	//	add ohkawara 2017/02/22
	$tag_chk_flg = 0;
	if ($RAKU_LIST) {
		$line = $RAKU_LIST[$g_num];
		$LINE_VAL = str_getcsv($line,',','"');
		$seted_category_id = $LINE_VAL[3];

		if ($category_id != $seted_category_id) {
			if (isset($RKT_DIR_LIST[$seted_category_id]) && $RKT_DIR_LIST[$seted_category_id] != 1) {
				$tag_chk_flg = 1;
				$category_id = $seted_category_id;
			}
		}
	}

	//	タグID
	//	add ohkawara 2017/02/21
	$set_tag_id = "";
	$TAG_LIST = array();
	$brand_name = $L_BRAND[$brand];

	//if ($TAG_BRAND_LIST) {											//	del ohkawara 2017/04/06
	//	foreach ($TAG_BRAND_LIST AS $key_name => $id_num) {				//	del ohkawara 2017/04/06
	if ($TAG_BRAND_LIST[$rd_id]) {										//	add ohkawara 2017/04/06
		foreach ($TAG_BRAND_LIST[$rd_id] AS $key_name => $id_num) {		//	add ohkawara 2017/04/06
			if (preg_match("/$key_name/", $brand_name)) {
				$TAG_LIST[] = $id_num;
				break;
			}
		}
	}
	//	サイズは以下で設定

	//	サイズチェック
	unset($val);
	$new_pic_check = 0;
	$op_check = 0;
	$op_check2 = 0;
	$jan_code = "";	//	add ookawara 2016/06/22
	if ($concaramu != "d") {
		//	add ookawara 2015/09/17	start
		$L_SIZE = array();
		if ($size_list) {
			$op_size = unserialize($size_list);
		}
		if ($op_size) {
			foreach ($op_size AS $key => $VAL) {

				//	add ookawara 2016/06/22
				if ($VAL['jan'] != "none" || $VAL['jan'] > 0) {
					if (!preg_match("/[^0-9]/", $VAL['jan'])) {
						$jan_code = $VAL['jan'];
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
		if ($L_SIZE) {
			foreach ($L_SIZE AS $val) {
				$val = preg_replace("/㎝/","cm",$val);	//	2010/06/10 ookawara
				$val = trim($val);
				if ($val == "") { continue; }
##				$val = eregi_replace("\[.*\]","",$val);
                $val = preg_replace("/\[.*\]/i","",$val);

				if ($state <= 2) {
##					if (ereg("^\*",$val)) {
                    if ( preg_match("/^\*/",$val)) {
						$op_check = 1;
						continue;
					}
##					$val = eregi_replace("^\/","",$val);
					$val = preg_replace("/^\//i","",$val);
				}

##				if (ereg("^\/",$val)) { $new_pic_check = 1; }
				if (preg_match("/^\//",$val)) { $new_pic_check = 1; }

				if ($val == "F" || 
					$val == "フリー" || 
					$val == "フリーサイズ" || 
					$val == "--" || 
					$val == "フットサルサイズ" || 
					$val == "設定なし"
				) {
					$op_check2 = 1;	//	add ookawara 2010/05/25
					unset($val);
					continue;
				} elseif ($val) {
					$op_check2 = 1;	//	add ookawara 2010/05/25
					$flag = 1;
				}

				//	サイズ　タグID取得
				//	add ohkawara 2017/02/21
				$size_val = preg_replace("/cm/", "", $val);
				//if ($TAG_SIZE_LIST[$size_val]) {						//	del ohkawara 2017/04/06
				//	$TAG_LIST[] = $TAG_SIZE_LIST[$size_val];			//	del ohkawara 2017/04/06
				//}														//	del ohkawara 2017/04/06
				if ($TAG_SIZE_LIST[$rd_id][$size_val]) {				//	add ohkawara 2017/04/06
					$TAG_LIST[] = $TAG_SIZE_LIST[$rd_id][$size_val];	//	add ohkawara 2017/04/06
				}														//	add ohkawara 2017/04/06
			}
		}
	}

	//	サイズ　タグIDセット
	if ($TAG_LIST) {
		foreach ($TAG_LIST AS $set_id) {
			if ($set_tag_id != "") {
				$set_tag_id .= "/";
			}
			$set_tag_id .= $set_id;
		}
	}
	if ($tag_chk_flg == 1) {
		$set_tag_id = "";
	}

	//	add ookawara 2010/05/25
	if ($op_check == 1 && $op_check2 != 1) {
		return;
	}

	// 商品画像タイプ
	$image_type = RAKUTEN_IMAGE_TYPE;// add thao 2023/06/01

	if ($concaramu != "d") {
#		$g_name_ = mb_convert_kana("$g_name","S","EUC-JP");
		$g_name_ = mb_convert_kana("$g_name","S","UTF-8");
		//	一枚目画像
		$imagef_file = IMG_F_DIR."/$code" . ".jpg";
		if (file_exists($imagef_file)) {
			$imgfile = "$dir01/$g_num.jpg"; 
			if (!file_exists($imgfile)) {
				if (filemtime($imagef_file) > CHECK_TIME || $new_pic_check == 1 || $UP_PIC_LIST[$g_num] != "") {	//	add  || $UP_PIC_LIST[$g_num] != "" ookawara 2014/02/06
					copy($imagef_file,$imgfile);
				}
			}
			$image01_path = "/image01/$image01/$g_num.jpg";// add thao 2023/06/01
			 $img_msg = "https://image.rakuten.co.jp/nbs-soccer/cabinet/image01/$image01/$g_num.jpg ";
		}

		//	二枚目画像
		$imageb_file = IMG_B_DIR."/$code" . ".jpg";
		if (file_exists($imageb_file)) {
			$imgfile = "$dir02/$g_num.jpg";
			if (!file_exists($imgfile)) {
				if (filemtime($imageb_file) > CHECK_TIME || $new_pic_check == 1 || $UP_PIC_LIST[$g_num] != "") {	//	add  || $UP_PIC_LIST[$g_num] != "" ookawara 2014/02/06
				//if (filectime($imageb_file) > CHECK_TIME || $new_pic_check == 1) {
				//if (filectime($imageb_file) > CHECK_TIME) {
					copy($imageb_file,$imgfile);
				}
			}
			$image02_path = "/image02/$image02/$g_num.jpg";// add thao 2023/06/01
			$img_msg .= "https://image.rakuten.co.jp/nbs-soccer/cabinet/image02/$image02/$g_num.jpg ";
		}

		//	add ookawara 2013/07/04
		//	3枚目画像
		$image03_file = IMG_03_DIR."/$code" . ".jpg";
		if (file_exists($image03_file)) {
			$imgfile = "$dir03/$g_num.jpg";
			if (!file_exists($imgfile)) {
				if (filemtime($image03_file) > CHECK_TIME || $new_pic_check == 1 || $UP_PIC_LIST[$g_num] != "") {	//	add  || $UP_PIC_LIST[$g_num] != "" ookawara 2014/02/06
					copy($image03_file,$imgfile);
				}
			}
			$image03_path = "/image03/$image03/$g_num.jpg";// add thao 2023/06/01
			$img_msg .= "https://image.rakuten.co.jp/nbs-soccer/cabinet/image03/$image03/$g_num.jpg ";
		}

		//	add ookawara 2013/10/10
		//	4枚目画像
		$image04_file = IMG_04_DIR."/$code" . ".jpg";
		if (file_exists($image04_file)) {
			$imgfile = "$dir04/$g_num.jpg";
			if (!file_exists($imgfile)) {
				if (filemtime($image04_file) > CHECK_TIME || $new_pic_check == 1 || $UP_PIC_LIST[$g_num] != "") {	//	add  || $UP_PIC_LIST[$g_num] != "" ookawara 2014/02/06
					copy($image04_file,$imgfile);
				}
			}
			$image04_path = "/image04/$image04/$g_num.jpg";// add thao 2023/06/01
			$img_msg .= "https://image.rakuten.co.jp/nbs-soccer/cabinet/image04/$image04/$g_num.jpg ";
		}

		//	add ookawara 2013/10/10
		//	5枚目画像
		$image05_file = IMG_05_DIR."/$code" . ".jpg";
		if (file_exists($image05_file)) {
			$imgfile = "$dir05/$g_num.jpg";
			if (!file_exists($imgfile)) {
				if (filemtime($image05_file) > CHECK_TIME || $new_pic_check == 1 || $UP_PIC_LIST[$g_num] != "") {	//	add  || $UP_PIC_LIST[$g_num] != "" ookawara 2014/02/06
					copy($image05_file,$imgfile);
				}
			}
			$image05_path = "/image05/$image05/$g_num.jpg";// add thao 2023/06/01
			$img_msg .= "https://image.rakuten.co.jp/nbs-soccer/cabinet/image05/$image05/$g_num.jpg ";
		}

		//	add ookawara 2015/09/27
		//	6枚目画像
		$image06_file = IMG_06_DIR."/$code" . ".jpg";
		if (file_exists($image06_file)) {
			$imgfile = "$dir06/$g_num.jpg";
			if (!file_exists($imgfile)) {
				if (filemtime($image06_file) > CHECK_TIME || $new_pic_check == 1 || $UP_PIC_LIST[$g_num] != "") {
					copy($image06_file,$imgfile);
				}
			}
			$image06_path = "/image06/$image06/$g_num.jpg";// add thao 2023/06/01
			$img_msg .= "https://image.rakuten.co.jp/nbs-soccer/cabinet/image06/$image06/$g_num.jpg ";
		}
		$image07_file = IMG_07_DIR."/$code" . ".jpg";
		if (file_exists($image07_file)) {
			$imgfile = "$dir07/$g_num.jpg";
			if (!file_exists($imgfile)) {
				if (filemtime($image07_file) > CHECK_TIME || $new_pic_check == 1 || $UP_PIC_LIST[$g_num] != "") {
					copy($image07_file,$imgfile);
				}
			}
			$image07_path = "/image07/$image07/$g_num.jpg";// add thao 2023/06/01
			$img_msg .= "https://image.rakuten.co.jp/nbs-soccer/cabinet/image07/$image07/$g_num.jpg ";
		}
		$image08_file = IMG_08_DIR."/$code" . ".jpg";
		if (file_exists($image08_file)) {
			$imgfile = "$dir08/$g_num.jpg";
			if (!file_exists($imgfile)) {
				if (filemtime($image08_file) > CHECK_TIME || $new_pic_check == 1 || $UP_PIC_LIST[$g_num] != "") {
					copy($image08_file,$imgfile);
				}
			}
			$image08_path = "/image08/$image08/$g_num.jpg";// add thao 2023/06/01
			$img_msg .= "https://image.rakuten.co.jp/nbs-soccer/cabinet/image08/$image08/$g_num.jpg ";
		}
		$image09_file = IMG_09_DIR."/$code" . ".jpg";
		if (file_exists($image09_file)) {
			$imgfile = "$dir09/$g_num.jpg";
			if (!file_exists($imgfile)) {
				if (filemtime($image09_file) > CHECK_TIME || $new_pic_check == 1 || $UP_PIC_LIST[$g_num] != "") {
					copy($image09_file,$imgfile);
				}
			}
			$image09_path = "/image09/$image09/$g_num.jpg";// add thao 2023/06/01
			$img_msg .= "https://image.rakuten.co.jp/nbs-soccer/cabinet/image09/$image09/$g_num.jpg ";
		}

	}


	$size_title = "";
	$zaiko_type = "0";


	if ($price2 < 1) { unset($price2); }
	if ($price1 == $price2) { unset($price2); }
	if ($RAKU_SP_LIST[$g_num] > 0 && $price2 < 1) {
		$price2 = "0";
	}

	//	金額確認送料無料コメントセット
	if ($price1 >= RAKUTEN_SOURYOU_PRICE && defined("RAKUTEN_SOURYOU_MESSAGE")) {
		$g_name = RAKUTEN_SOURYOU_MESSAGE.$g_name;
	}

	//	キーワードセット
	if ($point_type == 1 && $wariritu >= 50) {
		//	割引額によってポイント変更
		$point_type = 2;
		$point = RAKUTEN_POINT_TE;
	}

	if ($point_type == 1 && defined("RAKUTEN_KEYWORD_TU") && RAKUTEN_KEYWORD_TU) {
		$g_name .= RAKUTEN_KEYWORD_TU;
	} elseif ($point_type == 2 && defined("RAKUTEN_KEYWORD_TE") && RAKUTEN_KEYWORD_TE) {
		$g_name .= RAKUTEN_KEYWORD_TE;
	}
	$g_name  = preg_replace("/,/","、",$g_name);					//	add ookawara 2013/05/22

	if (RAKUTEN_POINT_NEW != 1) {
		$point = "";
		$point_start = "";
		$point_last = "";
	}

	//	キャッチコピー
	//	add ookawara 2013/12/17 start
	//	表示優先順位
	//	1.○以上で送料無料
	//	2.割引率
	//	3.メーカー

	$pcccopy = "";
	$moccopy = "";
	$pc_max_num = 0;
	$mo_max_num = 0;
	//	1.○以上で送料無料
	if (defined("RAKUTEN_ATO_SOURYOU_MESSAGE") && $price1 < RAKUTEN_SOURYOU_PRICE) {
		$pcccopy = RAKUTEN_ATO_SOURYOU_MESSAGE."<br>";
		$moccopy = RAKUTEN_ATO_SOURYOU_MESSAGE." ";
	}
	$pc_max_num = strlen($pcccopy);
	$mo_max_num = strlen($moccopy);
	//	2.割引率
	if ($wariritu) {
		$pc_add_flg = 0;
		$mo_add_flg = 0;
		//$wariritsu_msg = "★".$wariritu."%OFF★";	//	del ookawara 2014/04/30
		$wariritsu_msg_num = strlen($wariritsu_msg);
		$sub_pc_max_num = $pc_max_num + $wariritsu_msg_num;
		$sub_mo_max_num = $mo_max_num + $wariritsu_msg_num;
		if ($sub_pc_max_num <= 174) {
			$pcccopy .= $wariritsu_msg;
			$pc_max_num = $sub_pc_max_num;
			$pc_add_flg = 1;
		}
		if ($sub_mo_max_num <= 60) {
			$moccopy .= $wariritsu_msg;
			$mo_max_num = $sub_mo_max_num;
			$mo_add_flg = 1;
		}

		if (defined(RAKUTEN_DISCOUNT_TIME) && $wariritu <= RAKUTEN_DISCOUNT_PAR) {
			$warikikan = "【".RAKUTEN_DISCOUNT_TIME."】";
			$warikikan_num = strlen($warikikan);
			$sub_pc_max_num = $pc_max_num + $warikikan_num;
			$sub_mo_max_num = $mo_max_num + $warikikan_num;
			if ($sub_pc_max_num <= 174 && $pc_add_flg == 1) {
				$pcccopy .= $warikikan;
				$pc_max_num = $sub_pc_max_num;
			}
			if ($sub_mo_max_num <= 60 && $mo_add_flg == 1) {
				$moccopy .= $warikikan;
				$mo_max_num = $sub_mo_max_num;
			}
		}
		$kaigyou = "<br>";
		$kaigyou_num = strlen($kaigyou);
		$sub_pc_max_num = $pc_max_num + $kaigyou_num;
		if ($sub_pc_max_num <= 174 && $pc_add_flg == 1) {
			$pcccopy .= $kaigyou;
			$pc_max_num = $sub_pc_max_num;
		}

		$kaigyou = " ";
		$kaigyou_num = strlen($kaigyou);
		$sub_mo_max_num = $mo_max_num + $kaigyou_num;
		if ($sub_mo_max_num <= 60 && $mo_add_flg == 1) {
			$moccopy .= $kaigyou;
			$mo_max_num = $sub_mo_max_num;
		}
	}
	if ($L_BRAND[$brand]) {
		$brand_msg = $L_BRAND[$brand]."<br>";
		$brand_msg_num = strlen($brand_msg);
		$sub_pc_max_num = $pc_max_num + $brand_msg_num;
		$sub_mo_max_num = $mo_max_num + $brand_msg_num;
		if ($sub_pc_max_num <= 174) {
			$pcccopy .= $brand_msg;
			$pc_max_num = $sub_pc_max_num;
		}

		$brand_msg = $L_BRAND[$brand];
		$brand_msg_num = strlen($brand_msg);
		$sub_mo_max_num = $mo_max_num + $brand_msg_num;
		if ($sub_mo_max_num <= 60) {
			$moccopy .= $brand_msg;
			$mo_max_num = $sub_mo_max_num;
		}
	}
	//	add ookawara 2013/12/17 end

	//	納期関係設定
	//	add ookawara 2015/01/16
	$zaiko_type = 1;
	$zaiko_num = 0;
	$zaiko_hyouji = 0;
	//$zaiko_nasi_order = 1;	//	del ohkawara 2017/02/22
	$zaiko_nasi_order = 0;		//	add ohkawara 2017/02/22		//	在庫が無い場合は注文を受け付けないようにすべて設定

	//	del ohkawara 2017/04/10
	//$zaiko_ari_nouki_msg = "";
	//$zaiko_nashi_nouki_msg = 1;
	//if ($att_check == 1) {
	//	$zaiko_ari_nouki_msg = "3";		//	add ohkawara	2017/04/05
	//	//$zaiko_ari_nouki_msg = 2;
	//	$zaiko_nashi_nouki_msg = "2";
	//}

	//	add ohkawara 2017/04/10
	$zaiko_ari_nouki_msg = 1;
	$zaiko_nashi_nouki_msg = 2;
	if ($att_check == 1) {
		$zaiko_ari_nouki_msg = "3";
	}


	//	特価商品在庫設定
	//	add ookawara 2014/07/31
	$cyumon_uketsuke = -1;
	//$zaiko_num = "";			//	del ookawara 2015/01/16
	//$zaiko_hyouji = "";		//	del ookawara 2015/01/16
	//	add ookawara 2014/12/29 start
	$cnt = 0;
	$sql  = "SELECT COUNT(*) AS cnt FROM ".T_CATEGORY.
			" WHERE cate1 IN ('23','24')".
			" AND g_num='".$g_num."';";
	if ($result = mysqli_query($conn_id,$sql)) {
		$list = mysqli_fetch_array($result);
		$cnt = $list['cnt'];
	}
	//if ($cnt > 0) {					//	del ookawara 2015/09/17
	if ($cnt > 0 || $zaiko == 1) {		//	add ookawara 2015/09/17
	//	add ookawara 2014/12/29 end
	//if (preg_match("/特価商品/", $cate1_name)) {													//	del ookawara 2014/12/24
	//if (preg_match("/特価商品/", $cate1_name) || preg_match("/限定入荷一点限り/", $cate1_name)) {	//	add ookawara 2014/12/24	/
		$cyumon_uketsuke = 0;
		$zaiko_type = 1;
		$zaiko_num = 1;
		$zaiko_nasi_order = 0;			//	add ookawara 2015/01/16
		if ($att_check != 1) {			//	add ohkawara	2017/04/05
			$zaiko_ari_nouki_msg = 1;	//	add ookawara 2015/01/16
		}								//	add ohkawara	2017/04/05
		$zaiko_nashi_nouki_msg = "";	//	add ookawara 2015/01/19
	//	add ohkawara 2017/02/15	start
	} else {
		$cyumon_uketsuke = -1;
		$zaiko_type = "1";
		$zaiko_num = RAKUTEN_DEFO_ZAIKO;
		$zaiko_nasi_order = "0";
	//	add ohkawara 2017/02/15	end
	}

	//	add ookawara 2015/07/06
	$sp_msg = $mobile_msg;
	if (strlen($pc_msg) < 5120) {
		$sp_msg = $pc_msg;
		$sp_msg = preg_replace("/\<p style=\"width: 500px;\"\>/", "", $sp_msg);
	}

	//	add	ookawara	2016/06/22
	//$code2 = $code;	//	del ohkawara 2017/02/20
	$code2 = "";			//	add ohkawara 2017/02/20
	if ($jan_code > 0) {
		$code2 = $jan_code;
	}
	$code_none = "";
	if ($code2 == 0) {
		//$code2 = 0;			//	add ohkawara 2018/10/24	//	del ohkawara 2018/11/01
		$code_none = 5;
	}

	//	書籍などの時、カタログID（JAN）を削除する
	//	2017/08/07	様子見で一時削除
	//	add ohkawara 2017/08/02
	//if ($cate1 == 92 && $cate2 == 14) {
	//	$code = "";
	//	$code2 = "";
	//	$code_none = 5;
	//}

	if ($concaramu && $concaramu != "d") {

		$r1_csv = "g{$g_num},{$code},{$g_name},{$search_display},{$taxable},,,{$point},{$point_start},{$point_last},1,,1,,{$zaiko_hyouji},0,{$category_id},,{$pcccopy},{$pc_msg},{$sp_msg},,,,{$image_type},{$image_type},{$set_tag_id},{$pcccopy},{$pc_msg},{$moccopy},{$g_name},{$price1},{$price2},{$taxable},0,,,,0,{$souko},1,1,0,1,,0,{$pc_msg},{$sp_msg},,{$img_msg},{$img_alt_msg},{$movie_msg},,{$cyumon_uketsuke},{$zaiko_type},{$zaiko_num},{$zaiko_hyouji},{$size_title},,,,,,{$code2},,{$zaiko_nasi_order},{$zaiko_ari_nouki_msg},{$zaiko_nashi_nouki_msg},,$point,$point_kikan,,,,,,2,,0,0,{$code_none},0,,0,,,,\n";	//	add ohkawara 2020/02/18

		//$r1_csv = "{$concaramu},g{$g_num},{$code},{$category_id},{$set_tag_id},{$pcccopy},{$moccopy},{$g_name},{$price1},{$price2},{$taxable},0,,,,0,{$souko},1,1,0,1,,0,{$pc_msg},{$sp_msg},,{$img_msg},{$img_alt_msg},{$movie_msg},,{$cyumon_uketsuke},{$zaiko_type},{$zaiko_num},{$zaiko_hyouji},{$size_title},,,,,,{$code2},,{$zaiko_nasi_order},{$zaiko_ari_nouki_msg},{$zaiko_nashi_nouki_msg},,$point,$point_kikan,,,,,,2,,0,0,{$code_none},0,,0,,\n";	//	add ohkawara 2020/02/07	//	del ohkawara 2020/02/18

		//$r1_csv = "{$concaramu},g{$g_num},{$code},{$category_id},{$set_tag_id},{$pcccopy},{$moccopy},{$g_name},{$price1},{$price2},{$taxable},0,,,,0,{$souko},1,1,0,1,,0,{$pc_msg},{$sp_msg},,{$img_msg},{$img_alt_msg},{$movie_msg},,{$cyumon_uketsuke},{$zaiko_type},{$zaiko_num},{$zaiko_hyouji},{$size_title},,,,,,{$code2},,{$zaiko_nasi_order},{$zaiko_ari_nouki_msg},{$zaiko_nashi_nouki_msg},,$point,$point_kikan,,,,,,2,,0,0,{$code_none},0,,0\n";	//	add ohkawara 2018/10/22	//	del ohkawara 2020/02/07

		//$r1_csv = "{$concaramu},g{$g_num},{$code},{$category_id},{$set_tag_id},{$pcccopy},{$moccopy},{$g_name},{$price1},{$price2},{$taxable},0,,,,0,{$souko},1,1,0,1,,0,{$pc_msg},{$mobile_msg},{$sp_msg},,{$img_msg},{$img_alt_msg},{$movie_msg},,{$cyumon_uketsuke},{$zaiko_type},{$zaiko_num},{$zaiko_hyouji},{$size_title},,,,,,{$code2},,{$zaiko_nasi_order},{$zaiko_ari_nouki_msg},{$zaiko_nashi_nouki_msg},,$point,$point_kikan,,,,,,2,,0,0,{$code_none},0\n";	//	add ohkawara 2017/02/21	//	del ohkawara 2018/10/22

		//$r1_csv = "{$concaramu},g{$g_num},{$code},{$category_id},{$pcccopy},{$moccopy},{$g_name},{$price1},{$price2},{$taxable},0,,,,0,{$souko},1,1,0,1,1,0,{$pc_msg},{$mobile_msg},{$sp_msg},,{$img_msg},{$img_alt_msg},{$movie_msg},,{$cyumon_uketsuke},{$zaiko_type},{$zaiko_num},{$zaiko_hyouji},{$size_title},,,,,{$code2},,{$zaiko_nasi_order},{$zaiko_ari_nouki_msg},{$zaiko_nashi_nouki_msg},,$point,$point_kikan,,,,,,\n";	//	add ookawara 2016/06/22
		//$r1_csv = "{$concaramu},g{$g_num},{$code},{$category_id},{$pcccopy},{$moccopy},{$g_name},{$price1},{$price2},{$taxable},0,,,,0,{$souko},1,1,0,1,1,0,{$pc_msg},{$mobile_msg},{$sp_msg},,{$img_msg},{$img_alt_msg},{$movie_msg},,{$cyumon_uketsuke},{$zaiko_type},{$zaiko_num},{$zaiko_hyouji},{$size_title},,,,,{$code},,{$zaiko_nasi_order},{$zaiko_ari_nouki_msg},{$zaiko_nashi_nouki_msg},,$point,$point_kikan,,,,,,\n";	//	add ookawara 2015/07/06	//	del ookawara 2016/06/22
		//$r1_csv = "{$concaramu},g{$g_num},{$code},{$category_id},{$pcccopy},{$moccopy},{$g_name},{$price1},{$price2},{$taxable},0,,,,0,{$souko},1,1,0,1,1,0,{$pc_msg},{$mobile_msg},,{$img_msg},{$img_alt_msg},,,-1,{$zaiko_type},,,{$size_title},,,,,{$code},,,,,,$point,$point_kikan,,,,,,\n";
		//$r1_csv = "{$concaramu},g{$g_num},{$code},{$category_id},{$pcccopy},{$moccopy},{$g_name},{$price1},{$price2},{$taxable},0,,,,0,{$souko},1,1,0,1,1,0,{$pc_msg},{$mobile_msg},,{$img_msg},{$img_alt_msg},{$movie_msg},,{$cyumon_uketsuke},{$zaiko_type},{$zaiko_num},{$zaiko_hyouji},{$size_title},,,,,{$code},,,,,,$point,$point_kikan,,,,,,\n";
		//$r1_csv = "{$concaramu},g{$g_num},{$code},{$category_id},{$pcccopy},{$moccopy},{$g_name},{$price1},{$price2},{$taxable},0,,,,0,{$souko},1,1,0,1,1,0,{$pc_msg},{$mobile_msg},,{$img_msg},{$img_alt_msg},{$movie_msg},,{$cyumon_uketsuke},{$zaiko_type},{$zaiko_num},{$zaiko_hyouji},{$size_title},,,,,{$code},,{$zaiko_nasi_order},{$zaiko_ari_nouki_msg},{$zaiko_nashi_nouki_msg},,$point,$point_kikan,,,,,,\n";	//	del ookawara 2015/07/06
	}

	return $r1_csv;

}



//	割引率計算
function wariritu($price,$sale_price) {

	$wariritu = 0;
	if ($price && $sale_price) {
		$wariritu = 100 - round($sale_price/$price*100);
	}
	if ($wariritu < 0) { $wariritu = 0; }

	return $wariritu;
}



//	楽天セレクト
//function rakuten_select_csv($list,$RAKU_S_LIST) {				//	del ookawara 2015/01/15
function rakuten_select_csv($list, &$RAKU_S_LIST, $att_check) {	//	add ookawara 2015/01/15

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
	$price = $list['price'];
	$sale_price = $list['sale_price'];
	$options = $list['options'];
	$caption = $list['caption'];
	$brand = $list['brand'];
	$taxable = TAXABLE;
	$zaiko = $list['zaiko'];			//	add ookawara 2015/09/17
	$size_list = $list['size_list'];	//	add ookawara 2015/09/17
	//$name_head = $list['name_head'];	//	add ookawara 2015/09/17		//	del ookawara 2015/09/24
	//$g_name = $name_head.$g_name;	//	add ookawara 2015/09/17			//	del ookawara 2015/09/24


	//	サイズ	サイズは、登録か、削除のみ　変更は無し
	//	add ookawara 2015/09/17	start
	$L_SIZE = array();
	if ($size_list) {
		$op_size = unserialize($size_list);
	}
	if ($op_size) {
		foreach ($op_size AS $key => $VAL) {
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

	if ($L_SIZE) {
		foreach ($L_SIZE AS $val) {
			$val = trim($val);
			if ($val == "") { continue; }
##			$val = eregi_replace("\[.*\]","",$val);
			$val = preg_replace("/\[.*\]/i","",$val);

			if ($state < 3) {
##				if (ereg("^\*",$val)) { continue; }
				if (preg_match("/^\*/",$val)) { continue; }
##				$val = eregi_replace("^\/","",$val);
				$val = preg_replace("/^\//i","",$val);
			}

			if ($val == "F" || 
				$val == "フリー" || 
				$val == "フリーサイズ" || 
				$val == "--" || 
				$val == "フットサルサイズ" || 
				$val == "設定なし"
			) {
				unset($val);
				continue;
			}
##			$val = ereg_replace(":","：",$val);
			$val = preg_replace("/:/","：",$val);

			$size_name = "サイズ";
			if ($g_num == "7347" || $g_num == "7348" || $g_num == "7349") {
				$size_name = "カラー";
			}

			//	add ookawara 2015/01/15
			$haisou_kikan = 1;
			if ($att_check == 1) {
				$haisou_kikan = 2;
			}
			$haisou_kikan = "";		//	サイズ毎に在庫を設定する場合に利用するため値を利用しないようにする。

			//	すでに登録されているかチェック
			if ($val && !$RAKU_S_LIST[$g_num][$val]) {
				//$select_csv .= "\"n\",\"g{$g_num}\",\"s\",\"{$size_name}\",\"{$val}\",\"\",\"\",\"\",\"\",\"\",\"\"\n";		//	del ookawara 2015/01/15
				//$select_csv .= "\"n\",\"g{$g_num}\",\"s\",\"{$size_name}\",\"{$val}\",\"\",\"\",\"\",\"\",\"\",\"\",\"{$haisou_kikan}\"\n";	//	add ookawara 2015/01/15	//	del ohkawara 2020/02/07
				$select_csv .= "\"n\",\"g{$g_num}\",\"s\",\"{$size_name}\",\"{$val}\",\"\",\"\",\"\",\"\",\"\",\"\",,,,,,,\n";	//	add ohkawara 2020/02/07
			}
			$CHECK_SELECT[$val] = $val;
		}
	}

	return array($select_csv,$CHECK_SELECT);

}



//	楽天カテゴリー
function rakuten_cat_csv($list, $L_CATE, &$RAKU_C_LIST) {
global $attention,$NEWCATE;

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
	$price = $list['price'];
	$sale_price = $list['sale_price'];
	$options = $list['options'];
	$caption = $list['caption'];
	$brand = $list['brand'];
	$taxable = TAXABLE;
	//$name_head = $list['name_head'];	//	add ookawara 2015/09/17	//	del ookawara 2015/09/24
	//$g_name = $name_head.$g_name;	//	add ookawara 2015/09/17	//	del ookawara 2015/09/24


	//	カテゴリー
	$cate1_name = $L_CATE[$cate1][0][0];
	$cate2_name = $L_CATE[$cate1][$cate2][0];
	$cate3_name = $L_CATE[$cate1][$cate2][$cate3];
	$cate0_name = $NEWCATE[$cate1]['name'];

	$category_list = $cate0_name;
	if ($cate0_name != $cate1_name) {
		$category_list .= "\\".$cate1_name;
	}
	if ($cate1_name != $cate2_name) {
		$category_list .= "\\".$cate2_name;
	}
	if ($cate2_name != $cate3_name) {
		$category_list .= "\\".$cate3_name;
	}
	$category_list = trim($category_list);

	if (!$RAKU_C_LIST[$g_num][$category_list]) {
		$cat_csv = "n,g{$g_num},{$g_name},{$category_list},{$yuusen},,\n";
	}

	return array($cat_csv,$category_list);

}



