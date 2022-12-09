<?PHP
//	チェック
function check($KAGOS,$OPTIONS) {

	$customer = $_SESSION['customer'];
	$opt = $_SESSION['opt'];

	if (!$KAGOS && $customer) {
##		$KAGO = split("<>",$customer);
		$KAGO = explode("<>",$customer);

		unset($KAGOS);
		unset($CHECK);
		foreach ($KAGO AS $val) {
##			list($hinban_,$title_,$kakaku_,$num_) = split("::",$val);
			list($hinban_,$title_,$kakaku_,$num_) = explode("::",$val);
			if (!$title_) { continue; }
			else {
				$KAGOS[] = "$val";
				$CHECK[$hinban_] = $hinban_;
			}
		}

		if (!$KAGOS) { unset($_SESSION['customer']); }
	}

	if (!$OPTIONS && $opt) {
##		$OPTIONS_L = split("<>",$opt);
		$OPTIONS_L = explode("<>",$opt);

		unset($OPTIONS);
		$op_max_num = 0;
		foreach ($OPTIONS_L AS $val) {
##			list($op_num,$hinban_,$title_,$seban_l_,$seban_num_,$sename_l_,$sename_name_,$muneban_l_,$muneban_num_,$pant_l_,$pant_num_,$bach_l_) = split("::",$val);
			list($op_num,$hinban_,$title_,$seban_l_,$seban_num_,$sename_l_,$sename_name_,$muneban_l_,$muneban_num_,$pant_l_,$pant_num_,$bach_l_) = explode("::",$val);
			if ($op_max_num < $op_num) { $op_max_num = $op_num; }
			if (!$hinban_) { continue; }
			elseif ($hinban_ != "mochikomi" && !$CHECK[$hinban_]) { continue; }
			else { $OPTIONS[] = "$val"; }
		}

		if (!$OPTIONS) { unset($_SESSION['opt']); }
	}

	return array($KAGOS,$OPTIONS);

}


//	追加
function add($hinban,$title,$kakaku) {
global $conn_id;

	$customer = $_SESSION['customer'];

	unset($ERROR);
	if (!$hinban) { $ERROR[] = "商品番号が入力されておりません。"; }
	if (!$title) { $ERROR[] = "商品名が入力されておりません。"; }
	if (!$kakaku) { $ERROR[] = "商品価格が入力されておりません。"; }
	if ($title) {
##		$title = eregi_replace("\・","/",$title);
		$title = preg_replace("/\・/i","/",$title);
##		$title = eregi_replace("\･","/",$title);
		$title = preg_replace("/\･/i","/",$title);
	}

	//	ログ取得
	$ip = getenv("REMOTE_ADDR");
	$host = gethostbyaddr($ip);
	if (!$host) { $host = $ip; }

	$sql  = "INSERT INTO cart_access" .
			" (code,name,kakaku,ip,host,time)" .
			" VALUES('$hinban','$title','$kakaku','$ip','$host',NOW());";
	$sql1 = pg_exec($conn_id,$sql);

	unset($KAGOS);
	if (!$ERROR) {
##		$KAGO = split("<>",$customer);
		$KAGO = explode("<>",$customer);
		$flag = 0;
		foreach ($KAGO AS $val) {
##			list($hinban_,$title_,$kakaku_,$num_) = split("::",$val);
			list($hinban_,$title_,$kakaku_,$num_) = explode("::",$val);
			if ($title_ == "") { continue; }
			elseif ($hinban_ == $hinban) {
				++$num_;
				$KAGOS[] = "$hinban_::$title_::$kakaku_::$num_::";
				$flag = 1;
			}
			else { $KAGOS[] = "$val"; }
		}
		if ($flag == 0) { $KAGOS[] = "$hinban::$title::$kakaku::1::"; }
	}

	list($KAGOS,$OPTIONS) = check($KAGOS,$OPTIONS);

	return array($KAGOS,$OPTIONS,$ERROR);

}


//	変更
function hen($hinban) {
global $hinban,$num,$KAGOS,$ERROR/*2013/09/02yoshizawa 数量変更エラー時に$OPTIONSが作られないので変数追加→*/ , $OPTIONS;

	$customer = $_SESSION['customer'];

	$num = $_POST['num'];
#	$num = mb_convert_kana($num,n,"EUC-JP");
	$num = mb_convert_kana($num,n,"UTF-8");
##	if (ereg("[^0-9]",$num)) { $ERROR[] = "注文変更数が不正です。"; }	/* 2013/09/09 add y0oshizawa */
	if (preg_match("/[^0-9]/",$num)) { $ERROR[] = "注文変更数が不正です。"; }	/* 2013/09/09 add y0oshizawa */

	if (!$ERROR) {
		if ($num <= 0) {
			$KAGOS = del($hinban);
		}else {
			unset($KAGOS);
##			$KAGO = split("<>",$customer);
			$KAGO = explode("<>",$customer);
			$flag = 0;
			foreach ($KAGO AS $val) {
##				list($hinban_,$title_,$kakaku_,$num_) = split("::",$val);
				list($hinban_,$title_,$kakaku_,$num_) = explode("::",$val);
				if ($title_ == "") {
					continue;
				} elseif ($hinban_ == $hinban) {
					$KAGOS[] = "$hinban_::$title_::$kakaku_::$num::";
				} else {
					$KAGOS[] = "$val";
				}
			}
			list($KAGOS,$OPTIONS) = check($KAGOS,$OPTIONS);
		}
	}

	return array($KAGOS,$OPTIONS,$ERROR);

}


//	削除
function del($hinban) {

	unset($KAGOS);
	$customer = $_SESSION['customer'];
##	$KAGO = split("<>",$customer);
	$KAGO = explode("<>",$customer);

	foreach ($KAGO AS $val) {
##		list($hinban_,$title_,$kakaku_,$num_) = split("::",$val);
		list($hinban_,$title_,$kakaku_,$num_) = explode("::",$val);
		if ($hinban_ == $hinban || $title_ == "") { continue; }
		else { $KAGOS[] = "$val"; }
	}
	if (!$KAGOS) {
		unset($_SESSION['customer']);
		unset($_SESSION['addr']);
	}

	list($KAGOS,$OPTIONS) = check($KAGOS,$OPTIONS);

	return array($KAGOS,$OPTIONS);

}

//	削除マーキング
function del_op($op_num) {

	unset($OPTIONS);
	$opt = $_SESSION['opt'];
##	$OPTIONS_L = split("<>",$opt);
	$OPTIONS_L = explode("<>",$opt);

	foreach ($OPTIONS_L AS $val) {
##		list($op_num_,$hinban_) = split("::",$val);
		list($op_num_,$hinban_) = explode("::",$val);
		if ($op_num == $op_num_ || $hinban_ == "") { continue; }
		else { $OPTIONS[] = "$val"; }
	}
	if (!$OPTIONS) { unset($_SESSION['opt']); }

	list($KAGOS,$OPTIONS) = check($KAGOS,$OPTIONS);

	return array($KAGOS,$OPTIONS);

}

?>
