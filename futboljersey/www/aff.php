<?PHP
//	アフィリエイトCookie埋め込み転送プログラム

	//	サブルーチンフォルダー
	$SUB_DIR = "./sub";
	//	アフィリエイト関連フォルダー
	$AF_DIR = "./affiliate";

	include("$SUB_DIR/setup.inc");
	include("$SUB_DIR/array.inc");
	include ("../cone.inc");

	session_cache_limiter('nocache');
	session_start();

	$PHP_SELF = $_SERVER['PHP_SELF'];

	$LIST = explode("/",$PHP_SELF);
	unset($LIST[0]);
	unset($LIST[1]);

	//	アフィリエイター番号抜き出し
	//	商品・ページ番号抜きだし
	if ($LIST) {
		foreach ($LIST AS $VAL) {
			if (!$VAL) { continue; }
##			if (!$af_num && ereg("^i_",$VAL)) {
			if (!$af_num && preg_match( "/^i_/", $VAL )) {
##				$af_num = ereg_replace("[^0-9]","",$VAL);
				$af_num = preg_replace( "/[^0-9]/", "", $VAL );
			}
##			elseif (!$page_num && (ereg("^l",$VAL) || ereg("^s",$VAL) || !ereg("[^0-9]",$VAL) || ereg("^g",$VAL) || ereg("^a",$VAL))) {
			elseif (!$page_num && (preg_match("/^l/",$VAL) || preg_match("/^s/",$VAL) || !preg_match("/[^0-9]/",$VAL) || preg_match("/^g/",$VAL) || preg_match("/^a/",$VAL))) {
				$page_num = $VAL;
			}
		}
	}

##	if (ereg("^a",$page_num)) {
    if (preg_match("/^a/",$page_num)) {
##		$page_num = eregi_replace("^a","g",$page_num);
		$page_num = preg_replace("/^a/i","g",$page_num);
	}

	//	アフィリエイトユーザーチェック
	if ($af_num) {
		$sql  = "SELECT af_num FROM $afuser_table" .
				" WHERE af_num='$af_num' AND state='0' LIMIT 1;";
		if ($result = mysqli_query($conn_id,$sql)) {
			$list = mysqli_fetch_array($result);
			$af_num_ = $list['af_num'];
		}
		if (!$af_num_) { unset($af_num); }
	}

	$refere = $_SERVER['HTTP_REFERER'];
	$ip = getenv("REMOTE_ADDR");
	$host = gethostbyaddr($ip);
	if (!$host) { $host = $ip; }
##	if ($af_num && !ereg("^https://www.futboljersey.com/",$refere) && !ereg("^https://futboljersey.com/",$refere)) {
    if ($af_num && !preg_match("/^https:\/\/www.futboljersey.com\//",$refere) && !preg_match("/^https:\/\/futboljersey.com\//",$refere)) {
		setcookie("affid",$af_num,time() + $aff_time,"/",".futboljersey.com");
		$_SESSION['affid'] = $af_num;

		$sql  = "INSERT INTO $afrefere_table" .
				" (af_num,refere,click_time,ip,host)" .
				" VALUES('$af_num','$refere',now(),'$ip','$host');";
		$result = mysqli_query($conn_id,$sql);
	}

	mysqli_close($conn_id);

	if ($page_num) {
		$page_link = "/goods/$page_num/";
	}
	else {
		$page_link = "/";
	}

	header ("Location: $page_link\n\n");

	exit;

?>
