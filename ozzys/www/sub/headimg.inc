<?PHP
//	ヘッド背景画 HEADIMG
function headimg($HEADIMG) {

	srand((double)microtime()*10000000);
	$hrn = array_rand($HEADIMG,1);
	$headimg = $HEADIMG[$hrn];

	return $headimg;

}
?>
