<?PHP
/*

	ネイバーズスポーツ	購入履歴表示プログラム

*/


//	履歴一覧
function list_html() {

	$PHP_SELF = $_SERVER['PHP_SELF'];

	list($email, $pass) = explode("<>", $_SESSION['idpass']);

	$sql =  "SELECT kojin.kojin_num, sells.sells_num FROM ".T_KOJIN." as kojin, ".T_SELLS." as sells".
			" WHERE kojin.email='".$email."'".
			" AND kojin.kojin_num=sells.kojin_num".
			" AND kojin.kojin_num<='100001'".
			" GROUP BY kojin.kojin_num, sells.sells_num".
			" ORDER BY sells.sells_num DESC;";
	if ($result = pg_query(DB, $sql)) {
		$count = pg_num_rows($result);
	}

	$html  = "<h2 class=\"title-nbs title-items-list\">お買い物履歴 一覧</h2>\n";
	$html .= "<div id=\"rireki\">\n";

	if ($count < 1) {
		$html .= "<table>\n";
		$html .= "  <tr>\n";
		$html .= "    <th>ご注文されておりません。</th>\n";
		$html .= "  </tr>\n";
		$html .= "</table>\n";
	} else {
		$html .= "<table class=\"table-title\">\n";
		$html .= "  <tr>\n";
		$html .= "    <th>お買い物履歴の詳細を見たい場合は、詳細ボタンを押して下さい。</th>\n";
		$html .= "  </tr>\n";
		$html .= "</table>\n";

		$html .= "<form action=\"".$PHP_SELF."\" method=\"POST\" id=\"rireki_view\">\n";

		$html .= "<table class=\"list-table rireki-table\">\n";
		$html .= "  <tr class=\"list-table-pc\">\n";
		$html .= "    <th>ご注文番号</th>\n";
		$html .= "    <th>ご注文日</th>\n";
		$html .= "    <th>発送状況</th>\n";
		$html .= "    <th>詳細</th>\n";
		$html .= "  </tr>\n";

		while ($list = pg_fetch_array($result)) {
			$sells_num = $list['sells_num'];
			$kojin_num = $list['kojin_num'];

			$year = substr($sells_num, 0, 2);
			$year = $year + 2000;
			$mon  = substr($sells_num, 2, 2);
			$day  = substr($sells_num, 4, 2);
			$hacyu = $year."年".$mon."月".$day."日";

			$s0 = 0;
			$s1 = 0;
			$s2 = 0;
			$s3 = 0;
			$sql2 = "SELECT send FROM ".T_SELLS.
					" WHERE sells_num='".$sells_num."';";
			if ($result2 = pg_query(DB, $sql2)) {
				while ($list = pg_fetch_array($result2)) {
					$send = $list['send'];

					if ($send == 0) {
						$s0 = 1;
					} elseif ($send == 1) {
						$s1 = 1;
					} elseif ($send == 2) {
						$s2 = 1;
					} elseif ($send == 3) {
						$s3 = 1;
					}
				}
			}

			if ($s3 == 1) {
				$jyou = "お取り寄せ中";
			} elseif ($s0 == 0 && $s1 == 1 && $s2 == 0 && $s3 == 0) {
				$jyou = "発送済み";
			} elseif ($s0 == 0 && $s1 == 0 && $s2 == 1 && $s3 == 0) {
				$jyou = "キャンセル";
			} else {
				$jyou = "発送準備中";
			}

			$html .= "  <tr>\n";
			$html .= "    <td data-label=\"ご注文番号\">".$sells_num."</td>\n";
			$html .= "    <td data-label=\"ご注文日\">".$hacyu."</td>\n";
			$html .= "    <td data-label=\"発送状況\">".$jyou."</td>\n";
			$html .= "    <td data-label=\"詳細\" class=\"detail-text\"><input type=\"button\" value=\"詳細\" class=\"btn-standard\" OnClick=\"view_rireki('".$sells_num."');\" /></td>\n";
			$html .= "  </tr>\n";
		}
		$html .= "</table>\n";
		$html .= "<input type=\"hidden\" name=\"mode\" value=\"syo\" />\n";
		$html .= "<input type=\"hidden\" name=\"kojin_num\" value=\"".$kojin_num."\" />\n";
		$html .= "<input type=\"hidden\" name=\"sells_num\" id=\"rireki_code\" />\n";
		$html .= "</form>\n";
	}
	$html .= "</div>\n";

	return $html;
}



//	履歴詳細
function syousai_html() {
	//グローバル宣言
	global $SEBAN_N, $SENAME_N, $MUNEBAN_N, $PANT_N, $BACH_N;

	$PHP_SELF = $_SERVER['PHP_SELF'];

	//ご注文番号を取得
	$sells_num = $_POST['sells_num'];

	//会員番号を取得
	$kojin_num = $_POST['kojin_num'];

	//ご注文日を抜き出す
	$year = substr($sells_num, 0, 2);
	$year = $year + 2000;
	$mon  = substr($sells_num, 2, 2);
	$day  = substr($sells_num, 4, 2);
	$hacyu = $year."年".$mon."月".$day."日";

	$html .= "<h2 class=\"title-nbs title-items-list\">お買い物履歴 詳細</h2>\n";
	$html .= "<div id=\"rireki\">\n";
	

	$html .= "<table class=\"list-title\">\n";
	$html .= "  <tr>\n";
	$html .= "    <th class=\"edit-word-break\">ご注文番号</th>\n";
	$html .= "    <td>".$sells_num."</td>\n";
	$html .= "  </tr>\n";
	$html .= "  <tr>\n";
	$html .= "    <th class=\"edit-word-break\">ご注文日</th>\n";
	$html .= "    <td>".$hacyu."</td>\n";
	$html .= "  </tr>\n";
	$html .= "</table>\n";
	
	$html .= "<table class=\"list-table rireki-table\">\n";
	$html .= "  <tr class=\"list-table-pc\">\n";
	$html .= "    <th class=\"hinban\">商品番号</th>\n";
	$html .= "    <th>商品名</th>\n";
	$html .= "    <th>注文数</th>\n";
	$html .= "    <th>発送状況</th>\n";
	$html .= "  </tr>\n";

	
	//購入した商品データを取得
	$sql  = "SELECT hinban, title, buy_n, send, h_time FROM ".T_SELLS.
			" WHERE sells_num='".$sells_num."'".
			" AND kojin_num='".$kojin_num."'".
			" ORDER BY hinban;";	//	hinbanフィールドの昇順に並ぶ
		
	//SQLを実行
	if ($result = pg_query(DB, $sql)) {
		//購入した商品データの項目数の結果を取得
		$count = pg_num_rows($result);
	}
	while ($list = pg_fetch_array($result)) {	//	pg_fetch_array  行（縦列）を配列として取得する

		//商品データを変数に代入
		$hinban = $list['hinban'];	//	商品番号
		$title = $list['title'];	//	商品名
		$buy_n = $list['buy_n'];	//	注文数
		$send = $list['send'];		//	発送状況
		$h_time = $list['h_time'];	//	発送時間
		
		//list  配列と同様の形式で、複数の変数への代入を行う
		//split  正規表現により文字列を分割し、配列に格納する
##		list($k_year, $k_mon, $k_day) = split("-", $h_time);
		list($k_year, $k_mon, $k_day) = explode("-", $h_time);

		if ($send == 0) {
			$jyo = "準備中";
		} elseif ($send == 1) {
			$jyo = $k_year."/".$k_mon."/".$k_day."<br />発送済み";
		} elseif ($send == 2) {
			$jyo = "キャンセル";
		} elseif ($send == 3) {
			$jyo = $k_year."/".$k_mon."/".$k_day."<br />入荷予定";
		}

		if ($hinban == "option") {
			$hinban = "マーキング";

			//マーキング商品に該当する情報を「optionテーブル」から取得
			$sql2 = "SELECT * FROM option".
					" WHERE sells_num='".$sells_num."'".
					" AND option_num='".$title."';";
			if ($result2 = pg_query(DB, $sql2)) {
				$list2 = pg_fetch_array($result2);	//pg_fetch_array-取得した行（レコード）を 配列で返します。
				$hinban_ = $list2['hinban'];
				$title_ = $list2['title'];
				$seban_l = $list2['seban_l'];
				$seban_num = $list2['seban_num'];
				$sename_l = $list2['sename_l'];
				$sename_name = $list2['sename_name'];
				$muneban_l = $list2['muneban_l'];
				$muneban_num = $list2['muneban_num'];
				$pant_l = $list2['pant_l'];
				$pant_num = $list2['pant_num'];
				$bach_l = $list2['bach_l'];

				$title = "マーキング商品名：".$title_." <br>\n";

				if ($seban_l) {
					$title .= "背番号：".$SEBAN_N[$seban_l]." [ ".$seban_num." ] <br />\n";
				}
				if ($sename_l) {
					$title .= "背ネーム：".$SENAME_N[$sename_l]." [ ".$sename_name." ] <br />\n";
				}
				if ($muneban_l) {
					$title .= "胸番号：".$MUNEBAN_N[$muneban_l]." [ ".$muneban_num." ] <br />\n";
				}
				if ($pant_l) {
					$title .= "パンツ番号：".$PANT_N[$pant_l]." [ ".$pant_num." ] <br />\n";
				}
				if ($bach_l) {
					$title .= "バッジ：".$BACH_N[$bach_l]." <br />\n";
				}

				$buy_n = 1;

			}
		}

		$html .= "  <tr class=\"edit-table-text\">\n";
		$html .= "    <td data-label=\"商品番号\">".$hinban."</td>\n";	
		$html .= "    <td data-label=\"商品名\">".$title."</td>\n";
    	$html .= "    <td data-label=\"注文数\">".$buy_n."</td>\n";
		$html .= "    <td data-label=\"発送状況\">".$jyo."</td>\n";
		$html .= "  </tr>\n";	
		$html .= "  </tr>\n";

	}

	$html .= "</table>\n";
	$html .= "<div class=\"button_gui edit-button\">\n";
	$html .= "  <form action=\"".$PHP_SELF."\" method=\"POST\">\n";
	$html .= "  <input type=\"submit\" class=\"btn-standard\" value=\"戻る\" class=\"button\">\n";
	$html .= "  </form>\n";
	$html .= "</div>\n";
	$html .= "</div>\n";

	return $html;
}
?>
