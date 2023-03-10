<?PHP	//	商品管理	************************************************************************

//	ポスデータアップ

	//	ポスファイルアップフォルダー
	define("UPFILE_DIR","./temp3/");

	//	ポスファイルアップフォルダーパス
	define("UPFILE_PASS","/home/ozzys/ozzys01/www/master/temp2/");

	//	ロックフォルダー
	define("LOCK_DIR","./lock");

	//	仮テーブル
	define("T_KARI","kari2");

	//	商品基本テーブル
	define("T_GOODS","goods");

	//	PDA用商品基本テーブル
	define("T_GOODS2","goods2");

	//	db接続
	include "../../cone.inc";
	define("conn_id",$conn_id);

	//	内税分類コード
	$UCHIZEI_L = array('501'=>'1','503'=>'1','906'=>'1','909'=>'1');

	//	変数配列
	$KEY_L = array(
				'0' => 'pluid',
				'1' => 'goods_name',
				'2' => 'stock',
				'3' => 'x',
				'4' => 'price',
				'5' => 'x',
				'6' => 'x',
				'7' => 'class_l',
				'8' => 'class_m',
				'9' => 'class_s',
				'10' => 'm_price',
				'11' => 'maker_id',
				'12' => 'maker_name'
			);

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

//	set_time_limit(600);

	//	ロック開始
	if (file_exists(LOCK_DIR)) {
		$dir_time = filectime(LOCK_DIR);
		$base_time = time() - 60 * 60;
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
	$sql  = "TRUNCATE TABLE ".T_KARI;
	pg_exec(conn_id,$sql);

	//	仮アップテーブルシーケンシャルクリアー
	$sql  = "select setval('kari2_num_seq',1,false);";
	pg_exec(conn_id,$sql);

	//	PDA用テーブルクリアー
//	$sql  = "TRUNCATE TABLE ".T_GOODS2;
//	pg_exec(conn_id,$sql);

	//	PDA用テーブルシーケンシャルクリアー
//	$sql  = "select setval('goods2_num_seq',1,false);";
//	pg_exec(conn_id,$sql);

	//	ファイル読み込み
	if (file_exists(UPFILE_DIR)) {
		$strDir = opendir(UPFILE_DIR);
		while($strfle = readdir($strDir)) {
##			if (!ereg("^\.",$strfle)) {
			if (!preg_match("/^\./",$strfle)) {
				$file = UPFILE_DIR.$strfle;
				if (file_exists($file)) {
					$ERROR = set_data($file);
					//	ファイル削除
//					if (file_exists($file)) { unlink($file); }

					if ($ERROR) {
						if (file_exists(LOCK_DIR)) { rmdir(LOCK_DIR); }
						return $ERROR;
					}
				}
			}
		}
	}

/*
	//	メインテーブルデータークリアー
	$sql  = "TRUNCATE TABLE ".T_GOODS;
	pg_exec(conn_id,$sql);
	//	メインテーブルシーケンシャルクリアー
	$sql  = "select setval('goods_num_seq',1,false);";
	pg_exec(conn_id,$sql);

	//	データーコピー
	$sql  = "INSERT INTO ".T_GOODS.
			" SELECT * FROM ".T_KARI.";";
	pg_exec(conn_id,$sql);
*/
	//	キャッシュ削除
	del_cash();

	if (file_exists(LOCK_DIR)) { rmdir(LOCK_DIR); }

	return $ERROR;

}


function set_data($file) {
	global $UCHIZEI_L,$KEY_L;

	//	ファイル読み込み
	$LIST = file($file);
	unset($LIST[0]);
	unset($LIST[1]);

	if (!is_array($LIST)) {
		return $ERROR;
	}

	$i = 1;
	unset($sql);
	foreach ($LIST AS $line) {
$aaa = "枻出版社";
echo $aaa."<br>\n";
echo "1<>".$line."<br>\n";
		## $line = mb_convert_encoding($line,"eucJP-win","SJIS-win");
		$line = mb_convert_encoding($line,"UTF-8","SJIS-win");
##		$line = ereg_replace("薯T出版社","枻出版社",$line);
		$line = preg_replace("/薯T出版社/","枻出版社",$line);
echo "2<>".$line."<br>\n";
##		$line = ereg_replace("\"","",$line);
		$line = preg_replace("/\"/","",$line);
##		$line = ereg_replace("\.00","",$line);
		$line = preg_replace("/\.00/","",$line);
		##  $line = mb_convert_kana($line,"KV","EUC-JP");
		$line = mb_convert_kana($line,"KV","UTF-8");
echo "3<>".$line."<br>\n";
		$VALUE = explode(",",$line);
		if (is_array($VALUE)) {
			foreach ($VALUE AS $key => $val) {
				$$KEY_L[$key] = trim($val);
				if ($KEY_L[$key] == "goods_name") {
##					$goods_name = ereg_replace("#","＃",$goods_name);
					$goods_name = preg_replace("/#/","＃",$goods_name);
				} elseif ($KEY_L[$key] == "maker_id") {
					$maker_id = sprintf("%07d",$maker_id);
				}
			}
		}

		$add_check = 0;
##		if (($class_l != 5 && !eregi("(.*):(.*):(.*):(.*):N",$goods_name)
		if (($class_l != 5 && !preg_match("/(.*):(.*):(.*):(.*):N/i",$goods_name)
##			 && !eregi("(.*):(.*):(.*):(.*):H",$goods_name) && !eregi("(.*):(.*):(.*):(.*):K",$goods_name))
			 && !preg_match("/(.*):(.*):(.*):(.*):H/i",$goods_name) && !preg_match("/(.*):(.*):(.*):(.*):K/i",$goods_name))
			 || strlen($pluid) < 12) {
			$add_check = 1;
//			continue;
		}

		if ($UCHIZEI_L[$class_m] == 1) {	//	実際は $UCHIZEI_L[$class_m] == 1
			$price = floor($price / 1.05);
			$m_price = floor($m_price / 1.05);
		}

##		$goods_names = split(":","$goods_name");
		$goods_names = explode(":","$goods_name");
		$maker = $goods_names[0];
		$goods = $goods_names[1];
		$color = $goods_names[2];
		$size = $goods_names[3];
if ($aaa == $maker) {
echo "true<>$aaa<>$maker<br>\n";
} else {
echo "false<>$aaa<>$maker<br>\n";
}
		if (!$maker_id) {
			$strlen = strlen($pluid);
			if ($strlen == 12) {
				$str = "0" . "$pluid";
			} else {
				$str = "$pluid";
			}
			$maker_id = substr($str,0,7) * 1;
		}
		if (!$maker_id) { $maker_id = "0"; }

		if (!$maker_name && $maker && $goods) {
			$maker_name = $maker;
		}
		if ($maker && !$goods && $maker_name) {
			$maker = $maker_name;
		}

		$sql2 .= "INSERT INTO ".T_GOODS2.
				" (pluid, goods_name, stock, price, class_l, class_m, class_s, maker_id, maker_name,".
				" m_price, maker, goods, color, size)".
				" VALUES ('$pluid', '$goods_name', '$stock', '$price', '$class_l',".
				" '$class_m', '$class_s', '$maker_id', '$maker_name',".
				" '$m_price', '$maker', '$goods', '$color', '$size');";
		$i++;

		if ($add_check == 0) {
			$sql .= "INSERT INTO ".T_KARI.
					" (pluid, goods_name, stock, price, class_l, class_m, class_s, maker_id, maker_name,".
					" m_price, maker, goods, color, size)".
					" VALUES ('$pluid', '$goods_name', '$stock', '$price', '$class_l',".
					" '$class_m', '$class_s', '$maker_id', '$maker_name',".
					" '$m_price', '$maker', '$goods', '$color', '$size');";
		}

		if ($i >= 50) {
/*
			if ($sql2) {
				if (!pg_exec(conn_id,$sql2)) {
					$ERROR[] = "INSERT GOODS2 ERROR";
					break;
				}
			}
*/
			if ($sql) {
				if (!pg_exec(conn_id,$sql)) {
					$ERROR[] = "INSERT GOODS ERROR";
					break;
				}
			}
			$i = 1;
			unset($sql);
			unset($sql2);
		}
	}

	if ($sql) {
		if (!pg_exec(conn_id,$sql)) { $ERROR[] = "LAST INSERT GOODS ERROR"; }
	}

/*
	if ($sql2) {
		if (!pg_exec(conn_id,$sql2)) { $ERROR[] = "LAST INSERT GOODS2 ERROR"; }
	}
*/

	return $ERROR;

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
?>