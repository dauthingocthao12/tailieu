<?PHP
// ------------------------------------- //
//
//	商品管理
//	NEWポスデーターアップ
//	2009/03/05
//	AZET OOKAWARA
//
//	税込み商品の金額が、税率変わったときに変更処理を加える必要有り。
//
// ------------------------------------- //

//	基本設定

	//	PHP設定一時変更	//add ookawara 2013/01/24
	ini_set('memory_limit', '512M');	//	update ookawara 2015/11/20
//	ini_set('post_max_size', '32M');
//	ini_set('upload_max_filesize', '32M');

	//	テスト設定
	define("SET_TEST","");

	//	ポスファイルアップフォルダー
	define("UPFILE_DIR","./temp/");
	define("UPFILE_BKP_DIR", "./temp_bkp/");

	//	ロックフォルダー
	define("LOCK_DIR","./".SET_TEST."lock");

	//	仮テーブル
	define("TABLE_KARI",SET_TEST."kari");

	//	商品基本テーブル
	define("TABLE_GOODS",SET_TEST."goods");

	//	PDA用商品基本テーブル
	define("TABLE_PDA_SEARCH",SET_TEST."pda_search");

	//	メーカーテーブル
	define("TABLE_MAKER",SET_TEST."maker");

	//	商品詳細テーブル
	define("TABLE_LIST","list");

	//	db接続
	include "../../cone.inc";
	define("conn_id",$conn_id);

	//	内税分類コード
//	$UCHIZEI_L = array('501'=>'1','503'=>'1','906'=>'1','909'=>'1');	//	del ookawara 2009/04/30
	$UCHIZEI_L = array('906'=>'1','909'=>'1');							//	add ookawara 2009/04/30

	//	変数配列
	$KEY_L = array(
				'0' => 'pluid',			//	PLUid
				'1' => 'goods_name',	//	商品名称
				'2' => 'stock',			//	帳簿在庫数
				'3' => 'ji_kikan',		//	期間実績		//	change ookawara 2010/11/04
				'4' => 'ji_zenki',		//	前期間実績		//	change ookawara 2010/11/04
				'5' => 'ji_zenzen',		//	前々期間実績	//	change ookawara 2010/11/04
				'6' => 'ru_touki',		//	当期実績_累計	//	change ookawara 2010/11/04
				'7' => 'ru_zenki',		//	前期実績_累計	//	change ookawara 2010/11/04
				'8' => 'ru_zenzen',		//	前々期実績_累計	//	change ookawara 2010/11/04
				'9' => 'class_m',		//	中分類コード
				'10' => 'm_price',		//	定価単価
				'11' => 'price',		//	売価単価
				'12' => 'gen_cyoku',	//	原価単価(直)	//	change ookawara 2010/11/04
				'13' => 'gen_hei'		//	原価単価(平)	//	change ookawara 2010/11/04
			);

// add simon 2018-07-23
require_once('../sub/souryou.inc');

function goods_1() {

	if ($_POST['mode'] == "update") {
		$ERROR = update();
	}

	$html = default_html($ERROR);

	echo $html;

}


function default_html($ERROR) {

	$html .= "<br>\n";

	if ($ERROR) {
		$html .= "<font color=\"#ff0000\">エラー</font><br>\n";
		foreach ($ERROR AS $val) {
			$html .= $val."<br>\n";
		}
		$html .= "<br>\n";
	} elseif ($_POST['mode']) {
		$html .= "更新完了致しました。";
		$html .= "<br>\n";
	}

	//	アップファイルチェック
	$flag = 0;
	$file_name_list = "";
	if (file_exists(UPFILE_DIR)) {
		$strDir = opendir(UPFILE_DIR);
		while($strfle = readdir($strDir)) {
##			if (!ereg("^\.",$strfle)) {
			if (!preg_match("/^\./",$strfle)) {
				$flag = 1;
				$file_name_list .= "・".$strfle."<br>\n";
			}
		}
	}

	if ($flag == 1) {
		$html .= <<<EOT
	<FORM action='{$_SERVER['PHP_SELF']}' method='POST'>
	<INPUT type='hidden' name='main' value='{$_POST['main']}'>
	<INPUT type='hidden' name='goods' value='{$_POST['goods']}'>
	<INPUT type='hidden' name='mode' value='update'>
	<TABLE border="0" cellpadding="3" cellspacing="2" bgcolor="#999999">
	  <TBODY>
	    <TR bgcolor="#cccccc">
	      <TD>以下のファイルをアップロードします。<br>
	      よろしければ実行ボタンを押してください。
	      </TD>
	    </TR>
	    <TR bgcolor="#ffffff">
	      <TD>$file_name_list</TD>
	    </TR>
	    <TR bgcolor="#ffffff">
	      <TD><INPUT type='submit' value='実行'></TD>
	    </TR>
	  </TBODY>
	</TABLE>
	</FORM>

EOT;
	} else {
		$html .= "更新するファイルがアップロードされておりません。<br>\n";
	}

	return $html;

}


function update() {

	set_time_limit(600);

	//	ロック開始
	if (file_exists(LOCK_DIR)) {
		$dir_time = filectime(LOCK_DIR);
		$base_time = time() - (60 * 60);
		if ($base_time < $dir_time) {
			$ERROR[] = "既に実行済みです。しばらくお待ちください。";
			return $ERROR;
		} else {
			rmdir(LOCK_DIR);
		}
	}

	//	ロック
	mkdir(LOCK_DIR,0777);
	chmod(LOCK_DIR,0777);

	//	仮アップテーブルクリアー
	$sql  = "TRUNCATE TABLE ".TABLE_KARI;
	pg_exec(conn_id,$sql);

	//	仮アップテーブルシーケンシャルクリアー
	$seek_name = SET_TEST."kari_num_seq";
	$sql  = "select setval('$seek_name',1,false);";
	pg_exec(conn_id,$sql);

	//	PDA用テーブルクリアー
	$sql  = "TRUNCATE TABLE ".TABLE_PDA_SEARCH;
	pg_exec(conn_id,$sql);

	//	ファイル読み込み
	if (file_exists(UPFILE_DIR)) {
		$strDir = opendir(UPFILE_DIR);
		while($strfle = readdir($strDir)) {
##			if (!ereg("^\.",$strfle)) {
			if (!preg_match("/^\./",$strfle)) {
				$file = UPFILE_DIR.$strfle;
				if (file_exists($file)) {
					//$ERROR = set_data($file);			//	del ookawara 2016/01/12
					$SET_ERR = set_data($file);			//	add ookawara 2016/01/12
					$ERROR = $SET_ERR['err'];			//	add ookawara 2016/01/12
					$ERROR_PLUID = $SET_ERR['pluid'];	//	add ookawara 2016/01/12
					//	ファイル削除
					if (file_exists($file)) {
                        // unlink($file);                                    // del simon 2018-12-10
                        // >>> add simon 2018-12-10
                        chmod(UPFILE_DIR.$strfle, 0777);
                        $rename_ok = @rename(UPFILE_DIR.$strfle, UPFILE_BKP_DIR.$strfle);
                        if(!$rename_ok) {
                            $ERROR[] = "CSVファイルを移動に失敗が有りました。";
                        }
                        // <<<
                    }

					if ($ERROR) {
						if (file_exists(LOCK_DIR)) { rmdir(LOCK_DIR); }
						return $ERROR;
					}
				}
			}
		}
	}

	//	メインテーブルデータークリアー
	$sql  = "TRUNCATE TABLE ".TABLE_GOODS;
	pg_exec(conn_id,$sql);
	//	メインテーブルシーケンシャルクリアー
	$seek_name = SET_TEST."goods_num_seq";
	$sql  = "select setval('$seek_name',1,false);";
	pg_exec(conn_id,$sql);

	//	データーコピー
	$sql  = "INSERT INTO ".TABLE_GOODS.
			" SELECT * FROM ".TABLE_KARI.";";
	pg_exec(conn_id,$sql);

	//	キャッシュ削除
	if (SET_TEST == "") { del_cash(); }

	if (file_exists(LOCK_DIR)) { rmdir(LOCK_DIR); }

	//	add ookawara 2016/01/12
	if (count($ERROR_PLUID) > 0) {
		$ERROR[] = "エラーデーターリスト goods_name err:商品名情報エラー、Pluid err:Pluidエラーとなります。";
		foreach ($ERROR_PLUID AS $pluid) {
			$ERROR[] = $pluid;
		}
	}

	return $ERROR;

}


function set_data($file) {
	global $UCHIZEI_L,$KEY_L;

/*
	//	del ookawara 2015/10/22
	//	ファイル読み込み
	$LIST = file($file);

	if (!is_array($LIST)) {
		return $ERROR;
	}

	//	文字コード変換
	mb_convert_variables(mb_internal_encoding(),'SJIS',$LIST);

	$i = 1;
	unset($sql);
	$MAKER_LIST = array();
	foreach ($LIST AS $line) {
*/

	//	add ookawara 2015/10/22 start
	$i = 1;
	$MAKER_LIST = array();
	$INSERT = array();	//	add ookawara 2015/11/20
	$fp = fopen($file, "r");
	$ERROR_PLUID = array();	//	add ookawara 2016/01/12
	while ($line = fgets($fp)) {
	##	$line = mb_convert_encoding($line, "eucJP-win", "SJIS-win");
	$line = mb_convert_encoding($line, "UTF-8", "SJIS-win");
	//	add ookawara 2015/10/22 end

		$pluid = 0;
		$goods_name = "";
		$stock = 0;
		$price = 0;
		$class_l = 0;
		$class_m = 0;
		$class_s = 0;
		$maker_id = "";
		$maker_name = "";
		$m_price = 0;
		$maker = "";
		$goods = "";
		$color = "";
		$size = "";

##		$line = ereg_replace("\"","",$line);
		$line = preg_replace("/\"/","",$line);
##		$line = ereg_replace("\.00","",$line);
		$line = preg_replace("/\.00/","",$line);
		//$line = mb_convert_kana($line,"KV","EUC-JP");		//	del ookawara 2015/10/22
		## $line = mb_convert_kana($line,"KV","eucJP-win");	//	add ookawara 2015/10/22
		$line = mb_convert_kana($line,"KV","UTF-8");
		$VALUE = explode(",",$line);
		if (is_array($VALUE)) {
			foreach ($VALUE AS $key => $val) {
				$val = trim($val);
				if ($val != "") {
					$$KEY_L[$key] = $val;
					if ($KEY_L[$key] == "goods_name") {
##						$goods_name = ereg_replace("#","＃",$goods_name);
						$goods_name = preg_replace("/#/","＃",$goods_name);
					}
				}
			}
		}

		if ($pluid == "PLUid") { continue; }

		$add_check = 0;
		//	del ookawara 2016/01/12
		//	add ookawara 2011/11/30
		//if (!eregi("(.*):(.*):(.*):(.*):[A-Y]",$goods_name) || strlen($pluid) < 12) {
		//	$add_check = 1;
		//}

		//	add ookawara 2016/01/12
##		if (!eregi("(.*):(.*):(.*):(.*):[A-Z]",$goods_name)) { // edit simon 2019-01-08 [A-Y] => [A-Z] (新しい竿の送料フラグの為：X,Y,Z)
		if (!preg_match("/(.*):(.*):(.*):(.*):[A-Z]/i",$goods_name)) { // edit simon 2019-01-08 [A-Y] => [A-Z] (新しい竿の送料フラグの為：X,Y,Z)
			$add_check = 1;
			$ERROR_PLUID[] = "goods_name err ".$pluid;	//	add ookawara 2016/01/12
		}
		//if (strlen($pluid) < 12 || strlen($pluid) > 13) {									//	del ohkawara 2018/09/01
		if (strlen($pluid) < 12 || strlen($pluid) > 13 || preg_match("/[^0-9]/", $pluid)) {	//	add ohkawara 2018/09/01
			$add_check = 1;
			$ERROR_PLUID[] = "Pluid err ".$pluid;	//	add ookawara 2016/01/12
		}

		//	del ookawara 2011/11/30
		//if (($class_l != 5 && !eregi("(.*):(.*):(.*):(.*):N",$goods_name)
		//	 && !eregi("(.*):(.*):(.*):(.*):H",$goods_name) && !eregi("(.*):(.*):(.*):(.*):K",$goods_name))
		//	 || strlen($pluid) < 12) {
		//	$add_check = 1;
		//}

		if ($UCHIZEI_L[$class_m] == 1 && $price != "") {	//	実際は $UCHIZEI_L[$class_m] == 1
			$price = floor($price / 1.05);
			$m_price = floor($m_price / 1.05);
		}

		$goods_name = addslashes($goods_name);
##		$goods_names = split(":","$goods_name");
		$goods_names = explode(":","$goods_name");
		$set_flag = "";	//	add ookawara 2009/09/28
		if ($goods_names[0]) { $maker    = $goods_names[0]; }
		if ($goods_names[1]) { $goods    = $goods_names[1]; }
		if ($goods_names[2]) { $color    = $goods_names[2]; }
		if ($goods_names[3]) { $size     = $goods_names[3]; }
		if ($goods_names[4]) { $set_flag = $goods_names[4]; }	//	add ookawara 2009/09/28

		if (!$maker_name && $maker && $goods) {
			$maker_name = $maker;
		}
		if ($maker && !$goods && $maker_name) {
			$maker = $maker_name;
		}
##		if (ereg("出版社",$maker) && strlen($maker) == 7) {
		if (preg_match("/出版社/",$maker) && strlen($maker) == 7) {
			$maker = "枻出版社";
		}
##		if (ereg("出版社",$maker_name) && strlen($maker_name) == 7) {
		if (preg_match("/出版社/",$maker_name) && strlen($maker_name) == 7) {
			$maker_name = "枻出版社";
		}

		//	add ookawara 2015/12/23
		//	class_mに正しくない値がある為のチェック
		//if (preg_match("/[^0-9]/", $class_m)) {							//	del ohkawara 2018/09/01
		if (preg_match("/[^0-9]/", $class_m) || strlen($class_m) > 3) {		//	add ohkawara 2018/09/01
			$add_check = 1;
			$ERROR_PLUID[] = "class_m err ".$pluid;	//	add ookawara 2016/01/12
//			continue;														//	add ohkawara 2018/09/01
		}

		//	integerの超える値が入っていた場合のエラーチェック追加
		if ( intval($ji_kikan) > 2147483647 || intval($ji_zenki) > 2147483647 || intval($ji_zenzen) > 2147483647
			 || intval($ru_touki) > 2147483647 || intval($ru_zenki) > 2147483647 || intval($ru_zenki) > 2147483647
			 || intval($ru_zenzen) > 2147483647 || intval($gen_cyoku) > 2147483647 || intval($gen_hei) > 2147483647 ) {
			$add_check = 1;
			$ERROR_PLUID[] = "integer err ".$pluid;
		}

		if ($class_m) {
			$class_l = substr($class_m,0,1);
			$class_s = $class_m."00";
		}

		$price = floor($price);
		$m_price = floor($m_price);

		if (trim($maker) && !trim($goods)) {
			$goods = $maker;
			$maker = "No Maker";
		}
		$TB_GOODS[$pluid]['pluid'] = $pluid;
		$TB_GOODS[$pluid]['stock'] = $stock;
		$TB_GOODS[$pluid]['price'] = $price;
		$TB_GOODS[$pluid]['class_m'] = $class_m;
		$TB_GOODS[$pluid]['maker'] = addslashes(htmlspecialchars($maker));
		$TB_GOODS[$pluid]['goods'] = addslashes(htmlspecialchars($goods));
		$TB_GOODS[$pluid]['color'] = addslashes(htmlspecialchars($color));
		$TB_GOODS[$pluid]['size'] = addslashes(htmlspecialchars($size));
		$TB_GOODS[$pluid]['set_flag'] = trim($set_flag);	//	add ookawara 2009/09/28
		//	add ookawara 2010/11/04
		$TB_GOODS[$pluid]['ji_kikan'] = trim($ji_kikan);
		$TB_GOODS[$pluid]['ji_zenki'] = trim($ji_zenki);
		$TB_GOODS[$pluid]['ji_zenzen'] = trim($ji_zenzen);
		$TB_GOODS[$pluid]['ru_touki'] = trim($ru_touki);
		$TB_GOODS[$pluid]['ru_zenki'] = trim($ru_zenki);
		$TB_GOODS[$pluid]['ru_zenzen'] = trim($ru_zenzen);
		$TB_GOODS[$pluid]['gen_cyoku'] = trim($gen_cyoku);
		$TB_GOODS[$pluid]['gen_hei'] = trim($gen_hei);

		//	add ohkawara 2018/09/01	//	PDA用のデーターに不具合が有った場合登録しないように削除
		if ($add_check == 1) {
			unset($TB_GOODS[$pluid]);
		}

		if ($add_check < 1) {
			// add simon 2018-07-03 
			// 特別のフラグを使う
			$set_flag_insert = null;
			if(in_array($set_flag, array_keys(Souryou::$ROD_SOURYOU_L))) {
				$set_flag_insert = $set_flag;
			}

			//	add ookawara 2015/11/20
			$INSERT[] = " ('$pluid', '$goods_name', '$stock', '$price', '$class_l',".
						" '$class_m', '$class_s', '$maker_id', '$maker_name',".
						" '$m_price', '$maker', '$goods', '$color', '$size', '$set_flag_insert')";

			$MAKER_LIST[$maker] = 1;
			$i++;
		}

		//if ($i >= 50) {	//	del ookawara 2015/10/22
		if ($i >= 40) {		//	add ookawara 2015/10/22
			//if ($sql) {	//	del ookawara 2015/11/20

			//	add ookawara 2015/11/20 start
			if ($INSERT) {
				$sql = importKariSql($INSERT);
				if (!pg_exec(conn_id,$sql)) {

					$ERROR[] = "INSERT GOODS ERROR";
//$ERROR[] = $sql;
					break;
				}
			}
			$i = 1;
			$INSERT = array();	//	add ookawara 2015/11/20
			unset($sql);
		}
	}
	fclose($fp);	//	add ookawara 2015/10/22
	//unset($LIST);	//	add ookawara 2015/03/30	一時メモリー不足のため削除	//	del ookawara 2015/10/22

	//if ($sql) {	//	del ookawara 2015/11/20
	//	add ookawara 2015/11/20 start
	if ($INSERT) {
		$sql = importKariSql($INSERT);
		if (!pg_exec(conn_id,$sql)) {
			$ERROR[] = "LAST INSERT GOODS ERROR";
//$ERROR[] = $sql;
		}
		$INSERT = array();	//	add ookawara 2015/11/20
	}

	//	メーカー登録
	$sql  = "SELECT maker_num, maker, maker_name FROM ".TABLE_MAKER.";";
	if ($result = pg_exec(conn_id,$sql)) {
		while ($list=pg_fetch_array($result, NULL, PGSQL_ASSOC)) {
			$maker_name = $list['maker'];
			$TB_MAKER[$maker_name]['maker_num'] = $list['maker_num'];
			$TB_MAKER[$maker_name]['maker_name'] = $list['maker_name'];
			if ($MAKER_LIST[$maker_name]) { unset($MAKER_LIST[$maker_name]); }
		}
	}
	$max_maker_num = 1;
	$sql  = "SELECT MAX(maker_num) AS max FROM ".TABLE_LIST.";";
	if ($result = pg_exec(conn_id,$sql)) {
		$list = pg_fetch_array($result, NULL, PGSQL_ASSOC);
		$max_maker_num = $list['max'] + 1;
	}
	if (count($MAKER_LIST)) {
		$sql = "";
		foreach ($MAKER_LIST AS $key => $val) {
			$sql .= "INSERT INTO ".TABLE_MAKER.
					" (maker_num, maker_name, maker_url, maker)".
					" VALUES ('$max_maker_num','$key','','$key');";
			$TB_MAKER[$key]['maker_num'] = $max_maker_num;
			$TB_MAKER[$key]['maker_name'] = $key;
			$max_maker_num++;
		}
		if ($sql) {
			if (!pg_exec(conn_id,$sql)) { $ERROR[] = "LAST INSERT MAKER ERROR"; }
		}
	}

	//	PDA用テーブル登録
	if ($TB_GOODS) {
		//	メーカーテーブル情報
		//	$TB_MAKER[$maker'] メーカー名

		//	listテーブル取得
		$sql  = "SELECT list_num, pluid, goods_name, size, color FROM ".TABLE_LIST.
				" WHERE state='0';";
		if ($result = pg_exec(conn_id,$sql)) {
			while ($list=pg_fetch_array($result, NULL, PGSQL_ASSOC)) {
				$list_num = $list['list_num'];
				$pluid = $list['pluid'];
				$goods_name = $list['goods_name'];
				$size = $list['size'];
				$color = $list['color'];

				$TB_LIST[$pluid]['list_num'] = $list_num;
				$TB_LIST[$pluid]['l_goods_name'] = addslashes(htmlspecialchars($goods_name));
				$TB_LIST[$pluid]['l_size'] = addslashes(htmlspecialchars($size));
				$TB_LIST[$pluid]['l_color'] = addslashes(htmlspecialchars($color));
			}
		}

		//	登録データー作成
		$i = 1;
		$sql = "";
		$max_maker_num += 1;
		$INSERT = array();	//	add ookawara 2015/11/20
		foreach ($TB_GOODS AS $pluid => $val) {

			//	add ookawara 2015/01/12
			if (strlen($pluid) > 13) {
				continue;
			}

			$maker = $TB_GOODS[$pluid]['maker'];
			if ($TB_LIST[$pluid]['list_num'] < 1) {
				$TB_LIST[$pluid]['list_num'] = "0";
			}
			if (!$TB_GOODS[$pluid]['goods']) {
				$TB_GOODS[$pluid]['goods'] = "No Goods Name";
			}
			if ($TB_LIST[$pluid]['l_goods_name']) {
				$l_goods_name = $TB_LIST[$pluid]['l_goods_name'];
			} else {
				$l_goods_name = $TB_GOODS[$pluid]['goods'];
			}
			if ($TB_MAKER[$maker]['maker_num'] < 1) {
				$TB_MAKER[$maker]['maker_num'] = $max_maker_num;
				$TB_MAKER[$maker]['maker_name'] = $maker."&nbsp;(NoNet)";
				$max_maker_num++;
			}

			//	add ookawara 2015/12/23
			//	class_mに正しくない値がある為のチェック
			if (preg_match("/[^0-9]/", $TB_GOODS[$pluid]['class_m'])) {
				continue;
			}

			//$sql .= "INSERT INTO ".TABLE_PDA_SEARCH.
			//		" VALUES(".

			$INSERT[] = "(".	//	add ookawara 2015/11/20
						" '".$pluid."'".
						",'".$TB_GOODS[$pluid]['stock']."'".
						",'".$TB_GOODS[$pluid]['price']."'".
						",'".$TB_GOODS[$pluid]['class_m']."'".
						",'".$TB_GOODS[$pluid]['maker']."'".
						",'".$TB_GOODS[$pluid]['goods']."'".
						",'".$TB_GOODS[$pluid]['color']."'".
						",'".$TB_GOODS[$pluid]['size']."'".
						",'".$TB_LIST[$pluid]['list_num']."'".
						",'".$l_goods_name."'".
						",'".$TB_LIST[$pluid]['l_size']."'".
						",'".$TB_LIST[$pluid]['l_color']."'".
						",'".$TB_MAKER[$maker]['maker_num']."'".
						",'".$TB_MAKER[$maker]['maker_name']."'".
						",'".$pluid.
							" ".$TB_GOODS[$pluid]['maker'].
							" ".$TB_GOODS[$pluid]['goods'].
							" ".$TB_GOODS[$pluid]['color'].
							" ".$TB_GOODS[$pluid]['size'].
							" ".$TB_LIST[$pluid]['l_goods_name'].
							" ".$TB_LIST[$pluid]['l_size'].
							" ".$TB_LIST[$pluid]['l_color'].
							" ".$TB_MAKER[$maker]['maker_name'].
						"'".
						",'".$TB_GOODS[$pluid]['set_flag']."'".	//	add ookawara 2009/09/28
						//	add ookawara 2010/11/04
						",'".$TB_GOODS[$pluid]['ji_kikan']."'".
						",'".$TB_GOODS[$pluid]['ji_zenki']."'".
						",'".$TB_GOODS[$pluid]['ji_zenzen']."'".
						",'".$TB_GOODS[$pluid]['ru_touki']."'".
						",'".$TB_GOODS[$pluid]['ru_zenki']."'".
						",'".$TB_GOODS[$pluid]['ru_zenzen']."'".
						",'".$TB_GOODS[$pluid]['gen_cyoku']."'".
						",'".$TB_GOODS[$pluid]['gen_hei']."'".
						")";	//	add ookawara 2015/11/20
						//");";	//	del ookawara 2015/11/20
			$i++;
			if ($i >= 10) {
				//	インサート
				//	add ookawara 2015/11/20 start
				if ($INSERT) {
					$sql_list = "";
					foreach ($INSERT AS $key => $val) {
						if ($sql_list) { $sql_list .= ","; }
						$sql_list .= $val;
					}

					$sql  = "INSERT INTO ".TABLE_PDA_SEARCH.
							" VALUES".$sql_list.";";
				//	add ookawara 2015/11/20 end
					if (!pg_exec(conn_id,$sql)) {
						$ERROR[] = "INSERT PDA_SEARCH ERROR 1";
                         $ERROR[] = $sql;
						break;
					}
				}	//	add ookawara 2015/11/20
				$sql = "";
				$i = 1;
				$INSERT = array();	//	add ookawara 2015/11/20
			}
		}

		//	add ookawara 2015/11/20
		if ($INSERT) {
			$sql_list = "";
			foreach ($INSERT AS $key => $val) {
				if ($sql_list) { $sql_list .= ","; }
				$sql_list .= $val;
			}
			$sql  = "INSERT INTO ".TABLE_PDA_SEARCH.
					" VALUES".$sql_list.";";

			if (!pg_exec(conn_id,$sql)) {
					$ERROR[] = "INSERT PDA_SEARCH ERROR 2";
                     $ERROR[] = $sql;
			}
		}
	}

	$SET_ERR['err'] = $ERROR;
	$SET_ERR['pluid'] = $ERROR_PLUID;
	return $SET_ERR;	//	add ookawara 2016/01/12
	//return $ERROR;					//	del ookawara 2016/01/12

}


//	キャッシュデーター削除
function del_cash() {

	$ST_DIR = "../log";
	$strDir = opendir($ST_DIR);
	while($ID1 = readdir($strDir)) {
##		if (eregi("\.htm",$ID1)) {
		if (preg_match("/\.htm/i",$ID1)) {
			$file = "$ST_DIR/$ID1";
			unlink($file);
		}
	}

}


// add simon 2018-07-03 >>>
/**
 * kariテーブルのデータの登録のクエリの準備
 *
 * @author Azet
 * @param array $INSERT_
 * @return string
 */
function importKariSql($INSERT_) {
	$sql_list = join(",", $INSERT_);

	$sql  = "INSERT INTO ".TABLE_KARI.
		" (pluid, goods_name, stock, price, class_l, class_m, class_s, maker_id, maker_name,".
		" m_price, maker, goods, color, size, set_flag)".
		" VALUES".$sql_list.";";
	//	add ookawara 2015/11/20 end

	return $sql;
}
// <<<

