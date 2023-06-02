<?PHP
//	カテゴリー挿入タグリスト
function goods_9() {
// global	$PHP_SELF,$CATE_DIR,$main,$goods,$IMAGEF,$cate_table,$goods_table,$mode;
global	$IMAGEF,$PHP_SELF,$CATE_DIR,$goods_table,$cate_table;
$mode=$_POST['mode'];

	$IMAGEF = "../imagef";

	$cate_table = "category";
	$goods_table = "goods";

	if ($mode == "make") { make_pic(); }

	first_00();

}



function first_00() {
// global	$PHP_SELF,$CATE_DIR,$main,$goods,$msg,$ERROR;
global	$PHP_SELF,$CATE_DIR,$ERROR;
$main=$_POST['main'];
$goods=$_POST['goods'];
global $msg;// @source make_pic()

	if ($ERROR) { ERROR($ERROR); }

	echo <<<WAKABA
$msg
<FORM action="$PHP_SELF" method="POST">
<INPUT type="hidden" name="main" value="$main">
<INPUT type="hidden" name="goods" value="$goods">
<INPUT type="hidden" name="mode" value="make">
コピーした商品の画像を反映させます。<BR>
作成ボタンを押して下さい。<BR>
<INPUT type="submit" value="画像作成">
</FORM>

WAKABA;

}



function make_pic() {
// global $cate_table,$goods_table,$IMAGEF,$msg,$ERROR;
global $IMAGEF,$ERROR,$goods_table,$cate_table;
global $msg;

	include ("../../cone.inc");

	$sql =  "SELECT j.code, i.copy_num FROM $cate_table i,$goods_table j" .
			" WHERE i.g_num=j.g_num AND i.copy_num >= 1 ORDER BY j.code;";
	if (!$result = mysqli_query($conn_id,$sql)) {
		$ERROR[] = "複製登録商品情報を読み込みできなかったか、登録されておりません。";
	}
	else {
		$max = mysqli_num_rows($result);
		for($i=0; $i<$max; $i++) {
			list($code_,$copy_num_) = mysqli_fetch_array($result);
			$m_pic = "$IMAGEF/$code_.jpg";
			$t_pic = "$IMAGEF/$code_" . "-$copy_num_.jpg";
			//if (file_exists($m_pic)) {	//	del ookawara 2013/07/03
			if (file_exists($m_pic) && !file_exists($t_pic)) {	//	add ookawara 2013/07/03
				if (!copy($m_pic,$t_pic)) {
					$ERROR[] = "Code : $code_ の画像を複製できませんでした。";
				}
			//} else {	//	del ookawara 2013/07/03
			} elseif (!file_exists($m_pic)) {	//	add ookawara 2013/07/03
				$ERROR[] = "Code : $code_ の画像が登録されておりません。";
			}
		}
		if (!$ERROR) { $msg = "画像作成完了いたしました。"; }

	}

}
?>
