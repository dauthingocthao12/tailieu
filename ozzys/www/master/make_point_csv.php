<?PHP
/*

	ポイント所持者リスト作成CSV

*/

	include './array.inc';
	include "../../cone.inc";

	//	リスト読み込み
	$csv = "";
	$sql  = "SELECT distinct e.kojin_num, e.name_s, e.name_n,".
			" e.prf, e.city, e.add1, e.add2, e.email, e.point".
			" FROM kojin e".
			" WHERE e.point!='0'".
			" AND e.saku='0'";
			" ORDER BY e.kojin_num;";
	if ($result = pg_query($conn_id,$sql)) {
		WHILE ($list = pg_fetch_array($result)) {
			foreach($list AS $key => $val) {
				## $val = mb_convert_kana($val,"asKV","EUC-JP");
				$val = mb_convert_kana($val,"asKV","UTF-8");
				$$key = trim($val);
			}
			$csv .= "\"".$kojin_num."\",\"".$name_s." ".$name_n."\",\"".$PRF_N[$prf].$city.$add1.$add2."\",".
					"\"".$email."\",\"".$point."\",\n";
		}
	}

	$csv = "\"会員番号\",\"名前\",\"住所\",\"メールアドレス\",\"ポイント\",\n".$csv;

	$filename = "point_list_".date("YmdHis").".csv";

	header("Cache-Control: public");
	header("Pragma: public");
	header("Content-disposition: attachment;filename=$filename");
	if (stristr($HTTP_USER_AGENT, "MSIE")) {
		header("Content-Type: text/octet-stream");
	} else {
		header("Content-Type: application/octet-stream;");
	}

	## $csv = mb_convert_encoding($csv,"sjis-win","EUC-JP");
	$csv = mb_convert_encoding($csv,"sjis-win","UTF-8");
//	$csv = replace_decode_sjis($csv);

	echo $csv;

	pg_close($conn_id);

	exit;

?>
