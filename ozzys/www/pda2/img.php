<?PHP
/*

	ozzys様
	PDA商品管理システム
	画像表示プログラム

*/

	//	設定ファイル読込
	define(SET_DIR,"./item_sub/");
	include_once(SET_DIR."main_lib.php");
	include_once(SET_DIR."main_array.php");

	//	値取得
	define("TYPE",$_GET['t']);			//	画像タイプ（c:カテゴリー、i:作品）
	define("FILENAME",$_GET['f']);		//	ファイル名
	define("MW",$_GET['w']);			//	最大画像サイズ（横幅）
	define("MH",$_GET['h']);			//	最大画像サイズ（高さ）

	//	ファイル名
	$dir = "";
	switch (TYPE) {
		case "i";	//	作品
			$dir = ".".DIR_IMG_ITEM;
			break;
		case "c";	//	カテゴリー
			$dir = ".".DIR_IMG_CATEGORY;
			break;
		default;	//	全カテゴリー
			$dir = ".".DIR_IMG_ITEM;
			break;
	}

	$img_file = "";
	if (FILENAME && file_exists($dir.FILENAME)) {
		$img_file = $dir.FILENAME;
	} else {
		$img_file = ".".DIR_IMG_ITEM."NoImage.gif";
	}

	//	サイズ決定
	$flag = 0;
	if (file_exists($img_file)) {
		$list = getimagesize($img_file);
		switch ($list[2]){
			case 1 :
				$img_in = ImageCreateFromGIF($img_file);
				break;
			case 2 :
				$img_in = ImageCreateFromJPEG($img_file);
				break;
			case 3 :
				$img_in = ImageCreateFromPNG($img_file);
				break;
		}

		$wid = $list[0];
		$hig = $list[1];
		if ((MW && $wid > MW) || (MH && $hig > MH)) {
			$w_ritu = 1;
			$h_ritu = 1;
			if (MW) {
				$w_ritu = MW / $wid;
			}
			if (MH) {
				$h_ritu = MH / $hig;
			}
			if ($w_ritu < $h_ritu) {
				$ritu = $w_ritu;
			} else {
				$ritu = $h_ritu;
			}
			$width = $wid * $ritu;
			$height = $hig * $ritu;
			$flag = 1;
		} else {
			$width = $wid;
			$height = $hig;
		}
	}

	//	縮小
	if ($list[2] == 1) {
		$img_out = ImageCreate($width,$height);
		$trans = imagecolorallocate($img_out,0,0,0);
		imagefill($img_out,0,0,$trans);
		imagecolortransparent($img_out,$trans);
	} else {
		$img_out = ImageCreateTrueColor($width,$height);
	}
	imagecopyresized($img_out,$img_in,0,0,0,0,$width,$height,$wid,$hig);

	switch ($list[2]){
		case 1 :
			header("content-type: image/gif");
			ImageGIF($img_out);
			break;
		case 2 :
			header("content-type: image/jpeg");
			ImageJPEG($img_out);
			break;
		case 3 :
			header("Content-Type: image/png");
			ImagePNG($img_out);
			break;
	}

	//	メモリー解放
	@imagedestroy($img_in);
	@imagedestroy($img_out);

	exit;

?>