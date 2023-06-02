<?PHP
//	画像読み込み表示プログラム

	//	サブルーチンフォルダー
	$SUB_DIR = "./sub";

	//	商品画像フォルダー
	//	1枚目
	$G_IMG_F_DIR = "./imagef";
	//	2枚目
	$G_IMG_B_DIR = "./imageb";

	//	ノーマル画像フォルダー
	$N_IMG_DIR = "./image";

	include("../cone.inc");
	include("$SUB_DIR/setup.inc");
	include("$SUB_DIR/array.inc");

	$PHP_SELF = $_SERVER['PHP_SELF'];

	$LIST = explode("/",$PHP_SELF);
	unset($LIST[0]);
	unset($LIST[1]);

	//	商品番号抜きだし
	if ($LIST) {
		foreach ($LIST AS $VAL) {
			if (!$VAL) { continue; }
##			if (!$item_num && ereg("^g",$VAL)) {
			if (!$item_num && preg_match("/^g/",$VAL)) {
##				$item_num = eregi_replace("[^0-9]","",$VAL);
				$item_num = preg_replace("/[^0-9]/i","",$VAL);
##			} elseif (!$goods_num && ereg("^a",$VAL)) {
			} elseif (!$goods_num && preg_match("/^a/",$VAL)) {
##				$goods_num = eregi_replace("[^0-9]","",$VAL);
				$goods_num = preg_replace("/[^0-9]/i","",$VAL);
			} elseif (!$width && !$height) {
##				if (ereg("^w",$VAL)) {
				if (preg_match("/^w/",$VAL)) {
##					$width = eregi_replace("[^0-9]","",$VAL);
					$width = preg_replace("/[^0-9]/","",$VAL);
				}
##				elseif (ereg("^h",$VAL)) {
			    elseif (preg_match("/^h/",$VAL)) {
##					$height = eregi_replace("[^0-9]","",$VAL);
  					$height = preg_replace("/[^0-9]/i","",$VAL);
				}
##			} elseif (ereg("^b",$VAL)) {
			} elseif (preg_match("/^b/",$VAL)) {
				$type = 1;
			}
		}
	}

	//	商品画像ファイル名取得
	if ($goods_num > 0) {
			$sql  = "SELECT code FROM $goods_table" .
					" WHERE g_num='$goods_num' LIMIT 1";
			if ($result = mysqli_query($conn_id,$sql)) {
				$list = mysqli_fetch_array($result);
				$code = $list['code'];
			}
	} else {
		if ($item_num > 0) {
			$sql  = "SELECT j.code FROM $cate_table i,$goods_table j" .
					" WHERE i.g_num=j.g_num AND i.num='$item_num' LIMIT 1";
			if ($result = mysqli_query($conn_id,$sql)) {
				$list = mysqli_fetch_array($result);
				$code = $list['code'];
			}
		}
	}

	//	商品画像、表？裏存在チェック
	if ($code) {
		if ($type != 1) { 
			$img_file = "$G_IMG_F_DIR/$code.jpg";
		}
		else {
			$img_file = "$G_IMG_B_DIR/$code.jpg";
		}
		$type = 2;
		if (!file_exists($img_file)) { unset($img_file); }
	}

	if (!$img_file) {
		$img_file = "$N_IMG_DIR/futobol_ba.gif";
		$p_type = 1;
		$type = 3;
		header("content-type: image/gif");
	}
	else {
		header("content-type: image/jpeg");
	}

	//	画像情報読み込み
	$flag = 0;
	if ($type != 3 && ($width || $height)) {
		list($p_width,$p_height,$p_type) = getimagesize($img_file);
		if (($width >= 100 && $width < $p_width) || ($height >= 100 && $height < $p_height)) {
			$flag = 1;
			//	サムネイルサイズ
			if ($width) {
				$ritu = $width / $p_width;
				$height = floor($p_height * $ritu + 0.5);
			}
			else {
				$ritu = $height / $p_height;
				$width = floor($p_width * $ritu + 0.5);
			}
		}
	}

	//	画像リサイズ
	if ($flag == 1) {
		$src = ImageCreateFromJPEG($img_file);
		$dst = ImageCreateTrueColor($width,$height);
		ImageCopyResized($dst, $src, 0, 0, 0, 0, $width, $height, $p_width, $p_height);
		ImageJPEG($dst);
		ImageDestroy($dst);
	}
	else {
		readfile($img_file);
	}

	if ($conn_id) { mysqli_close($conn_id); }

	exit;

?>
