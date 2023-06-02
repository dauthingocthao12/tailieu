<?PHP
//	アフィリエイトサブルーチンファイル

//	獲得ポイント
function point_checks($af_num) {
global $conn_id,$afuser_table,$appoint_table,$application_table,$set_point_mon;

	//	確定ポイント(全て)
	$order_time = mktime(0,0,0,date("m")-$set_point_mon,1,date("Y"));
	$order_day = date("Y-m-d",$order_time);
	$sql =  "SELECT SUM(point) AS dec_point FROM $appoint_table" .
			" WHERE af_num='$af_num' AND state='1'" .
			" AND send_day<'$order_day';";
	if ($result = mysqli_query($conn_id,$sql)) {
		$list = mysqli_fetch_array($result);
		(int)$dec_point = $list['dec_point'];
	}

	//	支払い情報(全て)
	$sql =  "SELECT SUM(af_point) AS pay_point FROM $application_table" .
			" WHERE af_num='$af_num' AND state!='2';";
	if ($result = mysqli_query($conn_id,$sql)) {
		$list = mysqli_fetch_array($result);
		(int)$pay_point = $list['pay_point'];
	}

	//	獲得確定ポイント
	$point = $dec_point - $pay_point;
	if ($point < 1) { $point = 0; }

	return $point;

}



//	カタカナチェック
function isZenKkana($data) {

    //magic_quotes_gpcがONの時は、エスケープを解除する
    if (get_magic_quotes_gpc()) {
        $data = stripslashes($data);
    }

    $data= trim($data);
    $pat = "^[ァアィイゥウェエォオカガキギクグケゲコゴサザシジスズセゼソゾタダチヂッツヅテデトドナニヌネノハバパヒビピフブプヘベペホボポマミムメモャヤュユョヨラリルレロヮワヰヱヲン]+$";   
 if (mb_ereg_match($pat, $data)) {
        return true;
    } else {
        return false;
    }

}



//	数字チェック
function isNum($data) {

    $pat = "^[0-9]+$";
##    if (ereg($pat, trim($data))) {
	  if (preg_match($pat, trim($data))) {
        return true;
    } else {
        return false;
    }

}



//	ロック
function lock($lockdir) {

	if (file_exists($lockdir)) {
		$now = time();
		list($device,$inode,$remode,$num_of_link,$user_id,$group_id,$rdev,$size,
		$atime,$mtime,$ctime,$blocksize,$num_of_blocks) = stat($lockdir);
		$sa = $now - $mtime;

		if ($sa >= 30) {
			if (file_exists($lockdir)) { rmdir($lockdir); }
		}
	}

	$flag = 0;
	for($i=1; $i<=5; $i++) {
		if (!file_exists($lockdir)) {
			mkdir ($lockdir,0777);
			@chmod ($lockdir,0777);
			$flag = 1;
			break;
		}
		else {
			sleep(1);
		}
	}

}
?>
