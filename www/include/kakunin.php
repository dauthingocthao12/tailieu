<?php
/*

	ネイバーズスポーツ	発送状況プログラム

*/


//	入力画面
function kakunin_html($ERROR) {

	$html = "";

	if ($ERROR) {
		$error_html = error_html($ERROR);
	}

	$email = $_POST['email'];
	$sells_num = $_POST['sells_num'];

	$INPUTS = array();
	$DEL_INPUTS = array();

	$INPUTS['ERROR'] = $error_html;		//	エラーメッセージ
	$INPUTS['EMAIL'] = $email;			//	入力メールアドレス
	$INPUTS['SELLSNUM'] = $sells_num;	//	ご注文番号
/*
	if (!$ERROR) {
		$DEL_INPUTS['ERRORMSG'] = 1;
	}
*/
	//	html作成・置換
	$make_html = new read_html();
	$make_html->set_dir(TEMPLATE_DIR);
	$make_html->set_file("kakunin.htm");
	$make_html->set_rep_cmd($INPUTS);
	$make_html->set_del_cmd($DEL_INPUTS);
	$html = $make_html->replace();

	return $html;
}



//	入力された値で情報があるかチェック
function kakunin_check(&$ERROR) {

	$ERROR = array();

	$email = trim($_POST['email']);
	$sells_num = trim($_POST['sells_num']);
#	$email = mb_convert_kana($email, "a", "EUC-JP");			//	"a"→全角英数字を半角に
	$email = mb_convert_kana($email, "a", "UTF-8");				//	"a"→全角英数字を半角に
	$email = strtolower($email);								//	strtolower 文字列を小文字にする
#	$sells_num = mb_convert_kana($sells_num, "n", "EUC-JP");	//	"n"→全角数字を半角に
	$sells_num = mb_convert_kana($sells_num, "n", "UTF-8");		//	"n"→全角数字を半角に
	$sells_num = preg_replace("/[^0-9]/", "", $sells_num);		//	先頭の数字を消す


	if (!$email) {
		$ERROR[] = "メールアドレスが入力されておりません。";
	}
	if (!$sells_num) {
		$ERROR[] = "ご注文番号が入力されておりません。";
	}

	if ($ERROR) {
		return;
	}

	$sql =  "SELECT sells.hinban, sells.title, sells.buy_n, sells.send, sells.h_time FROM ".T_KOJIN." as kojin, ".T_SELLS." as sells".
			" WHERE sells.sells_num='".$sells_num."'".
			" AND kojin.email='".$email."'".
			" AND sells.kojin_num=kojin.kojin_num".
			" ORDER BY sells.hinban;";
	if ($result = pg_query(DB, $sql)){
		$count = pg_num_rows($result);
	}

	if ($count < 1) {
		$ERROR[] = "注文番号または注文時のメールアドレスが間違っています。";
	}


}



//	発送詳細
function kakunin_syousai_html() {

	$PHP_SELF = $_SERVER['PHP_SELF'];

	$email = trim($_POST['email']);
	$sells_num = trim($_POST['sells_num']);
#	$email = mb_convert_kana($email, "a", "EUC-JP");			//	"a"→全角英数字を半角に
	$email = mb_convert_kana($email, "a", "UTF-8");				//	"a"→全角英数字を半角に
	$email = strtolower($email);								//	strtolower 文字列を小文字にする
#	$sells_num = mb_convert_kana($sells_num, "n", "EUC-JP");	//	"n"→全角数字を半角に
	$sells_num = mb_convert_kana($sells_num, "n", "UTF-8");		//	"n"→全角数字を半角に
	$sells_num = preg_replace("/[^0-9]/", "", $sells_num);		//	先頭の数字を消す

	$year = substr($sells_num, 0, 2);
	$year = $year + 2000;
	$mon  = substr($sells_num, 2, 2);
	$day  = substr($sells_num, 4, 2);
	$hacyu = $year."年".$mon."月".$day."日";

	$html .= "<h2 class=\"title-nbs title-items-list\">商品発送状況　詳細</h2>\n";
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
	$html .= "<table class=\"kakunin list-table\">\n";
	$html .= "	<tr class=\"list-table-pc\">\n";
	$html .= "		<th data-label=\"商品番号\">商品番号</th>\n";
	$html .= "		<th data-label=\"商品名\">商品名</th>\n";
	$html .= "		<th data-label=\"注文数\">注文数</th>\n";
	$html .= "		<th data-label=\"発送状況\">発送状況</th>\n";
	$html .= "	</tr>\n";

	$sql =  "SELECT sells.hinban, sells.title, sells.buy_n, sells.send, sells.h_time FROM ".T_KOJIN." as kojin, ".T_SELLS." as sells".
			" WHERE sells.sells_num='".$sells_num."'".
			" AND kojin.email='".$email."'".
			" AND sells.kojin_num=kojin.kojin_num".
			" ORDER BY sells.hinban;";
	if ($result = pg_query(DB, $sql)){

		while ($list = pg_fetch_array($result)){//	pg_fetch_array  行（縦列）を配列として取得する

			//	商品データを変数に代入
			$hinban = $list['hinban'];	//	商品番号
			$title = $list['title'];	//	商品名
			$buy_n = $list['buy_n'];	//	注文数
			$send = $list['send'];		//	発送状況
			$h_time = $list['h_time'];	//	発送時間

			//	list	配列と同様の形式で、複数の変数への代入を行う
			//	split	正規表現により文字列を分割し、配列に格納する
##			list($k_year, $k_mon, $k_day) = split("-", $h_time);
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

				$sql2 = "SELECT * FROM option".
						" WHERE sells_num='".$sells_num."'".
						" AND option_num='".$title."';";

				if ($result2 = pg_query(DB, $sql2)) {
					$list2 = pg_fetch_array($result2);	//	pg_fetch_array-取得した行（レコード）を配列で返します。
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

					$title  = "マーキング商品名：".$title_." <br>\n";
					if ($seban_l) {
						$title .= "背番号：".$SEBAN_N[$seban_l]." [ ".$seban_num." ] <br>\n";
					}
					if ($sename_l) {
						$title .= "背ネーム：".$SENAME_N[$sename_l]." [ ".$sename_name." ] <br>\n";
					}
					if ($muneban_l) {
						$title .= "胸番号：".$MUNEBAN_N[$muneban_l]." [ ".$muneban_num." ] <br>\n";
					}
					if ($pant_l) {
						$title .= "パンツ番号：".$PANT_N[$pant_l]." [ ".$pant_num." ] <br>\n";
					}
					if ($bach_l) {
						$title .= "バッジ：".$BACH_N[$bach_l]." <br>\n";
					}

					$buy_n = 1;
				}

			}
			$html .= "	<tr class=\"edit-table-text\">\n";
			$html .= "		<td data-label=\"発送状況\">".$hinban."</td>\n";
			$html .= "		<td data-label=\"商品名\">".$title."</td>\n";
			$html .= "		<td data-label=\"注文数\">".$buy_n."</td>\n";
			$html .= "		<td data-label=\"発送状況\">".$jyo."</td>\n";
			$html .= "	</tr>\n";
		}
	}
	$html .= "</table>\n";
	$html .= "	<div class=\"button_gui edit-button\">\n";
	$html .= "		<form action=\"".$PHP_SELF."\">\n";
	$html .= "			<input type=\"submit\" class=\"submit\" value=\"戻る\">\n";
	$html .= "		</form>\n";
	$html .= "	</div>\n";
	//$html .= "</div>\n";

	return $html;

}
?>