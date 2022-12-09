<?PHP
//	モールディレクトリーデーター更新
ini_set("memory_limit", "2048M");

	//	基本設定
	define("FILE_DIR","./mool_dir_file");			//	モールディレクトリー保存フォルダー
	define("Y_DIR_FILE","yd.csv");					//	Yahooディレクトリーファイル
	define("Y_TIME_FILE","yd_time.txt");			//	Yahooディレクトリー更新ファイル

	define("R_DIR_FILE","rd.csv");					//	楽天ディレクトリーファイル
	define("R_TIME_FILE","rd_time.txt");			//	楽天ディレクトリー更新ファイル


	//	add ohkawara 2017/02/21
	define("R_TAG_FILE","rt.csv");					//	楽天タグファイル
	define("R_TAG_TIME_FILE","rt_time.txt");		//	楽天タグ更新ファイル


	define("A1_DIR_FILE","a1d.csv");				//	Amazonシューズディレクトリーファイル
	define("A1_TIME_FILE","a1d_time.txt");			//	Amazonシューズディレクトリー更新ファイル

	define("A2_DIR_FILE","a2d.csv");				//	Amazonスポーツディレクトリーファイル
	define("A2_TIME_FILE","a2d_time.txt");			//	Amazonスポーツディレクトリー更新ファイル

	define("A3_DIR_FILE","a3d.csv");				//	Amazonファッションディレクトリーファイル
	define("A3_TIME_FILE","a3d_time.txt");			//	Amazonファッションディレクトリー更新ファイル

	//	add ohkawara 2017/02/07
	define("W_DIR_FILE","wd.csv");					//	Wowmaディレクトリーファイル
	define("W_TIME_FILE","wd_time.txt");			//	Wowmaディレクトリー更新ファイル


	//ini_set();	//	add ohkawara 2017/04/06 //パラメター不足　2022/11/16

	//	db table name
	define("T_MOOL_YD","mool_yd");	//	Yahooディレクトリー
	define("T_MOOL_RD","mool_rd");	//	楽天ディレクトリー
	define("T_MOOL_RT","mool_rt");	//	楽天タグ			//	add ohkawara 2017/02/21
	define("T_MOOL_AD","mool_ad");	//	Amazonディレクトリー
	define("T_MOOL_WD","mool_wd");	//	Wowmaディレクトリー	//	add ohkawara 2017/02/07

function goods_mool_dir() {

	if ($_POST['action'] == "rakuten") {
		update_rd($ERROR,$MSG);
	} elseif ($_POST['action'] == "rakuten_tag") {	//	add ohkawara 2017/02/21
		update_rt($ERROR,$MSG);						//	add ohkawara 2017/02/21
	} elseif ($_POST['action'] == "yahoo") {
		update_yd($ERROR,$MSG);
	} elseif ($_POST['action'] == "amazon1") {
		update_ad($ERROR,$MSG,1);
	} elseif ($_POST['action'] == "amazon2") {
		update_ad($ERROR,$MSG,2);
	} elseif ($_POST['action'] == "amazon3") {
		update_ad($ERROR,$MSG,3);
	} elseif ($_POST['action'] == "wowma") {	//	add ohkawara 2017/02/07
		update_wd($ERROR,$MSG);					//	add ohkawara 2017/02/07
	}

	$html = defaults($ERROR,$MSG);

	echo $html;

}



//	初期表示
function defaults($ERROR,$MSG) {
	// global $PHP_SELF,$main,$goods;
	// global $conn_id;	//	add ohkawara 2017/11/22
	global $PHP_SELF,$conn_id;
    $main=$_POST['main'];
	$goods=$_POST['goods'];
	
	$html = "";

	//	エラーメッセージ
	if ($ERROR) {
		$html .= ERROR($ERROR);
	}

	//	更新完了メッセージ
	if ($MSG) {
		$html .= "<b>".$MSG."</b><br />\n<br />\n";
	}

	//	楽天ディレクトリー用
	$rakuten_time = "---------";
	$file_name = FILE_DIR."/".R_TIME_FILE;
	if (file_exists($file_name)) {
		$rt = filemtime(FILE_DIR."/".R_TIME_FILE);
		$rakuten_time = date("Y/m/d H:i:s",$rt);
	}
	$html .= "<form action=\"".$PHP_SELF."\" method=\"POST\" enctype=\"multipart/form-data\">\n";
	$html .= "<input type=\"hidden\" name=\"main\" value=\"".$main."\" />\n";
	$html .= "<input type=\"hidden\" name=\"goods\" value=\"".$goods."\" />\n";
	$html .= "<input type=\"hidden\" name=\"action\" value=\"rakuten\" />\n";
	$html .= "楽天ディレクトリーファイル 最終更新日：".$rakuten_time."<br />\n";
	$html .= "<input type=\"file\" size=\"60\" name=\"rakuten\" />\n";
	$html .= "<input type=\"submit\" value=\"登録\" />\n";
	$html .= "</form>\n";
	$html .= "<br />\n";
	$html .= "<br />\n";

	//	楽天タグ用
	$rakuten_tag_time = "---------";
	$file_name = FILE_DIR."/".R_TAG_TIME_FILE;
	if (file_exists($file_name)) {
		$rtt = filemtime(FILE_DIR."/".R_TAG_TIME_FILE);
		$rakuten_tag_time = date("Y/m/d H:i:s",$rtt);
	}
	$html .= "<form action=\"".$PHP_SELF."\" method=\"POST\" enctype=\"multipart/form-data\">\n";
	$html .= "<input type=\"hidden\" name=\"main\" value=\"".$main."\" />\n";
	$html .= "<input type=\"hidden\" name=\"goods\" value=\"".$goods."\" />\n";
	$html .= "<input type=\"hidden\" name=\"action\" value=\"rakuten_tag\" />\n";
	$html .= "楽天タグファイル 最終更新日：".$rakuten_tag_time."<br />\n";

	//	ファイル毎の更新時間表示
	//	add ohkawara 2017/11/22
	$sql  = "SELECT f_name, update_time FROM ".T_MOOL_RT.
			" GROUP BY f_name, update_time".
			" ORDER BY f_name;";
	if ($result = pg_query($conn_id,$sql)) {
		while ($list = pg_fetch_array($result)) {
			$f_name = $list['f_name'];
			$update_time = $list['update_time'];

			$html .= "　".$f_name."　".$update_time."<br>\n";

		}
	}

	$html .= "<input type=\"file\" size=\"60\" name=\"rakuten\" />\n";
	$html .= "<input type=\"submit\" value=\"登録\" />\n";
	$html .= "</form>\n";
	$html .= "<br />\n";
	$html .= "<br />\n";


	//	Yahoo用
	$yahoo_time = "---------";
	$file_name = FILE_DIR."/".Y_TIME_FILE;
	if (file_exists($file_name)) {
		$yt = filemtime(FILE_DIR."/".Y_TIME_FILE);
		$yahoo_time = date("Y/m/d H:i:s",$yt);
	}
	$html .= "<form action=\"".$PHP_SELF."\" method=\"POST\" enctype=\"multipart/form-data\">\n";
	$html .= "<input type=\"hidden\" name=\"main\" value=\"".$main."\" />\n";
	$html .= "<input type=\"hidden\" name=\"goods\" value=\"".$goods."\" />\n";
	$html .= "<input type=\"hidden\" name=\"action\" value=\"yahoo\" />\n";
	$html .= "Yahooディレクトリーファイル 最終更新日：".$yahoo_time."<br />\n";
	$html .= "<input type=\"file\" size=\"60\" name=\"yahoo\" />\n";
	$html .= "<input type=\"submit\" value=\"登録\" />\n";
	$html .= "</form>\n";
	$html .= "<br />\n";
	$html .= "<br />\n";

	$html .= "Amazon情報はタブ区切り情報をアップ ヘッダも消して下さい。<br>\n";

	//	Amazon用	シューズ
	$amazon1_time = "---------";
	$file_name = FILE_DIR."/".A1_TIME_FILE;
	if (file_exists($file_name)) {
		$a1t = filemtime(FILE_DIR."/".A1_TIME_FILE);
		$amazon1_time = date("Y/m/d H:i:s",$a1t);
	}
	$html .= "<form action=\"".$PHP_SELF."\" method=\"POST\" enctype=\"multipart/form-data\">\n";
	$html .= "<input type=\"hidden\" name=\"main\" value=\"".$main."\" />\n";
	$html .= "<input type=\"hidden\" name=\"goods\" value=\"".$goods."\" />\n";
	$html .= "<input type=\"hidden\" name=\"action\" value=\"amazon1\" />\n";
	$html .= "Amazonシューズディレクトリー（NodeID）ファイル 最終更新日：".$amazon1_time."<br />\n";
	$html .= "<input type=\"file\" size=\"60\" name=\"amazon\" />\n";
	$html .= "<input type=\"submit\" value=\"登録\" />\n";
	$html .= "</form>\n";
	$html .= "<br />\n";
	$html .= "<br />\n";


	//	Amazon用	スポーツ
	$amazon2_time = "---------";
	$file_name = FILE_DIR."/".A2_TIME_FILE;
	if (file_exists($file_name)) {
		$a2t = filemtime(FILE_DIR."/".A2_TIME_FILE);
		$amazon2_time = date("Y/m/d H:i:s",$a2t);
	}
	$html .= "<form action=\"".$PHP_SELF."\" method=\"POST\" enctype=\"multipart/form-data\">\n";
	$html .= "<input type=\"hidden\" name=\"main\" value=\"".$main."\" />\n";
	$html .= "<input type=\"hidden\" name=\"goods\" value=\"".$goods."\" />\n";
	$html .= "<input type=\"hidden\" name=\"action\" value=\"amazon2\" />\n";
	$html .= "Amazonスポーツディレクトリー（NodeID）ファイル 最終更新日：".$amazon2_time."<br />\n";
	$html .= "<input type=\"file\" size=\"60\" name=\"amazon\" />\n";
	$html .= "<input type=\"submit\" value=\"登録\" />\n";
	$html .= "</form>\n";
	$html .= "<br />\n";
	$html .= "<br />\n";


	//	Amazon用	ファッション
	$amazon3_time = "---------";
	$file_name = FILE_DIR."/".A3_TIME_FILE;
	if (file_exists($file_name)) {
		$a3t = filemtime(FILE_DIR."/".A3_TIME_FILE);
		$amazon3_time = date("Y/m/d H:i:s",$a3t);
	}
	$html .= "<form action=\"".$PHP_SELF."\" method=\"POST\" enctype=\"multipart/form-data\">\n";
	$html .= "<input type=\"hidden\" name=\"main\" value=\"".$main."\" />\n";
	$html .= "<input type=\"hidden\" name=\"goods\" value=\"".$goods."\" />\n";
	$html .= "<input type=\"hidden\" name=\"action\" value=\"amazon3\" />\n";
	$html .= "Amazonファッションディレクトリー（NodeID）ファイル 最終更新日：".$amazon3_time."<br />\n";
	$html .= "<input type=\"file\" size=\"60\" name=\"amazon\" />\n";
	$html .= "<input type=\"submit\" value=\"登録\" />\n";
	$html .= "</form>\n";
	$html .= "<br />\n";
	$html .= "<br />\n";


	//	add ohkawara 2017/02/07
	//	Wowma用
	$wowma_time = "---------";
	$file_name = FILE_DIR."/".W_TIME_FILE;
	if (file_exists($file_name)) {
		$wt = filemtime(FILE_DIR."/".W_TIME_FILE);
		$wowma_time = date("Y/m/d H:i:s",$wt);
	}
	$html .= "<form action=\"".$PHP_SELF."\" method=\"POST\" enctype=\"multipart/form-data\">\n";
	$html .= "<input type=\"hidden\" name=\"main\" value=\"".$main."\" />\n";
	$html .= "<input type=\"hidden\" name=\"goods\" value=\"".$goods."\" />\n";
	$html .= "<input type=\"hidden\" name=\"action\" value=\"wowma\" />\n";
	$html .= "Wowmaディレクトリーファイル(SJIS) 最終更新日：".$wowma_time."<br />\n";
	$html .= "<input type=\"file\" size=\"60\" name=\"wowma\" />\n";
	$html .= "<input type=\"submit\" value=\"登録\" />\n";
	$html .= "</form>\n";
	$html .= "<br />\n";
	$html .= "<br />\n";




	return $html;
}



//	楽天ディレクトリー更新作業
function update_rd(&$ERROR,&$MSG) {

	$file_name = FILE_DIR."/".R_DIR_FILE;

	if (is_uploaded_file($_FILES["rakuten"]["tmp_name"])) {
		if (move_uploaded_file($_FILES["rakuten"]["tmp_name"], $file_name)) {
			@chmod($file_name, 0666);
		} else {
			$ERROR[] = "ファイルをアップロードできません。";
			return;
		}
	} else {
		$ERROR[] = "ファイルが選択されていません。";
		return;
	}

	if (file_exists($file_name)) {

		//	サッカー、フットサル以外のキーワードで必ず必要なディレクトリID
		$CHECK_SET_ID = array('551180');	//	add ohkawara 2017/05/17

		//	DB情報削除
		$sql  = "TRUNCATE TABLE ".T_MOOL_RD;
		pg_exec(conn_id,$sql);

		$LIST = file($file_name);
		unset($LIST[0]);
		if ($LIST) {
#			mb_convert_variables("eucjp-win", "sjis-win", $LIST);
			mb_convert_variables("UTF-8", "sjis-win", $LIST);
			$i = 1;
			$sql = "";
			foreach ($LIST AS $LINE) {
				$flg = 0;
				list($dir_id,$dir_name) = explode(",",$LINE);
				$dir_id = gomitori($dir_id);
				$dir_name = gomitori($dir_name);

				if (!$dir_id || !$dir_name) { continue; }

				//	サッカー　or　フットサル　キーワードが有るかチェック
				if (preg_match("/サッカー|フットサル/",$dir_name)) {
					$flg = 1;
				//	add ohkawara 2017/05/17
				} elseif (array_search($dir_id, $CHECK_SET_ID) !== FALSE) {
					$flg = 1;
				}

				//	db登録
				$sql  .= "INSERT INTO ".T_MOOL_RD.
						" VALUES('".$dir_id."','".$dir_name."','".$flg."');";

				$i++;
				if ($i >= 10) {
					//	インサート
					if (!pg_exec(conn_id,$sql)) {
						$ERROR[] = "INSERT ERROR";
						return;
					}
					$sql = "";
					$i = 1;
				}

			}

			if ($sql) {
				if (!pg_exec(conn_id,$sql)) {
					$ERROR[] = "INSERT ERROR";
					return;
				}
			}

		}
	} else {
		$ERROR[] = "アップロードファイルが確認できません。";
		return;
	}

	//	更新ファイル作成
	$file_name = FILE_DIR."/".R_TIME_FILE;
	$OUT = fopen($file_name,"w");
	fwrite($OUT,"\n");
	fclose($OUT);
	@chmod($file_name,0666);

	$MSG = "楽天ディレクトリーファイル更新しました。";

}



//	楽天タグ更新作業
//	add ohkawara 2017/02/21
function update_rt(&$ERROR,&$MSG) {

	$file_name = FILE_DIR."/".R_TAG_FILE;

	//	add ohkawara 2017/11/22
	$f_name = "";
	$update_time = "";

	if (is_uploaded_file($_FILES["rakuten"]["tmp_name"])) {
		if (move_uploaded_file($_FILES["rakuten"]["tmp_name"], $file_name)) {
			@chmod($file_name, 0666);

			//	add ohkawara 2017/11/22
			$f_name = $_FILES["rakuten"]["name"];
			$update_time = date("Y-m-d H:i:s", time());
		} else {
			$ERROR[] = "ファイルをアップロードできません。";
			return;
		}
	} else {
		$ERROR[] = "ファイルが選択されていません。";
		return;
	}

	if (file_exists($file_name)) {
		//	DB情報削除
		//$sql  = "TRUNCATE TABLE ".T_MOOL_RT;	//	del ohkawara 2017/11/22
		$sql  = "DELETE FROM ".T_MOOL_RT." WHERE f_name='".$f_name."';";	//	add ohkawara 2017/11/22
		pg_exec(conn_id,$sql);

		$LIST = file($file_name);
		unset($LIST[0]);
		if ($LIST) {


			$i = 1;
			$sql = "";
			//foreach ($LIST AS $LINE) {			//	del ohkawara 2017/04/06
			foreach ($LIST AS $del_key => $LINE) {		//	add ohkawara 2017/04/06

#				mb_convert_variables("eucjp-win", "sjis-win", $LINE);	//	add ohkawara 2017/11/22
				mb_convert_variables("UTF-8", "sjis-win", $LINE);	//	add ohkawara 2017/11/22

				$lists = explode(",",$LINE);

				//	add ohkawara 2017/04/06
				$rd_id = $lists[0];		//	ディレクトリID
				unset($lists[0]);		//	ディレクトリID
				unset($lists[1]);		//	パス名
				$tag_class = $lists[2];	//	分類
				$tag_class = preg_replace("/\"/", "", $tag_class);
				unset($lists[2]);		//	分類
				$tag = $lists[3];		//	タグ名
				$tag = preg_replace("/\"/", "", $tag);
				unset($lists[3]);		//	タグ名
				$tag_id = $lists[4];	//	タグID
				unset($lists[4]);		//	タグID
				unset($lists[5]);		//	項目選択肢別在庫登録可


				//	del ohkawara 2017/04/06
				//unset($lists[0]);		//	ジャンル
				//$tag_class = $lists[1];	//	タグの分類
				//unset($lists[1]);
				//$tag = $lists[2];		//	タグ
				//unset($lists[2]);
				//$tag_id = $lists[3];	//	タグID
				//unset($lists[3]);

				$CHK_DIR = $lists;		//	登録可能な全商品ディレクトリ

				//if (!array_search("スポーツ・アウトドア", $CHK_DIR)) {				//	del ohkawara 2017/04/06
				//	continue;															//	del ohkawara 2017/04/06
				//} elseif (!preg_match("/ブランド|サイズ|靴サイズ/", $tag_class)) {	//	del ohkawara 2017/04/06
				if (!preg_match("/ブランド|サイズ|靴サイズ/", $tag_class)) {			//	add ohkawara 2017/04/06
					unset($LIST[$del_key]);	//	add ohkawara 2017/04/06
					continue;
				}

				$tag = $quote = pg_escape_string($tag);	//	add ohkawara 2022/09/05

				//	db登録
				$sql  .= "INSERT INTO ".T_MOOL_RT.
						//" VALUES('".$rd_id."','".$tag_id."','".$tag."','".$tag_class."');";	//	add '".$rd_id."', ohkawara 2017/04/06	//	del ohkawara 2017/11/22
						" VALUES('".$rd_id."','".$tag_id."','".$tag."','".$tag_class."','".$f_name."','".$update_time."');";	//	add  ohkawara 2017/11/22
				$i++;
				if ($i >= 10) {
					//	インサート
					if (!pg_exec(conn_id,$sql)) {
						$ERROR[] = "INSERT ERROR(1)";
						return;
					}
					$sql = "";
					$i = 1;
				}

				unset($LIST[$del_key]);	//	add ohkawara 2017/04/06
			}

			if ($sql) {
				if (!pg_exec(conn_id,$sql)) {
					$ERROR[] = "INSERT ERROR(2)";
					return;
				}
			}

		}
	} else {
		$ERROR[] = "アップロードファイルが確認できません。";
		return;
	}

	//	更新ファイル作成
	$file_name = FILE_DIR."/".R_TAG_TIME_FILE;
	$OUT = fopen($file_name,"w");
	fwrite($OUT,"\n");
	fclose($OUT);
	@chmod($file_name,0666);

	$MSG = "楽天タグファイル更新しました。";

}



//	Yahoo更新作業
function update_yd(&$ERROR,&$MSG) {

	$file_name = FILE_DIR."/".Y_DIR_FILE;

	if (is_uploaded_file($_FILES["yahoo"]["tmp_name"])) {
		if (move_uploaded_file($_FILES["yahoo"]["tmp_name"], $file_name)) {
			@chmod($file_name, 0666);
		} else {
			$ERROR[] = "ファイルをアップロードできません。";
			return;
		}
	} else {
		$ERROR[] = "ファイルが選択されていません。";
		return;
	}

	if (file_exists($file_name)) {

		//	DB情報削除
		$sql  = "TRUNCATE TABLE ".T_MOOL_YD;
		pg_exec(conn_id,$sql);

		$LIST = file($file_name);
		unset($LIST[0]);
		if ($LIST) {
#			mb_convert_variables("eucjp-win", "sjis-win", $LIST);
			mb_convert_variables("UTF-8", "sjis-win", $LIST);
			$i = 1;
			$sql = "";
			foreach ($LIST AS $LINE) {
				$flg = 0;
				//list($id,$name,$path_name,$spec_id,$spec_name,$spec_value_id,$spec_value_name) = explode(",",$LINE);
				list($id,$name,$path_name,$spec_id,$spec_name,$spec_value_name,$spec_value_id,$selection_type,$data_type) = explode(",",$LINE);
				$id = gomitori($id);
				$name = gomitori($name);
				$path_name = gomitori($path_name);
				$spec_id = gomitori($spec_id);
				$spec_name = gomitori($spec_name);
				$spec_value_id = gomitori($spec_value_id);
				$spec_value_name = gomitori($spec_value_name);

				if (!$id || !$name || !$path_name) { continue; }

				//	サッカー　or　フットサル　キーワードが有るかチェック
				if (preg_match("/サッカー|フットサル/",$path_name)) { $flg = 1; }

				//	db登録
				$sql  .= "INSERT INTO ".T_MOOL_YD.
						" VALUES('".$id."','".$name."','".$path_name."','".$spec_id."','".$spec_name."','".$spec_value_id."','".$spec_value_name."','".$flg."');";

				$i++;
				if ($i >= 10) {
					//	インサート
					if (!pg_exec(conn_id,$sql)) {
						$ERROR[] = "INSERT ERROR";
						return;
					}
					$sql = "";
					$i = 1;
				}

			}

			if ($sql) {
				if (!pg_exec(conn_id,$sql)) {
					$ERROR[] = "INSERT ERROR";
					return;
				}
			}

		}
	} else {
		$ERROR[] = "アップロードファイルが確認できません。";
		return;
	}

	//	更新ファイル作成
	$file_name = FILE_DIR."/".Y_TIME_FILE;
	$OUT = fopen($file_name,"w");
	fwrite($OUT,"\n");
	fclose($OUT);
	@chmod($file_name,0666);

	$MSG = "Yahooディレクトリーファイル更新しました。";

}



//	Amazon更新作業
function update_ad(&$ERROR, &$MSG, $node_type = 1) {

	$file_name = FILE_DIR."/".A1_DIR_FILE;
	if ($node_type == 2) {
		$file_name = FILE_DIR."/".A2_DIR_FILE;
	} elseif ($node_type == 3) {
		$file_name = FILE_DIR."/".A3_DIR_FILE;
	}

	if (is_uploaded_file($_FILES["amazon"]["tmp_name"])) {
		if (move_uploaded_file($_FILES["amazon"]["tmp_name"], $file_name)) {
			@chmod($file_name, 0666);
		} else {
			$ERROR[] = "ファイルをアップロードできません。";
			return;
		}
	} else {
		$ERROR[] = "ファイルが選択されていません。";
		return;
	}

	if (file_exists($file_name)) {

		//	DB情報削除
		$sql  = "DELETE FROM ".T_MOOL_AD;
		$sql .= " WHERE node_type='".$node_type."';";
		pg_exec(conn_id,$sql);

		$LIST = file($file_name);

		unset($LIST[0]);
		if ($LIST) {
#			mb_convert_variables("eucjp-win", "sjis-win", $LIST);
			mb_convert_variables("UTF-8", "sjis-win", $LIST);
			$i = 1;
			$sql = "";
			foreach ($LIST AS $LINE) {
				$flg = 0;
				list($node_id,$node_name) = explode("\t",$LINE);

				$node_id = gomitori($node_id);
				$node_name = gomitori($node_name);

				if (!$node_id || !$node_name) { continue; }

				//	サッカー　or　フットサル　キーワードが有るかチェック
				if (preg_match("/サッカー|フットサル|エナメルバッグ|競技備品\/審判用品/",$node_name)) { $flg = 1; }

				//	db登録
				$sql  .= "INSERT INTO ".T_MOOL_AD.
						" VALUES('".$node_id."','".$node_type."','".$node_name."','".$flg."');";

				$i++;
				if ($i >= 10) {
					//	インサート
					if (!pg_exec(conn_id,$sql)) {
						$ERROR[] = "INSERT ERROR";
						return;
					}
					$sql = "";
					$i = 1;
				}

			}

			if ($sql) {
				if (!pg_exec(conn_id,$sql)) {
					$ERROR[] = "INSERT ERROR";
					return;
				}
			}

		}

		//	DBゴミ整理
		$sql  = "VACUUM mool_ad;";
		pg_exec(conn_id,$sql);

	} else {
		$ERROR[] = "アップロードファイルが確認できません。";
		return;
	}

	//	更新ファイル作成
	$a_time_file = A1_TIME_FILE;
	if ($node_type == 2) {
		$a_time_file = A2_TIME_FILE;
	} elseif ($node_type == 3) {
		$a_time_file = A3_TIME_FILE;
	}
	$file_name = FILE_DIR."/".$a_time_file;
	$OUT = fopen($file_name,"w");
	fwrite($OUT,"\n");
	fclose($OUT);
	@chmod($file_name,0666);

	$MSG = "Amazonディレクトリーファイル更新しました。";

}



//	Wowma更新作業
//	add ohkawara 2017/02/07
function update_wd(&$ERROR,&$MSG) {

	$file_name = FILE_DIR."/".W_DIR_FILE;

	if (is_uploaded_file($_FILES["wowma"]["tmp_name"])) {
		if (move_uploaded_file($_FILES["wowma"]["tmp_name"], $file_name)) {
			@chmod($file_name, 0666);
		} else {
			$ERROR[] = "ファイルをアップロードできません。";
			return;
		}
	} else {
		$ERROR[] = "ファイルが選択されていません。";
		return;
	}

	if (file_exists($file_name)) {

		//	DB情報削除
		$sql  = "TRUNCATE TABLE ".T_MOOL_WD;
		pg_exec(conn_id,$sql);

		$LIST = file($file_name);
		unset($LIST[0]);
		if ($LIST) {
#			mb_convert_variables("eucjp-win", "sjis-win", $LIST);
			mb_convert_variables("UTF-8", "sjis-win", $LIST);
			$i = 1;
			$sql = "";
			foreach ($LIST AS $LINE) {
				$flg = 0;
				list($dir_id, $venue, $dir_name1, $dir_name2, $dir_name3, $dir_name4, $dir_name5, $dir_name6) = explode(",",$LINE);
				$dir_id = gomitori($dir_id);
				$dir_name  = "";
				$dir_name1 = gomitori($dir_name1);
				$dir_name2 = gomitori($dir_name2);
				$dir_name3 = gomitori($dir_name3);
				$dir_name4 = gomitori($dir_name4);
				$dir_name5 = gomitori($dir_name5);
				$dir_name6 = gomitori($dir_name6);

				if ($dir_name1 != "") {
					$dir_name = $dir_name1;
					if ($dir_name2 != "") {
						$dir_name .= " > ".$dir_name2;
						if ($dir_name3 != "") {
							$dir_name .= " > ".$dir_name3;
							if ($dir_name4 != "") {
								$dir_name .= " > ".$dir_name4;
								if ($dir_name5 != "") {
									$dir_name .= " > ".$dir_name5;
									if ($dir_name6 != "") {
										$dir_name .= " > ".$dir_name6;
									}
								}
							}
						}
					}
				}

				if (!$dir_id || !$dir_name) { continue; }

				//	サッカー　or　フットサル　キーワードが有るかチェック
				if (preg_match("/サッカー|フットサル/",$dir_name)) { $flg = 1; }

				//	db登録
				$sql  .= "INSERT INTO ".T_MOOL_WD.
						" VALUES('".$dir_id."','".$dir_name."','".$flg."');";

				$i++;
				if ($i >= 10) {
					//	インサート
					if (!pg_exec(conn_id,$sql)) {
						$ERROR[] = "INSERT ERROR";
						return;
					}
					$sql = "";
					$i = 1;
				}

			}

			if ($sql) {
				if (!pg_exec(conn_id,$sql)) {
					$ERROR[] = "INSERT ERROR";
					return;
				}
			}

		}
	} else {
		$ERROR[] = "アップロードファイルが確認できません。";
		return;
	}

	//	更新ファイル作成
	$file_name = FILE_DIR."/".W_TIME_FILE;
	$OUT = fopen($file_name,"w");
	fwrite($OUT,"\n");
	fclose($OUT);
	@chmod($file_name,0666);

	$MSG = "Wowmaディレクトリーファイル更新しました。";

}



//	カラム情報ゴム取り除き
function gomitori($val) {

	$val = trim($val);
	$val = preg_replace("/^\"/","",$val);
	$val = preg_replace("/\"$/","",$val);
	$val = trim($val);
	$val = addslashes($val);

	return $val;
}
?>