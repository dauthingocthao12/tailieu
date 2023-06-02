<?PHP
/*

	ネイバーズスポーツ	商品表示システム

*/

	//	サブルーチンフォルダー
	define("SUB_DIR", "./sub");

	//	新ルーチンフォルダー
	define("INCLUDE_DIR", "./include/");

	//	テンプレートフォルダー
	define("TEMPLATE_DIR", "./template/");

	include_once("../cone.inc");
	include_once(SUB_DIR."/setup.inc");
	include_once(SUB_DIR."/array.inc");

	include_once(INCLUDE_DIR."common.php");
	include_once(INCLUDE_DIR."logincheck.php");
	include_once(INCLUDE_DIR."pankuzu_list.php");
	include_once(INCLUDE_DIR."goods_disp_list.php");
	include_once(INCLUDE_DIR."head.php");
	include_once(INCLUDE_DIR."navi.php");
	include_once(INCLUDE_DIR."aff_url.php");
	include_once(INCLUDE_DIR."ossm.php");
	include_once(INCLUDE_DIR."foot.php");


	session_start();

	//	ログインチェック
	login_check();

	$PHP_SELF = $_SERVER['PHP_SELF'];

	//	ログアウトチェック
	if (preg_match("/out$/", $PHP_SELF)) {
		$PHP_SELF = preg_replace("/\/out$/" ,"", $PHP_SELF);
		$PHP_SELF = preg_replace("/out$/" ,"", $PHP_SELF);
		unset($_SESSION['idpass']);
		unset($_COOKIE['idpass']);
		setcookie("idpass");
	}

	//	スラッシュチェック
	if (!preg_match("/\.html$/", $PHP_SELF) && !preg_match("/\.htm$/", $PHP_SELF) && !preg_match("/\/$/", $PHP_SELF)) {
		$sent_url = $PHP_SELF."/";
		header ("Location: $sent_url\n\n");
		exit;
	}

	//	階層データー読込
	$LIST = explode("/", $PHP_SELF);
	unset($LIST[0]);
	unset($LIST[1]);
	unset($HLIST);
	$i = 1;
	foreach ($LIST AS $VAL) {
		if ($i > 5) { break; }
		$VAL = trim($VAL);
		if (!$VAL) {
			continue;
		} else {
			$strlen = strlen($VAL);
			if (preg_match("/\.htm/", $VAL)) {
				$NLIST[$i] = $VAL;
				break;
			} elseif (preg_match("/^s/", $VAL)) {
				$VAL = preg_replace("/[^0-9]/", "", $VAL);
				$NLIST[1] = substr($VAL, 0, 2);
				$NLIST[2] = "s".substr($VAL, 2, 2);
				$indexs = (int)substr($VAL, 4);
				if ($indexs > 1) {
					$NLIST[3] = "index".$indexs.".htm";
				}
				break;
			} elseif (preg_match("/^l/", $VAL)) {
				$VAL = preg_replace("/[^0-9]/", "", $VAL);
				$LIST[$i] = $VAL;
				$NLIST[1] = substr($VAL, 0, 2);
				$NLIST[2] = "s".substr($VAL, 2, 2);
				$NLIST[3] = "l".substr($VAL, 4, 4);
				$indexs = (int)substr($VAL, 6);
				if ($indexs > 1) {
					$NLIST[4] = "index".$indexs.".htm";
				}
				break;
			} elseif (preg_match("/^g/", $VAL)) {
				$VAL = preg_replace("/[^0-9]/", "", $VAL);
				$sql  = "SELECT cate1, cate2, cate3 FROM ".$cate_table.
						" WHERE num='".$VAL."'".
						" LIMIT 1;";
				if ($result = mysqli_query($conn_id, $sql)) {
					$list = mysqli_fetch_array($result);
					$NLIST[1] = $list['cate1'];
					$NLIST[2] = "s".$list['cate2'];
					$NLIST[3] = "l".$list['cate3'];
				}
				$NLIST[4] = "g".$VAL;
				break;
			} elseif ($i == 1) {
				$NLIST[$i] = $VAL;
			} elseif ($i == 2) {
				$NLIST[$i] = "s".$VAL;
			} elseif ($i == 3) {
				$NLIST[$i] = "l".$VAL;
			} elseif ($i == 4) {
				$NLIST[$i] = "g".$VAL;
			}
		}
		$i++;
	}
	$LIST = $NLIST;

	$VALUE = array();
	$i=1;
	$CHECK = array();
	if ($LIST) {
		foreach ($LIST AS $VAL) {
			if ($VAL != "") {
				if (preg_match("/\.htm/", $VAL)) {
					$VALUE[$i] = preg_replace("/[^0-9]/", "", $VAL);
					$CHECK['p'] = preg_replace("/[^0-9]/", "", $VAL);
					break;
				} elseif (preg_match("/^g/", $VAL)) {
					if ($CHECK['g']) {
						continue;
					} else {
						$CHECK['g'] = $VAL;
						$VALUE[$i] = preg_replace("/[^0-9]/", "", $VAL);
					}
				} elseif (preg_match("/^l/", $VAL)) {
					if ($CHECK['l'] || $CHECK['g'] || $i > 4) {
						continue;
					} else {
						$CHECK['l'] = $VAL;
						$VALUE[$i] = preg_replace("/[^0-9]/", "", $VAL);
					}
##				} elseif (eregi("^s",$VAL)) {
				} elseif (preg_match("/^s/i",$VAL)) {
					if ($CHECK['s'] || $CHECK['l'] || $CHECK['g'] || $i > 3) {
						continue;
					} else {
						$CHECK['s'] = $VAL;
						$VALUE[$i] = preg_replace("/[^0-9]/", "", $VAL);
					}
##				} elseif (!eregi("[^0-9]",$VAL)) {
				} elseif (!preg_match("/[^0-9]/i",$VAL)) {
					if ($CHECK['main'] || $CHECK['s'] || $CHECK['l'] || $CHECK['g'] || $i > 2) {
						continue;
					} else {
						$CHECK['main'] = $VAL;
						$VALUE[$i] = preg_replace("/[^0-9]/", "", $VAL);
					}
				} else {
					break;
				}
				$i++;
			}
		}
	}

	$main = "";
	$vc = array();
	//	パンくずリスト
	if ($CHECK) {
		list($title, $link) = read_pankuzu_list($CHECK);
	}
//pre($CHECK);
	if ($CHECK['g']) {	//	商品詳細
		include_once(INCLUDE_DIR."goods_syousai.php");
		//$main_contents  = goods_syousai($VALUE, $CHECK);						//	del ookawara 2016/12/02
		list($main_contents, $description) = goods_syousai($VALUE, $CHECK);		//	add ookawara 2016/12/02
	} elseif ($CHECK['l']) {	//	商品一覧
		include_once(INCLUDE_DIR."goods_list.php");
		$main_contents  = goods_list($VALUE, $CHECK);
	} elseif ($CHECK['s']) {	//	メーカー名一覧＆商品名一覧
		include_once(INCLUDE_DIR."goods_sublist2.php");
		$main_contents  = goods_sublist2($VALUE, $CHECK);
		$main_contents .= goods_disp_list($VALUE, $CHECK, $VC);
	} elseif ($CHECK['main']) {	//	サブカテゴリー一覧
		include_once(INCLUDE_DIR."goods_sublist.php");
		$main_contents  = goods_sublist($VALUE, $CHECK);
		$main_contents .= goods_disp_list($VALUE, $CHECK, $VC);
	} elseif ($CHECK['sample']) {	//	メインカテゴリー一覧サンプル
		$value = $VALUE[1];
		//	メインカテゴリー一覧
		include_once(INCLUDE_DIR."goods_mainlist2.php");
		$main_contents  = goods_mainlist2($VALUE, $CHECK, $VC);
		$title = "カテゴリー 一覧";
	} else {
		$value = $VALUE[1];
		//	メインカテゴリー一覧
##		if (ereg("sample",$PHP_SELF)) {
		if (preg_match("/sample/",$PHP_SELF)) {
			include_once(INCLUDE_DIR."goods_mainlist2.php");
			$main_contents  = goods_mainlist2($VALUE, $CHECK);
		} else {
			include_once(INCLUDE_DIR."goods_mainlist.php");
			$main_contents  = goods_mainlist($VALUE, $CHECK);
		}
		$title = "カテゴリー 一覧";
//echo("5<BR>\n");
	}

	//	ヘッダ
	$head = read_head();

	//	ナビ
	$navi = read_navi();

	//	アフィリエイトURL表示
	$aff_url = aff_url($CHECK);

	//	お勧め商品
	$ossm = read_ossm();

	//	フッタ
	$foot = read_foot();

	//	add ookawara 2016/12/02
	//	description
	$description .= HEADDESCRIPTION;

	$INPUTS = array();
	$DEL_INPUTS = array();

	$INPUTS['TITLE'] = $title;					//	ページタイトル
	$INPUTS['KEYWORDS'] = $title;				//	ページキーワード
	$INPUTS['DESCRIPTION'] = $description;		//	Description
	$INPUTS['HEAD'] = $head;					//	ヘッダ
	$INPUTS['NAVI'] = $navi;					//	ナビ
	$INPUTS['PANKUZULIST'] = $link;				//	パンくずリスト
	$INPUTS['MAINCONTENTS'] = $main_contents;	//	コンテンツ
	$INPUTS['AFFURL'] = $aff_url;				//	アフェリエイトURL表示
	$INPUTS['OSSM'] = $ossm;					//	お勧め商品
	$INPUTS['FOOT'] = $foot;					//	フッタ

	//	html作成・置換
	$make_html = new read_html();
	$make_html->set_dir(TEMPLATE_DIR);
	$make_html->set_file("default.htm");
	$make_html->set_rep_cmd($INPUTS);
	$make_html->set_del_cmd($DEL_INPUTS);
	$html = $make_html->replace();

	echo $html;

	exit;
?>